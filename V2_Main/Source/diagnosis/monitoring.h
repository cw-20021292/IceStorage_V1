#ifndef __MONITORING_H__
#define __MONITORING_H__

#include "prj_type.h"
#include "WIFI_SetPartData.h"



// EMPTY DATA
#define INIT_DATA   9999U

typedef enum _input_src_
{

    INPUT_SRC_12V,
    INPUT_SRC_24V,
    INPUT_SRC_AC,
    INPUT_SRC_FEEDBACK,
    INPUT_SRC_SENSOR,

} InputSrc;


void ClearLoadAdc(U8 Src);
void ClearLoadCurrent(U8 Src);

U16 GetLoadAdc(E_API_A1080_T Id);
void SetLoadAdc(E_API_A1080_T Id, U16 mu16Adc);

U16 GetLoadCurrent(E_API_A1080_T Id);
void SetLoadCurrent(E_API_A1080_T Id, U16 mu16Current);

U8 GetLoadPowerOnOffStatus(U8 Src);

void ControlMonitoring(void);

#endif
