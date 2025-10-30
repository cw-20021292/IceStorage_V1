#include "hw.h"
#include "process_eol.h"
#include "eol.h"
#include "parser_eol.h"

#include "key.h"
#include "eeprom.h"
#include "rtc.h"
#include "util.h"
#include "comm.h"
#include "timer.h"

#include "valve.h"
#include "relay.h"
#include "service.h"
#include "ice_door.h"
#include "gas_switch_valve.h"
#include "temp.h"
#include "hal_adc.h"
#include "hal_input.h"
#include "hal_pump.h"
#include "hal_uv.h"
#include "hal_fan_motor.h"
#include "hal_step_motor.h"
#include "ice_full.h"
#include "ice_tray.h"
#include "level.h"
#include "drain_pump.h"
#include "err_temp_eva_1.h"
#include "cold_water.h"
#include "leak.h"
#include "flow_valve.h"
#include "flow_meter.h"
#include "voice.h"
#include "WIFI_Control.h"




// ?ƒ‰ë§? ?œ ë¡? ?‹œ?Š¤?…œ ê²??‚¬
enum
{
    EOL_ICE_INIT_ICE,
    EOL_ICE_CHECK_ICE,
    EOL_ICE_SWITCH,
    EOL_ICE_WAIT_SWITCH,
    EOL_ICE_INIT_COLD,
    EOL_ICE_CHECK_COLD,
    EOL_ICE_DONE
};


static void CheckEeprom(void)
{
    static U8 mu8Init = FALSE;
    static U8 mu8RetryCount = 5;
    U8 mu8Ret = FALSE;
    U8 mu8Ret_Rtc = FALSE;


    if( mu8Init == TRUE )
    {
        return ;
    }

    // Check eeprom
    mu8Ret = TestEeprom();
    mu8Ret_Rtc = TestRtc();
    if( mu8Ret == TRUE && mu8Ret_Rtc == TRUE )
    {
        mu8Init = TRUE;

        InitRtcTime();
        SetEolCheckStatus( EOL_CHK_ID_EEPROM, 0xA5 );  // OK
    }
    else
    {
        SetEolCheckStatus( EOL_CHK_ID_EEPROM, 0x01 );  // ERROR
    }

    if( --mu8RetryCount == 0 )
    {
        mu8Init = TRUE;
    }
}

U8 MovingIceTray(U8 mu8Step )
{
    switch( mu8Step )
    {
        case 0:
            MoveIceTrayIcingEol();
            mu8Step++;
            break;

        case 1:
            if( GetIceTrayStatus() == ICE_TRAY_ICING )
            {
                mu8Step++;
            }
            break;

        case 2:
            MoveIceTrayDeIcing();
            mu8Step++;
            break;

        case 3:
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                mu8Step++;
            }
            break;

        case 4:
            mu8Step = 0;
            break;

        default :
            mu8Step = 0;
            break;
    }

    return mu8Step;
}

static void TestIceTray(void)
{
    static U8 mu8Step = 0;

    mu8Step = MovingIceTray( mu8Step );
    if( mu8Step == 2 )
    {
        SetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_ICING, 1 );
    }
    else if( mu8Step == 4 )
    {
        SetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_DEICING, 1 );
    }
}



static void CheckSensorDrainPump(void)
{
    U16 mu16Tmp;
    U16 mu16Adc;
    static U16 mu16Time = 50;  // 5sec

    if( mu16Time != 0 )
    {
        mu16Time--;
        HAL_TurnOnPump( HAL_PUMP_DRAIN, 24.0f );
    }
    else
    {
        HAL_TurnOffPump( HAL_PUMP_DRAIN );
    }

    mu16Tmp = GetEolCheckStatus( EOL_CHK_ID_DRAIN_PUMP );
    mu16Adc = HAL_GetAdcValue( ANI_PUMP_DRAIN_FB );
    if( mu16Tmp < mu16Adc )
    {
        SetEolCheckStatus( EOL_CHK_ID_DRAIN_PUMP, mu16Adc );
    }
}

#if 0
static void CheckSensorSodaPump(void)
{
    U16 mu16Tmp;
    U16 mu16Adc;

    mu16Tmp = GetEolCheckStatus( EOL_CHK_ID_SODA_PUMP );
    mu16Adc = HAL_GetAdcValue( ANI_PUMP_SODA_CURRENT );
    if( mu16Tmp < mu16Adc )
    {
        SetEolCheckStatus( EOL_CHK_ID_SODA_PUMP, mu16Adc );
    }
}
#endif


static void CheckSensorIceFull(void)
{
    U16 mu16Tmp;
    U16 mu16Adc;


    StartCheckIceFull();
    mu16Tmp = GetEolCheckStatus( EOL_CHK_ID_ICE_FULL );
    mu16Adc = HAL_GetAdcValue( ANI_SENSOR_INFRARED );
    if( mu16Tmp < mu16Adc )
    {
        SetEolCheckStatus( EOL_CHK_ID_ICE_FULL, mu16Adc );
    }
}

U16 the_eol_uv_ice_door = 0;
U16 the_eol_uv_water_out = 0;
static void CheckSensorFrontUvLed(void)
{
    U16 mu16Tmp;
    U16 mu16Adc;
    static U16 mu16Time = 30;  // 3sec

    // time limit, protect eyes of workers...
    if( mu16Time != 0 )
    {
        mu16Time--;

        HAL_TurnOnUv( HAL_UV_ICE_DOOR );
        HAL_TurnOnUv( HAL_UV_WATER_OUT );
    }
    else
    {
        HAL_TurnOffUv( HAL_UV_ICE_DOOR );
        HAL_TurnOffUv( HAL_UV_WATER_OUT );
    }

    // ICE TANK
    mu16Tmp = GetEolCheckStatus( EOL_CHK_ID_UV_ICE_DOOR );
    mu16Adc = HAL_GetAdcValue( ANI_UV_ICE_DOOR );
    the_eol_uv_ice_door = mu16Adc;
    if( mu16Tmp < mu16Adc )
    {
        SetEolCheckStatus( EOL_CHK_ID_UV_ICE_DOOR, mu16Adc );
    }

    // ICE TRAY
    mu16Tmp = GetEolCheckStatus( EOL_CHK_ID_UV_WATER_OUT );
    mu16Adc = HAL_GetAdcValue( ANI_UV_WATER_OUT );
    the_eol_uv_water_out = mu16Adc;
    if( mu16Tmp < mu16Adc )
    {
        SetEolCheckStatus( EOL_CHK_ID_UV_WATER_OUT, mu16Adc );
    }
}

static void CheckSensorBodyUvLed(void)
{
    U16 mu16Tmp;
    U16 mu16Adc;
    static U16 mu16Time = 30;  // 3sec

    // time limit, protect eyes of workers...
    if( mu16Time != 0 )
    {
        mu16Time--;

        HAL_TurnOnUv( HAL_UV_ICE_TANK );
        HAL_TurnOnUv( HAL_UV_ICE_TRAY );
    }
    else
    {
        HAL_TurnOffUv( HAL_UV_ICE_TANK );
        HAL_TurnOffUv( HAL_UV_ICE_TRAY );
    }

    // ICE TANK
    mu16Tmp = GetEolCheckStatus( EOL_CHK_ID_UV_ICE_TANK );
    mu16Adc = HAL_GetAdcValue( ANI_UV_ICE_TANK );
    if( mu16Tmp < mu16Adc )
    {
        SetEolCheckStatus( EOL_CHK_ID_UV_ICE_TANK, mu16Adc );
    }

    // ICE TRAY
    mu16Tmp = GetEolCheckStatus( EOL_CHK_ID_UV_ICE_TRAY );
    mu16Adc = HAL_GetAdcValue( ANI_UV_ICE_TRAY );
    if( mu16Tmp < mu16Adc )
    {
        SetEolCheckStatus( EOL_CHK_ID_UV_ICE_TRAY, mu16Adc );
    }
}

static U8 IsDetectLeak(void)
{
    U16 mu16Adc;

    TURN_ON_LEAK_ONOFF();
    mu16Adc = HAL_GetAdcValue( ANI_SENSOR_LEAK );
    if( mu16Adc > 50 )
    {
        return TRUE;
    }

    return FALSE;
}

static void CheckSensorLeak(void)
{
    if( IsDetectLeak() == TRUE )
    {
        SetEolCheckStatus( EOL_CHK_ID_LEAK, 1 );
    }
    else
    {
        SetEolCheckStatus( EOL_CHK_ID_LEAK, 0 );
    }
}

static void CheckSensorFlowMeter(void)
{
    U16 mu16Val;

    mu16Val = GetFlowMeterHz();
    if( GetEolCheckStatus( EOL_CHK_ID_FLOW_METER ) < mu16Val )
    {
        SetEolCheckStatus( EOL_CHK_ID_FLOW_METER, mu16Val );
    }
}

static void CheckTankCover(void)
{
    // LOW : CLOSE 
    // HIGH : OPEN
    static U8 mu8Prev = LOW; 
    U8 mu8Val;


    mu8Val = HAL_GetInputValue( IN_TANK_OPEN );
    if( mu8Prev != mu8Val)
    {
        mu8Prev = mu8Val;
        if( mu8Val == LOW )
        {
            SetEolTestInputBit( MK_EOL_TANK_COVER );
        }
    }
}

static void CheckSensor(void)
{
    CheckTankCover();
    CheckEeprom();

    // ICE TRAY
    TestIceTray();

    CheckSensorDrainPump();
    //CheckSensorSodaPump();
    CheckSensorIceFull();
}


// ?–¼?Œ ?”¼?” ?Š¤?¬ë¥? ?™?ž‘.
// ?•„?´?Š¤?„ ?„?–´ OPEN/CLOSE ë°˜ë³µ ?™?ž‘.
static void CheckVisualTest(void)
{
    U8 mu8Status;


    // Ice Door
    mu8Status = GetIceDoorStatus();
    if( mu8Status == ICE_DOOR_OPEN )
    {
        CloseIceDoor();
    }
    else if( mu8Status  == ICE_DOOR_CLOSE )
    {
        OpenIceDoor();
    }

    // Feeder Screw 
    TurnOnRelay( RELAY_FEEDER_CW );
    TurnOffRelay( RELAY_FEEDER_CCW );

}

static void StopVisualTest(void)
{
    // Close ice door
    if( IsOpenIceDoor() == TRUE )
    {
        CloseIceDoor();
    }

    // Feeder Screw 
    TurnOffRelay( RELAY_FEEDER_CW );
    TurnOffRelay( RELAY_FEEDER_CCW );
}


static void OnOffDrain(void)
{
#if 0
    // ë°°ìˆ˜ ?™?ž‘
    if( GetEolDrainWater() == TRUE )
    {
        OpenValve( VALVE_DRAIN );
        HAL_TurnOnPump( HAL_PUMP_DRAIN, 24.0f );
    }
    else
    {
        CloseValve( VALVE_DRAIN );
        HAL_TurnOffPump( HAL_PUMP_DRAIN );
    }
#endif
}

#if 0
#define EOL_DRAIN_TIME   2400U // 4min = 4x60x10 = 2400 @100ms
static void DrainWater(void)
{
    static U16 mu16DrainTimer = EOL_DRAIN_TIME; 

    OnOffDrain();

    // ?ƒ‰?ˆ˜ ????ˆ˜?œ„ ?„¼?„œ ë¯¸ê°ì§? ?›„, ì¶”ê?? ?™?ž‘ ?›„ ì¢…ë£Œ
    if( GetEolDrainWater() == TRUE )
    {
        if( IsDetectTankLevel( LEVEL_ID_COLD, LEVEL_DETECT_LOW ) == TRUE )
        {
            mu16DrainTimer = EOL_DRAIN_TIME; 
        }

        if( mu16DrainTimer == 0 )
        {
            Reset();
        }
        else
        {
            mu16DrainTimer--;
        }
    }
}
#endif

static void InnerProcessFront(void)
{
    // Vol up MAX
    SetVoiceVolume( MAX_VOICE_VOLUME );

    // EEPROM
    CheckEeprom();

    // Check UV 
    CheckSensorFrontUvLed();

    // CHECK LEAK
    CheckSensorLeak();

#if 0
    if( GetWifiStatusValue(WIFI_STATUS_TEST) != 0 )
    {
        // CONNCTED
        SetEolTestInputBit( MK_EOL_WIFI );
    }
    else
    {
        // DIS-CONNTECT
        ClearEolTestInputBit( MK_EOL_WIFI );
    }
#endif

    // Visual Test ( ICE DOOR, FEEDER SCREW )
    // ì¡°ë„ ?„¼?„œ ê°ì?? ?…Œ?Š¤?Š¸ ?•˜ë©? ?•„?´?Š¤ ?„?–´ ?‹«?Œ
#if !CONFIG_JIG_RBK_FRONT
    if( (GetEolTestInputVal() & MK_EOL_LEVER_WATER ) != 0 )
    {
        StopVisualTest();
    }
    else
    {
        CheckVisualTest();
    }
#endif

#if CONFIG_JIG_RBK_FRONT
    // DONE - OUTPUT PORT 
    if( (GetEolTestInputVal() & MK_EOL_FILTER_COVER) != 0 )
    {
        // TURN ON OUTPUT +24V
        CloseValve( VALVE_NOS );
    }
    else
    {
        // TURN OFF OUPTU +24V
        OpenValve( VALVE_NOS );
    }
#endif

    return ;
}

static void ModeReady(void)
{
    // Visual Test
    CheckVisualTest();

    // Check EOL Input 
    CheckSensor();

    OnOffDrain();

    HAL_TurnOnFanMotor();

    // STOP ON/OFF HEATER TIMER ISR
    R_TAU0_Channel2_Stop(); 

    // Filter cover ?ƒ?ƒœ?— ë¬´ê???•˜ê²? Open
    OpenValve( VALVE_NOS );
    CloseValve( VALVE_ICE_TRAY_IN );
}

static void ModeInit(void)
{
    TurnOffRelay( RELAY_COMP );
    TurnOffRelay( RELAY_FEEDER_CW );
    TurnOffRelay( RELAY_FEEDER_CCW );


    // ë°°ìˆ˜ ?ŽŒ?”„ OFF
    HAL_TurnOffPump( HAL_PUMP_DRAIN );
    HAL_TurnOffFanMotor();

    CloseValve( VALVE_ALL );
    OpenValve( NORMAL_OPEN_VALVES );

}


#define EOL_HEATER_DELAY_TIME   5       // 0.5sec @100ms
U8 RunCheckHeater   = TRUE;             // ?ˆœê°? ?˜¨?ˆ˜ ?žˆ?„° ê²??‚¬
// ?ˆœê°? ?˜¨?ˆ˜ ?žˆ?„° ê²??‚¬
// return TRUE : ê²??‚¬ ?™„ë£?, FALSE : ê²??‚¬ ì§„í–‰ ì¤?
static void CheckHeaterFeedback(void)
{
    static U8 mu8Step = 0;
    static U16 mu16Delay = EOL_HEATER_DELAY_TIME;
    U16 mu16Val = 0;
    U16 mu16NewVal = 0;


    if( RunCheckHeater == FALSE )
    {
        TURN_OFF_HEATER_RELAY();
        TURN_OFF_HEATER_TRIAC();

        return ;
    }

    switch( mu8Step )
    {
        case 0:
            // CHECK HEATER LOW SIDE
            TURN_ON_HEATER_TRIAC();
            TURN_OFF_HEATER_RELAY();

            mu16Val = HAL_GetAdcValue( ANI_HEATER_FB );
            if( GetEolCheckStatus( EOL_CHK_ID_HEATER_TRIAC ) < mu16Val )
            {
                SetEolCheckStatus( EOL_CHK_ID_HEATER_TRIAC, mu16Val );
            }

            if( mu16Delay != 0 )
            {
                mu16Delay--;
            }
            else
            {
                mu16Delay = EOL_HEATER_DELAY_TIME;
                mu8Step++;
            }

            break;

        case 1:
            // CHECK HEATER HIGH SIDE
            TURN_OFF_HEATER_TRIAC();
            TURN_ON_HEATER_RELAY();

            mu16Val = HAL_GetAdcValue( ANI_HEATER_FB );
            if( GetEolCheckStatus( EOL_CHK_ID_HEATER_RELAY ) < mu16Val )
            {
                SetEolCheckStatus( EOL_CHK_ID_HEATER_RELAY, mu16Val );
            }

            if( mu16Delay != 0 )
            {
                mu16Delay--;
            }
            else
            {
                mu8Step++;
            }
            break;

        case 2:
            TURN_OFF_HEATER_RELAY();
            TURN_OFF_HEATER_TRIAC();

            RunCheckHeater = FALSE;
            break;

        default:
            break;
    }
}

// ICE TRAY MOTOR
U8 RunCheckIceTray  = FALSE;        // ?•„?´?Š¤ ?Š¸? ˆ?´ ê²??‚¬
static void CheckLoadIceTray(void)
{
    static U8 mu8Go = 0;
    U8 mu8Icing;
    U8 mu8DeIcing;


    mu8Icing    = GET_MICRO_SW_ICE_TRAY_1();
    mu8DeIcing  = GET_MICRO_SW_ICE_TRAY_2();


    if( RunCheckIceTray == FALSE )
    {
        TurnOffRelay( RELAY_ICE_TRAY_CCW );
        TurnOffRelay( RELAY_ICE_TRAY_CW );
        return ;
    }

    if( mu8Icing == LOW && mu8DeIcing == LOW )
    {
        // ëª¨ë‘ ê°ì??, ? •ì§?
        mu8Go = 0;
    }
    else if( mu8Icing == HIGH && mu8DeIcing == LOW && mu8Go != 1  ) 
    {
        // ?ƒˆë¹? ê°ì?? ?ƒ?ƒœ
        // ? œë¹? ë¯¸ê°ì§?, ? œë¹? ?´?™
        mu8Go = 1;

        SetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_DEICING, 1 );
    }
    else if( mu8Icing == LOW && mu8DeIcing == HIGH && mu8Go != 2)
    {
        // ? œë¹? ê°ì?? ?ƒ?ƒœ
        // ?ƒˆë¹? ë¯¸ê°ì§?, ?ƒˆë¹? ?´?™
        mu8Go = 2;

        SetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_ICING, 1 );
    }
    else if( mu8Icing == HIGH && mu8DeIcing == HIGH )
    {
        // ëª¨ë‘ ë¯¸ê°ì§?,  ?ƒˆë¹? ?œ„ì¹? ?´?™
        if( mu8Go == 0 )
        {
            mu8Go = 2;  
        }
    }

    if( mu8Go == 0 )
    {
        // ? •ì§? 
        TurnOffRelay( RELAY_ICE_TRAY_CCW );
        TurnOffRelay( RELAY_ICE_TRAY_CW );
    }
    else if( mu8Go == 1)
    {
        // ? œë¹? ?´?™
        TurnOffRelay( RELAY_ICE_TRAY_CCW );
        TurnOnRelay( RELAY_ICE_TRAY_CW );
    }
    else
    {
        // ?ƒˆë¹? ?´?™
        TurnOnRelay( RELAY_ICE_TRAY_CCW );
        TurnOffRelay( RELAY_ICE_TRAY_CW );
    }
}

//typedef struct _eol_seald_test_
//{
//    U8 CompTargetRps;
//    U16 CompoffTime;
//    U8 GasSwitch;
//    U8 
//
//}EOL_Seal_T;
#define EOL_COMP_RPS    45U
#define EOL_COMP_OFF_DELAY_TIME     3000U   //5min @100ms
U8 EOL_PrevComp = OFF;
U8 EOL_Comp = OFF;
U8 EOL_GasSwitch = GAS_SWITCH_COLD;
U16 EOL_CompOffDelay = 0;
static void TEST_Sealed(void)
{
    // ë°¸ë¸Œ ? œ?–´
    OpenValve( VALVE_NOS );         
    OpenValve( VALVE_ROOM_COLD_OUT);    
    OpenValve( VALVE_HOT_OUT );     
    OpenValve( VALVE_HOT_DRAIN );     

    CloseValve( VALVE_HOT_IN );      
    CloseValve( VALVE_COLD_IN );     
    CloseValve( VALVE_ROOM_COLD_OUT );    
    CloseValve( VALVE_COLD_AIR );    
    CloseValve( VALVE_COLD_DRAIN );  
    CloseValve( VALVE_COLD_FLUSH );  
    CloseValve( VALVE_ICE_TRAY_IN ); 

    // ?˜¨?ˆ˜ ?œ ?Ÿ‰ ì¡°ì ˆ ë°¸ë¸Œ OPEN
    if( HAL_GetTargetStep( SM_ID_2 ) == 1800 )
    {
        HAL_SetTargetStep( SM_ID_2, 1800 );    // for power on init
    }

    // COMP
    if( EOL_Comp == ON )
    {
        if( EOL_CompOffDelay == 0 )
        {
            SetCompBldcTargetRps( EOL_COMP_RPS );
        }
    }
    else
    {
        SetCompBldcTargetRps( 0U );
    }

    if( EOL_PrevComp != EOL_Comp )
    {
        EOL_PrevComp = EOL_Comp;
        if( EOL_Comp == OFF )
        {
            EOL_CompOffDelay = EOL_COMP_OFF_DELAY_TIME;
        }
    }

    // GAS SWITCH 
    if( EOL_GasSwitch == GAS_SWITCH_COLD )
    {
        GasSwitchCold();
    }
    else if( EOL_GasSwitch == GAS_SWITCH_ICE )
    {
        GasSwitchIce();
    }
    else if( EOL_GasSwitch == GAS_SWITCH_HOTGAS )
    {
        GasSwitchHotGas();
    }

    // DC FAN ON
    TURN_ON_FAN_MOTOR_1();
    
    // TEST ?‹œê°? ë¬´í•œ...
    SetEolTestTimer( DEFAULT_TEST_TIME_VALUE );
}

static void InnerProcessLoad(void)
{
    U8 mu8Mode;

    mu8Mode = GetEolMode();
    switch( mu8Mode )
    {
        ///////////////// TEST SEALED ///////////////////////
        case EOL_MODE_SEALED_INIT:
            StopEolDrainWater();
            TEST_StopFlowValve();
            ModeInit();

            GasSwitchInit();
            SetEolMode( EOL_MODE_SEALED );
            break;

        case EOL_MODE_SEALED:
            TEST_Sealed();
            break;
        ///////////////// TEST SEALED ///////////////////////

        case EOL_MODE_READY:
            SetInputConfDetectTime( IN_FILTER, INPUT_DELAY_100MS );
            SetInputConfDetectTime( IN_TANK_OPEN, INPUT_DELAY_100MS );
            ModeReady();
            SetVoiceVolume( MAX_VOICE_VOLUME );

            // ?ˆ„?ˆ˜ ?„¼?„œ ê°ì?? ë°? ?ƒ±?¬ë¦¬ë“œ ?Š¤?œ„ì¹? ê°ì???˜ë©?,
            // ?•„?„° ? ˆë²? ?…Œ?Š¤?Š¸ ?™?ž‘ ?—¬ë¶?
            // ë¶??•˜ ?…Œ?Š¤?Š¸ë¡? ? „?™˜
            if( (GetEolTestInputVal() & MK_EOL_TANK_COVER ) == MK_EOL_TANK_COVER
                    && ( (GetEolTestInputVal() & MK_EOL_FILTER_ONOFF ) != 0 )
                    && IsDetectLeak() == TRUE )
            {
                SetEolTestMode( EOL_TEST_MODE_2 );
                SetEolMode( EOL_MODE_INIT );
            }

            // ?˜¨?ˆ˜ ì¶”ì¶œ ?…Œ?Š¤?Š¸ ì§„ê³µì§ˆì†Œ...
            // ?œ ?Ÿ‰ ì¡°ì ˆ ë°¸ë¸Œ OPEN/CLOSE ë°˜ë³µ
            TEST_StartFlowValve();
            TEST_RepeatFlowValve();
            CheckSensorFlowMeter();
            break;

        case EOL_MODE_INIT:

            // ?˜¨?ˆ˜ ì¶”ì¶œ ?…Œ?Š¤?Š¸ ì¢…ë£Œ
            TEST_StopFlowValve();

            // ë°°ìˆ˜ ê¸°ëŠ¥?´ ì¼œì ¸ ?žˆ?‹¤ë©? ê°•ì œë¡? OFF
            StopEolDrainWater();

            // ?†µ?‹  ? œ?–´ ???ê¸? 
            ModeInit();

            TestIceTray();
            CheckSensorIceFull();
            CheckSensorBodyUvLed();
            CheckSensorLeak();
            CheckSensorFlowMeter();
            CheckHeaterFeedback();
            break;

        case EOL_MODE_CHECK_SENSOR:
            TestIceTray();
            CheckSensorIceFull();
            CheckSensorBodyUvLed();
            CheckSensorLeak();
            CheckSensorFlowMeter();
            CheckHeaterFeedback();
            break;

        case EOL_MODE_CHECK_LOAD:
            CheckLoadIceTray();
            TEST_RepeatGasValve();
            TEST_RepeatIceDoor();
            TEST_RepeatFlowValve();
            break;

        case EOL_MODE_DONE:
            HAL_TurnOnFanMotor();
            break;

        default:
            break;
    }
}


U8 IsReadyEolInitMode(void)
{
    // ?ˆ„?ˆ˜ ?„¼?„œ ê°ì?? ë°? ?ƒ±?¬ë¦¬ë“œ ?Š¤?œ„ì¹? ê°ì???˜ë©?,
    // ?•„?„° ? ˆë²? ?…Œ?Š¤?Š¸ ?™?ž‘ ?—¬ë¶?
    // ë¶??•˜ ?…Œ?Š¤?Š¸ë¡? ? „?™˜
    if( (GetEolTestInputVal() & MK_EOL_TANK_COVER ) == MK_EOL_TANK_COVER
            && ( (GetEolTestInputVal() & MK_EOL_FILTER_ONOFF ) != 0 )
            && IsDetectLeak() == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

static void UpdateSealdTimer(void)
{
    if( EOL_CompOffDelay != 0 )
    {
        EOL_CompOffDelay--;
    }
}

void ProcessEol(void)
{
    // check expired test time out
    if( GetEolTestTimer() == 0 )
    {
        Reset();
        return ;
    }


    if( GetEolType() == EOL_TYPE_FRONT )
    {
        InnerProcessFront();
    }
    else
    {
        UpdateSealdTimer();
        InnerProcessLoad();
    }
}

