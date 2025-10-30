#include "hw.h"
#include "hal_led.h"
#include "front.h"


#define ALL_ON      0xFFU
#define ALL_OFF     0x00U


typedef struct _leds_
{
    U8 Leds[ MAX_LED_BUF ];
    U8 Duty;
} Led_T;

Led_T   OnOff;
Led_T   Dimming;

void HAL_InitLed(void)
{
    MEMSET((void __FAR *)&OnOff.Leds[0], OFF, MAX_LED_BUF );
    MEMSET((void __FAR *)&Dimming.Leds[0], OFF, MAX_LED_BUF );

    OnOff.Duty = DEFAULT_ONOFF_DUTY;
    Dimming.Duty = DEFAULT_DIMMING_DUTY;
}

// ON/OFF
void HAL_TurnOnOffLEDAll(U8 mu8OnOff)
{
    if( mu8OnOff == ON )
    {
        MEMSET((void __FAR *)&OnOff.Leds[0], ALL_ON, MAX_LED_BUF );
    }
    else 
    {
        MEMSET((void __FAR *)&OnOff.Leds[0], ALL_OFF, MAX_LED_BUF );
    }
}

// 온소 LED는 모두 SKIP
static U8 IsValidCp(LedId_T led)
{
    if( led == HOT_COFFE 
     || led == HOT_TEA 
     || led == HOT_LOCK 
     || led == ICON_HOT_LOCK 
     || led == HOT_WATER 
     || led == ICE_MAKE 
     )
    {
        return FALSE;
    }
    
    return TRUE;
}

// CHPI -> CP 모델로 변경
static LedId_T MaskChp2Cp(LedId_T led)
{
    if( led == CP_ICE_TURBO )
    {
        led = HOT_WATER;
    }
    else if( led == CP_ICE_SIZE )
    {
        led = ICE_MODE;
    }
    else if( led == CP_ICE_MAKE )
    {
        led = HOT_LOCK;
    }

    return led;
}


void HAL_TurnOnOffLED(LedId_T led, U8 mu8OnOff)
{
    U8 mu8Byte;
    U8 mu8Bit;

    // CP 모델 LED MASK RE-MAPPING
    if( GetSystem() == SYSTEM_CP )
    {
        if( IsValidCp( led ) == FALSE )
        {
            return ;
        }

        led = MaskChp2Cp( led );
    }

    mu8Byte = led / 8;
    mu8Bit = led % 8;
    if( mu8OnOff == ON )
    {
        OnOff.Leds[ mu8Byte ] |= ((U8)1U<<mu8Bit);
    }
    else
    {
        OnOff.Leds[ mu8Byte ] &= ~((U8)1U<<mu8Bit);
    }
}

void HAL_TurnOnOffLEDEx(LedId_T led, U8 mu8OnOff)
{
    U8 mu8Byte;
    U8 mu8Bit;

    mu8Byte = led / 8;
    mu8Bit = led % 8;
    if( mu8OnOff == ON )
    {
        OnOff.Leds[ mu8Byte ] |= ((U8)1U<<mu8Bit);
    }
    else
    {
        OnOff.Leds[ mu8Byte ] &= ~((U8)1U<<mu8Bit);
    }

}

void HAL_GetLedOnOffStatus(U8 *pBuf)
{
    MEMCPY( (void __FAR *)pBuf, (const void __FAR *)&OnOff.Leds[0], MAX_LED_BUF );
}

void HAL_SetLedDuty(U8 mu8Duty)
{
    OnOff.Duty = mu8Duty;
    if( OnOff.Duty > 100 )
    {
        OnOff.Duty = 100;
    }
}

U8 HAL_GetLedDuty(void)
{
    return OnOff.Duty;
}


// DIMMING 
void HAL_DimmingLedAll(U8 mu8OnOff)
{
    if( mu8OnOff == ON )
    {
        MEMSET((void __FAR *)&Dimming.Leds[0], ALL_ON, MAX_LED_BUF );
    }
    else 
    {
        MEMSET((void __FAR *)&Dimming.Leds[0], ALL_OFF, MAX_LED_BUF );
    }
}

void HAL_DimmingLed(LedId_T led, U8 mu8OnOff)
{
    U8 mu8Byte;
    U8 mu8Bit;

    // CP 모델 LED MASK RE-MAPPING
    if( GetSystem() == SYSTEM_CP )
    {
        if( IsValidCp( led ) == FALSE )
        {
            return ;
        }

        led = MaskChp2Cp( led );
    }

    mu8Byte = led / 8;
    mu8Bit = led % 8;
    if( mu8OnOff == ON )
    {
        Dimming.Leds[ mu8Byte ] |= ((U8)1U<<mu8Bit);
    }
    else
    {
        Dimming.Leds[ mu8Byte ] &= ~((U8)1U<<mu8Bit);
    }
}

void HAL_GetLedDimmingStatus(U8 *pBuf)
{
    MEMCPY( (void __FAR *)pBuf, (const void __FAR *)&Dimming.Leds[0], MAX_LED_BUF );
}

void HAL_SetLedDimmingDuty(U8 mu8Duty)
{
    Dimming.Duty = mu8Duty;
    if( mu8Duty > 100 )
    {
        Dimming.Duty = 100;
    }
}

U8 HAL_GetLedDimmingDuty(void)
{
    return Dimming.Duty;
}

