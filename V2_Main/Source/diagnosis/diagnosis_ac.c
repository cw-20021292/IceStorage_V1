#include "hw.h"
#include "diagnosis_ac.h"


#include "diagnosis_common.h"
#include "diagnosis.h"
#include "monitoring.h"
#include "smart_check.h"

#include "ice.h"
#include "ice_maker.h"
#include "ice_tray.h"
#include "ice_tray_test.h"
#include "relay.h"
#include "comp.h"
#include "comp_bldc.h"
#include "gas_switch_valve.h"

#include "error.h"

DoDiagnosisAc_T       DiagnosisAc;


void SetDiagnosisPartAc(U16 mu16Part)
{
    DiagnosisAc.Mode |= mu16Part;
}

U16 GetDiagnosisPartAc(void)
{
    return DiagnosisAc.Mode;
}

void ClearDiagnosisPartAc(U16 mu16Part)
{
    DiagnosisAc.Mode &= ~mu16Part;
}

U16 IsSetDiagnosisPartAc(U16 mu16Part)
{
    if ( (DiagnosisAc.Mode & mu16Part ) == mu16Part )
    {
        return TRUE;
    }
    
    return FALSE;
}


void StartDiagnosisAc(void)
{
    DiagnosisAc.Mode = DIAGNOSIS_PART_AC;

    DiagnosisAc.IsNeedDummy = FALSE;
}

void StopDiagnosisAc(void)
{
    DiagnosisAc.Mode = DIAGNOSIS_PART_AC_NONE;

    // here... do init load..
    //

    TurnOffRelay( RELAY_ICE_TRAY_CW );
    TurnOffRelay( RELAY_ICE_TRAY_CW );

    TurnOffRelay( RELAY_FEEDER_CW );
    TurnOffRelay( RELAY_FEEDER_CCW );

    SetCompBldcErrorComm( FALSE );          // 압축기 통신 에러 해제 
    SetCompBldcErrorCode( ERR_BLDC_NONE );  // 압축기 에러 상태 해제
    SetCompBldcLastedErrorCode( ERR_BLDC_NONE );    // 압축기 이전 에러 상태 해제
    SetCompBldcTargetRps( 0 );

    ClearIceMakeMode(ICE_MODE_ALL);
    if( DiagnosisAc.IsNeedDummy == TRUE )
    {
        DiagnosisAc.IsNeedDummy = FALSE;
        SetIceMakeMode(ICE_MODE_DUMMY_DEICING);
    }
    else
    {
        SetIceMakeMode(ICE_MODE_INIT|ICE_MODE_OFF_LOAD);
    }
    ClearLoadAdc(INPUT_SRC_AC);
    ClearLoadCurrent(INPUT_SRC_AC);
}

// AC LOAD
static void InitDiagnosisAcInit(void)
{
    DiagnosisAc.StepInit        = 0;
    DiagnosisAc.StepIceTray     = 0;
    DiagnosisAc.StepBldcComp    = 0;

    DiagnosisAc.TimeOut         = 0;

    DiagnosisAc.Icing           = 0;
    DiagnosisAc.Deicing         = 0;
    DiagnosisAc.IceTrayStatus   = 0;
}

static DStatus_T DoDiagnosisAcInit(void)
{
    DStatus_T mStatus = STATUS_GOING;

    switch(DiagnosisAc.StepInit)
    {
        case STEP_AC_INIT_START :

            TURN_OFF_ICE_TRAY_CW();
            TURN_OFF_ICE_TRAY_CCW();

            ClearLoadAdc(INPUT_SRC_AC);
            ClearLoadCurrent(INPUT_SRC_AC);

            DiagnosisAc.StepInit++;
            DiagnosisAc.TimeOut = STER_WAIT_LOAD_CLEAR;
            break;

        case STEP_AC_INIT_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisAc.StepInit = 0;
            break;
    }


    return mStatus;
}

#define COMP_MIN_ON_TIME     60        // 1min 
U16 GetDiagnoisCompOnWaitTime(void)
{
    U16 mu16Time;
    U32 mu32OnTime; // @1sec


    mu32OnTime = GetCompOnTime();

    if( mu32OnTime >= COMP_MIN_ON_TIME ) 
    {
        return 0;
    }

    mu16Time = (U16)(COMP_MIN_ON_TIME - mu32OnTime);
    mu16Time *= 10;     // conv sec to ms

    return mu16Time;
}

//#define COMP_MIN_OFF_TIME    180        // 3min
#define COMP_MIN_OFF_TIME    300        // 5min
U16 GetDiagnoisCompOffWaitTime(void)
{
    U16 mu16Time;
    U32 mu32OffTime; // @1sec


    mu32OffTime = GetCompOffTime();

    if( mu32OffTime >= COMP_MIN_OFF_TIME ) 
    {
        return 0;
    }

    mu16Time = (U16)(COMP_MIN_OFF_TIME - mu32OffTime);
    mu16Time *= 10;     // conv sec to ms

    return mu16Time;
}

// 압축기 에러
enum
{
    DIAG_ERR_BLDC_NONE, 

    DIAG_ERR_BLDC_COMM, 
    DIAG_ERR_BLDC_STARTING_FAIL, 
    DIAG_ERR_BLDC_OVER_CURRENT, 
    DIAG_ERR_BLDC_ABNORMAL_VOLTAGE, 
    DIAG_ERR_BLDC_OVERHEAT_CONTROLLER, 
    DIAG_ERR_BLDC_DISCONNECTION_COMM, 
};


U32 dbg_diag_comp_time = 0;
static DStatus_T DoDiagnosisBldcComp(void)
{
    DStatus_T mStatus = STATUS_GOING;
	U8 gu8orgErrorNum;

	gu8orgErrorNum = ERR_PROTECT_COMP_COMM;
    switch(DiagnosisAc.StepBldcComp)
    {
        case STEP_AC_BLDC_COMP_CHECK :
            if( IsTurnOnCompBldc() == TRUE )
            {
                if( GetGasSwitchStatus() != GAS_SWITCH_COLD)
                {
                    DiagnosisAc.IsNeedDummy = TRUE;
                }

                SetCompBldcTargetRps( MIN_RPS );
                DiagnosisAc.StepBldcComp = STEP_AC_BLDC_COMP_ON;
            }
            else
            {
                SetCompBldcErrorComm( FALSE );          // 압축기 통신 에러 해제 
                SetCompBldcErrorCode( ERR_BLDC_NONE );  // 압축기 에러 상태 해제
                SetCompBldcLastedErrorCode( ERR_BLDC_NONE );    // 압축기 이전 에러 상태 해제
                DiagnosisAc.StepBldcComp = STEP_AC_BLDC_COMP_OFF;
            }

            break;

        case STEP_AC_BLDC_COMP_OFF :
            DiagnosisAc.TimeOut = GetDiagnoisCompOffWaitTime();
            DiagnosisAc.StepBldcComp++;
            break;

        case STEP_AC_BLDC_COMP_OFF_WAIT :
            if( DiagnosisAc.TimeOut != 0 )
            {
                DiagnosisAc.TimeOut--;
            }
            else
            {
                SetCompBldcTargetRps( MIN_RPS );
                DiagnosisAc.StepBldcComp++;
            }
            break;

        case STEP_AC_BLDC_COMP_ON :
            DiagnosisAc.TimeOut = GetDiagnoisCompOnWaitTime();
            DiagnosisAc.StepBldcComp = STEP_AC_BLDC_COMP_WAIT;
            break;


        case STEP_AC_BLDC_COMP_WAIT :
            if( DiagnosisAc.TimeOut != 0 )
            {
                DiagnosisAc.TimeOut--;
            }
            else
            {
                U16 mu16Error = DIAG_ERR_BLDC_NONE;

                // 통신 에러
#if 0
                if( GetCompBldcErrorComm() == TRUE )
                {
                    mu16Error = DIAG_ERR_BLDC_COMM;
                }
                else if( GetCompBldcErrorCode() != ERR_BLDC_NONE )
                {
                    mu16Error = GetCompBldcLastedErrorCode() + DIAG_ERR_BLDC_COMM /*offset*/;
                }
#else
			    if( GetCompBldcErrorComm() == TRUE )
			    {
			    	mu16Error = 27;
			    }
			    if(GetCompBldcLastedErrorCode() == ERR_COMP_CURRENT_SENSING - gu8orgErrorNum )
			    {
			    	mu16Error = 81;
			    }
			    else if(GetCompBldcLastedErrorCode() == ERR_COMP_STARTING_FAIL - gu8orgErrorNum )
			    {
			    	mu16Error = 82;
			    }
			    else if(GetCompBldcLastedErrorCode() == ERR_COMP_OVER_CURRENT - gu8orgErrorNum )
			    {
			    	mu16Error = 83;
			    }
			    else if(GetCompBldcLastedErrorCode() == ERR_COMP_OVERHEAT - gu8orgErrorNum)
			    {
			    	mu16Error = 84;
			    }
			    else if(GetCompBldcLastedErrorCode() == ERR_COMP_DISCONNECTION_OF_COMM - gu8orgErrorNum)
			    {
			    	mu16Error = 85;
			    }
			    else if(GetCompBldcLastedErrorCode() == ERR_COMP_ABNORMAL_VOLTAGE - gu8orgErrorNum)
			    {
			    	mu16Error = 86;
			    }
				else {}
#endif
				SetSmartCheckDataId(WIFI_PART_0201_COLD_BLDC, mu16Error );
				
                SetCompBldcTargetRps( 0 );
                DiagnosisAc.StepBldcComp++;
            }
            break;

        case STEP_AC_BLDC_COMP_DONE :
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisAc.StepBldcComp = 0;
            break;
    }

    dbg_diag_comp_time++;

    return mStatus;
}


enum
{
    ICE_TRAY_STATUS_OK,
    ICE_TRAY_STATUS_ICING_ERR,
    ICE_TRAY_STATUS_DEICING_ERR,
    ICE_TRAY_STATUS_DETECT_ERR,
};

U16 dbg_diag_ice_tray_time = 0;
static DStatus_T DoDiagnosisIceTray(void)
{
    DStatus_T mStatus = STATUS_GOING;


    switch(DiagnosisAc.StepIceTray)
    {
        case STEP_AC_ICE_TRAY_START :

            StartIceTrayTest();

            DiagnosisAc.TimeOut = STER_WAIT_LOAD_TRAY_OP;
            DiagnosisAc.StepIceTray++;
            break;

        case STEP_AC_ICE_TRAY_CHECKING :
            if( GetIceTrayTestStatus() == FALSE )
            {
                DiagnosisAc.StepIceTray++;
            }
            else
            {
                if( DiagnosisAc.TimeOut != 0 )
                {
                    DiagnosisAc.TimeOut--;
                }
                else
                {
                    DiagnosisAc.StepIceTray++;
                }
            }
            break;

        case STEP_AC_ICE_TRAY_DONE :
            TurnOffRelay( RELAY_ICE_TRAY_CW );
            TurnOffRelay( RELAY_ICE_TRAY_CCW );


            DiagnosisAc.Icing   = GetIceTrayTestCheckIcing();
            DiagnosisAc.Deicing = GetIceTrayTestCheckDeIcing();
            if( DiagnosisAc.Icing == 1 
                && DiagnosisAc.Deicing == 1)
            {
                DiagnosisAc.IceTrayStatus = ICE_TRAY_STATUS_OK;
            }
            else if( DiagnosisAc.Icing == 1 
                && DiagnosisAc.Deicing == 0)
            {
                DiagnosisAc.IceTrayStatus = ICE_TRAY_STATUS_DEICING_ERR;
            }
            else if( DiagnosisAc.Icing == 0 
                && DiagnosisAc.Deicing == 1)
            {
                DiagnosisAc.IceTrayStatus = ICE_TRAY_STATUS_ICING_ERR;
            }
            else
            {
                DiagnosisAc.IceTrayStatus = ICE_TRAY_STATUS_DETECT_ERR;
            }

            SetSmartCheckDataId(WIFI_PART_0805_ICE_TRAY_SENSING_SW, DiagnosisAc.IceTrayStatus);
            mStatus = STATUS_DONE;
            break;

        default :
             DiagnosisAc.StepIceTray = 0;
            break;
    }

    dbg_diag_ice_tray_time++;
    return mStatus;
}




DiagnosisEventList_T    DiagnosisPartAcEventList[] =
{
/*      Diagnosis Part,                 Function,              Initial,      */
    { DIAGNOSIS_PART_AC_INIT,         DoDiagnosisAcInit,       InitDiagnosisAcInit   },    
    { DIAGNOSIS_PART_AC_BLDC_COMP,    DoDiagnosisBldcComp,     InitDiagnosisAcInit   },
    { DIAGNOSIS_PART_AC_ICE_TRAY,     DoDiagnosisIceTray,      InitDiagnosisAcInit   }, 
};
#define SZ_DIAGNOSIS_PART_AC_EVENT_LIST ( sizeof(DiagnosisPartAcEventList) / sizeof(DiagnosisEventList_T) )

U32 dbg_the_ac_time = 0;
void DiagnosisPartAcEventHandler(void)
{
    DStatus_T mStatus;
    U8 i;
    DiagnosisEventList_T *pList = NULL;

    for ( i = 0; i < SZ_DIAGNOSIS_PART_AC_EVENT_LIST ; i++ )
    {
        pList = &DiagnosisPartAcEventList[ i ];

        // Check Mode
        if (IsSetDiagnosisPartAc(pList->Part) == TRUE )
        {
            // Do action
            mStatus = pList->DoAction();
            if ( mStatus == STATUS_DONE )               //Do function
            {
                pList->InitAction();                    // Initial Function
                ClearDiagnosisPartAc( pList->Part );   // Mode Clear
            }
            else if( mStatus == STATUS_ERR )
            {
                // 진단 중 에러 조건 발생되면  중지
                StopDiagnosis();
            }

            dbg_the_ac_time++;
            break;
        }
    }
}
