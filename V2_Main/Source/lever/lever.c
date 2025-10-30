#include "hw.h"
#include "hal_key.h"
#include "lever.h"

#include <string.h>


/* Status */
#define  LEVER_STATUS_NONE            0      
#define  LEVER_STATUS_PUSHED          1
#define  LEVER_STATUS_POP             2
#define  LEVER_STATUS_TIME_OUT        3


/* Event Timeout condition */
#define  EVENT_TIME_OUT_LONG_2      800     // 8sec
#define  EVENT_TIME_OUT_LONG        200     // 2sec
#define  EVENT_TIME_OUT_SHORT       10      // 100ms

typedef struct _key_
{
    U16  Val;           // PUSH LEVER VAL
    U16  ValPop;        // PUSH LEVER VAL
    U8   Status;        // PUSH or POP
    U16  PushedTime;    // PUSH TIME...
    U8   EventFlag;     // Event Flag..
} Lever_T;

Lever_T   Lever[ MAX_LEVER_ID ];


void InitLever(void)
{
    MEMSET( (void __FAR *)&Lever, 0, sizeof( Lever ) );
}


U16 GetLeverVal(U8 mu8Id )
{
    return Lever[ mu8Id ].Val;
}

U16 GetLeverPopVal(U8 mu8Id )
{
    return Lever[ mu8Id ].ValPop;
}

void SetLeverEventFlag(U8 mu8Id, U8 mu8Event )
{
    Lever[ mu8Id ].EventFlag |= mu8Event;
}

void ClearLeverEventFlag(U8 mu8Id, U8 mu8Event )
{
    Lever[ mu8Id ].EventFlag &= ~mu8Event;
}

U8 IsSetLeverEventFlag(U8 mu8Id, U8 mu8Event )
{
    if( (Lever[ mu8Id ].EventFlag & mu8Event) ==  mu8Event )
    {
        return TRUE;
    }

    return FALSE;
}

static void ScanLever(U8 mu8Id, U16 mu8Val);
void ProcessScanLever(void)
{
    ScanLever( LEVER_ID_WATER, HAL_GetLeverWater() );
    ScanLever( LEVER_ID_ICE, HAL_GetLeverIce() );
}

/*
 * timeout 10ms...
 *
 */
static void ScanLever(U8 mu8Id, U16 mu8Val)
{
    static U16 mu16PrevLeverVal[ MAX_LEVER_ID ];


    // 1. Get Lever value 
    if( mu16PrevLeverVal[ mu8Id ] != mu8Val )
    {
        mu16PrevLeverVal[ mu8Id ]   = mu8Val;
        Lever[ mu8Id ].Val = mu8Val;  
        if( mu8Val == 0 )
        {
            Lever[ mu8Id ].ValPop = mu8Val;  
        }

        if( mu8Val != 0 )
        {
            //Init Event
            Lever[ mu8Id ].Status        = LEVER_STATUS_PUSHED;
            Lever[ mu8Id ].PushedTime    = 0;  
            ClearLeverEventFlag( mu8Id, LEVER_EVENT_PUSEHD_ALL );
        }
        else 
        {
            if( Lever[ mu8Id ].PushedTime >= EVENT_TIME_OUT_SHORT )
            {
                Lever[ mu8Id ].Status = LEVER_STATUS_POP;
                SetLeverEventFlag( mu8Id, LEVER_EVENT_POP );
            }
            else
            {
                Lever[ mu8Id ].Status = LEVER_STATUS_NONE;
            }
        }
    }
    
    if( Lever[ mu8Id ].Status == LEVER_STATUS_PUSHED )
    {
        Lever[ mu8Id ].PushedTime++;

        if( Lever[ mu8Id ].PushedTime == EVENT_TIME_OUT_LONG_2 )
        {
            SetLeverEventFlag( mu8Id, LEVER_EVENT_PUSHED_LONG_2 );
        }
        else if( Lever[ mu8Id ].PushedTime == EVENT_TIME_OUT_LONG )
        {
            SetLeverEventFlag( mu8Id, LEVER_EVENT_PUSHED_LONG );
        }
        else if( Lever[ mu8Id ].PushedTime == EVENT_TIME_OUT_SHORT )
        {
            SetLeverEventFlag( mu8Id, LEVER_EVENT_PUSHED_SHORT );
        }

        if( Lever[ mu8Id ].PushedTime > EVENT_TIME_OUT_LONG_2 )
        {
            Lever[ mu8Id ].Status = LEVER_STATUS_TIME_OUT;
        }
    }
}

