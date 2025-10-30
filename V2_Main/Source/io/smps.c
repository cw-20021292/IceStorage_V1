#include "hw.h"
#include "smps.h"

#include "hal_uv.h"
#include "gas_switch_valve.h"


// ICE DOOR, ICE WATER on Front board...
static U8 IsTurnOnUvOnBoard(void)
{
    if( HAL_GetUvOnOffStatus( HAL_UV_ICE_TANK ) != 0 )
    {
        return TRUE;    // ON
    }

    if( HAL_GetUvOnOffStatus( HAL_UV_ICE_TRAY ) != 0 )
    {
        return TRUE;    // ON
    }

    if( HAL_GetUvOnOffStatus( HAL_UV_WATER_OUT ) != 0 )
    {
        return TRUE;    // ON
    }

    return FALSE;   // OFF
}

void ControlSmps(void)
{
    U8 mu8OnOff = OFF;


    // GAS VALVE
    if( GetGasSwitchMode() != GAS_SWITCH_MODE_NONE )
    {
        mu8OnOff = ON;
    }

    // UV CHECK
    if( IsTurnOnUvOnBoard() == ON )
    {
        mu8OnOff = ON;
    }

    // CONTROL SMPS 12V
#if CONFIG_UV_MANUAL
    TURN_ON_SMPS_12V();
#else
    if( mu8OnOff == ON )
    {
        TURN_ON_SMPS_12V();
    }
    else
    {
        TURN_OFF_SMPS_12V();
    }
#endif

}
