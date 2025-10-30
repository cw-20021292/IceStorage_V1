#ifndef __HEATER_OUT_H__
#define __HEATER_OUT_H__

#include "prj_type.h"



// HEATER EFFICIENCY
#define HEATER_EFFICIENCY   0.95f   /* 0.95% */

// HEATER OUTPUT ( POWER )
// 0 ~ 100%
#define MAX_HEATER_OUT      100U       // 2900(~2860) WATT 100%
#define MIN_HEATER_OUT      0U


typedef struct _heater_output_
{
    U16 Output;     // Hetaer Output :  0 ~ 100 

    U8 Region;      // single or double

    // TRIAC OUTPUT
    U16 MaxTick;
    U16 TargetTick;           
    U16 NextTargetTick;           
    U16 CurrentTick;           
} HeaterOut_T;

// 초기화
void  InitHeaterOut(void);
void  GetHeaterOutData(HeaterOut_T *pData);

#define REGION_SINGLE    0
#define REGION_DOUBLE    1
U8 GetHeaterOutRegion(void);
U16 CalcHeaterAdc2Watt(U16 mu16Adc);

void SetHeaterOutput(U16 mu16Output);
U16 GetHeaterOutput(void);

#endif /* __HEATER_OUT_H__ */
