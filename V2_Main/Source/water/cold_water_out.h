#ifndef __COLD_WATER_OUT_H__
#define __COLD_WATER_OUT_H__

#include "prj_type.h"
#include "water_out_type.h"

// 추출 시작 명령
// mType : 추출 타입
void TurnOnColdOut(WaterOutType_T mType);

// 추출 중지 명령
void TurnOffColdOut(void);

// 추출 진행량( 0 ~ 100% )
U8 GetColdOutPercent(void);

// 추출 진행량( 0~ 999 mL )
U16 GetColdOutAmount(void);

// 추출 제어
U8 ControlWaterColdOut(void);

#endif /* __COLD_WATER_OUT_H__ */
