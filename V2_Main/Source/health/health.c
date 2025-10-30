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


    // �Ϲ� ��..
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


// �ｺ ��� ��� ���� ���� �˻�
//  -. RTC ����
//  -. ���� ���� ����
U8 IsConfHealth(void)
{
#if CONFIG_STER
    NFC_TagTime_T mTime;


    // RTC ���� 
    if( IsRTC_Error() == TRUE )
    {
        return FALSE;
    }

    // ��� ���� ���� ����
    if( GetSterPeriodMode() == 0xFF 
            || GetHealthHour() == 0xFF
            || GetHealthMin() == 0xFF 
            || GetSterPeriodMode() == 0xFF )
    {
        return FALSE;
    }
#endif

    // ��� ���� ���� ����
    if( IsValidHealth() == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

// �ｺ ��� ���� ���� ���� �˻�
U8  IsStartHealth(void)
{
    // ��� ������ FULL Ÿ���� ������ �Ѵ�...
    if( IsGoingOnDrainWaterFull() == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

/* ���� ��� */
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


// ���� �ð� (��)
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

// ���� �ð� (��)
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


// ���� �ð� ����
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


// ���� �ð� ������ �� �ð� ��ȯ
U8 GetHealthHourAdjust(void)
{
    U8 mu8Hour;
    U8 mu8Mode;


    mu8Hour = GetHealthHour();
    mu8Mode = GetHealthAdjustMode();
    if( mu8Mode == ADJUST_MODE_BEFORE )    // -2�ð� 
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
    else if( mu8Mode == ADJUST_MODE_AFTER )   // +2�ð� 
    {
        /* ��� �ð��� 2�ð� ������ �̵� */
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

// ����� ��ǰ �����?
// RETURN
//  TRUE -��� ��
//  FALSE - �̻�� ��
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

// ���� ���� ���� �˻� 
// RETURN
//  - FALSE ; ���� ����
//  - TRUE :  ���� ������
U8 IsValidHealth(void)
{
    // �÷����� �Ϸ���� �ʾ����� ����
    if( IsDoneFlushInitSetup() == FALSE )
    {
        return FALSE;
    }

    // ���� ������ ����ħ�� �ƴ� ���, 
    if( GetOutageStatus(TYPE_WATER) == FLOW_OUTAGE 
            || GetOutageStatus( TYPE_HOT_WATER ) == FLOW_OUTAGE )
    {
        return FALSE;
    }

    // ���� ����
    if( IsErrorType( ERR_TYPE_STER ) == TRUE )
    {
        return FALSE;
    }

    // ��� ��
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
    static U8 mu8Alaram = FALSE; // �˶� �ð��� �� ���� ���۵ǵ���...
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

    // 22~24�ð� ���̿� �� ��뷮 ������ 7�� ī���͸� �ʱ�ȭ�Ѵ�.
    // �ʱ�ȭ �ϰ�, ��� �ѹ� ���� �Ǳ� ������ ī���ʹ� 8�� �����Ѵ�.
    if( mu32RemainTime <= Health.Conf2Hour )
    {
        if( IsUserUsing() == TRUE )
        {
            Health.UnusedWeekDays = Health.ConfWeekDays;
        }
    }

    // 23~24�ð� ���̿� ���� ������ ������, +1Hour �߰��Ѵ�.
    // ��, 1ȸ�� +1 Hour�� ���� �� �ִ�.
    // �ܿ� �ð� ���.. 1Hour?
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

    // 24�ð� ���..
    if( Health.TargetTimeOut > Health.CurrentTimeOut )
    {
        mu8Action = FALSE;

        Health.CurrentTimeOut++;
    }
    else
    {
        mu8Action = TRUE;

        // �ð� �缳��
        ResetTime();

        // 7�� ���� �� ��뷮�� ������ ���� ��� �ð��� +1�ð� ���.
        MakeOneHourEarly();
    }

    return mu8Action;
}


// ��� ���� �ð� �������� �ð� �缳��
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

// ���� 
void  ControlHealth(void)
{
    static U8 mu8CheckAction = FALSE; 


#if CONFIG_STER
    // RTC ���� 
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

    // Health Mode ����
    if( mu8CheckAction == TRUE )
    {
        // ��ȯ ��� ��� ������ �Ǿ� �ִ� ���, ��� ����
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

