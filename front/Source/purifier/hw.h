/**
 * File : hw.h
 * 
 * Hardware Pin Define
*/
#ifndef __HW_H__
#define __HW_H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
#include "r_cg_serial.h"
#include "r_cg_macrodriver.h"

/**
 * Load : Watchdog Timer Reset
*/
#define	RESET_WDT()						                  R_WDT_Restart()


/* SPI - VOICE IC */
#define  P_SPI_RESET                P10.0
#define  TURN_ON_SPI_RESET()        do{ P_SPI_RESET = 1; }while(0)
#define  TURN_OFF_SPI_RESET()       do{ P_SPI_RESET = 0; }while(0)

#define  P_SPI_CSB                  P14.7
#define  TURN_ON_SPI_CSB()          do{ P_SPI_CSB = 1; }while(0)
#define  TURN_OFF_SPI_CSB()         do{ P_SPI_CSB = 0; }while(0)

#define  P_SPI_CLK        P14.6
#define  TURN_ON_SPI_CLK()          do{ P_SPI_CLK = 1; }while(0)
#define  TURN_OFF_SPI_CLK()         do{ P_SPI_CLK = 0; }while(0)

#define  P_SPI_SDI        P11.1
#define  WRITE_SPI_SDI( val )       do{ P_SPI_SDI = val; }while(0)

#define  P_SPI_SEL        P10.0
#define  TURN_ON_SPI_SEL()          do{ P_SPI_SEL = 1; }while(0)
#define  TURN_OFF_SPI_SEL()         do{ P_SPI_SEL = 0; }while(0)


/**
 * Load : Model
*/

/**
 * Load : Lcd
*/

/**
 * Load : Comp
*/

/**
 * Load : Heater
*/

/**
 * Load : Ac Motor
*/

/**
 * Load : Step Motor
*/

/**
 * Load : Smps
*/

/**
 * Load : Level
*/

/**
 * Load : Valve
*/

/**
 * Load : Buzzer
*/

/**
 * Load : Sensor
*/

/**
 * Load : Ster (Uv, Ew)
*/


/**
 * Load : Key
*/
/* Complete */
#define P_KEY_ALL_LCOK                            P15.5
#define GET_STATUS_KEY_ALL_LOCK()                 (P_KEY_ALL_LCOK)
/* Complete */
#define P_KEY_COLD_OFF                            P15.3
#define GET_STATUS_KEY_COLD_OFF()                 (P_KEY_COLD_OFF)
/* Complete */
#define P_KEY_HOT_LOCK                            P2.4
#define GET_STATUS_KEY_HOT_LOCK()                 (P_KEY_HOT_LOCK)

#define P_KEY_ICE_WATER_LOCK                      P2.3
#define GET_STATUS_KEY_ICE_WATER_LOCK()           (P_KEY_ICE_WATER_LOCK)
/* Complete */
#define P_KEY_ICE_LOCK                            P4.1
#define GET_STATUS_KEY_ICE_LOCK()                 (P_KEY_ICE_LOCK)

#define P_KEY_HOT_OFF                             P2.5
#define GET_STATUS_KEY_HOT_OFF()                  (P_KEY_HOT_OFF)
/* Complete */
#define P_KEY_ICE_OFF                             P12.3
#define GET_STATUS_KEY_ICE_OFF()                  (P_KEY_ICE_OFF)

#define P_KEY_ICE_WATER_AMOUNT                    P2.7
#define GET_STATUS_KEY_ICE_WATER_AMOUNT()         (P_KEY_ICE_WATER_AMOUNT)
/* Complete */
#define P_KEY_HOT                                 P2.3
#define GET_STATUS_KEY_HOT()                      (P_KEY_HOT)
/* Complete */
#define P_KEY_ROOM                                P2.5
#define GET_STATUS_KEY_ROOM()                     (P_KEY_ROOM)
/* Complete */
#define P_KEY_COLD                                P2.7
#define GET_STATUS_KEY_COLD()                     (P_KEY_COLD)
/* Complete */
#define P_KEY_AMOUNT                              P15.1
#define GET_STATUS_KEY_AMOUNT()                   (P_KEY_AMOUNT)
/* Complete */
#define P_KEY_ICE                                 P12.2
#define GET_STATUS_KEY_ICE()                      (P_KEY_ICE)
/* Complete */
#define P_KEY_ICE_WATER                           P12.4
#define GET_STATUS_KEY_ICE_WATER()                (P_KEY_ICE_WATER)

/**
 * Load : Lever
*/
/* Complete */
#define P_LEVER_ICE                               P13.7
#define GET_STATUS_LEVER_ICE()                    (P_LEVER_ICE)
/* Complete */
#define P_LEVER_WATER                             P15.6
#define GET_STATUS_LEVER_WATER()                  (P_LEVER_WATER)

/**
 * Load : Led
*/
/* 고온살균 텍스트 - Complete */
#define P_LED_UV_HOT_STER_TXT                      P1.4
#define TURN_ON_LED_UV_HOT_STER_TXT()              do{ P_LED_UV_HOT_STER_TXT = 1; }while(0)
#define TURN_OFF_LED_UV_HOT_STER_TXT()             do{ P_LED_UV_HOT_STER_TXT = 0; }while(0)
#define GET_STATUS_LED_UV_HOT_STER_TXT()           (P_LED_UV_HOT_STER_TXT)

/* UV살균 텍스트 - Complete */
#define P_LED_UV_CARE_TXT                          P1.5
#define TURN_ON_LED_UV_CARE_SYSTEM()               do{ P_LED_UV_CARE_TXT = 1; }while(0)
#define TURN_OFF_LED_UV_CARE_SYSTEM()              do{ P_LED_UV_CARE_TXT = 0; }while(0)
#define GET_STATUS_LED_UV_CARE_SYSTEM()            (P_LED_UV_CARE_TXT)

/* UV 얼음저장고 - Complete */
#define P_LED_UV_ICE_TANK                          P8.5
#define TURN_ON_LED_UV_ICE_TANK()                  do{ P_LED_UV_ICE_TANK = 1; }while(0)
#define TURN_OFF_LED_UV_ICE_TANK()                 do{ P_LED_UV_ICE_TANK = 0; }while(0)
#define GET_STATUS_LED_UV_ICE_TANK()               (P_LED_UV_ICE_TANK)

/* UV 얼음트레이 - Complete */
#define P_LED_UV_ICE_TRAY                          P8.6
#define TURN_ON_LED_UV_ICE_TRAY()                  do{ P_LED_UV_ICE_TRAY = 1; }while(0)
#define TURN_OFF_LED_UV_ICE_TRAY()                 do{ P_LED_UV_ICE_TRAY = 0; }while(0)
#define GET_STATUS_LED_UV_ICE_TRAY()               (P_LED_UV_ICE_TRAY)

/* UV 얼음파우셋 - Complete */
#define P_LED_UV_ICE_FAUCET                        P5.3
#define TURN_ON_LED_UV_ICE_FAUCET()                do{ P_LED_UV_ICE_FAUCET = 1; }while(0)
#define TURN_OFF_LED_UV_ICE_FAUCET()               do{ P_LED_UV_ICE_FAUCET = 0; }while(0)
#define GET_STATUS_LED_UV_ICE_FAUCET()             (P_LED_UV_ICE_FAUCET)

/* UV 추출 파우셋 - Complete */
#define P_LED_UV_WATER_FAUCET                      P5.4
#define TURN_ON_LED_UV_WATER_FAUCET()              do{ P_LED_UV_WATER_FAUCET = 1; }while(0)
#define TURN_OFF_LED_UV_WATER_FAUCET()             do{ P_LED_UV_WATER_FAUCET = 0; }while(0)
#define GET_STATUS_LED_UV_WATER_FAUCET()           (P_LED_UV_WATER_FAUCET)

/* 온도단계 1 LED - Complete */
#define P_LED_TEMP_STEP_1                          P8.7
#define TURN_ON_LED_TEMP_STEP_1()                  do{ P_LED_TEMP_STEP_1 = 1; }while(0)
#define TURN_OFF_LED_TEMP_STEP_1()                 do{ P_LED_TEMP_STEP_1 = 0; }while(0)
#define GET_STATUS_LED_TEMP_STEP_1()               (P_LED_TEMP_STEP_1)

/* 온도단계 2 LED - Complete */
#define P_LED_TEMP_STEP_2                          P3.0
#define TURN_ON_LED_TEMP_STEP_2()                  do{ P_LED_TEMP_STEP_2 = 1; }while(0)
#define TURN_OFF_LED_TEMP_STEP_2()                 do{ P_LED_TEMP_STEP_2 = 0; }while(0)
#define GET_STATUS_LED_TEMP_STEP_2()               (P_LED_TEMP_STEP_2)

/* 온도단계 3 LED - Complete */
#define P_LED_TEMP_STEP_3                          P5.0
#define TURN_ON_LED_TEMP_STEP_3()                  do{ P_LED_TEMP_STEP_3 = 1; }while(0)
#define TURN_OFF_LED_TEMP_STEP_3()                 do{ P_LED_TEMP_STEP_3 = 0; }while(0)
#define GET_STATUS_LED_TEMP_STEP_3()               (P_LED_TEMP_STEP_3)

/* 온도단계 4 LED - Complete */
#define P_LED_TEMP_STEP_4                          P5.1
#define TURN_ON_LED_TEMP_STEP_4()                  do{ P_LED_TEMP_STEP_4 = 1; }while(0)
#define TURN_OFF_LED_TEMP_STEP_4()                 do{ P_LED_TEMP_STEP_4 = 0; }while(0)
#define GET_STATUS_LED_TEMP_STEP_4()               (P_LED_TEMP_STEP_4)

/* 온도단계 5 LED - Complete */
#define P_LED_TEMP_STEP_5                          P5.2
#define TURN_ON_LED_TEMP_STEP_5()                  do{ P_LED_TEMP_STEP_5 = 1; }while(0)
#define TURN_OFF_LED_TEMP_STEP_5()                 do{ P_LED_TEMP_STEP_5 = 0; }while(0)
#define GET_STATUS_LED_TEMP_STEP_5()               (P_LED_TEMP_STEP_5)

/* 얼음가득 - Complete */
#define P_LED_STATUS_ICE_FULL                      P1.6
#define TURN_ON_LED_STATUS_ICE_FULL()              do{ P_LED_STATUS_ICE_FULL = 1; }while(0)
#define TURN_OFF_LED_STATUS_ICE_FULL()             do{ P_LED_STATUS_ICE_FULL = 0; }while(0)
#define GET_STATUS_LED_STATUS_ICE_FULL()           (P_LED_STATUS_ICE_FULL)

/* 얼음부족 - Complete */
#define P_LED_STATUS_ICE_LACK                      P1.7
#define TURN_ON_LED_STATUS_ICE_LACK()              do{ P_LED_STATUS_ICE_LACK = 1; }while(0)
#define TURN_OFF_LED_STATUS_ICE_LACK()             do{ P_LED_STATUS_ICE_LACK = 0; }while(0)
#define GET_STATUS_LED_STATUS_ICE_LACK()           (P_LED_STATUS_ICE_LACK)



/* 'C - Complete */
#define P_LED_STATUS_UNIT_CELSIUS                  P9.2
#define TURN_ON_LED_STATUS_UNIT_CELSIUS()          do{ P_LED_STATUS_UNIT_CELSIUS = 1; }while(0)
#define TURN_OFF_LED_STATUS_UNIT_CELSIUS()         do{ P_LED_STATUS_UNIT_CELSIUS = 0; }while(0)
#define GET_STATUS_LED_STATUS_UNIT_CELSIUS()       (P_LED_STATUS_UNIT_CELSIUS)

/* COMPLETE */
#define P_LED_AMOUNT_BAR_1                         P11.7
#define TURN_ON_LED_STATUS_BAR_LOW()               do{ P_LED_AMOUNT_BAR_1 = 1; }while(0)
#define TURN_OFF_LED_STATUS_BAR_LOW()              do{ P_LED_AMOUNT_BAR_1 = 0; }while(0)
#define GET_STATUS_LED_STATUS_BAR_LOW()            (P_LED_AMOUNT_BAR_1)

/* COMPLETE */
#define P_LED_AMOUNT_BAR_2                         P10.1
#define TURN_ON_LED_STATUS_BAR_2()                 do{ P_LED_AMOUNT_BAR_2 = 1; }while(0)
#define TURN_OFF_LED_STATUS_BAR_2()                do{ P_LED_AMOUNT_BAR_2 = 0; }while(0)
#define GET_STATUS_LED_STATUS_BAR_2()              (P_LED_AMOUNT_BAR_2)

/* COMPLETE */
#define P_LED_AMOUNT_BAR_3                         P0.1
#define TURN_ON_LED_STATUS_BAR_MIDDLE()            do{ P_LED_AMOUNT_BAR_3 = 1; }while(0)
#define TURN_OFF_LED_STATUS_BAR_MIDDLE()           do{ P_LED_AMOUNT_BAR_3 = 0; }while(0)
#define GET_STATUS_LED_STATUS_BAR_MIDDLE()         (P_LED_AMOUNT_BAR_3)

/* COMPLETE */
#define P_LED_AMOUNT_BAR_4                         P0.0
#define TURN_ON_LED_STATUS_BAR_HIGH()              do{ P_LED_AMOUNT_BAR_4 = 1; }while(0)
#define TURN_OFF_LED_STATUS_BAR_HIGH()             do{ P_LED_AMOUNT_BAR_4 = 0; }while(0)
#define GET_STATUS_LED_STATUS_BAR_HIGH()           (P_LED_AMOUNT_BAR_4)

/* COMPLETE */
#define P_LED_STATUS_BAR_INFINITE                  P9.3
#define TURN_ON_LED_STATUS_BAR_INFINITE()          do{ P_LED_STATUS_BAR_INFINITE = 1; }while(0)
#define TURN_OFF_LED_STATUS_BAR_INFINITE()         do{ P_LED_STATUS_BAR_INFINITE = 0; }while(0)
#define GET_STATUS_LED_STATUS_BAR_INFINITE()       (P_LED_STATUS_BAR_INFINITE)

/* COMPLETE */
#define P_LED_STATUS_UNIT_ML                       P11.5
#define TURN_ON_LED_STATUS_UNIT_ML()               do{ P_LED_STATUS_UNIT_ML = 1; }while(0)
#define TURN_OFF_LED_STATUS_UNIT_ML()              do{ P_LED_STATUS_UNIT_ML = 0; }while(0)
#define GET_STATUS_LED_STATUS_UNIT_ML()            (P_LED_STATUS_UNIT_ML)

/* COMPLETE */
#define P_LED_STATUS_UNIT_L                        P11.6
#define TURN_ON_LED_STATUS_UNIT_L()                do{ P_LED_STATUS_UNIT_ML = 1; }while(0)
#define TURN_OFF_LED_STATUS_UNIT_L()               do{ P_LED_STATUS_UNIT_ML = 0; }while(0)
#define GET_STATUS_LED_STATUS_UNIT_L()             (P_LED_STATUS_UNIT_ML)

/* Right Water Out Faucet (Complete) */
#define P_LED_EXTRACT_RIGHT_BLUE                   P12.7
#define TURN_ON_LED_EXTRACT_RIGHT_BLUE()           do{ P_LED_EXTRACT_RIGHT_BLUE = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_RIGHT_BLUE()          do{ P_LED_EXTRACT_RIGHT_BLUE = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_RIGHT_BLUE()        (P_LED_EXTRACT_RIGHT_BLUE)

#define P_LED_EXTRACT_RIGHT_GREEN                  P12.6
#define TURN_ON_LED_EXTRACT_RIGHT_GREEN()          do{ P_LED_EXTRACT_RIGHT_GREEN = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_RIGHT_GREEN()         do{ P_LED_EXTRACT_RIGHT_GREEN = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_RIGHT_GREEN()       (P_LED_EXTRACT_RIGHT_GREEN)

#define P_LED_EXTRACT_RIGHT_RED                    P12.5
#define TURN_ON_LED_EXTRACT_RIGHT_RED()            do{ P_LED_EXTRACT_RIGHT_RED = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_RIGHT_RED()           do{ P_LED_EXTRACT_RIGHT_RED = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_RIGHT_RED()         (P_LED_EXTRACT_RIGHT_RED)

#define P_LED_EXTRACT_RIGHT_WHITE                  P14.2
#define TURN_ON_LED_EXTRACT_RIGHT_WHITE()          do{ P_LED_EXTRACT_RIGHT_WHITE = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_RIGHT_WHITE()         do{ P_LED_EXTRACT_RIGHT_WHITE = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_RIGHT_WHITE()       (P_LED_EXTRACT_RIGHT_WHITE)

/* Left Ice Out Faucet (Outer) (Complete) */
#define P_LED_EXTRACT_LEFT_BLUE                   P4.4
#define TURN_ON_LED_EXTRACT_LEFT_BLUE()           do{ P_LED_EXTRACT_LEFT_BLUE = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_LEFT_BLUE()          do{ P_LED_EXTRACT_LEFT_BLUE = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_LEFT_BLUE()        (P_LED_EXTRACT_LEFT_BLUE)

#define P_LED_EXTRACT_LEFT_GREEN                  P4.3
#define TURN_ON_LED_EXTRACT_LEFT_GREEN()          do{ P_LED_EXTRACT_LEFT_GREEN = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_LEFT_GREEN()         do{ P_LED_EXTRACT_LEFT_GREEN = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_LEFT_GREEN()       (P_LED_EXTRACT_LEFT_GREEN)

#define P_LED_EXTRACT_LEFT_RED                    P4.2
#define TURN_ON_LED_EXTRACT_LEFT_RED()            do{ P_LED_EXTRACT_LEFT_RED = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_LEFT_RED()           do{ P_LED_EXTRACT_LEFT_RED = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_LEFT_RED()         (P_LED_EXTRACT_LEFT_RED)

#define P_LED_EXTRACT_LEFT_WHITE                  P7.1
#define TURN_ON_LED_EXTRACT_LEFT_WHITE()          do{ P_LED_EXTRACT_LEFT_WHITE = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_LEFT_WHITE()         do{ P_LED_EXTRACT_LEFT_WHITE = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_LEFT_WHITE()       (P_LED_EXTRACT_LEFT_WHITE)

/* Left Ice Out Faucet (Inner) (Complete) */
#define P_LED_EXTRACT_LEFT_BLUE_I                   P4.5
#define TURN_ON_LED_EXTRACT_LEFT_BLUE_I()           do{ P_LED_EXTRACT_LEFT_BLUE_I = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_LEFT_BLUE_I()          do{ P_LED_EXTRACT_LEFT_BLUE_I = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_LEFT_BLUE_I()        (P_LED_EXTRACT_LEFT_BLUE_I)

#define P_LED_EXTRACT_LEFT_GREEN_I                  P4.6
#define TURN_ON_LED_EXTRACT_LEFT_GREEN_I()          do{ P_LED_EXTRACT_LEFT_GREEN_I = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_LEFT_GREEN_I()         do{ P_LED_EXTRACT_LEFT_GREEN_I = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_LEFT_GREEN_I()       (P_LED_EXTRACT_LEFT_GREEN_I)

#define P_LED_EXTRACT_LEFT_RED_I                    P4.7
#define TURN_ON_LED_EXTRACT_LEFT_RED_I()            do{ P_LED_EXTRACT_LEFT_RED_I = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_LEFT_RED_I()           do{ P_LED_EXTRACT_LEFT_RED_I = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_LEFT_RED_I()         (P_LED_EXTRACT_LEFT_RED_I)

#define P_LED_EXTRACT_LEFT_WHITE_I                  P6.7
#define TURN_ON_LED_EXTRACT_LEFT_WHITE_I()          do{ P_LED_EXTRACT_LEFT_WHITE_I = 1; }while(0)
#define TURN_OFF_LED_EXTRACT_LEFT_WHITE_I()         do{ P_LED_EXTRACT_LEFT_WHITE_I = 0; }while(0)
#define GET_STATUS_LED_EXTRACT_LEFT_WHITE_I()       (P_LED_EXTRACT_LEFT_WHITE_I)

/* Complete */
#define P_LED_AMOUNT                              P3.6
#define TURN_ON_LED_AMOUNT()                      do{ P_LED_AMOUNT = 1; }while(0)
#define TURN_OFF_LED_AMOUNT()                     do{ P_LED_AMOUNT = 0; }while(0)
#define GET_STATUS_LED_AMOUNT()                   (P_LED_AMOUNT)

/* Complete */
#define P_LED_COLD                                P3.3
#define TURN_ON_LED_COLD()                        do{ P_LED_COLD = 1; }while(0)
#define TURN_OFF_LED_COLD()                       do{ P_LED_COLD = 0; }while(0)
#define GET_STATUS_LED_COLD()                     (P_LED_COLD)

/* Complete */
#define P_LED_ROOM                                P3.2
#define TURN_ON_LED_ROOM()                        do{ P_LED_ROOM = 1; }while(0)
#define TURN_OFF_LED_ROOM()                       do{ P_LED_ROOM = 0; }while(0)
#define GET_STATUS_LED_ROOM()                     (P_LED_ROOM)

/* Complete */
#define P_LED_HOT                                 P2.3
#define TURN_ON_LED_HOT()                         do{ P_LED_HOT = 1; }while(0)
#define TURN_OFF_LED_HOT()                        do{ P_LED_HOT = 0; }while(0)
#define GET_STATUS_LED_HOT()                      (P_LED_HOT)

/* Complete */
#define P_LED_ICE_WATER                           P7.4
#define TURN_ON_LED_ICE_WATER()                   do{ P_LED_ICE_WATER = 1; }while(0)
#define TURN_OFF_LED_ICE_WATER()                  do{ P_LED_ICE_WATER = 0; }while(0)
#define GET_STATUS_LED_ICE_WATER()                (P_LED_ICE_WATER)

/* Complete */
#define P_LED_ICE                                 P7.3
#define TURN_ON_LED_ICE()                         do{ P_LED_ICE = 1; }while(0)
#define TURN_OFF_LED_ICE()                        do{ P_LED_ICE = 0; }while(0)
#define GET_STATUS_LED_ICE()                      (P_LED_ICE)

/* Complete */
#define P_LED_ALL_LOCK                            P14.1
#define TURN_ON_LED_ALL_LOCK()                    do{ P_LED_ALL_LOCK = 1; }while(0)
#define TURN_OFF_LED_ALL_LOCK()                   do{ P_LED_ALL_LOCK = 0; }while(0)
#define GET_STATUS_LED_ALL_LOCK()                 (P_LED_ALL_LOCK)

// 1. FAST_ICE / P14.5
#define P_FAST_ICE                P14.5
#define TURN_ON_FAST_ICE()        do{ P_FAST_ICE = 1; }while(0)
#define TURN_OFF_FAST_ICE()       do{ P_FAST_ICE = 0; }while(0)
#define GET_STATUS_FAST_ICE()     (P_FAST_ICE)

// 2. SLEEP_ICON / P5.7
#define P_SLEEP_ICON                P5.7
#define TURN_ON_SLEEP_ICON()        do{ P_SLEEP_ICON = 1; }while(0)
#define TURN_OFF_SLEEP_ICON()       do{ P_SLEEP_ICON = 0; }while(0)
#define GET_STATUS_SLEEP_ICON()     (P_SLEEP_ICON)

// 3. COLON / P5.5
#define P_COLON                P5.5
#define TURN_ON_COLON()        do{ P_COLON = 1; }while(0)
#define TURN_OFF_COLON()       do{ P_COLON = 0; }while(0)
#define GET_STATUS_COLON()     (P_COLON)

// 4. RAMEN / P9.1
#define P_RAMEN                P9.1
#define TURN_ON_RAMEN()        do{ P_RAMEN = 1; }while(0)
#define TURN_OFF_RAMEN()       do{ P_RAMEN = 0; }while(0)
#define GET_STATUS_RAMEN()     (P_RAMEN)

// 5. DRIP_COFFEE / P9.0
#define P_DRIP_COFFEE                P9.0
#define TURN_ON_DRIP_COFFEE()        do{ P_DRIP_COFFEE = 1; }while(0)
#define TURN_OFF_DRIP_COFFEE()       do{ P_DRIP_COFFEE = 0; }while(0)
#define GET_STATUS_DRIP_COFFEE()     (P_DRIP_COFFEE)

// 6. TEA / P1.0
#define P_TEA                P1.0
#define TURN_ON_TEA()        do{ P_TEA = 1; }while(0)
#define TURN_OFF_TEA()       do{ P_TEA = 0; }while(0)
#define GET_STATUS_TEA()     (P_TEA)

// 7. MY1 / P1.1
#define P_MY1                P1.1
#define TURN_ON_MY1()        do{ P_MY1 = 1; }while(0)
#define TURN_OFF_MY1()       do{ P_MY1 = 0; }while(0)
#define GET_STATUS_MY1()     (P_MY1)

// 8. MY2 / P1.2
#define P_MY2                P1.2
#define TURN_ON_MY2()        do{ P_MY2 = 1; }while(0)
#define TURN_OFF_MY2()       do{ P_MY2 = 0; }while(0)
#define GET_STATUS_MY2()     (P_MY2)

// 9. MY3 / P1.3
#define P_MY3                P1.3
#define TURN_ON_MY3()        do{ P_MY3 = 1; }while(0)
#define TURN_OFF_MY3()       do{ P_MY3 = 0; }while(0)
#define GET_STATUS_MY3()     (P_MY3)

// 10. SEG DOT / P11.4
#define P_SEG_DOT                P11.4
#define TURN_ON_SEG_DOT()        do{ P_SEG_DOT = 1; }while(0)
#define TURN_OFF_SEG_DOT()       do{ P_SEG_DOT = 0; }while(0)
#define GET_STATUS_SEG_DOT()     (P_SEG_DOT)

// 11. FILTER_CLEAN_TXT / P11.3
#define P_FILTER_CLEAN_TXT                P11.3
#define TURN_ON_FILTER_CLEAN_TXT()        do{ P_FILTER_CLEAN_TXT = 1; }while(0)
#define TURN_OFF_FILTER_CLEAN_TXT()       do{ P_FILTER_CLEAN_TXT = 0; }while(0)
#define GET_STATUS_FILTER_CLEAN_TXT()     (P_FILTER_CLEAN_TXT)

// 12. WIFI / P11.2
#define P_WIFI                P11.2
#define TURN_ON_WIFI()        do{ P_WIFI = 1; }while(0)
#define TURN_OFF_WIFI()       do{ P_WIFI = 0; }while(0)
#define GET_STATUS_WIFI()     (P_WIFI)

// 13. SUNNY / P9.7
#define P_SUNNY                P9.7
#define TURN_ON_SUNNY()        do{ P_SUNNY = 1; }while(0)
#define TURN_OFF_SUNNY()       do{ P_SUNNY = 0; }while(0)
#define GET_STATUS_SUNNY()     (P_SUNNY)

// 14. CLOUD / P9.6
#define P_CLOUD                P9.6
#define TURN_ON_CLOUD()        do{ P_CLOUD = 1; }while(0)
#define TURN_OFF_CLOUD()       do{ P_CLOUD = 0; }while(0)
#define GET_STATUS_CLOUD()     (P_CLOUD)

// 15. RAIN / P9.5
#define P_RAIN                P9.5
#define TURN_ON_RAIN()        do{ P_RAIN = 1; }while(0)
#define TURN_OFF_RAIN()       do{ P_RAIN = 0; }while(0)
#define GET_STATUS_RAIN()     (P_RAIN)

// 16. PERCENT / P9.4
#define P_PERCENT                P9.4
#define TURN_ON_PERCENT()        do{ P_PERCENT = 1; }while(0)
#define TURN_OFF_PERCENT()       do{ P_PERCENT = 0; }while(0)
#define GET_STATUS_PERCENT()     (P_PERCENT)

// 17. ICE_WATER_KEY / P7.4
#define P_ICE_WATER_KEY                P7.4
#define TURN_ON_ICE_WATER_KEY()        do{ P_ICE_WATER_KEY = 1; }while(0)
#define TURN_OFF_ICE_WATER_KEY()       do{ P_ICE_WATER_KEY = 0; }while(0)
#define GET_STATUS_ICE_WATER_KEY()     (P_ICE_WATER_KEY)

// 18. ICE_KEY / P7.3
#define P_ICE_KEY                P7.3
#define TURN_ON_ICE_KEY()        do{ P_ICE_KEY = 1; }while(0)
#define TURN_OFF_ICE_KEY()       do{ P_ICE_KEY = 0; }while(0)
#define GET_STATUS_ICE_KEY()     (P_ICE_KEY)

// 19. HOT_WATER_KEY / P3.1
#define P_HOT_WATER_KEY                P3.1
#define TURN_ON_HOT_WATER_KEY()        do{ P_HOT_WATER_KEY = 1; }while(0)
#define TURN_OFF_HOT_WATER_KEY()       do{ P_HOT_WATER_KEY = 0; }while(0)
#define GET_STATUS_HOT_WATER_KEY()     (P_HOT_WATER_KEY)

// 20. ROOM_WATER_KEY / P3.2
#define P_ROOM_WATER_KEY                P3.2
#define TURN_ON_ROOM_WATER_KEY()        do{ P_ROOM_WATER_KEY = 1; }while(0)
#define TURN_OFF_ROOM_WATER_KEY()       do{ P_ROOM_WATER_KEY = 0; }while(0)
#define GET_STATUS_ROOM_WATER_KEY()     (P_ROOM_WATER_KEY)

// 21. COLD_WATER_KEY / P3.3
#define P_COLD_WATER_KEY                P3.3
#define TURN_ON_COLD_WATER_KEY()        do{ P_COLD_WATER_KEY = 1; }while(0)
#define TURN_OFF_COLD_WATER_KEY()       do{ P_COLD_WATER_KEY = 0; }while(0)
#define GET_STATUS_COLD_WATER_KEY()     (P_COLD_WATER_KEY)

// 22. AMOUNT_KEY / P3.6
#define P_AMOUNT_KEY                P3.6
#define TURN_ON_AMOUNT_KEY()        do{ P_AMOUNT_KEY = 1; }while(0)
#define TURN_OFF_AMOUNT_KEY()       do{ P_AMOUNT_KEY = 0; }while(0)
#define GET_STATUS_AMOUNT_KEY()     (P_AMOUNT_KEY)

// 23. MY_KEY / P3.7
#define P_MY_KEY                P3.7
#define TURN_ON_MY_KEY()        do{ P_MY_KEY = 1; }while(0)
#define TURN_OFF_MY_KEY()       do{ P_MY_KEY = 0; }while(0)
#define GET_STATUS_MY_KEY()     (P_MY_KEY)

// 24. ICE_TYPE_NORMAL / P8.4
#define P_ICE_TYPE_NORMAL                P8.4
#define TURN_ON_ICE_TYPE_NORMAL()        do{ P_ICE_TYPE_NORMAL = 1; }while(0)
#define TURN_OFF_ICE_TYPE_NORMAL()       do{ P_ICE_TYPE_NORMAL = 0; }while(0)
#define GET_STATUS_ICE_TYPE_NORMAL()     (P_ICE_TYPE_NORMAL)

// 25. ICE_TYPE_PIECE / P8.3
#define P_ICE_TYPE_PIECE                P8.3
#define TURN_ON_ICE_TYPE_PIECE()        do{ P_ICE_TYPE_PIECE = 1; }while(0)
#define TURN_OFF_ICE_TYPE_PIECE()       do{ P_ICE_TYPE_PIECE = 0; }while(0)
#define GET_STATUS_ICE_TYPE_PIECE()     (P_ICE_TYPE_PIECE)

// 26. ICE_LOCKER / P8.2
#define P_ICE_LOCKER                P8.2
#define TURN_ON_ICE_LOCKER()        do{ P_ICE_LOCKER = 1; }while(0)
#define TURN_OFF_ICE_LOCKER()       do{ P_ICE_LOCKER = 0; }while(0)
#define GET_STATUS_ICE_LOCKER()     (P_ICE_LOCKER)

// 27. ICE_HOT_WATER_LOCKER / P7.5
#define P_ICE_HOT_WATER_LOCKER                P7.5
#define TURN_ON_ICE_HOT_WATER_LOCKER()        do{ P_ICE_HOT_WATER_LOCKER = 1; }while(0)
#define TURN_OFF_ICE_HOT_WATER_LOCKER()       do{ P_ICE_HOT_WATER_LOCKER = 0; }while(0)
#define GET_STATUS_ICE_HOT_WATER_LOCKER()     (P_ICE_HOT_WATER_LOCKER)

// 28. ICE_COMP_WHITE / P10.6
#define P_ICE_COMP_WHITE                P10.6
#define TURN_ON_ICE_COMP_WHITE()        do{ P_ICE_COMP_WHITE = 1; }while(0)
#define TURN_OFF_ICE_COMP_WHITE()       do{ P_ICE_COMP_WHITE = 0; }while(0)
#define GET_STATUS_ICE_COMP_WHITE()     (P_ICE_COMP_WHITE)

// 29 ICE_COMP_BLUE / P10.3
#define P_ICE_COMP_BLUE                P10.3
#define TURN_ON_ICE_COMP_BLUE()        do{ P_ICE_COMP_BLUE = 1; }while(0)
#define TURN_OFF_ICE_COMP_BLUE()       do{ P_ICE_COMP_BLUE = 0; }while(0)
#define GET_STATUS_ICE_COMP_BLUE()     (P_ICE_COMP_BLUE)

// 31. ICE_SIZE_KEY / P12.1
#define P_ICE_SIZE_KEY                P12.1
#define TURN_ON_ICE_SIZE_KEY()        do{ P_ICE_SIZE_KEY = 1; }while(0)
#define TURN_OFF_ICE_SIZE_KEY()       do{ P_ICE_SIZE_KEY = 0; }while(0)
#define GET_STATUS_ICE_SIZE_KEY()     (P_ICE_SIZE_KEY)

// 32. ICE_BIG_ICON / P8.1
#define P_ICE_BIG_ICON                P8.1
#define TURN_ON_ICE_BIG_ICON()        do{ P_ICE_BIG_ICON = 1; }while(0)
#define TURN_OFF_ICE_BIG_ICON()       do{ P_ICE_BIG_ICON = 0; }while(0)
#define GET_STATUS_ICE_BIG_ICON()     (P_ICE_BIG_ICON)

// 33. ICE_SMALL_ICON / P0.5
#define P_ICE_SMALL_ICON                P0.5
#define TURN_ON_ICE_SMALL_ICON()        do{ P_ICE_SMALL_ICON = 1; }while(0)
#define TURN_OFF_ICE_SMALL_ICON()       do{ P_ICE_SMALL_ICON = 0; }while(0)
#define GET_STATUS_ICE_SMALL_ICON()     (P_ICE_SMALL_ICON)

// 34. ICE_LINE / P8.0
#define P_ICE_LINE                P8.0
#define TURN_ON_ICE_LINE()        do{ P_ICE_LINE = 1; }while(0)
#define TURN_OFF_ICE_LINE()       do{ P_ICE_LINE = 0; }while(0)
#define GET_STATUS_ICE_LINE()     (P_ICE_LINE)

// 35. HOT_WATER_LOCK_KEY / P6.5
#define P_HOT_WATER_LOCK_KEY                P6.5
#define TURN_ON_HOT_WATER_LOCK_KEY()        do{ P_HOT_WATER_LOCK_KEY = 1; }while(0)
#define TURN_OFF_HOT_WATER_LOCK_KEY()       do{ P_HOT_WATER_LOCK_KEY = 0; }while(0)
#define GET_STATUS_HOT_WATER_LOCK_KEY()     (P_HOT_WATER_LOCK_KEY)

// 36. FAST_ICE_KEY / P6.6
#define P_FAST_ICE_KEY                P6.6
#define TURN_ON_FAST_ICE_KEY()        do{ P_FAST_ICE_KEY = 1; }while(0)
#define TURN_OFF_FAST_ICE_KEY()       do{ P_FAST_ICE_KEY = 0; }while(0)
#define GET_STATUS_FAST_ICE_KEY()     (P_FAST_ICE_KEY)

// 37. COLD_TEMP / P3.5
#define P_COLD_TEMP                P3.5
#define TURN_ON_COLD_TEMP()        do{ P_COLD_TEMP = 1; }while(0)
#define TURN_OFF_COLD_TEMP()       do{ P_COLD_TEMP = 0; }while(0)
#define GET_STATUS_COLD_TEMP()     (P_COLD_TEMP)

// 38. COLD_TEMP_WEAK / P3.4
#define P_COLD_TEMP_WEAK                P3.4
#define TURN_ON_COLD_TEMP_WEAK()        do{ P_COLD_TEMP_WEAK = 1; }while(0)
#define TURN_OFF_COLD_TEMP_WEAK()       do{ P_COLD_TEMP_WEAK = 0; }while(0)
#define GET_STATUS_COLD_TEMP_WEAK()     (P_COLD_TEMP_WEAK)

// 39. COLD_TEMP_STRONG / P10.5
#define P_COLD_TEMP_STRONG                P10.5
#define TURN_ON_COLD_TEMP_STRONG()        do{ P_COLD_TEMP_STRONG = 1; }while(0)
#define TURN_OFF_COLD_TEMP_STRONG()       do{ P_COLD_TEMP_STRONG = 0; }while(0)
#define GET_STATUS_COLD_TEMP_STRONG()     (P_COLD_TEMP_STRONG)

// 40. COLD_TEMP_LINE / P10.4
#define P_COLD_TEMP_LINE                P10.4
#define TURN_ON_COLD_TEMP_LINE()        do{ P_COLD_TEMP_LINE = 1; }while(0)
#define TURN_OFF_COLD_TEMP_LINE()       do{ P_COLD_TEMP_LINE = 0; }while(0)
#define GET_STATUS_COLD_TEMP_LINE()     (P_COLD_TEMP_LINE)

// 41. COLD_OFF_KEY / P12.0
#define P_COLD_OFF_KEY                P12.0
#define TURN_ON_COLD_OFF_KEY()        do{ P_COLD_OFF_KEY = 1; }while(0)
#define TURN_OFF_COLD_OFF_KEY()       do{ P_COLD_OFF_KEY = 0; }while(0)
#define GET_STATUS_COLD_OFF_KEY()     (P_COLD_OFF_KEY)

// 42. SLEEP_MODE_KEY / P14.0
#define P_SLEEP_MODE_KEY                P14.0
#define TURN_ON_SLEEP_MODE_KEY()        do{ P_SLEEP_MODE_KEY = 1; }while(0)
#define TURN_OFF_SLEEP_MODE_KEY()       do{ P_SLEEP_MODE_KEY = 0; }while(0)
#define GET_STATUS_SLEEP_MODE_KEY()     (P_SLEEP_MODE_KEY)

// 43. ALL_LOCK_KEY / P15.5
#define P_ALL_LOCK_KEY                P15.5
#define TURN_ON_ALL_LOCK_KEY()        do{ P_ALL_LOCK_KEY = 1; }while(0)
#define TURN_OFF_ALL_LOCK_KEY()       do{ P_ALL_LOCK_KEY = 0; }while(0)
#define GET_STATUS_ALL_LOCK_KEY()     (P_ALL_LOCK_KEY)

// 44. ICE_LOCK_KEY / P7.0
#define P_ICE_LOCK_KEY                P7.0
#define TURN_ON_ICE_LOCK_KEY()        do{ P_ICE_LOCK_KEY = 1; }while(0)
#define TURN_OFF_ICE_LOCK_KEY()       do{ P_ICE_LOCK_KEY = 0; }while(0)
#define GET_STATUS_ICE_LOCK_KEY()     (P_ICE_LOCK_KEY)

/* Complete */
#define P_LED_WELCOME_LEFT                                 P14.3
#define TURN_ON_LED_WELCOME_LEFT()                         do{ P_LED_WELCOME_LEFT = 1; }while(0)
#define TURN_OFF_LED_WELCOME_LEFT()                        do{ P_LED_WELCOME_LEFT = 0; }while(0)
#define GET_STATUS_LED_WELCOME_LEFT()                      (P_LED_WELCOME_LEFT)

/* Complete */
#define P_LED_WELCOME_RIGHT                                 P14.4
#define TURN_ON_LED_WELCOME_RIGHT()                         do{ P_LED_WELCOME_RIGHT = 1; }while(0)
#define TURN_OFF_LED_WELCOME_RIGHT()                        do{ P_LED_WELCOME_RIGHT = 0; }while(0)
#define GET_STATUS_LED_WELCOME_RIGHT()                      (P_LED_WELCOME_RIGHT)

/* Complete - 얼음OFF */
#define P_LED_ICE_OFF                                       P6.4
#define TURN_ON_LED_ICE_OFF()                               do{ P_LED_ICE_OFF = 1; }while(0)
#define TURN_OFF_LED_ICE_OFF()                              do{ P_LED_ICE_OFF = 0; }while(0)
#define GET_STATUS_LED_ICE_OFF()                            (P_LED_ICE_OFF)

/* Complete 얼음크기 버튼 */
#define P_LED_ICE_SIZE_SEL                                       P0.6
#define TURN_ON_LED_ICE_SIZE_SEL()                               do{ P_LED_ICE_SIZE_SEL = 1; }while(0)
#define TURN_OFF_LED_ICE_SIZE_SEL()                              do{ P_LED_ICE_SIZE_SEL = 0; }while(0)
#define GET_STATUS_LED_ICE_SIZE_SEL()                            (P_LED_ICE_SIZE_SEL)

/**
 * Load : Uv
*/

/**
 * Load : Fan
*/

/**
 * Load : Leak
*/

/**
 * Load : WiFi
*/


/**
 * Load : Unused Pin
*/
// #define P_UNUSED_OUT_1                       P14.3
// #define P_UNUSED_OUT_2                       P14.4
// #define P_UNUSED_OUT_3                       P14.5
// #define P_UNUSED_OUT_4                       P0.0
// #define P_UNUSED_OUT_5                       P0.1
// #define P_UNUSED_OUT_6                       P0.4
// #define P_UNUSED_OUT_7                       P0.7
// #define P_UNUSED_OUT_8                       P10.2
// #define P_UNUSED_OUT_9                       P13.0
// #define P_UNUSED_OUT_10                      P15.6
// #define P_UNUSED_OUT_11                      P10.0
// #define P_UNUSED_OUT_12                      P14.7
// #define P_UNUSED_OUT_13                      P14.6
// #define P_UNUSED_OUT_14                      P8.1
// #define P_UNUSED_OUT_15                      P8.0
// #define P_UNUSED_OUT_16                      P7.6
// #define P_UNUSED_OUT_17                      P7.7
// #define P_UNUSED_OUT_18                      P6.7
// #define P_UNUSED_OUT_19                      P6.6
// #define P_UNUSED_OUT_20                      P6.5
// #define P_UNUSED_OUT_21                      P6.4
// #define P_UNUSED_OUT_22                      P3.1
// #define P_UNUSED_OUT_23                      P6.3
// #define P_UNUSED_OUT_24                      P6.2
// #define P_UNUSED_OUT_25                      P12.1
// #define P_UNUSED_OUT_26                      P12.2
// #define P_UNUSED_OUT_27                      P13.7
// #define P_UNUSED_OUT_28                      P12.3
// #define P_UNUSED_OUT_29                      P12.4
// #define P_UNUSED_OUT_30                      P12.5

#endif    /* __HW_H__ */
