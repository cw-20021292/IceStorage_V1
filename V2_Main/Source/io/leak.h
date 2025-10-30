#ifndef __LEAK_H__
#define __LEAK_H__

#include "prj_type.h"

//#define  LEAK_ADC_LEVEL             750 /* 3.6v */        // AIS 3.0 SODA 
//#define  LEAK_ADC_LEVEL             475 /* 3.6v */        // ICON AIS 너무 낮음.. 미감지 되는 현상 있음..  ( guardian )
#define  LEAK_ADC_LEVEL               612 /* 2.3v */        // ICON AIS 

typedef struct _water_leakage_
{
    /* Status */
    U8 Status;      // 누수 발생 상태( TRUE : 누수, FALSE : 정상 )
    
    /* CHECK */
    U16 WaitTime;    // 다음 검사 대기 시간
    U16 CheckTime;   // 검사 시간
    U16 CheckCount;  // 기준치 초과 카운트 
} Leakage_T;

void InitLeak(void);

void GetLeakData( Leakage_T *pData );

U8 GetLeakStatus(void);

U8 IsLeakSemi(void);

void ProcessLeak(void);

#endif /* __LEAK_H__ */
