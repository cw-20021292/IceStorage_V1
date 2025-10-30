#include "hw.h"
#include "diagnosis_12v.h"

#include "hal_step_motor.h"
#include "diagnosis_common.h"
#include "diagnosis.h"
#include "monitoring.h"
#include "smart_check.h"
#include "gas_switch_valve.h"


DoDiagnosis12v_T      Diagnosis12v;

void SetDiagnosisPart12V(U16 mu16Part)
{
    Diagnosis12v.Mode |= mu16Part;
}

U16 GetDiagnosisPart12V(void)
{
    return Diagnosis12v.Mode;
}

void ClearDiagnosisPart12V(U16 mu16Part)
{
    Diagnosis12v.Mode &= ~mu16Part;
}

U16 IsSetDiagnosisPart12V(U16 mu16Part)
{
    if ( (Diagnosis12v.Mode & mu16Part ) == mu16Part )
    {
        return TRUE;
    }
    
    return FALSE;
}

void StartDiagnosis12V(void)
{
    Diagnosis12v.Mode = DIAGNOSIS_PART_12V;
}

void StopDiagnosis12V(void)
{
    Diagnosis12v.Mode = DIAGNOSIS_PART_12V_NONE;

    // TODO:
    TURN_OFF_SMPS_12V();
    TURN_OFF_STEP_MOTOR_GAS_SW_1();
    TURN_OFF_STEP_MOTOR_GAS_SW_2();
    TURN_OFF_STEP_MOTOR_GAS_SW_3();
    TURN_OFF_STEP_MOTOR_GAS_SW_4();

    HAL_SetInActive(SM_ID_2, 0 );
    GasSwitchInit();
    ClearLoadAdc(INPUT_SRC_12V);
    ClearLoadCurrent(INPUT_SRC_12V);
}


// 12V LOAD
static void InitDiagnosis12vInit(void)
{
    Diagnosis12v.StepInit = 0;
    Diagnosis12v.StepGasSwitch = 0;

    Diagnosis12v.TimeOut = 0;

    Diagnosis12v.Current = 0;
}

static DStatus_T DoDiagnosis12vInit(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(Diagnosis12v.StepInit)
    {
        case STEP_12V_INIT_START :
            Diagnosis12v.StepInit++;
            Diagnosis12v.TimeOut = STER_WAIT_LOAD_CLEAR;

            // TODO:
            TURN_ON_SMPS_12V();
            TURN_OFF_STEP_MOTOR_GAS_SW_1();
            TURN_OFF_STEP_MOTOR_GAS_SW_2();
            TURN_OFF_STEP_MOTOR_GAS_SW_3();
            TURN_OFF_STEP_MOTOR_GAS_SW_4();

            HAL_SetInActive(SM_ID_2, 1 );
            //R_TAU0_Channel1_Stop();   
            break;

        case STEP_12V_INIT_WAIT_STOP_LOAD :
            if( GetLoadPowerOnOffStatus(INPUT_SRC_12V) == 0 )
            {
                ClearLoadAdc(INPUT_SRC_12V);
                ClearLoadCurrent(INPUT_SRC_12V);
                Diagnosis12v.StepInit++;
            }
            else
            {
                if ( Diagnosis12v.TimeOut != 0 )
                {
                    Diagnosis12v.TimeOut--;
                }
                else
                {
                    mStatus = STATUS_ERR;
                    Diagnosis12v.StepInit = STEP_12V_INIT_DONE;
                }
            }
            break;

        case STEP_12V_INIT_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             Diagnosis12v.StepInit = 0;
            break;
    }

    return mStatus;
}

static DStatus_T DoDiagnosisGasSwitch(void)
{
    DStatus_T mStatus = STATUS_GOING;


    switch(Diagnosis12v.StepGasSwitch)
    {
        case STEP_12V_GAS_SWITCH_1 :
            TURN_ON_STEP_MOTOR_GAS_SW_1();
            Diagnosis12v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis12v.StepGasSwitch++;
            break;

        case STEP_12V_GAS_SWITCH_1_WAIT :
            if( Diagnosis12v.TimeOut != 0 )
            {
                Diagnosis12v.TimeOut--;
            }
            else
            {
                Diagnosis12v.Current = GetLoadCurrent(WIFI_PART_0808_ICE_COURSE_CHANGE_VV);
                TURN_OFF_STEP_MOTOR_GAS_SW_1();
                Diagnosis12v.StepGasSwitch++;
            }
            break;

        case STEP_12V_GAS_SWITCH_2 :
            TURN_ON_STEP_MOTOR_GAS_SW_2();
            Diagnosis12v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis12v.StepGasSwitch++;
            break;

        case STEP_12V_GAS_SWITCH_2_WAIT :
            if( Diagnosis12v.TimeOut != 0 )
            {
                Diagnosis12v.TimeOut--;
            }
            else
            {
                Diagnosis12v.Current += GetLoadCurrent(WIFI_PART_0808_ICE_COURSE_CHANGE_VV);
                TURN_OFF_STEP_MOTOR_GAS_SW_2();
                Diagnosis12v.StepGasSwitch++;
            }
            break;

        case STEP_12V_GAS_SWITCH_3 :
            TURN_ON_STEP_MOTOR_GAS_SW_3();
            Diagnosis12v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis12v.StepGasSwitch++;
            break;

        case STEP_12V_GAS_SWITCH_3_WAIT :
            if( Diagnosis12v.TimeOut != 0 )
            {
                Diagnosis12v.TimeOut--;
            }
            else
            {
                Diagnosis12v.Current += GetLoadCurrent(WIFI_PART_0808_ICE_COURSE_CHANGE_VV);
                TURN_OFF_STEP_MOTOR_GAS_SW_3();
                Diagnosis12v.StepGasSwitch++;
            }
            break;

        case STEP_12V_GAS_SWITCH_4 :
            TURN_ON_STEP_MOTOR_GAS_SW_4();
            Diagnosis12v.TimeOut = STER_WAIT_LOAD_OP_3S;
            Diagnosis12v.StepGasSwitch++;
            break;

        case STEP_12V_GAS_SWITCH_4_WAIT :
            if( Diagnosis12v.TimeOut != 0 )
            {
                Diagnosis12v.TimeOut--;
            }
            else
            {
                Diagnosis12v.Current += GetLoadCurrent(WIFI_PART_0808_ICE_COURSE_CHANGE_VV);
                TURN_OFF_STEP_MOTOR_GAS_SW_4();
                Diagnosis12v.StepGasSwitch++;
            }
            break;

        case STEP_12V_GAS_SWITCH_DONE :
            Diagnosis12v.Current /= 4;
            SetSmartCheckDataId(WIFI_PART_0808_ICE_COURSE_CHANGE_VV, Diagnosis12v.Current);
            mStatus = STATUS_DONE;
            break;

        default :
             Diagnosis12v.StepGasSwitch = 0;
            break;
    }

    return mStatus;
}



static const DiagnosisEventList_T    DiagnosisPart12VEventList[] =
{
/*      Diagnosis Part,                 Function,                 Initial,      */
    { DIAGNOSIS_PART_12V_INIT,          DoDiagnosis12vInit,       InitDiagnosis12vInit },
    { DIAGNOSIS_PART_12V_GAS_SWITCH,    DoDiagnosisGasSwitch,     InitDiagnosis12vInit },
};
#define SZ_DIAGNOSIS_PART_12V_EVENT_LIST ( sizeof(DiagnosisPart12VEventList) / sizeof(DiagnosisEventList_T) )

 void DiagnosisPart12VEventHandler(void)
{
    DStatus_T mStatus;
    U8 i;
    DiagnosisEventList_T *pList = NULL;

    for ( i = 0; i < SZ_DIAGNOSIS_PART_12V_EVENT_LIST ; i++ )
    {
        pList = &DiagnosisPart12VEventList[ i ];

        // Check Mode
        if (IsSetDiagnosisPart12V(pList->Part) == TRUE )
        {
            // Do action
            mStatus = pList->DoAction();
            if ( mStatus == STATUS_DONE )               //Do function
            {
                pList->InitAction();                    // Initial Function
                ClearDiagnosisPart12V( pList->Part );   // Mode Clear
            }
            else if( mStatus == STATUS_ERR )
            {
                // 진단 중 에러 조건 발생되면  중지
                StopDiagnosis();
            }
            break;
        }
    }
}


