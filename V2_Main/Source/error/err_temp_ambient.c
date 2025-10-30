#include "err_temp_ambient.h"
#include "err_temp.h"
#include "hal_adc.h"

static U8 mu8CountList[4] = 
{ 
    0, 
    COUNT_NUM, 
    COUNT_NUM, 
    COUNT_NUM 
};

U8 CheckErrTempAmbient(U8 mu8Error)
{
    U16 mu16Adc;

    mu16Adc = HAL_GetAdcValue( ANI_TEMP_AMBIENT );
    return CheckErrorTemp( mu8Error, mu16Adc, mu8CountList );
}


U8 ReleaseErrTempAmbient(U8 mu8Error)
{
    return CheckErrTempAmbient(mu8Error);
}
