#ifndef __DIAGNOSIS_AC_H__
#define __DIAGNOSIS_AC_H__

#include "prj_type.h"


/*진단 AC 초기화 - INIT */
typedef enum
{
    STEP_AC_INIT_START,             // 모든 부하 정지
    STEP_AC_INIT_DONE
} DiagnosisAcInitStep_T;


/*진단 12V 아이스 트레이 - ICE TRAY */
typedef enum
{
    STEP_AC_ICE_TRAY_START,
    STEP_AC_ICE_TRAY_CHECKING,
    STEP_AC_ICE_TRAY_DONE
} DiagnosisAcIceTrayStep_T;


/*진단 AC COMP -BLDC COMP */
typedef enum
{
    STEP_AC_BLDC_COMP_CHECK,
    STEP_AC_BLDC_COMP_OFF,
    STEP_AC_BLDC_COMP_OFF_WAIT,
    STEP_AC_BLDC_COMP_ON,
    STEP_AC_BLDC_COMP_WAIT,
    STEP_AC_BLDC_COMP_DONE
} DiagnosisAcBldcCompStep_T;


typedef struct _do_diagnosis_ac_
{
    U16 Mode;

    // Step
    DiagnosisAcInitStep_T        StepInit;
    DiagnosisAcBldcCompStep_T    StepBldcComp;
    DiagnosisAcIceTrayStep_T     StepIceTray;

    // Op Time
    U16 TimeOut;
    
    U8 IsNeedDummy;         // true : 검사 종료 후 dummy...
    U8 Icing;
    U8 Deicing;
    U8 IceTrayStatus;
} DoDiagnosisAc_T;

// AC LOAD
#define DIAGNOSIS_PART_AC_NONE        (0x0000)
#define DIAGNOSIS_PART_AC_INIT        (0x0001)
#define DIAGNOSIS_PART_AC_BLDC_COMP   (0x0002)
#define DIAGNOSIS_PART_AC_ICE_TRAY    (0x0004)
#define DIAGNOSIS_PART_AC  \
    (DIAGNOSIS_PART_AC_INIT|\
     DIAGNOSIS_PART_AC_BLDC_COMP|\
     DIAGNOSIS_PART_AC_ICE_TRAY)
     

void SetDiagnosisPartAc(U16 mu16Part);
U16 GetDiagnosisPartAc(void);
void ClearDiagnosisPartAc(U16 mu16Part);
void StartDiagnosisAc(void);
void StopDiagnosisAc(void);

U16 GetDiagnoisCompOffWaitTime(void);
U16 GetDiagnoisCompOnWaitTime(void);

U16 IsSetDiagnosisPartAc(U16 mu16Part);
void DiagnosisPartAcEventHandler(void);

#endif  /* __DIAGNOSIS_H__ */
