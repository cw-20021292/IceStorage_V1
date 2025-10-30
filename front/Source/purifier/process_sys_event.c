/**
 * File : process_sys_event.c
 *
 * Process Time Scheduler
**/
//#include <stdio.h>
//#include <string.h>

#include "process_sys_event.h"

#include "hal_serial.h"

#include "timer.h"
#include "comm.h"
#include "key.h"
#include "led.h"
#include "purifier.h"
#include "voice.h"
#include "voice_nxd1005.h"
#include "parser_subfront.h"
/**
 * To Do
 * EOL / FCT / DIAGNOSIS ?곹깭???곕씪 ???대㉧ ?ㅼ?以꾨쭅 ?????덈룄濡??댁빞 ??
**/

/**
 * @brief     Function For Initialize System
 * @detail    none
 * @param     none
 * @return    none
**/
void InitSystem(void)
{
    InitComm();
    InitLed();
    InitPurifier();
    InitKey();
    InitVoice();
}

/**
 * @brief     Function For Initialize Process Timer
 * @detail    none
 * @param     none
 * @return    none
**/
void InitStartTimer(void)
{
    InitTimer();

    StartTimer( TIMER_PROCESS, TIMER_ID_1MS,   1);
    StartTimer( TIMER_PROCESS, TIMER_ID_10MS,  10);
    StartTimer( TIMER_PROCESS, TIMER_ID_100MS, 100);
    StartTimer( TIMER_PROCESS, TIMER_ID_1SEC,  SEC(1));
    StartTimer( TIMER_PROCESS, TIMER_ID_10SEC, SEC(1));
    StartTimer( TIMER_PROCESS, TIMER_ID_40SEC, SEC(600));
    StartTimer( TIMER_PROCESS, TIMER_ID_1MIN,  SEC(1));

    StartTimer( TIMER_PROCESS, TIMER_ID_MAIN_TX_ERR, SEC(3));
}

/**
 * Nomal Function
**/
/**
 * @brief     Function For Handing per 1ms
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Normal_1ms_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_1MS, 1);

    ProcessScanKey();
}

/**
 * @brief     Function For Handing per 10ms
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Normal_10ms_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_10MS, 10);
}

/**
 * @brief     Function For Handing per 100ms
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Normal_100ms_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_100MS, 100);

    /* Main Tx Error Display */
    DispMainRxErr();

    /* Main Tx Error Limit Timer */
    UpdateMainRxErrTimer();

    /* SubFront Tx Error Limit Timer */
    UpdateSubFrontRxErrTimer();
    
    /* Voice IC Added */
    NXD_ProcessVoice();
}

/**
 * @brief     Function For Handing per 1sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Normal_1sec_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_1SEC, SEC(1));
}

/**
 * @brief     Function For Handing per 10sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Normal_10sec_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_10SEC, SEC(10));
}

/**
 * @brief     Function For Handing per 40sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Normal_40sec_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_40SEC, SEC(40));
}

/**
 * @brief     Function For Handing per 1min
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Normal_1min_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_1MIN, SEC(60));
}

static void Evt_Main_Rx_Err( void )
{
    SetMainRxErr(TRUE);
}

// 50ms 주기로 FRONT COMMUNICATION
#define FRONT_REFRESH_TIME     50 // @50ms...
/**
 * @brief SUB FRONT로 주기적으로 데이터를 보내는 Handler.
 * @attention 주기는 위에 #define 되어있음
 * 
 */
static void Evt_SubFront_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_SUB_FRONT, FRONT_REFRESH_TIME);

    SetCommHeader(COMM_ID_SUB_FRONT, PKT_REQ_SUB_FRONT);
    StartTimer(TIMER_USER, TIMER_USER_ID_COMM_FRONT_SUB_TX, 0);
}

/**
 * @brief FRONT SUB RX 통신 에러 헨들러
 * 
 */
static void Evt_SubFrontRxErr_Handler(void)
{

}

/**
 * Process
**/
typedef void (*FPAction_T)(void);
typedef struct _sys_event_
{
    U8          timerId;
    FPAction_T  pFunc;
    FPAction_T  pFuncDiag;
    FPAction_T  pFuncFct;
    FPAction_T  pFuncEol;
} SSysEvent_T;

const static SSysEvent_T SysEventList[] =
{
    /* Process Scheduler Timer */
    /* TIMER ID,                    NORMAL,                     DIAGNOSIS,  FCT,    EOL */
    { TIMER_ID_1MS,                 Evt_Normal_1ms_Handler,     NULL,      NULL,    NULL },
    { TIMER_ID_10MS,                Evt_Normal_10ms_Handler,    NULL,      NULL,    NULL },
    { TIMER_ID_100MS,               Evt_Normal_100ms_Handler,   NULL,      NULL,    NULL },
    { TIMER_ID_1SEC,                Evt_Normal_1sec_Handler,    NULL,      NULL,    NULL },
    { TIMER_ID_10SEC,               Evt_Normal_10sec_Handler,   NULL,      NULL,    NULL },
    { TIMER_ID_40SEC,               Evt_Normal_40sec_Handler,   NULL,      NULL,    NULL },
    { TIMER_ID_1MIN,                Evt_Normal_1min_Handler,    NULL,      NULL,    NULL },

    /* MAIN 통신에러 검사 */
    { TIMER_ID_MAIN_TX_ERR,         Evt_Main_Rx_Err,            NULL,      NULL,    NULL },
    /* SUB FRONT TX POLLING */
    { TIMER_ID_SUB_FRONT,           Evt_SubFront_Handler,       NULL,      NULL,    NULL },
    /* SUB FRONT RX ERROR */
    { TIMER_ID_SUB_FRONT_RX_ERR,    Evt_SubFrontRxErr_Handler,  NULL,      NULL,    NULL },

    /* User Timer */
};
#define	SZ_SYS_EVENT_LIST     ( sizeof(SysEventList) / sizeof(SSysEvent_T) )

/**
 * @brief     Function For Scheduler Of Process
 * @detail    Unit : 1ms
 * @param     none
 * @return    none
**/
void ProcessEventHandler(void)
{
    U8          i = 0U;
    FPAction_T  func = NULL;
    SSysEvent_T *pSysEventList = NULL;

    for( i=0; i<SZ_SYS_EVENT_LIST; i++ )
    {
        pSysEventList = &SysEventList[i];

        if( IsExpiredTimer(TIMER_PROCESS, pSysEventList->timerId ) == TIMER_EXPIRE )
        {
            // Disable Expired Timer
            DisableTimer( TIMER_PROCESS, pSysEventList->timerId );

            func = pSysEventList->pFunc;

            // Call a event handl
            if( func != NULL )
            {
                func();
            }
        }
    }
}
