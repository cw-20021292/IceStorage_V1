#ifndef __DIAGNOSIS_FEED_BACK_H__
#define __DIAGNOSIS_FEED_BACK_H__

#include "prj_type.h"

/*진단 FEEDBACK 초기화 - INIT */
typedef enum
{
    STEP_FEEDBACK_INIT_START,             // 모든 부하 정지
    STEP_FEEDBACK_INIT_WAIT_STOP_LOAD,    // 모든 부하 정지 Wait ( 24V, 12V, AC, Feedback 부하가 모두 OFF인지 체크 )
    STEP_FEEDBACK_INIT_DONE
} DiagnosisFeedbackInitStep_T;

/*진단 FEEDBACK Uv - UV */
typedef enum
{
    STEP_FEEDBACK_UV_WATER_OUT,
    STEP_FEEDBACK_UV_WATER_OUT_WAIT,
    STEP_FEEDBACK_UV_ICE_DOOR,
    STEP_FEEDBACK_UV_ICE_DOOR_WAIT,
    STEP_FEEDBACK_UV_ICE_TANK,
    STEP_FEEDBACK_UV_ICE_TANK_WAIT,
    STEP_FEEDBACK_UV_ICE_TRAY,
    STEP_FEEDBACK_UV_ICE_TRAY_WAIT,
    STEP_FEEDBACK_UV_DONE
} DiagnosisFeedbackUvStep_T;

/*진단 - HEATER */
typedef enum
{
    STEP_FEEDBACK_HEATER_1,
    STEP_FEEDBACK_HEATER_1_WAIT,
    STEP_FEEDBACK_HEATER_2,
    STEP_FEEDBACK_HEATER_2_WAIT,
    STEP_FEEDBACK_HEATER_DONE
} DiagnosisFeedbackHeaterStep_T;

/*진단 - DRAIN PUMP */
typedef enum
{
    STEP_FEEDBACK_DRAIN_PUMP,
    STEP_FEEDBACK_DRAIN_PUMP_WAIT,
    STEP_FEEDBACK_DRAIN_PUMP_DONE
} DiagnosisFeedbackDrainPumpStep_T;

/*진단 - FAN */
typedef enum
{
    STEP_FEEDBACK_FAN,
    STEP_FEEDBACK_FAN_WAIT,
    STEP_FEEDBACK_FAN_DONE
} DiagnosisFeedbackFanStep_T;


typedef struct _do_diagnosis_feedback_
{
    U16 Mode;

    // Step
    DiagnosisFeedbackInitStep_T           StepInit;
    DiagnosisFeedbackUvStep_T             StepUv;
    DiagnosisFeedbackDrainPumpStep_T      StepDrainPump;
    DiagnosisFeedbackFanStep_T            StepFan;
    DiagnosisFeedbackHeaterStep_T         StepHeater;

    // Op Time
    U16 TimeOut;
} DoDiagnosisFeedback_T;

// FEEDBACK LOAD
#define DIAGNOSIS_PART_FEEDBACK_NONE            (0x0000)
#define DIAGNOSIS_PART_FEEDBACK_INIT            (0x0001)
#define DIAGNOSIS_PART_FEEDBACK_UV              (0x0002)
#define DIAGNOSIS_PART_FEEDBACK_DRAIN_PUMP      (0x0004)
#define DIAGNOSIS_PART_FEEDBACK_FAN             (0x0008)
#define DIAGNOSIS_PART_FEEDBACK_HEATER          (0x0010)
#define DIAGNOSIS_PART_FEEDBACK  \
    (DIAGNOSIS_PART_FEEDBACK_INIT|\
     DIAGNOSIS_PART_FEEDBACK_UV| \
     DIAGNOSIS_PART_FEEDBACK_DRAIN_PUMP|\
     DIAGNOSIS_PART_FEEDBACK_FAN|\
     DIAGNOSIS_PART_FEEDBACK_HEATER)

#define DIAGNOSIS_PART_FEEDBACK_CP  \
    (DIAGNOSIS_PART_FEEDBACK_INIT|\
     DIAGNOSIS_PART_FEEDBACK_UV| \
     DIAGNOSIS_PART_FEEDBACK_DRAIN_PUMP|\
     DIAGNOSIS_PART_FEEDBACK_FAN)


void SetDiagnosisPartFeedback(U16 mu16Part);
U16 GetDiagnosisPartFeedback(void);
void ClearDiagnosisPartFeedback(U16 mu16Part);
U16 IsSetDiagnosisPartFeedback(U16 mu16Part);
void StartDiagnosisFeedback(void);
void StopDiagnosisFeedback(void);

void DiagnosisPartFeedbackEventHandler(void);
#endif  /* __DIAGNOSIS_H__ */
