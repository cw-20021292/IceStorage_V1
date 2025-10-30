#include "mode.h"
#include "hal_led.h"



U8 gu8Mode = MODE_NONE;
U8 gu8Step = 0;
void InitMode(void)
{
    gu8Mode = MODE_NONE;
}

void SetMode(U8 mu8Mode)
{
    if( gu8Mode != mu8Mode )
    {
        gu8Mode = mu8Mode;
        gu8Step = 0;
    }
}

U8 GetMode(void)
{
    return gu8Mode;
}


U16 mu16Delay = 0;
U8 led_buf[MAX_LED];
#define ON_TIME         30      // 3sec, @100ms
#define OFF_TIME        10      // 1sec, @100ms
static void ModeFota(void)
{
    switch( gu8Step )
    {
        case 0:
            memset( &led_buf[0], 0, MAX_LED );
            HAL_SetOnOffLED( &led_buf[0], MAX_LED );
            HAL_SetDimmingLED( &led_buf[0], MAX_LED );      // LED DIMMING ON/OFF

            gu8Step++;
            break;

        case 1:
            HAL_TurnOnOffLED( ICON_WIFI, ON );
            mu16Delay = ON_TIME;
            gu8Step++;
            break;

        case 2:
            if( --mu16Delay == 0 )
            {
                gu8Step++;
            }
            break;

        case 3:
            HAL_TurnOnOffLED( ICON_WIFI, OFF );
            mu16Delay = OFF_TIME;
            gu8Step++;
            break;

        case 4:
            if( --mu16Delay == 0 )
            {
                gu8Step = 0;
            }
            break;

        default:
            break;
    }
}

void ProcessMode(void)
{
    if( gu8Mode == MODE_FOTA )
    {
        // wifi led 3sec on - wifi led 1sec off - repeat....

        ModeFota();
    }
}
