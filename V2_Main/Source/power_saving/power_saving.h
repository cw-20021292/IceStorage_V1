#ifndef __POWER_SAVING_H__
#define __POWER_SAVING_H__

#include "prj_type.h"

/* Status */
#define  STATUS_WAKE_UP    0
#define  STATUS_SLEEP      1

/* ALARAM TIMER */
#define ALARM_ON        0
#define ALARM_OFF       1
#define ALARAM_NUM      2

/* TIMER */
typedef struct _power_saving_
{
    U8 Config;          /* 절전 기능 사용 여부 : TRUE or FALSE */

    U8 PrevStatus;     
    U8 Status;          /* WAKE_UP or SLEEP */

    // ALARM TIMER
    U8 ConfHour[ALARAM_NUM];
    U8 ConfMin[ALARAM_NUM];
    
    U32 OffTime;
    U32 OnTime;
    U32 CurrentTime;

} PowerSaving_T;


void InitSaving(void);
void GetSavingData(PowerSaving_T *pData );

void SetSavingConfig(U8 mu8Val );
U8 GetSavingConfig(void);

#if 0
void SetSavingAlarmTime(U8 mType, U8 mHour, U8 mMin);
void GetSavingAlarmTime(U8 mType, U8 *pHour, U8 *pMin);
#else
void SetSavingAlarmHour(U8 mType, U8 mHour);
U8 GetSavingAlarmHour(U8 mType);

void SetSavingAlarmMin(U8 mType, U8 mMin);
U8 GetSavingAlarmMin(U8 mType);
#endif

U8 GetSavingStatus(void);
void WakeUpSavingMode(void);

void ProcessSaving(void);

#endif /* __POWER_SAVING_H__ */
