#include "cold_water_out.h"
#include "valve.h"
#include "user.h"
#include "flow_meter.h"
#include "water_out_common.h"
#include "drain_water.h"
#include "hot_water_out.h"
#include "error.h"
#include "sound.h"


WaterOut_T   ColdOut;


// 10ml ?ã®?úÑÎ°? ????û•
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

    AddUserUsageWater(  mu32Amount );
}

void TurnOnColdOut(WaterOutType_T mType)
{
    ColdOut.Out = TRUE;
    ColdOut.Type = mType;
    ColdOut.TargetAmount    = GetFindOutTargetTime( ColdOut.Type );
}

void TurnOffColdOut(void)
{
    ColdOut.Out = FALSE;
}


static U8 IsChangedOutStatus(void)
{
    if( ColdOut.Out != ColdOut.PrevOut )
    {
        ColdOut.PrevOut = ColdOut.Out;

        return TRUE;
    }
    
    return FALSE;
}

static void UpdateColdCurrentAmount(void)
{
    // PaddingAmountÍ∞? 0?ù¥ ?ê†?ïåÍπåÏ???äî CurrentAmountÎ•? Ï¶ùÍ???ãú?Ç§Ïß? ?ïä?äî?ã§...
    // ?Éâ?àò ?É±?Å¨ ?óê?ñ¥Ï∏µÏúºÎ°? ?ù∏?ï¥?Ñú ?ú†?üâ?Ñº?ÑúÎ°? ?†ï?ôï?ïú Ï∂îÏ∂ú?üâ?ù¥ Í≥ÑÏÇ∞?êòÏß? ?ïä?äî?ã§.
    if( ColdOut.PaddingAmount == 0 )
    {
        ColdOut.CurrentAmount++;
    }
    else
    {
        ColdOut.PaddingAmount--;
    }
}
static void RegisterOutageColdOut(void)
{
    ColdOut.CurrentAmount = 0UL;

    // Register Function call - Ï∂îÏ∂ú?üâ Ï¶ùÍ?? ?ï®?àò
    RegisterFlowMeterId( FLOW_ID_WATER, UpdateColdCurrentAmount );

    // Start Outage...
    StartCheckOutage( OUTAGE_DELAY_TIME, OUTAGE_CHECK_TIME_10SEC, TYPE_WATER );
}

static void UnRegisterOutageColdOut(void)
{
    // DrainWater Water Consume
    UpdateDrainColdWaterConsume(  ColdOut.CurrentAmount );

    // Update User Usage
    UpdateUserUsage( ColdOut.CurrentAmount );
    ColdOut.CurrentAmount = 0UL;

    // UnRegister...
    RegisterFlowMeterId( FLOW_ID_WATER, NULL );

    StopCheckOutage();
}

// WATER_OUT_STEP
enum
{
    OUT_CLOSE_AIR,
    OUT_OPEN_WATER_IN,
    OUT_INIT_OUTAGE,
    OUT_OPEN_WATER_OUT,
};


// ?ûÖ?àò Î∞∏Î∏å Î®ºÏ?? open
static void OpenValveColdOut(void)
{
    switch( ColdOut.OpenStep )
    {
        case 0:
            CloseValve( VALVE_COLD_AIR );
            OpenValve( VALVE_COLD_IN );
            ColdOut.PaddingAmount = AMOUNT_10;

            ColdOut.WaitTime = WAIT_100MS;
            ColdOut.OpenStep++;
            break;

        case 1:
            if( ColdOut.WaitTime != 0 )
            {
                ColdOut.WaitTime--;
            }
            else
            {
                RegisterOutageColdOut();
                OpenValve( VALVE_ROOM_COLD_OUT);
                ColdOut.OpenStep++;
            }
            break;

        case 2:
            OpenValve( VALVE_COLD_IN );

            OpenValve( VALVE_ROOM_COLD_OUT);
            CloseValve( VALVE_COLD_AIR );
            break;

        default:
            break;
    }
}

enum
{
    STOP_CLOSE_WATER_OUT,
    STOP_CLOSE_WATER_IN,
    STOP_OPEN_AIR,
};

// Ï∂îÏ∂ú Ï§ëÏ??
static void CloseValveColdOut(void)
{
    switch( ColdOut.CloseStep )
    {
        case 0:
            CloseValve( VALVE_ROOM_COLD_OUT );
            OpenValve( VALVE_COLD_AIR );

            if( ColdOut.CurrentAmount > AMOUNT_50 )
            {
                UpdateHotOutLPM( GetFlowMeterLPM() );
            }

            UnRegisterOutageColdOut();

            ColdOut.WaitTime = WAIT_100MS;
            ColdOut.CloseStep++;
            break;

        case 1:
            if( ColdOut.WaitTime != 0 )
            {
                ColdOut.WaitTime--;
            }
            else
            {
                OpenValve( VALVE_COLD_AIR );

                CloseValve( VALVE_COLD_IN );
                CloseValve( VALVE_ROOM_COLD_OUT );
                ColdOut.CloseStep++;
            }
            break;

        case 2:
            // do noting....
            break;

        default:
            break;
    }
}

U8 GetColdOutPercent(void)
{
    return CalcPercent( ColdOut.TargetAmount, ColdOut.CurrentAmount );
}

// Ï∂îÏ∂ú ÏßÑÌñâ?üâ( 0~ 999 mL )
U16 GetColdOutAmount(void)
{
    return CalcLiter( ColdOut.CurrentAmount );
}


// Ï∂îÏ∂ú Ï¢ÖÎ£å ?ôï?ù∏ 
static U8 IsDoneColdOut(void)
{
    // ?ã®?àò Ï°∞Í±¥?ù¥Î©? Ï¢ÖÎ£å?ïú?ã§.
    if( IsErrorType( ERR_TYPE_COLD_WATER_OUT ) == TRUE )
    {
        return TRUE;
    }

    // Î™©Ìëú Ï∂îÏ∂ú?üâ ÎßåÌÅºÎß? Ï∂îÏ∂ú?ïòÍ≥? Ï¢ÖÎ£å?ïú?ã§.
    if( ColdOut.TargetAmount <= ColdOut.CurrentAmount )
    {
        return TRUE;
    }

    return FALSE;
}

static void Outflow(U8 mu8Out)
{
    if( mu8Out == TRUE )
    {
        ColdOut.CloseStep = 0;
        OpenValveColdOut();
    }
    else
    {
        ColdOut.OpenStep = 0;
        CloseValveColdOut();
    }
}


// ?Éâ?àò Ï∂îÏ∂ú ?†ú?ñ¥
U8 ControlWaterColdOut(void)
{
    if( IsChangedOutStatus() == TRUE )
    {
        if( ColdOut.Out == TRUE )
        {
            // Do nothing...
        }
    }

    if( ColdOut.Out == TRUE )
    {
        if( IsDoneColdOut() == TRUE )
        {
            TurnOffColdOut();
            Sound(SOUND_EFFLUENT_END);
            return WATER_OUT_DONE;
        }
    }

    Outflow(ColdOut.Out);
    return WATER_OUT_GOING;
}
