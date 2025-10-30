#ifndef __DIAGNOSIS_12V_H__
#define __DIAGNOSIS_12V_H__

#include "prj_type.h"

/*진단 12V 초기화 - INIT */
typedef enum
{
    STEP_12V_INIT_START,             // 모든 부하 정지
    STEP_12V_INIT_WAIT_STOP_LOAD,    // 모든 부하 정지 Wait ( 24V, 12V, AC, Feedback 부하가 모두 OFF인지 체크 )
    STEP_12V_INIT_DONE
} Diagnosis12vInitStep_T;

/*진단 12V GAS - Gas Switch */
typedef enum
{
    STEP_12V_GAS_SWITCH_1,
    STEP_12V_GAS_SWITCH_1_WAIT,
    STEP_12V_GAS_SWITCH_2,
    STEP_12V_GAS_SWITCH_2_WAIT,
    STEP_12V_GAS_SWITCH_3,
    STEP_12V_GAS_SWITCH_3_WAIT,
    STEP_12V_GAS_SWITCH_4,
    STEP_12V_GAS_SWITCH_4_WAIT,
    STEP_12V_GAS_SWITCH_DONE
} Diagnosis12vGasSwitchStep_T;

typedef struct _do_diagnosis_12v_
{
    U16 Mode;

    // Step
    Diagnosis12vInitStep_T          StepInit;
    Diagnosis12vGasSwitchStep_T     StepGasSwitch;

    // Op Time
    U16 TimeOut;

    U16 Current;
} DoDiagnosis12v_T;



// 12V LOAD
#define DIAGNOSIS_PART_12V_NONE            0x0000
#define DIAGNOSIS_PART_12V_INIT            0x0001
#define DIAGNOSIS_PART_12V_GAS_SWITCH      0x0002
#define DIAGNOSIS_PART_12V  \
    (\
     DIAGNOSIS_PART_12V_INIT|\
     DIAGNOSIS_PART_12V_GAS_SWITCH\
     )


void SetDiagnosisPart12V(U16 mu16Part);
U16 GetDiagnosisPart12V(void);
void ClearDiagnosisPart12V(U16 mu16Part);
U16 IsSetDiagnosisPart12V(U16 mu16Part);

void StartDiagnosis12V(void);
void StopDiagnosis12V(void);

void DiagnosisPart12VEventHandler(void);

#endif  /* __DIAGNOSIS_H__ */
