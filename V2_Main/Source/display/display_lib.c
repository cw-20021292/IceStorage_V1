#include "display_lib.h"

typedef struct _disp_timer_
{
    U16 TimerBlink[ MAX_DISP_TIMER_BLINK_ID ];
    U16 TimerCount[ MAX_DISP_TIMER_BLINK_ID ];

    U16 Timer[ MAX_DISP_KEY_TIMER_ID ];
} DispTimer_T;

DispTimer_T DispTimer;


void InitDispTimer(void)
{
    DispTimer.TimerCount[ DISP_TIMER_ID_500MS ]  = TIMER_500MS;
    DispTimer.TimerCount[ DISP_TIMER_ID_1SEC ]   = TIMER_1SEC;
}

U8 IsExpiredDispTimer(U8 id)
{
    if( DispTimer.Timer[ id ] == 0 )
    {
        return TRUE;
    }

    return FALSE;
}

void SetDispTimer(U8 id, U16 time)
{
    DispTimer.Timer[ id ] = time;
}


U8 IsExpiredDispBlinkTimer(U8 id)
{

    if( DispTimer.TimerBlink[ id ] == 0 )
    {
        return TRUE;
    }

    return FALSE;
}

static void ResetDispBlinkTimer( mu8TimerId )
{
    DispTimer.TimerBlink[ mu8TimerId ] = DispTimer.TimerCount[ mu8TimerId ];
}

/* LED 점멸 제어 함수 
 * Param
 *  - mu8OnOff : LED on/off 명령
 *  - pFun : LED 제어 함수 포인터 주소
 *  - mu16Timer : 타이머 변수 
 */
U8 BlinkLED( U8 mu8OnOff, void (*pFun)(U8 mu8OnOff), U8 mu8TimerId )
{
    if( pFun == NULL )
    {
        return ;  // 에러 처리..
    }

    if( IsExpiredDispBlinkTimer( mu8TimerId ) == TRUE )
    {
        if( mu8OnOff == OFF )
        {
            mu8OnOff = ON;
        }
        else
        {
            mu8OnOff = OFF;
        }
    }

    pFun( mu8OnOff );  
    return mu8OnOff;
}


void UpdateDisplayTimer(void)
{
    U8 i;


    /* Update Blink Timer */
    for( i = 0; i < MAX_DISP_TIMER_BLINK_ID; i++ )
    {
        if( DispTimer.TimerBlink[ i ] == 0 )
        {
            ResetDispBlinkTimer( i );
        }
        else
        {
            DispTimer.TimerBlink[ i ]--;
        }
    }

    /* Update Key Timer */
    for( i = 0; i < MAX_DISP_KEY_TIMER_ID; i++ )
    {
        if( DispTimer.Timer[ i ] != 0 )
        {
            DispTimer.Timer[ i ]--;
        }
    }
}
