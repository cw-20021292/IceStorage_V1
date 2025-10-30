/**
 * File : main.c
 * 
 * Purifier Main Loop
*/
#include "mcu.h"
#include "hw.h"
#include "parser.h"
#include "util.h"

#include "process_sys_event.h"

/**
 * @ brief    main loop
 * @ detail   none  
 * @ param  none
 * @ return   none
*/
void main(void)
{
    /// Initialize
    Delay_MS(1400U);  /// actually 500ms....

    RESET_WDT();

    InitSystem();
    InitStartTimer();
    EI();

    while(1)
    {
        RESET_WDT();

        /// System Scheduler
        ProcessEventHandler();

        /// Communication ( Uart )
        RecvPacketHandler();
        SendPacketHandler();
    }
}
