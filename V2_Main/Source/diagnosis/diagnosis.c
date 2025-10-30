#include "hw.h"
#include "diagnosis.h"

#include "diagnosis_12v.h"
#include "diagnosis_24v.h"
#include "diagnosis_ac.h"
#include "diagnosis_feedback.h"
#include "diagnosis_sensor.h"

#include "front.h"
#include "ice.h"
#include "comp.h"
#include "temp.h"


typedef struct _diagnosis_
{
    U8 Start;
    U8 Done;

    U16 TotalTime;
    U16 CurrentTime;
    U8 Progress;
} Diagnosis_T;

Diagnosis_T Diagnosis;

// 진단 시작/중지
#define DIAG_TIME_CHP   550     // general load..
#define DIAG_TIME_CP    400     // general load...
#define DIAG_TIME_AC    600     // 10sec.. ice tray
static U16 GetDiagnosisRunTime(void)
{
    U16 time;

    if( GetSystem() == SYSTEM_CHP )
    {
        if( GetCompOnOff() == TRUE )
        {
            time =  DIAG_TIME_CHP + DIAG_TIME_AC;
            return time;
        }
        else
        {
            time =  GetDiagnoisCompOnWaitTime();
            time += GetDiagnoisCompOffWaitTime();
            time +=  DIAG_TIME_AC;
            if( time > DIAG_TIME_CHP )
            {
                time -= DIAG_TIME_CHP;
            }

            return time;
        }
    }
    else
    {
        if( GetCompOnOff() == TRUE )
        {
            time =  DIAG_TIME_CP + DIAG_TIME_AC;
            return time;
        }
        else
        {
            time =  GetDiagnoisCompOnWaitTime();
            time += GetDiagnoisCompOffWaitTime();
            time +=  DIAG_TIME_AC;
            if( time > DIAG_TIME_CP )
            {
                time -= DIAG_TIME_CP;
            }

            return time;
        }
    }
}
void StartDiagnosis(void)
{
    Diagnosis.Start = TRUE;
    Diagnosis.CurrentTime = 0;
    Diagnosis.TotalTime = GetDiagnosisRunTime();
    

    StartDiagnosis12V();
    StartDiagnosis24V();
    StartDiagnosisAc();
    StartDiagnosisFeedback();
    StartDiagnosisSensor();

}

void StopDiagnosis(void)
{
    Diagnosis.Start = FALSE;

    StopDiagnosis12V();
    StopDiagnosis24V();
    StopDiagnosisAc();
    StopDiagnosisFeedback();
    StopDiagnosisSensor();
}

U8 IsStartDiagnosis(void)
{
    return Diagnosis.Start;
}


static void SetCompleteDiagnosis(void)
{
    Diagnosis.Done = TRUE;
}

void ClearCompleteDiagnosis(void)
{
    Diagnosis.Done = FALSE;
}

U8 GetCompleteDiagnosis(void)
{
    return Diagnosis.Done;
}


void SetDiagnosisProgress(U16 mu16TotalTime, U16 mu16CurrentTime )
{
    U16 Progress = 0;

    Progress = (U16)(((U32)(mu16CurrentTime * (100UL))) / (U32)mu16TotalTime);

    if( Progress >= 100 )
    {
        Diagnosis.Progress = 100;
    }
    else if( Progress >= 90)
    {
        Diagnosis.Progress = 90;
    }
    else if( Progress >= 80)
    {
        Diagnosis.Progress = 80;
    }
    else if( Progress >= 70)
    {
        Diagnosis.Progress = 70;
    }
    else if( Progress >= 60)
    {
        Diagnosis.Progress = 60;
    }
    else if( Progress >= 50)
    {
        Diagnosis.Progress = 50;
    }
    else if( Progress >= 40)
    {
        Diagnosis.Progress = 40;
    }
    else if( Progress >= 30)
    {
        Diagnosis.Progress = 30;
    }
    else if( Progress >= 20)
    {
        Diagnosis.Progress = 20;
    }
    else if( Progress >= 10)
    {
        Diagnosis.Progress = 10;
    }
    else
    {
        Diagnosis.Progress = 0;
    }
}

U8 GetDiagnosisProgress(void)
{
    return Diagnosis.Progress;
}


void ControlDiagnosis(void)
{
    // 진단 
    if( IsStartDiagnosis() == FALSE )
    {
        return;
    }

    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );
    TurnOnTempSensor( TEMP_ID_ROOM_WATER, DEFAULT_TEMP_READ_TIME );

    // 진단 완료
    if( GetDiagnosisPart12V() == DIAGNOSIS_PART_12V_NONE 
            && GetDiagnosisPart24V() == DIAGNOSIS_PART_24V_NONE 
            && GetDiagnosisPartAc() == DIAGNOSIS_PART_AC_NONE
            && GetDiagnosisPartFeedback() == DIAGNOSIS_PART_FEEDBACK_NONE 
            && GetDiagnosisPartSensor() == DIAGNOSIS_PART_SENSOR_NONE )
    {
        StopDiagnosis();
        SetCompleteDiagnosis();
        SetDiagnosisProgress(Diagnosis.TotalTime, Diagnosis.TotalTime);

        return;
    }

    Diagnosis.CurrentTime++;
    if( Diagnosis.CurrentTime >= Diagnosis.TotalTime )
    {
        Diagnosis.CurrentTime = Diagnosis.TotalTime - 1;
    }
    SetDiagnosisProgress(Diagnosis.TotalTime, Diagnosis.CurrentTime);

    DiagnosisPart12VEventHandler();
    DiagnosisPart24VEventHandler();
    DiagnosisPartAcEventHandler();
    DiagnosisPartFeedbackEventHandler();
    if( GetDiagnosisPart12V() == DIAGNOSIS_PART_12V_NONE
            && GetDiagnosisPart24V() == DIAGNOSIS_PART_24V_NONE )
    {
        DiagnosisPartSensorEventHandler();
    }
}
