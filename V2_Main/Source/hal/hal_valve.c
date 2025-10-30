#include "hw.h"
#include "hal_valve.h"

static void HAL_Power(void)
{
    if( GET_STATUS_VALVE_ROOM_COLD_OUT() != 0 
            || GET_STATUS_VALVE_COLD_OUT() != 0 
            || GET_STATUS_VALVE_HOT_OUT() != 0
      )
    {
        OPEN_VALVE_POWER_OUT();
    }
    else
    {
        CLOSE_VALVE_POWER_OUT();
    }
}

static void HAL_Open(Valve_T valve )
{
    if( HAL_VALVE_NOS == valve )
    {
       OPEN_VALVE_NOS(); 
    }
    else if( HAL_VALVE_ROOM_COLD_OUT == valve )
    {
        OPEN_VALVE_ROOM_COLD_OUT();
    }
    else if( HAL_VALVE_ROOM_IN == valve )
    {
        OPEN_VALVE_ROOM_IN();
    }
    else if( HAL_VALVE_HOT_IN == valve )
    {
        OPEN_VALVE_HOT_IN();
    }
    else if( HAL_VALVE_HOT_OUT == valve )
    {
        OPEN_VALVE_HOT_OUT();
    }
    else if( HAL_VALVE_ICE_WATER_OUT == valve )
    {
        OPEN_VALVE_ICE_WATER_OUT();
    }
    else if( HAL_VALVE_HOT_DRAIN == valve )
    {
        OPEN_VALVE_HOT_DRAIN();
    }
    else if( HAL_VALVE_COLD_AIR == valve )
    {
        OPEN_VALVE_COLD_AIR();
    }
    else if( HAL_VALVE_COLD_IN == valve )
    {
        OPEN_VALVE_COLD_IN();
    }
    else if( HAL_VALVE_COLD_DRAIN == valve )
    {
        OPEN_VALVE_COLD_DRAIN();
    }
    else if( HAL_VALVE_COLD_FLUSH == valve )
    {
        OPEN_VALVE_COLD_FLUSH();
    }
    else if( HAL_VALVE_ICE_TRAY_IN == valve )
    {
        OPEN_VALVE_ICE_TRAY_IN();
    }
#if 0
    else if( HAL_VALVE_SODA_OUT == valve )
    {
        OPEN_VALVE_SODA_OUT();
        OPEN_VALVE_POWER_OUT();
    }
    else if( HAL_VALVE_SODA_DRAIN == valve )
    {
        OPEN_VALVE_SODA_DRAIN();
    }
    else if( HAL_VALVE_FEED == valve )
    {
       OPEN_VALVE_FEED(); 
    }
#endif

}


static void HAL_Close(Valve_T valve)
{
    if( HAL_VALVE_NOS == valve )
    {
       CLOSE_VALVE_NOS(); 
    }
    else if( HAL_VALVE_ROOM_COLD_OUT == valve )
    {
        CLOSE_VALVE_ROOM_COLD_OUT();
    }
    else if( HAL_VALVE_ROOM_IN == valve )
    {
        CLOSE_VALVE_ROOM_IN();
    }
    else if( HAL_VALVE_HOT_IN == valve )
    {
        CLOSE_VALVE_HOT_IN();
    }
    else if( HAL_VALVE_HOT_OUT == valve )
    {
        CLOSE_VALVE_HOT_OUT();
    }
    else if( HAL_VALVE_ICE_WATER_OUT == valve )
    {
        CLOSE_VALVE_ICE_WATER_OUT();
    }
    else if( HAL_VALVE_HOT_DRAIN == valve )
    {
        CLOSE_VALVE_HOT_DRAIN();
    }
    else if( HAL_VALVE_COLD_AIR == valve )
    {
        CLOSE_VALVE_COLD_AIR();
    }
    else if( HAL_VALVE_COLD_IN == valve )
    {
        CLOSE_VALVE_COLD_IN();
    }
    else if( HAL_VALVE_COLD_DRAIN == valve )
    {
        CLOSE_VALVE_COLD_DRAIN();
    }
    else if( HAL_VALVE_COLD_FLUSH == valve )
    {
        CLOSE_VALVE_COLD_FLUSH();
    }
    else if( HAL_VALVE_ICE_TRAY_IN == valve )
    {
        CLOSE_VALVE_ICE_TRAY_IN();
    }
#if 0
    else if( HAL_VALVE_SODA_OUT == valve )
    {
        CLOSE_VALVE_SODA_OUT();
    }
    else if( HAL_VALVE_SODA_DRAIN == valve )
    {
        CLOSE_VALVE_SODA_DRAIN();
    }
    else if( HAL_VALVE_FEED == valve )
    {
       CLOSE_VALVE_FEED(); 
    }
#endif

}

void HAL_OpenValve(Valve_T valve )
{
    HAL_Open(valve);
    HAL_Power();
}

void HAL_CloseValve(Valve_T valve)
{
    HAL_Close(valve);
    HAL_Power();
}
