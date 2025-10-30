#include "hw.h"
#include "hal_fan_motor.h"


/* DC FAN MOTOR */
void HAL_TurnOnFanMotor(void)
{
    TURN_ON_FAN_MOTOR_1();
}

void HAL_TurnOffFanMotor(void)
{
    TURN_OFF_FAN_MOTOR_1();
}

