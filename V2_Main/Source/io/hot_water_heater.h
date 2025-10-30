/*
 * 순간 온수 히터 모듈
 * 
 */
#ifndef __HOT_WATER_HEATER_H__
#define __HOT_WATER_HEATER_H__

#include "prj_type.h"

// HEATER MAX/MIN WATT 
#if	1
#define MAX_HEATER_WATT         3400U   /* 3.4kw */
#define MID_HEATER_WATT         2900U   /* 2.9kw */
#define MIN_HEATER_WATT         2300U   /* 2.3kw */

#define HIGH_HEATER_WATT        3200U   /* 3.3kw */
#else // icon_2 40cc로 고려하였을때의 값 
#define MAX_HEATER_WATT         3400U   /* 3.4kw */
#define MID_HEATER_WATT         2700U   /* 2.7kw */
#define MIN_HEATER_WATT         2000U   /* 2.0kw */

#define HIGH_HEATER_WATT        3000U   /* 3.0kw */
#endif

typedef signed short int WATT_T;

// HEATER CLASS (LOW-MID-HIGH)
typedef enum
{
    HEATER_CLASS_LOW,       // 하한치
    HEATER_CLASS_MID,       // 중간 
    HEATER_CLASS_HIGH,      // 상한치
    HEATER_CLASS_NUM
}HeaterClass_T;

typedef struct _base_watt_sensing_
{
    U32 Sum;
    U16 Count;
    U16 Average;

    U16 StableTime;             // 대기 시간
    U16 CheckTime;              // Base Watt 검사 시간 타이머 @10ms..
} AverageAdc_T;

typedef struct _hot_water_heater_
{
    // TEMP
    TEMP_T  TargetOut;          // 히터 추출 목표 온도

    HeaterClass_T Class;

    // HEATER WATT 
    WATT_T RawWatt;             // 측정치 순수 계산값
    WATT_T BaseWatt;            // 측정치
    //WATT_T OffsetWatt;          // 보정치
    WATT_T Watt;                // 최종값 ( BaseWatt + OffsetWatt )

    // HEATER TARGET WATT
    WATT_T RawTargetWatt;       // 목표 WATT ( Raw data )
    WATT_T TargetWatt;          // 목표 WATT ( Raw data * k constat )
    WATT_T OffsetWatt;          // 목표 WATT
} HotWaterHeater_T;

typedef struct _heater_offset_info_
{
    TEMP_T tDelta;

    U8  PrevTrans;
    U8  Trans;

    U16 UpCount;
    U16 DownCount;
    U16 SetCount;

    WATT_T OffsetWatt;
    WATT_T AdjustWatt;
    WATT_T MaxWatt;
    WATT_T MinWatt;
    
} HeaterOffset_T;

// 초기화
void  InitHotWaterHeater(void);

void SetHotWaterHeaterOffsetWatt(WATT_T mWatt);

void  GetHotWaterHeaterData(HotWaterHeater_T *pData);

void SetHeaterTempTargetOut(TEMP_T mTargetTemp);

TEMP_T GetHeaterTempTargetOut(void);

HeaterClass_T GetHeaterClass(void);

WATT_T GetHeaterBaseWatt(void);

WATT_T GetHeaterWatt(void);

// 히터 WATT 측정
void InitCheckHeaterWatt(void);
void CheckHeaterWatt(U16 mu16Output);

// 히터 목표 WATT 계산
void CalcHeaterTargetWatt(F32 mf32LPM);

void CalcOutputOffset(F32 mLPM, TEMP_T tTarget, TEMP_T tHotOut);

U16 CheckOverHeating(I16 mi16Output);

// 히터 offset 출력 값 초기화
void ResetHeaterOffset(void);

// 히터 출력율 계산
U16 CalcHeaterOutput(void);

void PreheatHeaterOutput(void);

#endif /* __HOT_WATER_HEATER_H__ */
