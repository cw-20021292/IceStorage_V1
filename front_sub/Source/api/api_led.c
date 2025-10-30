/**
 * File : api_led.c
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#include "hw.h"

#include "api_led.h"
#include "hal_timer.h"

void LedInit(void)
{
    HAL_TIMER01_START();
}

// --- SEGMENT 1 (A-G) ---

void LedOnOffSeg1A(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_1A();
    }
    else
    {
        TURN_OFF_LED_7SEG_1A();
    }
}

void LedOnOffSeg1B(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_1B();
    }
    else
    {
        TURN_OFF_LED_7SEG_1B();
    }
}

void LedOnOffSeg1C(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_1C();
    }
    else
    {
        TURN_OFF_LED_7SEG_1C();
    }
}

void LedOnOffSeg1D(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_1D();
    }
    else
    {
        TURN_OFF_LED_7SEG_1D();
    }
}

void LedOnOffSeg1E(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_1E();
    }
    else
    {
        TURN_OFF_LED_7SEG_1E();
    }
}

void LedOnOffSeg1F(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_1F();
    }
    else
    {
        TURN_OFF_LED_7SEG_1F();
    }
}

void LedOnOffSeg1G(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_1G();
    }
    else
    {
        TURN_OFF_LED_7SEG_1G();
    }
}

// --- SEGMENT 2 (A-G) ---

void LedOnOffSeg2A(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_2A();
    }
    else
    {
        TURN_OFF_LED_7SEG_2A();
    }
}

void LedOnOffSeg2B(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_2B();
    }
    else
    {
        TURN_OFF_LED_7SEG_2B();
    }
}

void LedOnOffSeg2C(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_2C();
    }
    else
    {
        TURN_OFF_LED_7SEG_2C();
    }
}

void LedOnOffSeg2D(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_2D();
    }
    else
    {
        TURN_OFF_LED_7SEG_2D();
    }
}

void LedOnOffSeg2E(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_2E();
    }
    else
    {
        TURN_OFF_LED_7SEG_2E();
    }
}

void LedOnOffSeg2F(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_2F();
    }
    else
    {
        TURN_OFF_LED_7SEG_2F();
    }
}

void LedOnOffSeg2G(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_2G();
    }
    else
    {
        TURN_OFF_LED_7SEG_2G();
    }
}

// --- SEGMENT 3 (A-G) ---

void LedOnOffSeg3A(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_3A();
    }
    else
    {
        TURN_OFF_LED_7SEG_3A();
    }
}

void LedOnOffSeg3B(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_3B();
    }
    else
    {
        TURN_OFF_LED_7SEG_3B();
    }
}

void LedOnOffSeg3C(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_3C();
    }
    else
    {
        TURN_OFF_LED_7SEG_3C();
    }
}

void LedOnOffSeg3D(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_3D();
    }
    else
    {
        TURN_OFF_LED_7SEG_3D();
    }
}

void LedOnOffSeg3E(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_3E();
    }
    else
    {
        TURN_OFF_LED_7SEG_3E();
    }
}

void LedOnOffSeg3F(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_3F();
    }
    else
    {
        TURN_OFF_LED_7SEG_3F();
    }
}

void LedOnOffSeg3G(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_3G();
    }
    else
    {
        TURN_OFF_LED_7SEG_3G();
    }
}

// --- SEGMENT 4 (A-G) ---

void LedOnOffSeg4A(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_4A();
    }
    else
    {
        TURN_OFF_LED_7SEG_4A();
    }
}

void LedOnOffSeg4B(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_4B();
    }
    else
    {
        TURN_OFF_LED_7SEG_4B();
    }
}

void LedOnOffSeg4C(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_4C();
    }
    else
    {
        TURN_OFF_LED_7SEG_4C();
    }
}

void LedOnOffSeg4D(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_4D();
    }
    else
    {
        TURN_OFF_LED_7SEG_4D();
    }
}

void LedOnOffSeg4E(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_4E();
    }
    else
    {
        TURN_OFF_LED_7SEG_4E();
    }
}

void LedOnOffSeg4F(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_4F();
    }
    else
    {
        TURN_OFF_LED_7SEG_4F();
    }
}

void LedOnOffSeg4G(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_4G();
    }
    else
    {
        TURN_OFF_LED_7SEG_4G();
    }
}

// --- SEGMENT 5 (A-G) ---

void LedOnOffSeg5A(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_5A();
    }
    else
    {
        TURN_OFF_LED_7SEG_5A();
    }
}

void LedOnOffSeg5B(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_5B();
    }
    else
    {
        TURN_OFF_LED_7SEG_5B();
    }
}

void LedOnOffSeg5C(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_5C();
    }
    else
    {
        TURN_OFF_LED_7SEG_5C();
    }
}

void LedOnOffSeg5D(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_5D();
    }
    else
    {
        TURN_OFF_LED_7SEG_5D();
    }
}

void LedOnOffSeg5E(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_5E();
    }
    else
    {
        TURN_OFF_LED_7SEG_5E();
    }
}

void LedOnOffSeg5F(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_5F();
    }
    else
    {
        TURN_OFF_LED_7SEG_5F();
    }
}

void LedOnOffSeg5G(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_5G();
    }
    else
    {
        TURN_OFF_LED_7SEG_5G();
    }
}

// --- SEGMENT 6 (A-G) ---

void LedOnOffSeg6A(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_6A();
    }
    else
    {
        TURN_OFF_LED_7SEG_6A();
    }
}

void LedOnOffSeg6B(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_6B();
    }
    else
    {
        TURN_OFF_LED_7SEG_6B();
    }
}

void LedOnOffSeg6C(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_6C();
    }
    else
    {
        TURN_OFF_LED_7SEG_6C();
    }
}

void LedOnOffSeg6D(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_6D();
    }
    else
    {
        TURN_OFF_LED_7SEG_6D();
    }
}

void LedOnOffSeg6E(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_6E();
    }
    else
    {
        TURN_OFF_LED_7SEG_6E();
    }
}

void LedOnOffSeg6F(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_6F();
    }
    else
    {
        TURN_OFF_LED_7SEG_6F();
    }
}

void LedOnOffSeg6G(U8 onOff)
{
    if( onOff == ON )
    {
        TURN_ON_LED_7SEG_6G();
    }
    else
    {
        TURN_OFF_LED_7SEG_6G();
    }
}
