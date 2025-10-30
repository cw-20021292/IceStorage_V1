#ifndef __HAL_RELAY_H__
#define __HAL_RELAY_H__

typedef enum
{
    HAL_RELAY_COMP,
//    HAL_RELAY_HEATER_HIGH,
    //HAL_RELAY_ICE_HEATER,

    HAL_RELAY_FEEDER_CW,
    HAL_RELAY_FEEDER_CCW,

    HAL_RELAY_ICE_TRAY_CW,
    HAL_RELAY_ICE_TRAY_CCW,

    MAX_HAL_RELAY_NUM
} Relay_T;


void HAL_TurnOnRelay( Relay_T	relay);
void HAL_TurnOffRelay( Relay_T	relay);


#endif /* __HAL_RELAY_H__ */
