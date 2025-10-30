#include "mcu.h"

#include "hw.h"
#include "timer.h"
#include "util.h"
#include "parser.h"

#include "process_sys_event.h"

/* Init Timer */
static void InitStartTimers( void )
{
    InitTimer();

    StartTimer( TIMER_ID_1MS,   1);
    StartTimer( TIMER_ID_10MS,  10);
    StartTimer( TIMER_ID_100MS, 100);
}


void main( void )
{
    Delay_MS( 500 );

    RESET_WDT();

    InitSystem();
    InitStartTimers();
    EI();

    while(1)
    {
        RESET_WDT();

        ProcessEventHandler();

        /* COMM */
        RecvPacketHandler();
        SendPacketHandler();
    }
}
