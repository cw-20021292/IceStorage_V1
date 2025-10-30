#ifndef __HEALTH_H__
#define __HEALTH_H__

#include "prj_type.h"

typedef struct _health_
{
    U8 Mode;      // ��� ( ���, ��� )

    U8 Hour;      // ���� �ð�(��, 0 ~ 23 )
    U8 Min;       // ���� �ð�(��, 0, 30 )
    U8 AdjustMode; // ���� ���� ��� ( ADJUST_MODE_NONE, BEFORE, AFTER )


    U32 ConfTargetTime;
    U32 Conf1Hour;
    U32 Conf2Hour;
    U32 ConfMaxTargetTime;
    U8  ConfWeekDays;

    U8  UnusedWeekDays;  // �Ϲ� ��, 7�� ī����
    U32 TargetTimeOut;   // �Ϲݸ� ��� ��ǥ �ð�  (24hour)
    U32 CurrentTimeOut;  // �Ϲݸ� ��� ���� �ð�
} Health_T;


void  InitHealth(void);
void  GetHealthData(Health_T *pData);

// �ｺ ��� ���� ���� ����
// Return : 
// TRUE - ���� ����, FALSE - ���� �Ұ�
U8    IsConfHealth(void);

// �ｺ ��� ���� ������ �˻� 
// Return :
// TRUE - ���� ��, FALSE - ���� ��
U8    IsStartHealth(void);

#define  HEALTH_MODE_NONE     0x00  // ����
#define  HEALTH_MODE_STER     0x01  // ���
#define  HEALTH_MODE_DRAIN    0x02  // ��� 
#define  HEALTH_MODE_ALL      ( HEALTH_MODE_STER | HEALTH_MODE_DRAIN )
void  SetHealthModeVal(U8 mu8Mode );
void  SetHealthMode(U8 mu8Mode );
void  ClearHealthMode(U8 mu8Mode );
U8    IsSetHealthMode(U8 mu8Mode );
U8    GetHealthMode(void);

// ���� �ð� (��)
void SetHealthHour(U8 mu8Val );
U8 GetHealthHour(void);

// ���� �ð� (��)
void SetHealthMin(U8 mu8Val );
U8 GetHealthMin(void);

// ���� �ð� ����
#define  ADJUST_MODE_NONE     0   
#define  ADJUST_MODE_BEFORE   1   
#define  ADJUST_MODE_AFTER    2
U8 GetHealthAdjustMode(void);
void SetHealthAdjustMode(U8 mu8Mode);
U8 GetHealthHourAdjust(void);

// 24hour = 24h x 60min x 60sec = 86400sec 
#define  NONE_RTC_TARGET_TIME      86400UL
// 1hour = 1h x 60min x 60sec = 
#define  NONE_RTC_1HOUR            3600UL
#define  NONE_RTC_2HOUR            7200UL
#define  MAX_NONE_RTC_TIME_OUT     ( NONE_RTC_TARGET_TIME + NONE_RTC_1HOUR )
#define  DEFAULT_UNUSED_WEEK_DAYS  7U
void SetHealthConfDrainWater(U32 mu32Target, U32 mu32Hour_1, U32 mu32Hour_2, U8 mu8Days);

U8 IsValidHealth(void);

U8 GetAlaramByNoneRTC(U8 mu8Action);

// Test
void UpdateHealthOnTime(void);
void UpdateHealthOnTime_2(void);
void UpdateHealthOnTimeOneHour(void);
void UpdateHealthOnTimeTwoHour(void);
void UpdateHealthOnTimeWeek(void);

// ����
void  ControlHealth(void);


#endif /* __HEALTH_H__ */
