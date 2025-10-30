#include "fan_motor.h"
#include "hal_fan_motor.h"
#include "comp.h"

#define FAN_OFF_TIME        300     // 5min @1sec

typedef struct _dc_fan_
{
    U16 OnTime;             // FAN ON 유지 시간 @1sec
    U8 OnOff[FAN_TYPE_NUM];
} Fan_T;

Fan_T   Fan;


void InitFanMotor(void)
{
    Fan.OnOff[FAN_TYPE_COLD] = OFF;
    Fan.OnOff[FAN_TYPE_ICE]  = OFF;
    Fan.OnTime = 0;

    HAL_TurnOffFanMotor();
}

void TurnOnFan(U8 mType)
{
    Fan.OnOff[mType] = ON;
}

void TurnOffFan(U8 mType)
{
    Fan.OnOff[mType] = OFF;
}

void TurnOnOffFan(U8 mType, U8 mu8OnOff)
{
    Fan.OnOff[mType] = mu8OnOff;
}


void ControlFanMotor(void)
{
    // FAN OFF, COMP OFF
    // -> RESET TIMER...
    if( Fan.OnOff[ FAN_TYPE_COLD ] == OFF
            && Fan.OnOff[ FAN_TYPE_ICE ] == OFF
            && GetCompOffTime() < 3 
      )
    {
        Fan.OnTime = FAN_OFF_TIME;
    }

    // FAN CONTROL....
    if( Fan.OnOff[ FAN_TYPE_COLD ] == ON 
            || Fan.OnOff[ FAN_TYPE_ICE ] == ON 
            || (Fan.OnTime != 0 && IsTurnOnCompBldc() == FALSE )   
      )
    {
        HAL_TurnOnFanMotor();
    }
    else
    {
        HAL_TurnOffFanMotor();
    }


    // FAN TIMER....
    if( Fan.OnTime != 0 )
    {
        Fan.OnTime--;
    }
}

