#include "prj_type.h"
#include "process_uv.h"
#include "uv.h"
#include "util.h"
#include "valve.h"

#include "ice.h"
#include "ice_full.h"
#include "ice_door.h"
#include "ice_tray.h"
#include "water_out.h"
#include "hal_input.h"
#include "hal_uv.h"
#include "power_saving.h"
#include "health.h"


// ON/OFF TIME @100ms
#define UV_3_SEC           30UL         // 3sec @100ms
#define UV_7_SEC           70UL         // 7sec @100ms
#define UV_2_MIN           1200UL       // 2 min * * 60 sec 
#define UV_15_MIN          9000UL       // 15 min * * 60 sec 
#define UV_30_MIN          18000UL      // 30 min * * 60 sec 
#define UV_1_HOUR          36000UL      // 1 hour * 60min * 60sec
#define UV_2_HOUR          72000UL      // 2 hour * 60min * 60sec
#define UV_4_HOUR          144000UL     // 4 hour * 60min * 60sec
#define UV_6_HOUR          216000UL     // 6 hour * 60min * 60sec
#define UV_24_HOUR         864990UL     // 24 hour * 60min * 60sec

// ?��?�� 최�?? ?��?�� ?��?�� 
//#define LIMIT_COUNT_ICE_DOOR      3
//#define LIMIT_COUNT_WATER_OUT     3
//#define LIMIT_COUNT_ICE_TANK      3
#define LIMIT_COUNT_ICE_TRAY      6

typedef struct _uv_info_
{
    U32 ResetTimer;

    U32 OutRegularTimer;    // 추출�? ?���? ?���?(?��?��/�?)
    U32 IceRegularTimer;    // ?��?��?�� ?��?��
}UvInfo_T;
UvInfo_T UvInfo;


static void InitUvLimitCount(void);
void InitUv(void)
{
    UvInfo.ResetTimer       = UV_24_HOUR;
    UvInfo.OutRegularTimer  = UV_6_HOUR;
    UvInfo.IceRegularTimer  = UV_6_HOUR;        

    // CONFIG UV LIMITED
    SetLimitConfUvId( UV_ICE_DOOR,  UNLIMITED );
    SetLimitConfUvId( UV_WATER_OUT, UNLIMITED );
    SetLimitConfUvId( UV_ICE_TANK,  UNLIMITED );
    SetLimitConfUvId( UV_ICE_TRAY,  LIMITED );

    SetLimitConfUvId( UV_ICE_DOOR_SHOT,  UNLIMITED );
    SetLimitConfUvId( UV_WATER_OUT_SHOT, UNLIMITED );
    SetLimitConfUvId( UV_ICE_TRAY_SHOT,  UNLIMITED );

    // CONFIG UV LIMITED NUM
    InitUvLimitCount();
}

static void InitUvLimitCount(void)
{
    //SetLimitCountUvId( UV_ICE_DOOR,  LIMIT_COUNT_ICE_DOOR );
    //SetLimitCountUvId( UV_WATER_OUT, LIMIT_COUNT_WATER_OUT );
    //SetLimitCountUvId( UV_ICE_TANK,  LIMIT_COUNT_ICE_TANK );
    SetLimitCountUvId( UV_ICE_TRAY,  LIMIT_COUNT_ICE_TRAY );
}

static void ResetLimitCount(void)
{
    if( UvInfo.ResetTimer != 0 )
    {
        UvInfo.ResetTimer--;
    }
    else
    {
        UvInfo.ResetTimer = UV_24_HOUR;

        InitUvLimitCount();
    }
}


typedef struct _uv_ctrl_
{
    UvId_T  Id;
    U32 OffTime;
    U32 OnTime;

    U8 PrevStatus;
    U8 CurrentStatus;
    U8 TriggerStatus;
    U8 (*GetStatus)(void);
} UvCtrl_T;

static U8 IsWaterOut(void);
static U8 IsIceOut(void);
UvCtrl_T    CtrlList[] = 
{
    { UV_WATER_OUT_SHOT, UV_3_SEC, UV_7_SEC,  0, 0, FALSE, IsWaterOut },
    { UV_ICE_DOOR_SHOT,  UV_3_SEC, UV_7_SEC,  0, 0, FALSE, IsIceOut },
};
#define SZ_CTRL_LIST    ( sizeof(CtrlList)/sizeof(UvCtrl_T))


void CheckUv(void)
{
    U8 i;
    UvCtrl_T    *pItem;

    for( i = 0; i < SZ_CTRL_LIST; i++ )
    {
        pItem = &CtrlList[ i ];

        pItem->CurrentStatus = pItem->GetStatus();
        if( pItem->PrevStatus != pItem->CurrentStatus )
        {
            if( pItem->CurrentStatus == pItem->TriggerStatus )
            {
                TurnOnUvId( pItem->Id, pItem->OffTime, pItem->OnTime );
            }
            else
            {
                TurnOffUvId( pItem->Id );
            }

            pItem->PrevStatus = pItem->CurrentStatus;
        }
    }
}


static U8 IsWaterOut(void)
{
    if( GetWaterOut() == TRUE )
    {
        if( IsOpenValve( VALVE_ROOM_COLD_OUT ) == TRUE 
                || IsOpenValve( VALVE_ROOM_COLD_OUT) == TRUE 
                || IsOpenValve( VALVE_HOT_OUT ) == TRUE )
        {
            return TRUE;
        }
    }

    return FALSE;
}

static U8 IsIceOut(void)
{
    return IsOpenIceDoor();
}


// ?��?�� 추출, �? 추출�? ?���? ?���?
static void CheckRegularOutUv(void)
{
    if( UvInfo.OutRegularTimer != 0 )
    {
        UvInfo.OutRegularTimer--;
    }
    else
    {
        UvInfo.OutRegularTimer = UV_6_HOUR; // + UV_30_MIN;

        TurnOnUvId( UV_WATER_OUT, 0, UV_30_MIN );
        TurnOnUvId( UV_ICE_DOOR,  0, UV_30_MIN );
    }
}

// ?��?�� ?��?�� ?���? ?���?
static void CheckRegularIceUv(void)
{
    if( UvInfo.IceRegularTimer != 0 )
    {
        UvInfo.IceRegularTimer--;
    }
    else
    {
        UvInfo.IceRegularTimer = UV_6_HOUR + UV_2_HOUR;

        TurnOnUvId( UV_ICE_TANK, 0, UV_2_HOUR );
    }
}

// ?��?��?�� ?��?��?�� ?���? ?���?
// 최초 만빙?��, 1?���? ?���? ?��?��
// ?��?��?�� ?��?��?�� ?��치�?? ?��?��?���? 취소
#if 0
static void CheckRegularIceTrayUv(void)
{
    static U8 mu8CheckAction = FALSE;


    // 취침 ?��?�� 중이 ?��?���? 만빙?�� ?��까�?? ???�? ?��?���? ?��?��
    mu8CheckAction = GetAlaramByNoneRTC( mu8CheckAction );
    if( mu8CheckAction == TRUE )
    {
        if( GetSavingStatus() == TRUE )
        {
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                mu8CheckAction = FALSE;
                TurnOnUvId( UV_ICE_TRAY, 0, UV_1_HOUR );
            }
        }
        else 
        {
            if( GetIceFullStatus() == TRUE )
            {
                mu8CheckAction = FALSE;
                TurnOnUvId( UV_ICE_TRAY, 0, UV_1_HOUR );
            }
        }
    }

    // ?��?��?���? ?���? ?��치�?? ?��?���? 중�??
    if( GetIceTrayStatus() != ICE_TRAY_DEICING )
    {
        TurnOffUvId( UV_ICE_TRAY );
    }
}
#else
// ?��?��?�� ?��?��?�� ?���? ?���?
// 만빙?��, 1?���? ?���? ?��?��
// ?��?��?�� ?��?��?�� ?��치�?? ?��?��?���? 취소
static void CheckRegularIceTrayUv(void)
{
    static U8 mu8Prev = FALSE;
    U8 mu8Cur;


    mu8Cur = GetIceFullStatus();
    if( mu8Prev != mu8Cur )
    {
        mu8Prev = mu8Cur;
        if( mu8Cur == TRUE )
        {
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                TurnOnUvId( UV_ICE_TRAY, 0, UV_1_HOUR );
            }
        }
    }

    // ?��?��?���? ?���? ?��치�?? ?��?���? 중�??
    if( GetIceTrayStatus() != ICE_TRAY_DEICING )
    {
        TurnOffUvId( UV_ICE_TRAY );
    }
}
#endif

static void CheckPauseUv(void)
{
    U8 mu8Pause = FALSE;


    // 1. ICE TANK
    if( IsIceOut() == TRUE )
    {
        mu8Pause = TRUE;
    }

    if( HAL_GetInputValue( IN_TANK_OPEN_UV ) == HIGH )
    {
        mu8Pause = TRUE;
    }

    SetPauseUvId( UV_ICE_TANK, mu8Pause );
    SetPauseUvId( UV_ICE_TRAY, mu8Pause );
    SetPauseUvId( UV_ICE_TRAY_SHOT, mu8Pause );


    // 2. WATER OUT 
    if( IsWaterOut() == TRUE )
    {
        SetPauseUvId( UV_WATER_OUT, TRUE );
    }
    else
    {
        SetPauseUvId( UV_WATER_OUT, FALSE );
    }

    // 3. ICE OUT 
    if( IsIceOut() == TRUE )
    {
        SetPauseUvId( UV_ICE_DOOR, TRUE );
    }
    else
    {
        SetPauseUvId( UV_ICE_DOOR, FALSE );
    }
}

static void CheckConfigUv(void)
{
    if( GetIceConfigMake() == FALSE )
    {
        SetConfigUvId( UV_ICE_TANK,         FALSE );
        SetConfigUvId( UV_ICE_TRAY,         FALSE );
        SetConfigUvId( UV_ICE_TRAY_SHOT,    FALSE );
        SetConfigUvId( UV_ICE_DOOR,         FALSE );
    }
    else
    {
        SetConfigUvId( UV_ICE_TANK,         TRUE );
        SetConfigUvId( UV_ICE_TRAY,         TRUE );
        SetConfigUvId( UV_ICE_TRAY_SHOT,    TRUE );
        SetConfigUvId( UV_ICE_DOOR,         TRUE );
    }
}

/////////////////////////////////////////////////////////////////////////
// UV ?��?�� ?��?�� ?��?��?�� 코드 
////////////////////////////////////////////////////////////////////////
#if CONFIG_UV_MANUAL
U8 dbg_uv_ice_door  = 0;        
U8 dbg_uv_water_out = 0;
U8 dbg_uv_ice_tray  = 0;
U8 dbg_uv_ice_tank  = 0;
static void TestUvManual(void)
{
    if( dbg_uv_ice_door != 0 )
    {
        HAL_TurnOnUv ( HAL_UV_ICE_DOOR );
    }
    else
    {
        HAL_TurnOffUv ( HAL_UV_ICE_DOOR );
    }

    if( dbg_uv_water_out != 0 )
    {
        HAL_TurnOnUv ( HAL_UV_WATER_OUT );
    }
    else
    {
        HAL_TurnOffUv ( HAL_UV_WATER_OUT );
    }

    if( dbg_uv_ice_tray != 0 )
    {
        HAL_TurnOnUv ( HAL_UV_ICE_TANK );
    }
    else
    {
        HAL_TurnOffUv ( HAL_UV_ICE_TANK );
    }

    if( dbg_uv_ice_tank != 0 )
    {
        HAL_TurnOnUv ( HAL_UV_ICE_TRAY );
    }
    else
    {
        HAL_TurnOffUv ( HAL_UV_ICE_TRAY );
    }
}
#endif

void ProcessUv(void)
{
#if CONFIG_UV_MANUAL
    TestUvManual();
#else

    CheckConfigUv();
    CheckUv();
    CheckRegularOutUv();
    CheckRegularIceUv();
    CheckRegularIceTrayUv();
    CheckPauseUv();

    ControlUv();

    ResetLimitCount();
#endif
}
