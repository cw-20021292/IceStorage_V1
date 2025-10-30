#include "health.h"
#include "eeprom.h"

#include "rtc.h"
#include "error.h"
#include "drain_water.h"
#include "water_out.h"
#include "ice_door.h"
#include "flush_water_out.h"
#include "flow_meter.h"


Health_T Health;


#define DEFAULT_HEALTH_MODE ( HEALTH_MODE_DRAIN )

void  InitHealth(void)
{
    Health.Mode       = DEFAULT_HEALTH_MODE;

    Health.Hour       = 0;
    Health.Min        = 0;
    Health.AdjustMode = 0;


    // 일반 모델..
    Health.ConfTargetTime   = NONE_RTC_TARGET_TIME;
    Health.Conf1Hour        = NONE_RTC_1HOUR;
    Health.Conf2Hour        = NONE_RTC_2HOUR;
    Health.ConfMaxTargetTime = MAX_NONE_RTC_TIME_OUT;
    Health.ConfWeekDays     = DEFAULT_UNUSED_WEEK_DAYS;

    Health.UnusedWeekDays   = DEFAULT_UNUSED_WEEK_DAYS;
    Health.TargetTimeOut    = NONE_RTC_TARGET_TIME; // target 24hour timeout..@1sec
    Health.CurrentTimeOut   = 0; // current 24hour timeout..@1sec
}

void  GetHealthData(Health_T *pData)
{
    memcpy( pData, &Health, sizeof( Health_T ));
}


// 헬스 기능 사용 가능 여부 검사
//  -. RTC 오류
//  -. 설정 정보 여부
U8 IsConfHealth(void)
{
#if CONFIG_STER
    NFC_TagTime_T mTime;


    // RTC 상태 
    if( IsRTC_Error() == TRUE )
    {
        return FALSE;
    }

    // 살균 설정 정보 유무
    if( GetSterPeriodMode() == 0xFF 
            || GetHealthHour() == 0xFF
            || GetHealthMin() == 0xFF 
            || GetSterPeriodMode() == 0xFF )
    {
        return FALSE;
    }
#endif

    // 살균 동작 가능 유무
    if( IsValidHealth() == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

// 헬스 기능 동작 가능 여부 검사
U8  IsStartHealth(void)
{
    // 배수 동작은 FULL 타입일 때에만 한다...
    if( IsGoingOnDrainWaterFull() == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

/* 위생 모드 */
void SetHealthModeVal(U8 mu8Mode)
{
    Health.Mode = mu8Mode;
}

void  SetHealthMode(U8 mu8Mode )
{
#if !CONFIG_STER
    mu8Mode &= ~HEALTH_MODE_STER;
#endif

    Health.Mode |= mu8Mode;
    SaveEepromId( EEP_ID_CONF_HEALTH );
}

void  ClearHealthMode(U8 mu8Mode )
{
    Health.Mode &= ~mu8Mode;
    SaveEepromId( EEP_ID_CONF_HEALTH );
}

U8 IsSetHealthMode(U8 mu8Mode )
{
    if( (Health.Mode & mu8Mode ) == mu8Mode )
    {
        return TRUE;
    }

    return FALSE;
}

U8 GetHealthMode(void)
{
    return Health.Mode;
}


// 예약 시간 (시)
void SetHealthHour(U8 mu8Val )
{
    if( mu8Val < 24U )
    {
        if( Health.Hour != mu8Val )
        {
            SaveEepromId(EEP_ID_STER_RESERVATION_HOUR);
        }
        Health.Hour = mu8Val;
    }
}

U8 GetHealthHour(void)
{
    return Health.Hour;
}

// 예약 시간 (분)
void SetHealthMin(U8 mu8Val )
{
    if( mu8Val == 30U || mu8Val == 0U )
    {
        if( Health.Min != mu8Val )
        {
            SaveEepromId(EEP_ID_STER_RESERVATION_MIN);
        }
        Health.Min = mu8Val;
    }
}

U8 GetHealthMin(void)
{
    return Health.Min;
}


// 예약 시간 보정
void SetHealthAdjustMode(U8 mu8Mode)
{
    if( mu8Mode > ADJUST_MODE_AFTER )
    {
        return ;
    }

    Health.AdjustMode = mu8Mode;
    SaveEepromId( EEP_ID_STER_ADJUST_MODE );
}

U8 GetHealthAdjustMode(void)
{
    return Health.AdjustMode;
}


// 예약 시간 보정이 된 시간 반환
U8 GetHealthHourAdjust(void)
{
    U8 mu8Hour;
    U8 mu8Mode;


    mu8Hour = GetHealthHour();
    mu8Mode = GetHealthAdjustMode();
    if( mu8Mode == ADJUST_MODE_BEFORE )    // -2시간 
    {
        if( mu8Hour >= 2 )  
        {
            mu8Hour = mu8Hour - 2;
        }
        else if( mu8Hour == 1 ) 
        {
            mu8Hour = 23;
        }
        else if( mu8Hour == 0 ) 
        {
            mu8Hour = 22;
        }
    }
    else if( mu8Mode == ADJUST_MODE_AFTER )   // +2시간 
    {
        /* 살균 시간을 2시간 앞으로 이동 */
        if( mu8Hour <= 21 )  
        {
            mu8Hour = mu8Hour + 2;
        }
        else if( mu8Hour == 22 ) 
        {
            mu8Hour = 0;
        }
        else if( mu8Hour == 23 ) 
        {
            mu8Hour = 1;
        }
    }

    return mu8Hour;
}

// 사용자 제품 사용중?
// RETURN
//  TRUE -사용 중
//  FALSE - 미사용 중
static U8 IsUserUsing(void)
{
    if( GetWaterOut() == TRUE )
    {
        return TRUE;
    }

    if( IsOpenIceDoor() == TRUE )
    {
        return TRUE;
    }
        

    return FALSE;
}

static void ResetTime(void)
{
    Health.TargetTimeOut  = Health.ConfTargetTime;
    Health.CurrentTimeOut = 0;
}

void SetHealthConfDrainWater(U32 mu32Target, U32 mu32Hour_1, U32 mu32Hour_2, U8 mu8Days )
{
    Health.ConfTargetTime   = mu32Target;
    Health.Conf1Hour        = mu32Hour_1;
    Health.Conf2Hour        = mu32Hour_2;
    Health.ConfWeekDays     = mu8Days;
    Health.ConfMaxTargetTime = (mu32Target + mu32Hour_1);

    ResetTime();
}

// 위생 동작 조건 검사 
// RETURN
//  - FALSE ; 중지 조건
//  - TRUE :  동작 가능함
U8 IsValidHealth(void)
{
    // 플러싱이 완료되지 않았으면 중지
    if( IsDoneFlushInitSetup() == FALSE )
    {
        return FALSE;
    }

    // 정수 수위가 물넘침이 아닌 경우, 
    if( GetOutageStatus(TYPE_WATER) == FLOW_OUTAGE 
            || GetOutageStatus( TYPE_HOT_WATER ) == FLOW_OUTAGE )
    {
        return FALSE;
    }

    // 부하 에러
    if( IsErrorType( ERR_TYPE_STER ) == TRUE )
    {
        return FALSE;
    }

    // 사용 중
    if( IsUserUsing() == TRUE )
    {
        return FALSE;
    }

    if( IsStartDrainWater() == TRUE )
    {
        return FALSE;
    }

    return TRUE;
}

static U8 GetAlaramByRTC(U8 mu8Action)
{
    static U8 mu8Alaram = FALSE; // 알람 시간에 한 번만 동작되도록...
    U8 mu8Hour;
    U8 mu8Min;


    mu8Hour = GetHealthHourAdjust();
    mu8Min  = GetHealthMin();
    if ( IsAlaramTime(mu8Hour, mu8Min, 0) == STATUS_ALARAM
              && IsAlaramTime(mu8Hour, mu8Min + 10, 0) == STATUS_NOT_YET )

    {
        if( mu8Alaram == FALSE )
        {
            mu8Alaram = TRUE;
            mu8Action  = TRUE;
        }
    }
    else
    {
        mu8Alaram = FALSE;
        mu8Action  = FALSE;
    }


    return mu8Action;
}



static void MakeOneHourEarly(void)
{
    if( Health.UnusedWeekDays != 0 )
    {
        Health.UnusedWeekDays--;
    }
    else
    {
        Health.UnusedWeekDays = Health.ConfWeekDays;
        Health.CurrentTimeOut = Health.Conf1Hour;
    }
}

U8 GetAlaramByNoneRTC(U8 mu8Action)
{
    U32 mu32RemainTime;


    mu32RemainTime = Health.TargetTimeOut - Health.CurrentTimeOut;

    // 22~24시간 사이에 물 사용량 있으면 7일 카운터를 초기화한다.
    // 초기화 하고, 즉시 한번 감소 되기 때문에 카운터는 8로 설정한다.
    if( mu32RemainTime <= Health.Conf2Hour )
    {
        if( IsUserUsing() == TRUE )
        {
            Health.UnusedWeekDays = Health.ConfWeekDays;
        }
    }

    // 23~24시간 사이에 추출 동작이 있으면, +1Hour 추가한다.
    // 단, 1회만 +1 Hour를 더할 수 있다.
    // 잔여 시간 계산.. 1Hour?
    if( mu32RemainTime <= Health.Conf1Hour )
    {
        if( IsUserUsing() == TRUE )
        {
            if( Health.TargetTimeOut < Health.ConfMaxTargetTime )
            {
                Health.TargetTimeOut += Health.Conf1Hour;
            }
        }
    }

    // 24시간 계산..
    if( Health.TargetTimeOut > Health.CurrentTimeOut )
    {
        mu8Action = FALSE;

        Health.CurrentTimeOut++;
    }
    else
    {
        mu8Action = TRUE;

        // 시간 재설정
        ResetTime();

        // 7일 동안 물 사용량이 없으면 다음 배수 시간을 +1시간 당김.
        MakeOneHourEarly();
    }

    return mu8Action;
}


// 살균 동작 시간 직전으로 시간 재설정
void UpdateHealthOnTime(void)
{
    TimeData_T mTime;
    U8 mu8Hour;
    U8 mu8Min;


    mu8Hour = GetHealthHourAdjust();
    mu8Min  = GetHealthMin();
    GetRtcTime( &mTime );

    if( mu8Min == 0 )
    {
        if( mu8Hour != 0 )
        {
            mTime.Hour = mu8Hour - 1;
        }
        else
        {
            mTime.Hour = 23;
        }

        mTime.Min = 59;
    }
    else
    {
        mTime.Hour = mu8Hour;
        mTime.Min = 29;
    }

    mTime.Sec = 55;

    SetRtcTime( &mTime );

}

void UpdateHealthOnTime_2(void)
{
    Health.CurrentTimeOut = Health.TargetTimeOut - 5UL;
}

void UpdateHealthOnTimeOneHour(void)
{
    Health.CurrentTimeOut = Health.TargetTimeOut - (NONE_RTC_1HOUR);
}

void UpdateHealthOnTimeTwoHour(void)
{
    Health.CurrentTimeOut = Health.TargetTimeOut - (NONE_RTC_2HOUR);
}

void UpdateHealthOnTimeWeek(void)
{
    Health.UnusedWeekDays = 0;
}

// 제어 
void  ControlHealth(void)
{
    static U8 mu8CheckAction = FALSE; 


#if CONFIG_STER
    // RTC 에러 
    if( IsRTC_Error() == TRUE 
            && GetHealthMode() != HEALTH_MODE_NONE )
    {
        ClearHealthMode( HEALTH_MODE_ALL );
        return ;
    }
#endif
    mu8CheckAction = GetAlaramByNoneRTC( mu8CheckAction );

    // On/Off
    if( Health.Mode == HEALTH_MODE_NONE )
    {
        mu8CheckAction = FALSE;
        return ;
    }

    // Check conditions
    if( IsValidHealth() == FALSE )
    {
        return ;
    }

    // Check reservation time
#if CONFIG_STER
    mu8CheckAction = GetAlaramByRTC( mu8CheckAction );
#endif

    // Health Mode 제어
    if( mu8CheckAction == TRUE )
    {
        // 순환 배수 사용 설정이 되어 있는 경우, 배수 시작
        if( IsSetHealthMode( HEALTH_MODE_DRAIN ) == TRUE )
        {
            StartDrainWater( HALF_DRAIN );
        }

        if( IsStartDrainWater() == TRUE )
        {
            mu8CheckAction = FALSE;
        }
    }
}

