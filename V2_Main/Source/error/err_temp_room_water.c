#include "err_temp_room_water.h"
#include "err_temp.h"
#include "hal_adc.h"
#include "temp.h"

static U8 mu8CountList[4] = 
{ 
    0,         /* Previous status */
    COUNT_NUM, 
    COUNT_NUM, 
    COUNT_NUM 
};


U8 CheckErrTempRoomWater(U8 mu8Error)
{
    U16 mu16Adc;

    mu16Adc = HAL_GetAdcValue( ANI_TEMP_ROOM_WATER );
    return CheckErrorTemp( mu8Error, mu16Adc, mu8CountList );
}


U8 ReleaseErrTempRoomWater(U8 mu8Error)
{
    // 에러 상태이면, 센서 상태를 상시로 읽는다.
    TurnOnTempSensor( TEMP_ID_ROOM_WATER, 10 );

    return CheckErrTempRoomWater(mu8Error);
}
