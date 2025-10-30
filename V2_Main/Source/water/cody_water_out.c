#include "cody_water_out.h"
#include "cody_kit.h"
#include "valve.h"
#include "flow_meter.h"
#include "water_out.h"
#include "water_out_common.h"
#include "error.h"
#include "sound.h"


#define DEFAULT_WAIT_TIME   20  // 200ms... @10ms..

#define ROOM_OUT_TIME      3000UL   // 30sec
#define COLD_OUT_TIME      12000UL  // 4min
#define COLD_RINS_TIME     12000UL  // 4min
#define ROOM_RINSE_TIME    3000UL   // 30sec
WaterOut_T   CodyOut;



void TurnOnCodyOut(WaterOutType_T mType)
{
    CodyOut.Out = TRUE;
    CodyOut.Type = mType;

    // ?��?��?�� ?��?���? ?��?�� ?��간으�? 처리
    CodyOut.TargetAmount =  ROOM_OUT_TIME + COLD_OUT_TIME + ROOM_RINSE_TIME;
}

void TurnOffCodyOut(void)
{
    CodyOut.Out = FALSE;
}


static U8 IsChangedOutStatus(void)
{
    if( CodyOut.Out != CodyOut.PrevOut )
    {
        CodyOut.PrevOut = CodyOut.Out;

        return TRUE;
    }
    
    return FALSE;
}

static void RegisterOutageCodyOut(void)
{
    CodyOut.CurrentAmount = 0UL;

    // Start Check Outage...
    StartCheckOutage( OUTAGE_DELAY_TIME, OUTAGE_CHECK_TIME_10SEC, TYPE_WATER );
}

static void UnRegisterOutageCodyOut(void)
{
    // Update User Usage
    CodyOut.CurrentAmount = 0UL;

    // UnRegister...
    RegisterFlowMeterId( FLOW_ID_WATER, NULL );

    // Stop Check Outage
    StopCheckOutage();
}

enum
{
    OUT_INIT_OUTAGE,
    OUT_STER_ROOM_WATER,          // ?���? ?��?�� 
    OUT_STER_COLD_WATER,          // ?���? + ?���? ?��?�� 
    OUT_STER_COLD_WATER_2,         // ?���? + ?���? ?��?�� 
    OUT_STER_RINSE_COLD_WATER_3,  // ?���? + ?���? ?��?�� 
    OUT_RINSE_ROOM_WATER          // ?���? ?��?�� 
};

// 추출 ?��?��
static void OpenValveCodyOut(void)
{

    switch( CodyOut.OpenStep )
    {
        case OUT_INIT_OUTAGE:

            RegisterOutageCodyOut();

            SetCodyKitStep( CODY_STEP_ROOM );

            CodyOut.OpenStep++;
            break;

        case OUT_STER_ROOM_WATER:
            OpenValve( VALVE_ROOM_COLD_OUT );

            if( ++CodyOut.CurrentAmount >= ROOM_OUT_TIME )
            {
                CloseValve( VALVE_ROOM_COLD_OUT );
                CodyOut.OpenStep++;
            }

            // ?��?�� 0.5�? ?�� ?��?�� 출력
            if( CodyOut.CurrentAmount == 150 )
            {
                Sound( SOUND_STER_START );
            }
            break;

        case OUT_STER_COLD_WATER:
            SetCodyKitStep( CODY_STEP_COLD );

            CloseValve( VALVE_COLD_AIR );
            OpenValve( VALVE_COLD_IN );

            CodyOut.WaitTime = WAIT_100MS;
            CodyOut.OpenStep++;
            break;

        case OUT_STER_COLD_WATER_2:
            if( CodyOut.WaitTime != 0 )
            {
                CodyOut.WaitTime--;
            }
            else
            {
                OpenValve( VALVE_ROOM_COLD_OUT);
                if( ++CodyOut.CurrentAmount >= (ROOM_OUT_TIME + COLD_OUT_TIME + COLD_RINS_TIME ) )
                {
                    CloseValve( VALVE_ROOM_COLD_OUT );
                    OpenValve( VALVE_COLD_AIR );

                    CodyOut.WaitTime = WAIT_100MS;
                    CodyOut.OpenStep++;
                }

                // ?���? ?��?��
                if( CodyOut.CurrentAmount == ( ROOM_OUT_TIME + COLD_OUT_TIME ) )
                {
                    Sound( SOUND_STER_RINSE );
                }

            }
            break;

        case OUT_STER_RINSE_COLD_WATER_3:
            if( CodyOut.WaitTime != 0 )
            {
                CodyOut.WaitTime--;
            }
            else
            {
                SetCodyKitStep( CODY_STEP_ROOM );

                OpenValve( VALVE_ROOM_COLD_OUT );
                CodyOut.OpenStep++;
                break;
            }
            break;

        case OUT_RINSE_ROOM_WATER:
            ++CodyOut.CurrentAmount;
            OpenValve( VALVE_ROOM_COLD_OUT );
            break;

        default:
            break;
    }
}

enum
{
    STOP_INIT_OUTAGE,
    STOP_CLOSE_WATER_OUT,
};
// 추출 종료
static void CloseValveCodyOut(void)
{
    switch( CodyOut.CloseStep )
    {
        case STOP_INIT_OUTAGE:


            CloseValve( VALVE_ROOM_COLD_OUT );

            CloseValve( VALVE_ROOM_COLD_OUT );
            CloseValve( VALVE_COLD_IN );
            OpenValve( VALVE_COLD_AIR );

            UnRegisterOutageCodyOut();
            CodyOut.CloseStep++;
            break;

        case STOP_CLOSE_WATER_OUT:
            // do noting....

            break;

        default:
            break;
    }
}



// 추출?�� 반환 ( percent )
U8 GetCodyOutPercent(void)
{
    return CalcPercent( CodyOut.TargetAmount, CodyOut.CurrentAmount );
}


// 추출?�� ml ?��?�� 반환
U16 GetCodyOutAmount(void)
{
    return CalcLiter( CodyOut.CurrentAmount );
}

// 추출 종료 ?��?��
static U8 IsDoneCodyOut(void)
{
    // 목표 추출?�� 만큼�? 추출?���? 종료?��?��.
    if( CodyOut.TargetAmount <= CodyOut.CurrentAmount )
    {
        Sound( SOUND_STER_DONE );
        return TRUE;
    }

    // ?��?��, ?��?�� 조건?���? 종료?��?��.
    if( IsErrorType( ERR_TYPE_ROOM_WATER_OUT ) == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

static void Outflow(U8 mu8Out)
{
    if( mu8Out == TRUE )
    {
        CodyOut.CloseStep = 0;
        OpenValveCodyOut();
    }
    else
    {
        CodyOut.OpenStep = 0;
        CloseValveCodyOut();
    }
}

// ?��?�� 추출 ?��?��
U8 ControlWaterCodyOut(void)
{
    if( IsChangedOutStatus() == TRUE )
    {
        if( CodyOut.Out == TRUE )
        {
            // Do nothing..
            SetCodyKitStatus( TRUE );
        }
        else
        {
            // CODY KIT ?��?��
            SetCodyKitStatus( FALSE );
            SetWaterOutSelect( SEL_WATER_ROOM );
        }
    }

    if( CodyOut.Out == TRUE )
    {
        if( IsDoneCodyOut() == TRUE )
        {
            TurnOffCodyOut();
            return WATER_OUT_DONE;
        }
    }

    Outflow(CodyOut.Out);
    return WATER_OUT_GOING;
}
