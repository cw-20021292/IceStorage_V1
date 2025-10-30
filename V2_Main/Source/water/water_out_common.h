#ifndef __WATER_OUT_COMMON_H__
#define __WATER_OUT_COMMON_H__

#include "prj_type.h"
#include "water_out_type.h"

// VALVE WATI TIME
#define WAIT_100MS   10     // 100ms @10ms..
#define WAIT_200MS   20     // 200ms @10ms..
#define WAIT_300MS   30     // 300ms @10ms..
#define WAIT_400MS   40     // 300ms @10ms..
#define WAIT_500MS   50     // 500ms @10ms..
#define WAIT_1000MS  100    // 1sec  @10ms..

#define ML_UNIT 10U /* 최소 단위 10ml */

U32 GetFindOutTargetTime(WaterOutType_T mType);

// 추출 진행량( 0 ~ 100% )
U8 CalcPercent(U32 mu32Target, U32 mu32Current);

// 추출 진행량( mL )
U16 CalcLiter(U32 mu32Amount);

// ml를 추출얄으로 변환
U32 CalcAmount(U16 mLiter);

#ifdef CONFIG_WIFI_WATER_AMOUNT
extern U16 GetWaterAmountConfig(U16 mu8type);
#endif // end of CONFIG_WIFI_WATER_AMOUNT

#endif /* __WATER_OUT_COMMON_H__ */
