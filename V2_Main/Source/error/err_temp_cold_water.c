#include "err_temp_cold_water.h"
#include "err_temp.h"
#include "hal_adc.h"
#include "cold_water.h"
#include "temp.h"

static U8 mu8CountList[4] = 
{ 
    0,
    COUNT_NUM, 
    COUNT_NUM, 
    COUNT_NUM 
};

U8 CheckErrTempColdWater(U8 mu8Error)
{
    U16 mu16Adc;

    if( GetColdWaterConfigMake() == FALSE )
    {
        return FALSE; // no error
    }
    mu16Adc = HAL_GetAdcValue( ANI_TEMP_COLD_WATER );
    return CheckErrorTemp( mu8Error, mu16Adc, mu8CountList );
}

U8 ReleaseErrTempColdWater(U8 mu8Error )
{
    if( GetColdWaterConfigMake() == FALSE )
    {
        return FALSE; // no error
    }

    // 에러 상태이면, 센서 상태를 상시로 읽는다.
    TurnOnTempSensor( TEMP_ID_COLD_WATER, 10 );

    return CheckErrTempColdWater( mu8Error );
}
