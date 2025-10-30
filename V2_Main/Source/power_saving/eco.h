#ifndef __ECO_H__
#define __ECO_H__

#include "prj_type.h"

/* Status */
typedef enum
{
    ECO_STOP,
    ECO_START,
} EcoStatus_T;



typedef struct _eco_mode_
{
    EcoStatus_T Status;          

    U32 StartTime;
    U32 StopTime;

    U8  IsColdOutStopTime;
    U8  IsColdOut;
    U8  ColdOutCount;
    U32 ColdOutTime;

} Eco_T;


void InitEco(void);

EcoStatus_T GetEcoStatus(void);

void ResetEco(void);

void CountEcoColdOut(void);

void ProcessEco(void);

#endif /* __ECO_H__ */
