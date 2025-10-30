#include "err_soda_pump.h"
#include "soda_pump.h"


U8 CheckErrSodaPump(U8 mu8Error)
{
    return GetSodaPumpErrorDisp();
}

U8 ReleaseErrSodaPump(U8 mu8Error)
{
    return GetSodaPumpErrorDisp();
}
