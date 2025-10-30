#ifndef __TEMP_TABLE_H__
#define __TEMP_TABLE_H__

#include "prj_type.h"

#define CONFIG_TEMP_TABLE_HOT           0   // Tank 
#define CONFIG_TEMP_TABLE_COLD_ROOM     1   // 신규 냉수 센서 (20210616)
#define CONFIG_TEMP_TABLE_AMBI          1  
#define CONFIG_TEMP_TABLE_EVA           1  
#define CONFIG_TEMP_TABLE_ICE_TRAY_IN   1  

#define CONFIG_TEMP_TABLE_TDS           0
#define CONFIG_TEMP_TABLE_HOT_OUT       1   // Tankless( HOT OUT, HEATER IN )
#define CONFIG_TEMP_TABLE_HOT_IN        1   // Tankless( HOT IN)

#if CONFIG_TEMP_TABLE_HOT
TEMP_T   ConvAdc2Temp_HotWater(U16 mu16Adc);    
#endif

#if CONFIG_TEMP_TABLE_AMBI
TEMP_T   ConvAdc2Temp_Ambient(U16 mu16Adc);
#endif

#if CONFIG_TEMP_TABLE_EVA
TEMP_T   ConvAdc2Temp_Eva(U16 mu16Adc);
#endif

#if CONFIG_TEMP_TABLE_COLD_ROOM
TEMP_T   ConvAdc2Temp_ColdWater(U16 mu16Adc);
TEMP_T   ConvAdc2Temp_RoomWater(U16 mu16Adc);
#endif

#if CONFIG_TEMP_TABLE_TDS
TEMP_T   ConvAdc2Temp_TDSIn(U16 mu16Adc);
TEMP_T   ConvAdc2Temp_TDSOut(U16 mu16Adc);
#endif

#if CONFIG_TEMP_TABLE_HOT_OUT
TEMP_T   ConvAdc2Temp_HotOut(U16 mu16Adc);  // 순간 온수 / 온수 히터 내부 
#endif


#if CONFIG_TEMP_TABLE_HOT_IN
TEMP_T   ConvAdc2Temp_HotIn(U16 mu16Adc);   // 순간 입수 
#endif

#if CONFIG_TEMP_TABLE_ICE_TRAY_IN   
TEMP_T   ConvAdc2Temp_IceTrayIn(U16 mu16Adc);   // ICE TRAY 입수 온도
#endif

#endif /* __TEMP_TABLE_H__ */
