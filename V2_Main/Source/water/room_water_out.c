#include "room_water_out.h"
#include "valve.h"
#include "user.h"
#include "flow_meter.h"
#include "water_out_common.h"
#include "hot_water_out.h"
#include "error.h"
#include "sound.h"


#define DEFAULT_WAIT_TIME   20  // 200ms... @10ms..
// Tankless

WaterOut_T   RoomOut;


static void UpdateUserUsage(U32 mu32Amount)
{
    U32 mu32TargetAmount;

    mu32TargetAmount = GetFindOutTargetTime( OUT_AMOUNT_10 );
    if( mu32TargetAmount != 0 )
    {
        mu32Amount = mu32Amount / mu32TargetAmount;
        mu32Amount *= 10;
    }
    else
    {
        mu32Amount = 0;
    }

    AddUserUsageWater( mu32Amount );
}

void TurnOnRoomOut(WaterOutType_T mType)
{
    RoomOut.Out = TRUE;
    RoomOut.Type = mType;
    RoomOut.TargetAmount = GetFindOutTargetTime( RoomOut.Type );
}

void TurnOffRoomOut(void)
{
    RoomOut.Out = FALSE;
}


static U8 IsChangedOutStatus(void)
{
    if( RoomOut.Out != RoomOut.PrevOut )
    {
        RoomOut.PrevOut = RoomOut.Out;

        return TRUE;
    }
    
    return FALSE;
}

static void UpdateRoomCurrentAmount(void)
{
    RoomOut.CurrentAmount++;
}

static void RegisterOutageRoomOut(void)
{
    RoomOut.CurrentAmount = 0UL;

    // Register Function call - ����??? ��??? ??????
    RegisterFlowMeterId( FLOW_ID_WATER, UpdateRoomCurrentAmount );

    // Start Check Outage...
    StartCheckOutage( OUTAGE_DELAY_TIME, OUTAGE_CHECK_TIME_10SEC, TYPE_WATER );
}

static void UnRegisterOutageRoomOut(void)
{
    // Update User Usage
    UpdateUserUsage( RoomOut.CurrentAmount );
    RoomOut.CurrentAmount = 0UL;

    // UnRegister...
    RegisterFlowMeterId( FLOW_ID_WATER, NULL );

    // Stop Check Outage
    StopCheckOutage();
}

enum
{
    OUT_INIT_OUTAGE,            // �������� ����
    OUT_OPEN_WATER_OUT,         // ������������ OPEN
    IN_OPEN_WATER_OUT,          // �����Լ���� OPEN
};

// ���� ??????
static void OpenValveRoomOut(void)
{
    switch( RoomOut.OpenStep )
    {
        case OUT_INIT_OUTAGE:
            RegisterOutageRoomOut();

            RoomOut.WaitTime = 0;
            RoomOut.OpenStep++;
            break;

        case OUT_OPEN_WATER_OUT:
            if( RoomOut.WaitTime != 0 )
            {
                RoomOut.WaitTime--;
            }
            else
            {
                OpenValve( VALVE_ROOM_COLD_OUT );

                RoomOut.OpenStep++;
                RoomOut.WaitTime = 0;
            }
            break;

        case IN_OPEN_WATER_OUT:
            if( RoomOut.WaitTime != 0 )
            {
                RoomOut.WaitTime--;
            }
            else
            {
                OpenValve( VALVE_ROOM_IN );
            }
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
// ���� ����
static void CloseValveRoomOut(void)
{
    switch( RoomOut.CloseStep )
    {
        case STOP_INIT_OUTAGE:
            CloseValve( VALVE_ROOM_COLD_OUT );

            if( RoomOut.CurrentAmount > AMOUNT_50 )
            {
                UpdateHotOutLPM( GetFlowMeterLPM() );
            }

            UnRegisterOutageRoomOut();
            RoomOut.CloseStep++;
            break;

        case STOP_CLOSE_WATER_OUT:
            // do noting....
           CloseValve( VALVE_ROOM_IN );
            break;

        default:
            break;
    }
}



// ����??? ��ȯ ( percent )
U8 GetRoomOutPercent(void)
{
    return CalcPercent( RoomOut.TargetAmount, RoomOut.CurrentAmount );
}


// ����??? ml ?????? ��ȯ
U16 GetRoomOutAmount(void)
{
    return CalcLiter( RoomOut.CurrentAmount );
}

// ���� ���� ??????
static U8 IsDoneRoomOut(void)
{
    if( IsErrorType( ERR_TYPE_ROOM_WATER_OUT ) == TRUE )
    {
        return TRUE;
    }

    // ��ǥ ����??? ��ŭ?? ����????? ����??????.
    if( RoomOut.TargetAmount <= RoomOut.CurrentAmount )
    {
        return TRUE;
    }

    return FALSE;
}

static void Outflow(U8 mu8Out)
{
    if( mu8Out == TRUE )
    {
        RoomOut.CloseStep = 0;
        OpenValveRoomOut();
    }
    else
    {
        RoomOut.OpenStep = 0;
        CloseValveRoomOut();
    }
}

// ?????? ���� ??????
U8 ControlWaterRoomOut(void)
{
    if( IsChangedOutStatus() == TRUE )
    {
        if( RoomOut.Out == TRUE )
        {
            // Do nothing..
        }
        else
        {
            // Do nothing..
        }
    }

    if( RoomOut.Out == TRUE )
    {
        if( IsDoneRoomOut() == TRUE )
        {
            TurnOffRoomOut();
            Sound(SOUND_EFFLUENT_END);
            return WATER_OUT_DONE;
        }
    }

    Outflow(RoomOut.Out);
    return WATER_OUT_GOING;
}
