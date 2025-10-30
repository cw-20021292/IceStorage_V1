#ifndef __HEALTH_H__
#define __HEALTH_H__

#include "prj_type.h"

typedef struct _health_
{
    U8 Mode;      // 모드 ( 살균, 배수 )

    U8 Hour;      // 예약 시간(시, 0 ~ 23 )
    U8 Min;       // 예약 시간(분, 0, 30 )
    U8 AdjustMode; // 예약 보정 모드 ( ADJUST_MODE_NONE, BEFORE, AFTER )


    U32 ConfTargetTime;
    U32 Conf1Hour;
    U32 Conf2Hour;
    U32 ConfMaxTargetTime;
    U8  ConfWeekDays;

    U8  UnusedWeekDays;  // 일반 모델, 7일 카운터
    U32 TargetTimeOut;   // 일반모델 배수 목표 시간  (24hour)
    U32 CurrentTimeOut;  // 일반모델 배수 현재 시간
} Health_T;


void  InitHealth(void);
void  GetHealthData(Health_T *pData);

// 헬스 기능 설정 가능 여부
// Return : 
// TRUE - 설정 가능, FALSE - 설정 불가
U8    IsConfHealth(void);

// 헬스 기능 동작 중인지 검사 
// Return :
// TRUE - 동작 중, FALSE - 정지 중
U8    IsStartHealth(void);

#define  HEALTH_MODE_NONE     0x00  // 없음
#define  HEALTH_MODE_STER     0x01  // 살균
#define  HEALTH_MODE_DRAIN    0x02  // 배수 
#define  HEALTH_MODE_ALL      ( HEALTH_MODE_STER | HEALTH_MODE_DRAIN )
void  SetHealthModeVal(U8 mu8Mode );
void  SetHealthMode(U8 mu8Mode );
void  ClearHealthMode(U8 mu8Mode );
U8    IsSetHealthMode(U8 mu8Mode );
U8    GetHealthMode(void);

// 예약 시간 (시)
void SetHealthHour(U8 mu8Val );
U8 GetHealthHour(void);

// 예약 시간 (분)
void SetHealthMin(U8 mu8Val );
U8 GetHealthMin(void);

// 예약 시간 보정
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

// 제어
void  ControlHealth(void);


#endif /* __HEALTH_H__ */
