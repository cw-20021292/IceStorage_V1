#ifndef __DIAGNOSIS_SENSOR_H__
#define __DIAGNOSIS_SENSOR_H__

#include "prj_type.h"

/*진단 SENSOR 초기화 - INIT */
typedef enum
{
    STEP_SENSOR_INIT_START,             // 모든 부하 정지
    STEP_SENSOR_INIT_WAIT_STOP_LOAD,    // 모든 부하 정지 Wait ( 24V, 12V, AC, Feedback 부하가 모두 OFF인지 체크 )
    STEP_SENSOR_INIT_DONE
} DiagnosisSensorInitStep_T;

/*진단 SENSOR ICE FULLL */
typedef enum
{
    STEP_SENSOR_ICE_FULL,
    STEP_SENSOR_ICE_FULL_WAIT,
    STEP_SENSOR_ICE_FULL_DONE
} DiagnosisSensorIceFullStep_T;

/*진단 SENSOR Leak */
typedef enum
{
    STEP_SENSOR_LEAK,
    STEP_SENSOR_LEAK_WAIT,
    STEP_SENSOR_LEAK_DONE
} DiagnosisSensorLeakStep_T;

/*진단 SENSOR Flow meter */
typedef enum
{
    STEP_SENSOR_FLOW_METER,
    STEP_SENSOR_FLOW_METER_OPEN_DRAIN,
    STEP_SENSOR_FLOW_METER_WAIT,
    STEP_SENSOR_FLOW_METER_DONE
} DiagnosisSensorFlowMeterStep_T;

typedef struct _do_diagnosis_sensor_
{
    U16 Mode;

    // Step
    DiagnosisSensorInitStep_T       StepInit;
    DiagnosisSensorIceFullStep_T    StepIceFull;
    DiagnosisSensorLeakStep_T       StepLeak;
    DiagnosisSensorFlowMeterStep_T  StepFlowMeter;

    U8 Leak;
    U16 LeakCount;

    // Op Time
    U16 TimeOut;
} DoDiagnosisSensor_T;

// SENSOR LOAD
#define DIAGNOSIS_PART_SENSOR_NONE              (0x0000)
#define DIAGNOSIS_PART_SENSOR_INIT              (0x0001)
#define DIAGNOSIS_PART_SENSOR_ICE_FULL          (0x0002)
#define DIAGNOSIS_PART_SENSOR_LEAK              (0x0004)
#define DIAGNOSIS_PART_SENSOR_FLOW_METER        (0x0008)
#define DIAGNOSIS_PART_SENSOR  \
    (DIAGNOSIS_PART_SENSOR_INIT|\
     DIAGNOSIS_PART_SENSOR_ICE_FULL| \
     DIAGNOSIS_PART_SENSOR_LEAK| \
     DIAGNOSIS_PART_SENSOR_FLOW_METER)


void SetDiagnosisPartSensor(U16 mu16Part);
U16 GetDiagnosisPartSensor(void);
void ClearDiagnosisPartSensor(U16 mu16Part);
U16 IsSetDiagnosisPartSensor(U16 mu16Part);
void StartDiagnosisSensor(void);
void StopDiagnosisSensor(void);

void DiagnosisPartSensorEventHandler(void);
#endif  /* __DIAGNOSIS_H__ */
