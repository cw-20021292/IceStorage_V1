#include "err_leak.h"
#include "leak.h"


U8 CheckErrLeak(U8 mu8Error)
{
    return GetLeakStatus();
}

U8 ReleaseErrLeak(U8 mu8Error)
{
    return GetLeakStatus();
}
