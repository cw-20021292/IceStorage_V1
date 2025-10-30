#ifndef __TEMP_H__
#define __TEMP_H__

#include "prj_type.h"


// TEMP ID 
enum
{
    TEMP_ID_AMBIENT,          
    TEMP_ID_ROOM_WATER,    
    TEMP_ID_COLD_WATER,    
    TEMP_ID_EVA_1,              
    TEMP_ID_HOT_OUT,       
    TEMP_ID_HOT_IN,        
    TEMP_ID_HEATER,        
    TEMP_ID_MAX           
};



#define DEFAULT_INIT_TEMP_READ_TIME    600U // 10min @1sec
#define DEFAULT_TEMP_READ_TIME          10U // 10sec @1sec

TEMP_T GetTemp(U8 id);

// 센서 읽기 동작 시간
// 센서 읽기 자연 시간 반환
U16 GetTempSensorTime(U8 mu8Id);

// mu8Id : TEMP_ID_XXXX
// mu16TimeOut : @1sec..
void TurnOnTempSensor(U8 mu8Id, U16 mu16TimeOut);
void TurnOffTempSensor(U8 mu8Id);

// Override
void SetOverrideTemp(U8 mu8Id, U8 mOverride, TEMP_T mTemp );
TEMP_T GetOverrideTemp(U8 mu8Id, U8 mOverride );

void ProcessTemp(void);
#endif /* __TEMP_H__ */
