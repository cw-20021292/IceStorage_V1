#include "hw.h"
#include "hal_uv.h"

U8 gu8UvWater = OFF;
U8 gu8UvIce = OFF;

void HAL_InitUv(void)
{
    TURN_OFF_UV_ICE();
    TURN_OFF_UV_WATER();

    gu8UvWater = OFF;
    gu8UvIce = OFF;
}

void HAL_SetUvOnOffId(U8 id, U8 OnOff)
{
    if( id == UV_WATER )
    {
        gu8UvWater = OnOff;
        if( OnOff == ON )
        {
            TURN_ON_UV_WATER();
        }
        else
        {
            TURN_OFF_UV_WATER();
        }
    }
    else if( id == UV_ICE )
    {
        gu8UvIce = OnOff;
        if( OnOff == ON )
        {
            TURN_ON_UV_ICE();
        }
        else
        {
            TURN_OFF_UV_ICE();
        }
    }
}


void HAL_ControlUv(void)
{
    if( gu8UvWater == ON )
    {
        TURN_ON_UV_WATER();
    }
    else
    {
        TURN_OFF_UV_WATER();
    }

    if( gu8UvIce == ON )
    {
        TURN_ON_UV_ICE();
    }
    else
    {
        TURN_OFF_UV_ICE();
    }

    if( gu8UvIce == ON || gu8UvWater == ON )
    {
        TURN_ON_24V();
    }
    else
    {
        TURN_OFF_24V();
    }
}
