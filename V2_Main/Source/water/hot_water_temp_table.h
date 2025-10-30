#ifndef __HOT_WATER_TEMP_TABLE_H__
#define __HOT_WATER_TEMP_TABLE_H__

#include "prj_type.h"



// 목표 예열 온도 반환
// mu8Sel : 커피, 차, 분유...
// mu8Class : Heater class ( low, high, mid )
// mu8TempRegion : Temp Region ( low, mid, high )
TEMP_T GetHotTargetPreHeatTemp( U8 mu8Sel, U8 mu8Class, U8 mu8TempRegion );

// 목표 제어 온도 반환
TEMP_T GetHotTargetOutTemp( U8 mu8Sel, U8 mu8Class, U8 mu8TempRegion );


#endif /* __HOT_WATER_TEMP_TABLE_H__ */
