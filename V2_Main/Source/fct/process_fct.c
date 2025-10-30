#include "hw.h"
#include "process_fct.h"
#include "fct.h"

#include "hal_adc.h"
#include "hal_step_motor.h"
#include "hal_pump.h"
#include "hal_led.h"

#include "key.h"
#include "eeprom.h"
#include "rtc.h"
#include "util.h"
#include "comm.h"
#include "timer.h"
#include "display.h"
#include "temp.h"
#include "comp_bldc.h"

#include "valve.h"
#include "relay.h"
#include "service.h"
#include "ice_full.h"
#include "water_out.h"
#include "front.h"
#include "WIFI_Control.h"


static U8 IsExpiredTestTime(void)
{
    if( GetFctTestTimer() != 0 )
    {
        return FALSE;   // going on
    }

    return TRUE;  // timeout..
}


static void InitLoad(void)
{
    U8 mu8Ret;
    U8 mu8Ret_Rtc;

    // Off UART
    //R_UART0_Stop(); // COMP
    //R_UART1_Stop(); // WIFI
    R_UART3_Stop(); // EOL


    R_TAU0_Channel2_Stop(); // HEATER CONTORL TIMER ISR

    // Init Step Motor
    HAL_InitStepVal( SM_ID_0 );
    HAL_InitStepVal( SM_ID_1 );
    HAL_InitStepVal( SM_ID_2 );

    // Turn off Valves
    CloseValve( VALVE_ALL );
    OpenValve( NORMAL_OPEN_VALVES );

    // Turn off Relay
    TurnOffRelayAll();

    TURN_ON_SMPS_12V();
    OPEN_VALVE_POWER_OUT();

    // ?Š¤?… ëª¨í„° ? œ?–´ ë¹„í™œ?„±?™”
    HAL_SetInActive( SM_ID_0, 1 );      // GAS SWITCH
    HAL_SetInActive( SM_ID_1, 1 );      // ICE DOOR
    HAL_SetInActive( SM_ID_2, 1 );      // HOT WATER OUT

    // TEST, EEPROM
    mu8Ret = TestEeprom();
    mu8Ret_Rtc = TestRtc();
    if( mu8Ret == TRUE && mu8Ret_Rtc == TRUE)
    {
        InitRtcTime();
        SetFctTestInputBit( MK_FCT_EEPROM );
    }
    else
    {
        ClearFctTestInputBit( MK_FCT_EEPROM );
    }
}


static void ControlStopGasValve(void)
{
    P_STEP_MOTOR_GAS_SW_1  = 0;
    P_STEP_MOTOR_GAS_SW_2  = 0;
    P_STEP_MOTOR_GAS_SW_3  = 0;
    P_STEP_MOTOR_GAS_SW_4  = 0;
}

static void ControlMoveGasValve(void)
{
    static U8 mu8Count = 0;
    static U8 mu8Step = 0;

    if( mu8Count != 0 )
    {
        mu8Count--;
        return ;
    }
    mu8Count = 100;
    switch( mu8Step++ )
    {
       case 0:
            P_STEP_MOTOR_GAS_SW_1  = 1;
            P_STEP_MOTOR_GAS_SW_2  = 0;
            P_STEP_MOTOR_GAS_SW_3  = 0;
            P_STEP_MOTOR_GAS_SW_4  = 0;
            break;
        case 1:
            P_STEP_MOTOR_GAS_SW_1  = 0;
            P_STEP_MOTOR_GAS_SW_2  = 1;
            P_STEP_MOTOR_GAS_SW_3  = 0;
            P_STEP_MOTOR_GAS_SW_4  = 0;
            break;
        case 2:
            P_STEP_MOTOR_GAS_SW_1  = 0;
            P_STEP_MOTOR_GAS_SW_2  = 0;
            P_STEP_MOTOR_GAS_SW_3  = 1;
            P_STEP_MOTOR_GAS_SW_4  = 0;
            break;
        case 3:
            P_STEP_MOTOR_GAS_SW_1  = 0;
            P_STEP_MOTOR_GAS_SW_2  = 0;
            P_STEP_MOTOR_GAS_SW_3  = 0;
            P_STEP_MOTOR_GAS_SW_4  = 1;
            mu8Step = 0;
            break;
        default:
            mu8Step = 0;
            break;
    }
}


// ?˜¨?„ ë²”ìœ„ ?œ ?š¨?„± ?™•?¸
static U8 IsValidRangeTemp(U8 mu8Id, TEMP_T tMin, TEMP_T tMax)
{
    if( GetTemp( mu8Id ) >= tMin
            && GetTemp( mu8Id ) <= tMax )
    {
        return TRUE;
    }

    return FALSE;
}

static void CheckCommComp(void)
{
    if( GetCompBldcCommTest() == FALSE )
    {
        ClearFctTestInputBit( MK_FCT_COMM_COMP );
    }
    else
    {
        SetFctTestInputBit( MK_FCT_COMM_COMP );
    }
}

static void CheckCommEol(void)
{
    static U8 mu8Step = 0;

    switch( mu8Step )
    {
        case 0:
            PM_EOL_RX = 1;
            PM_EOL_TX = 0;

            ClearFctTestInputBit( MK_FCT_COMM_EOL );
            mu8Step++;

            break;

        case 1:
            P_EOL_TX = 1;
            if( P_EOL_RX == 1 )
            {
                mu8Step++;
            }
            break;

        case 2:
            P_EOL_TX = 0;
            if( P_EOL_RX == 0 )
            {
                mu8Step++;
            }
            break;

        case 3:
            P_EOL_TX = 1;
            if( P_EOL_RX == 1 )
            {
                mu8Step++;
            }
            break;
    
        case 4:
            SetFctTestInputBit( MK_FCT_COMM_EOL );
            break;

        default:
            mu8Step = 0;
            break;
    }
}

// ë²„ì „ ?‘œ?‹œ
static U8 DispVersionOnTime(void)
{
    static U16 mu16Delay = 100;    // 2sec.. @10ms..

    if( mu16Delay != 0)
    {
        mu16Delay--;
        DispVersion( VERSION_MAJOR );
        return FALSE;
    }

    return TRUE;
}

static U8 DispSystemOnTime(void)
{
    static U16 mu16Delay = 100;    // 2sec.. @10ms..

    if( mu16Delay != 0)
    {
        mu16Delay--;
        if( GetSystem() == SYSTEM_CHP )
        {
            DispSegmentChar( SEGMENT_CHAR_ID_CHP  );
        }
        else
        {
            DispSegmentChar( SEGMENT_CHAR_ID_CP  );
        }
        return FALSE;
    }

    return TRUE;
}

void ProcessFctLoadTest(void)
{
    // TURN OFF ALL LEDS
    TurnOffAllLED();

    // DISPLAY VERSION ON LCD 
    if( DispVersionOnTime() == FALSE )
    {
        return ;
    }

    if( DispSystemOnTime() == FALSE )
    {
        return ;
    }


    // ?Š¤?… ëª¨í„° ? œ?–´ ?™œ?„±?™”
    //HAL_SetInActive( SM_ID_0, 0 );      // GAS SWITCH
    HAL_SetInActive( SM_ID_1, 0 );      // ICE DOOR
    HAL_SetInActive( SM_ID_2, 0 );      // HOT WATER OUT

    // WIFI COMM 
    if( GetWifiStatusValue(WIFI_STATUS_TEST) == 1 )
    {
        HAL_TurnOnOffLED( ICON_WIFI,     ON );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_WIFI,     OFF );
    }
    

    // EEPROM 
    if( IsSetFctTestInputVal( MK_FCT_EEPROM ) == TRUE )
    {
        HAL_TurnOnOffLED( ICON_SUN,     ON );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_SUN,     OFF );
    }

    // BLDC COMP COMM.
    CheckCommComp();
    if( IsSetFctTestInputVal( MK_FCT_COMM_COMP ) == TRUE )
    {
        HAL_TurnOnOffLED( ICON_CLOUD,     ON );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_CLOUD,     OFF );
    }

    // EOL COMM.
    CheckCommEol();
    if( IsSetFctTestInputVal( MK_FCT_COMM_EOL ) == TRUE )
    {
        HAL_TurnOnOffLED( ICON_RAIN,     ON );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_RAIN,     OFF );
    }
    
    // ICE FULL
    if( GetIceFullStatus() == TRUE )
    {
        HAL_TurnOnOffLED( ICON_ICE_FULL,   ON );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_ICE_FULL,   OFF );
    }

    /*********************************************************************************
     * KEY 
     */

    // KEY 
    if( IsSetFctTestInputVal( MK_FCT_KEY_SETTING ) == TRUE )
    {
        CloseValve( VALVE_NOS );
        HAL_TurnOnOffLED( SETTING,   ON );

        // Feedback - 24V ( 1.2v ~ 1.8v )
        if( HAL_GetAdcValue( ANI_24V_FB ) >= 60
                && HAL_GetAdcValue( ANI_24V_FB ) <= 120 )
        {
            HAL_TurnOnOffLED( ICON_FLUSHING,   ON );
        }
        else
        {
            HAL_TurnOnOffLED( ICON_FLUSHING,   OFF );
        }
    }
    else
    {
        OpenValve( VALVE_NOS );
        HAL_TurnOnOffLED( SETTING,   OFF );
        HAL_TurnOnOffLED( ICON_FLUSHING,   OFF );
    }

    if( IsSetFctTestInputVal( MK_FCT_KEY_ICE ) == TRUE )
    {
        OpenValve( VALVE_ICE_TRAY_IN );
        if( GetSystem() == SYSTEM_CHP )
        {
            HAL_TurnOnOffLED( ICE_MODE,   ON );
        }
        else
        {
            HAL_TurnOnOffLED( CP_ICE_SIZE,   ON );
        }
    }
    else
    {
        CloseValve( VALVE_ICE_TRAY_IN );
        if( GetSystem() == SYSTEM_CHP )
        {
            HAL_TurnOnOffLED( ICE_MODE,   OFF );
        }
        else
        {
            HAL_TurnOnOffLED( CP_ICE_SIZE,   OFF );
        }
    }


    if( IsSetFctTestInputVal( MK_FCT_KEY_HOT ) == TRUE )
    {
        if( GetSystem() == SYSTEM_CHP )
        {
            OpenValve( VALVE_HOT_OUT );
            HAL_TurnOnOffLED( HOT_WATER,   ON );
        }
        else
        {
            HAL_TurnOnOffLED( CP_ICE_TURBO_2,   ON );
            HAL_TurnOnOffLED( CP_ICE_TURBO,   ON );
        }
    }
    else
    {
        CloseValve( VALVE_HOT_OUT );
        if( GetSystem() == SYSTEM_CHP )
        {
            HAL_TurnOnOffLED( HOT_WATER,   OFF );
        }
        else
        {
            HAL_TurnOnOffLED( CP_ICE_TURBO_2,   OFF );
            HAL_TurnOnOffLED( CP_ICE_TURBO,   OFF );
        }
    }

    if( IsSetFctTestInputVal( MK_FCT_KEY_ROOM ) == TRUE )
    {
        OpenValve( VALVE_ROOM_COLD_OUT );
        HAL_TurnOnOffLED( ROOM_WATER,   ON );
    }
    else
    {
        CloseValve( VALVE_ROOM_COLD_OUT );
        HAL_TurnOnOffLED( ROOM_WATER,   OFF );
    }


    if( IsSetFctTestInputVal( MK_FCT_KEY_COLD ) == TRUE )
    {
        OpenValve( VALVE_ROOM_COLD_OUT);
        HAL_TurnOnOffLED( COLD_WATER,   ON );
    }
    else
    {
        CloseValve( VALVE_ROOM_COLD_OUT );
        HAL_TurnOnOffLED( COLD_WATER,   OFF );
    }


    if( IsSetFctTestInputVal( MK_FCT_KEY_COLD_MAKE ) == TRUE )
    {
        OpenValve( VALVE_COLD_DRAIN );
        if( GetSystem() == SYSTEM_CP )
        {
            OpenValve( VALVE_COLD_FLUSH );
        }
        HAL_TurnOnOffLED( COLD_MAKE,   ON );
    }
    else
    {
        CloseValve( VALVE_COLD_DRAIN );
        CloseValve( VALVE_COLD_FLUSH );
        HAL_TurnOnOffLED( COLD_MAKE,   OFF );
    }

    if( IsSetFctTestInputVal( MK_FCT_KEY_AMOUNT ) == TRUE )
    {
        CloseValve( VALVE_COLD_AIR );
        HAL_TurnOnOffLED( AMOUNT,   ON );
    }
    else
    {
        OpenValve( VALVE_COLD_AIR );
        HAL_TurnOnOffLED( AMOUNT,   OFF );
    }


    {
        static U8 mu8OneShot = FALSE;

        if( IsSetFctTestInputVal( MK_FCT_KEY_SAVE ) == TRUE )
        {
            if( mu8OneShot == FALSE )
            {
                mu8OneShot = TRUE;
                StartTimer( TIMER_ID_COMP, SEC(0) );
            }
            TurnOnRelay( RELAY_COMP );

            HAL_TurnOnOffLED( SAVING,   ON );
        }
        else
        {
            mu8OneShot = FALSE;

            StopTimer( TIMER_ID_COMP );
            TurnOffRelay( RELAY_COMP );

            HAL_TurnOnOffLED( SAVING,   OFF );
        }
    }

    if( IsSetFctTestInputVal( MK_FCT_KEY_HOT_LOCK ) == TRUE )
    {
        TURN_ON_UV_ICE_TANK();
        if( GetSystem() == SYSTEM_CHP )
        {
            HAL_TurnOnOffLED( HOT_LOCK,   ON );
        }
        else
        {
            HAL_TurnOnOffLED( CP_ICE_MAKE,   ON );
        }
    }
    else
    {
        TURN_OFF_UV_ICE_TANK();
        if( GetSystem() == SYSTEM_CHP )
        {
            HAL_TurnOnOffLED( HOT_LOCK,   OFF );
        }
        else
        {
            HAL_TurnOnOffLED( CP_ICE_MAKE,   OFF );
        }
    }

    if( IsSetFctTestInputVal( MK_FCT_KEY_LOCK_ALL ) == TRUE )
    {
        OpenValve( VALVE_COLD_IN );
        HAL_TurnOnOffLED( LOCK_ALL,   ON );
    }
    else
    {
        CloseValve( VALVE_COLD_IN );
        HAL_TurnOnOffLED( LOCK_ALL,   OFF );
    }


    // UV ICE TANK
    if( IsSetFctTestInputVal( MK_FCT_KEY_ICE_MAKE ) == TRUE )
    {
        OpenValve( VALVE_HOT_IN );
        HAL_TurnOnOffLED( ICE_MAKE,   ON );
    }
    else
    {
        CloseValve( VALVE_HOT_IN );
        HAL_TurnOnOffLED( ICE_MAKE,   OFF );
    }

    // UV ICE TRAY
    if( IsSetFctTestInputVal( MK_FCT_KEY_ICE_LOCK ) == TRUE )
    {
        TURN_ON_UV_ICE_TRAY();
        HAL_TurnOnOffLED( ICE_LOCK,   ON );
    }
    else
    {
        TURN_OFF_UV_ICE_TRAY();
        HAL_TurnOnOffLED( ICE_LOCK,   OFF );
    }


    // HEATER LOW 
    {
        // ?žˆ?„° ë³´í˜¸ ?™?ž‘ ( ìµœë?? 3ì´ˆë§Œ ON )
        static U16 mu16OnTime = 300U; // @10ms = 3sec, 

        if( IsSetFctTestInputVal( MK_FCT_KEY_ICE_MAKE ) == TRUE )
        {
            if( mu16OnTime != 0 )
            {
                mu16OnTime--;

                TURN_ON_HEATER_TRIAC();
            }
            else
            {
                TURN_OFF_HEATER_TRIAC();
            }
        }
        else
        {
            mu16OnTime = 300U;

            TURN_OFF_HEATER_TRIAC();
        }
    }


    // HETAER HIGH
    if( GetSystem() == SYSTEM_CHP )
    {
        // ?žˆ?„° ë³´í˜¸ ?™?ž‘ ( ìµœë?? 3ì´ˆë§Œ ON )
        static U16 mu16OnTime = 300U; // @10ms = 3sec, 

        if( IsSetFctTestInputVal( MK_FCT_KEY_HOT ) == TRUE )
        {
            if( mu16OnTime != 0 )
            {
                mu16OnTime--;

                TURN_ON_HEATER_RELAY();
                HAL_TurnOnOffLED( ICE_TURBO,   ON );
            }
            else
            {
                TURN_OFF_HEATER_RELAY();
                HAL_TurnOnOffLED( ICE_TURBO,   OFF );
            }
        }
        else
        {
            mu16OnTime = 300U;

            TURN_OFF_HEATER_RELAY();
            HAL_TurnOnOffLED( ICE_TURBO,   OFF );
        }
    }
    else
    {
            TURN_OFF_HEATER_RELAY();
    }

    // COMP SUPPLY AC POWER
    if( IsSetFctTestInputVal( MK_FCT_LEVER_WATER ) == TRUE )
    {
        HAL_TurnOnOffLED( WATER_OUT_2,   ON );
        HAL_TurnOnOffLED( WATER_OUT_1,   ON );
    }
    else
    {
        HAL_TurnOnOffLED( WATER_OUT_2,   OFF );
        HAL_TurnOnOffLED( WATER_OUT_1,   OFF );
    }

    if( IsSetFctTestInputVal( MK_FCT_LEVER_ICE ) == TRUE )
    {
        if( GetSystem() == SYSTEM_CHP )
        {
            CloseValve( VALVE_HOT_DRAIN );  // on
        }
        HAL_TurnOnOffLED( ICE_OUT_2,   ON );
        HAL_TurnOnOffLED( ICE_OUT_1,   ON );
    }
    else
    {
        OpenValve( VALVE_HOT_DRAIN );   // off
        HAL_TurnOnOffLED( ICE_OUT_2,   OFF );
        HAL_TurnOnOffLED( ICE_OUT_1,   OFF );
    }

    /*********************************************************
     * INPUT
     */

    {
        static U8 mu8Off = TRUE;

        // ICE TRAY MS-1  ( ICE DOOR )
        if( GET_MICRO_SW_ICE_TRAY_1() != 0 )
        {
            mu8Off = TRUE;

            if( HAL_GetCurrentStep( SM_ID_1 ) == 1000 )
            {
                HAL_SetTargetStep( SM_ID_1, 0 );
            }
            else if( HAL_GetCurrentStep( SM_ID_1 ) == 0 )
            {
                HAL_SetTargetStep( SM_ID_1, 1000 );
            }
        }
        else
        {
            if( mu8Off == TRUE )
            {
                mu8Off = FALSE;
                HAL_StopMove( SM_ID_1 );
                HAL_InitStepVal( SM_ID_1 );
            }
        }
    }


    // ICE TRAY MS-2  ( GAS SWITCH )
    if( GET_MICRO_SW_ICE_TRAY_2() != 0 )
    {
        ControlMoveGasValve();

        // Feedback - 12V  ( 1.2v ~ 1.8v )
        if( HAL_GetAdcValue( ANI_12V_FB ) >= 60 
                && HAL_GetAdcValue( ANI_12V_FB ) <= 120 )
        {
            HAL_TurnOnOffLED( ICON_DRAIN,   ON );
        }
        //else
        //{
        //    HAL_TurnOnOffLED( ICON_DRAIN,   OFF );
        //}

    }
    else
    {
        ControlStopGasValve();
        HAL_TurnOnOffLED( ICON_DRAIN,   OFF );
    }

    // LEVER WATER  ( HOT WATER )
    {
        static U8 mu8Off = TRUE;


        if( IsSetFctTestInputVal( MK_FCT_LEVER_WATER ) == TRUE )
        {
            mu8Off = TRUE;

            if( GetSystem() == SYSTEM_CHP )
            {
                if( HAL_GetCurrentStep( SM_ID_2 ) == 1000 )
                {
                    HAL_SetTargetStep( SM_ID_2, 0 );
                }
                else if( HAL_GetCurrentStep( SM_ID_2 ) == 0 )
                {
                    HAL_SetTargetStep( SM_ID_2, 1000 );
                }
            }
        }
        else
        {
            if( mu8Off == TRUE )
            {
                mu8Off = FALSE;
                HAL_StopMove( SM_ID_2 );
                HAL_InitStepVal( SM_ID_2 );
            }
        }
    }

    // HEATER INPUT - DRAIN PUMP
    if( GET_STATUS_MODEL_INPUT() == HIGH )
    {
        HAL_TurnOnOffLED( ICON_UV_SMART_CARE,   ON );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_UV_SMART_CARE,   OFF );
    }

    // DRAIN HIGH, LOW 
    if( GET_DRAIN_TANK_HIGH() == HIGH )
    {
        TurnOnRelay( RELAY_ICE_TRAY_CW );
        TurnOffRelay( RELAY_ICE_TRAY_CCW );
    }
    else if( GET_DRAIN_TANK_LOW() == HIGH )
    {
        TurnOffRelay( RELAY_ICE_TRAY_CW );
        TurnOnRelay( RELAY_ICE_TRAY_CCW );
    }
    else
    {
        TurnOffRelay( RELAY_ICE_TRAY_CW );
        TurnOffRelay( RELAY_ICE_TRAY_CCW );
    }

    // TANK REED(SERVICE) SW, FILTER DETECT 
    if( GET_REED_SW_TANK_OPEN() == HIGH )
    {
        TurnOnRelay( RELAY_FEEDER_CCW );
        TurnOffRelay( RELAY_FEEDER_CW );
    }
    else if( GET_STATUS_FILTER_DETECT() == HIGH )
    {
        TurnOffRelay( RELAY_FEEDER_CCW );
        TurnOnRelay( RELAY_FEEDER_CW );
    }
    else
    {
        TurnOffRelay( RELAY_FEEDER_CW );
        TurnOffRelay( RELAY_FEEDER_CCW );
    }

    // TANK REED(SERVICE) SW-2
    if( GET_REED_SW_TANK_OPEN_2() == HIGH )
    {
        TURN_ON_FAN_MOTOR_1();
    }
    else
    {
        TURN_OFF_FAN_MOTOR_1();
    }


    // COLD TEMPERTURE
    DispSegmentError( 0xFF );
    if( IsValidRangeTemp( TEMP_ID_COLD_WATER, 22.0f, 28.0f ) == FALSE )
    {
        DispSegmentError( 1 );
    }

    // TEMP AMBIENT
    if( IsValidRangeTemp( TEMP_ID_AMBIENT, 22.0f, 28.0f ) == FALSE )
    {
        DispSegmentError( 2 );
    }

    // TEMP ROOM 
    if( IsValidRangeTemp( TEMP_ID_ROOM_WATER, 22.0f, 28.0f ) == FALSE )
    {
        DispSegmentError( 3 );
    }

    // TEMP EVA
    if( IsValidRangeTemp( TEMP_ID_EVA_1, 22.0f, 28.0f ) == FALSE )
    {
        DispSegmentError( 4 );
    }

    if( GetSystem() == SYSTEM_CHP )
    {
        // TEMP HOT IN 
        if( IsValidRangeTemp( TEMP_ID_HOT_IN, 22.0f, 28.0f ) == FALSE )
        {
            DispSegmentError( 5 );
        }

        // TEMP HOT OUT
        if( IsValidRangeTemp( TEMP_ID_HOT_OUT, 22.0f, 28.0f ) == FALSE )
        {
            DispSegmentError( 6 );
        }

        // TEMP HEATER
        if( IsValidRangeTemp( TEMP_ID_HEATER, 22.0f, 28.0f ) == FALSE )
        {
            DispSegmentError( 7 );
        }
    }

    // Leak ( 1.2v ~ 1.8v )
    TURN_ON_LEAK_ONOFF();
    if( HAL_GetAdcValue( ANI_SENSOR_LEAK ) >= 800 )
    {
        TURN_ON_SWING_BAR();
    }
    else
    {
        TURN_OFF_SWING_BAR();
    }

    // FLOW SENSOR INPUT
    if( GET_FLOW_SENSOR() == HIGH )
    {
        HAL_TurnOnOffLED( ICON_FILTER,   ON );
        HAL_TurnOnPump( HAL_PUMP_DRAIN, 24.0f );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_FILTER,   OFF );
        HAL_TurnOffPump( HAL_PUMP_DRAIN );
    }


    // Feedback - DrainPump ( 1.2v ~ 1.8v )
    if( HAL_GetAdcValue( ANI_PUMP_DRAIN_FB ) >= 250 
            && HAL_GetAdcValue( ANI_PUMP_DRAIN_FB ) <= 370 )
    {
        HAL_TurnOnOffLED( ICON_SAVING,   ON );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_SAVING,   OFF );
    }

    // Feedback - Uv ice out
    if( HAL_GetAdcValue( ANI_UV_ICE_DOOR ) >= 10 
            && HAL_GetAdcValue( ANI_UV_ICE_DOOR ) <= 60 )
    {
        HAL_TurnOnOffLED( ICON_UV_ICE,   ON );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_UV_ICE,   OFF );
    }


    // Feedback - Uv water out
    if( HAL_GetAdcValue( ANI_UV_WATER_OUT ) >= 10 
            && HAL_GetAdcValue( ANI_UV_WATER_OUT ) <= 60 )
    {
        HAL_TurnOnOffLED( ICON_UV_WATER,   ON );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_UV_WATER,   OFF );
    }

    // Feedback - Uv ICE TRAY
    if( HAL_GetAdcValue( ANI_UV_ICE_TRAY ) >= 10 
            && HAL_GetAdcValue( ANI_UV_ICE_TRAY ) <= 60 )
    {
        HAL_TurnOnOffLED( ICON_UV_ICE_TRAY,   ON );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_UV_ICE_TRAY,   OFF );
    }

    // Feedback - Uv ICE TANK
    if( HAL_GetAdcValue( ANI_UV_ICE_TANK ) >= 10 
            && HAL_GetAdcValue( ANI_UV_ICE_TANK ) <= 60 )
    {
        HAL_TurnOnOffLED( ICON_UV_ICE_TANK,   ON );
    }
    else
    {
        HAL_TurnOnOffLED( ICON_UV_ICE_TANK,   OFF );
    }





    // Feedback - heater 
    if( GetSystem() == SYSTEM_CHP )
    {
        if( HAL_GetAdcValue( ANI_HEATER_FB ) >= 30 
                && HAL_GetAdcValue( ANI_HEATER_FB ) <= 80 )
        {
            HAL_TurnOnOffLED( AMOUNT_120,   ON );
            HAL_TurnOnOffLED( AMOUNT_250,   ON );
            HAL_TurnOnOffLED( AMOUNT_500,   ON );
        }
        else
        {
            HAL_TurnOnOffLED( AMOUNT_120,   OFF );
            HAL_TurnOnOffLED( AMOUNT_250,   OFF );
            HAL_TurnOnOffLED( AMOUNT_500,   OFF );
        }
    }
}


void ProcessFct(void)
{
    static U8 mu8Init = 0;

    // check expired test time out
    if( IsExpiredTestTime() == TRUE )
    {
        Reset();
        return ;
    }

    if( mu8Init == 0 )
    {
        mu8Init = 1;
        InitLoad();
    }

    StartCheckIceFull();
}

