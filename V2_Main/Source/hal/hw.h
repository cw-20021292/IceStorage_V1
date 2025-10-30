#ifndef __HW__H__
#define __HW__H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
#include "r_cg_adc.h"
#include "r_cg_serial.h"
#include "r_cg_intc.h"
#include "r_cg_macrodriver.h"


/* HW
 *
 * TIMER12-13 : PWM 
 *  -. DRAIN PUMP OUTPUT
 *
 * TIMER0 
 *  -. BASE TIMER 1ms
 *
 *  TIMER1
 *  -. STEP MOTOR 1ms
 *
 *  TIMER2
 *  -. HEATER OUT ( TRIAC )
 *  -. 8.33ms ( 120hz @ 60hz )
 *
 * PIN INTERRUPT
 *  -. FLOW METER
 *  -. INTP11
 *
 */


/* Watchdog timer reset */
#define	RESET_WDT()					R_WDT_Restart()

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : Comp 
 * Max (2kg) : P7.2
 * Standard (1kg) : P7.2
*/
/* COMP POWER */
#define  P_COMP_ONOFF               P7.2
#define  TURN_ON_COMP()             do{ P_COMP_ONOFF = 1; }while(0)
#define  TURN_OFF_COMP()            do{ P_COMP_ONOFF = 0; }while(0)
#define  GET_STATUS_COMP()          ( P_COMP_ONOFF )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : Feeder CW
 * Max (2kg) : P7.4
 * Standard (1kg) : P7.4
*/
/* ICE FEEDER CW */
#define  P_FEEDER_CW                P7.4
#define  TURN_ON_FEEDER_CW()        do{ P_FEEDER_CW = 1; }while(0)
#define  TURN_OFF_FEEDER_CW()       do{ P_FEEDER_CW = 0; }while(0)
#define  GET_STATUS_FEEDER_CW()     ( P_FEEDER_CW )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : Feeder CCW
 * Max (2kg) : P7.5
 * Standard (1kg) : P7.5
*/
/* ICE FEEDER CCW */
#define  P_FEEDER_CCW                P7.5
#define  TURN_ON_FEEDER_CCW()        do{ P_FEEDER_CCW = 1; }while(0)
#define  TURN_OFF_FEEDER_CCW()       do{ P_FEEDER_CCW = 0; }while(0)
#define  GET_STATUS_FEEDER_CCW()     ( P_FEEDER_CCW )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : DRAIN TANK (¼öÀ§¼¾¼­)
 * Max (2kg) : LOW P9.3 / HIGH P9.4
 * Standard (1kg) : LOW P9.3 / HIGH P9.4
*/
/* DRAIN TANK HIGH LEVEL */
#define  P_DRAIN_TANK_HIGH             P9.4
#define  GET_DRAIN_TANK_HIGH()         ( P_DRAIN_TANK_HIGH )

/* DRAIN TANK LOW LEVEL */
#define  P_DRAIN_TANK_LOW              P9.3
#define  GET_DRAIN_TANK_LOW()          ( P_DRAIN_TANK_LOW )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : Step Motor (³Ã¸ÅÀüÈ¯¹ëºê)
 * Max (2kg) : P7.1 / P0.5 / P7.0 / P0.6
 * Standard (1kg) : P7.1 / P0.5 / P7.0 / P0.6
*/
/* STEP  MOTOR - GAS VALVE */
// A,B,/A,/B
#define  P_STEP_MOTOR_GAS_SW_1         P7.1     // A
#define  P_STEP_MOTOR_GAS_SW_2         P0.5     // B
#define  P_STEP_MOTOR_GAS_SW_3         P7.0     // /A
#define  P_STEP_MOTOR_GAS_SW_4         P0.6     // /B

#define  TURN_ON_STEP_MOTOR_GAS_SW_1()     do { P_STEP_MOTOR_GAS_SW_1 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_GAS_SW_2()     do { P_STEP_MOTOR_GAS_SW_2 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_GAS_SW_3()     do { P_STEP_MOTOR_GAS_SW_3 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_GAS_SW_4()     do { P_STEP_MOTOR_GAS_SW_4 = 1; }while(0)

#define  TURN_OFF_STEP_MOTOR_GAS_SW_1()     do { P_STEP_MOTOR_GAS_SW_1 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_GAS_SW_2()     do { P_STEP_MOTOR_GAS_SW_2 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_GAS_SW_3()     do { P_STEP_MOTOR_GAS_SW_3 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_GAS_SW_4()     do { P_STEP_MOTOR_GAS_SW_4 = 0; }while(0)

#define  GET_STEP_MOTOR_GAS_SW_1()     (P_STEP_MOTOR_GAS_SW_1)
#define  GET_STEP_MOTOR_GAS_SW_2()     (P_STEP_MOTOR_GAS_SW_2)
#define  GET_STEP_MOTOR_GAS_SW_3()     (P_STEP_MOTOR_GAS_SW_3)
#define  GET_STEP_MOTOR_GAS_SW_4()     (P_STEP_MOTOR_GAS_SW_4)

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : Step Motor (Ice Door)
 * Max (2kg) : P3.3 / P3.2 / P10.6 / P10.5
 * Standard (1kg) : P3.3 / P3.2 / P10.6 / P10.5
*/
/* STEP MOTOR - ICE DOOR  */
#define  P_STEP_MOTOR_ICE_DOOR_1       P3.3
#define  P_STEP_MOTOR_ICE_DOOR_2       P3.2
#define  P_STEP_MOTOR_ICE_DOOR_3       P10.6
#define  P_STEP_MOTOR_ICE_DOOR_4       P10.5

#define  TURN_ON_STEP_MOTOR_ICE_DOOR_1()     do { P_STEP_MOTOR_ICE_DOOR_1 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_ICE_DOOR_2()     do { P_STEP_MOTOR_ICE_DOOR_2 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_ICE_DOOR_3()     do { P_STEP_MOTOR_ICE_DOOR_3 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_ICE_DOOR_4()     do { P_STEP_MOTOR_ICE_DOOR_4 = 1; }while(0)

#define  TURN_OFF_STEP_MOTOR_ICE_DOOR_1()     do { P_STEP_MOTOR_ICE_DOOR_1 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_ICE_DOOR_2()     do { P_STEP_MOTOR_ICE_DOOR_2 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_ICE_DOOR_3()     do { P_STEP_MOTOR_ICE_DOOR_3 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_ICE_DOOR_4()     do { P_STEP_MOTOR_ICE_DOOR_4 = 0; }while(0)

#define  GET_STEP_MOTOR_ICE_DOOR_4()   (P_STEP_MOTOR_ICE_DOOR_4)
#define  GET_STEP_MOTOR_ICE_DOOR_3()   (P_STEP_MOTOR_ICE_DOOR_3)
#define  GET_STEP_MOTOR_ICE_DOOR_2()   (P_STEP_MOTOR_ICE_DOOR_2)
#define  GET_STEP_MOTOR_ICE_DOOR_1()   (P_STEP_MOTOR_ICE_DOOR_1)

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : Step Motor (À¯·®Á¶Àý¹ëºê)
 * Max (2kg) : P8.2 / P8.3 / P8.4 / P8.5
 * Standard (1kg) : P8.2 / P8.3 / P8.4 / P8.5
*/
/* STEP MOTOR - FLOW_VALVE ( HOT WATER OUT )  */
#define  P_STEP_MOTOR_FLOW_VALVE_1      P8.2
#define  P_STEP_MOTOR_FLOW_VALVE_2      P8.3
#define  P_STEP_MOTOR_FLOW_VALVE_3      P8.4
#define  P_STEP_MOTOR_FLOW_VALVE_4      P8.5

#define  TURN_ON_STEP_MOTOR_FLOW_VALVE_1()     do { P_STEP_MOTOR_FLOW_VALVE_1 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_FLOW_VALVE_2()     do { P_STEP_MOTOR_FLOW_VALVE_2 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_FLOW_VALVE_3()     do { P_STEP_MOTOR_FLOW_VALVE_3 = 1; }while(0)
#define  TURN_ON_STEP_MOTOR_FLOW_VALVE_4()     do { P_STEP_MOTOR_FLOW_VALVE_4 = 1; }while(0)

#define  TURN_OFF_STEP_MOTOR_FLOW_VALVE_1()     do { P_STEP_MOTOR_FLOW_VALVE_1 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_FLOW_VALVE_2()     do { P_STEP_MOTOR_FLOW_VALVE_2 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_FLOW_VALVE_3()     do { P_STEP_MOTOR_FLOW_VALVE_3 = 0; }while(0)
#define  TURN_OFF_STEP_MOTOR_FLOW_VALVE_4()     do { P_STEP_MOTOR_FLOW_VALVE_4 = 0; }while(0)

#define  GET_STEP_MOTOR_FLOW_VALVE_1()          (P_STEP_MOTOR_FLOW_VALVE_1)
#define  GET_STEP_MOTOR_FLOW_VALVE_2()          (P_STEP_MOTOR_FLOW_VALVE_2)
#define  GET_STEP_MOTOR_FLOW_VALVE_3()          (P_STEP_MOTOR_FLOW_VALVE_3)
#define  GET_STEP_MOTOR_FLOW_VALVE_4()          (P_STEP_MOTOR_FLOW_VALVE_4)

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : Drain Pump (PWM)
 * Max (2kg) : P6.7
 * Standard (1kg) : P6.7
*/
/* DRAIN PUMP PWM OUTPUT */
#define  P_DRAIN_PUMP                  P6.7
#define  TURN_ON_DRAIN_PUMP()          do{ P_DRAIN_PUMP = 1; }while(0)
#define  TURN_OFF_DRAIN_PUMP()         do{ P_DRAIN_PUMP = 0; }while(0)

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : ÇÊÅÍ ¸®µå½ºÀ§Ä¡
 * Max (2kg) : P12.4
 * Standard (1kg) : P12.4
*/
/* FILTER DETECT */
#define  P_FILTER_DETECT               P12.4
#define  GET_STATUS_FILTER_DETECT()    ( P_FILTER_DETECT )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : Swing Bar
 * Max (2kg) : P5.2
 * Standard (1kg) : P5.6
*/
/* SWING BAR */
#define  P_SWING_BAR                   P5.2
#define  TURN_ON_SWING_BAR()           do{ P_SWING_BAR = 1; }while(0)
#define  TURN_OFF_SWING_BAR()          do{ P_SWING_BAR = 0; }while(0)
#define  GET_STATUS_SWING_BAR()        ( P_SWING_BAR )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : UV LED (Tray)
 * Max (2kg) : P13.0
 * Standard (1kg) : P13.0
*/
/* UV_ICE_TRAY ON/OFF (1)*/
#define  P_UV_ONOFF_ICE_TRAY           P13.0
#define  TURN_ON_UV_ICE_TRAY()         do{ P_UV_ONOFF_ICE_TRAY = 1; }while(0)
#define  TURN_OFF_UV_ICE_TRAY()        do{ P_UV_ONOFF_ICE_TRAY = 0; }while(0)
#define  GET_STATUS_UV_ICE_TRAY()      ( P_UV_ONOFF_ICE_TRAY )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : UV LED (Tank)
 * Max (2kg) : P0.7
 * Standard (1kg) : P0.7
*/
/* UV_ICE TANK ON/OFF (2)*/
#define  P_UV_ONOFF_ICE_TANK            P0.7
#define  TURN_ON_UV_ICE_TANK()          do{ P_UV_ONOFF_ICE_TANK = 1; }while(0)
#define  TURN_OFF_UV_ICE_TANK()         do{ P_UV_ONOFF_ICE_TANK = 0; }while(0)
#define  GET_STATUS_UV_ICE_TANK()       ( P_UV_ONOFF_ICE_TANK )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : WIFI Pin
 * Max (2kg) : P0.4 / 0.1
 * Standard (1kg) : P0.4 / 0.1
*/
/* WIFI */
#define P_WIFI_ONOFF                            P0.4
#define TURN_ON_WIFI_MODULE()                  do{ P_WIFI_ONOFF = 1; }while(0)
#define TURN_OFF_WIFI_MODULE()                 do{ P_WIFI_ONOFF = 0; }while(0)

#define P_WIFI_RESET                            P0.1
#define TURN_ON_RESET_WIFI_MODULE()            do{ P_WIFI_RESET = 1; }while(0)
#define TURN_OFF_RESET_WIFI_MODULE()           do{ P_WIFI_RESET = 0; }while(0)

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : TRAY MICRO SWITCH
 * Max (2kg) : P14.5 / 0.0
 * Standard (1kg) : P14.5 / 0.0
*/
/* ICE TRAY - MICRO SW (LOW)*/
#define  P_MICRO_SW_ICE_TRAY_1          P14.5
#define  GET_MICRO_SW_ICE_TRAY_1()      ( P_MICRO_SW_ICE_TRAY_1 )

/* ICE TRAY - MICRO SW (HIGH)*/
#define  P_MICRO_SW_ICE_TRAY_2          P0.0
#define  GET_MICRO_SW_ICE_TRAY_2()      ( P_MICRO_SW_ICE_TRAY_2 )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : TANK COVER
 * Max (2kg) : P13.7 / 12.2
 * Standard (1kg) : P13.7 / 12.2
*/
/* ICE TANK COVER REED_1 */
#define  P_REED_SW_TANK_OPEN            P13.7
#define  GET_REED_SW_TANK_OPEN()        ( P_REED_SW_TANK_OPEN )

/* ICE TANK COVER REED_2 */
#define  P_REED_SW_TANK_OPEN_2          P12.2
#define  GET_REED_SW_TANK_OPEN_2()      ( P_REED_SW_TANK_OPEN_2 )

#if 0
/* COLD TANK OVERFLOW */
#define  P_COLD_TANK_LOW                P7.5
#define  GET_COLD_TANK_LOW()            ( P_COLD_TANK_LOW )

/* COLD TANK HIGH */
#define  P_COLD_TANK_HIGH               P7.5
#define  GET_COLD_TANK_HIGH()           ( P_COLD_TANK_HIGH )

/* COLD TANK OVERFLOW */
#define  P_COLD_TANK_OVERFLOW           P10.1
#define  GET_COLD_TANK_OVERFLOW()       ( P_COLD_TANK_OVERFLOW )
#endif

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : ICE LOW, FULL IR POWER
 * Max (2kg) : P14.1(Àú) / P14.2(¸¸)
 * Standard (1kg) : P14.1(Àú) / P14.2(¸¸)
*/
/* ICE - ICE FULL (¸¸ºù) */
#define  P_ICE_INFRARED                     P14.2
#define  TURN_ON_ICE_INFRARED()             do{ P_ICE_INFRARED = 1; }while(0)
#define  TURN_OFF_ICE_INFRARED()            do{ P_ICE_INFRARED = 0; }while(0)
#define  GET_STATUS_ICE_INFRARED()          ( P_ICE_INFRARED )

/* ICE - ICE LOW (Àúºù) */
#define  P_ICE_INFRARED_LOW                P14.1
#define  TURN_ON_ICE_INFRARED_LOW()        do{ P_ICE_INFRARED_LOW = 1; }while(0)
#define  TURN_OFF_ICE_INFRARED_LOW()       do{ P_ICE_INFRARED_LOW = 0; }while(0)
#define  GET_STATUS_ICE_INFRARED_LOW()     ( P_ICE_INFRARED_LOW )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : CONDENSOR FAN
 * Max (2kg) : P11.4
 * Standard (1kg) : P11.4
*/
/* Condensor DC FAN MOTOR - 1 */
#define  P_FAN_MOTOR_1                      P11.4
#define  TURN_ON_FAN_MOTOR_1()              do{ P_FAN_MOTOR_1 = 1; }while(0)
#define  TURN_OFF_FAN_MOTOR_1()             do{ P_FAN_MOTOR_1 = 0; }while(0)
#define  GET_STATUS_FAN_MOTOR_1_ONOFF()     (P_FAN_MOTOR_1)

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : COLD TEMP SENSOR POWER
 * Max (2kg) : P6.3
 * Standard (1kg) : P6.3
*/
/* COLD TEMP ADC ON/OFF - ROOM TEMPERTURE  */
#define  P_ADC_COLD_ONOFF           P6.3
#define  TURN_ON_ADC_COLD()         do{ P_ADC_COLD_ONOFF = 1; }while(0)
#define  TURN_OFF_ADC_COLD()        do{ P_ADC_COLD_ONOFF = 0; }while(0)

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : MODEL DIVIDE PIN (CPI / CHPI)
 * Max (2kg) : P7.6
 * Standard (1kg) : P7.6
*/
#define P_MODEL_DIV                  P7.6
#define GET_STATUS_MODEL_INPUT()     (P_MODEL_DIV )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : ROOM TEMP SENSOR POWER
 * Max (2kg) : P6.2
 * Standard (1kg) : P6.2
*/
/* ROOM ADC ON/OFF - ROOM TEMPERTURE  */
#define  P_ADC_ROOM_ONOFF           P6.2
#define  TURN_ON_ADC_ROOM()         do{ P_ADC_ROOM_ONOFF = 1; }while(0)
#define  TURN_OFF_ADC_ROOM()        do{ P_ADC_ROOM_ONOFF = 0; }while(0)

/* CODY SERVICE S/W */
#define  P_SERVICE_SW               P14.5
#define  GET_STATUS_SERVICE()       ( P_SERVICE_SW )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : °øÁ¤°Ë»ç Åë½Å PIN
 * Max (2kg) : P14.4 / P14.3
 * Standard (1kg) : P14.4 / P14.3
*/
/* EOL COMM LINE */
#define  PM_EOL_RX                  PM14.3
#define  P_EOL_RX                   P14.3

#define  PM_EOL_TX                  PM14.4
#define  P_EOL_TX                   P14.4

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : BLDC COMP Åë½Å PIN
 * Max (2kg) : P1.3 / P1.4
 * Standard (1kg) : P1.3 / P1.4
*/
/* BLDC COMP COMM LINE */
#define  PM_COMP_RX                  PM1.4
#define  P_COMP_RX                   P1.4

#define  PM_COMP_TX                  PM1.3
#define  P_COMP_TX                   P1.3

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : FLOW SENSEOR
 * Max (2kg) : P7.7
 * Standard (1kg) : P7.7
*/
#define P_FLOW_SENSOR               P7.7
#define  GET_FLOW_SENSOR()          ( P_FLOW_SENSOR )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : HEATER RELAY / TRIAC
 * Max (2kg) : P4.7 / P10.3
 * Standard (1kg) : P4.7 / P10.3
*/
// RELAY
#define P_HEATER_RELAY               P4.7
#define TURN_ON_HEATER_RELAY()       do{ P_HEATER_RELAY = 1; }while(0)
#define TURN_OFF_HEATER_RELAY()      do{ P_HEATER_RELAY = 0; }while(0)
#define GET_STATUS_HEATER_RELAY()    (P_HEATER_RELAY )

// TRIAC
#define P_HEATER_TRIAC                P10.3
#define TURN_ON_HEATER_TRIAC()        do{ P_HEATER_TRIAC = 1; }while(0)
#define TURN_OFF_HEATER_TRIAC()       do{ P_HEATER_TRIAC = 0; }while(0)
#define GET_STATUS_HEATER_TRIAC()     (P_HEATER_TRIAC )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : NOS V/V
 * Max (2kg) : P8.1
 * Standard (1kg) : ?
*/
/* VALVE - NOS */
#define  P_VALVE_NOS                        P8.1
#define  OPEN_VALVE_NOS()                   do{ P_VALVE_NOS = 0; }while(0)
#define  CLOSE_VALVE_NOS()                  do{ P_VALVE_NOS = 1; }while(0)
#define  GET_STATUS_VALVE_NOS()             ( P_VALVE_NOS )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : ROOM COLD OUT V/V
 * Max (2kg) : P8.6
 * Standard (1kg) : ?
*/
/* VALVE - ROOM OUT */
#define  P_VALVE_ROOM_COLD_OUT              P8.6
#define  OPEN_VALVE_ROOM_COLD_OUT()         do{ P_VALVE_ROOM_COLD_OUT = 1; }while(0)
#define  CLOSE_VALVE_ROOM_COLD_OUT()        do{ P_VALVE_ROOM_COLD_OUT = 0; }while(0)
#define  GET_STATUS_VALVE_ROOM_COLD_OUT()   ( P_VALVE_ROOM_COLD_OUT )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : TRAY IN V/V
 * Max (2kg) : P5.1
 * Standard (1kg) : ?
*/
/* VALVE - ICE TRAY */
#define  P_VALVE_ICE_TRAY_IN                P5.1
#define  OPEN_VALVE_ICE_TRAY_IN()           do{ P_VALVE_ICE_TRAY_IN = 1; }while(0)
#define  CLOSE_VALVE_ICE_TRAY_IN()          do{ P_VALVE_ICE_TRAY_IN = 0; }while(0)
#define  GET_STATUS_VALVE_ICE_TRAY_IN()     ( P_VALVE_ICE_TRAY_IN )

/* ¹Ì»ç¿ë */
/* VALVE - COLD OUT */
#define  P_VALVE_COLD_OUT                   P10.2
#define  OPEN_VALVE_COLD_OUT()              do{ P_VALVE_COLD_OUT = 1; }while(0)
#define  CLOSE_VALVE_COLD_OUT()             do{ P_VALVE_COLD_OUT = 0; }while(0)
#define  GET_STATUS_VALVE_COLD_OUT()        ( P_VALVE_COLD_OUT )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : COLD IN V/V
 * Max (2kg) : P8.7
 * Standard (1kg) : ?
*/
/* VALVE - COLD IN */
#define  P_VALVE_COLD_IN                    P8.7
#define  OPEN_VALVE_COLD_IN()               do{ P_VALVE_COLD_IN = 1; }while(0)
#define  CLOSE_VALVE_COLD_IN()              do{ P_VALVE_COLD_IN = 0; }while(0)
#define  GET_STATUS_VALVE_COLD_IN()         ( P_VALVE_COLD_IN )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : HOT/COLD VOERFLOW V/V
 * Max (2kg) : P5.6
 * Standard (1kg) : ?
*/
/* VALVE - COLD AIR ( NOS ) */
#define P_VALVE_COLD_AIR                    P5.6
#define OPEN_VALVE_COLD_AIR()               do{ P_VALVE_COLD_AIR = 0; }while(0)
#define CLOSE_VALVE_COLD_AIR()              do{ P_VALVE_COLD_AIR = 1; }while(0)
#define GET_STATUS_VALVE_COLD_AIR()         ( P_VALVE_COLD_AIR )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : COLD DRAIN V/V
 * Max (2kg) : P5.3
 * Standard (1kg) : ?
*/
#define P_VALVE_COLD_DRAIN                  P5.3
#define OPEN_VALVE_COLD_DRAIN()             do{ P_VALVE_COLD_DRAIN = 1; }while(0)
#define CLOSE_VALVE_COLD_DRAIN()            do{ P_VALVE_COLD_DRAIN = 0; }while(0)
#define GET_STATUS_VALVE_COLD_DRAIN()       ( P_VALVE_COLD_DRAIN )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : HOT IN V/V
 * Max (2kg) : P5.3
 * Standard (1kg) : ?
*/
/* VALVE - HOT IN */
#define  P_VALVE_HOT_IN                     P8.0
#define  OPEN_VALVE_HOT_IN()                do{ P_VALVE_HOT_IN = 1; }while(0)
#define  CLOSE_VALVE_HOT_IN()               do{ P_VALVE_HOT_IN = 0; }while(0)
#define  GET_STATUS_VALVE_HOT_IN()          ( P_VALVE_HOT_IN )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : HOT DRAIN V/V (NOS)
 * Max (2kg) : P5.0
 * Standard (1kg) : ?
*/
#define P_VALVE_HOT_DRAIN                   P5.0
#define OPEN_VALVE_HOT_DRAIN()              do{ P_VALVE_HOT_DRAIN = 0; }while(0)
#define CLOSE_VALVE_HOT_DRAIN()             do{ P_VALVE_HOT_DRAIN = 1; }while(0)
#define  GET_STATUS_VALVE_HOT_DRAIN()       ( P_VALVE_HOT_DRAIN )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : HOT OUT V/V
 * Max (2kg) : P3.0
 * Standard (1kg) : ?
*/
#define P_VALVE_HOT_OUT                     P3.0
#define OPEN_VALVE_HOT_OUT()                do{ P_VALVE_HOT_OUT = 1; }while(0)
#define CLOSE_VALVE_HOT_OUT()               do{ P_VALVE_HOT_OUT = 0; }while(0)
#define  GET_STATUS_VALVE_HOT_OUT()         ( P_VALVE_HOT_OUT )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : ROOM IN V/V
 * Max (2kg) : P5.4
 * Standard (1kg) : ?
*/
#define P_VALVE_ROOM_IN                     P5.4
#define OPEN_VALVE_ROOM_IN()                do{ P_VALVE_ROOM_IN = 1; }while(0)
#define CLOSE_VALVE_ROOM_IN()               do{ P_VALVE_ROOM_IN = 0; }while(0)
#define GET_STATUS_VALVE_ROOM_IN()          ( P_VALVE_ROOM_IN )

/**
 * @attention ¸ÅÄª¿Ï·á
 * Load : ICE WATER OUT V/V
 * Max (2kg) : P5.5
 * Standard (1kg) : ?
*/
#define P_VALVE_ICE_WATER_OUT()             P5.5
#define OPEN_VALVE_ICE_WATER_OUT()          do{ P_VALVE_ROOM_IN = 1; }while(0)
#define CLOSE_VALVE_ICE_WATER_OUT()         do{ P_VALVE_ROOM_IN = 0; }while(0)
#define GET_STATUS_VALVE_ICE_WATER_OUT()    ( P_VALVE_ROOM_IN )

/**************************************************************************************************/
/* ICE TARY CW */
#define  P_ICE_TRAY_CW                P4.1
#define  TURN_ON_ICE_TRAY_CW()        do{ P_ICE_TRAY_CW = 1; }while(0)
#define  TURN_OFF_ICE_TRAY_CW()       do{ P_ICE_TRAY_CW = 0; }while(0)
#define  GET_STATUS_ICE_TRAY_CW()     ( P_ICE_TRAY_CW )

/* ICE TRAY CCW */
#define  P_ICE_TRAY_CCW                P4.2
#define  TURN_ON_ICE_TRAY_CCW()        do{ P_ICE_TRAY_CCW = 1; }while(0)
#define  TURN_OFF_ICE_TRAY_CCW()       do{ P_ICE_TRAY_CCW = 0; }while(0)
#define  GET_STATUS_ICE_TRAY_CCW()     ( P_ICE_TRAY_CCW )

/* SMPS 12V ON/OFF */
#define  P_SMPS_12V                    P4.7
#define  TURN_ON_SMPS_12V()            do{ P_SMPS_12V = 0; }while(0)
#define  TURN_OFF_SMPS_12V()           do{ P_SMPS_12V = 1; }while(0)
#define  GET_STATUS_SMPS_12V()         ( P_SMPS_12V )

/* ??? */
/* VALVE - OUT POWER - DC 24V */
// ROOM OUT, HOT OUT, COLD OUT, COLD DRAIN
#define  P_VALVE_POWER_OUT                  P7.4
#define  OPEN_VALVE_POWER_OUT()             do{ P_VALVE_POWER_OUT = 1; }while(0)
#define  CLOSE_VALVE_POWER_OUT()            do{ P_VALVE_POWER_OUT = 0; }while(0)
#define  GET_STATUS_VALVE_POWER_OUT()       ( P_VALVE_POWER_OUT )

/* ??? */
/* VALVE - COLD FLUSHING (FEED) */
#define  P_VALVE_COLD_FLUSH                 P5.2
#define  OPEN_VALVE_COLD_FLUSH()            do{ P_VALVE_COLD_FLUSH = 1; }while(0)
#define  CLOSE_VALVE_COLD_FLUSH()           do{ P_VALVE_COLD_FLUSH = 0; }while(0)
#define  GET_STATUS_VALVE_COLD_FLUSH()      (P_VALVE_COLD_FLUSH)

/* ¹Ì»ç¿ë */
/* LEAK ON/OFF */
#define  P_LEAK_ONOFF                P13.0
#define  TURN_ON_LEAK_ONOFF()        do{ P_LEAK_ONOFF = 0; }while(0)
#define  TURN_OFF_LEAK_ONOFF()       do{ P_LEAK_ONOFF = 1; }while(0)
#define  GET_STATUS_LEAK_ONOFF()     ( P_LEAK_ONOFF )

#endif /* __HW__H__ */

