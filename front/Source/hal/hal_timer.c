/**
 * File : hal_timer.c
 * 
 * Hardware Abstraction Layer
 * Depend on Renesas MCU Chip
**/
#pragma interrupt INTTM00 BaseTimer_Interrupt
#pragma interrupt INTTM01 LedDimmingTimer_Interrupt

#include "hw.h"
#include "hal_timer.h"

#include "timer.h"
#include "led.h"

#if ( CONFIG_TIMER00 == 1 )

/**
 * @brief     Base Interrupt service routin
 * @detail    TAU0 / Channel0 / Interval Timer / 1ms
 * @param     none
 * @return    none
**/
__interrupt static void BaseTimer_Interrupt(void)
{
    volatile FPUserISR userISR = GetTimerISR();

    UpdateTimer();

    if( userISR != NULL )
    {
        userISR();
    }
}

#endif

#if ( CONFIG_TIMER01 == 1 )

/**
 * @brief     Led Dimming Interrupt service routin
 * @detail    TAU0 / Channel1 / Interval Timer / 1ms
 * @param     none
 * @return    none
**/
__interrupt static void LedDimmingTimer_Interrupt(void)
{
    EI();
    ControlLed();
}

#endif

#if ( CONFIG_TIMER04_05 == 1 )

/* Not Used Interrupt */

#endif


#if ( CONFIG_TIMER13 == 1 )

/* Not Used Interrupt */

#endif
