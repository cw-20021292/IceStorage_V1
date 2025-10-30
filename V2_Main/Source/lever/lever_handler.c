#include "lever_handler.h"
#include "lever_common_handler.h"

#include "water_out.h"
#include "flush_water_out.h"
//#include "tank_clean.h"
#include "front.h"
#include "process_display.h"
#include "sound.h"
#include "drain.h"
#include "drain_water.h"
#include "ice.h"
#include "ice_door.h"
#include "ice_tray_test.h"
#include "cold_water.h"
#include "error.h"
#include "flow_meter.h"
#include "cody_kit.h"
#include "power_saving.h"
#include "hot_water.h"

#include "voice.h"

static U8 LeverOpenIceDoor(void);

// 읾반 모드
/* WATER OUT */
static U8 LeverOpenTapWater(void);
static U8 LeverCloseTapWater(void);
static U8 LeverOpenTapWaterLong(void);

/* ICE DOOR */
static U8 LeverCloseIceDoor(void);

const static LeverEventList_T LeverEventList[] =
{
    /*   Short,                Long(2초),            Long2 (8초)           Pop  */
    {  LeverOpenTapWater,    LeverOpenTapWaterLong,  LeverCloseTapWater,   NULL  },
    {  LeverOpenIceDoor,     NULL,                   NULL,                 LeverCloseIceDoor   }
};


// 코디 점검 모드
/* WATER OUT */
static U8 CodyLeverOpenTapWater(void);
static U8 CodyLeverOpenTapWaterLong(void);

/* ICE DOOR */
static U8 CodyLeverOpenIceDoor(void);
static U8 CodyLeverCloseIceDoor(void);
const static LeverEventList_T CodyLeverEventList[] =
{
    /* Short,                   Long,                       Long2                  Pop  */
    {  CodyLeverOpenTapWater,   CodyLeverOpenTapWaterLong,  LeverCloseTapWater,    NULL  },
    {  CodyLeverOpenIceDoor,    NULL,                       NULL,                  CodyLeverCloseIceDoor   }
};


// 특수 메뉴 모드
/* WATER OUT */
static U8 ServiceLeverOpenTapWater(void);

const static LeverEventList_T ServiceLeverEventList[] =
{
    /* Short,                       Long(2초),              Long2 (8초)           Pop  */
    {  ServiceLeverOpenTapWater,    LeverOpenTapWaterLong,  LeverCloseTapWater,   NULL  },
    {  LeverOpenIceDoor,            NULL,                   NULL,                 LeverCloseIceDoor   }
};

// 추출 메뉴로 이동


/* WATER OUT - START */
static U8 LeverOpenTapWater(void)
{
    U8 mu8Sound = SOUND_EFFLUENT;


    if( GetCodyKitStatus() == TRUE 
            && GetWaterOut() == TRUE )
    {
        StopWaterOut();
        return SOUND_EFFLUENT_END;
    }

    // 메멘토 표시 중
    if( GetMementoDisp() == TRUE )
    {
        return SOUND_NONE;
    }

    /* 배수 중지 */
    if( ResetDrainWater() == TRUE )
    {
        return SOUND_CANCEL;
    }

    /* 전체 잠금 */
    if( GetLockAll() == LOCK )
    {
        return SOUND_NONE;
        //StartDispTimerId( DISP_TIMER_ALARM_LOCK_ALL );
        //return SOUND_ALARM_LOCK_ALL;
    }

    // 연속 추출 중이면, 추출 중지
    if( GetWaterOut() == TRUE )//&& GetWaterOutContinue() == TRUE )
    {
        StopWaterOut();
        StartDispTimerId( DISP_TIMER_ERROR );
        return SOUND_EFFLUENT_END;
    }

    // 물 추출 종료 후, 일정 시간동안 재추출 금지
    if( IsExpiredOutProtectTime() == FALSE )
    {
        return SOUND_NONE;
    }

    // 추출 재시도하면 단수 에러 해제
    SetOutageStatus( TYPE_WATER, FLOW_SUPPLY );

    // 이중 추출 종료음 발생을 막기 위해서 추가된 코드
    // 저수위에 대한 냉수 추출 종료음은 water_out에서 발생함.
    //if( IsTankCleanMode() == FALSE )
    //{
    //    if( GetWaterOutSelect() == SEL_WATER_COLD 
    //            && GetColdWaterLevel() == LEVEL_LOW )
    //    {
    //        return SOUND_NONE;
    //    }
    //}

#if 0
    // 탱크 비우기 모드
    if( IsTankCleanMode() == TRUE )
    {
        SetTankClean( TRUE );
        StartDrainPumpOnLevel();
    }
#endif

    // 연속 추출음 설정
    if( //GetWaterOutContinue() == TRUE && 
            GetWaterOutAmoutType() == FIXED_AMOUNT_INFINITY )
    {
        mu8Sound = SOUND_EFFLUENT_CONTINUE;
    }

    // 온수 추출 사운드
    if( GetWaterOutSelect() == SEL_WATER_HOT )
    {
        if( GetHotSelect() == SEL_HOT_USER )
        {
            mu8Sound = SOUND_HOT_OUT_USER;
        }
        else
        {
            mu8Sound = SOUND_HOT_OUT;
        }
    }

    SetSettingDisp( FALSE );
    
    mu8Sound = StartWaterOutFlush( mu8Sound );

    return mu8Sound;
}


// 초기 채우기 동작은 강제 취소
static U8 CancelFlushPowreOn(void)
{
    if( IsDoneFlushStatus() == TRUE 
            && GetFlushPowerOn() == FLUSH_STATUS_INIT )
    {
        SetColdWaterInitFull( TRUE );
        SetFlushPowerOn( FLUSH_STATUS_DONE );
        SetWaterOutSelect( SEL_WATER_ROOM );

        return TRUE;
    }

    return FALSE;
}

static U8 LeverCloseTapWater(void)
{
    U8 mu8Sound = SOUND_NONE;

    // 추출 중이면, 추출 중지
    if( GetWaterOut() == TRUE )
    {
        StopWaterOut();
        
        CancelFlushPowreOn();
        SetFotaReboot( FALSE );
        return SOUND_EFFLUENT_END;
    }

    if( CancelFlushPowreOn() == TRUE )
    {
        return SOUND_CANCEL;
    }

    return mu8Sound;
}

static U8 WaterOutInfinity(void)
{
    if( GetWaterOut() == TRUE )
    {
        // Skip long button when FLUSHING...
        if( IsDoneFlushInitSetup() == FALSE )
        {
            return FALSE;
        }

        if( GetWaterOutAmoutType() != FIXED_AMOUNT_INFINITY )
        {
            SetWaterOutInfinityLongKey();
            return TRUE;
        }
    }

    return FALSE;
}

static U8 LeverOpenTapWaterLong(void)
{
    U8 mu8Sound = SOUND_NONE;


    if( WaterOutInfinity() == TRUE )
    {
        mu8Sound =  SOUND_EFFLUENT_CONTINUE;
    }

    // Long key 입력으로 연속 추출시, 탱크 비움 기능 동작
#if 0
    if( IsTankCleanMode() == TRUE )
    {
        SetTankClean( TRUE );
        StartDrainPumpOnLevel();
    }
#endif

    SetSettingDisp( FALSE );
    return mu8Sound;
}


/* ICE DOOR - PUSHED( OPEN )*/
static U8 LeverOpenIceDoor(void)
{
    U8 mu8Sound = SOUND_ALARM_ICE;


    if( IsDoneFlushInitSetup() == FALSE )
    {
        return SOUND_NONE;
    }

    if( GetMementoDisp() == TRUE )
    {
        return SOUND_NONE;
    }

    if( ResetDrainWater() == TRUE )
    {
        return SOUND_CANCEL;
    }

    SetSettingDisp( FALSE );
    
    /* 전체 잠금 */
    if( GetLockAll() == LOCK )
    {
        return SOUND_NONE;
        //StartDispTimerId( DISP_TIMER_ALARM_LOCK_ALL );
        //return SOUND_ALARM_LOCK_ALL;
    }

    /* 얼음 잠금 시, 에러 표시 */
    if( GetLockIce() == TRUE )
    {
        StartDispTimerId( DISP_TIMER_ALARM_LOCK_ICE );
		if( GetVoiceLevel() == VOICE_LEVEL_VOICE_BEEP ) 
		{
        	return SOUND_ALARM_ICE_LOCK;
		}
		else if( GetVoiceLevel() == VOICE_LEVEL_BEEP ) 
		{
        	return SOUND_ERROR;
		}
		else if( GetVoiceLevel() == VOICE_LEVEL_OFF ) 
		{
        	return SOUND_NONE;
		}
        //return SOUND_ALARM_ICE_LOCK;
    }

    /* 얼음 OFF 시, 알림 표시 */
    if( GetIceConfigMake() == FALSE )
    {
        StartDispTimerId( DISP_TIMER_ALARM_MAKE_OFF_ICE );
		if( GetVoiceLevel() == VOICE_LEVEL_VOICE_BEEP ) 
		{
        	mu8Sound = SOUND_ALARM_ICE_OFF;
		}
		else if( GetVoiceLevel() == VOICE_LEVEL_BEEP ) 
		{
        	mu8Sound = SOUND_ERROR;
		}
		else if( GetVoiceLevel() == VOICE_LEVEL_OFF ) 
		{
        	mu8Sound = SOUND_NONE;
		}
        //mu8Sound = SOUND_ALARM_ICE_OFF;
    }
    /* 슬립 중 얼음 부족 알림 */
    else if( GetSavingStatus() == STATUS_SLEEP )
    {
        mu8Sound = SOUND_ALARM_SLEEP_NO_ICE;
    }

    OpenIceDoor();
    return mu8Sound;
}


/*ICE DOOR - POP ( CLOSE ) */
static U8 LeverCloseIceDoor(void)
{
    U8 mu8Sound = SOUND_EFFLUENT_END;

    /*  잠금 시, 소리 없음 */
    if( GetLockIce() == TRUE 
            || GetLockAll() == TRUE 
            || GetSoundDisp() == TRUE 
            || GetMementoDisp() == TRUE 
            || GetSettingDisp() == TRUE 
            || GetClockDisp() == TRUE 
            || GetCustomerDisp() == TRUE 
            || IsDoneFlushInitSetup() == FALSE 
      )
    {
        return SOUND_NONE;
    }

    // 추출 시간이 짧으면 안내
    // 취침 모드에서는 하지 않음.
    if( GetIceConfigMake() == TRUE 
            && GetSavingStatus() == FALSE )
    {
        if( GetIceDoorOpeningTime() < ICE_DOOR_OPEN_ALARAM_TIME )
        {
            mu8Sound = SOUND_ALARM_ICE_OUT;
        }
    }

    /* 연속 추출 시에는 키 입력 초기화 */
    CloseIceDoor();
    StartDispTimerId( DISP_TIMER_ERROR );
    return mu8Sound;
}


/* WATER OUT */
static U8 CodyLeverOpenTapWater(void)
{
    U8 mu8Sound = SOUND_EFFLUENT;


    if( GetWaterOut() == TRUE )
    {
        StopWaterOut();
        return SOUND_EFFLUENT_END;
    }

    // 물 추출 종료 후, 일정 시간동안 재추출 금지
    if( IsExpiredOutProtectTime() == FALSE )
    {
        return SOUND_NONE;
    }

    // 소다 저수위 추출 금지
    //if( GetWaterOutSelect() == SEL_WATER_SODA 
    //        && GetColdWaterLevel() == LEVEL_LOW )
    //{
    //    return SOUND_ERROR;
    //}

    StartWaterOut();
    
    return mu8Sound;
}

static U8 CodyLeverOpenTapWaterLong(void)
{
    U8 mu8Sound = SOUND_NONE;


    if( WaterOutInfinity() == TRUE )
    {
        mu8Sound =  SOUND_EFFLUENT_CONTINUE;
    }

    return mu8Sound;
}

/* ICE DOOR */
static U8 CodyLeverOpenIceDoor(void)
{
    OpenIceDoor();

    return SOUND_EFFLUENT;
}

static U8 CodyLeverCloseIceDoor(void)
{
    CloseIceDoor();
    return SOUND_EFFLUENT_END;
}


///// SERVICE MENU
/* WATER OUT - START */
static U8 ServiceLeverOpenTapWater(void)
{
    U8 mu8Sound = SOUND_EFFLUENT;


    // 연속 추출 중이면, 추출 중지
    if( GetWaterOut() == TRUE )//&& GetWaterOutContinue() == TRUE )
    {
        StopWaterOut();
        return SOUND_EFFLUENT_END;
    }

    // 물 추출 종료 후, 일정 시간동안 재추출 금지
    if( IsExpiredOutProtectTime() == FALSE )
    {
        return SOUND_NONE;
    }

    // 소다 저수위 추출 금지
    //if( GetWaterOutSelect() == SEL_WATER_SODA 
    //        && GetColdWaterLevel() == LEVEL_LOW )
    //{
    //    return SOUND_ERROR;
    //}

    // 소다 냉수 탱크 에러 추출 금지
    //if( GetWaterOutSelect() == SEL_WATER_SODA 
    //        && IsError( ERR_COLD_LOW_LEVEL ) == TRUE )
    //{
    //    return SOUND_ERROR;
    //}

#if CONFIG_STER
    if( GetSterErrWaterOut() == TRUE )
    {
        return SOUND_ERROR;
    }
#endif

    // 탱크 비우기 모드
#if 0
    if( IsTankCleanMode() == TRUE )
    {
        SetTankClean( TRUE );
        StartDrainPumpOnLevel();
    }
#endif

    // 연속 추출음 설정
    if( GetWaterOutAmoutType() == FIXED_AMOUNT_INFINITY )
    {
        mu8Sound = SOUND_EFFLUENT_CONTINUE;
    }
    
    StartWaterOut();

    return mu8Sound;
}



void* GetLeverEventList(void)
{
#if 0
    if( GetCodyService() == TRUE )
    {
        return (void *)CodyLeverEventList;
    }
#endif
    //else if( IsMenuService() == TRUE )
    //{
    //    return (void *)ServiceLeverEventList;
    //}
    // Return address of lever event table 
    return (void *)LeverEventList;
}

