#ifndef __HAL_UV_H__
#define __HAL_UV_H__

#include "prj_type.h"

typedef enum
{
    HAL_UV_ICE_DOOR,
    HAL_UV_WATER_OUT,
    HAL_UV_ICE_TANK,
    HAL_UV_ICE_TRAY,

    MAX_HAL_UV_NUM
} Hal_Uv_T;


void HAL_TurnOnUv( Hal_Uv_T	mId);

void HAL_TurnOffUv( Hal_Uv_T	mId);


// Return UV On/Off Status..
// 0: OFF
// 1: ON
U8 HAL_GetUvOnOffStatus( Hal_Uv_T mId );

#endif /* __HAL_UV_H__ */
