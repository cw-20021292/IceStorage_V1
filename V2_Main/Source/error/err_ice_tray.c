#include "err_ice_tray.h"
#include "ice_tray.h"


/* ICE TRAY ERROR - MOVING */
U8 CheckErrIceTrayMoving(U8 mu8Error)
{
    return GetIceTrayErrorMoving();
}

/* ICE TRAY ERROR - MICRO SWITCH DETECT */
U8 CheckErrIceTrayMicroSwitch(U8 mu8Error)
{
    return GetIceTrayErrorMicroSwitch();
}

