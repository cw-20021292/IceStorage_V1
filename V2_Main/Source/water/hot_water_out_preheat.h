#ifndef __HOT_WATER_OUT_PREHEAT_H__
#define __HOT_WATER_OUT_PREHEAT_H__

#include "prj_type.h"
#include "water_out_type.h"


typedef enum _preheat_type_
{
    LOW_PREHEAT,            // 예열 온도 보다 낮음
    MIDDLE_PREHEAT,         // 예열과 목표 사이에 있음
    OVER_PREHEAT            // 목표보다 높음
} PreHeatType_T;


typedef struct _preheat_
{
    PreHeatType_T   Type;
    U8      DispStep;          // 프리히팅 진행 표시 단계 ( 1단, 2단, 3단 )
    U16     HeatingTime;       // 동작 시간
    U16     MaxTimeOut;        // 최대 동작 시간
    U16     CalculatedTimeOut;  // 계산된 동작 시간


    // for debug..
    TEMP_T  tHotIn;
    TEMP_T  tPreheatOut;       // 히팅 종료 시점 온도
    TEMP_T  tPreheatHeater;    // 히팅 종료 시점 온도

    U16     HeaterOnTime;      // 히터 동작 시간
    U16     HeaterConfOnTime;      // 히터 동작 시간
} PreHeat_T;


PreHeatType_T   GetHotOutPreHeatType(void);

#define PREHEAT_PROGRESS_LEVEL_3    2
#define PREHEAT_PROGRESS_LEVEL_2    1
#define PREHEAT_PROGRESS_LEVEL_1    0
// 예열 진행 단계 ( 3단계: 0 ~ 2 )
U8 GetHotOutPreHeatLevel(void);

U8 DoPreHeating(U8 *pStep);

#endif /* __HOT_WATER_OUT_PREHEAT_H__ */
