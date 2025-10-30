#ifndef __FLOW_VALVE_H__
#define __FLOW_VALVE_H__

#include "prj_type.h"
#include "hal_step_motor.h"


typedef struct _flow_valve_
{
    /* STATUS */
    U8 Status;       
    
    /* MODE */
    U8 Mode;         
    U8 InitStep;
    U8 OpenStep;
    U8 TargetStep;
    U8 OffsetStep;
    U8 TestStep;

    sm_step_t Offset;
    sm_step_t Target;

} FlowValve_T;

void InitFlowValve(void);

void  GetFlowValveData(FlowValve_T *pData);

#define  FLOW_VALVE_MOVING   0     
#define  FLOW_VALVE_STOP     1     
U8 GetFlowValveStatus(void);

// 밸브 초기화 
void FlowValveInit(void);

// 밸브 최대 OPEN
void FlowValveFullOpen(void);

// 밸브 target 위치로 이동
#define MIN_FLOW_VALVE_TARGET   700         // about under 0.3 LPM
#define MAX_FLOW_VALVE_TARGET   1700        // about over 1.1 LPM
#define DEFAULT_FLOW_TARGET     1600        // about 1 LPM
void FlowValveTarget(I16 target);

I16 GetFlowValveTarget(void);

// 밸브 현재 위치 기준에서 offset 만큼 이동
void FlowValveOffset(I16 offset);

// 밸브 제어
void ControlFlowValve(void);

// TEST
void TEST_StartFlowValve(void);
void TEST_StopFlowValve(void);
void TEST_RepeatFlowValve(void);

#endif /* __FLOW_VALVE_H__ */
