#ifndef __HAL_ADC_H__
#define __HAL_ADC_H__

#include "prj_type.h"

/* ADC ID */
enum
{
    ANI_TEMP_HOT_OUT,           // 온수 추출단
    ANI_TEMP_HEATER,            // 온수 히터 내부 온도
    ANI_TEMP_AMBIENT,    
    ANI_TEMP_ROOM_WATER, 
    ANI_TEMP_COLD_WATER, 

    ANI_TEMP_EVA_1,             
    ANI_SENSOR_LEAK,
    ANI_SENSOR_INFRARED,        // ICE FULL
    ANI_DC_FAN,                 // DC FAN

    ANI_TEMP_HOT_IN,
    ANI_24V_FB,                 // Diagnostic 24V feedback current
    ANI_12V_FB,                 // Diagnostic 12V feedback current
    ANI_UV_ICE_TRAY,            // UV - ICE TRAY

    ANI_HEATER_FB,              // Hot water Heater feedback current
    ANI_UV_ICE_TANK,            // UV - ICE TANK
    ANI_PUMP_DRAIN_FB,          // drain pump feedback current 

    // From front...
    ANI_UV_ICE_DOOR,            // UV - ICE DOOR
    ANI_UV_WATER_OUT,           // UV - WATER OUT

    ANI_MAX_NUM
};

void HAL_InitAdc(void);

U16 HAL_GetAdcValue(U8  mu8Id );

void HAL_SetAdcValue(U8 mu8Id, U16 mu16Adc );

void HAL_TurnOnSensingAdc(U8 mu8Id );

void HAL_TurnOffSensingAdc(U8 mu8Id );

void HAL_ProcessAdc(void);

#endif /* __HAL_ADC_H__ */

