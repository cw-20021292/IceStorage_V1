#include "err_temp_heater.h"
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


U8 CheckErrTempHeater(U8 mu8Error)
{
    U16 mu16Adc;

    mu16Adc = HAL_GetAdcValue( ANI_TEMP_HEATER );
    return CheckErrorTemp( mu8Error, mu16Adc, mu8CountList );
}


U8 ReleaseErrTempHeater(U8 mu8Error)
{
    return CheckErrTempHeater(mu8Error);
}
