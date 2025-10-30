/**
 * File : api_key.c
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#include "hw.h"

#include "api_key.h"

#define KEY_PUSHED        LOW
#define LEVER_PUSHED      LOW

#define DEBOUNCE_TIMEOUT (50)    /// Debounce Timeout, 50ms @1ms

typedef struct _api_key_
{
    U32 FrontKey;
    U32 OutKey;
    U32 Key;
} SApiKey_T;

SApiKey_T ApiKey;

/* Part For Control Key In Front */

/**
 * @ brief     Function For Initialize Key Info
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void InitApiKey(void)
{
    ApiKey.FrontKey = 0UL;
    ApiKey.OutKey = 0UL;
    ApiKey.Key = 0UL;
}

/**
 * @ brief            Function For Set Key Info (from front)
 * @ detail           none
 * @ param   xUkey    Key Info From Front
 * @ return           none
**/
void SetFrontKey(U32 key)
{
    ApiKey.FrontKey = key;
}

/**
 * @ brief            Function For Get Key Info (from front)
 * @ detail           Front (Uart)
 * @ param            none
 * @ return           Key Info From Front
**/
U32 GetFrontKey(void)
{
    return ApiKey.FrontKey;
}

/**
 * @ brief            Function For Set Key Info
 * @ detail           Main (Self)
 * @ param   xUkey    Key Info From Front
 * @ return           none
**/
void SetOutKey(U32 key)
{
    ApiKey.OutKey = key;
}

/**
 * @ brief            Function For Get Key Info
 * @ detail           none
 * @ param            none
 * @ return           Key Info From Front
**/
U32 GetOutKey(void)
{
    return ApiKey.OutKey;
}

/**
 * @ brief            Function For Get Key Info
 * @ detail           none
 * @ param            none
 * @ return           Key Info From Front
**/
U32 GetKey(void)
{
    ApiKey.Key = ( (ApiKey.FrontKey) | (ApiKey.OutKey) );

    return ApiKey.Key;
}

#if( CONFIG_USE_FRONT_KEY == 1 )
/**
 * @ brief            Function For Scan Front Key
 * @ detail           none
 * @ param            none
 * @ return           none
**/
void ScanKeyFront(void)
{
    static U16 debounce = 0U;
    static U32 prevKey = 0UL;
    U32 key = 0UL;

    /// SCAN KEY
    if( GET_STATUS_KEY_ALL_LOCK() == KEY_PUSHED )
    {
        key |= KEY_1;
    }

    if( GET_STATUS_KEY_COLD_OFF() == KEY_PUSHED )
    {
        key |= KEY_2;
    }

    if( GET_STATUS_KEY_HOT_LOCK() == KEY_PUSHED )
    {
        key |= KEY_3;
    }

    // if( GET_STATUS_KEY_ICE_WATER_LOCK() == KEY_PUSHED )
    // {
    //     key |= KEY_4;
    // }

    if( GET_STATUS_KEY_ICE_LOCK() == KEY_PUSHED )
    {
        key |= KEY_5;
    }

    // if( GET_STATUS_KEY_HOT_OFF() == KEY_PUSHED )
    // {
    //     key |= KEY_6;
    // }

    if( GET_STATUS_KEY_ICE_OFF() == KEY_PUSHED )
    {
        key |= KEY_7;
    } 
  
    // if( GET_STATUS_KEY_ICE_WATER_AMOUNT() == KEY_PUSHED )
    // {
    //     key |= KEY_8;
    // }

    if( GET_STATUS_KEY_HOT() == KEY_PUSHED )
    {
        key |= KEY_9;
    }

    if( GET_STATUS_KEY_ROOM() == KEY_PUSHED )
    {
        key |= KEY_10;
    }

    if( GET_STATUS_KEY_COLD() == KEY_PUSHED )
    {
        key |= KEY_11;
    }

    if( GET_STATUS_KEY_AMOUNT() == KEY_PUSHED )
    {
        key |= KEY_12;
    }

    if( GET_STATUS_KEY_ICE() == KEY_PUSHED )
    {
        key |= KEY_13;
    }

    if( GET_STATUS_KEY_ICE_WATER() == KEY_PUSHED )
    {
        key |= KEY_14;
    }

    // RESET KEY COUNT
    if( prevKey != key )
    {
        prevKey = key;
        debounce = DEBOUNCE_TIMEOUT;
        return;
    }

    if( debounce != 0 )
    {
        debounce--;
        return;
    }

    /// UPDATE NEW KEY
    SetFrontKey(key); 
}
#endif

#if( CONFIG_USE_MAIN_KEY == 1 )
/**
 * @ brief            Function For Scan Main Key
 * @ detail           none
 * @ param            none
 * @ return           none
**/
void ScanKeyMain(void)
{

}
#endif

#if( CONFIG_USE_OUT_KEY == 1 )
/**
 * @ brief            Function For Scan Main Key Out
 * @ detail           none
 * @ param            none
 * @ return           none
**/
void ScanKeyOut(void)
{
    static U16 debounce = 0U;
    static U32 prevKey = 0UL;
    U32 key = 0UL;

    /// SCAN KEY
    if( GET_STATUS_LEVER_ICE() == KEY_PUSHED )
    {
        key |= KEY_15;
    }

    if( GET_STATUS_LEVER_WATER() == KEY_PUSHED )
    {
        key |= KEY_16;
    }

    // RESET KEY COUNT
    if( prevKey != key )
    {
        prevKey = key;
        debounce = DEBOUNCE_TIMEOUT;
        return;
    }

    if( debounce != 0 )
    {
        debounce--;
        return;
    }

    /// UPDATE NEW KEY
    SetOutKey(key); 
}
#endif
