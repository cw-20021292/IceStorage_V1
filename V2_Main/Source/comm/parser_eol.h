#ifndef __PARSER_EOL_H__
#define __PARSER_EOL_H__

#include "prj_type.h"
#include "comm.h"

#define PKT_EOL_NONE                  0x0000

#define PKT_EOL_MODEL                 0x2000

#define PKT_EOL_SETTING               0x3000

    /* ETC */
#define PKT_EOL_WIFI                  0x3010
#define PKT_EOL_UV_1_ICE              0x3011
#define PKT_EOL_FLOW_SESNOR           0x3012
#define PKT_EOL_AC_DC                 0x3013
#define PKT_EOL_UV_2_WATER            0x3014
#define PKT_EOL_SAVING_PHOTO          0x301A
#define PKT_EOL_UV_3_ICE_TRAY         0x301B
#define PKT_EOL_UV_4_ICE_TANK         0x301C
#define PKT_EOL_ICE_FULL              0x301D

    /* SENSOR */
#define PKT_EOL_SENSOR_TEMP           0x3020
#define PKT_EOL_SENSOR_LEVEL          0x3030
#define PKT_EOL_SENSOR                0x3040
#define PKT_EOL_SENSOR_HEATER         0x3050

    /* LOAD */
#define PKT_EOL_NO_LOAD               0x5000

#define PKT_EOL_HEATER_ON             0x5001   // 순간 온수, 일반 온수
#define PKT_EOL_HEATER_OFF            0x5002

#define PKT_EOL_COLD_IN_VALVE_ON      0x5102
#define PKT_EOL_COLD_IN_VALVE_OFF     0x5103

#define PKT_EOL_HOT_IN_VALVE_ON       0x5104
#define PKT_EOL_HOT_IN_VALVE_OFF      0x5105

#define PKT_EOL_ROOM_OUT_VALVE_ON     0x5106
#define PKT_EOL_ROOM_OUT_VALVE_OFF    0x5107

#define PKT_EOL_COLD_OUT_VALVE_ON     0x5108
#define PKT_EOL_COLD_OUT_VALVE_OFF    0x5109

#define PKT_EOL_HOT_OUT_VALVE_ON      0x510A
#define PKT_EOL_HOT_OUT_VALVE_OFF     0x510B

#define PKT_EOL_NOS_VALVE_ON          0x5110
#define PKT_EOL_NOS_VALVE_OFF         0x5111

#define PKT_EOL_HOT_DRAIN_VALVE_ON    0x5114
#define PKT_EOL_HOT_DRAIN_VALVE_OFF   0x5115

#define PKT_EOL_ICE_TRAY_VALVE_ON     0x5116
#define PKT_EOL_ICE_TRAY_VALVE_OFF    0x5117

#define PKT_EOL_DRAIN_VALVE_ON        0x5118      // COLD DRAIN
#define PKT_EOL_DRAIN_VALVE_OFF       0x5119      // COLD DRAIN

#define PKT_EOL_COLD_OVF_VALVE_ON     0x512C      // COLD AIR ( OVER FLOW )
#define PKT_EOL_COLD_OVF_VALVE_OFF    0x512D      // COLD AIR ( OVER FLOW )

#define PKT_EOL_COLD_FLUSH_VALVE_ON   0x513A      // COLD FLUSING
#define PKT_EOL_COLD_FLUSH_VALVE_OFF  0x513B      // COLD FLUSING
    
#define PKT_EOL_FAN_MOTOR_ON          0x5200
#define PKT_EOL_FAN_MOTOR_OFF         0x5201

#define PKT_EOL_GAS_SWITCH_ON         0x5202
#define PKT_EOL_GAS_SWITCH_OFF        0x5203

#define PKT_EOL_ICE_TRAY_MOTOR_ON     0x5206
#define PKT_EOL_ICE_TRAY_MOTOR_OFF    0x5207

#define PKT_EOL_ICE_DOOR_MOTOR_ON     0x5208
#define PKT_EOL_ICE_DOOR_MOTOR_OFF    0x5209

#define PKT_EOL_ICE_FEEDER_MOTOR_ON   0x520A
#define PKT_EOL_ICE_FEEDER_MOTOR_OFF  0x520B

#define PKT_EOL_SWING_BAR_ON          0x5210
#define PKT_EOL_SWING_BAR_OFF         0x5211

#define PKT_EOL_HOT_FLOW_VALVE_ON     0x5212
#define PKT_EOL_HOT_FLOW_VALVE_OFF    0x5213

#define PKT_EOL_DRAIN_PUMP_ON         0x5302
#define PKT_EOL_DRAIN_PUMP_OFF        0x5303


/* CHECK COLD/ICE SYSTEM */
#define PKT_EOL_COMP_ON               0x9900
#define PKT_EOL_COMP_OFF              0x9901

#define PKT_EOL_ICE_SYSTEM_ICE        0x9902
#define PKT_EOL_ICE_SYSTEM_COLD       0x9903
#define PKT_EOL_ICE_SYSTEM_DEICING    0x9904

#define PKT_EOL_COMPLETE              0xFF00

/* DEBUG MESSAGE */
#define PKT_EOL_DEBUG_CONTROL         0x00E0
#define PKT_EOL_DEBUG_1               0x00E1
#define PKT_EOL_DEBUG_2               0x00E2
#define PKT_EOL_DEBUG_3               0x00E3
#define PKT_EOL_DEBUG_4               0x00E4
#define PKT_EOL_DEBUG_5               0x00E5
#define PKT_EOL_DEBUG_6               0x00E6
#define PKT_EOL_DEBUG_7               0x00E7


I16 IsValidPkt_EOL( U8 *buf, I16 len );
I16 ParserPkt_EOL( U8 *buf, I16 len);
I16 Crc16_EOL( U8 *buf, I16 len );
I16 MakePkt_EOL( CommHeader_T *p_comm, U8 *buf );

#endif /* __PARSER_EOL_H__ */
