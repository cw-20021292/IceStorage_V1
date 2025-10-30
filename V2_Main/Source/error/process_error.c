#include "process_error.h"
#include "error.h"

#include "eeprom.h"
#include "service.h"
#include "level.h"
#include "front.h"
#include "WIFI_Control.h"

#include "err_temp_cold_water.h"
#include "err_temp_room_water.h"
#include "err_temp_eva_1.h"
#include "err_temp_ambient.h"
#include "err_temp_hot_water.h"
#include "err_temp_hot_water_in.h"
#include "err_temp_heater.h"
#include "err_temp_hot_overheat.h"
#include "err_leak.h"
#include "err_ice_tray.h"
#include "err_drain_pump.h"
#include "err_comp_bldc.h"
#include "err_comp_bldc_comm.h"
#include "err_outage_water.h"
#include "err_outage_hot_water.h"
//#include "err_outage_ice_tray.h"
//#include "err_leak_flow.h"


typedef U8 (*ErrFun_T)(U8 mu8Error);
typedef struct _error_list_
{
    ErrorId_T Id;           // 에러 ID
    U8 PrevError;           // 이전 에러 
    U8 Error;               // 현재 에러
    U8 SterActive;          // 살균 중 에러 표시
    U8 DispActive;          // 에러 표시 여부
    U8 CHPActive;            // CHP MODEL 전용 ( TRUE이면, CHP 전용 )

    ErrFun_T Check;         // 에러 검출 함수
    ErrFun_T Release;       // 에러 해제 함수
} ErrorList_T;

void InitErrorList(void)
{
    InitError();

    InitErrCompBldcComm();
    InitErrCompBldc();
}


#if 1
ErrorList_T ErrList[] = 
{
    /*ID                        		PrevError Error   Ster      Disp     CHP(ONLY) Check error                    Check release error */
    { ERR_TEMP_EVA_1,           		FALSE,    FALSE,  FALSE,    FALSE,   FALSE,    CheckErrTempEva_1,             ReleaseErrTempEva_1 },
    { ERR_MICRO_SW_MOVE,	       	 	FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrIceTrayMoving,         NULL },  // There is no release error
    { ERR_MICRO_SW_DETECT,  	    	FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrIceTrayMicroSwitch,    NULL },  // There is no release error
    { ERR_DRAIN_PUMP,           		FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrDrainPump,             NULL },   // There is no release error

    { ERR_TEMP_HEATER,          		FALSE,    FALSE,  TRUE,     TRUE,    TRUE,     CheckErrTempHeater,            ReleaseErrTempHeater },
    { ERR_TEMP_HOT_WATER,       		FALSE,    FALSE,  TRUE,     TRUE,    TRUE,     CheckErrTempHotWater,          ReleaseErrTempHotWater },
    { ERR_TEMP_HOT_WATER_IN,    		FALSE,    FALSE,  TRUE,     TRUE,    TRUE,     CheckErrTempHotWaterIn,        ReleaseErrTempHotWaterIn },
    { ERR_TEMP_COLD_WATER,      		FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrTempColdWater,         ReleaseErrTempColdWater },
    { ERR_TEMP_AMBIENT,         		FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrTempAmbient,           ReleaseErrTempAmbient },
    { ERR_TEMP_ROOM_WATER,      		FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrTempRoomWater,         ReleaseErrTempRoomWater },
    { ERR_HEATER_OVERHEAT,      		FALSE,    FALSE,  TRUE,     TRUE,    TRUE,     CheckErrHeaterOverheat,        ReleaseErrHeaterOverheat }, 
    { ERR_COMP,                 		FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrCompBldc,              NULL      },  // There is no release error
    { ERR_COMP_COMM,            		FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrCompBldcComm,          NULL      },  // There is no release error

    { ERR_PROTECT_COMP,         		FALSE,    FALSE,  FALSE,    FALSE,   FALSE,    CheckProtectCompBldc,          ReleaseProtectCompBldc      },
    { ERR_PROTECT_COMP_COMM,    		FALSE,    FALSE,  FALSE,    FALSE,   FALSE,    CheckProtectCompBldcComm,      ReleaseProtectCompBldcComm  },
	/*.. sean [24-10-15] 기존 프로그램 에러코드에서 분리  ..*/
#if 1
    { ERR_COMP_CURRENT_SENSING,         FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrCompCurrentSensing,  	    NULL },
    { ERR_COMP_STARTING_FAIL,			FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrCompStartingFail,	    	NULL },
    { ERR_COMP_OVER_CURRENT,			FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrCompOverCurrent,         	NULL },
    { ERR_COMP_OVERHEAT,     			FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrCompOverheat,        		NULL },
    { ERR_COMP_DISCONNECTION_OF_COMM,	FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrCompDisconnection,           NULL },
    { ERR_COMP_ABNORMAL_VOLTAGE,		FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrCompAbnormalVoltage,			NULL },
#endif

    { ERR_OUTAGE_WATER,         		FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrOutageWater,           ReleaseErrOutageWater },
    { ERR_OUTAGE_HOT_WATER,     		FALSE,    FALSE,  TRUE,     TRUE,    TRUE,     CheckErrOutageHotWater,        ReleaseErrOutageHotWater },
    //{ ERR_LEAK_FLOW,            FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrLeakFlow,              NULL }, 
    { ERR_LEAK,                 FALSE,    FALSE,  TRUE,     TRUE,    FALSE,    CheckErrLeak,                  ReleaseErrLeak }, 



    //{ ERR_OUTAGE_ICE_TRAY,      FALSE,    FALSE,  FALSE,    FALSE,   FALSE,    CheckErrOutageIceTray,         ReleaseErrOutageIceTray },
    //{ ERR_TEMP_EVA_2,           FALSE,    FALSE,  FALSE,    FALSE,   CheckErrTempEva_2,             ReleaseErrTempEva_2 },
    //{ ERR_COLD_HIGH_LEVEL,      FALSE,    FALSE,  TRUE,     TRUE,    CheckErrColdHigh,              NULL },  // There is no release error
    //{ ERR_COLD_OVF,             FALSE,    FALSE,  TRUE,     TRUE,    CheckErrColdOvf,               NULL },
    //{ ERR_COLD_LOW_LEVEL,       FALSE,    FALSE,  TRUE,     TRUE,    CheckErrColdLow,               ReleaseErrColdLow },
    //{ ERR_ROOM_OVF,             FALSE,    FALSE,  TRUE,     TRUE,    CheckErrRoomOvf,               NULL },  // There is no release error
    //{ ERR_ROOM_COMPLEX,         FALSE,    FALSE,  TRUE,     TRUE,    CheckErrRoomComplex,           NULL },  // There is no release error
    //{ ERR_ROOM_HIGH_LEVEL,      FALSE,    FALSE,  TRUE,     TRUE,    CheckErrRoomHigh,              NULL },  // There is no release error
    //{ ERR_ROOM_LOW_LEVEL,       FALSE,    FALSE,  TRUE,     TRUE,    CheckErrRoomLow,               ReleaseErrRoomLow },
    //{ ERR_SODA_PUMP,            FALSE,    FALSE,  TRUE,     TRUE,     CheckErrSodaPump,             ReleaseErrSodaPump },   

    //{ ERR_COMM_FRONT_DISPLAY, FALSE,     FALSE,  TRUE,     FALSE,   CheckCommFrontDisplay,        ReleaseCommFrontDisplay },   
    //{ ERR_COMM_LCD,           FALSE,     FALSE,  TRUE,     FALSE,   CheckCommFrontLcd,            ReleaseCommFrontLcd },   
};
#else
// ERROR TEST
ErrorList_T ErrList[] = 
{
    /*ID                        PrevError Error   Ster      Disp     CHP(ONLY) Check error                    Check release error */
    { ERR_TEMP_EVA_1,           FALSE,    FALSE,  FALSE,    FALSE,   FALSE,   NULL, NULL }, 
    { ERR_OUTAGE_ICE_TRAY,      FALSE,    FALSE,  FALSE,    FALSE,   FALSE,   NULL, NULL }, 
    { ERR_MICRO_SW_MOVE,        FALSE,    FALSE,  TRUE,     TRUE,    FALSE,   NULL, NULL }, 
    { ERR_MICRO_SW_DETECT,      FALSE,    FALSE,  TRUE,     TRUE,    FALSE,   NULL, NULL }, 
    { ERR_DRAIN_PUMP,           FALSE,    FALSE,  TRUE,     TRUE,    FALSE,   NULL, NULL }, 

    { ERR_TEMP_HEATER,          FALSE,    FALSE,  TRUE,     TRUE,    TRUE,    NULL, NULL }, 
    { ERR_TEMP_HOT_WATER,       FALSE,    FALSE,  TRUE,     TRUE,    TRUE,    NULL, NULL }, 
    { ERR_TEMP_HOT_WATER_IN,    FALSE,    FALSE,  TRUE,     TRUE,    TRUE,    NULL, NULL }, 
    { ERR_TEMP_COLD_WATER,      FALSE,    FALSE,  TRUE,     TRUE,    FALSE,   NULL, NULL }, 
    { ERR_TEMP_AMBIENT,         FALSE,    FALSE,  TRUE,     TRUE,    FALSE,   NULL, NULL }, 
    { ERR_TEMP_ROOM_WATER,      FALSE,    FALSE,  TRUE,     TRUE,    FALSE,   NULL, NULL }, 
    { ERR_HEATER_OVERHEAT,      FALSE,    FALSE,  TRUE,     TRUE,    TRUE,    NULL, NULL }, 

    { ERR_COMP,                 FALSE,    FALSE,  TRUE,     TRUE,    FALSE,   NULL, NULL }, 
    { ERR_COMP_COMM,            FALSE,    FALSE,  TRUE,     TRUE,    FALSE,   NULL, NULL }, 

    { ERR_PROTECT_COMP,         FALSE,    FALSE,  FALSE,    FALSE,   FALSE,   NULL, NULL }, 
    { ERR_PROTECT_COMP_COMM,    FALSE,    FALSE,  FALSE,    FALSE,   FALSE,   NULL, NULL }, 

    { ERR_OUTAGE_WATER,         FALSE,    FALSE,  TRUE,     TRUE,    FALSE,   NULL, NULL }, 
    { ERR_OUTAGE_HOT_WATER,     FALSE,    FALSE,  TRUE,     TRUE,    TRUE,    NULL, NULL }, 

    { ERR_LEAK_FLOW,            FALSE,    FALSE,  TRUE,     TRUE,    FALSE,   NULL, NULL }, 
    { ERR_LEAK,                 FALSE,    FALSE,  TRUE,     TRUE,    FALSE,   NULL, NULL }
};
#endif
#define  SZ_ERR_LIST      (sizeof(ErrList)/sizeof(ErrorList_T))


U8 dbg_iot_err = 0;
static ErrorId_T FindCheckError(  ErrorList_T *pErrList, U16 mu16Size )
{
    U8 i;
    ErrFun_T pCheck;
    ErrFun_T pRelease;
    ErrorId_T mId = ERR_NONE;
    ErrorId_T mNewId = ERR_NONE;
    U8 mu8Error = FALSE;
    ErrorDate_T mDate;
    ErrorList_T *pList = NULL;
    U8 mu8IsChangedErrorStatus = FALSE;


    GetErrorDate( &mDate );
    for( i = 0; i < mu16Size; i++ ) 
    {
        pList = (pErrList + i );

        // Mapping error status..
        mId      = pList->Id;
        mu8Error = pList->Error;
        pCheck   = pList->Check;
        pRelease = pList->Release;

#if CONFIG_STER
        // 살균 동작 중에는 예외처리되는 에러가 있다.
        if( IsStartSter() == TRUE )
        {
            if( pList->SterActive == FALSE )
            {
                continue;
            }
        }
#endif

        // CP 모델에서는 CHP 에러를 SKIP한다.
        if( GetSystem() == SYSTEM_CP )
        {
            if( pList->CHPActive == TRUE )
            {
                continue;
            }
        }

        // 에러 발생/해제
        if( mu8Error == TRUE )
        {
#if CONFIG_STER
            if( GetSterErrWaterOut() == FALSE )
#endif
            {
                if( pRelease != NULL )
                {
                    mu8Error = pRelease( mu8Error );
                }
            }
        }
        else
        {
            if( pCheck != NULL )
            {
                mu8Error = pCheck( mu8Error );
            }
        }

        // Update new error status
        pList->Error = mu8Error;
        if( pList->PrevError != pList->Error )
        {
            pList->PrevError = pList->Error;

            //SetErrorStatus( mId, pList->Error );
            SetErrorStatusDate( mId, pList->Error, &mDate );

            // WIFI (화면에 표시되는 에러만 서버에 전달 )
            if( pList->DispActive == TRUE )
            {
               mu8IsChangedErrorStatus = TRUE; 
            }
        }

        if( pList->DispActive == TRUE )
        {
            //  Top priority Error
            if( GetErrorStatus( mId ) == TRUE )
            {
                mNewId = mId;
            }
        }

    }

    // WIFI 
    if( mu8IsChangedErrorStatus == TRUE )
    {
        WifiSendData(WIFI_DATA_ERROR);
        dbg_iot_err++;
    }

    return mNewId;
}


#if (CONFIG_TYPE_TANK == TANK_RO )
static void ResetRoomLevelErrorCount(void)
{
    static U8 mu8PrevTankOpen = TRUE;
    static U8 mu8CurTankOpen = TRUE;


    mu8CurTankOpen = GetServiceCheckTankOpen();
    if( mu8PrevTankOpen == TRUE 
            &&  mu8CurTankOpen== FALSE )
    {
        ClearTankLevelErrorCount();
    }

    if( mu8PrevTankOpen != mu8CurTankOpen )
    {
        mu8PrevTankOpen = mu8CurTankOpen;
    }
}
#endif


void ProcessError(void)
{
#if CONFIG_ERR_ALL
    ErrorId_T mNewId = ERR_NONE;

    // Check Error
    mNewId = FindCheckError( &ErrList[0], SZ_ERR_LIST );

    // Update New Error Id
    UpdateNewError( mNewId );

#if (CONFIG_TANK_TYPE == TANK_RO)
    ResetRoomLevelErrorCount();
#endif

#endif  // CONFIG_ERR_ALL
}
