/**
 * ë°°ìˆ˜( ??™ / ?ˆ˜?™ ) ê¸°ëŠ¥?— ????•œ ëª¨ë“ˆ
 *
 */
#include "empty_cold_tank.h"
#include "valve.h"
#include "error.h"

enum
{
    STEP_EMPTY_READY,      // ë°°ìˆ˜ ì¤?ë¹?
    STEP_EMPTY,
    STEP_EMPTY_COLD,
    STEP_EMPTY_CLOSE,
    STEP_EMPTY_DONE,
};

// ?ƒ‰?ˆ˜ ?ƒ±?¬ 800ml ì¶”ì¶œ ?‹œê°„ì?? 5ë¶? 30ì´?
// ?„¤? • ?‹œê°„ì?? 5ë¶? 40ì´?
#define FAUCET_TIMEOUT       3400UL //  5m 40sec * 60s * 10, @100ms

// ?„¤? • ?‹œê°„ì?? 12ë¶?
#define DRAIN_TIMEOUT       7200UL //  12m * 60s * 10, @100ms


EmptyColdTank_T     EmptyCold;


static U8 IsValidStartDrain(void);

void  InitEmptyColdTank(void)
{
    EmptyCold.Start  = FALSE;

}

void  GetEmptyColdTankData(EmptyColdTank_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&EmptyCold, sizeof( EmptyColdTank_T ));
}


// ??™ ë°°ìˆ˜ ?‹œ?‘/ì¤‘ì??
void StartEmptyColdTank(ColdEmptyType_T mu8Type)
{
    EmptyCold.Start = TRUE;

    EmptyCold.Type = mu8Type;
    EmptyCold.Step  = 0;
}


//void StartEmptyColdTank(U8 mu8Type)
//{
//    EmptyCold.Start = TRUE;
//}

void StopEmptyColdTank(void)
{
    if( EmptyCold.Start == TRUE )
    {
        if( EmptyCold.Step <= STEP_EMPTY_COLD )
        {
            EmptyCold.Step = STEP_EMPTY_CLOSE;
        }
    }
}

static void StopStatus(void)
{
    EmptyCold.Start = FALSE;
}

U8 IsStartEmptyColdTank(void)
{
    return EmptyCold.Start;
}

ColdEmptyType_T GetEmptyColdTankType(void)
{
    return EmptyCold.Type;
}

// ?ƒ‰?ˆ˜ ì¶”ì¶œ ?ŒŒ?š°?…‹ ë°¸ë¸Œë¡? ë¹„ìš°ê¸?
static U8 DoEmptyColdFaucet(void)
{
    static U8 mu16WaitTime = 0;
    U8 mStatus = FALSE;


    switch(EmptyCold.Step)
    {
        case STEP_EMPTY_READY:
            CloseValve( VALVE_COLD_IN );
            CloseValve( VALVE_ROOM_COLD_OUT );
            CloseValve( VALVE_COLD_DRAIN );

            OpenValve( VALVE_COLD_AIR );

            EmptyCold.ExitTime = FAUCET_TIMEOUT;
            mu16WaitTime = 2;   // 200ms..
            EmptyCold.Step++;
            break;

        case STEP_EMPTY:
            if( mu16WaitTime != 0 )
            {
                mu16WaitTime--;
            }
            else
            {
                OpenValve( VALVE_ROOM_COLD_OUT);
                EmptyCold.Step++;
            }
            break;

        case STEP_EMPTY_COLD:
            // ?ƒ‰?ˆ˜ ?ƒ±?¬ ë¹„ìš°ê¸?
            CloseValve( VALVE_COLD_IN );
            CloseValve( VALVE_COLD_DRAIN );

            OpenValve( VALVE_COLD_AIR );
            OpenValve( VALVE_ROOM_COLD_OUT);

            // ìµœë?? ë°°ìˆ˜ ?‹œê°? ?„¤? •
            if( EmptyCold.ExitTime != 0 )
            {
                EmptyCold.ExitTime--;
            }
            else
            {
                EmptyCold.Step++;
            }
            break;

        case STEP_EMPTY_CLOSE:
            CloseValve( VALVE_COLD_IN );
            CloseValve( VALVE_ROOM_COLD_OUT );

            OpenValve( VALVE_COLD_AIR );
            CloseValve( VALVE_COLD_DRAIN );

            EmptyCold.Step++;
            break;

        case STEP_EMPTY_DONE:
            mStatus = TRUE;
            break;

        default:
            EmptyCold.Step = 0;
            break;
    }


    return mStatus;
}

// ?ƒ‰?ˆ˜ ?ƒ±?¬ ë°°ìˆ˜ ë°¸ë¸Œë¡? ë¹„ìš°ê¸?
static U8 DoEmptyColdDrainValve(void)
{
    static U8 mu16WaitTime = 0;
    U8 mStatus = FALSE;


    switch(EmptyCold.Step)
    {
        case STEP_EMPTY_READY:
            CloseValve( VALVE_COLD_IN );
            CloseValve( VALVE_ROOM_COLD_OUT );
            CloseValve( VALVE_COLD_DRAIN );

            OpenValve( VALVE_COLD_AIR );

            EmptyCold.ExitTime = DRAIN_TIMEOUT;
            mu16WaitTime = 2;   // 200ms..
            EmptyCold.Step++;
            break;

        case STEP_EMPTY:
            if( mu16WaitTime != 0 )
            {
                mu16WaitTime--;
            }
            else
            {
                OpenValve( VALVE_COLD_DRAIN );
                EmptyCold.Step++;
            }
            break;

        case STEP_EMPTY_COLD:
            // ?ƒ‰?ˆ˜ ?ƒ±?¬ ë¹„ìš°ê¸?
            CloseValve( VALVE_COLD_IN );
            CloseValve( VALVE_COLD_DRAIN );

            OpenValve( VALVE_COLD_AIR );
            OpenValve( VALVE_COLD_DRAIN );

            // ìµœë?? ë°°ìˆ˜ ?‹œê°? ?„¤? •
            if( EmptyCold.ExitTime != 0 )
            {
                EmptyCold.ExitTime--;
            }
            else
            {
                EmptyCold.Step++;
            }
            break;

        case STEP_EMPTY_CLOSE:
            CloseValve( VALVE_COLD_IN );
            CloseValve( VALVE_ROOM_COLD_OUT );

            OpenValve( VALVE_COLD_AIR );
            CloseValve( VALVE_COLD_DRAIN );

            EmptyCold.Step++;
            break;

        case STEP_EMPTY_DONE:
            mStatus = TRUE;
            break;

        default:
            EmptyCold.Step = 0;
            break;
    }


    return mStatus;
}

static U8 DoEmptyCold(void)
{
    if( EmptyCold.Type == TYPE_FAUCET )
    {
        return DoEmptyColdFaucet();
    }
    
    // if EMPTY_COLD_TYPE_DRAIN_VALVE
    return DoEmptyColdDrainValve();
}

void ControlEmptyColdTank(void)
{
    U8 mu8Ret = FALSE;
    

    if( IsStartEmptyColdTank() == FALSE )
    {
        return ;
    }

    // ?—?Ÿ¬ ë°œìƒ?‹œ ì¢…ë£Œ
    if( IsErrorType( ERR_TYPE_EMPTY_COLD ) == TRUE )
    {
        StopEmptyColdTank();
    }

    // ë°°ìˆ˜...
    if( DoEmptyCold() == TRUE )
    {
        StopStatus();
    }
}

