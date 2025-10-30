#include "hw.h"
#include "diagnosis_24v.h"

#include "diagnosis_common.h"
#include "diagnosis.h"
#include "monitoring.h"
#include "smart_check.h"

#include "front.h"
#include "valve.h"
#include "swing_bar.h"
#include "hal_step_motor.h"


DoDiagnosis24v_T      Diagnosis24v;

void SetDiagnosisPart24V(U16 mu16Part)
{
    Diagnosis24v.Mode |= mu16Part;
}

U16 GetDiagnosisPart24V(void)
{
    return Diagnosis24v.Mode;
}

void ClearDiagnosisPart24V(U16 mu16Part)
{
    Diagnosis24v.Mode &= ~mu16Part;
}

U16 IsSetDiagnosisPart24V(U16 mu16Part)
{
    if ( (Diagnosis24v.Mode & mu16Part ) == mu16Part )
    {
        return TRUE;
    }
    
    return FALSE;
}

void StartDiagnosis24V(void)
{
    if( GetSystem() == SYSTEM_CHP )
    {
        Diagnosis24v.Mode = DIAGNOSIS_PART_24V;
    }
    else
    {
        Diagnosis24v.Mode = DIAGNOSIS_PART_24V_CP;
    }
}

static void InitLoads(void)
{

    CloseValve( VALVE_ALL );
    OpenValve( NORMAL_OPEN_VALVES );

    TURN_OFF_STEP_MOTOR_ICE_DOOR_1();
    TURN_OFF_STEP_MOTOR_ICE_DOOR_2();
    TURN_OFF_STEP_MOTOR_ICE_DOOR_3();
    TURN_OFF_STEP_MOTOR_ICE_DOOR_4();

    TURN_OFF_STEP_MOTOR_FLOW_VALVE_1();
    TURN_OFF_STEP_MOTOR_FLOW_VALVE_2();
    TURN_OFF_STEP_MOTOR_FLOW_VALVE_3();
    TURN_OFF_STEP_MOTOR_FLOW_VALVE_4();

    TURN_OFF_SWING_BAR();

}

void StopDiagnosis24V(void)
{
    Diagnosis24v.Mode = DIAGNOSIS_PART_24V_NONE;

    InitLoads();

    HAL_SetInActive(SM_ID_0, 0 );
    HAL_SetInActive(SM_ID_1, 0 );
    ClearLoadAdc(INPUT_SRC_24V);
    ClearLoadCurrent(INPUT_SRC_24V);
}

// 24V LOAD
static void InitDiagnosis24vInit(void)
{
    Diagnosis24v.StepInit = 0;
    Diagnosis24v.StepValve = 0;
    Diagnosis24v.StepIceDoor = 0;
    Diagnosis24v.StepFlowValve = 0;
    Diagnosis24v.StepSwingBar = 0;

    Diagnosis24v.TimeOut = 0;

    Diagnosis24v.Current = 0;
}

static DStatus_T DoDiagnosis24vInit(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(Diagnosis24v.StepInit)
    {
        case STEP_24V_INIT_START :
            InitLoads();
            HAL_SetInActive(SM_ID_0, 1 );
            HAL_SetInActive(SM_ID_1, 1 );

            Diagnosis24v.StepInit++;
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_CLEAR;
            break;

        case STEP_24V_INIT_WAIT_STOP_LOAD :
            if( GetLoadPowerOnOffStatus(INPUT_SRC_24V) == 0 )
            {
                ClearLoadAdc(INPUT_SRC_24V);
                ClearLoadCurrent(INPUT_SRC_24V);
                Diagnosis24v.StepInit++;
            }
            else
            {
                if ( Diagnosis24v.TimeOut != 0 )
                {
                    Diagnosis24v.TimeOut--;
                }
                else
                {
                    mStatus = STATUS_ERR;
                    Diagnosis24v.StepInit = STEP_24V_INIT_DONE;
                }
            }
            break;

        case STEP_24V_INIT_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             Diagnosis24v.StepInit = 0;
            break;
    }

    return mStatus;
}

static Diagnosis24vInitStep_T DiagValve(Diagnosis24vInitStep_T Step, U16 Valve, E_API_A1080_T API_ID )
{
    if( Step == Diagnosis24v.StepValve)
    {
        CloseValve( Valve );
        Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
        Diagnosis24v.StepValve++;
    }
    else
    {
        if( Diagnosis24v.TimeOut != 0 )
        {
            Diagnosis24v.TimeOut--;
        }
        else
        {
            SetSmartCheckDataId(API_ID, GetLoadCurrent(API_ID));
            OpenValve( Valve );
            Diagnosis24v.StepValve++;
        }

        return TRUE;
    }

    return FALSE;
}


static DStatus_T DoDiagnosisValve(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(Diagnosis24v.StepValve)
    {
        case STEP_24V_VALVE_NOS_VALVE :
            CloseValve( VALVE_NOS );
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
            Diagnosis24v.StepValve++;
            break;

        case STEP_24V_VALVE_NOS_VALVE_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0001_VALVE_NOS, GetLoadCurrent(WIFI_PART_0001_VALVE_NOS));
                OpenValve( VALVE_NOS );
                Diagnosis24v.StepValve++;
            }
            break;

        case STEP_24V_VALVE_ROOM_COLD_OUT_VALVE :
            OpenValve( VALVE_ROOM_COLD_OUT );
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
            Diagnosis24v.StepValve++;
            break;

        case STEP_24V_VALVE_ROOM_COLD_OUT_VALVE_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0003_VALVE_PURE_OUT, GetLoadCurrent(WIFI_PART_0003_VALVE_PURE_OUT));
                CloseValve( VALVE_ROOM_COLD_OUT );
                Diagnosis24v.StepValve++;
            }
            break;

        case STEP_24V_VALVE_COLD_OUT_VALVE :
            OpenValve( VALVE_ROOM_COLD_OUT);
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
            Diagnosis24v.StepValve++;
            break;

        case STEP_24V_VALVE_COLD_OUT_VALVE_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0004_VALVE_COLD_OUT, GetLoadCurrent(WIFI_PART_0004_VALVE_COLD_OUT));
                CloseValve( VALVE_ROOM_COLD_OUT );
                if( GetSystem() == SYSTEM_CHP )
                {
                    Diagnosis24v.StepValve++;
                }
                else
                {
                    Diagnosis24v.StepValve = STEP_24V_VALVE_COLD_AIR_VALVE;
                }
            }
            break;


        case STEP_24V_VALVE_HOT_OUT_VALVE :
            OpenValve( VALVE_HOT_OUT );
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
            Diagnosis24v.StepValve++;
            break;

        case STEP_24V_VALVE_HOT_OUT_VALVE_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0002_VALVE_HOT_OUT, GetLoadCurrent(WIFI_PART_0002_VALVE_HOT_OUT));
                CloseValve( VALVE_HOT_OUT );
                Diagnosis24v.StepValve++;
            }
            break;

        case STEP_24V_VALVE_HOT_IN_VALVE :
            OpenValve( VALVE_HOT_IN );
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
            Diagnosis24v.StepValve++;
            break;

        case STEP_24V_VALVE_HOT_IN_VALVE_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0009_VALVE_HOT_IN, GetLoadCurrent(WIFI_PART_0009_VALVE_HOT_IN));
                CloseValve( VALVE_HOT_IN );
                Diagnosis24v.StepValve++;
            }
            break;


        case STEP_24V_VALVE_HOT_DRAIN_VALVE :
            CloseValve( VALVE_HOT_DRAIN );
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
            Diagnosis24v.StepValve++;
            break;

        case STEP_24V_VALVE_HOT_DRAIN_VALVE_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_000A_VALVE_HOT_DRAIN, GetLoadCurrent(WIFI_PART_000A_VALVE_HOT_DRAIN));
                OpenValve( VALVE_HOT_DRAIN );
                Diagnosis24v.StepValve++;
            }
            break;

        case STEP_24V_VALVE_COLD_AIR_VALVE :
            CloseValve( VALVE_COLD_AIR );
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
            Diagnosis24v.StepValve++;
            break;

        case STEP_24V_VALVE_COLD_AIR_VALVE_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_000B_VALVE_COLD_AIR, GetLoadCurrent(WIFI_PART_000B_VALVE_COLD_AIR));
                OpenValve( VALVE_COLD_AIR );
                Diagnosis24v.StepValve++;
            }
            break;


        case STEP_24V_VALVE_COLD_IN_VALVE :
            OpenValve( VALVE_COLD_IN );
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
            Diagnosis24v.StepValve++;
            break;

        case STEP_24V_VALVE_COLD_IN_VALVE_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_000C_VALVE_COLD_IN, GetLoadCurrent(WIFI_PART_000C_VALVE_COLD_IN));
                CloseValve( VALVE_COLD_IN );
                Diagnosis24v.StepValve++;
            }
            break;

        case STEP_24V_VALVE_COLD_DRAIN_VALVE :
            OpenValve( VALVE_COLD_DRAIN );
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
            Diagnosis24v.StepValve++;
            break;

        case STEP_24V_VALVE_COLD_DRAIN_VALVE_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_000D_VALVE_COLD_DRAIN, GetLoadCurrent(WIFI_PART_000D_VALVE_COLD_DRAIN));
                CloseValve( VALVE_COLD_DRAIN );

                if( GetSystem() == SYSTEM_CHP )
                {
                    Diagnosis24v.StepValve = STEP_24V_VALVE_ICE_TRAY_VALVE;
                }
                else
                {
                    Diagnosis24v.StepValve++;
                }
            }
            break;

        case STEP_24V_VALVE_FLUSHING_VALVE :
            OpenValve( VALVE_COLD_FLUSH );
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
            Diagnosis24v.StepValve++;
            break;

        case STEP_24V_VALVE_FLUSHING_VALVE_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0018_VALVE_FLUSHING, GetLoadCurrent(WIFI_PART_0018_VALVE_FLUSHING));
                CloseValve( VALVE_COLD_FLUSH );
                Diagnosis24v.StepValve++;
            }
            break;

        case STEP_24V_VALVE_ICE_TRAY_VALVE :
            OpenValve( VALVE_ICE_TRAY_IN );
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_1S;
            Diagnosis24v.StepValve++;
            break;

        case STEP_24V_VALVE_ICE_TRAY_VALVE_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0015_VALVE_ICE_TRAY_IN, GetLoadCurrent(WIFI_PART_0015_VALVE_ICE_TRAY_IN));
                CloseValve( VALVE_ICE_TRAY_IN );
                Diagnosis24v.StepValve++;
            }
            break;

        case STEP_24V_VALVE_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             Diagnosis24v.StepValve = 0;
            break;
    }

    return mStatus;
}


static DStatus_T DoDiagnosisFlowValve(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(Diagnosis24v.StepFlowValve)
    {
        case STEP_24V_FLOW_VALVE_1 :
            TURN_ON_STEP_MOTOR_FLOW_VALVE_1();
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis24v.StepFlowValve++;
            break;

        case STEP_24V_FLOW_VALVE_1_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                Diagnosis24v.Current = GetLoadCurrent(WIFI_PART_0304_HOT_FLOW_MOTOR);
                TURN_OFF_STEP_MOTOR_FLOW_VALVE_1();
                Diagnosis24v.StepFlowValve++;
            }
            break;

        case STEP_24V_FLOW_VALVE_2 :
            TURN_ON_STEP_MOTOR_FLOW_VALVE_2();
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis24v.StepFlowValve++;
            break;

        case STEP_24V_FLOW_VALVE_2_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                Diagnosis24v.Current += GetLoadCurrent(WIFI_PART_0304_HOT_FLOW_MOTOR);
                TURN_OFF_STEP_MOTOR_FLOW_VALVE_2();
                Diagnosis24v.StepFlowValve++;
            }
            break;

        case STEP_24V_FLOW_VALVE_3 :
            TURN_ON_STEP_MOTOR_FLOW_VALVE_3();
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis24v.StepFlowValve++;
            break;

        case STEP_24V_FLOW_VALVE_3_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                Diagnosis24v.Current += GetLoadCurrent(WIFI_PART_0304_HOT_FLOW_MOTOR);
                TURN_OFF_STEP_MOTOR_FLOW_VALVE_3();
                Diagnosis24v.StepFlowValve++;
            }
            break;

        case STEP_24V_FLOW_VALVE_4 :
            TURN_ON_STEP_MOTOR_FLOW_VALVE_4();
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis24v.StepFlowValve++;
            break;

        case STEP_24V_FLOW_VALVE_4_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                Diagnosis24v.Current += GetLoadCurrent(WIFI_PART_0304_HOT_FLOW_MOTOR);
                TURN_OFF_STEP_MOTOR_FLOW_VALVE_4();
                Diagnosis24v.StepFlowValve++;
            }
            break;

        case STEP_24V_FLOW_VALVE_DONE :
            Diagnosis24v.Current /= 4;
            SetSmartCheckDataId(WIFI_PART_0304_HOT_FLOW_MOTOR, Diagnosis24v.Current);
            mStatus = STATUS_DONE;
            break;

        default :
             Diagnosis24v.StepFlowValve = 0;
            break;
    }

    return mStatus;
}

static DStatus_T DoDiagnosisIceDoor(void)
{
    DStatus_T mStatus = STATUS_GOING;


    switch(Diagnosis24v.StepIceDoor)
    {
        case STEP_24V_ICE_DOOR_1 :
            TURN_ON_STEP_MOTOR_ICE_DOOR_1();
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis24v.StepIceDoor++;
            break;

        case STEP_24V_ICE_DOOR_1_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                Diagnosis24v.Current = GetLoadCurrent(WIFI_PART_0807_ICE_DOOR_STEPMOTOR);
                TURN_OFF_STEP_MOTOR_ICE_DOOR_1();
                Diagnosis24v.StepIceDoor++;
            }
            break;

        case STEP_24V_ICE_DOOR_2 :
            TURN_ON_STEP_MOTOR_ICE_DOOR_2();
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis24v.StepIceDoor++;
            break;

        case STEP_24V_ICE_DOOR_2_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                Diagnosis24v.Current += GetLoadCurrent(WIFI_PART_0807_ICE_DOOR_STEPMOTOR);
                TURN_OFF_STEP_MOTOR_ICE_DOOR_2();
                Diagnosis24v.StepIceDoor++;
            }
            break;

        case STEP_24V_ICE_DOOR_3 :
            TURN_ON_STEP_MOTOR_ICE_DOOR_3();
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis24v.StepIceDoor++;
            break;

        case STEP_24V_ICE_DOOR_3_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                Diagnosis24v.Current += GetLoadCurrent(WIFI_PART_0807_ICE_DOOR_STEPMOTOR);
                TURN_OFF_STEP_MOTOR_ICE_DOOR_3();
                Diagnosis24v.StepIceDoor++;
            }
            break;

        case STEP_24V_ICE_DOOR_4 :
            TURN_ON_STEP_MOTOR_ICE_DOOR_4();
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis24v.StepIceDoor++;
            break;

        case STEP_24V_ICE_DOOR_4_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                Diagnosis24v.Current += GetLoadCurrent(WIFI_PART_0807_ICE_DOOR_STEPMOTOR);
                TURN_OFF_STEP_MOTOR_ICE_DOOR_4();
                Diagnosis24v.StepIceDoor++;
            }
            break;

        case STEP_24V_ICE_DOOR_DONE :
            Diagnosis24v.Current /= 4;
            SetSmartCheckDataId(WIFI_PART_0807_ICE_DOOR_STEPMOTOR, Diagnosis24v.Current);
            mStatus = STATUS_DONE;
            break;

        default :
             Diagnosis24v.StepIceDoor = 0;
            break;
    }

    return mStatus;
}


static DStatus_T DoDiagnosisSwingBar(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(Diagnosis24v.StepSwingBar)
    {
        case STEP_24V_SWING_BAR :
            TURN_ON_SWING_BAR();
            Diagnosis24v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis24v.StepSwingBar++;
            break;

        case STEP_24V_SWING_BAR_WAIT :
            if( Diagnosis24v.TimeOut != 0 )
            {
                Diagnosis24v.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0802_ICE_SWING_BAR, GetLoadCurrent(WIFI_PART_0802_ICE_SWING_BAR));
                TURN_OFF_SWING_BAR();
                Diagnosis24v.StepSwingBar++;
            }
            break;

        case STEP_24V_SWING_BAR_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             Diagnosis24v.StepSwingBar = 0;
            break;
    }


    return mStatus;
}


const static DiagnosisEventList_T    DiagnosisPart24VEventList[] =
{
/*      Diagnosis Part,               Function,            Initial,      */
    { DIAGNOSIS_PART_24V_INIT,        DoDiagnosis24vInit,   InitDiagnosis24vInit   },
    { DIAGNOSIS_PART_24V_VALVE,       DoDiagnosisValve,     InitDiagnosis24vInit   },
    { DIAGNOSIS_PART_24V_FLOW_VALVE,  DoDiagnosisFlowValve, InitDiagnosis24vInit   },
    { DIAGNOSIS_PART_24V_ICE_DOOR,    DoDiagnosisIceDoor,   InitDiagnosis24vInit   },
    { DIAGNOSIS_PART_24V_SWING_BAR,   DoDiagnosisSwingBar,  InitDiagnosis24vInit   },
};
#define SZ_DIAGNOSIS_PART_24V_EVENT_LIST ( sizeof(DiagnosisPart24VEventList) / sizeof(DiagnosisEventList_T) )

void DiagnosisPart24VEventHandler(void)
{
    DStatus_T mStatus;
    U8 i;
    DiagnosisEventList_T *pList = NULL;

    for ( i = 0; i < SZ_DIAGNOSIS_PART_24V_EVENT_LIST ; i++ )
    {
        pList = &DiagnosisPart24VEventList[ i ];

        // Check Mode
        if (IsSetDiagnosisPart24V(pList->Part) == TRUE )
        {
            // Do action
            mStatus = pList->DoAction();
            if ( mStatus == STATUS_DONE )               //Do function
            {
                pList->InitAction();                    // Initial Function
                ClearDiagnosisPart24V( pList->Part );   // Mode Clear
            }
            else if( mStatus == STATUS_ERR )
            {
                // ÏßÑÎã® Ï§? ?óê?ü¨ Ï°∞Í±¥ Î∞úÏÉù?êòÎ©?  Ï§ëÏ??
                StopDiagnosis();
            }
            break;
        }
    }
}

