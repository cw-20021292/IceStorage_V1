#include "hw.h"
#include "hal_uv.h"
#include "uv.h"
#include "util.h"

#include "ice.h"
#include "ice_door.h"
#include "ice_full.h"
#include "water_out.h"
#include "hal_input.h"


  
typedef struct _uv_item_
{
    UvId_T Id;
    Hal_Uv_T Id_Hal;

    U8 Config;      // 일시 정지
    U8 Pause;       // 일시 정지

    U32 OffTime;    // @100ms
    U32 OnTime;     // @100ms
    U32 ConfOnTime; // @100ms

    U8  IsLimited;  // LIMITED or UNLIMITED
    U8  Count;      // LIMIT COUNT
} UvItem_T;

UvItem_T UvOnOffList[] = 
{
    /* ID,                  HAL ID,            CONFIG,  PAUSE, T_OFF,  T_ON, TC_ON  LIMIT,      LIMIT COUNTER */
    { UV_ICE_DOOR,          HAL_UV_ICE_DOOR,   TRUE,    OFF,   0,      0,    0,     UNLIMITED,  0 },
    { UV_ICE_DOOR_SHOT,     HAL_UV_ICE_DOOR,   FALSE,   OFF,   0,      0,    0,     UNLIMITED,  0 },

    { UV_WATER_OUT,         HAL_UV_WATER_OUT,  TRUE,    OFF,   0,      0,    0,     UNLIMITED,  0 },
    { UV_WATER_OUT_SHOT,    HAL_UV_WATER_OUT,  FALSE,   OFF,   0,      0,    0,     UNLIMITED,  0 },

    { UV_ICE_TRAY,          HAL_UV_ICE_TRAY,   TRUE,    OFF,   0,      0,    0,     LIMITED,    6 },
    { UV_ICE_TRAY_SHOT,     HAL_UV_ICE_TRAY,   FALSE,   OFF,   0,      0,    0,     UNLIMITED,  0 },

    { UV_ICE_TANK,          HAL_UV_ICE_TANK,   TRUE,    OFF,   0,      0,    0,     UNLIMITED,  0 },
};

#define SZ_UV_LIST      (sizeof( UvOnOffList ) / sizeof( UvItem_T ))



// 0보다 크면, ON 
// 0보다 작으면, OFF
I8 UvOnOff[MAX_HAL_UV_NUM];
static void InitOnOffUv(void)
{
    U8 i;

    for( i = 0; i < MAX_HAL_UV_NUM ; i++ )
    {
        UvOnOff[ i ] = 0;
    }
}

static void TurnOnUv( Hal_Uv_T	mId)
{
    UvOnOff[ mId ]++;
}

static void TurnOffUv( Hal_Uv_T	mId)
{
    if( UvOnOff[ mId ] > 0 )
    {
        UvOnOff[ mId ]--;
    }
}

static void OnOffUv(void)
{
    U8 i;

    for( i = 0; i < MAX_HAL_UV_NUM ; i++ )
    {
        if( UvOnOff[ i ] > 0 )
        {
            HAL_TurnOnUv( i );
        }
        else 
        {
            HAL_TurnOffUv( i );
        }
    }
}


static U8 FindListIndexId(UvId_T id, U8 *pIndex)
{
    U8 i;

    for( i = 0; i < SZ_UV_LIST ; i++ )
    {
        if( UvOnOffList[ i ].Id == id )
        {
            *pIndex = i;
             return TRUE;
        }
    }

    return FALSE;
}


void SetConfigUvId(UvId_T id, U8 config)
{
    U8 mu8Index;

    if( FindListIndexId( id, &mu8Index ) == TRUE )
    {
        UvOnOffList[ mu8Index ].Config = config;
    }
}

U8 GetConfigUvId(UvId_T id)
{
    U8 mu8Index;

    if( FindListIndexId( id, &mu8Index ) == TRUE )
    {
        return UvOnOffList[ mu8Index ].Config;
    }

    return FALSE;
}


// unlimited 타입이면, 항상 ok(FALSE)
// 그렇지 않은 경우, count에 의해 확인
static U8 IsOverLimitCount(U8 mu8Index)
{
    if( UvOnOffList[ mu8Index ].IsLimited == UNLIMITED )
    {
        return FALSE;   
    }

    if( UvOnOffList[ mu8Index ].Count == 0 )
    {
        return TRUE;
    }

    return FALSE;
}

// limit count 확인해서 count가 없으면 on 설정이 불가
void TurnOnUvId(UvId_T id, U32 OffTime, U32 OnTime )
{
    U8 mu8Index;

    if( FindListIndexId( id, &mu8Index ) == TRUE )
    {
        if( UvOnOffList[ mu8Index ].Config == FALSE )
        {
            return ;
        }

        if( IsOverLimitCount( mu8Index ) == TRUE )
        {
            return ;
        }

        DOWN_COUNT( UvOnOffList[ mu8Index ].Count );
        UvOnOffList[ mu8Index ].OffTime     = OffTime;
        UvOnOffList[ mu8Index ].OnTime      = OnTime;
        UvOnOffList[ mu8Index ].ConfOnTime  = OnTime;
    }
}

void TurnOffUvId(UvId_T id)
{
    U8 mu8Index;

    if( FindListIndexId( id, &mu8Index ) == TRUE )
    {
        UvOnOffList[ mu8Index ].OffTime     = 0;
        UvOnOffList[ mu8Index ].OnTime      = 0;
        UvOnOffList[ mu8Index ].ConfOnTime  = 0;
    }
}

U8  IsTurnOnUvId(UvId_T id)
{
    U8 mu8Index;

    if( FindListIndexId( id, &mu8Index ) == TRUE )
    {
        if( UvOnOffList[ mu8Index ].OffTime == 0 
            && UvOnOffList[ mu8Index ].OnTime != 0 )
        {
            return TRUE;
        }
    }

    return FALSE;
}

void SetPauseUvId(UvId_T id, U8 status)
{
    U8 mu8Index;

    if( FindListIndexId( id, &mu8Index ) == TRUE )
    {
        UvOnOffList[ mu8Index ].Pause = status;
    }
}


U8 GetUvOnPercent(UvId_T id)
{
    U8 mu8Index;
    U32 mu32Current;
    U32 mu32Target;

    if( FindListIndexId( id, &mu8Index ) == TRUE )
    {
        mu32Current = UvOnOffList[mu8Index].ConfOnTime - UvOnOffList[mu8Index].OnTime;
        mu32Target  = UvOnOffList[mu8Index].ConfOnTime;

        return (U8)(( (F32)mu32Current / (F32)mu32Target ) * 100.0f);
    }

    return 100;
}

void SetLimitConfUvId(UvId_T id, U8 IsLimited)
{
    U8 mu8Index;

    if( FindListIndexId( id, &mu8Index ) == TRUE )
    {
        UvOnOffList[ mu8Index ].IsLimited = IsLimited;
    }
}

void SetLimitCountUvId(UvId_T id, U8 mu8Count)
{
    U8 mu8Index;

    if( FindListIndexId( id, &mu8Index ) == TRUE )
    {
        UvOnOffList[ mu8Index ].Count = mu8Count;
    }
}


void ControlUv(void)
{
    U8 i;
    UvItem_T *pItem = NULL;


    InitOnOffUv();
    for( i = 0; i < SZ_UV_LIST ; i++ )
    {
        pItem = &UvOnOffList[ i ];

        // Check Pause
        if( pItem->Pause == TRUE )
        {
            DOWN_COUNT( pItem->OffTime );
            continue;
        }

        // Check On/Off
        if( pItem->OffTime == 0 
                && pItem->OnTime != 0)
        {
            TurnOnUv( pItem->Id_Hal );
            DOWN_COUNT( pItem->OnTime );
        }
        else
        {
            DOWN_COUNT( pItem->OffTime );
        }
    }

    OnOffUv();
}
