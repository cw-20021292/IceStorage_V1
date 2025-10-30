#include "hw.h"
#include "hal_uv.h"


#define UV_OFF      0
#define UV_ON       1

 U8 gu8UvOnOff_IceDoor    = UV_OFF;
 U8 gu8UvOnOff_WaterOut   = UV_OFF;


void HAL_TurnOnUv( Hal_Uv_T	mId)
{
    if( HAL_UV_ICE_DOOR  == mId )
    {
        gu8UvOnOff_IceDoor = UV_ON;
    }
    else if( HAL_UV_WATER_OUT    == mId )
    {
        gu8UvOnOff_WaterOut = UV_ON;
    }
    else if( HAL_UV_ICE_TANK  == mId )
    {
        TURN_ON_UV_ICE_TANK();
    }
    else if( HAL_UV_ICE_TRAY  == mId )
    {
        TURN_ON_UV_ICE_TRAY();
    }
}

void HAL_TurnOffUv( Hal_Uv_T	mId)
{
    if( HAL_UV_ICE_DOOR  == mId )
    {
        gu8UvOnOff_IceDoor = UV_OFF;
    }
    else if( HAL_UV_WATER_OUT    == mId )
    {
        gu8UvOnOff_WaterOut = UV_OFF;
    }
    else if( HAL_UV_ICE_TANK  == mId )
    {
        TURN_OFF_UV_ICE_TANK();
    }
    else if( HAL_UV_ICE_TRAY  == mId )
    {
        TURN_OFF_UV_ICE_TRAY();
    }
}


// Return UV On/Off Status..
// 0: OFF
// 1: ON
U8 HAL_GetUvOnOffStatus( Hal_Uv_T mId )
{
    U8 mu8OnOff = OFF;

    if( HAL_UV_ICE_DOOR  == mId )
    {
        mu8OnOff = gu8UvOnOff_IceDoor;
    }
    else if( HAL_UV_WATER_OUT == mId )
    {
        mu8OnOff = gu8UvOnOff_WaterOut;
    }
    else if( HAL_UV_ICE_TANK  == mId )
    {
        mu8OnOff = GET_STATUS_UV_ICE_TANK();
    }
    else if( HAL_UV_ICE_TRAY  == mId )
    {
        mu8OnOff = GET_STATUS_UV_ICE_TRAY();
    }

    return mu8OnOff;
}

