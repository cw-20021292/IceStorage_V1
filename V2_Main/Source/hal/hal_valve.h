#ifndef __HAL_VALVE_H__
#define __HAL_VALVE_H__

#include "prj_type.h"

typedef enum
{
    HAL_VALVE_NOS,            
    HAL_VALVE_ROOM_COLD_OUT,
    HAL_VALVE_ROOM_IN,
    HAL_VALVE_HOT_IN,       
    HAL_VALVE_HOT_OUT, 
    HAL_VALVE_HOT_DRAIN,          
    HAL_VALVE_COLD_AIR,       
    HAL_VALVE_COLD_IN,        
    HAL_VALVE_COLD_DRAIN,
    HAL_VALVE_COLD_FLUSH,    
    HAL_VALVE_ICE_TRAY_IN,    
    HAL_VALVE_ICE_WATER_OUT,

    //HAL_VALVE_DRAIN,          
    //HAL_VALVE_FEED,           
    //HAL_VALVE_SODA_OUT,          
    //HAL_VALVE_SODA_DRAIN,          

    MAX_HAL_VALVE_NUM
} Valve_T;

void HAL_OpenValve( Valve_T valve);
void HAL_CloseValve( Valve_T valve);


#endif /* __HAL_VALVE_H__ */
