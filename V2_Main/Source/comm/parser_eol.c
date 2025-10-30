/***********************************************************************************************
 * INCLUDE 
 */

#include "hw.h"
#include "parser_eol.h"
#include "parser_debug.h"
#include "util.h"
#include "eol.h"
#include "process_eol.h"
#include "timer.h"

#include "temp.h"
#include "err_temp.h"
#include "level.h"
#include "relay.h"
#include "valve.h"
#include "cold_water.h"
#include "comp.h"
#include "ice.h"
#include "ice_full.h"
#include "ice_maker.h"
#include "ice_door.h"
#include "ice_tray.h"
#include "drain_pump.h"
#include "hal_adc.h"
#include "hal_pump.h"
#include "hal_step_motor.h"
#include "hal_fan_motor.h"
#include "hal_uv.h"
#include "gas_switch_valve.h"
#include "power_saving.h"
#include "error.h"
#include "leak.h"
#include "rtc.h"
#include "filter.h"
#include "front.h"
#include "flow_valve.h"

#include "service.h"
#include "WIFI_Control.h"

#include <stdio.h>
#include <stdlib.h>

/***********************************************************************************************
 * DEFINITION 
 */
#define  PKT_STX     0x01
#define  PKT_ETX     0x04

#define  PKT_ACK     0x06
#define  PKT_NAK     0x15

#define FIELD_STX               0
#define FIELD_PACKET_TYPE       1
#define FIELD_PAYLOAD           5
#define FIELD_CHECKSUM          7
#define FIELD_ETX               9

#define LENGTH_PACKET_TYPE      4
#define	MIN_PKT_SZ  6	    /* STX + TYPE + ETX + CRC(2byes) */


static U8	check_crc( U8 *buf, I16 len )
{
    U8 i = 0;
    U8 mu8Chksum = 0;

    for( i = 0; i < len - 3 ; i++ )
    {
        mu8Chksum ^= buf[ i ];
    }

    return mu8Chksum;
}

static U8 IsTrueChksum( U8 *buf, I16 len )
{
    U8 mu8Cal;
    U8 mu8Val;


    mu8Cal = check_crc( buf, len );
    mu8Val = ConvAsc2Byte( buf[ len - 3 ], buf[ len - 2 ] );
    if( mu8Cal != mu8Val )
    {
        return FALSE;
    }

    return TRUE;
}

// Return Packet type assic to hex
static U16 GetPacketType(U8 *buf, U8 len)
{
    U8 mu8High = 0;
    U8 mu8Low = 0;
    U16 mu16PktType;

    // Skip DEBUG PACKET TYPE
    mu8High = ConvAsc2Byte( buf[0], buf[1] );
    mu8Low  = ConvAsc2Byte( buf[2], buf[3] );
    mu16PktType = GET_UINT_WORD( mu8Low, mu8High );

    return mu16PktType;
}

static U8 IsDebugMsssage(U8 *buf)
{
    U16 mu16PktType = 0;

    // Skip DEBUG PACKET TYPE
    mu16PktType = GetPacketType( &buf[ FIELD_PACKET_TYPE ], LENGTH_PACKET_TYPE );
    if( mu16PktType >= PKT_EOL_DEBUG_CONTROL 
            && mu16PktType <= PKT_EOL_DEBUG_6 )
    {
        return TRUE;    // DEBUG MESSAGE
    }
    else
    {
        return FALSE;   // EOL MESSAGE 
    }
}

static U8 IsStartMessage(U8 *buf)
{
    U16 mu16PktType = 0;

    // Skip DEBUG PACKET TYPE
    mu16PktType = GetPacketType( &buf[ FIELD_PACKET_TYPE ], LENGTH_PACKET_TYPE );
    if( mu16PktType == PKT_EOL_MODEL )
    {
        return TRUE;    // CHECK MESSAGE
    }
    else
    {
        return FALSE;   // OTHERS MESSAGE
    }
}

// ?àò?ã† Î∞õÏ?? ?å®?Ç∑?ùò ?ú†?ö®?Ñ± Í≤??Ç¨
I16 IsValidPkt_EOL( U8 *buf, I16 len )
{
    if( buf == NULL )
    {
        return FALSE;
    }

    if( len < MIN_PKT_SZ )
    {
        return FALSE;
    }

    if( buf[ FIELD_STX ] != PKT_STX 
            || buf[ len - 1] != PKT_ETX )
    {
        return FALSE;
    }

    if( IsTrueChksum( buf, len ) == FALSE )
    {
        return FALSE;
    }

    // ?îîÎ≤ÑÍπÖ Î©îÏãúÏß??ù∏ Í≤ΩÏö∞ ?†ï?ÉÅ Ï≤òÎ¶¨...
    if( IsDebugMsssage( buf ) == TRUE )
    {
        return TRUE;    // SKIP DEBUG MESSAGE 
    }

    // Í≤??Ç¨ ÏßÑÏûÖ Ïª§Îß®?ìú?äî ?ûÖ?†• ?óà?ö©
    if( GetEolStatus() == FALSE  )
    {
        // ?ã®, Í≤??Ç¨ ÏßÑÏûÖ ?ãúÍ∞? Ï¥àÍ≥º?êòÎ©? SKIP
        if( IsExpiredEolLimitTimer() == TRUE )
        {
            return FALSE;
        }

        if( IsStartMessage( buf ) == TRUE )
        {
            return TRUE;
        }

        return FALSE;
    }
    else
    {
        // Í≤??Ç¨ Ï¢ÖÎ£å ?õÑ?óê?äî ?Üµ?ã† Î©îÏãúÏß?Î•? ?àò?ã† Î∞õÏ?? ?ïä?äî?ã§.
        if( GetEolMode() == EOL_MODE_SEALED 
                || GetEolMode() == EOL_MODE_DONE )
        {
            return FALSE;
        }

        // ?Öå?ä§?ä∏ Î™®Îìú 2?ã®Í≥ÑÍ?? ?ïÑ?ãå ?ÉÅ?Éú?óê?Ñú
        // PKT_EOL_MODEL ?†ïÎ≥¥Í?? ?ïÑ?ãåÍ≤? ?ûÖ?†•?êòÎ©? drop?ïú?ã§..
        if( GetEolTestMode() != EOL_TEST_MODE_2 )
        {
            if( IsStartMessage( buf ) == FALSE )
            {
                return FALSE;
            }
        }

        //if( GetEolMode() == EOL_MODE_READY 
        //        && GetEolTestMode() == EOL_TEST_MODE_2 )
        //{
        //    if( IsStartMessage( buf ) == TRUE )
        //    {
        //        return TRUE;
        //    }
        //}
    }


#if 0
    // Í≤??Ç¨ ÏßÑÏûÖ Ïª§Îß®?ìú?äî ?ûÖ?†• ?óà?ö©
    if( GetEolStatus() == FALSE  
            && GetEolMode() == EOL_MODE_READY 
      )
    {
        // ?ã®, Í≤??Ç¨ ÏßÑÏûÖ ?ãúÍ∞? Ï¥àÍ≥º?êòÎ©? SKIP
        if( IsExpiredEolLimitTimer() == TRUE )
        {
            return FALSE;
        }

        if( IsStartMessage( buf ) == TRUE )
        {
            return TRUE;
        }
    }

    // Í≤??Ç¨ Î™®Îìú ÏßÑÏûÖ ?õÑ
    // ?òê?äî Í≤??Ç¨ Ï¢ÖÎ£å ?õÑ?óê?äî ?Üµ?ã† Î©îÏãúÏß?Î•? ?àò?ã† Î∞õÏ?? ?ïä?äî?ã§.
    if( GetEolMode() == EOL_MODE_SEALED_INIT 
            || GetEolMode() == EOL_MODE_SEALED 
            || GetEolMode() == EOL_MODE_READY 
            || GetEolMode() == EOL_MODE_DONE )
    {
        // ?ã®, ?îîÎ≤ÑÍπÖ Î©îÏãúÏß??äî ?òà?ô∏Î°? ?àò?ã†Î∞õÎäî?ã§.
        if( IsDebugMsssage( buf ) == TRUE )
        {
            return TRUE;    // SKIP DEBUG MESSAGE 
        }
        else
        {

            return FALSE;
        }
    }
#endif

    return TRUE;
}


typedef	void (*ActionParser_t)(U8 *buf, I16 len);
typedef struct _parser_list_t
{
    U16 Type;
    ActionParser_t ParserPkt;
} parser_list_t;

static void ParserStart(U8 *buf, I16 len);
static void ParserSensor(U8 *buf, I16 len);
static void ParserNoLoad(U8 *buf, I16 len);
//static void ParserHeaterOn(U8 *buf, I16 len);
//static void ParserHeaterOff(U8 *buf, I16 len);
static void ParserColdInValveOn(U8 *buf, I16 len);
static void ParserColdInValveOff(U8 *buf, I16 len);
static void ParserHotInValveOn(U8 *buf, I16 len);
static void ParserHotInValveOff(U8 *buf, I16 len);
static void ParserRoomOutValveOn(U8 *buf, I16 len);
static void ParserRoomOutValveOff(U8 *buf, I16 len);
static void ParserColdOutValveOn(U8 *buf, I16 len);
static void ParserColdOutValveOff(U8 *buf, I16 len);
static void ParserHotOutValveOn(U8 *buf, I16 len);
static void ParserHotOutValveOff(U8 *buf, I16 len);
static void ParserNosValveOn(U8 *buf, I16 len);
static void ParserNosValveOff(U8 *buf, I16 len);
static void ParserHotDrainValveOn(U8 *buf, I16 len);
static void ParserHotDrainValveOff(U8 *buf, I16 len);
static void ParserIceTrayValveOn(U8 *buf, I16 len);
static void ParserIceTrayValveOff(U8 *buf, I16 len);
static void ParserDrainValveOn(U8 *buf, I16 len);
static void ParserDrainValveOff(U8 *buf, I16 len);
static void ParserColdOvfValveOn(U8 *buf, I16 len);
static void ParserColdOvfValveOff(U8 *buf, I16 len);
static void ParserColdFlushValveOn(U8 *buf, I16 len);
static void ParserColdFlushValveOff(U8 *buf, I16 len);
static void ParserFanMotoreOn(U8 *buf, I16 len);
static void ParserFanMotorOff(U8 *buf, I16 len);
static void ParserGasSwitchOn(U8 *buf, I16 len);
static void ParserGasSwitchOff(U8 *buf, I16 len);
static void ParserIceTrayMotorOn(U8 *buf, I16 len);
static void ParserIceTrayMotorOff(U8 *buf, I16 len);
static void ParserIceDoorMotorOn(U8 *buf, I16 len);
static void ParserIceDoorMotorOff(U8 *buf, I16 len);
static void ParserFeederMotorOn(U8 *buf, I16 len);
static void ParserFeederMotorOff(U8 *buf, I16 len);
static void ParserSwingBarOn(U8 *buf, I16 len);
static void ParserSwingBarOff(U8 *buf, I16 len);
static void ParserHotFlowValveOn(U8 *buf, I16 len);
static void ParserHotFlowValveOff(U8 *buf, I16 len);
static void ParserDrainPumpOn(U8 *buf, I16 len);
static void ParserDrainPumpOff(U8 *buf, I16 len);
static void ParserCompOn(U8 *buf, I16 len);
static void ParserCompOff(U8 *buf, I16 len);
static void ParserIceSystemCold(U8 *buf, I16 len);
static void ParserIceSystemIce(U8 *buf, I16 len);
static void ParserIceSystemDeicing(U8 *buf, I16 len);
static void ParserComplete(U8 *buf, I16 len);

const static parser_list_t parser_list[] = 
{

    /* CHECK COLD/ICE SYSTEM */
    { PKT_EOL_COMP_ON,                  ParserCompOn },            
    { PKT_EOL_COMP_OFF,                 ParserCompOff },           
    { PKT_EOL_ICE_SYSTEM_COLD,          ParserIceSystemCold },    
    { PKT_EOL_ICE_SYSTEM_ICE,           ParserIceSystemIce },     
    { PKT_EOL_ICE_SYSTEM_DEICING,       ParserIceSystemDeicing }, 

    { PKT_EOL_COMPLETE,                 ParserComplete },
    { PKT_EOL_MODEL,                    ParserStart },
    { PKT_EOL_SETTING,                  ParserSensor },             

    /* ETC */
    { PKT_EOL_WIFI,                     NULL }, 
    { PKT_EOL_UV_1_ICE,                 NULL }, 
    { PKT_EOL_FLOW_SESNOR,              NULL }, 
    { PKT_EOL_AC_DC,                    NULL }, 
    { PKT_EOL_UV_2_WATER,               NULL }, 
//    { PKT_EOL_SAVING_PHOTO,             NULL }, 
    { PKT_EOL_UV_3_ICE_TRAY,            NULL }, 
    { PKT_EOL_UV_4_ICE_TANK,            NULL }, 
    { PKT_EOL_ICE_FULL,                 NULL }, 

    /* SENSOR */
    { PKT_EOL_SENSOR_TEMP,              NULL }, 
    { PKT_EOL_SENSOR_LEVEL,             NULL }, 
    { PKT_EOL_SENSOR,                   NULL }, 
    { PKT_EOL_SENSOR_HEATER,            NULL }, 

    /* LOAD */
    { PKT_EOL_NO_LOAD,                  ParserNoLoad },             

    //{ PKT_EOL_HEATER_ON,                ParserHeaterOn },    
    //{ PKT_EOL_HEATER_OFF,               ParserHeaterOff },   

    { PKT_EOL_COLD_IN_VALVE_ON,         ParserColdInValveOn },    
    { PKT_EOL_COLD_IN_VALVE_OFF,        ParserColdInValveOff },   

    { PKT_EOL_HOT_IN_VALVE_ON,          ParserHotInValveOn },     
    { PKT_EOL_HOT_IN_VALVE_OFF,         ParserHotInValveOff },    

    { PKT_EOL_ROOM_OUT_VALVE_ON,        ParserRoomOutValveOn },   
    { PKT_EOL_ROOM_OUT_VALVE_OFF,       ParserRoomOutValveOff },  

    { PKT_EOL_COLD_OUT_VALVE_ON,        ParserColdOutValveOn },   
    { PKT_EOL_COLD_OUT_VALVE_OFF,       ParserColdOutValveOff },  

    { PKT_EOL_HOT_OUT_VALVE_ON,         ParserHotOutValveOn },    
    { PKT_EOL_HOT_OUT_VALVE_OFF,        ParserHotOutValveOff },   

    { PKT_EOL_NOS_VALVE_ON,             ParserNosValveOn },        
    { PKT_EOL_NOS_VALVE_OFF,            ParserNosValveOff },       

    { PKT_EOL_HOT_DRAIN_VALVE_ON,       ParserHotDrainValveOn },  
    { PKT_EOL_HOT_DRAIN_VALVE_OFF,      ParserHotDrainValveOff }, 

    { PKT_EOL_ICE_TRAY_VALVE_ON,        ParserIceTrayValveOn },   
    { PKT_EOL_ICE_TRAY_VALVE_OFF,       ParserIceTrayValveOff },  

    { PKT_EOL_DRAIN_VALVE_ON,           ParserDrainValveOn },      
    { PKT_EOL_DRAIN_VALVE_OFF,          ParserDrainValveOff },     

    { PKT_EOL_COLD_OVF_VALVE_ON,        ParserColdOvfValveOn },   
    { PKT_EOL_COLD_OVF_VALVE_OFF,       ParserColdOvfValveOff },  

    { PKT_EOL_COLD_FLUSH_VALVE_ON,       ParserColdFlushValveOn },
    { PKT_EOL_COLD_FLUSH_VALVE_OFF,      ParserColdFlushValveOff},

    { PKT_EOL_FAN_MOTOR_ON,             ParserFanMotoreOn },        
    { PKT_EOL_FAN_MOTOR_OFF,            ParserFanMotorOff },       

    { PKT_EOL_GAS_SWITCH_ON,            ParserGasSwitchOn },       
    { PKT_EOL_GAS_SWITCH_OFF,           ParserGasSwitchOff },      

    { PKT_EOL_ICE_TRAY_MOTOR_ON,        ParserIceTrayMotorOn },   
    { PKT_EOL_ICE_TRAY_MOTOR_OFF,       ParserIceTrayMotorOff },  

    { PKT_EOL_ICE_DOOR_MOTOR_ON,        ParserIceDoorMotorOn },   
    { PKT_EOL_ICE_DOOR_MOTOR_OFF,       ParserIceDoorMotorOff },  

    { PKT_EOL_ICE_FEEDER_MOTOR_ON,      ParserFeederMotorOn }, 
    { PKT_EOL_ICE_FEEDER_MOTOR_OFF,     ParserFeederMotorOff },

    { PKT_EOL_SWING_BAR_ON,             ParserSwingBarOn },       
    { PKT_EOL_SWING_BAR_OFF,            ParserSwingBarOff },      

    { PKT_EOL_HOT_FLOW_VALVE_ON,        ParserHotFlowValveOn },  
    { PKT_EOL_HOT_FLOW_VALVE_OFF,       ParserHotFlowValveOff }, 

    { PKT_EOL_DRAIN_PUMP_ON,            ParserDrainPumpOn },      
    { PKT_EOL_DRAIN_PUMP_OFF,           ParserDrainPumpOff },     


    /* DEBUG MESSAGE */
    { PKT_EOL_DEBUG_CONTROL,         ParserDebugControl },
    { PKT_EOL_DEBUG_1,               NULL },
    { PKT_EOL_DEBUG_2,               NULL },
    { PKT_EOL_DEBUG_3,               NULL },
    { PKT_EOL_DEBUG_4,               NULL },
    { PKT_EOL_DEBUG_5,               NULL },
    { PKT_EOL_DEBUG_6,               NULL },
    //{ PKT_EOL_DEBUG_7,               NULL },
};

#define SZ_PARSER_TABLE (sizeof( parser_list ) / sizeof( parser_list_t ))

// ?àò?ã† Î∞õÏ?? ?å®?Ç∑ ?åå?ã±
I16 ParserPkt_EOL( U8 *buf, I16 len)
{
    U8 mu8Index = 0;
    U16 mu16PktType = 0;
    ActionParser_t	pParser = NULL;


    // Get Packet
    mu16PktType = GetPacketType( &buf[FIELD_PACKET_TYPE], LENGTH_PACKET_TYPE );
    for( mu8Index = 0; mu8Index < SZ_PARSER_TABLE; mu8Index++ )
    {
        if( mu16PktType == parser_list[ mu8Index ].Type )
        {
            pParser = parser_list[ mu8Index ].ParserPkt;
            if( pParser != NULL )
            {
                pParser(&buf[FIELD_PAYLOAD], len);
            }

            // ACK 
            SetCommHeader( COMM_ID_EOL, mu16PktType );
            StartTimer( TIMER_ID_COMM_EOL_TX, 0 );
            break;
        }
    }

    return 0;
}


//extern U8 RunCheckHeater;
extern U8 RunCheckIceTray;
static void TurnOffAllLoad(void)
{
    // step ice door 
    TEST_StopIceDoor();
    TEST_StopFlowValve();
    TEST_StopGasValve();

    // stop ice tray
    RunCheckIceTray = FALSE;
    //RunCheckHeater  = FALSE;


    // Turn off Valves ( Exccept Normal Open valves )
    CloseValve( VALVE_ALL );
    // Turn on normal open valves
    OpenValve( NORMAL_OPEN_VALVES );

    // Turn off Relay
    TurnOffRelayAll();

    // HOT WATER HEATER
    TURN_OFF_HEATER_RELAY();
    TURN_OFF_HEATER_TRIAC();

    // Turn Off Drain Pump
    TurnOffDrainPump();
    HAL_TurnOffPump( HAL_PUMP_DRAIN );

    // Turn off Swing Bar
    TURN_OFF_SWING_BAR();

    // Turn Off UV LED
    HAL_TurnOffUv( HAL_UV_ICE_TANK );
    HAL_TurnOffUv( HAL_UV_ICE_TRAY );
    HAL_TurnOffUv( HAL_UV_ICE_DOOR );
    HAL_TurnOffUv( HAL_UV_WATER_OUT );

    // Turn off Dc FAN
    HAL_TurnOffFanMotor();
    SetCompBldcTargetRps( 0 );
}

#if 0
#define EOL_LOAD_MODE_1 0x01        // Í≤??Ç¨ Î™®Îìú ÏßÑÏûÖ 
#define EOL_LOAD_MODE_2 0x02        // ?ûê?èô?ôî Í≤??Ç¨ ?ãú?ûë
static void ParserStart(U8 *buf, I16 len)
{
    U8 mu8TestMode;


    // conv assic -> decimal
    mu8TestMode = ConvAsc2Byte( buf[0], buf[1] );

    // ?ûòÎ™ªÎêú Î™®ÎìúÍ∞? ?Ñò?ñ¥Î™®Î?? SKIP
    if(  mu8TestMode != EOL_LOAD_MODE_1 
            && mu8TestMode != EOL_LOAD_MODE_2 )
    {
        return ;
    }

    // Î™®Îìú 2?äî ?ù¥?†Ñ Î™®ÎìúÍ∞? 1?ùº?ïå?óêÎß? ÏßÑÏûÖ Í∞??ä•?ïò?ã§.
    if( mu8TestMode == EOL_LOAD_MODE_2 
            && GetEolTestMode() != EOL_LOAD_MODE_1 )
    
    {
        return ;
    }


    StartEolLoad();

    // ?ãú?ûë ?ã§?å® ?ñà?úºÎ©?... Skip..
    if( GetEolStatus() == FALSE )
    {
        return ;
    }


    if( mu8TestMode == EOL_LOAD_MODE_1 )
    {
        // Î∞∏Î∏å Ï¥àÍ∏∞?ôî..
        GasSwitchInit();

        SetEolTestMode( mu8TestMode );
        SetEolMode( EOL_MODE_READY );
    }
    else if( GetEolTestMode() == EOL_LOAD_MODE_1 
            && mu8TestMode == EOL_LOAD_MODE_2 )
    {
        if( IsReadyEolInitMode() == TRUE )
        {
            SetEolTestMode( mu8TestMode );
            SetEolMode( EOL_MODE_INIT );
        }
    }
    else
    {
        return ;
    }
}
#endif

static void ParserStart(U8 *buf, I16 len)
{
    // Î™®Îìú 2?äî ?ù¥?†Ñ Î™®ÎìúÍ∞? 1?ùº?ïå?óêÎß? ÏßÑÏûÖ Í∞??ä•?ïò?ã§.
    StartEolLoad();

    // ?ãú?ûë ?ã§?å® ?ñà?úºÎ©?... Skip..
    if( GetEolStatus() == FALSE )
    {
        return ;
    }

    if( GetEolTestMode() == EOL_TEST_MODE_0 )
    {
        // Î∞∏Î∏å Ï¥àÍ∏∞?ôî..
        GasSwitchInit();

        SetEolTestMode( EOL_TEST_MODE_1 );
        SetEolMode( EOL_MODE_READY );
    }
}

static void ParserSensor(U8 *buf, I16 len)
{
    SetEolMode( EOL_MODE_CHECK_SENSOR );
}

static void ParserNoLoad(U8 *buf, I16 len)
{
    SetEolMode( EOL_MODE_CHECK_LOAD );

    TurnOffAllLoad();
}

//static void ParserHeaterOn(U8 *buf, I16 len)
//{
//    RunCheckHeater = TRUE;
//}
//
//static void ParserHeaterOff(U8 *buf, I16 len)
//{
//    RunCheckHeater = FALSE;
//}

static void ParserColdInValveOn(U8 *buf, I16 len)
{
    OpenValve( VALVE_COLD_IN );
}

static void ParserColdInValveOff(U8 *buf, I16 len)
{
    CloseValve( VALVE_COLD_IN );
}

static void ParserHotInValveOn(U8 *buf, I16 len)
{
    OpenValve( VALVE_HOT_IN );
}

static void ParserHotInValveOff(U8 *buf, I16 len)
{
    CloseValve( VALVE_HOT_IN );
}

static void ParserRoomOutValveOn(U8 *buf, I16 len)
{
    OpenValve( VALVE_ROOM_COLD_OUT );
}

static void ParserRoomOutValveOff(U8 *buf, I16 len)
{
    CloseValve( VALVE_ROOM_COLD_OUT );
}

static void ParserColdOutValveOn(U8 *buf, I16 len)
{
    OpenValve( VALVE_ROOM_COLD_OUT);
}

static void ParserColdOutValveOff(U8 *buf, I16 len)
{
    CloseValve( VALVE_ROOM_COLD_OUT );
}

static void ParserHotOutValveOn(U8 *buf, I16 len)
{
    OpenValve( VALVE_HOT_OUT );
}

static void ParserHotOutValveOff(U8 *buf, I16 len)
{
    CloseValve( VALVE_HOT_OUT );
}

static void ParserNosValveOn(U8 *buf, I16 len)
{
    // Normal Open Type
    CloseValve( VALVE_NOS );
}

static void ParserNosValveOff(U8 *buf, I16 len)
{
    // Normal Open Type
    OpenValve( VALVE_NOS );
}

static void ParserHotDrainValveOn(U8 *buf, I16 len)
{
    // Normal Open Type
    CloseValve( VALVE_HOT_DRAIN );
}

static void ParserHotDrainValveOff(U8 *buf, I16 len)
{
    // Normal Open Type
    OpenValve( VALVE_HOT_DRAIN );
}

static void ParserIceTrayValveOn(U8 *buf, I16 len)
{
    OpenValve( VALVE_ICE_TRAY_IN );
}

static void ParserIceTrayValveOff(U8 *buf, I16 len)
{
    CloseValve( VALVE_ICE_TRAY_IN );
}

static void ParserDrainValveOn(U8 *buf, I16 len)
{
    OpenValve( VALVE_COLD_DRAIN );
}

static void ParserDrainValveOff(U8 *buf, I16 len)
{
    CloseValve( VALVE_COLD_DRAIN );
}

static void ParserColdOvfValveOn(U8 *buf, I16 len)
{
    // Normal Open Type
    CloseValve( VALVE_COLD_AIR );
}

static void ParserColdOvfValveOff(U8 *buf, I16 len)
{
    // Normal Open Type
    OpenValve( VALVE_COLD_AIR );
}

static void ParserColdFlushValveOn(U8 *buf, I16 len)
{
    OpenValve( VALVE_COLD_FLUSH );
}

static void ParserColdFlushValveOff(U8 *buf, I16 len)
{
    CloseValve( VALVE_COLD_FLUSH );
}

static void ParserFanMotoreOn(U8 *buf, I16 len)
{
    HAL_TurnOnFanMotor();
}

static void ParserFanMotorOff(U8 *buf, I16 len)
{
    HAL_TurnOffFanMotor();
}

static void ParserGasSwitchOn(U8 *buf, I16 len)
{
    TEST_StartGasValve();
}

static void ParserGasSwitchOff(U8 *buf, I16 len)
{
    TEST_StopGasValve();
}

static void ParserIceTrayMotorOn(U8 *buf, I16 len)
{
    RunCheckIceTray = TRUE;
}

static void ParserIceTrayMotorOff(U8 *buf, I16 len)
{
    RunCheckIceTray = FALSE;
}

static void ParserIceDoorMotorOn(U8 *buf, I16 len)
{
    TEST_StartIceDoor();
}

static void ParserIceDoorMotorOff(U8 *buf, I16 len)
{
    TEST_StopIceDoor();
}

static void ParserFeederMotorOn(U8 *buf, I16 len)
{
    TurnOnRelay( RELAY_FEEDER_CW );
    TurnOffRelay( RELAY_FEEDER_CCW );
}

static void ParserFeederMotorOff(U8 *buf, I16 len)
{
    TurnOffRelay( RELAY_FEEDER_CW );
    TurnOffRelay( RELAY_FEEDER_CCW );
}

static void ParserSwingBarOn(U8 *buf, I16 len)
{
    TURN_ON_SWING_BAR();
}

static void ParserSwingBarOff(U8 *buf, I16 len)
{
    TURN_OFF_SWING_BAR();
}

static void ParserHotFlowValveOn(U8 *buf, I16 len)
{
    TEST_StartFlowValve();
}

static void ParserHotFlowValveOff(U8 *buf, I16 len)
{
    TEST_StopFlowValve();
}

static void ParserDrainPumpOn(U8 *buf, I16 len)
{
    HAL_TurnOnPump( HAL_PUMP_DRAIN, 24.0f );
}

static void ParserDrainPumpOff(U8 *buf, I16 len)
{
    HAL_TurnOffPump( HAL_PUMP_DRAIN );
}

// [0] : TARGET RPS
static void ParserCompOn(U8 *buf, I16 len)
{
    RPS_T mRps;

    mRps = ConvAsc2Byte( buf[0], buf[1] );
    SetCompBldcTargetRps( mRps );
}

static void ParserCompOff(U8 *buf, I16 len)
{
    SetCompBldcTargetRps( 0 );
}

static void ParserIceSystemCold(U8 *buf, I16 len)
{
    GasSwitchCold();
}

static void ParserIceSystemIce(U8 *buf, I16 len)
{
    GasSwitchIce();
}

static void ParserIceSystemDeicing(U8 *buf, I16 len)
{
    GasSwitchHotGas();
}

static void ParserComplete(U8 *buf, I16 len)
{
    SetEolMode( EOL_MODE_DONE );

    TurnOffAllLoad();
}



/* Make checksum */
I16	Crc16_EOL( U8 *buf, I16 len )
{
    U8 mu8Chksum = 0;
    U8 crc_buf[5];


    if( len < MIN_PKT_SZ )
    {
        return 0; // error..
    }

    mu8Chksum = check_crc( buf, len);
    SPRINTF( (char __FAR *)crc_buf, (const char __FAR *)"%02X", mu8Chksum );

    buf[ len - 3 ] = crc_buf[0];
    buf[ len - 2 ] = crc_buf[1];

    return len;
}

typedef	I16	(*Action_t)( U8 *buf, U16 mu16PktType );
typedef struct _make_list_t
{
    U16 Type;
    Action_t MakePkt;
} make_list_t;

static I16	MakePkt_Model( U8 *buf, U16 mu16PktType );
static I16	MakePkt_Setting( U8 *buf, U16 mu16PktType );

static I16	MakePkt_Wifi( U8 *buf, U16 mu16PktType );
static I16	MakePkt_UvIce( U8 *buf, U16 mu16PktType );
static I16	MakePkt_FlowSensor( U8 *buf, U16 mu16PktType );
static I16	MakePkt_CurrentAcDc( U8 *buf, U16 mu16PktType );
static I16	MakePkt_UvWater( U8 *buf, U16 mu16PktType );
//static I16	MakePkt_SavingPhoto( U8 *buf, U16 mu16PktType );
static I16	MakePkt_UvIceTray( U8 *buf, U16 mu16PktType );
static I16	MakePkt_UvIceTank( U8 *buf, U16 mu16PktType );
static I16	MakePkt_IceFull( U8 *buf, U16 mu16PktType );

static I16	MakePkt_SensorTemp( U8 *buf, U16 mu16PktType );
static I16	MakePkt_SensorLevel( U8 *buf, U16 mu16PktType );
static I16	MakePkt_Sensor( U8 *buf, U16 mu16PktType );
static I16	MakePkt_SensorHeater( U8 *buf, U16 mu16PktType );
static I16	MakePkt_SystemCold( U8 *buf, U16 mu16PktType );
static I16	MakePkt_SystemIce( U8 *buf, U16 mu16PktType );
static I16	MakePkt_NoPayLoad( U8 *buf, U16 mu16PktType );

const static make_list_t make_list[] = 
{
    /* CHECK COLD/ICE SYSTEM */
    { PKT_EOL_COMP_ON,                  MakePkt_NoPayLoad },            
    { PKT_EOL_COMP_OFF,                 MakePkt_NoPayLoad },           
    { PKT_EOL_ICE_SYSTEM_COLD,          MakePkt_NoPayLoad },    
    { PKT_EOL_ICE_SYSTEM_ICE,           MakePkt_NoPayLoad },     
    { PKT_EOL_ICE_SYSTEM_DEICING,       MakePkt_NoPayLoad }, 

    { PKT_EOL_COMPLETE,                 MakePkt_NoPayLoad },

    { PKT_EOL_MODEL,                    MakePkt_Model   },
    { PKT_EOL_SETTING,                  MakePkt_Setting },             

    /* ETC */
    { PKT_EOL_WIFI,                     MakePkt_Wifi        },                
    { PKT_EOL_UV_1_ICE,                 MakePkt_UvIce       },            
    { PKT_EOL_FLOW_SESNOR,              MakePkt_FlowSensor  },         
    { PKT_EOL_AC_DC,                    MakePkt_CurrentAcDc },               
    { PKT_EOL_UV_2_WATER,               MakePkt_UvWater     },          
//    { PKT_EOL_SAVING_PHOTO,             MakePkt_SavingPhoto },        
    { PKT_EOL_UV_3_ICE_TRAY,            MakePkt_UvIceTray   },       
    { PKT_EOL_UV_4_ICE_TANK,            MakePkt_UvIceTank   },       
    { PKT_EOL_ICE_FULL,                 MakePkt_IceFull     },            

    /* SENSOR */
    { PKT_EOL_SENSOR_TEMP,              MakePkt_SensorTemp  },         
    { PKT_EOL_SENSOR_LEVEL,             MakePkt_SensorLevel },        
    { PKT_EOL_SENSOR,                   MakePkt_Sensor      },              
    { PKT_EOL_SENSOR_HEATER,            MakePkt_SensorHeater },       

    /* LOAD */
    { PKT_EOL_NO_LOAD,                  MakePkt_NoPayLoad },             

    { PKT_EOL_HEATER_ON,                MakePkt_NoPayLoad },    
    { PKT_EOL_HEATER_OFF,               MakePkt_NoPayLoad },   

    { PKT_EOL_COLD_IN_VALVE_ON,         MakePkt_NoPayLoad },    
    { PKT_EOL_COLD_IN_VALVE_OFF,        MakePkt_NoPayLoad },   

    { PKT_EOL_HOT_IN_VALVE_ON,          MakePkt_NoPayLoad },     
    { PKT_EOL_HOT_IN_VALVE_OFF,         MakePkt_NoPayLoad },    

    { PKT_EOL_ROOM_OUT_VALVE_ON,        MakePkt_NoPayLoad },   
    { PKT_EOL_ROOM_OUT_VALVE_OFF,       MakePkt_NoPayLoad },  

    { PKT_EOL_COLD_OUT_VALVE_ON,        MakePkt_NoPayLoad },   
    { PKT_EOL_COLD_OUT_VALVE_OFF,       MakePkt_NoPayLoad },  

    { PKT_EOL_HOT_OUT_VALVE_ON,         MakePkt_NoPayLoad },    
    { PKT_EOL_HOT_OUT_VALVE_OFF,        MakePkt_NoPayLoad },   

    { PKT_EOL_NOS_VALVE_ON,             MakePkt_NoPayLoad },        
    { PKT_EOL_NOS_VALVE_OFF,            MakePkt_NoPayLoad },       

    { PKT_EOL_HOT_DRAIN_VALVE_ON,       MakePkt_NoPayLoad },  
    { PKT_EOL_HOT_DRAIN_VALVE_OFF,      MakePkt_NoPayLoad }, 

    { PKT_EOL_ICE_TRAY_VALVE_ON,        MakePkt_NoPayLoad },   
    { PKT_EOL_ICE_TRAY_VALVE_OFF,       MakePkt_NoPayLoad },  

    { PKT_EOL_DRAIN_VALVE_ON,           MakePkt_NoPayLoad },      
    { PKT_EOL_DRAIN_VALVE_OFF,          MakePkt_NoPayLoad },     

    { PKT_EOL_COLD_OVF_VALVE_ON,        MakePkt_NoPayLoad },   
    { PKT_EOL_COLD_OVF_VALVE_OFF,       MakePkt_NoPayLoad },  

    { PKT_EOL_COLD_FLUSH_VALVE_ON,      MakePkt_NoPayLoad },
    { PKT_EOL_COLD_FLUSH_VALVE_OFF,     MakePkt_NoPayLoad },

    { PKT_EOL_FAN_MOTOR_ON,             MakePkt_NoPayLoad },        
    { PKT_EOL_FAN_MOTOR_OFF,            MakePkt_NoPayLoad },       

    { PKT_EOL_GAS_SWITCH_ON,            MakePkt_NoPayLoad },       
    { PKT_EOL_GAS_SWITCH_OFF,           MakePkt_NoPayLoad },      

    { PKT_EOL_ICE_TRAY_MOTOR_ON,        MakePkt_NoPayLoad },   
    { PKT_EOL_ICE_TRAY_MOTOR_OFF,       MakePkt_NoPayLoad },  

    { PKT_EOL_ICE_DOOR_MOTOR_ON,        MakePkt_NoPayLoad },   
    { PKT_EOL_ICE_DOOR_MOTOR_OFF,       MakePkt_NoPayLoad },  

    { PKT_EOL_ICE_FEEDER_MOTOR_ON,      MakePkt_NoPayLoad }, 
    { PKT_EOL_ICE_FEEDER_MOTOR_OFF,     MakePkt_NoPayLoad },

    { PKT_EOL_SWING_BAR_ON,             MakePkt_NoPayLoad },       
    { PKT_EOL_SWING_BAR_OFF,            MakePkt_NoPayLoad },      

    { PKT_EOL_HOT_FLOW_VALVE_ON,        MakePkt_NoPayLoad },  
    { PKT_EOL_HOT_FLOW_VALVE_OFF,       MakePkt_NoPayLoad }, 

    { PKT_EOL_DRAIN_PUMP_ON,            MakePkt_NoPayLoad },      
    { PKT_EOL_DRAIN_PUMP_OFF,           MakePkt_NoPayLoad },     



    /* DEBUG MESSAGE */
    { PKT_EOL_DEBUG_CONTROL,         NULL },
    { PKT_EOL_DEBUG_1,               MakePkt_Debug_1 },
    { PKT_EOL_DEBUG_2,               MakePkt_Debug_2 },
    { PKT_EOL_DEBUG_3,               MakePkt_Debug_3 },
    { PKT_EOL_DEBUG_4,               MakePkt_Debug_4 },
    { PKT_EOL_DEBUG_5,               MakePkt_Debug_5 },
    { PKT_EOL_DEBUG_6,               MakePkt_Debug_6 },
    //{ PKT_EOL_DEBUG_7,               MakePkt_Debug_7 },
};
#define	SZ_MAKE_TABLE	( sizeof( make_list ) / sizeof( make_list_t ))

I16	MakePkt_EOL( CommHeader_T *pComm, U8 *buf )
{
    U16 mu16Type;
    Action_t	p_make_pkt;
    U8 i;
    I16	len	= -1;


    for( i = 0; i < SZ_MAKE_TABLE; i++ )
    {
        mu16Type = make_list[ i ].Type;
        p_make_pkt = make_list[ i ].MakePkt;



        if( mu16Type == (U16)pComm )
        {
            if( p_make_pkt != NULL )
            {
                len = p_make_pkt( buf, mu16Type );
            }
            break;
        }
    }

    return len;
}


static I16	MakePkt_Model( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", MODEL_CODE );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetEolTestMode() );
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

U8 GetSystemEol(void)
{
    if( GetSystem() == SYSTEM_CHP )
    {
        return 0x01;    //EOL_CHP
    }

    return 0;   // EOL_CP
}
static I16	MakePkt_Setting( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetEolCheckStatus( EOL_CHK_ID_EEPROM ) );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", 0 );                   // UNUSED - CHECK SUM 
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02d", VERSION_MAJOR );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", 0 );                   // UNUSED - Í≥†ÎèÑ Î™®Îìú
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", COUNTRY_CODE_KOR );    // Íµ?Í∞?ÏΩîÎìú
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetSystemEol() );      
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

// TBD
static I16	MakePkt_Wifi( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;

    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetWifiStatusValue(WIFI_STATUS_TEST) );
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

static I16	MakePkt_UvIce( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", HAL_GetAdcValue( ANI_UV_ICE_DOOR ));
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

// TBD
static I16	MakePkt_FlowSensor( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetEolCheckStatus( EOL_CHK_ID_FLOW_METER ) );
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

// CALC CURRENT - 24V, 12V
static U16 CalcCurrent( U16 mu16Adc )
{
    F32 mf32Current = 0.0f;


    mf32Current = (F32)mu16Adc * 2.43f;
    mf32Current += 0.5f;
    return (U16)(mf32Current);
}

// CALC CURRENT - DC FAN 
static U16 CalcCurrentFan( U16 mu16Adc )
{
    F32 mf32Current = 0.0f;

    mf32Current = ((F32)mu16Adc * 0.0049f) * 100.0f;
    mf32Current += 0.5f;
    return (U16)(mf32Current);
}

static I16	MakePkt_CurrentAcDc( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;
    U16 mu16RawAdc = 0U;
    U16 mu16Current = 0U;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );

    // 12V Feedback CURRENT
    mu16RawAdc = HAL_GetAdcValue( ANI_12V_FB );
    mu16Current = CalcCurrent( mu16RawAdc );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%06d", mu16Current );

    // 24V Feedback CURRENT
    mu16RawAdc = HAL_GetAdcValue( ANI_24V_FB );
    mu16Current = CalcCurrent( mu16RawAdc );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%06d", mu16Current );

    // DC FAN Feedback CURRENT
    mu16RawAdc = HAL_GetAdcValue( ANI_DC_FAN );
    mu16Current = CalcCurrentFan( mu16RawAdc );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%06d", mu16Current );

    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

static I16	MakePkt_UvWater( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );
    //mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetEolCheckStatus( EOL_CHK_ID_UV_WATER_OUT ) );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", HAL_GetAdcValue( ANI_UV_WATER_OUT ));
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

//static I16	MakePkt_SavingPhoto( U8 *buf, U16 mu16PktType )
//{
//    I16 mi16Len = 0;
//
//
//    buf[ mi16Len++ ] = PKT_STX;
//    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
//    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );
//    //mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetEolCheckStatus( EOL_CHK_ID_SAVING_PHOTO ) );
//    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", HAL_GetAdcValue( ANI_SAVING_PHOTO ));
//    buf[ mi16Len++ ] = 0;     // CRC - UPPER
//    buf[ mi16Len++ ] = 0;     // CRC - LOWER
//    buf[ mi16Len++ ] = PKT_ETX;
//
//    return mi16Len;
//}

static I16	MakePkt_UvIceTray( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetEolCheckStatus( EOL_CHK_ID_UV_ICE_TRAY ) );
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

static I16	MakePkt_UvIceTank( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetEolCheckStatus( EOL_CHK_ID_UV_ICE_TANK ) );
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

static I16	MakePkt_IceFull( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetEolCheckStatus( EOL_CHK_ID_ICE_FULL ) );
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}


// ?ò®?èÑÎ•? EOL ?òï?ãù?úºÎ°? Î≥??ôò
#define TEMP_ERROR      9999        // open, short
static U16 ConvEolTemp(U8 mu8AdcId, TEMP_T mTemp)
{
    U16 mu16Temp = TEMP_ERROR;
    U16 mu16Adc = 0;


    mu16Adc = HAL_GetAdcValue( mu8AdcId );
    if( GetTempErrorType( mu16Adc ) != STATUS_NORMAL )
    {
        mu16Temp = TEMP_ERROR;
    }
    else
    {
        // min/max -99.9 ['C] ~ +99.9 ['C]
        if( mTemp > 99.9f )
        {
            mTemp = 99.9f;
        }
        else if( mTemp < -99.9f )
        {
            mTemp = -99.9f;
        }

        if( mTemp > 0.0f )
        {
            mu16Temp = (U16)( mTemp * 10.0f );  // ?Üå?àò?†ê Ï≤´Ïß∏ ?ûêÎ¶¨ÍπåÏß? ?ëú?òÑ
        }
        else
        {
            // ?ùå?àò?ù∏ Í≤ΩÏö∞... 1xxxÎ°? ?ëú?òÑ...
            mTemp = -mTemp;                     // Î∂??ò∏ Î≥?Í≤?
            mu16Temp = (U16)( mTemp * 10.0f );  // ?Üå?à´?†ê Ï≤´Ïß∏ ?ûêÎ¶¨ÍπåÏß? ?ëú?òÑ
            mu16Temp += 1000;
        }
    }

    return mu16Temp;
}

static I16	MakePkt_SensorTemp( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;

    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );

    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04d", ConvEolTemp( ANI_TEMP_AMBIENT,     GetTemp( TEMP_ID_AMBIENT ) ));
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04d", ConvEolTemp( ANI_TEMP_COLD_WATER,  GetTemp( TEMP_ID_COLD_WATER) ));
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04d", ConvEolTemp( ANI_TEMP_HOT_IN,      GetTemp( TEMP_ID_HOT_IN) ));
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04d", ConvEolTemp( ANI_TEMP_HOT_OUT,     GetTemp( TEMP_ID_HOT_OUT) ));
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04d", ConvEolTemp( ANI_TEMP_HEATER,      GetTemp( TEMP_ID_HEATER) ));
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04d", ConvEolTemp( ANI_TEMP_EVA_1,       GetTemp( TEMP_ID_EVA_1) ));
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04d", ConvEolTemp( ANI_TEMP_ROOM_WATER,  GetTemp( TEMP_ID_ROOM_WATER) ));

    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

// ?àò?úÑ ?Ñº?Ñú Î∞òÌôò
// Ï≤¥Í≤∞?ù¥Î©? 0x01, ÎØ∏Ï≤¥Í≤? 0x00
// ?àò?úÑ ?Ñº?Ñú ÎØ∏Ï≤¥Í≤? ?ÉÅ?ÉúÎ©? high... 
// Î¨? Í∞êÏ?? ?ÉÅ?Éú?ù¥Î©? high...
// Ï≤¥Í≤∞ ?ÉÅ?Éú?ù∏?ç∞ ÎØ∏Ï†ëÏ¥âÏù¥Î©? low
static I16	MakePkt_SensorLevel( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );

    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", !GET_DRAIN_TANK_LOW() );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", !GET_DRAIN_TANK_HIGH() );
//    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", !GET_COLD_TANK_HIGH() );


    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

// 1. ?ïÑ?Ñ∞ Î¶¨Îìú?ä§?úÑÏπ?
// 2. ?ïÑ?Ñ∞ Î¶¨Îìú?ä§?úÑÏπ?
// 3. ?ïÑ?ù¥?ä§ ?ä∏?†à?ù¥ M/S
static I16	MakePkt_Sensor( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );

    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetFilterStatus( ID_FILTER_COVER ) );
    //mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetFilterStatus( ID_FILTER ) );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetEolCheckStatus( EOL_CHK_ID_FILTER_ONOFF ));

    if( GetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_DEICING ) == 0 && GetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_ICING ) == 0 )
    {
        // ?†ÑÏ≤? ÎØ∏Í∞êÏß?
        mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", 0 );
    }
    else if( GetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_DEICING ) == 0 && GetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_ICING ) == 1 )
    {
        // ?ÉàÎπ? ÎØ∏Í∞êÏß?
        mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", 1 );
    }
    else if( GetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_DEICING ) == 1 && GetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_ICING ) == 0 )
    {
        // ?†úÎπ? ÎØ∏Í∞êÏß?
        mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", 2 );
    }
    else
    {
        // ?†úÎπ?/?ÉàÎπ? Î™®Îëê Í∞êÏ??
        mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", 3 );
    }


    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

static I16	MakePkt_SensorHeater( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );

    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetEolCheckStatus( EOL_CHK_ID_HEATER_RELAY ) );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", GetEolCheckStatus( EOL_CHK_ID_HEATER_TRIAC ) );


    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}



static I16	MakePkt_NoPayLoad( U8 *buf, U16 mu16PktType )
{
    I16 mi16Len = 0;

    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%02X", PKT_ACK );
    mi16Len += SPRINTF( (char __FAR *)&buf[ mi16Len ], (const char __FAR *)"%04X", mu16PktType );
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}


