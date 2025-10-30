#include "hw.h"
#include "hal_key.h"


#define  HAL_KEY_PUSHED     LOW
#define  HAL_LEVER_PUSHED   LOW

#define  DEFAULT_COUNT      50
U32 gu32Key     = 0UL;       // 터치 버튼 
U32 gu32KeyOut  = 0UL;       // 추출 버튼

U32 HAL_GetKeyVal(void)
{
    U32 mu32Val = 0UL;

    mu32Val = gu32Key | gu32KeyOut;
    return mu32Val;
}

static void ScanKey(void)
{
    static U16 mu16Count = 0;
    static U32 mu32PrevVal = 0;
    U32 mu32Val = 0;


    // SCAN KEY
    if( P_KEY_1 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_1;
    }
    if( P_KEY_2 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_2;
    }
    if( P_KEY_3 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_3;
    }
    if( P_KEY_4 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_4;
    }
    if( P_KEY_5 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_5;
    }
    if( P_KEY_6 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_6;
    }
    if( P_KEY_7 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_7;
    }
    if( P_KEY_8 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_8;
    }
    if( P_KEY_9 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_9;
    }
    if( P_KEY_10 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_10;
    }
    if( P_KEY_11 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_11;
    }
    if( P_KEY_12 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_12;
    }
#if 0
    if( P_KEY_13 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_13;
    }
    if( P_KEY_14 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_14;
    }
    if( P_KEY_15 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_15;
    }
    if( P_KEY_16 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_16;
    }
    if( P_KEY_17 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_17;
    }
    if( P_KEY_18 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_18;
    }
#endif

    // RESET KEY COUNT
    if( mu32PrevVal != mu32Val )
    {
        mu32PrevVal = mu32Val;
        mu16Count = DEFAULT_COUNT;

        return ;
    }

    if( mu16Count != 0 )
    {
        mu16Count--;
        return ;
    }

    // UPDATE NEW KEY
    gu32Key = mu32Val;
}

static void ScanKeyOut(void)
{
    static U32 mu32Count = 0;
    static U32 mu32PrevVal = 0;
    U32 mu32Val = 0;


    // SCAN KEY
    if( P_KEY_19 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_19;
    }

    if( P_KEY_20 == HAL_KEY_PUSHED )
    {
        mu32Val |= HAL_KEY_20;
    }


    // RESET KEY COUNT
    if( mu32PrevVal != mu32Val )
    {
        mu32PrevVal = mu32Val;
        mu32Count = DEFAULT_COUNT;

        return ;
    }

    if( mu32Count != 0 )
    {
        mu32Count--;
        return ;
    }

    // UPDATE NEW KEY
    gu32KeyOut = mu32Val;
}

void HAL_ScanKey(void)
{
    ScanKey();
    ScanKeyOut();
}
