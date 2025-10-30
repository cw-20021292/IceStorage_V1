#pragma interrupt INTTM00 BaseTimerInterrupt

#include "hw.h"
#include "timer.h" 

#include <string.h>


/* BASE TIMER INTERRUPT */


#define HZ  (1UL)

typedef struct _timer_
{
    U8      enable;
    U8      type;
    U32 time_out;
} timer_info_t;

LOCAL timer_info_t      timer_info[ MAX_TIMER_ID_NUM ];

static void (*pFunUserISR)(void) = NULL ;   // 사용자 등록 ISR 함수


void InitTimer( void )
{
    MEMSET( (void __FAR *)timer_info, 0, sizeof( timer_info) );

   R_TAU0_Channel0_Start();   // Start Timer-00
}


void StartTimer( U8 id, U32 time_out )
{
    INTS_BYTE_BASE_TIMER();

    ENTER_CRITICAL_SECTION_TIMER();

    timer_info[ id ].enable     = 1;
    timer_info[ id ].time_out   = time_out / HZ;
#if (HZ != 1UL)
    if( (time_out % HZ) >= (U32)5 )
    {
        timer_info[ id ].time_out++;
    }
#endif

    EXIT_CRITICAL_SECTION_TIMER();

}

void    DisableTimer( U8 id )
{
    timer_info[ id ].enable     = 0;
    timer_info[ id ].time_out   = (U32)-1;
}

void    StopTimer( U8 id )
{
    DisableTimer( id );
}


U8 IsExpiredTimer( U8 id )
{
    U32 time_out;
    INTS_BYTE_BASE_TIMER();


    ENTER_CRITICAL_SECTION_TIMER();
    time_out = timer_info[ id ].time_out;
    EXIT_CRITICAL_SECTION_TIMER();

    if( timer_info[ id ].enable == 0 )
    {
        return TIMER_DISABLE;
    }

    if( time_out > 0 )
    {
        return TIMER_NOT_EXPIRE;
    }

    return TIMER_EXPIRE;
}

/* TIMER - MS */
static void UpdateTimer( void )
{
    U8  i;

    for( i = 0 ; i < MAX_TIMER_ID_NUM ; i++ )
    {
        if( timer_info[ i ].enable == 0 )
        {
            continue;
        }

        if( timer_info[ i ].time_out > 0 )
        {
            timer_info[ i ].time_out--;
        }
    }
}


/* 타이머 입터럽트 사용자 ISR 함수 등록 */
void RegisterTimerISR( void (*pUserISR)(void) )
{
    pFunUserISR = pUserISR;
}


/* INTERRUPT SERVICE ROUTIN */
void BaseTimerInterrupt(void)
{
    UpdateTimer();

    /* 사용자 등록 인터럽트 함수를 호출한다. */
    if( pFunUserISR != NULL )
    {
        pFunUserISR();
    }
}

