#include "err_temp_hot_water_in.h"
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


U8 CheckErrTempHotWaterIn(U8 mu8Error)
{
    U16 mu16Adc;

    mu16Adc = HAL_GetAdcValue( ANI_TEMP_HOT_IN );
    return CheckErrorTemp( mu8Error, mu16Adc, mu8CountList );
}


U8 ReleaseErrTempHotWaterIn(U8 mu8Error)
{
    return CheckErrTempHotWaterIn(mu8Error);
}
