#include "ice.h"
#include "ice_full.h"
#include "eeprom.h"
#include "error.h"
#include "power_saving.h"
#include "cold_water.h"
#include "comp.h"
#include "front.h"
#include "tank_clean.h"
#include "temp.h"
#include "flush_water_out.h"
#include "filter.h"
#include "service.h"
#include "flow_meter.h"
#include "ice_tray.h"
#include "process_make.h"

#include <string.h>

/* ICE FULL */
#define  ICE_CHECK_TIME    900   // @1sec, 15min

#define  TEMP_ICE_STOP_SEASON      25.0f
#define  ICE_STOP_TIME_SUMMER     18000U // @1sec, 5Hour
#define  ICE_STOP_TIME_WINTER     21600U // @1sec, 6Hour

/* DE-ICING 진입 시간 */
#define MIN_DEICING_TIME            180U //@1sec, 3min

#define TEMPORARY_ICE_COUNT   3UL		// 제빙 횟수 최대 카운트

Ice_T Ice;
//extern Ice_Turbo_T IceTurbo;

void InitIce(void)
{
    Ice.Turbo  = FALSE;
    Ice.Size   = ICE_LARGE;

    Ice.ConfigMake = TRUE;
    Ice.Make       = FALSE;


    Ice.Mode = ICE_MODE_DUMMY_DEICING;
#if (CONFIG_GAS_FILLING) || (CONFIG_JIG_GAS_SWITCH)
    Ice.Mode = ICE_MODE_NONE;
#endif

    Ice.FullCheckTimer = 0;
    Ice.StopMakeTimer = 0;

    Ice.DummyDeIcing = FALSE;

    Ice.DeIcingCompTime = MIN_DEICING_TIME;

	Ice.MakingIceCount = 0;

	//IceTurbo.Status = ICE_TURBO_NONE;
}

void GetIceData( Ice_T *pIce )
{
    MEMCPY( (void __FAR *)pIce, (const void __FAR *)&Ice, sizeof( Ice_T ));
}

#if 0
void  SetIceTurbo(U8 mu8Val)
{
    if( mu8Val != TRUE && mu8Val != FALSE )
    {
        return ;
    }
    Ice.Turbo = mu8Val;

	if(Ice.Turbo == TRUE)
	{
		SetIceTurboStatus(ICE_TURBO_START);
	}

    SaveEepromId( EEP_ID_CONF_TURBO );
}
#endif

void  SetIceTurbo(U8 mu8Val)
{
    if( mu8Val != TRUE && mu8Val != FALSE )
    {
        return ;
    }
    Ice.Turbo = mu8Val;

    SaveEepromId( EEP_ID_CONF_TURBO );
}

U8  GetIceTurbo(void)
{
    return Ice.Turbo;
}

void  SetIceSize(U8 mu8Val)
{
    Ice.Size = mu8Val;
    SaveEepromId( EEP_ID_ICE_SIZE );
}

U8  GetIceSize(void)
{
    return Ice.Size;
}

void SetIceConfigMake(U8 mu8Val)
{
    if( mu8Val != TRUE && mu8Val != FALSE )
    {
        return ;
    }

    Ice.ConfigMake = mu8Val;

    SaveEepromId( EEP_ID_CONF_MAKE_ICE );
}

U8 GetIceConfigMake(void)
{
    return Ice.ConfigMake;
}


void  SetIceMake(U8 mu8Val )
{
   Ice.Make = mu8Val;
}

U8    GetIceMake(void)
{
    return Ice.Make;
}


// 제빙 운전 온도 검사 
U8 IsValidDoIcingAmbiTempRange(void)
{
    TEMP_T  tAmbi;

    tAmbi = GetTemp( TEMP_ID_AMBIENT );
    if( tAmbi < 0.0f )
    {
        return FALSE;
    }

    return TRUE;
}

U8 IsValidDoIcingRoomTempRange(void)
{
    TEMP_T  tRoom;

    tRoom = GetTemp( TEMP_ID_ROOM_WATER );
    if( tRoom < 0.0f )
    {
        return FALSE;
    }

    return TRUE;
}

void SetIceMakeMode(U16 mu16Mode )
{
    Ice.Mode |= mu16Mode;
}

void ClearIceMakeMode(U16 mu16Mode )
{
    Ice.Mode &= ~mu16Mode;
}

U16 IsSetIceMakeMode(U16 mu16Mode )
{
    if( (Ice.Mode & mu16Mode ) == mu16Mode )
    {
        return TRUE;
    }

    return FALSE;
}

U16 GetIceMakeMode(void )
{
    return Ice.Mode;
}

void SetIceFullCheckTimer(U16 mu16Val )
{
    Ice.FullCheckTimer = mu16Val;
}

// 더미 탈빙 동작 여부 설정/반환
U8 GetIceDummyDeIcing( void )
{
    return Ice.DummyDeIcing;
}

void SetIceDummyDeIcing(U8 mu8Val )
{
    Ice.DummyDeIcing = mu8Val;
}


// 강제 탈빙을 위한 최소 압축기 운전 시간
U16 GetDeIcingCompOnTime(void)
{
    return Ice.DeIcingCompTime;
}

void ResetDeIcingCompOnTime(void)
{
     Ice.DeIcingCompTime = MIN_DEICING_TIME;
}

void UpdateDeIcingCompOnTime(void)
{
    if( Ice.DeIcingCompTime != 0 )
    {
        Ice.DeIcingCompTime--;
    }
}

void SetMakingIceCount(U8 count)
{
	Ice.MakingIceCount = count;
}

U8 GetMakingIceCount(void)
{
	return Ice.MakingIceCount;
}

U8 IsMaxMakingIceCount(void)
{
	if( Ice.MakingIceCount >= 3 )
	{
		return TRUE;
	}

	return FALSE;
}

void UpdateMakingIceCount(void)
{
	if( Ice.Turbo == FALSE )
	{
		Ice.MakingIceCount = 0;
		return;
	}

	if( Ice.MakingIceCount < TEMPORARY_ICE_COUNT)
	{
		Ice.MakingIceCount++;

		if( Ice.MakingIceCount == TEMPORARY_ICE_COUNT  )
		{
			SetColdWaterMake(FALSE);
		}
	}
}

void ClearMakingIceCount(void)
{
	Ice.MakingIceCount = 0;
}

/* 추출 대기 시간 초과하면 재시도 지연시간 */
#define ICE_OUTAGE_TIMER        600U    //10min // @100ms
void StartIceOutageTimer(void)
{
    Ice.OutageTimer = ICE_OUTAGE_TIMER;
}

static void CheckIceOutage(void)
{
    if( Ice.OutageTimer != 0 )
    {
        Ice.OutageTimer--;
    }

    if( GetFlowMeterLPM() > 0 )
    {
        Ice.OutageTimer = 0;
    }
}


void StopIceMakeLoad(void)
{
    // 제빙 시퀀스 해제
    ClearIceMakeMode( ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
    SetIceMakeMode(  ICE_MODE_OFF_LOAD );
}

// 더미 탈빙 
void SetDummyDeicing(void)
{
    // 제빙 시퀀스 해제
    //ClearIceMakeMode( ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
    ClearIceMakeMode( ICE_STATUS_PREHAT );

    // 더미 탈빙
    SetIceMakeMode( ICE_MODE_DUMMY_DEICING );
}

// 아이스 트레이 구속 탈빙
void SetTrayDeicing(void)
{
    // 제빙 시퀀스 해제
    ClearIceMakeMode(ICE_STATUS_INIT_PREHEAT);

    // 더미 탈빙
    ClearIceMakeMode( ICE_MODE_DUMMY_DEICING );

    // 트레이 탈빙
    SetIceMakeMode( ICE_MODE_TRAY_DEICING );

}

U8 IsStopIceMake(void)
{
    if( IsSetIceMakeMode( ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE 
            || IsSetIceMakeMode( ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

// 제빙 중지
U8 StopIceMake(void)
{
    if( IsSetIceMakeMode(ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE 
            || IsSetIceMakeMode(ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE
            || IsSetIceMakeMode( ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE
            || IsSetIceMakeMode( ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
    {
        // 초기화, 준비 단계이면 제빙 동작 취소하고 트레이 탈빙 위치로 이동
        ClearIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
        SetIceMakeMode( ICE_MODE_INIT | ICE_MODE_OFF_LOAD );

        return TRUE;
    }
    else if( IsSetIceMakeMode( ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
    {
        if( GetDeIcingCompOnTime() == 0 )
        {
            // 제빙 중이면, 탈빙 단계로 이동
            // 단, 제빙 최소 시간 만족 후...
            ClearIceMakeMode( ICE_MODE_ICING );
        }
        else
        {
            // 제빙 시간이 짧은 경우, 그냥 취소...
            ClearIceMakeMode( ICE_MODE_ICING | ICE_MODE_DEICING );
            SetIceMakeMode( ICE_MODE_INIT | ICE_MODE_OFF_LOAD );
        }

        return TRUE;
    }

    // 탈빙 중이면, 탈빙을 완료한다.
    return FALSE;
}

// 외기 온도에 따라서 만빙 후 제빙 중지 시간을 달리한다.
// 동절기에는 5시간, 하절기는 6시간 동작한다.
static U16 GetStopMakeTimer(void)
{
    if( GetTemp( TEMP_ID_AMBIENT ) >= TEMP_ICE_STOP_SEASON )
    {
        return ICE_STOP_TIME_SUMMER;
    }

    return ICE_STOP_TIME_WINTER;
}

/* 만빙검사 */
void  MakeIceFull(void)
{
    static U8 mu8PrevIceFull = FALSE;
    U8 mu8IceFull;

#if 0
    // 절전 중에 만빙이 되면, StopMakeTimer 만큼 제빙을 중지한다.
    mu8IceFull = GetIceFullStatus();
    if( mu8PrevIceFull != mu8IceFull )
    {
        mu8PrevIceFull = mu8IceFull;
        if( mu8IceFull == TRUE && GetSavingStatus() == TRUE )
        {
            Ice.StopMakeTimer = GetStopMakeTimer();
        }
    }
    // 단, 절전이 해제되면 제빙 중지 상태를 해제한다.
    if( GetSavingStatus() == FALSE )
    {
        Ice.StopMakeTimer = 0;
    }

    if( Ice.StopMakeTimer != 0 )
    {
        Ice.StopMakeTimer--;
    }
#endif


    // 만빙 검사 중이면, 만빙 검사 주기 타이머 업데이트
    if( IsDoneCheckIceFull() == FALSE )
    {
        SetIceFullCheckTimer( ICE_CHECK_TIME );
    }

    // 만빙 검사 타이머 감소...
    if( Ice.FullCheckTimer != 0 )
    {
        Ice.FullCheckTimer--;
        return ;
    }

    // 만빙 검사 시작
    // 그리고 타이머 reload
    StartCheckIceFull();
}


static U8 IsValidMake(void)
{
    // 설정에 의한 중지
    if( Ice.ConfigMake == FALSE )
    {
    	ClearMakingIceCount();
        return FALSE ;
    }

    /* 외기, 정수 온도 영하 조건 */
    if( IsValidDoIcingRoomTempRange() == FALSE 
            || IsValidDoIcingAmbiTempRange() == FALSE )
    {
        return FALSE;
    }

    if( IsDoneFlushStatus() == FALSE )
    {
        return FALSE;
    }

//    /* 강제 순환 배수 */
//    if( IsGoingOnDrainWaterFull() == TRUE )
//    {
//        return FALSE;
//    }

    /* 압축기 구속 시간 */
    if( IsExpiredCompProtectTime() == FALSE )
    {
        return FALSE;
    }

    // 압축기 보호 시간 초과 
    if( IsOverCompProtectOnTime() == TRUE )
    {
        return FALSE;
    }

    // 만빙 최초 검사가 완료되지 않은 경우, 제빙 동작하지 않는다.
    if( GetIceFullPowerOnCheck() == FALSE )
    {
        return  FALSE;
    }

    if( IsCompBldcProtectMode() == TRUE )
    {
        return FALSE;
    }

#if 0
    // 냉수 최초 만수위 조건 검사
    if( GetColdWaterInitFull() == FALSE )
    {
        return FALSE;
    }

    // 정수 저수위 조건
    if( GetRoomWaterLevel() == LEVEL_LOW )
    {
        return FALSE;
    }
#endif

#if 0
    // 절전 타이머 ( 절전 중 만빙이 된 경우 )
    if( Ice.StopMakeTimer != 0 )
    {
        return FALSE;
    }
#endif
    /* 슬립 중에는 정지 */
    if( GetSavingStatus() == TRUE )
    {
    	ClearMakingIceCount();
        return FALSE;
    }

    // 탱크 탑 커버 OPEN 시, 제빙 중지
    if( GetServiceCheckTankOpen() == TRUE )
    {
    	//ClearMakingIceCount();
        return FALSE;
    }

    // 필터 커버, 필터 스위치 OPEN시 중지
    if( IsOpenFilter( ID_FILTER_COVER ) == TRUE 
            || IsOpenFilter( ID_FILTER ) == TRUE )
    {
    	//ClearMakingIceCount();
        return FALSE;
    }

    // 에러에 의한 중지
    if( IsErrorType( ERR_TYPE_ICE_MAKE ) == TRUE )
    {
    	//ClearMakingIceCount();
        return FALSE;   // 제빙 못함~
    }

    // 단수로 인한 대기...
    if( Ice.OutageTimer != 0 )
    {
        return FALSE;
    }

    if( IsRetryIceTray() == TRUE )
    {
        return FALSE;
    }

#if 0
    // HEALTH 동작 중 ( 살균/배수 )
    if( IsStartHealth() == TRUE )
    {
        return FALSE;
    }

    // 냉수 청소 모드
    if( GetTankCleanOffLoad() == TRUE )
    {
        return FALSE;
    }

    if( GetCodyService() == TRUE )
    {
        return TRUE;
    }
#endif

    return TRUE;  // 제빙 go~ go~ 
}

void  MakeIce(void)
{
    U8 mu8Make = FALSE;


    CheckIceOutage();

    if( GetIceFullStatus() == FALSE )
    {
        mu8Make = TRUE;
    }
    else
    {
        mu8Make = FALSE;
    }

    if( IsValidMake() == FALSE )
    {
        mu8Make = FALSE;
    }

    // Set new make status
    SetIceMake( mu8Make );
}

void SetWifiIceMakeMode(U16 mu16Mode )
{
    Ice.WifiMode = mu16Mode;
}

U16 GetWifiIceMakeMode(void )
{
    return Ice.WifiMode;
}

