#ifndef __HAL_ADC_H__
#define __HAL_ADC_H__

#include "prj_type.h"

/* ADC ID */
enum
{
    ANI_UV_WATER_FB,
    ANI_UV_ICE_FB,

    ANI_MAX_NUM
};

void HAL_InitAdc(void);

U16 HAL_GetAdcValue(U8  mu8Id );

void HAL_TurnOnSensingAdc(U8 mu8Id );

void HAL_TurnOffSensingAdc(U8 mu8Id );

void HAL_ProcessAdc(void);

#endif /* __HAL_ADC_H__ */

