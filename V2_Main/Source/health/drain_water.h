#ifndef __DRAIN_WATER_H__
#define __DRAIN_WATER_H__

#include "prj_type.h"

typedef struct _drain_water_
{
    U8 Start;     // 배수 동작 ( 0: 중지, 1: 시작 )
    U8 Mode;      // 타입 ( 0: HALF DRAIN, 1: FULL DRAIN )

    U32 DrainAmount;        // 배수량 
    U32 ConsumColdWater;    // 냉수 최소 물 사용량(시간@100ms)
} DrainWater_T;


void  InitDrainWater(void);
void  GetDrainWaterData(DrainWater_T *pData);

// 배수 시작/중지
#define  HALF_DRAIN  0     // 반만 배출
#define  FULL_DRAIN  1     // 전부 배출
U8 StartDrainWater(U8 mu8Mode);
U8 IsStartDrainWater(void);

// 배수 타입( HALF or FULL )
// RETURN :
// TRUE - DRAIN_HALF 타입
// FALSE - DRAIN_FULL 타입
U8 IsDrainWaterHalf(void);


U8 GetDrainTimePercent(void);

// 순환 강제 배수 중이면, TRUE
// 그렇지 않으면 FALSE
U8 IsGoingOnDrainWaterFull(void);

U8 ResetDrainWater(void);

// 냉수 사용량 적산
void UpdateDrainColdWaterConsume(U32 amount);

void ControlDrainWater(void);

#endif /* __DRAIN_WATER_H__ */
