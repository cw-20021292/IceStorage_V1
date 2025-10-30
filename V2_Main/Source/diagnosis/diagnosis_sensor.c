#include "hw.h"
#include "diagnosis_sensor.h"

#include "diagnosis_common.h"
#include "diagnosis.h"
#include "monitoring.h"
#include "smart_check.h"
#include "valve.h"
#include "flow_meter.h"
#include "leak.h"
#include "hal_adc.h"



DoDiagnosisSensor_T   DiagnosisSensor;


void SetDiagnosisPartSensor(U16 mu16Part)
{
    DiagnosisSensor.Mode |= mu16Part;
}

U16 GetDiagnosisPartSensor(void)
{
    return DiagnosisSensor.Mode;
}

void ClearDiagnosisPartSensor(U16 mu16Part)
{
    DiagnosisSensor.Mode &= ~mu16Part;
}

U16 IsSetDiagnosisPartSensor(U16 mu16Part)
{
    if ( (DiagnosisSensor.Mode & mu16Part ) == mu16Part )
    {
        return TRUE;
    }
    
    return FALSE;
}

void StartDiagnosisSensor(void)
{
    DiagnosisSensor.Mode = DIAGNOSIS_PART_SENSOR;

}

static void InitLoads(void)
{
    TURN_OFF_LEAK_ONOFF();
    TURN_OFF_ICE_INFRARED();

    OpenValve( VALVE_COLD_AIR );
    CloseValve( VALVE_ROOM_COLD_OUT );
    CloseValve( VALVE_COLD_IN );

}

void StopDiagnosisSensor(void)
{
    DiagnosisSensor.Mode = DIAGNOSIS_PART_SENSOR_NONE;

    InitLoads();
    ClearLoadAdc(INPUT_SRC_SENSOR);
    ClearLoadCurrent(INPUT_SRC_SENSOR);
}


// SENSOR LOAD
static void InitDiagnosisSensorInit(void)
{
    DiagnosisSensor.StepInit        = 0;
    DiagnosisSensor.StepIceFull     = 0;
    DiagnosisSensor.StepLeak        = 0;
    DiagnosisSensor.StepFlowMeter   = 0;

    DiagnosisSensor.TimeOut = 0;
}

static DStatus_T DoDiagnosisSensorInit(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(DiagnosisSensor.StepInit)
    {
        case STEP_SENSOR_INIT_START :
            DiagnosisSensor.StepInit++;
            DiagnosisSensor.TimeOut = STER_WAIT_LOAD_CLEAR;

            InitLoads();

            DiagnosisSensor.Leak = FALSE;   // NORAML
            break;

        case STEP_SENSOR_INIT_WAIT_STOP_LOAD :
            if( GetLoadPowerOnOffStatus(INPUT_SRC_SENSOR) == 0 )
            {
                ClearLoadAdc(INPUT_SRC_SENSOR);
                ClearLoadCurrent(INPUT_SRC_SENSOR);
                DiagnosisSensor.StepInit++;
            }
            else
            {
                if ( DiagnosisSensor.TimeOut != 0 )
                {
                    DiagnosisSensor.TimeOut--;
                }
                else
                {
                    mStatus = STATUS_ERR;
                    DiagnosisSensor.StepInit = STEP_SENSOR_INIT_DONE;
                }
            }
            break;

        case STEP_SENSOR_INIT_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisSensor.StepInit = 0;
            break;
    }


    return mStatus;
}

static DStatus_T DoDiagnosisIceFull(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(DiagnosisSensor.StepIceFull)
    {
        case STEP_SENSOR_INIT_START :
            TURN_ON_ICE_INFRARED();
            DiagnosisSensor.TimeOut = STER_WAIT_LOAD_OP_3S;
            DiagnosisSensor.StepIceFull++;
            break;

        case STEP_SENSOR_INIT_WAIT_STOP_LOAD :
            if( DiagnosisSensor.TimeOut != 0 )
            {
                DiagnosisSensor.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0801_ICE_FULL_SENSOR, GetLoadCurrent(WIFI_PART_0801_ICE_FULL_SENSOR));
                TURN_OFF_ICE_INFRARED();
                DiagnosisSensor.StepIceFull++;
            }
            break;

        case STEP_SENSOR_INIT_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisSensor.StepIceFull = 0;
            break;
    }

    return mStatus;
}

static DStatus_T DoDiagnosisLeak(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(DiagnosisSensor.StepLeak)
    {
        case STEP_SENSOR_LEAK :
            TURN_ON_LEAK_ONOFF();
            DiagnosisSensor.TimeOut = STER_WAIT_LOAD_OP_5S;
            DiagnosisSensor.LeakCount = STER_WAIT_LOAD_OP_2S; // 2sec..
            DiagnosisSensor.Leak = FALSE;   // NORAML

            DiagnosisSensor.StepLeak++;
            break;

        case STEP_SENSOR_LEAK_WAIT :
            if( DiagnosisSensor.TimeOut != 0 )
            {
                DiagnosisSensor.TimeOut--;

                if( HAL_GetAdcValue( ANI_SENSOR_LEAK ) >= LEAK_ADC_LEVEL )
                {
                    // 2Ï¥àÍ∞Ñ ?ú†Ïß?...
                    if( DiagnosisSensor.LeakCount != 0 )
                    {
                        DiagnosisSensor.LeakCount--;
                    }
                }
                else
                {
                    DiagnosisSensor.LeakCount = STER_WAIT_LOAD_OP_2S;
                }
            }
            else
            {
                if( DiagnosisSensor.LeakCount == 0 )
                {
                    DiagnosisSensor.Leak = TRUE;    // LEAK
                }

                SetSmartCheckDataId(WIFI_PART_0402_SENSOR_LEAK, DiagnosisSensor.Leak );
                TURN_OFF_LEAK_ONOFF();
                DiagnosisSensor.StepLeak++;
            }
            break;

        case STEP_SENSOR_LEAK_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisSensor.StepLeak = 0;
            break;
    }

    return mStatus;
}

static DStatus_T DoDiagnosisFlowMeter(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(DiagnosisSensor.StepFlowMeter)
    {
        case STEP_SENSOR_FLOW_METER :
            CloseValve( VALVE_COLD_AIR );
            CloseValve( VALVE_ROOM_COLD_OUT );
            OpenValve( VALVE_COLD_IN );

            DiagnosisSensor.TimeOut = STER_WAIT_LOAD_OP_300MS;
            DiagnosisSensor.StepFlowMeter++;
            break;

        case STEP_SENSOR_FLOW_METER_OPEN_DRAIN:
            if( DiagnosisSensor.TimeOut != 0 )
            {
                DiagnosisSensor.TimeOut--;
            }
            else
            {
                OpenValve( VALVE_COLD_DRAIN );

                DiagnosisSensor.TimeOut = STER_WAIT_LOAD_OP_3S;
                DiagnosisSensor.StepFlowMeter++;
            }
            break;

        case STEP_SENSOR_FLOW_METER_WAIT :
            if( DiagnosisSensor.TimeOut != 0 )
            {
                DiagnosisSensor.TimeOut--;
            }
            else
            {
                SetSmartCheckDataId(WIFI_PART_0403_SENSOR_FLOW, GetFlowMeterCc() );

                OpenValve( VALVE_COLD_AIR );
                CloseValve( VALVE_ROOM_COLD_OUT );
                CloseValve( VALVE_COLD_IN );
                CloseValve( VALVE_COLD_DRAIN );
                DiagnosisSensor.StepFlowMeter++;
            }
            break;

        case STEP_SENSOR_FLOW_METER_DONE:
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisSensor.StepFlowMeter = 0;
            break;
    }

    return mStatus;
}

const static DiagnosisEventList_T    DiagnosisPartSensorEventList[] =
{
/*      Diagnosis Part,                       Function,               Initial,      */
    { DIAGNOSIS_PART_SENSOR_INIT,           DoDiagnosisSensorInit,    InitDiagnosisSensorInit  },    
    { DIAGNOSIS_PART_SENSOR_ICE_FULL,       DoDiagnosisIceFull,       InitDiagnosisSensorInit  }, 
    { DIAGNOSIS_PART_SENSOR_LEAK,           DoDiagnosisLeak,          InitDiagnosisSensorInit  }, 
    { DIAGNOSIS_PART_SENSOR_FLOW_METER,     DoDiagnosisFlowMeter,     InitDiagnosisSensorInit  }, 
};
#define SZ_DIAGNOSIS_PART_SENSOR_EVENT_LIST ( sizeof(DiagnosisPartSensorEventList) / sizeof(DiagnosisEventList_T) )

void DiagnosisPartSensorEventHandler(void)
{
    DStatus_T mStatus;
    U8 i;
    DiagnosisEventList_T *pList = NULL;

    for ( i = 0; i < SZ_DIAGNOSIS_PART_SENSOR_EVENT_LIST ; i++ )
    {
        pList = &DiagnosisPartSensorEventList[ i ];

        // Check Mode
        if (IsSetDiagnosisPartSensor(pList->Part) == TRUE )
        {
            // Do action
            mStatus = pList->DoAction();
            if ( mStatus == STATUS_DONE )               //Do function
            {
                pList->InitAction();                    // Initial Function
                ClearDiagnosisPartSensor( pList->Part );   // Mode Clear
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
