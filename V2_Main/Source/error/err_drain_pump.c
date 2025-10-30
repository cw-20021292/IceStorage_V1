#include "err_drain_pump.h"
#include "drain_pump.h"


U8 CheckErrDrainPump(U8 mu8Error)
{
    return GetDrainPumpError();
}

#if 0   // unused
U8 ReleaseErrDrainPump(U8 mu8Error)
{
    return GetDrainPumpError();
}
#endif
