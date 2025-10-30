#include <stdio.h>
#include <string.h>

#include "prj_type.h"
#include "timer.h"
#include "process_sys_event.h"

#include "water_out.h"
#include "process_display.h"
#include "relay.h"
#include "valve.h"
#include "ice_tray.h"
#include "ice_tray_test.h"
#include "ice_full.h"
#include "key.h"
#include "gas_switch_valve.h"
#include "process_key.h"
#include "key_handler.h"
#include "key_normal_handler.h"


#include "lever.h"
#include "process_lever.h"

#include "hal_step_motor.h"
#include "hal_serial.h"
#include "hal_adc.h"
#include "hal_led.h"
#include "hal_input.h"
#include "hal_uv.h"

#include "drain_pump.h"
#include "process_make.h"

#include "front.h"
#include "cold_water.h"
#include "ice.h"
#include "ice_door.h"
#include "ice_maker.h"
#include "drain.h"
#include "swing_bar.h"
#include "eeprom.h"
#include "power_saving.h"
#include "leak.h"
#include "error.h"
#include "process_error.h"
#include "service.h"
//#include "tank_clean.h"

#include "hot_water.h"
#include "hot_water_out.h"
#include "hot_water_flow.h"
#include "hot_water_heater.h"
#include "heater_out.h"

#include "energy.h"
#include "user.h"
#include "uv.h"
#include "process_uv.h"
#include "health.h"
#include "drain_water.h"
#include "health_maker.h"
#include "empty_cold_tank.h"
#include "cody_kit.h"

#include "rtc.h"

#include "eol.h"
#include "process_eol.h"
#include "fct.h"
#include "process_fct.h"
#include "time_short.h"

#include "parser.h"
#include "parser_front.h"
#include "parser_comp.h"

#include "temp.h"
#include "sound.h"
#include "voice.h"
#include "smps.h"

#include "flow_meter.h"
#include "flow_valve.h"
#include "filter.h"
#include "metering.h"
#include "comp.h"
#include "flush_water_out.h"

#include "smart_check.h"
#include "diagnosis.h"
#include "monitoring.h"


#include "WIFI_UserInterface_IconAIS.h"
#include "WIFI_Control.h"



typedef void(*Action_T)(void);
typedef struct _sys_event_
{
    U8   timerId;
    Action_T pFun;
    Action_T pFun_Diag;
    Action_T pFun_FCT;
    Action_T pFun_EOL;
} SysEvent_T;

/* NORMAL */
static void Evt_1ms_Handler( void );
static void Evt_10ms_Handler( void );
static void Evt_100ms_Handler( void );
static void Evt_1sec_Handler( void );
static void Evt_10sec_Handler( void );
static void Evt_40sec_Handler( void );
static void Evt_1min_Handler( void );
static void Evt_Debug_Handler( void );
static void Evt_Front_Handler( void );
static void Evt_DrainPump_Handler( void );
static void Evt_FrontRxErr_Handler( void );
static void Evt_Comp_Handler( void );
static void Evt_CompRxErr_Handler( void );
static void Evt_CheckSystem_Handler( void );

/* DIAG */
static void Evt_Diag_1ms_Handler( void );
static void Evt_Diag_10ms_Handler( void );
static void Evt_Diag_100ms_Handler( void );
static void Evt_Diag_1sec_Handler( void );
static void Evt_Diag_40sec_Handler( void );
static void Evt_Diag_1min_Handler( void );

/* EOL */
static void Evt_EOL_1ms_Handler(void);
static void Evt_EOL_10ms_Handler(void);
static void Evt_EOL_100ms_Handler(void);
static void Evt_EOL_1sec_Handler( void );
static void Evt_EOL_1min_Handler( void );

/* FCT */
static void Evt_FCT_1ms_Handler( void );
static void Evt_FCT_10ms_Handler( void );
static void Evt_FCT_100ms_Handler(void);
static void Evt_FCT_1sec_Handler( void );
static void Evt_FCT_1min_Handler( void );

/* TEST */
#if CONFIG_TEST_8585
static void Evt_Test_8585_Handler( void );
static void Evt_Test_8585_2_Handler( void );
#endif
#if CONFIG_TEST_8585_ICE
static void Evt_Test_8585_Ice_Handler( void );
#endif
#if CONFIG_JIG_GAS_SWITCH
static void Evt_Test_GasSwitch_Handler( void );
#endif

const static SysEvent_T	SysEventList[] =
{
    /* TIMER ID,                    NORMAL,                     DIAGNOSIS,                FCT,                    EOL */
    { TIMER_ID_1MS,                 Evt_1ms_Handler,            Evt_Diag_1ms_Handler,     Evt_FCT_1ms_Handler,    Evt_EOL_1ms_Handler },
    { TIMER_ID_10MS,                Evt_10ms_Handler,           Evt_Diag_10ms_Handler,    Evt_FCT_10ms_Handler,   Evt_EOL_10ms_Handler },
    { TIMER_ID_100MS,               Evt_100ms_Handler,          Evt_Diag_100ms_Handler,   Evt_FCT_100ms_Handler,  Evt_EOL_100ms_Handler },
    { TIMER_ID_1SEC,                Evt_1sec_Handler,           Evt_Diag_1sec_Handler,    Evt_FCT_1sec_Handler,   Evt_EOL_1sec_Handler },
    { TIMER_ID_10SEC,               Evt_10sec_Handler,          Evt_10sec_Handler,        NULL,                   NULL },
    { TIMER_ID_40SEC,               Evt_40sec_Handler,          Evt_Diag_40sec_Handler,   NULL,                   NULL },
    { TIMER_ID_1MIN,                Evt_1min_Handler,           Evt_Diag_1min_Handler,    Evt_FCT_1min_Handler,   Evt_EOL_1min_Handler },
    { TIMER_ID_FRONT,               Evt_Front_Handler,          Evt_Front_Handler,        Evt_Front_Handler,      Evt_Front_Handler },
#if !CONFIG_JIG_RBK_FRONT
    { TIMER_ID_DEBUG,               Evt_Debug_Handler,          Evt_Debug_Handler,        Evt_Debug_Handler,      Evt_Debug_Handler },
    { TIMER_ID_DRAIN_PUMP_24H,      Evt_DrainPump_Handler,      NULL,                     NULL,                   NULL },
#endif
    { TIMER_ID_COMM_FRONT_RX_ERR,   Evt_FrontRxErr_Handler,     NULL,                     NULL,                   NULL },

    { TIMER_ID_COMP,                Evt_Comp_Handler,           Evt_Comp_Handler,         Evt_Comp_Handler,       Evt_Comp_Handler },
    { TIMER_ID_COMM_COMP_RX_ERR,    Evt_CompRxErr_Handler,      Evt_CompRxErr_Handler,    NULL,                   NULL },
    { TIMER_ID_CHECK_SYSTEM,        Evt_CheckSystem_Handler,    Evt_CheckSystem_Handler,  NULL,                   Evt_CheckSystem_Handler },

    /* TEST */
#if CONFIG_TEST_8585
    { TIMER_ID_TEST_8585,           Evt_Test_8585_Handler,      NULL,                     NULL,                   NULL },
    { TIMER_ID_TEST_8585_2,         Evt_Test_8585_2_Handler,    NULL,                     NULL,                   NULL },
#endif

#if CONFIG_TEST_8585_ICE
    { TIMER_ID_TEST_8585_ICE,       Evt_Test_8585_Ice_Handler,  NULL,                     NULL,                   NULL },
#endif

#if CONFIG_JIG_GAS_SWITCH
    { TIMER_ID_TEST_100MS,          Evt_Test_GasSwitch_Handler, NULL,                     NULL,                   NULL },
#endif
};
#define	SZ_LIST		( sizeof( SysEventList ) / sizeof( SysEvent_T ) )

void ProcessEventHandler( void )
{
    U8 i;
    Action_T fun;

    for( i = 0; i < SZ_LIST ; i++ )
    {
        if( IsExpiredTimer( SysEventList[ i ].timerId ) == TIMER_EXPIRE )
        {
            // Disable Expired Timer
            DisableTimer( SysEventList[ i ].timerId );

            // NORMAL or EOL 
            if( GetEolStatus() == TRUE )
            {
                // Get Eol Test Mode Event Handler
                fun = SysEventList[ i ].pFun_EOL; 
            }
            else if( GetFctStatus() == TRUE )
            {
                // Get Eol Test Mode Event Handler
                fun = SysEventList[ i ].pFun_FCT; 
            }
            else if( IsStartDiagnosis() == TRUE )
            {
                // Get DIAG Test Mode Event Handler
                fun = SysEventList[ i ].pFun_Diag; 
            }
            else
            {
                // Get Normal Mode Event Handler
                fun = SysEventList[ i ].pFun;
            }

            // Call a event handl
            if( fun != NULL )
            {
                fun();
            }
        }
    }
}

static void Evt_1ms_Handler( void )
{
    StartTimer( TIMER_ID_1MS, 1);

    HAL_ProcessAdc();
    
}


static void Evt_10ms_Handler( void )
{
    StartTimer( TIMER_ID_10MS, 10);

    /* KEY BUTTON */
    ProcessScanKey();
    ProcessKeyEventHandler();

    /* LEVER */
    ProcessScanLever();
    ProcessLeverEventHandler();

    /* WATER OUT */
    ControlWaterOut();
    ControlValve();

    ControlIceTray();
    ControlRelayConcurrent();
    
    ControlSwingBar();

    HAL_ProcessInput();


    TestAutoCountingFlowMeter();    // for debug...
}


static void Evt_100ms_Handler(void)
{
    StartTimer( TIMER_ID_100MS, 100);
    
    ProcessTemp();

    ProcessCheckTankOpen();     /* Service Check - Tank Open */
    ProcessTankLevel();         /* DETECT TANK WATER LEVEL */
    ProcessLeak();              /* Detect water leakage */

    ProcessFlowMeter();          /* FLOW METER - CALC HZ */

    ControlColdWaterLevel();  /* Control Cold Tank Water Level */
    ControlDrainLevel();      /* Control Drain Tank Water Level */

    ControlGasSwitch();
    ControlSmps();  
    ControlIceDoor();
    ControlFlowValve();         /* Control Flow Valve for hot water out */


    ProcessError();
    ProcessDisplay();

    ProcessIceTrayTest();

    /* ICE */
    CheckIceFull(); 
    UpdateIceMakerTimer();

    UpdateEolTimer();

#if !CONFIG_JIG_RBK_FRONT
    ProcessFilter();
#endif

#if !CONFIG_TEST_8585
    ProcessUv();
#endif
    ControlDrainWater();
    ControlEmptyColdTank();

    ProcessMetering();

    CheckHotOutHeaterOnTime();

    /* SMART CHECK */
    ControlSmartCheck();
    ControlMonitoring();

    UpdateSoundTimer();

    WifiControlProcess(WIFI_TIME_100MS);
}


static void Evt_1sec_Handler( void )
{
    StartTimer( TIMER_ID_1SEC, SEC(1) );

    /* Make Cold Water */
    CheckColdWaterTempStatus();
    MakeColdWater();

    /* Make ICE */
    MakeIce();
    MakeIceFull();

    /* Process making  cold water & ice */
    ProcessMake();

    ControlRelayExclusive();
    ControlDrainPump();

    /* Power Saving */
    ProcessSaving();

    /* EEPROM Writting */
    ProcessEeprom();

    UpdateFctTimer();
    UpdateTestKeyLimitTimer();

    // Time Short 
    UpdateTimeShortTimer();
    ProcessTimeShort();
    
    /* ICE DOOR FORCED CLOSED */
    ControlForcedCloseIceDoor();
    CountingIceDoorOpeningTime();


    /* HEALTH */
    ControlHealth();

    //ProcessTankClean();
    ProcessGasSwitch();     

    ProcessEnergy();

    /* RTC READ & WRITE */
#if CONFIG_RTC_TEST
    if( GetWaterOut() == FALSE )
    {
        ProcessRtc();
    }
#endif

    ProcessChangedStatus();

    WifiControlProcess(WIFI_TIME_1SEC);
}

static void Evt_10sec_Handler( void )
{
    StartTimer( TIMER_ID_10SEC, SEC(10) );

#if !CONFIG_RTC_TEST
    /* RTC READ & WRITE */
    if( GetWaterOut() == FALSE )
    {
        ProcessRtc();
    }
#endif
}

static void Evt_40sec_Handler( void )
{
    StartTimer( TIMER_ID_40SEC, SEC(40) );

    // 10초 주기로 1초 동안만 냉수 센서 읽기
    if( GetColdWaterConfigMake() == TRUE )
    {
        TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );
    }
}

static void Evt_1min_Handler( void )
{
    StartTimer( TIMER_ID_1MIN, SEC(60) );

    /* SERVICE CHECK TIMER */
    ProcessServiceCheck();

    /* FILTER USAGE TIMER */
    CountFilterUsage();
    
    /* UNSUED TIME ( SILVER CARE ) */
    CountingUnusedTime();

    /* SMART CHECK */
    ControlContinueSmartCheck();

    WifiControlProcess(WIFI_TIME_1MIN);
}

static void Evt_Debug_Handler( void )
{
    StartTimer( TIMER_ID_COMM_DEBUG_TX, 0 );
    StartTimer( TIMER_ID_DEBUG, 150 );

}

// 50ms 주기로 FRONT COMMUNICATION
#define FRONT_REFRESH_TIME     50 // @50ms...
static void Evt_Front_Handler( void )
{
    StartTimer( TIMER_ID_FRONT, FRONT_REFRESH_TIME );

    SetCommHeader( COMM_ID_FRONT, PKT_FRONT_REQ_LED_3 );
    StartTimer( TIMER_ID_COMM_FRONT_TX, 0 );
}


static void Evt_DrainPump_Handler( void )
{
    StartDrainPumpOnLevel();
}

// LCD DISPLAY UPDATE
U8 the_retry_cnt = 0;

// FRONT RX 통신 에러 헨들러
static void Evt_FrontRxErr_Handler( void )
{
    // 강제로 아이스 도어 닫음
    if( IsOpenIceDoor() == TRUE )
    {
        CloseIceDoor();
        Sound( SOUND_EFFLUENT_END );
    }
}

static void Evt_Comp_Handler( void )
{
    StartTimer( TIMER_ID_COMP, SEC(2) );

    SetCommHeader( COMM_ID_COMP, PKT_COMP_REQ );
    StartTimer( TIMER_ID_COMM_COMP_TX, 0 );
}

// COMP BLDC COMM ERROR
static void Evt_CompRxErr_Handler( void )
{
    SetCompBldcErrorComm( TRUE );
}

// power on 이후 1초 후 단 한번 수행
static void Evt_CheckSystem_Handler( void )
{
#if 0
    // 시스템 종류 결정 ( CHP or CP )
    if( HAL_GetInputValue( IN_HEATER ) != 0 )
    {
        SetSystem( SYSTEM_CHP );
    }
    else
    {
        SetSystem( SYSTEM_CP );
        // CP모델에서는 온수 자동채우기 없으모로 CLEAR
        // CHP 모델에서는 자동 추출 종료 후, CLEAR
        SetFotaReboot( FALSE );
    }
#endif

//    if( GetSystem() == SYSTEM_CP )
//    {
//        // CP모델에서는 온수 자동채우기 없으모로 CLEAR
//        // CHP 모델에서는 자동 추출 종료 후, CLEAR
//        SetFotaReboot( FALSE );
//    }

#if !CONFIG_TEST_HOT_OUT
    if( GetLockAll() == UNLOCK )
    {
        // 전원 리셋 플러싱 
        StartWaterOutFlushPowerOn();
    }
#else
    SetFlushSetupInit( FLUSH_STATUS_DONE );
    SetFlushStatus( FLUSH_STATUS_DONE );
    //SetFlushPowerOn( FLUSH_STATUS_DONE );
    SetColdWaterInitFull( TRUE );
    StartWaterOutFlushPowerOn();
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
// DIAG
/////////////////////////////////////////////////////////////////////////////////////////

static void Evt_Diag_1ms_Handler( void )
{
    StartTimer( TIMER_ID_1MS, 1);

    HAL_ProcessAdc();
}

static void Evt_Diag_10ms_Handler( void )
{
    StartTimer( TIMER_ID_10MS, 10);

    ControlIceTray();
    ControlValve();
    ControlRelayConcurrent();
    HAL_ProcessInput();;
}

static void Evt_Diag_100ms_Handler( void )
{
    StartTimer( TIMER_ID_100MS, 100);

    ProcessTemp();
    ProcessTankLevel();       /* DETECT TANK WATER LEVEL */
    ProcessFlowMeter();          /* FLOW METER - CALC HZ */

    ProcessDisplay();

    ProcessIceTrayTest();

    ControlSmartCheck();
    ControlDiagnosis();
    ControlMonitoring();

    /* WIFI */
    WifiControlProcess(WIFI_TIME_100MS);    
}

static void Evt_Diag_1sec_Handler( void )
{
    StartTimer( TIMER_ID_1SEC, SEC(1) );

    ControlCompBldc();
    ControlRelayExclusive();
    ControlRelayConcurrent();

    ProcessChangedStatus();

    /* WIFI */
    WifiControlProcess(WIFI_TIME_1SEC);
}

static void Evt_Diag_40sec_Handler( void )
{
    StartTimer( TIMER_ID_40SEC, SEC(40) );

    // 10초 주기로 1초 동안만 냉수 센서 읽기
    if( GetColdWaterConfigMake() == TRUE )
    {
        TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );
    }
}

static void Evt_Diag_1min_Handler( void )
{
    StartTimer( TIMER_ID_1MIN, SEC(60) );

    /* WIFI */
    WifiControlProcess(WIFI_TIME_1MIN);
}


void TimerIsrCallback(void)
{
    FilterFlowMeterNoise();
}

void InitSystem(void)
{
    /* IO */
    InitRelay();
    InitValve();
    InitIceTray();
    InitIceTrayTest();
    InitDrainPump();
    InitDrain();
    InitKey();
    HAL_InitComm();
    HAL_InitAdc();
    HAL_InitLed();
    InitUv();
    InitVoice();
    InitSound();

    /* IO - ICE */
    HAL_InitStepMotor();
    InitGasSwitch();
    InitIceDoor();
    InitSwingBar();

    InitLeak();
    InitServiceCheck();
    InitDisplay();


    /* WATER */
    InitColdWater();
    InitHotWater();
    InitWaterOut();

    InitFlowMeter();
    InitFlowValve();

    /* HOT */
    InitHotOut();
    InitHotWaterFlow();
    InitHotWaterHeater();
    InitHeaterOut();

    /* ICE */
    InitFront();
    InitIce();
    InitIceFull();

    /* ERROR */
    InitErrorList();
    
    InitProcessMake();

    InitSaving();
    InitEol();
    InitFct();
    InitTimeShort();

    /* Ster */
    InitHealth();
    InitDrainWater();
    InitHealthDrain();
    InitUv();
    InitEmptyColdTank();

    InitUser();
    InitEnergy();
    InitFilter();
    InitMetering();
    InitCodyKit();

    InitSmartCheck();

    /* WIFI */
    WifiControlProcess(WIFI_TIME_INI);

    // 최초 냉수 센서 초기 읽기
    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_INIT_TEMP_READ_TIME );
    TurnOnTempSensor( TEMP_ID_ROOM_WATER, DEFAULT_INIT_TEMP_READ_TIME );

    // Register Timer Callback
    RegisterTimerISR( TimerIsrCallback );
}

static void Evt_FCT_1ms_Handler( void )
{
    StartTimer( TIMER_ID_1MS, 1);


    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_INIT_TEMP_READ_TIME );
    TurnOnTempSensor( TEMP_ID_ROOM_WATER, DEFAULT_INIT_TEMP_READ_TIME );

    HAL_ProcessAdc();
}

static void Evt_EOL_1ms_Handler(void)
{
    StartTimer( TIMER_ID_1MS, 1);

    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_INIT_TEMP_READ_TIME );
    TurnOnTempSensor( TEMP_ID_ROOM_WATER, DEFAULT_INIT_TEMP_READ_TIME );

    HAL_ProcessAdc();
}

static void Evt_EOL_10ms_Handler(void)
{
    U8 mu8EolMode;

    StartTimer( TIMER_ID_10MS, 10);

    HAL_ProcessInput();

    /* KEY BUTTON */
    ProcessScanKey();
    ProcessKeyEventHandler();

    /* LEVER */
    ProcessScanLever();
    ProcessLeverEventHandler();


    mu8EolMode = GetEolMode();
    if( mu8EolMode == EOL_MODE_READY 
            || mu8EolMode == EOL_MODE_DONE )
    {
        /* WATER OUT */
        ControlWaterOut();

    }

    if( mu8EolMode == EOL_MODE_READY 
            || mu8EolMode == EOL_MODE_INIT
            || mu8EolMode == EOL_MODE_CHECK_SENSOR
            || mu8EolMode == EOL_MODE_DONE 
            || mu8EolMode == EOL_MODE_SEALED_INIT 
            || mu8EolMode == EOL_MODE_SEALED 
            )
    {
        /* ICE TRAY */
        ControlIceTray();
    }
    
    ControlValve();
    ControlRelayConcurrent();
    CheckIceFull(); 
}

static void Evt_EOL_100ms_Handler(void)
{
    StartTimer( TIMER_ID_100MS, 100);

    ProcessCheckTankOpen();   /* Service Check - Tank Open */
    ProcessTankLevel();       /* DETECT TANK WATER LEVEL */

    ControlIceDoor();
    ControlFlowValve();         /* Control Flow Valve for hot water out */
    ProcessDisplay();

    /* EOL */
    UpdateEolTimer();
    ProcessEol();

#if !CONFIG_JIG_RBK_FRONT
    CheckFilterStatus();
#endif


    ProcessFlowMeter();          /* FLOW METER - CALC HZ */
    ControlGasSwitch();
    ControlSmps();
    ProcessMetering();

    UpdateSoundTimer();
    WifiControlProcess(WIFI_TIME_100MS);
}


static void Evt_EOL_1sec_Handler( void )
{
    StartTimer( TIMER_ID_1SEC, SEC(1) );

    ControlCompBldc();
    ControlRelayExclusive();
    WifiControlProcess(WIFI_TIME_1SEC);
}

static void Evt_EOL_1min_Handler( void )
{
    StartTimer( TIMER_ID_1MIN, SEC(60) );

    WifiControlProcess(WIFI_TIME_1MIN);
}

/* FCT */
static void Evt_FCT_10ms_Handler( void )
{
    StartTimer( TIMER_ID_10MS, 10);

    /* KEY BUTTON */
    ProcessScanKey();
    ProcessKeyEventHandler();

    /* LEVER */
    ProcessScanLever();
    ProcessLeverEventHandler();

    HAL_ProcessInput();
    ProcessFctLoadTest();

    CheckIceFull(); 
    ControlValve();
}


static void Evt_FCT_100ms_Handler(void)
{
    StartTimer( TIMER_ID_100MS, 100);

    UpdateSoundTimer();
    WifiControlProcess(WIFI_TIME_100MS);

}

static void Evt_FCT_1sec_Handler( void )
{
    StartTimer( TIMER_ID_1SEC, SEC(1) );

    UpdateFctTimer();
    ControlRelayExclusive();
    ControlRelayConcurrent();

    ProcessFct();

    WifiControlProcess(WIFI_TIME_1SEC);
}

static void Evt_FCT_1min_Handler( void )
{
    StartTimer( TIMER_ID_1MIN, SEC(60) );

    WifiControlProcess(WIFI_TIME_1MIN);
}


#if (CONFIG_TEST_8585_FILTER==0)

#define TEST_WATER_OUT_TIME     2100UL     // 35min ( 35x60 = 2100  )
#define TEST_WATER_OUT_TIME_2   1UL     // 35min ( 35x60 = 2100  )
U8 the_8585_step = 0;
static void Evt_Test_8585_Handler( void )
{
    if( GetHotWaterInitFull() == FALSE 
            || IsDoneFlushInitSetup() == FALSE )
    {
        the_8585_step = 0;
        StartTimer( TIMER_ID_TEST_8585, SEC( 1 ) );
        return ;
    }

    switch( the_8585_step )
    {
            // COLD
        case 0:
            if( GetWaterOut() == TRUE )
            {
                StartTimer( TIMER_ID_TEST_8585, SEC( 10 ) );
                break;
            }
            SetWaterOutSelect( SEL_WATER_COLD );
            SetWaterOutFixedAmountType( FIXED_AMOUNT_HALF );

            StartWaterOut();
            StartTimer( TIMER_ID_TEST_8585, SEC( 3600U ) );
            the_8585_step++;
            break;

        case 1:
            StartTimer( TIMER_ID_TEST_8585, SEC( 3600U ) );
            the_8585_step++;
            break;

        case 2:
            StartTimer( TIMER_ID_TEST_8585, SEC( 0 ) );
            the_8585_step = 0;
            break;

        default:
            StartTimer( TIMER_ID_TEST_8585, 0 );
            the_8585_step = 0;
            break;
    }
}


U8 the_8585_step_2 = 0;
static void Evt_Test_8585_2_Handler( void )
{
    if( GetHotWaterInitFull() == FALSE 
            || IsDoneFlushInitSetup() == FALSE )
    {
        the_8585_step_2 = 0;
        StartTimer( TIMER_ID_TEST_8585_2, SEC( 1 ) );
        return ;
    }

    switch( the_8585_step_2 )
    {
        case 0:
            StartTimer( TIMER_ID_TEST_8585_2, SEC( 2100U ) );   //35
            the_8585_step_2++;
            break;

            // ROOM
        case 1:
            if( GetWaterOut() == TRUE )
            {
                StartTimer( TIMER_ID_TEST_8585_2, SEC( 10 ) );
                break;
            }
            SetWaterOutSelect( SEL_WATER_ROOM );
            SetWaterOutFixedAmountType( FIXED_AMOUNT_HALF );

            StartWaterOut();
            StartTimer( TIMER_ID_TEST_8585_2, SEC( TEST_WATER_OUT_TIME_2 ) );
            the_8585_step_2++;
            break;

            // HOT
        case 2:
            if( GetWaterOut() == TRUE )
            {
                StartTimer( TIMER_ID_TEST_8585_2, SEC( 10 ) );
                break;
            }
            SetWaterOutSelect( SEL_WATER_HOT );
            SetWaterOutFixedAmountType( FIXED_AMOUNT_HALF );
            SetHotSelect( SEL_HOT_USER );

            StartWaterOut();
            StartTimer( TIMER_ID_TEST_8585_2, SEC( TEST_WATER_OUT_TIME_2 ) );
            the_8585_step_2 = 0;
            break;

        default:
            StartTimer( TIMER_ID_TEST_8585_2, 0 );
            the_8585_step_2 = 0;
            break;
    }
}
#else

#if CONFIG_TEST_8585
U8 the_8585_step = 0;
#define DBG_THE_24H     86400U
U32 the_8585_time = DBG_THE_24H;
static void Evt_Test_8585_Handler( void )
{

    if( IsDoneFlushInitSetup() == FALSE )
    {
        the_8585_step = 0;
        StartTimer( TIMER_ID_TEST_8585, SEC( 1 ) );
        return ;
    }

    switch( the_8585_step )
    {
        case 0:
            // 대기..
            if( GetWaterOut() == TRUE )
            {
                StartTimer( TIMER_ID_TEST_8585, SEC( 3 ) );
                break;
            }

            // 정수 추출 시작
            SetWaterOutSelect( SEL_WATER_ROOM );
            SetWaterOutFixedAmountType( FIXED_AMOUNT_INFINITY );
            StartWaterOut();

            StartTimer( TIMER_ID_TEST_8585, SEC( 1 ) );
            the_8585_time = DBG_THE_24H;
            the_8585_step++;
            break;

        case 1:
            if( the_8585_time != 0 )
            {
                the_8585_time--;

                // 정수 off이면 재추출..
                if( GetWaterOut() == FALSE )
                {
                    SetWaterOutSelect( SEL_WATER_ROOM );
                    SetWaterOutFixedAmountType( FIXED_AMOUNT_INFINITY );
                    StartWaterOut();
                }
            }
            else
            {
                StopWaterOut();
                the_8585_step++;
            }

            StartTimer( TIMER_ID_TEST_8585, SEC( 3 ) );
            break;

        case 2:
            // 냉수 추출
            SetWaterOutSelect( SEL_WATER_COLD );
            SetWaterOutFixedAmountType( FIXED_AMOUNT_INFINITY );
            StartWaterOut();

            StartTimer( TIMER_ID_TEST_8585, SEC( 1 ) );
            the_8585_time = DBG_THE_24H;
            the_8585_step++;
            break;

        case 3:
            if( the_8585_time != 0 )
            {
                the_8585_time--;

                //  냉수 off이면 재추출..
                if( GetWaterOut() == FALSE )
                {
                    SetWaterOutSelect( SEL_WATER_COLD );
                    SetWaterOutFixedAmountType( FIXED_AMOUNT_INFINITY );
                    StartWaterOut();
                }
            }
            else
            {
                StopWaterOut();
                if( GetSystem() == SYSTEM_CHP )
                {
                    the_8585_step++;
                }
                else
                {
                    the_8585_step = 6;
                }
            }

            StartTimer( TIMER_ID_TEST_8585, SEC( 3 ) );
            break;

        case 4:
            // 온수 추출
            SetWaterOutSelect( SEL_WATER_HOT );
            SetHotSelect( SEL_HOT_MILK );
            SetWaterOutFixedAmountType( FIXED_AMOUNT_INFINITY );
            StartWaterOut();

            StartTimer( TIMER_ID_TEST_8585, SEC( 1 ) );
            the_8585_time = DBG_THE_24H;
            the_8585_step++;
            break;

        case 5:
            if( the_8585_time != 0 )
            {
                the_8585_time--;

                //  온수 추출
                if( GetWaterOut() == FALSE )
                {
                    SetWaterOutSelect( SEL_WATER_HOT );
                    SetHotSelect( SEL_HOT_MILK );
                    SetWaterOutFixedAmountType( FIXED_AMOUNT_INFINITY );
                    StartWaterOut();
                }
            }
            else
            {
                StopWaterOut();
                the_8585_step++;
            }

            StartTimer( TIMER_ID_TEST_8585, SEC( 3 ) );
            break;

        case 6:
            SetSavingConfig( FALSE );
            DisableTimer( TIMER_ID_TEST_8585 );
            DisableTimer( TIMER_ID_TEST_8585_2 );
            the_8585_step = 0;

            break;

        default:
            the_8585_step = 0;
            break;
    }
}

static void Evt_Test_8585_2_Handler( void )
{
    // do noting...
}
#endif  /* #if CONFIG_TEST_8585 */

#endif /* #if (CONFIG_TEST_8585_FILTER==0) */


#if CONFIG_TEST_8585_ICE
U8 the_8585_step_ice = 0;
static void Evt_Test_8585_Ice_Handler( void )
{
    switch( the_8585_step_ice )
    {
        // ICE OUT
        case 0:
            if( GetIceFullStatus() == TRUE )
            {
                StartTimer( TIMER_ID_TEST_8585_ICE, SEC(1800UL) );
                the_8585_step_ice++;
            }
            else
            {
                StartTimer( TIMER_ID_TEST_8585_ICE, SEC(1) );
            }
            break;

        case 1:
            OpenIceDoor();
            StartTimer( TIMER_ID_TEST_8585_ICE, SEC(30) );
            the_8585_step_ice++;
            break;

        case 2:
            CloseIceDoor();
            StartTimer( TIMER_ID_TEST_8585_ICE, SEC(60) );
            the_8585_step_ice = 0;
            break;

        default:
            StartTimer( TIMER_ID_TEST_8585_ICE, SEC(0) );
            the_8585_step_ice = 0;
            break;
    }
}
#endif


#if CONFIG_JIG_GAS_SWITCH
U8 test_step = 0;
U16 test_count = 0;
U16 test_mu16Delay = 0;
#include "fan_motor.h"
static void Evt_Test_GasSwitch_Handler( void )
{

    SetIceConfigMake( FALSE );
    SetColdWaterConfigMake( FALSE );

    StartTimer( TIMER_ID_TEST_100MS, 100 );

    if( IsExpiredCompProtectTime() == FALSE )
    {
        test_step = 0;
        test_count = 72;
        return ;
    }

    if( IsError( ERR_COMP ) == TRUE 
            || IsError( ERR_COMP_COMM ) == TRUE )
    {
        TurnOffComp( COMP_TYPE_ICE );
        TurnOffFan( FAN_TYPE_ICE );
        StartTimer( TIMER_ID_TEST_100MS, SEC(10) );
        return ;
    }


    switch( test_step )
    {
        case 0:
            test_count--;
            if( test_count == 0 )
            {
                TurnOffComp( COMP_TYPE_ICE );
                TurnOffFan( FAN_TYPE_ICE );
                StartTimer( TIMER_ID_TEST_100MS, SEC(10) );
                return ;
            }

            GasSwitchIce();
            TurnOnCompRps(COMP_TYPE_ICE, 60 );
            TurnOnFan();
            test_mu16Delay = 300;    // 30sec
            test_step++;
            break;

        case 1:
            TurnOnCompRps(COMP_TYPE_ICE, 60 );
            TurnOnFan();
            GasSwitchIce();

            if( GetGasSwitchStatus() == GAS_SWITCH_ICE )
            {
                if( test_mu16Delay != 0 )
                {
                    test_mu16Delay--;
                }
                else
                {
                    GasSwitchHotGas();
                    TurnOnFan();
                    test_mu16Delay = 50; // 5sec

                    test_step++;
                }
            }
            break;

        case 2:
            GasSwitchHotGas();
            TurnOnFan();
            TurnOnCompRps(COMP_TYPE_ICE, 60 );
            if( GetGasSwitchStatus() == GAS_SWITCH_HOTGAS )
            {
                if( test_mu16Delay != 0 )
                {
                    test_mu16Delay--;
                }
                else
                {
                    test_step = 0;
                }
            }
            break;

        default:
            test_step = 0;
            break;
    }
}
#endif
