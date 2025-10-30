#include "hw.h"
#include "hal_key.h"


#define  HAL_KEY_PUSHED     LOW
#define  HAL_LEVER_PUSHED   LOW


typedef struct _hal_key_
{
    U32 Key;
    U8  WaterOut;
    U8  IceOut;
} TKeyVal;

TKeyVal HalKey;


void HAL_InitKey(void)
{
    HalKey.Key      = 0UL;
    HalKey.WaterOut = 0U;
    HalKey.IceOut   = 0U;
}

void HAL_SetKeyVal(U32 mu32Val)
{
    HalKey.Key = mu32Val;
}

U32 HAL_GetKeyVal(void)
{
    return HalKey.Key;
}


void HAL_SetLeverIce(U8 mu8Val)
{
    HalKey.IceOut = mu8Val;
}

U8 HAL_GetLeverIce(void)
{
    return HalKey.IceOut;
}


void HAL_SetLeverWater(U8 mu8Val)
{
    HalKey.WaterOut = mu8Val;
}


U8 HAL_GetLeverWater(void)
{
    return HalKey.WaterOut;
}
