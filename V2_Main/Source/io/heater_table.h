#ifndef __HEATER_TABLE_H__
#define __HEATER_TABLE_H__

#include "prj_type.h"
#include "hot_water_heater.h"

// 1도를 올리는데 필요한 WATT
WATT_T GetWattByLPM(F32 mLPM);

#if 0
// 와트에 따른 
U16 GetHeatTime(U16 mWatt, TEMP_T tTarget, TEMP_T tIn);
#endif

#endif /* __HEATER_TABLE_H__ */
