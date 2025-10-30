#include "err_temp_eva_1.h"
#include "err_temp.h"
#include "hal_adc.h"

static U8 mu8CountList[4] = 
{ 
    0,            /* Previous status */
    COUNT_NUM,    
    COUNT_NUM, 
    COUNT_NUM 
};


U8 CheckErrTempEva_1(U8 mu8Error)
{
    U16 mu16Adc;

    mu16Adc = HAL_GetAdcValue( ANI_TEMP_EVA_1 );
    return CheckErrorTemp( mu8Error, mu16Adc, mu8CountList );
}


U8 ReleaseErrTempEva_1(U8 mu8Error)
{
    return CheckErrTempEva_1(mu8Error);
}
