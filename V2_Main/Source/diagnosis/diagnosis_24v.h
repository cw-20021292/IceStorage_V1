#ifndef __DIAGNOSIS_24V_H__
#define __DIAGNOSIS_24V_H__

#include "prj_type.h"

/*ÏßÑÎã® 24V Ï¥àÍ∏∞?ôî - INIT */
typedef enum
{
    STEP_24V_INIT_START,             // Î™®Îì† Î∂??ïò ?†ïÏß?
    STEP_24V_INIT_WAIT_STOP_LOAD,    // Î™®Îì† Î∂??ïò ?†ïÏß? Wait ( 24V, 12V, AC, Feedback Î∂??ïòÍ∞? Î™®Îëê OFF?ù∏Ïß? Ï≤¥ÌÅ¨ )
    STEP_24V_INIT_DONE
} Diagnosis24vInitStep_T;

/*ÏßÑÎã® 24V Î∞∏Î∏å - VALVE */
//per 1sec = 10sec
typedef enum
{
    STEP_24V_VALVE_NOS_VALVE,
    STEP_24V_VALVE_NOS_VALVE_WAIT,
    STEP_24V_VALVE_ROOM_COLD_OUT_VALVE,
    STEP_24V_VALVE_ROOM_COLD_OUT_VALVE_WAIT,
    STEP_24V_VALVE_COLD_OUT_VALVE,
    STEP_24V_VALVE_COLD_OUT_VALVE_WAIT,
    STEP_24V_VALVE_HOT_OUT_VALVE,
    STEP_24V_VALVE_HOT_OUT_VALVE_WAIT,
    STEP_24V_VALVE_HOT_IN_VALVE,
    STEP_24V_VALVE_HOT_IN_VALVE_WAIT,
    STEP_24V_VALVE_HOT_DRAIN_VALVE,
    STEP_24V_VALVE_HOT_DRAIN_VALVE_WAIT,
    STEP_24V_VALVE_COLD_AIR_VALVE,
    STEP_24V_VALVE_COLD_AIR_VALVE_WAIT,
    STEP_24V_VALVE_COLD_IN_VALVE,
    STEP_24V_VALVE_COLD_IN_VALVE_WAIT,
    STEP_24V_VALVE_COLD_DRAIN_VALVE,
    STEP_24V_VALVE_COLD_DRAIN_VALVE_WAIT,
    STEP_24V_VALVE_FLUSHING_VALVE,
    STEP_24V_VALVE_FLUSHING_VALVE_WAIT,
    STEP_24V_VALVE_ICE_TRAY_VALVE,
    STEP_24V_VALVE_ICE_TRAY_VALVE_WAIT,
    STEP_24V_VALVE_DONE
} Diagnosis24vValveStep_T;


/*ÏßÑÎã® 24V Î∞∏Î∏å - ICE_DOOR */
// per 3sec = 12s
typedef enum
{
    STEP_24V_ICE_DOOR_1,
    STEP_24V_ICE_DOOR_1_WAIT,
    STEP_24V_ICE_DOOR_2,
    STEP_24V_ICE_DOOR_2_WAIT,
    STEP_24V_ICE_DOOR_3,
    STEP_24V_ICE_DOOR_3_WAIT,
    STEP_24V_ICE_DOOR_4,
    STEP_24V_ICE_DOOR_4_WAIT,
    STEP_24V_ICE_DOOR_DONE
} Diagnosis24vIceDoorStep_T;


/*ÏßÑÎã® 24V Î∞∏Î∏å - FLOW VALVE */
// per 3sec = 12s
typedef enum
{
    STEP_24V_FLOW_VALVE_1,
    STEP_24V_FLOW_VALVE_1_WAIT,
    STEP_24V_FLOW_VALVE_2,
    STEP_24V_FLOW_VALVE_2_WAIT,
    STEP_24V_FLOW_VALVE_3,
    STEP_24V_FLOW_VALVE_3_WAIT,
    STEP_24V_FLOW_VALVE_4,
    STEP_24V_FLOW_VALVE_4_WAIT,
    STEP_24V_FLOW_VALVE_DONE
} Diagnosis24vFlowValveStep_T;


/*ÏßÑÎã® 24V Î∞∏Î∏å - SWING_BAR */
// per 3sec = 3sec
typedef enum
{
    STEP_24V_SWING_BAR,
    STEP_24V_SWING_BAR_WAIT,
    STEP_24V_SWING_BAR_DONE
} Diagnosis24vSwingBarStep_T;



typedef struct _do_diagnosis_24v_
{
    U16 Mode;

    // Step
    Diagnosis24vInitStep_T          StepInit;
    Diagnosis24vValveStep_T         StepValve;
    Diagnosis24vFlowValveStep_T     StepFlowValve;
    Diagnosis24vIceDoorStep_T       StepIceDoor;
    Diagnosis24vSwingBarStep_T      StepSwingBar;

    // Op Time
    U16 TimeOut;

    U16 Current;
} DoDiagnosis24v_T;



// 24V LOAD
#define DIAGNOSIS_PART_24V_NONE       (0x0000)
#define DIAGNOSIS_PART_24V_INIT       (0x0001)
#define DIAGNOSIS_PART_24V_VALVE      (0x0002)
#define DIAGNOSIS_PART_24V_FLOW_VALVE (0x0004)
#define DIAGNOSIS_PART_24V_ICE_DOOR   (0x0008)
#define DIAGNOSIS_PART_24V_SWING_BAR  (0x0010)
#define DIAGNOSIS_PART_24V  \
    (DIAGNOSIS_PART_24V_INIT|\
     DIAGNOSIS_PART_24V_VALVE|\
     DIAGNOSIS_PART_24V_FLOW_VALVE| \
     DIAGNOSIS_PART_24V_ICE_DOOR| \
     DIAGNOSIS_PART_24V_SWING_BAR )

#define DIAGNOSIS_PART_24V_CP \
    (DIAGNOSIS_PART_24V_INIT|\
     DIAGNOSIS_PART_24V_VALVE|\
     DIAGNOSIS_PART_24V_ICE_DOOR| \
     DIAGNOSIS_PART_24V_SWING_BAR )



void SetDiagnosisPart24V(U16 mu16Part);
U16 GetDiagnosisPart24V(void);
void ClearDiagnosisPart24V(U16 mu16Part);
U16 IsSetDiagnosisPart24V(U16 mu16Part);
void StartDiagnosis24V(void);
void StopDiagnosis24V(void);

void DiagnosisPart24VEventHandler(void);
#endif  /* __DIAGNOSIS_H__ */
