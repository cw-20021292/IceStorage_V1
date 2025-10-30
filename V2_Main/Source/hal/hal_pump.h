#ifndef __HAL_PUMP_H__
#define __HAL_PUMP_H__

#include "prj_type.h"

// PUMP
#define  HAL_PUMP_DRAIN       0U
#define  HAL_PUMP_CIRCULATE   1U
#define  HAL_PUMP_SODA        2U

/*
 * mf32Volt 15V ~ 24V 
 */
void HAL_TurnOnPump(U8 mu8PumpId, F32 mf32Volt);
void HAL_TurnOffPump(U8 mu8PumpId);
U8 HAL_IsTurnOnDrainPump(void);


#endif /* __HAL_VALVE_H__ */
