#ifndef __FAN_MOTOR_H__
#define __FAN_MOTOR_H__

#include "prj_type.h"


#define FAN_TYPE_ICE    0
#define FAN_TYPE_COLD   1
#define FAN_TYPE_NUM    2

void InitFanMotor(void);

void TurnOnFan(U8 mType);

void TurnOffFan(U8 mType);

void TurnOnOffFan(U8 mType, U8 mu8OnOff);

void ControlFanMotor(void);

#endif /* __FAN_MOTOR_H__ */

