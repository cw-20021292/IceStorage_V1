#ifndef __HOT_WATER_FLOW_H__
#define __HOT_WATER_FLOW_H__

#include "prj_type.h"


//
// 순간 온수 히터 목표 유량 계산 모듈
// 
/* TARGET LPM MIN/MAX  */
//#define MIN_LPM    0.5f     // min flow rate 0.50 LPM
#define MIN_LPM    0.3f     // min flow rate 0.50 LPM
#define MAX_LPM    1.0f     // max flow rate 1.00 LPM


typedef struct _hot_water_flow_
{
    // FLOW 
    F32 RawTargetLPM;  // 목표 유량 LPM

    F32 TargetLPM;     // 목표 유량 LPM
    U16 TargetHz;      // 목표 유량 LPM을 HZ로 변환한 값
    U16 TargetStep;

    U16 MaxHz;          
    U16 MinHz;
} HotWaterFlow_T;


// 초기화
void  InitHotWaterFlow(void);

void  GetHotWaterFlowData(HotWaterFlow_T *pData);

// 목표 유량 반환(LPM)
F32 GetHotWaterTargetLPM(void);

// 목표 유량의 반환(HZ)
U16 GetHotWaterTargetHz(void);

void InitOffsetLPM(void);
F32 GetFlowOffsetLPM(void);
void SetFlowOffsetLPM(F32 mLPM);
F32 CalcOffsetLPM(F32 mTargetLPM, TEMP_T tTargetOut, TEMP_T tCurrentOut );

void InitAdjustFlowValve(void);
void AdjustFlowValveByTemp(TEMP_T tTarget, TEMP_T tCurrent);
void AdjustFlowValveByFlow(F32 mTargetLPM, F32 mCurrentLPM);

///////////////////////////////////////////////////////////////////////
// LPM vs step 계산
void InitFlowValveSampleLPM(void);

// 계산된 이력이 있는지?
U8 IsCalSampleLPM(void);

// 유량/밸브 위치 샘플값 측정
void SetFlowValveSampleLPM(F32 mLPM, I16 mStep);

// LPM에 대한 목표 STEP
I16 GetFlowValveStep(F32 mLPM);


// 목표 유량 계산 함수
void CalcHeaterTargetLPM(void);

#endif /* __HOT_WATER_FLOW_H__ */
