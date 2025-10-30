#include "hw.h"
#include "diagnosis_feedback.h"

#include "diagnosis_common.h"
#include "diagnosis.h"
#include "monitoring.h"
#include "smart_check.h"
#include "front.h"

#include "hal_uv.h"
#include "hal_pump.h"
#include "hal_fan_motor.h"
#include "heater_out.h"


DoDiagnosisFeedback_T DiagnosisFeedback;


void SetDiagnosisPartFeedback(U16 mu16Part)
{
    DiagnosisFeedback.Mode |= mu16Part;
}

U16 GetDiagnosisPartFeedback(void)
{
    return DiagnosisFeedback.Mode;
}

void ClearDiagnosisPartFeedback(U16 mu16Part)
{
    DiagnosisFeedback.Mode &= ~mu16Part;
}

U16 IsSetDiagnosisPartFeedback(U16 mu16Part)
{
    if ( (DiagnosisFeedback.Mode & mu16Part ) == mu16Part )
    {
        return TRUE;
    }
    
    return FALSE;
}


void StartDiagnosisFeedback(void)
{
    if( GetSystem() == SYSTEM_CHP )
    {
        DiagnosisFeedback.Mode = DIAGNOSIS_PART_FEEDBACK;
    }
    else
    {
        DiagnosisFeedback.Mode = DIAGNOSIS_PART_FEEDBACK_CP;
    }
}

void StopDiagnosisFeedback(void)
{
    DiagnosisFeedback.Mode = DIAGNOSIS_PART_FEEDBACK_NONE;

    InitHeaterOut();
    ClearLoadAdc(INPUT_SRC_FEEDBACK);
    ClearLoadCurrent(INPUT_SRC_FEEDBACK);
}


// FEEDBACK LOAD
static void InitDiagnosisFeedbackInit(void)
{
    DiagnosisFeedback.StepInit = 0;
    DiagnosisFeedback.StepUv = 0;
    DiagnosisFeedback.StepDrainPump = 0;
    DiagnosisFeedback.StepFan = 0;
    DiagnosisFeedback.StepHeater = 0;

    DiagnosisFeedback.TimeOut = 0;
}

static DStatus_T DoDiagnosisFeedbackInit(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(DiagnosisFeedback.StepInit)
    {
        case STEP_FEEDBACK_INIT_START :

            HAL_TurnOffUv( HAL_UV_WATER_OUT );
            HAL_TurnOffUv( HAL_UV_ICE_DOOR );
            HAL_TurnOffUv( HAL_UV_ICE_TANK );
            HAL_TurnOffUv( HAL_UV_ICE_TRAY );

            HAL_TurnOffFanMotor();
            HAL_TurnOffPump( HAL_PUMP_DRAIN );

            R_TAU0_Channel2_Stop();
            TURN_OFF_HEATER_TRIAC();
            TURN_OFF_HEATER_RELAY();

            DiagnosisFeedback.StepInit++;
            DiagnosisFeedback.TimeOut = STER_WAIT_LOAD_CLEAR;
            break;

        case STEP_FEEDBACK_INIT_WAIT_STOP_LOAD :
            if( GetLoadPowerOnOffStatus(INPUT_SRC_FEEDBACK) == 0 )
            {
                ClearLoadAdc(INPUT_SRC_FEEDBACK);
                ClearLoadCurrent(INPUT_SRC_FEEDBACK);
                DiagnosisFeedback.StepInit++;
            }
            else
            {
                if ( DiagnosisFeedback.TimeOut != 0 )
                {
                    DiagnosisFeedback.TimeOut--;
                }
                else
                {
                    mStatus = STATUS_ERR;
                    DiagnosisFeedback.StepInit = STEP_FEEDBACK_INIT_DONE;
                }
            }
            break;

        case STEP_FEEDBACK_INIT_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisFeedback.StepInit = 0;
            break;
    }

    return mStatus;
}


static DStatus_T DoDiagnosisDrainPump(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(DiagnosisFeedback.StepDrainPump)
    {
        case STEP_FEEDBACK_DRAIN_PUMP :
            HAL_TurnOnPump( HAL_PUMP_DRAIN, 24.0f );
            DiagnosisFeedback.TimeOut = STER_WAIT_LOAD_OP_3S;
            DiagnosisFeedback.StepDrainPump++;
            break;

        case STEP_FEEDBACK_DRAIN_PUMP_WAIT :
            if( DiagnosisFeedback.TimeOut != 0 )
            {
                DiagnosisFeedback.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_080A_ICE_DRAIN_PUMP, GetLoadCurrent(WIFI_PART_080A_ICE_DRAIN_PUMP));
                HAL_TurnOffPump( HAL_PUMP_DRAIN );
                DiagnosisFeedback.StepDrainPump++;
            }
            break;

        case STEP_FEEDBACK_DRAIN_PUMP_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisFeedback.StepDrainPump = 0;
            break;
    }

    return mStatus;
}


static DStatus_T DoDiagnosisFan(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(DiagnosisFeedback.StepFan)
    {
        case STEP_FEEDBACK_FAN :
            HAL_TurnOnFanMotor();
            DiagnosisFeedback.TimeOut = STER_WAIT_LOAD_OP_3S;
            DiagnosisFeedback.StepFan++;
            break;

        case STEP_FEEDBACK_FAN_WAIT :
            if( DiagnosisFeedback.TimeOut != 0 )
            {
                DiagnosisFeedback.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0204_COLD_FAN, GetLoadCurrent(WIFI_PART_0204_COLD_FAN));
                HAL_TurnOffFanMotor();
                DiagnosisFeedback.StepFan++;
            }
            break;

        case STEP_FEEDBACK_FAN_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisFeedback.StepFan = 0;
            break;
    }

    return mStatus;
}

static DStatus_T DoDiagnosisHeater(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(DiagnosisFeedback.StepHeater)
    {
        case STEP_FEEDBACK_HEATER_1 :
            TURN_ON_HEATER_RELAY();
            DiagnosisFeedback.TimeOut = STER_WAIT_LOAD_OP_2S;
            DiagnosisFeedback.StepHeater++;
            break;

        case STEP_FEEDBACK_HEATER_1_WAIT :
            if( DiagnosisFeedback.TimeOut != 0 )
            {
                DiagnosisFeedback.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0302_HOT_INSTANT_HEATER_1, GetLoadCurrent(WIFI_PART_0302_HOT_INSTANT_HEATER_1));
                TURN_OFF_HEATER_RELAY();
                DiagnosisFeedback.StepHeater++;
            }
            break;

        case STEP_FEEDBACK_HEATER_2 :
            TURN_ON_HEATER_TRIAC();
            DiagnosisFeedback.TimeOut = STER_WAIT_LOAD_OP_2S;
            DiagnosisFeedback.StepHeater++;
            break;

        case STEP_FEEDBACK_HEATER_2_WAIT :
            if( DiagnosisFeedback.TimeOut != 0 )
            {
                DiagnosisFeedback.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0303_HOT_INSTANT_HEATER_2, GetLoadCurrent(WIFI_PART_0303_HOT_INSTANT_HEATER_2));
                TURN_OFF_HEATER_TRIAC();
                DiagnosisFeedback.StepHeater++;
            }
            break;

        case STEP_FEEDBACK_HEATER_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisFeedback.StepHeater = 0;
            break;
    }

    return mStatus;
}


static DStatus_T DoDiagnosisUv(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(DiagnosisFeedback.StepUv)
    {
        case STEP_FEEDBACK_UV_WATER_OUT :
            HAL_TurnOnUv( HAL_UV_WATER_OUT );
            DiagnosisFeedback.TimeOut = STER_WAIT_LOAD_OP_3S;
            DiagnosisFeedback.StepUv++;
            break;

        case STEP_FEEDBACK_UV_WATER_OUT_WAIT :
            if( DiagnosisFeedback.TimeOut != 0 )
            {
                DiagnosisFeedback.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0501_STER_UV_FAUCET, GetLoadCurrent(WIFI_PART_0501_STER_UV_FAUCET));
                HAL_TurnOffUv( HAL_UV_WATER_OUT );
                DiagnosisFeedback.StepUv++;
            }
            break;

        case STEP_FEEDBACK_UV_ICE_DOOR :
            HAL_TurnOnUv( HAL_UV_ICE_DOOR );
            DiagnosisFeedback.TimeOut = STER_WAIT_LOAD_OP_3S;
            DiagnosisFeedback.StepUv++;
            break;

        case STEP_FEEDBACK_UV_ICE_DOOR_WAIT :
            if( DiagnosisFeedback.TimeOut != 0 )
            {
                DiagnosisFeedback.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0502_STER_UV_FAUCET_ICE, GetLoadCurrent(WIFI_PART_0502_STER_UV_FAUCET_ICE));
                HAL_TurnOffUv( HAL_UV_ICE_DOOR );
                DiagnosisFeedback.StepUv++;
            }
            break;

        case STEP_FEEDBACK_UV_ICE_TANK :
            HAL_TurnOnUv( HAL_UV_ICE_TANK );
            DiagnosisFeedback.TimeOut = STER_WAIT_LOAD_OP_3S;
            DiagnosisFeedback.StepUv++;
            break;

        case STEP_FEEDBACK_UV_ICE_TANK_WAIT :
            if( DiagnosisFeedback.TimeOut != 0 )
            {
                DiagnosisFeedback.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0503_STER_UV_ICE_TANK, GetLoadCurrent(WIFI_PART_0503_STER_UV_ICE_TANK));
                HAL_TurnOffUv( HAL_UV_ICE_TANK );
                DiagnosisFeedback.StepUv++;
            }
            break;

        case STEP_FEEDBACK_UV_ICE_TRAY :
            HAL_TurnOnUv( HAL_UV_ICE_TRAY );
            DiagnosisFeedback.TimeOut = STER_WAIT_LOAD_OP_3S;
            DiagnosisFeedback.StepUv++;
            break;

        case STEP_FEEDBACK_UV_ICE_TRAY_WAIT :
            if( DiagnosisFeedback.TimeOut != 0 )
            {
                DiagnosisFeedback.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0504_STER_UV_ICE_TRAY, GetLoadCurrent(WIFI_PART_0504_STER_UV_ICE_TRAY));
                HAL_TurnOffUv( HAL_UV_ICE_TRAY );
                DiagnosisFeedback.StepUv++;
            }
            break;            

        case STEP_FEEDBACK_UV_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisFeedback.StepUv = 0;
            break;
    }

    return mStatus;
}

static const DiagnosisEventList_T    DiagnosisPartFeedbackEventList[] =
{
/*      Diagnosis Part,                       Function,                   Initial,      */
    { DIAGNOSIS_PART_FEEDBACK_INIT,           DoDiagnosisFeedbackInit,    InitDiagnosisFeedbackInit    },    
    { DIAGNOSIS_PART_FEEDBACK_UV,             DoDiagnosisUv,              InitDiagnosisFeedbackInit    },
    { DIAGNOSIS_PART_FEEDBACK_DRAIN_PUMP,     DoDiagnosisDrainPump,       InitDiagnosisFeedbackInit    },
    { DIAGNOSIS_PART_FEEDBACK_FAN,            DoDiagnosisFan,             InitDiagnosisFeedbackInit    },
    { DIAGNOSIS_PART_FEEDBACK_HEATER,         DoDiagnosisHeater,          InitDiagnosisFeedbackInit    },
};
#define SZ_DIAGNOSIS_PART_FEEDBACK_EVENT_LIST ( sizeof(DiagnosisPartFeedbackEventList) / sizeof(DiagnosisEventList_T) )

void DiagnosisPartFeedbackEventHandler(void)
{
    DStatus_T mStatus;
    U8 i;
    DiagnosisEventList_T *pList = NULL;

    for ( i = 0; i < SZ_DIAGNOSIS_PART_FEEDBACK_EVENT_LIST ; i++ )
    {
        pList = &DiagnosisPartFeedbackEventList[ i ];

        // Check Mode
        if (IsSetDiagnosisPartFeedback(pList->Part) == TRUE )
        {
            // Do action
            mStatus = pList->DoAction();
            if ( mStatus == STATUS_DONE )               //Do function
            {
                pList->InitAction();                    // Initial Function
                ClearDiagnosisPartFeedback( pList->Part );   // Mode Clear
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
