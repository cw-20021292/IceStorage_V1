#include "hal_adc.h"

#include "power_saving.h"
#include "eeprom.h"
#include "comp.h"
#include "rtc.h"


#define COMP_ON_DELAY_TIME  300UL   // 5min.. @1sec

/* 0 ~ 24Hour */
/* default on tiem - 24:00 */
#define DEFAULT_ON_HOUR    0U      
#define DEFAULT_ON_MIN     0U

/* default off time - 06:00 */
#define DEFAULT_OFF_HOUR   6U
#define DEFAULT_OFF_MIN    0U

PowerSaving_T   Saving;


void InitSaving(void)
{
    Saving.Config = FALSE;

    Saving.PrevStatus = STATUS_WAKE_UP;       
    Saving.Status     = STATUS_WAKE_UP;    

    Saving.ConfHour[ ALARM_ON ] = DEFAULT_ON_HOUR;
    Saving.ConfMin[ ALARM_ON ]  = DEFAULT_ON_MIN;

    Saving.ConfHour[ ALARM_OFF ] = DEFAULT_OFF_HOUR;
    Saving.ConfMin[ ALARM_OFF ] = DEFAULT_OFF_MIN;
}

void GetSavingData(PowerSaving_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Saving, sizeof( PowerSaving_T ));
}

void SetSavingConfig(U8 mu8Val )
{
    if( mu8Val != TRUE && mu8Val != FALSE )
    {
        return ;
    }

    if( mu8Val != Saving.Config )
    {
        SaveEepromId( EEP_ID_CONF_POWER_SAVING );
    }

    Saving.Config = mu8Val;
}

U8 GetSavingConfig(void)
{
    return Saving.Config;
}

static U32 ConvTime( U8 mHour, U8 mMin, U8 mSec)
{
    U32 the_time;

    the_time = (mHour * 60UL) + mMin;
    the_time *= 60UL;
    the_time += mSec;

    return the_time;
}

#if 0
void SetSavingAlarmTime(U8 mType, U8 mHour, U8 mMin)
{
    Saving.ConfHour[ mType ] = mHour;
    Saving.ConfMin[ mType ] = mMin;

    SaveEepromId( EEP_ID_POWER_SAVING_TIME );
}

void GetSavingAlarmTime(U8 mType, U8 *pHour, U8 *pMin)
{
    if( pHour != NULL && pMin != NULL )
    {
        *pHour = Saving.ConfHour[ mType ];
        *pMin = Saving.ConfMin[ mType ];
    }
}
#endif
void SetSavingAlarmHour(U8 mType, U8 mHour)
{
    Saving.ConfHour[ mType ] = mHour;

    SaveEepromId( EEP_ID_POWER_SAVING_TIME );
}

U8 GetSavingAlarmHour(U8 mType)
{
    return Saving.ConfHour[ mType ];
}

void SetSavingAlarmMin(U8 mType, U8 mMin)
{
    Saving.ConfMin[ mType ] = mMin;

    SaveEepromId( EEP_ID_POWER_SAVING_TIME );
}

U8 GetSavingAlarmMin(U8 mType)
{
    return Saving.ConfMin[ mType ];
}

U8 GetSavingStatus(void)
{
    if( Saving.Config == FALSE )
    {
        return STATUS_WAKE_UP;
    }

    return Saving.Status;
}



void WakeUpSavingMode(void)
{
#if 0
    // 현재 운전 정지 상태이면,
    // 절전 해제 후, 5분 동안 COMP 중지 상태 유지한다.
    if( Saving.Status == STATUS_SLEEP 
            && GetCompOnOff() == FALSE )
    {
        SetCompProtectOffTime( COMP_ON_DELAY_TIME );
    }

    Saving.Status       = STATUS_WAKE_UP;
#endif
}


#if CONFIG_TEST_8585
void ProcessSaving(void)
{
    // empty..
    Saving.Status = STATUS_WAKE_UP;
}
#else
void ProcessSaving(void)
{
    TimeData_T  mTime;


    GetRtcTime( &mTime );
    Saving.CurrentTime  = ConvTime( mTime.Hour, mTime.Min, mTime.Sec );
    Saving.OffTime      = ConvTime( Saving.ConfHour[ALARM_OFF], Saving.ConfMin[ALARM_OFF], 0U );
    Saving.OnTime       = ConvTime( Saving.ConfHour[ALARM_ON], Saving.ConfMin[ALARM_ON], 0U );


    // OFF 시간과 ON 시간이 같으면 절전 해제...
    if( Saving.OffTime == Saving.OnTime )
    {
        Saving.Status = STATUS_WAKE_UP;
    }
    else if( Saving.OffTime > Saving.OnTime )
    {
        if( Saving.CurrentTime > Saving.OnTime 
                && Saving.CurrentTime < Saving.OffTime )
        {
            Saving.Status = STATUS_SLEEP;
        }
        else
        {
            Saving.Status = STATUS_WAKE_UP;
        }
    }
    else 
    {
        if( Saving.CurrentTime > Saving.OnTime 
                || Saving.CurrentTime < Saving.OffTime )
        {
            Saving.Status = STATUS_SLEEP;
        }
        else
        {
            Saving.Status = STATUS_WAKE_UP;
        }
    }
}
#endif

