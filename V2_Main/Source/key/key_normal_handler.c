#include "hw.h"
#include "key_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "front.h"
#include "hot_water.h"
#include "cold_water.h"
#include "water_out.h"
#include "tank_clean.h"
#include "ice.h"
#include "ice_maker.h"
#include "ice_door.h"
#include "ice_tray.h"
#include "power_saving.h"
#include "sound.h"
#include "eeprom.h"
#include "error.h"
#include "display_lib.h"
#include "display_clock.h"
#include "process_display.h"
#include "rtc.h"
#include "eol.h"
#include "fct.h"
#include "time_short.h"
#include "timer.h"
#include "timer_id.h"
#include "drain.h"
#include "health.h"
#include "drain_water.h"
#include "drain_pump.h"
#include "flush_water_out.h"
#include "leak.h"
#include "filter.h"
#include "flow_meter.h"
#include "voice.h"
#include "cody_kit.h"
#include "empty_cold_tank.h"
#include "flush_water_out.h"
#include "service.h"
#include "ice_tray_test.h"
#include "key_event_lib.h"
#include "WIFI_Control.h"

#if CONFIG_TEST
#include "comp.h"
#endif

extern U8 GetTempConfigList(U8 ConfigNum);

static void DispOnOff(U8 mu8OnOff);

static U8 SelSound(void);

/* ?�수 버튼 */
static U8 SelHot(void);

/* ?�수 버튼 */
static U8 SelCold(void);
static U8 OnColdMake(void);
static U8 OffColdMake(void);
static U8 OnOffColdMake(void);
static U8 OnOffEmptyColdFaucet(void);
static U8 OnOffEmptyColdDrain(void);


/* ?�수 버튼 */
static U8 SelRoom(void);
#if 0   // unsued
static U8 OnDecoLed(void);
static U8 OffDecoLed(void);
#endif
static U8 ClearMemento(void);

/* ?�속 추출 */
static U8 SelContyInfinity(void);

/* ?�정 버튼 */
static U8 SelSetting(void);
static U8 OnSetting(void);
#if CONFIG_GAS_FILLING
static U8 SelGasFiling(void);
#endif

/* 물양 ?�택 */
static U8 SelAmount(void);
#if 0
static U8 SelAmountEx(void);
#endif
static U8 ConfigAmount(void);


/* ?�빙 ?�정/?�제 */
static U8 OnIceMake(void);
static U8 OffIceMake(void);
static U8 OnOffIceMake(void);

static U8 SelIce(void);

/* ?�빙 ?�선 */
static U8 OnOffIceTurbo(void);

/* ?�음 ?�금 버튼 */
static U8 OnOffIceLock(void);

static U8 SelIceSize(void);

/* ?�수 ?�금 버튼 */
static U8 OnOffHotLock(void);

//static U8 OnOffHotUser(void);
static U8 OnOffHotTest(void);

/* ?�전 버튼 */
static U8 OnOffSave(void);

static U8 SettingSound(void);
static U8 SettingCustomer(void);
static U8 SettingClock(void);
static U8 SettingClockSave(void);
static U8 SelCancelFlush(void);
static U8 CancelFlush(void);

/* ?�용??물양 ?�정 */
//static U8 OnOffUserAmount(void);

//static U8 OnVersion(void);
static U8 OnOffMemento(void);

static U8 OnLockAll(void);
static U8 OffLockAll(void);
//static U8 OnOffLockAll(void);


//static U8 OnOffMute(void);
static U8 OnOffCodyKit(void);
static U8 OnTestIceTray(void);

/* ?�수 미사???�전 */
static U8 OnOffUnusedSave(void);

/* 강제 ?�빙 */
static U8 OnDeIcing(void);
static U8 OnManualDrain(void);

/* EEPROM 초기??*/
static U8 OnEraseEeprom(void);

/* WIFI */
static U8 OnWifiAp(void);
static U8 OnWifiBle(void);
static U8 OnWifiOnOff(void);

/* EOL */
static U8 OnEolStart(void);
static U8 OnFrontStart(void);
static U8 OnFctStart(void);
static U8 OnTimeShort(void);

static U8 OnTestShortCompTime(void);

/* UV TEST CODE */
static U8 OnOffUvIceTank(void);
static U8 OnOffUvIceTray(void);
static U8 OnOffUvIceOut(void);
static U8 OnOffUvWaterOut(void);

static U8 KeyContDownEx(void);
static U8 KeyContUpEx(void);
static U8 SelHotTest(void);


// ?�반 모드 리스??
KeyEventList_T KeyEventList[] =
{
    /* KEY,            Short,            2sec,           3sec,               5sec,           Pop,           TS */
    /* SINGLE KEY */
#if CONFIG_UV_MANUAL
    { K_HOT,            SelHot,          NULL,           NULL,           NULL,          NULL,           OnOffUvIceOut    },
    { K_ROOM,           SelRoom,         NULL,           NULL,           NULL,          NULL,           OnOffUvIceTank   },
    { K_COLD,           SelCold,         NULL,           NULL,           NULL,          NULL,           OnOffUvIceTray   },
    { K_AMOUNT,         SelAmount,       NULL,           ConfigAmount,   NULL,          NULL,           OnOffUvWaterOut  },
#else
    { K_HOT,            SelHot,          NULL,           OnOffHotLock,   NULL,          KeyStopCont,    KeyContDownEx },
    { K_ROOM,           SelRoom,         NULL,           OnWifiBle,      NULL,          NULL,           NULL },
    { K_COLD,           SelCold,         NULL,           OnColdMake,     OffColdMake,   KeyStopCont,    KeyContUpEx },
    { K_AMOUNT,         SelAmount,       NULL,           ConfigAmount,   NULL,          NULL,           NULL },
#endif  /* CONFIG_UV_MANUAL */

#if CONFIG_GAS_FILLING
    { K_SETTING,        SelSetting,      SelGasFiling,   NULL,               NULL,           NULL,          NULL },
#elif CONFIG_TEST_HOT_OUT
    { K_SETTING,        SelHotTest,      NULL,          NULL,               NULL,           NULL,          NULL },
#else
    { K_SETTING,        SelSetting,      NULL,            OnSetting,         NULL,           NULL,          NULL },
#endif  /* CONFIG_GAS_FILLING */

    { K_LOCK_ALL,       OnLockAll,       NULL,             OffLockAll,       NULL,           NULL,          NULL },
    { K_ICE_LOCK,       OnOffIceLock,    NULL,             NULL,             NULL,           NULL,          NULL },
    { K_ICE_MAKE,       OnOffIceMake,    NULL,             NULL,             NULL,           NULL,          NULL },
    { K_HOT_LOCK,       OnOffHotLock,    NULL,             NULL,             NULL,           NULL,          NULL },
    { K_SAVE,           OnOffSave,       NULL,             NULL,             NULL,           NULL,          NULL },
    { K_COLD_MAKE,      OnOffColdMake,   NULL,             NULL,             NULL,           NULL,          NULL },
    
    { K_ICE,            SelIce,          NULL,             OnIceMake,        OffIceMake,     NULL,          NULL },
#if CONFIG_UV_MANUAL
    { K_CP_ICE_TURBO,   OnOffIceTurbo,   NULL,             NULL,             NULL,           NULL,          OnOffUvIceOut },
#else
    { K_CP_ICE_TURBO,   OnOffIceTurbo,   NULL,             NULL,             NULL,           KeyStopCont,   KeyContDownEx },
#endif
    { K_CP_ICE_SIZE,    SelIceSize,      NULL,             OnIceMake,        OffIceMake,     NULL,          NULL },
    { K_CP_ICE_MAKE,    OnOffIceMake,    NULL,             NULL,             NULL,           NULL,          NULL },

    /* MULTI KEY */
    //{ K_HOT_USER,           SelSound,    NULL,             OnOffHotUser,     NULL,           NULL,          NULL },
#if  CONFIG_TEST_HOT_OUT 
    { K_HOT_TEST,           SelSound,    NULL,             OnOffHotTest,     NULL,           NULL,          NULL },
#endif
    { K_SETTING_SOUND,      SelSound,    NULL,             SettingSound,     NULL,           NULL,          NULL },
    { K_SETTING_CUSTOMER,   SelSound,    NULL,             SettingCustomer,  NULL,           NULL,          NULL },
    { K_SETTING_CLOCK,      SelSound,    NULL,             SettingClock,     NULL,           NULL,          NULL },
    { K_SETTING_CLOCK_CP,   SelSound,    NULL,             SettingClock,     NULL,           NULL,          NULL },
    { K_CANCEL_FLUSH,       SelCancelFlush,    NULL,       CancelFlush,      NULL,           NULL,          NULL },
    { K_CANCEL_FLUSH_CP,    SelCancelFlush,    NULL,       CancelFlush,      NULL,           NULL,          NULL },

    { K_EMPTY_COLD_DRAIN,    SelSound,    NULL,             OnOffEmptyColdDrain,   NULL,    NULL,          NULL },
    { K_EMPTY_COLD_DRAIN_CP, SelSound,    NULL,             OnOffEmptyColdDrain,   NULL,    NULL,          NULL },
    { K_EMPTY_COLD,          SelSound,    NULL,             OnOffEmptyColdFaucet,  NULL,    NULL,          NULL },
    { K_EMPTY_COLD_CP,       SelSound,    NULL,             OnOffEmptyColdFaucet,  NULL,    NULL,          NULL },
    { K_DRAIN_COLD,          SelSound,    NULL,             OnManualDrain,   NULL,           NULL,          NULL },
    { K_DRAIN_COLD_CP,       SelSound,    NULL,             OnManualDrain,   NULL,           NULL,          NULL },
    { K_DEICING,             SelSound,    OnDeIcing,        NULL,             NULL,           NULL,          NULL },
    //{ K_MANUAL_DRAIN,       SelSound,    OnManualDrain,    NULL,             NULL,           NULL,          NULL },

    { K_WIFI_AP,            SelSound,    NULL,             OnWifiAp,         NULL,           NULL,          NULL },
    { K_WIFI_ONOFF,         SelSound,    NULL,             OnWifiOnOff,      NULL,           NULL,          NULL },

    { K_CODY_KIT,           SelSound,    OnOffCodyKit,     NULL,             NULL,           NULL,          NULL },
    { K_CODY_KIT_CP,        SelSound,    OnOffCodyKit,     NULL,             NULL,           NULL,          NULL },

    { K_TEST_ICE_TRAY,      SelSound,    OnTestIceTray,    NULL,             NULL,           NULL,          NULL },
    { K_TEST_ICE_TRAY_CP,   SelSound,    OnTestIceTray,    NULL,             NULL,           NULL,          NULL },

    /* TEST */
#if CONFIG_TEST
    { K_TEST_COMP,      SelSound,     OnTestShortCompTime,        NULL, NULL, NULL, NULL },
#endif
    { K_TEST_TS,        SelSound,      OnTimeShort,               NULL, NULL, NULL, NULL },
    { K_TEST_FCT,       SelSound,      OnFctStart,                NULL, NULL, NULL, NULL },
    { K_TEST_FRONT,     SelSound,      OnFrontStart,              NULL, NULL, NULL, NULL },
    { K_TEST_EOL,       SelSound,      OnEolStart,                NULL, NULL, NULL, NULL },
    { K_TEST_EOL_CP,    SelSound,      OnEolStart,                NULL, NULL, NULL, NULL }
};

// ?�스??모드(TIME SHORT) 리스??
KeyEventList_T KeyEventTimeShortList[] =
{
    /* KEY,            Short,            2sec,  3sec,  5sec,  Pop,  TS */
    /* SINGLE KEY */
    { K_HOT,            SelHot,          NULL,  NULL,  NULL,  NULL, NULL },
    { K_ROOM,           SelRoom,         NULL,  NULL,  NULL,  NULL, OnWifiBle },
    { K_COLD,           SelCold,         NULL,  NULL,  NULL,  NULL, OnOffColdMake },
    { K_AMOUNT,         SelAmount,       NULL,  NULL,  NULL,  NULL, ConfigAmount },
    { K_LOCK_ALL,       OnLockAll,       NULL,  OffLockAll,  NULL,  NULL, NULL },

    { K_SETTING,        SelSetting,      NULL,  NULL,  NULL,  NULL, OnSetting },
    { K_ICE_LOCK,       OnOffIceLock,    NULL,  NULL,  NULL,  NULL, NULL },
    { K_ICE_MAKE,       OnOffIceMake,    NULL,  NULL,  NULL,  NULL, NULL },
    { K_HOT_LOCK,       OnOffHotLock,    NULL,  NULL,  NULL,  NULL, NULL },
    { K_SAVE,           OnOffSave,       NULL,  NULL,  NULL,  NULL, NULL },
    { K_COLD_MAKE,      OnOffColdMake,   NULL,  NULL,  NULL,  NULL, NULL },

    { K_ICE,            SelIce,          NULL,  NULL,  NULL,  NULL, NULL },
    { K_CP_ICE_TURBO,   OnOffIceTurbo,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_CP_ICE_SIZE,    SelIceSize,      NULL,  NULL,  NULL,  NULL, NULL },
    { K_CP_ICE_MAKE,    OnOffIceMake,    NULL,  NULL,  NULL,  NULL, NULL },

    /* MULTI KEY */
    { K_SETTING_SOUND,      SelSound,    NULL,  NULL,  NULL,  NULL, SettingSound     },
    { K_SETTING_CUSTOMER,   SelSound,    NULL,  NULL,  NULL,  NULL, SettingCustomer  },

    { K_DEICING,            SelSound,    NULL,  NULL,  NULL,  NULL, OnDeIcing      },
    //{ K_MANUAL_DRAIN,       SelSound,    NULL,  NULL,  NULL,  NULL, OnManualDrain  },

    { K_WIFI_AP,            SelSound,    NULL,  NULL,  NULL,  NULL, OnWifiAp       },
    { K_WIFI_ONOFF,         SelSound,    NULL,  NULL,  NULL,  NULL, OnWifiOnOff    },

    { K_TEST_COMP,          SelSound,    NULL,  NULL,  NULL,  NULL, OnTestShortCompTime },

    //{ K_VERSION,            SelSound,    NULL,  NULL,  NULL,  NULL, OnVersion     },
    { K_MEMENTO,            SelSound,    NULL,  NULL,  NULL,  NULL, OnOffMemento  },
};

// ?�스??모드(TIME SHORT - 메멘??) 리스??
KeyEventList_T KeyEventMementoList[] =
{
    /* KEY,            Short,            2sec,  3sec,  5sec,  Pop,  TS */
    /* SINGLE KEY */
    { K_SETTING,       NULL,         NULL,  NULL,  NULL,  NULL, OnOffMemento },
    { K_ROOM,          NULL,         NULL,  NULL,  NULL,  NULL, ClearMemento },
};

static U8 IsValidSoundMenu(U32 mu32Key)
{
    if( GetSoundDisp() == FALSE )
    {
        return TRUE;
    }

    if( GetSystem() == SYSTEM_CHP )
    {
        if( mu32Key != K_COLD
                && mu32Key != K_HOT
                && mu32Key != K_SETTING_SOUND 
          )
        {
            return FALSE;
        }
    }
    else
    {
        if( mu32Key != K_COLD
                && mu32Key != K_CP_ICE_TURBO
                && mu32Key != K_SETTING_SOUND 
          )
        {
            return FALSE;
        }
    }

    return TRUE;
}

static U8 IsValidClockMenu(U32 mu32Key)
{
    /* ?�간 ?�정 메뉴 */
    if( GetClockDisp() == TRUE )
    {
        if( GetSystem() == SYSTEM_CHP )
        {
            if( mu32Key != K_SETTING 
                    && mu32Key != K_HOT 
                    && mu32Key != K_COLD 
              )
            {
                return FALSE;
            }
        }
        else
        {
            if( mu32Key != K_SETTING 
                    && mu32Key != K_CP_ICE_TURBO
                    && mu32Key != K_COLD 
              )
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

U8 IsValidNormalKeyCondition(U32 mu32Key)
{
    /* 배수 중�? */
    if( ResetDrainWater() == TRUE )
    {
        Sound( SOUND_CANCEL );
        return FALSE;
    }

    /* ?�간 ?�정 */
    if( IsValidClockMenu( mu32Key ) == FALSE )
    {
        return FALSE;
    }

    /* ?�수 배수 - FAUCET */
    if( IsStartEmptyColdTank() == TRUE 
        && GetEmptyColdTankType() == TYPE_FAUCET )
    {
        if( mu32Key != K_EMPTY_COLD 
                && mu32Key != K_EMPTY_COLD_CP )
        {
            return FALSE;
        }
    }

    /* ?�수 배수 - DRAIN */
    if( IsStartEmptyColdTank() == TRUE 
        && GetEmptyColdTankType() == TYPE_DRAIN )
    {
        if( mu32Key != K_EMPTY_COLD_DRAIN 
                && mu32Key != K_EMPTY_COLD_DRAIN_CP )
        {
            return FALSE;
        }
    }


    // ?�운???�정 메뉴
    if( IsValidSoundMenu( mu32Key ) == FALSE )
    {
        return FALSE;
    }

    // 관�?모드 변�?
    if( GetCustomerDisp() == TRUE )
    {
        if( mu32Key != K_SETTING_CUSTOMER ) 
        {
            return FALSE;
        }
    }

    /* ?�터 커버 open ??*/
    if( GetFilterDisp() == TRUE )
    {
        if( mu32Key != K_CODY_KIT 
                && mu32Key != K_CODY_KIT_CP )
        {
            return FALSE;
        }
    }

    /* ?�체 ?�금 ?�태 */
    if( GetLockAll() == LOCK )
    {
        if( mu32Key != K_LOCK_ALL )
        {
            return FALSE;
        }
    }

    /* 초기 ?�러?��? ?�료 ?�태?�면 ?�스?�만 ?�용*/
    if( IsDoneFlushInitSetup() == FALSE )
    {
        if( mu32Key != K_TEST_FCT 
                && mu32Key != K_TEST_FRONT
                && mu32Key != K_TEST_EOL
                && mu32Key != K_TEST_EOL_CP
                && mu32Key != K_CANCEL_FLUSH
                && mu32Key != K_CANCEL_FLUSH_CP
                && mu32Key != K_COLD
                && mu32Key != K_LOCK_ALL 
                && mu32Key != K_ROOM 
                && mu32Key != K_WIFI_AP 
                && mu32Key != K_WIFI_ONOFF 
          )
        {
            return FALSE;
        }
    }

    /* SETTING MENU ?�서�??�용 */
    if( GetSettingDisp() == TRUE )
    {
        if(  mu32Key != K_ICE_LOCK
                && mu32Key != K_HOT_LOCK
                && mu32Key != K_COLD_MAKE
                && mu32Key != K_ICE_MAKE
                && mu32Key != K_SAVE
                && mu32Key != K_SETTING
                && mu32Key != K_LOCK_ALL 
                && mu32Key != K_CP_ICE_MAKE
          )
        {
            return FALSE;
        }
        
        // UPDATE SETTING MENU TIMER..
        StartDispTimerId( DISP_TIMER_SETTING_MENU );
    }
    else
    {
        /* SETTING MENU - ?�정 ?�태가 ?�닐 ??SKip */
        if( mu32Key == K_ICE_LOCK
                || mu32Key == K_HOT_LOCK
                || ( mu32Key == K_LOCK_ALL && GetLockAll() == UNLOCK )
                || mu32Key == K_COLD_MAKE
                || mu32Key == K_ICE_MAKE
                || mu32Key == K_SAVE
                || mu32Key == K_CP_ICE_MAKE
          )
        {
            return FALSE;
        }
    }

    /* MEMENTO DISPLAY */
    if( GetMementoDisp() == TRUE )
    {
        if( mu32Key != K_SETTING 
                && mu32Key != K_ROOM )
        {
            return FALSE;
        }
    }

#if 0
    /* ?�체 ?�금 - ?�제 버튼�??�용 */
    if( GetLockAll() == LOCK )
    {
        // CHP 모델?� ?�정-?�체?�금
        if( mu32Key != K_LOCK_ALL 
                && mu32Key != K_SETTING )
        {
            StartDispTimerId( DISP_TIMER_ALARM_SETTING );
            Sound( SOUND_ALARM_LOCK_ALL ); 
            return FALSE;
        }
    }
#endif

    /* �?추출 �?*/
    if( GetWaterOut() == TRUE 
            && GetWaterOutSelect() == SEL_WATER_CODY )
    {
        if( mu32Key != K_CODY_KIT 
            || mu32Key != K_CODY_KIT_CP )
        {
            Sound( SOUND_ERROR );
            return FALSE;
        }
    }

#if 0
    if( GetWaterOut() == TRUE 
            && GetWaterOutSelect() != SEL_WATER_CODY )
    {
        if( GetWaterOutSelect() != SEL_WATER_FLUSH )
        {
            Sound( SOUND_ERROR );
        }
        return FALSE;
    }
#else
    if( GetWaterOut() == TRUE 
            && GetWaterOutSelect() != SEL_WATER_CODY )
    {
        if( GetWaterOutSelect() != SEL_WATER_FLUSH )
        {
            // ?�온??추출... ?�러부?�??
            Sound( SOUND_ERROR );   
        }
        else
        {
            // ?�러??추출 �?.
            if( mu32Key == K_WIFI_AP 
                    || mu32Key == K_WIFI_ONOFF 
                    || mu32Key == K_ROOM )
            {
                return TRUE;    // wifi ?�어�??�력?� ?�용
            }
        }

        return FALSE;   // ?�반 추출?�면 FALSE
    }
#endif

    /* ?�음 추출 �?*/
    if( GetIceDoorStatus() == ICE_DOOR_OPEN 
            || GetIceDoorStatus() == ICE_DOOR_OPENING )
    {
        if( mu32Key != K_AMOUNT
                && mu32Key != K_ROOM
                && mu32Key != K_COLD
                && mu32Key != K_HOT )
        {
            return FALSE;
        }
    }

    return TRUE;
}

// 모드???�라 메멘???�?�쇼?? ?�반 3가지�?반환
void* GetNormalKeyEventList(void)
{
    if( GetMementoDisp() == TRUE )
    {
        return KeyEventMementoList;
    }
    else if( GetTimeShortStatus() == TRUE )
    {
        return KeyEventTimeShortList;
    }

    return KeyEventList;
}

// 모드???�라 메멘???�?�쇼?? ?�반 3가지�?반환
U8 GetNormalKeyEventListSize(void)
{
    if( GetMementoDisp() == TRUE )
    {
        return ( sizeof(KeyEventMementoList) / sizeof(KeyEventList_T) );
    }
    else if( GetTimeShortStatus() == TRUE )
    {
        return ( sizeof(KeyEventTimeShortList) / sizeof(KeyEventList_T) );
    }

    return ( sizeof(KeyEventList) / sizeof(KeyEventList_T) );
}



static U8 SetSelectWater( U8 mu8Id )
{
//    // 추출 중에?? �??�택???????�다.
//    if( GetWaterOut() == TRUE )
//    {
//        return FALSE;   // error..
//    }

    SetWaterOutSelect( mu8Id );
    return TRUE;
}

static void DispOnOff(U8 mu8OnOff)
{
    if( mu8OnOff == ON )
    {
        StartDispTimerId( DISP_TIMER_ON );
        StopDispTimerId( DISP_TIMER_OFF );
    }
    else
    {
        StartDispTimerId( DISP_TIMER_OFF );
        StopDispTimerId( DISP_TIMER_ON );
    }
}

static U8 SelSound(void)
{
    return SOUND_SELECT;
}

// ?�간 ?�정- ??�� 변�?
static U8 OnClockItem(void)
{
    if( GetClockDisp() == TRUE )
    {
        NextClock();
        StartDispTimerId( DISP_TIMER_NEXT );
        StartDispTimerId( DISP_TIMER_CLOCK_MENU );
        return TRUE;
    }

    return FALSE;
}

// ?�간 ?�정- 증�?
static U8 OnClockUp(void)
{
    if( GetClockDisp() == TRUE )
    {
        UpClock();
        StartDispTimerId( DISP_TIMER_UP );
        StartDispTimerId( DISP_TIMER_CLOCK_MENU );
        return TRUE;
    }

    return FALSE;
}

// ?�간 ?�정- 감소
static U8 OnClockDown(void)
{
    // ?�운??볼륨 조정
    if( GetClockDisp() == TRUE )
    {
        DownClock();

        StartDispTimerId( DISP_TIMER_DOWN );
        StartDispTimerId( DISP_TIMER_CLOCK_MENU );
        return TRUE;
    }

    return FALSE;
}

// ?�간 ?�정 메뉴 진입
static U8 SettingClock(void)
{
    if( GetClockDisp() == FALSE )
    {
        if( GetRtcConfigInit() == FALSE )
        {
            InitClock( TRUE );
        }
        else
        {
            InitClock( FALSE );
        }

        InitContKey( DEFAULT_CONT_TICK ); 
        SetSettingDisp( FALSE );    // ?�정 진입?� ?�제 
        SetClockDisp( TRUE );
    }

    StartDispTimerId( DISP_TIMER_CLOCK_MENU );
    return SOUND_SETUP;
}

static U8 SettingClockSave(void)
{
    SaveClock();
    SetClockDisp( FALSE );

    StartDispTimerId( DISP_TIMER_SETTING_CLOCK );
    return SOUND_SETUP;
}

void ContySettingClockKey(void)
{
    ControlContKey( OnClockDown, OnClockUp );
}

static U8 SelCancelFlush(void)
{
    if( IsExpiredTestKeyLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    return SOUND_SELECT;
}

// ?�러??강제 취소
static U8 CancelFlush(void)
{
    if( IsExpiredTestKeyLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }


    SetFlushSetupInit( FLUSH_STATUS_DONE );
    SetFlushStatus( FLUSH_STATUS_DONE );
    SetFlushPowerOn( FLUSH_STATUS_DONE );
    SaveEepromId( EEP_ID_FLUSH_STATUS );
    SaveEepromId( EEP_ID_FLUSH_SETUP_INIT );

    SetFilterChangedStatus( FILTER_MOUNTED );
    SaveEepromId( EEP_ID_FILTER_CHANGED );

    SetColdWaterInitFull( TRUE );
    SetFotaReboot( FALSE );
    SetWaterOutSelect( SEL_WATER_ROOM );

    return SOUND_SETUP;
}

static U8 OnVolumeUp(void)
{
    if( GetSoundDisp() == TRUE )
    {
        if( GetVoiceLevel() != VOICE_LEVEL_OFF )
        {
            StartDispTimerId( DISP_TIMER_UP );
            StartDispTimerId( DISP_TIMER_DOWN );
            VolumeUp();
        }

        StartDispTimerId( DISP_TIMER_SOUND_MENU );
        return TRUE;
    }

    return FALSE;
}

static U8 OnVolumeDown(void)
{
    // ?�운??볼륨 조정
    if( GetSoundDisp() == TRUE )
    {
        if( GetVoiceLevel() != VOICE_LEVEL_OFF )
        {
            StartDispTimerId( DISP_TIMER_UP );
            StartDispTimerId( DISP_TIMER_DOWN );
            VolumeDown();
        }

        StartDispTimerId( DISP_TIMER_SOUND_MENU );
        return TRUE;
    }

    return FALSE;
}

/* ?�수 버튼 */
static U8 SelHot(void)
{
    U8 mu8SelHot;

	if( OnVolumeDown() == TRUE )
    {
		if( GetVoiceLevel() == VOICE_LEVEL_VOICE_BEEP ) 
		{
        	return SOUND_SELECT;
		}
    }

    if( OnClockDown() == TRUE )
    {
		if( GetVoiceLevel() == VOICE_LEVEL_VOICE_BEEP ) 
		{
        	return SOUND_SELECT;
		}
    }    

    if( GetLockHot() == LOCK )
    {
        StartDispTimerId( DISP_TIMER_ALARM_LOCK_HOT );
		if( GetVoiceLevel() == VOICE_LEVEL_VOICE_BEEP ) 
		{
        	return SOUND_ALARM_LOCK_HOT;
		}
		else if( GetVoiceLevel() == VOICE_LEVEL_BEEP ) 
		{
        	return SOUND_ERROR;
		}
		else if( GetVoiceLevel() == VOICE_LEVEL_OFF ) 
		{
        	return SOUND_NONE;
		}
    }

    // ?�수 ?�수 ?�러 발생?�면 ?�동 ?�제???�까지 ?�수 추출 금�?
    if( GetOutageStatus( TYPE_HOT_WATER ) == FLOW_OUTAGE )
    {
    	StartDispTimerId( DISP_TIMER_ERROR );
    	return SOUND_ERROR_OUTAGE;
    }
    
    // ?�수 ?�러?�면 ?�택 금�?. ?�러 ?�시
    if( IsErrorType( ERR_TYPE_HOT_WATER_OUT ) == TRUE )
    {
        StartDispTimerId( DISP_TIMER_ERROR );
        return SOUND_ERROR;
    }

    if( GetWaterOutSelect() != SEL_WATER_HOT )
    {
		if( GetTempConfigList(HOT_WATER_CONF_NUM_FIFTH) != 0)
		{
			SetHotSelect( GetTempConfigList(HOT_WATER_CONF_NUM_FIFTH) );
		}
		else
		{
			SetHotSelect( GetTempConfigList(HOT_WATER_CONF_NUM_FOURTH) );
		}
    }
    else if( IsExpiredDispTimer( DISP_TIMER_KEY_HOT_TEMP ) == FALSE )
    {
        mu8SelHot = GetHotSelect();
        if( mu8SelHot == GetTempConfigList(HOT_WATER_CONF_NUM_FIFTH) )
        {
            SetHotSelect( GetTempConfigList(HOT_WATER_CONF_NUM_FOURTH) );
        }
        else if( mu8SelHot == GetTempConfigList(HOT_WATER_CONF_NUM_FOURTH) )
        {
            SetHotSelect( GetTempConfigList(HOT_WATER_CONF_NUM_THIRD) );
        }
        else if( mu8SelHot == GetTempConfigList(HOT_WATER_CONF_NUM_THIRD) )
        {
            SetHotSelect( GetTempConfigList(HOT_WATER_CONF_NUM_SECOND) );
        }
        else if( mu8SelHot == GetTempConfigList(HOT_WATER_CONF_NUM_SECOND) )
        {
            SetHotSelect( GetTempConfigList(HOT_WATER_CONF_NUM_FIRST) );
        }
        else if( mu8SelHot == GetTempConfigList(HOT_WATER_CONF_NUM_FIRST) )
        {
			if(GetTempConfigList(HOT_WATER_CONF_NUM_FIFTH) != 0)
			{
				SetHotSelect( GetTempConfigList(HOT_WATER_CONF_NUM_FIFTH) );
			}
			else
			{
				SetHotSelect( GetTempConfigList(HOT_WATER_CONF_NUM_FOURTH) );
			}
        }
    }

    /* ?�수 ?�택 */
    if( SetSelectWater( SEL_WATER_HOT ) == TRUE )
    {
        //StartDispTimerId( DISP_TIMER_KEY_AMOUNT );
        StartDispTimerId( DISP_TIMER_KEY_HOT_TEMP );
        StartDispTimerId( DISP_TIMER_KEY_HOT );

        return SOUND_SELECT;
    }

    return SOUND_ERROR;
}

/* ?�수 버튼 */
static U8 SelCold(void)
{
    U8 mu8Sound = SOUND_SELECT;

    // ?�터 ?�러???�계??경우 ?�터 ?�러??취소 
    if( GetFlushSetupInit() == FLUSH_STATUS_DONE
            && GetFlushStatus() == FLUSH_STATUS_INIT )
    {
        // ?�터 ?�러??진입 취소
        SetFlushStatus( FLUSH_STATUS_DONE );
        SetFlushPowerOn( FLUSH_STATUS_DONE );
        SaveEepromId( EEP_ID_FLUSH_STATUS );

        SetFilterChangedStatus( FILTER_MOUNTED );
        SaveEepromId( EEP_ID_FILTER_CHANGED );

        SetWaterOutSelect( SEL_WATER_ROOM );

        return SOUND_SELECT;
    }
    
    if( GetFlushSetupInit() == FLUSH_STATUS_INIT 
            || GetFlushPowerOn() == FLUSH_STATUS_INIT )
    {
        return SOUND_NONE;
    }

    // ?�운??볼륨 조정
    if( OnVolumeUp() == TRUE )
    {
        return SOUND_SELECT;
    }

    // ?�간 증�? 
    if( OnClockUp() == TRUE )
    {
        return SOUND_SELECT;
    }

    // ?�수 ?�러 ?�제
    ClearOutageError();

    if( IsErrorType( ERR_TYPE_COLD_WATER_OUT ) == TRUE )
    {
        StartDispTimerId( DISP_TIMER_ERROR );
        return SOUND_ERROR;
    }

    if( SetSelectWater( SEL_WATER_COLD ) == TRUE )
    {
        //StartDispTimerId( DISP_TIMER_KEY_AMOUNT );
        StopDispTimerId( DISP_TIMER_KEY_HOT );
        StopDispTimerId( DISP_TIMER_KEY_HOT_TEMP );
    }

    if( GetColdWaterConfigMake() == FALSE )
    {
        //mu8Sound = SOUND_ALARM_COLD_OFF;
        StartDispTimerId( DISP_TIMER_ALARM_MAKE_OFF_COLD );
		if( GetVoiceLevel() == VOICE_LEVEL_VOICE_BEEP ) 
		{
        	mu8Sound = SOUND_ALARM_COLD_OFF;
		}
		else if( GetVoiceLevel() == VOICE_LEVEL_BEEP ) 
		{
        	mu8Sound = SOUND_ERROR;
		}
		else if( GetVoiceLevel() == VOICE_LEVEL_OFF ) 
		{
        	mu8Sound = SOUND_NONE;
		}
    }

    return mu8Sound;
}

static U8 OnOffEmptyColdFaucet(void)
{
    if( IsStartEmptyColdTank() == FALSE )
    {
        StartEmptyColdTank( TYPE_FAUCET );
    }
    else
    {
        StopEmptyColdTank();
    }

    return SOUND_SELECT;
}

static U8 OnOffEmptyColdDrain(void)
{
    if( IsStartEmptyColdTank() == FALSE )
    {
        StartEmptyColdTank( TYPE_DRAIN );
    }
    else
    {
        StopEmptyColdTank();
    }

    return SOUND_SELECT;
}

/* ?�수 ?�성 On */
U8 gu8ColdOff = FALSE;
static U8 OnColdMake(void)
{
    if( GetClockDisp() == TRUE
            || GetCustomerDisp() == TRUE 
            || GetSoundDisp() == TRUE 
            || GetWaterOut() == TRUE )
    {
        return SOUND_NONE;
    }

    if( GetColdWaterConfigMake() == TRUE )
    {
        return SOUND_NONE;
    }

    gu8ColdOff = TRUE;
    SetColdWaterConfigMake( TRUE );
    UpdateWaterOutReturnTime();
    return SOUND_SETUP_COLD;
}

static U8 OffColdMake(void)
{
    if( GetClockDisp() == TRUE
            || GetCustomerDisp() == TRUE 
            || GetSoundDisp() == TRUE 
            || GetWaterOut() == TRUE )
    {
        return SOUND_NONE;
    }

    if( GetColdWaterConfigMake() == FALSE )
    {
        return SOUND_NONE;
    }

    if( gu8ColdOff == TRUE )
    {
        return SOUND_NONE;
    }

    SetColdWaterConfigMake( FALSE );
    ResetWaterOutReturnTime();
    //UpdateWaterOutReturnTime();
    return SOUND_CANCEL_COLD;
}

static U8 OnOffColdMake(void)
{
    U8 mu8Sound = SOUND_NONE;

    if( GetColdWaterConfigMake() == FALSE )
    {
        SetColdWaterConfigMake( TRUE );
        mu8Sound = SOUND_SETUP_COLD;
        DispOnOff( ON );
    }
    else
    {
        SetColdWaterConfigMake( FALSE );
        mu8Sound = SOUND_CANCEL_COLD;
        DispOnOff( OFF );
    }

    return mu8Sound;
}


/* ?�수 버튼 */
static U8 SelRoom(void)
{
    if( GetWaterOut() == TRUE 
            && GetWaterOutSelect() == SEL_WATER_FLUSH )
    {
        return SOUND_NONE;
    }
    // ?�재 ?�간 ?�정 중이�?버튼 Skip
    if( GetClockDisp() == TRUE )
    {
        return SOUND_NONE;
    }

    // 메멘??
    if( GetMementoDisp() == TRUE )
    {
        return SOUND_NONE;
    }

    // ?�수 ?�러 ?�제
    ClearOutageError();

    if( IsErrorType( ERR_TYPE_ROOM_WATER_OUT ) == TRUE )
    {
        StartDispTimerId( DISP_TIMER_ERROR );
        return SOUND_ERROR;
    }

    if( SetSelectWater( SEL_WATER_ROOM ) == TRUE )
    {
        //StartDispTimerId( DISP_TIMER_KEY_AMOUNT );
        StopDispTimerId( DISP_TIMER_KEY_HOT );
        StopDispTimerId( DISP_TIMER_KEY_HOT_TEMP );
        return SOUND_SELECT;
    }
    return SOUND_ERROR;
}

#if 0 // unused
static U8 OnDecoLed(void)
{
    if( GetConfigDeco( DECO_LED_BUTTON ) == TRUE )
    {
        return SOUND_NONE;
    }

    SetConfigDeco( DECO_LED_BUTTON, TRUE );
    return SOUND_SETUP;
}

static U8 OffDecoLed(void)
{
    /* DECO LED OFF */
    if( GetConfigDeco( DECO_LED_BUTTON ) == TRUE )
    {
        SetConfigDeco( DECO_LED_BUTTON, FALSE );
        return SOUND_CANCEL;
    }

    return SOUND_NONE;
}
#endif

static U8 ClearMemento(void)
{
    if( GetMementoDisp() == TRUE )
    {
        /* MEMENTO CLEAR */
        ClearMementoError();
        
        return SOUND_SELECT;
    }

    return SOUND_NONE;
}

// 추출 �?
static U8 IsWaterOutConty(U8 mu8Out, U8 mu8Conty)
{
    // 추출 중인??
    // ?�속?� ?�속?�고,
    // ?�버 ?�력???�는 ?�태
    if( mu8Out == TRUE      
            && mu8Conty == TRUE  
            && HAL_GetLeverWater() == 0   
      )
    {
        return TRUE;    // 추출 �?
    }

    return FALSE;   // 추출 ?�음
}

// ?�반 추출 �?
static U8 IsWaterOut(U8 mu8Out, U8 mu8Conty)
{
    // 추출 중인??
    // ?�속?� ?�니�?
    // ?�버 ?�력 중인 경우
    if( mu8Out == TRUE      
            && mu8Conty == FALSE  
            && HAL_GetLeverWater() != 0   
      )
    {
        return TRUE;    // 추출 �?
    }

    return FALSE;   // 추출 ?�음
}


static U8 ChangeAmount(U8 mu8Amount)
{
    if( ( mu8Amount == FIXED_AMOUNT_HALF) || ( mu8Amount == UNFIXED_AMOUNT_HALF) )
    {
    	if(GetWaterAmountConf(UNFIXED_AMOUNT_HALF) == TRUE)
    	{
			mu8Amount = UNFIXED_AMOUNT_ONE;
    	}
		else
		{
        	mu8Amount = FIXED_AMOUNT_ONE;
		}
    }
    else if( ( mu8Amount == FIXED_AMOUNT_ONE) || ( mu8Amount == UNFIXED_AMOUNT_ONE) )
    {
    	if(GetWaterAmountConf(UNFIXED_AMOUNT_ONE) == TRUE)
    	{
			mu8Amount = UNFIXED_AMOUNT_TWO;
    	}
		else
		{
        	mu8Amount = FIXED_AMOUNT_TWO;
		}
    }
    else if( ( mu8Amount == FIXED_AMOUNT_TWO) || ( mu8Amount == UNFIXED_AMOUNT_TWO ) )
    {
        mu8Amount = FIXED_AMOUNT_FOUR;
    }
    else if( mu8Amount == FIXED_AMOUNT_FOUR )
    {
        mu8Amount = FIXED_AMOUNT_INFINITY;
    }
    else if( mu8Amount == FIXED_AMOUNT_INFINITY )
    {
    	if(GetWaterAmountConf(UNFIXED_AMOUNT_HALF) == TRUE)
    	{
			mu8Amount = UNFIXED_AMOUNT_HALF;
    	}
		else
		{
        	mu8Amount = FIXED_AMOUNT_HALF;
		}
    }
	else 
    {
    	if(GetWaterAmountConf(UNFIXED_AMOUNT_HALF) == TRUE)
    	{
			mu8Amount = UNFIXED_AMOUNT_HALF;
    	}
		else
		{
        	mu8Amount = FIXED_AMOUNT_HALF;
		}
    }
    return mu8Amount;
}

// ?�량
static U8 SelAmount(void)
{
    U8 mu8Sound = SOUND_SELECT;
    U8 mu8Amount;


    if( GetWaterOut() == TRUE )
    {
        return SOUND_ERROR;
    }


    //if( IsExpiredDispTimer( DISP_TIMER_KEY_AMOUNT ) == FALSE )
    {
        mu8Amount = GetWaterOutAmoutType();
        mu8Amount = ChangeAmount( mu8Amount );
        SetWaterOutFixedAmountType( mu8Amount );
    }

    // 물양 ?�시 ?�간 갱신
    StartDispTimerId( DISP_TIMER_KEY_AMOUNT );

    // ?�수 ?�도 ?�시 ?��? 
    if( GetWaterOutSelect() == SEL_WATER_HOT )
    {
        StartDispTimerId( DISP_TIMER_KEY_HOT );
    }
    StopDispTimerId( DISP_TIMER_KEY_HOT_TEMP );

    return mu8Sound;
}

#if 0
static U8 AmountUser(void)
{
    IncrementWaterOutAmount();

    return SOUND_SELECT;
}
#endif

#if 0
static U8 SelAmountEx(void)
{
    if( GetSettingDisp() == TRUE 
            && GetWaterOutConfigUser() == TRUE )
    {
        IncrementWaterOutAmount();
    //    UpdateDispKeyAmount();
        StartDispTimerId( DISP_TIMER_KEY_AMOUNT );

        return SOUND_SELECT;
    }

    return SelAmount();
}
#endif

static U8 ConfigAmount(void)
{
    U8 mu8Amount;

    if( GetWaterOut() == TRUE )
    {
        return SOUND_NONE;
    }

    mu8Amount = GetWaterOutAmoutType();
    if( mu8Amount != FIXED_AMOUNT_INFINITY )
    {
        SetWaterOutConfFixedAmountType( mu8Amount );
        StartDispTimerId( DISP_TIMER_CONFIG_AMOUNT );
        return SOUND_SETUP;
    }

    return SOUND_NONE;
}

/* ?�정 메뉴 ?�시 */
// ?�간 ?�정 �?: (AM,?�간,�?
// ?�정 ?�면 ?�태 : ?�정 ?�태 종료
static U8 SelSetting(void)
{
    if( GetWaterOut() == TRUE 
            || IsOpenIceDoor() == TRUE )
    {
        return SOUND_NONE;
    }

    // ?�간 ?�정 ??�� 변�?
    if( GetClockDisp() == TRUE )
    {
        if( OnClockItem() == TRUE )
        {
            return SOUND_SELECT;
        }
    }

    // ?�정 ?�면 EXIT 
    if( GetSettingDisp() == TRUE )
    {
        SetSettingDisp( FALSE );
        StopDispTimerId( DISP_TIMER_KEY_AMOUNT );
        return SOUND_CANCEL;
    }

    return SOUND_SETTING_MENU;

}


// ?�정 메뉴 진입.
// ?�간 ?�정 �?: ?�간 ?�??기능.
static U8 OnSetting(void)
{
    if( GetWaterOut() == TRUE 
            || IsOpenIceDoor() == TRUE )
    {
        return SOUND_NONE;
    }

    // ?�재 ?�간 ?�정 중이�?버튼 Skip
    if( GetClockDisp() == TRUE )
    {
        return SettingClockSave();
    }


    // ?�정 ?�면 ?�시
    SetSettingDisp( TRUE );

    StartDispTimerId( DISP_TIMER_SETTING_MENU );
    StopDispTimerId( DISP_TIMER_KEY_AMOUNT );
    StopDispTimerId( DISP_TIMER_KEY_HOT );
    StopDispTimerId( DISP_TIMER_KEY_HOT_TEMP );

    return SOUND_NONE;
}

#if CONFIG_GAS_FILLING
extern U8 filling = 0;
#include "gas_switch_valve.h"
static U8 SelGasFiling(void)
{
    filling = !filling;

    if( filling == 1 )
    {
        SetColdWaterConfigMake( FALSE );
        SetIceConfigMake( FALSE );

        GasSwitchCold();
        SetCompMode( COMP_MODE_FORCED );
        TurnOnForcedComp();

        SetCompProtectOffTime( 0 );
        SetCompBldcTargetRps( 60 );
    }
    else
    {
        SetCompMode( COMP_MODE_NONE );
        SetColdWaterConfigMake( TRUE );
        SetIceConfigMake( TRUE );
    }

    return SOUND_SETUP;
}
#endif

// ?� - ??- 빠른/?� - 빠른/??

static U8 SelIce(void)
{
    U8 mu8Turbo;
    U8 mu8Size;


    if( GetIceConfigMake() == FALSE )
    {
        StartDispTimerId( DISP_TIMER_ALARM_MAKE_OFF_ICE );
		if( GetVoiceLevel() == VOICE_LEVEL_VOICE_BEEP ) 
		{
        	return SOUND_ALARM_ICE_OFF;
		}
		else if( GetVoiceLevel() == VOICE_LEVEL_BEEP ) 
		{
        	return SOUND_ERROR;
		}
		else if( GetVoiceLevel() == VOICE_LEVEL_OFF ) 
		{
        	return SOUND_NONE;
		}
        //return SOUND_ALARM_ICE_OFF;
    }

    // ?�성??
    if( IsExpiredDispTimer( DISP_TIMER_KEY_ICE ) == TRUE )
    {
        StartDispTimerId( DISP_TIMER_KEY_ICE );
        return SOUND_SELECT;
    }

    // ?�정 변�?
    mu8Turbo = GetIceTurbo();
    mu8Size = GetIceSize();
    if( mu8Turbo == FALSE && mu8Size == ICE_LARGE )
    {
        SetIceSize( ICE_SMALL );
    }
    else if( mu8Turbo == FALSE && mu8Size == ICE_SMALL )
    {
        SetIceTurbo( TRUE );
        SetIceSize( ICE_LARGE );
    }
    else if( mu8Turbo == TRUE && mu8Size == ICE_LARGE )
    {
        SetIceSize( ICE_SMALL );
    }
    else
    {
        ClearMakingIceCount();
        SetIceTurbo( FALSE );
        SetIceSize( ICE_LARGE );
    }

    StartDispTimerId( DISP_TIMER_KEY_ICE );

    return SOUND_SELECT;
}


/* ?�빙 쾌속 ?�정 */
static U8 OnOffIceTurbo(void)
{
    if( OnVolumeDown() == TRUE )
    {
        return SOUND_SELECT;
    }

    if( OnClockDown() == TRUE )
    {
        return SOUND_SELECT;
    }

    // ?�음 off, 버튼 ?�력 불�?
    if( GetIceConfigMake() == FALSE )
    {
        StartDispTimerId( DISP_TIMER_ALARM_MAKE_OFF_ICE );
        return SOUND_ALARM_ICE_OFF;
    }
    
    if( GetIceTurbo() == TRUE )
    {
    	ClearMakingIceCount();
        SetIceTurbo( FALSE );
        return SOUND_CANCEL;
    }

    SetIceTurbo( TRUE );
    return SOUND_SETUP;
}

/* ?�빙 기능 ?�정 */
U8 gu8IceOff = FALSE;
static U8 OnIceMake(void)
{
    if( GetIceConfigMake() == TRUE )
    {
        return SOUND_NONE;
    }

    gu8IceOff = TRUE;
    SetIceConfigMake( TRUE );
    StopTimer( TIMER_ID_DRAIN_PUMP_24H );
    return SOUND_SETUP_ICE;
}

static U8 OffIceMake(void)
{
    if( GetIceConfigMake() == FALSE )
    {
        return SOUND_NONE;
    }


    if( gu8IceOff == TRUE )
    {
        return SOUND_NONE;
    }
    SetIceConfigMake( FALSE );
    // 강제 ?�어 ?�기
    ForcedCloseIceDoor();
    // ?�레???�크 배수 
    //TurnOnDrainPump();
    StartDrainPumpOnLevel();
    // 24?�간 ?? ?�레???�크 배수
    StartTimer( TIMER_ID_DRAIN_PUMP_24H, SEC(86400) );

    return SOUND_CANCEL_ICE;
}

static U8 OnOffIceMake(void)
{
    if( GetIceConfigMake() == TRUE )
    {
        DispOnOff( OFF );
        return OffIceMake();
    }

    DispOnOff( ON );
    return OnIceMake();
}


/* ?�음 ?�금 ?�정/?�제 */
static U8 OnOffIceLock(void)
{
    if( GetLockIce() == LOCK )
    {
        SetLockIce( UNLOCK );
        StopDispTimerId( DISP_TIMER_ALARM_SETTING );
        return SOUND_UNLOCK_ICE;
    }

    SetLockIce( LOCK );
    return SOUND_LOCK_ICE;
}

/* ?�음 ?�기 변�?*/
static U8 SelIceSize(void)
{
    U8 mu8Size;

    // ?�음 off?�면, 버튼 ?�력 금�?
    if( GetIceConfigMake() == FALSE )
    {
        StartDispTimerId( DISP_TIMER_ALARM_MAKE_OFF_ICE );
        return SOUND_ALARM_ICE_OFF;
    }

    // ?�성??
    if( IsExpiredDispTimer( DISP_TIMER_KEY_ICE ) == TRUE )
    {
        StartDispTimerId( DISP_TIMER_KEY_ICE );
        return SOUND_SELECT;
    }

    mu8Size = GetIceSize();
    if( mu8Size == ICE_LARGE )
    {
        mu8Size = ICE_SMALL;
    }
    else
    {
        mu8Size = ICE_LARGE;
    }

    SetIceSize( mu8Size );

    StartDispTimerId( DISP_TIMER_KEY_ICE );
    return SOUND_SELECT;
}


/* ?�수 ?�금 버튼 */
static U8 OnOffHotLock(void)
{
    if( GetClockDisp() == TRUE
            || GetCustomerDisp() == TRUE 
            || GetSoundDisp() == TRUE 
            || GetWaterOut() == TRUE )
    {
        return SOUND_NONE;
    }

    // 볼륨 ?�다???�는 ?�간 ?�정?�에??skip
    //if( OnVolumeDown() == TRUE 
    //        || OnClockDown() == TRUE )
    //{
    //    return SOUND_NONE;
    //}

    /* ?�수 ?�금 / ?�제 */
    if( GetLockHot() == UNLOCK )
    {
        // ?�수 ?�택 중이�? 강제�??�수 ?�택?�로 ?�동 ?�킨??
        if( GetWaterOutSelect() == SEL_WATER_HOT )
        {
            SetWaterOutSelect( SEL_WATER_ROOM );
        }
        SetLockHot( LOCK );
        return SOUND_LOCK_HOT;
    }

    SetLockHot( UNLOCK );
    StopDispTimerId( DISP_TIMER_ALARM_SETTING );
    return SOUND_UNLOCK_HOT;
}

#if 0
static U8 OnOffHotUser(void)
{
    if( GetHotConfigUser() == TRUE )
    {
        SetHotConfigUser( FALSE );
        return SOUND_CANCEL;
    }

    SetHotConfigUser( TRUE );
    return SOUND_SETUP;
}
#endif

U8 dbg_hot_out_temp = 0;
static U8 OnOffHotTest(void)
{

    if( GetHotConfigTest() == TRUE )
    {
        SetHotConfigTest( FALSE );
        return SOUND_CANCEL;
    }

    SetHotConfigTest( TRUE );
    return SOUND_SETUP;
}


#if !CONFIG_TEST_8585
static U8 OnOffSave(void)
{
    // RTC 미설???�태??불허
    if( GetRtcConfigInit() == FALSE )
    {
        SetSavingConfig( FALSE );
        return SOUND_ALARM_SLEEP_TIME;
    }

    if( GetSavingConfig() == TRUE )
    {
        SetSavingConfig( FALSE );

        return SOUND_CANCEL_SLEEP;
    }

    SetSavingConfig( TRUE );

    return SOUND_SETUP_SLEEP;
}

#else
/* ?�전 ?�정/?�제  */
extern U8 the_8585_step;
extern U8 the_8585_step_2;
static U8 OnOffSave(void)
{
    if( GetSavingConfig() == TRUE )
    {
        SetSavingConfig( FALSE );

#if CONFIG_TEST_8585
        DisableTimer( TIMER_ID_TEST_8585 );
        DisableTimer( TIMER_ID_TEST_8585_2 );
        the_8585_step = 0;
#endif

#if CONFIG_TEST_8585_ICE
        DisableTimer( TIMER_ID_TEST_8585_ICE );
        the_8585_step_2 = 0;
#endif
        return SOUND_CANCEL;
    }

    SetSavingConfig( TRUE );

#if CONFIG_TEST_8585
    StartTimer( TIMER_ID_TEST_8585, SEC(20) );
    StartTimer( TIMER_ID_TEST_8585_2, SEC(20) );
#endif

#if CONFIG_TEST_8585_ICE
    StartTimer( TIMER_ID_TEST_8585_ICE, SEC(30) );
#endif
    return SOUND_SETUP;
}
#endif

/* ?�성 모드 ?�정 */
static U8 SettingSound(void)
{
    U8 mu8Level;
    U8 mu8Sound = SOUND_SELECT;


    mu8Level = GetVoiceLevel();

    if( GetSoundDisp() == FALSE )
    {
        // ?�정 ?�면 ?�성??
        SetSoundDisp( TRUE );
    }
    else
    {
        if( mu8Level == VOICE_LEVEL_OFF )
        {
            mu8Level = VOICE_LEVEL_VOICE_BEEP;
        }
        else if( mu8Level == VOICE_LEVEL_BEEP )
        {
            mu8Level = VOICE_LEVEL_OFF;
        }
        else 
        {
            mu8Level = VOICE_LEVEL_BEEP;
        }

        SetVoiceLevel( mu8Level );
    }


    // sound 출력 종류 결정
    if( mu8Level == VOICE_LEVEL_OFF )
    {
        mu8Sound = SOUND_SETTING_MUTE;
    }
    else if( mu8Level == VOICE_LEVEL_BEEP )
    {
        mu8Sound = SOUND_SETTING_EFFECT;
    }
    else 
    {
        mu8Sound = SOUND_SETTING_VOICE;
    }

    StartDispTimerId( DISP_TIMER_SOUND_MENU );

    return mu8Sound;
}

static U8 SettingCustomer(void)
{
    U8 mu8Type;
    U8 mu8Sound = SOUND_SETUP;


    if( GetCustomerDisp() == FALSE )
    {
        // ?�정 ?�면 ?�성??
        SetCustomerDisp( TRUE );
    }
    else
    {
        mu8Type = GetCustomerType();
        if( mu8Type == CUSTOMER_SELF )
        {
            mu8Type = CUSTOMER_VISIT;
        }
        else 
        {
            mu8Type = CUSTOMER_SELF;
        }

        SetCustomerType( mu8Type );
    }

    StartDispTimerId( DISP_TIMER_CUSTOMER_MENU );

    return SOUND_SETUP;
}


/* ?�용??물양 ?�정 */
#if 0
static U8 OnOffUserAmount(void)
{
    U8 mu8Sound = SOUND_NONE;

    if( GetWaterOutConfigUser() == TRUE )
    {
        SetWaterOutConfigUser( FALSE );
        mu8Sound = SOUND_CANCEL;
    }
    else
    {
        SetWaterOutConfigUser( TRUE );
        mu8Sound = SOUND_SETUP;
    }

    return mu8Sound;
}
#endif

/* 버전 ?�시 */
#if 0
static U8 OnVersion(void)
{
    SetVersionDisp(0);

    return SOUND_SELECT;
}
#endif

/* 메멘???�시 */
static U8 OnOffMemento(void)
{
    if( GetMementoDisp() == FALSE )
    {
        SetMementoDisp( TRUE );
    }
    else
    {
        SetMementoDisp( FALSE );
    }

    return SOUND_SELECT;
}


/* ?�체 ?�금 ?�정 */
static U8 OnLockAll(void)
{
    if( GetLockAll() == UNLOCK )
    {
        SetLockAll( LOCK );
        return SOUND_LOCK_ALL;
    }

    StartDispTimerId( DISP_TIMER_ALARM_LOCK_ALL );
	if( GetVoiceLevel() == VOICE_LEVEL_VOICE_BEEP ) 
	{
    	return SOUND_ALARM_LOCK_ALL;
	}
	else if( GetVoiceLevel() == VOICE_LEVEL_BEEP ) 
	{
    	return SOUND_ERROR;
	}
	else if( GetVoiceLevel() == VOICE_LEVEL_OFF ) 
	{
    	return SOUND_NONE;
	}
    //return SOUND_ALARM_LOCK_ALL;
}

/* ?�체 ?�금 ?�제 */
static U8 OffLockAll(void)
{
    if( GetLockAll() == LOCK )
    {
        SetLockAll( UNLOCK );
        return SOUND_UNLOCK_ALL;
    }

    return SOUND_NONE;
}


#if 0
static U8 OnOffLockAll(void)
{
    U8 mu8Sound = SOUND_LOCK_ALL;


    if( GetLockAll() == LOCK )
    {
        SetLockAll( UNLOCK );
        mu8Sound = SOUND_UNLOCK_ALL;
    }
    else
    {
        SetLockAll( LOCK );
    }

    return mu8Sound;
}
#endif

#if 0
static U8 OnOffMute(void)
{
    U8 mu8Sound = SOUND_SETTING_VOICE;

    if( IsSetSoundMute() == FALSE )
    {
        SetSoundMute();
    }
    else
    {
        ClearSoundMute();
    }

    return SOUND_SETTING_MUTE;
}
#endif


static U8 OnOffCodyKit(void)
{
    if( GetCodyKitStatus() == TRUE )
    {
        StopWaterOut();
    }
    else
    {
        SetSelectWater( SEL_WATER_CODY );
        StartWaterOut();
    }

    return SOUND_SETUP;
}

static U8 OnTestIceTray(void)
{
    if( GetServiceCheckTankOpen() == TRUE )
    {
        if( GetIceTrayTestStatus() == FALSE )
        {
            StopDummyDeIcing();
            StopIceMake();
            StartIceTrayTest();
            return SOUND_SETUP;
        }
    }

    return SOUND_NONE;
}


static U8 OnDeIcing(void)
{
    // ?�빙 중일 ?? 강제 ?�빙???�작 ?�킨?? 
    if( StopIceMake() == TRUE )
    {
        return SOUND_SETUP;
    }

    return SOUND_SELECT;
}


static U8 OnManualDrain(void)       /*Manual Drain*/
{
    if( IsValidHealth() == FALSE )
    {
        return SOUND_ERROR;
    }

    if( StartDrainWater( FULL_DRAIN ) == FALSE )
    {
        return SOUND_ERROR;
    }

    return SOUND_EFFLUENT;
}


static U8 OnEraseEeprom(void)
{
    if( IsExpiredConfigTime() == FALSE )
    {
        EraseEeprom();
        return SOUND_SELECT;
    }

    return SOUND_NONE;
}

static U8 OnWifiAp(void)
{
    WifiKey(WIFI_KEY_AP);

    // ?�전 AP ?�속 ?�러가 ?�는 경우 ?�시
    if( GetWifiStatusValue(WIFI_STATUS_ERROR) != 0 )
    {
        StartDispTimerId( DISP_TIMER_WIFI_AP_ERROR );
    }
    return SOUND_NONE;
}

static U8 OnWifiBle(void)
{
    if( GetWaterOut() == TRUE 
            && GetWaterOutSelect() != SEL_WATER_FLUSH )
    {
        return SOUND_NONE;
    }

    WifiKey(WIFI_KEY_BLE);

    if( GetWifiStatusValue(WIFI_STATUS_ERROR) != 0 )
    {
        StartDispTimerId( DISP_TIMER_WIFI_AP_ERROR );
    }

    return SOUND_NONE;
}

static U8 OnWifiOnOff(void)
{
    WifiKey(WIFI_KEY_ONOFF);

    return SOUND_NONE;
}


// EOL 검???�작
static U8 OnEolStart(void)
{
    if( StartEolLoad() == TRUE )
    {
        return SOUND_EOL;
    }

    return SOUND_NONE;
#if 0
    // EOL ?�스??진입 ?�간??만료?�면,
    // 부?�???�이 Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // ?�재 EOL ?�스??중이�? Pass
    if( GetEolStatus() == TRUE )
    {
        return SOUND_NONE;
    }

    // EOL ?�작
    StartEol( EOL_TYPE_LOAD );
    StartDisplayInit();
    SetVersionDisp( 0 );

    // WIFI 
    WifiKey(WIFI_KEY_TEST);

    return SOUND_EOL;
#endif
}

// FRONT 검???�작
static U8 OnFrontStart(void)
{
    // EOL ?�스??진입 ?�간??만료?�면,
    // 부?�???�이 Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // ?�재 EOL ?�스??중이�? Pass
    if( GetEolStatus() == TRUE )
    {
        return SOUND_NONE;
    }

    // ?�수 감�?
    if( GetLeakStatus() == TRUE 
            || IsLeakSemi() == TRUE )
    {
        return SOUND_ERROR;
    }

    // ?�위 ?�서 불량
    if( GetDrainLevel() != LEVEL_LOW )
    {
        return SOUND_ERROR;
    }

    // ?�터 리드?�위�?미감지 
    if( GetFilterStatus( ID_FILTER ) == FILTER_OPEN )
    {
        return SOUND_ERROR;
    }

    // MATCH FRONT?� MAIN CHP TYPE 
    if( GetSystem() != GetFrontSystem() )
    {
        return SOUND_ERROR;
    }

    // EOL ?�작
    StartEol( EOL_TYPE_FRONT );

    TurnOffDrainPumpEx();
    MoveIceTrayIcingEol();
    StartDisplayInit();
    SetVersionDisp( 0 );

    // WIFI 
    WifiKey(WIFI_KEY_TEST);

    return SOUND_EOL;
}


// TIME SHORT ?�스??모드
static U8 OnTimeShort(void)
{
    // 부?�???�이 Pass
    if( IsExpiredTimeShortLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // TIME SHORT ?�작
    StartTimeShort();
    return SOUND_EOL;
}


// FCT 검???�작
static U8 OnFctStart(void)
{
    // EOL ?�스??진입 ?�간??만료?�면,
    // 부?�???�이 Pass
    if( IsExpiredFctLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }


    // FCT ?�작
    StartFct();

    // WIFI 
    WifiKey(WIFI_KEY_TEST);
    return SOUND_SELECT;
}

static U8 OnTestShortCompTime(void)
{
    // ?�축�?구속 ?�간 0?�로 초기??
#if CONFIG_TEST
    SetCompProtectOffTime(0);
#endif
    return SOUND_SELECT;
}


static U8 KeyContDownEx(void)
{
    if( GetClockDisp() == TRUE )
    {
        return KeyContDown();
    }

    return SOUND_NONE;
}

static U8 KeyContUpEx(void)
{
    if( GetClockDisp() == TRUE )
    {
        return KeyContUp();
    }

    return SOUND_NONE;
}


#if CONFIG_TEST_HOT_OUT
static U8 SelHotTest(void)
{
    if( GET_STATUS_VALVE_HOT_IN() == 0 )
    {
        OPEN_VALVE_HOT_IN();
    }
    else
    {
        CLOSE_VALVE_HOT_IN();
    }

    return SOUND_SELECT;
}
#endif

/* UV TEST CODE */
#if CONFIG_UV_MANUAL
extern U8 dbg_uv_ice_door;
extern U8 dbg_uv_water_out;
extern U8 dbg_uv_ice_tray;
extern U8 dbg_uv_ice_tank;
static U8 OnOffUvIceTank(void)
{
    dbg_uv_ice_tank = !dbg_uv_ice_tank;
    return SOUND_SETUP;
}

static U8 OnOffUvIceTray(void)
{
    dbg_uv_ice_tray = !dbg_uv_ice_tray;
    return SOUND_SETUP;
}

static U8 OnOffUvIceOut(void)
{
    dbg_uv_ice_door = !dbg_uv_ice_door;
    return SOUND_SETUP;
}

static U8 OnOffUvWaterOut(void)
{
    dbg_uv_water_out = !dbg_uv_water_out;

    return SOUND_SETUP;
}
#endif
