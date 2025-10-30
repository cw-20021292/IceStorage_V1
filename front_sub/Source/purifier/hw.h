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
/***********************************************************************************************************************/
#define     P_LED_7SEG_1A           P0.1
#define     TURN_ON_LED_7SEG_1A()   do{ P_LED_7SEG_1A = 1; }while(0)
#define     TURN_OFF_LED_7SEG_1A()  do{ P_LED_7SEG_1A = 0; }while(0)
#define     GET_STATUS_LED_7SEG_1A() (P_LED_7SEG_1A)

#define     P_LED_7SEG_1B           P0.4
#define     TURN_ON_LED_7SEG_1B()   do{ P_LED_7SEG_1B = 1; }while(0)
#define     TURN_OFF_LED_7SEG_1B()  do{ P_LED_7SEG_1B = 0; }while(0)
#define     GET_STATUS_LED_7SEG_1B() (P_LED_7SEG_1B)

#define     P_LED_7SEG_1C           P13.0
#define     TURN_ON_LED_7SEG_1C()   do{ P_LED_7SEG_1C = 1; }while(0)
#define     TURN_OFF_LED_7SEG_1C()  do{ P_LED_7SEG_1C = 0; }while(0)
#define     GET_STATUS_LED_7SEG_1C() (P_LED_7SEG_1C)

#define     P_LED_7SEG_1D           P14.7
#define     TURN_ON_LED_7SEG_1D()   do{ P_LED_7SEG_1D = 1; }while(0)
#define     TURN_OFF_LED_7SEG_1D()  do{ P_LED_7SEG_1D = 0; }while(0)
#define     GET_STATUS_LED_7SEG_1D() (P_LED_7SEG_1D)

#define     P_LED_7SEG_1E           P14.6
#define     TURN_ON_LED_7SEG_1E()   do{ P_LED_7SEG_1E = 1; }while(0)
#define     TURN_OFF_LED_7SEG_1E()  do{ P_LED_7SEG_1E = 0; }while(0)
#define     GET_STATUS_LED_7SEG_1E() (P_LED_7SEG_1E)

#define     P_LED_7SEG_1F           P1.0
#define     TURN_ON_LED_7SEG_1F()   do{ P_LED_7SEG_1F = 1; }while(0)
#define     TURN_OFF_LED_7SEG_1F()  do{ P_LED_7SEG_1F = 0; }while(0)
#define     GET_STATUS_LED_7SEG_1F() (P_LED_7SEG_1F)

#define     P_LED_7SEG_1G           P1.1
#define     TURN_ON_LED_7SEG_1G()   do{ P_LED_7SEG_1G = 1; }while(0)
#define     TURN_OFF_LED_7SEG_1G()  do{ P_LED_7SEG_1G = 0; }while(0)
#define     GET_STATUS_LED_7SEG_1G() (P_LED_7SEG_1G)
/***********************************************************************************************************************/
#define     P_LED_7SEG_2A           P1.2
#define     TURN_ON_LED_7SEG_2A()   do{ P_LED_7SEG_2A = 1; }while(0)
#define     TURN_OFF_LED_7SEG_2A()  do{ P_LED_7SEG_2A = 0; }while(0)
#define     GET_STATUS_LED_7SEG_2A() (P_LED_7SEG_2A)

#define     P_LED_7SEG_2B           P1.3
#define     TURN_ON_LED_7SEG_2B()   do{ P_LED_7SEG_2B = 1; }while(0)
#define     TURN_OFF_LED_7SEG_2B()  do{ P_LED_7SEG_2B = 0; }while(0)
#define     GET_STATUS_LED_7SEG_2B() (P_LED_7SEG_2B)

#define     P_LED_7SEG_2C           P1.4
#define     TURN_ON_LED_7SEG_2C()   do{ P_LED_7SEG_2C = 1; }while(0)
#define     TURN_OFF_LED_7SEG_2C()  do{ P_LED_7SEG_2C = 0; }while(0)
#define     GET_STATUS_LED_7SEG_2C() (P_LED_7SEG_2C)

#define     P_LED_7SEG_2D           P1.5
#define     TURN_ON_LED_7SEG_2D()   do{ P_LED_7SEG_2D = 1; }while(0)
#define     TURN_OFF_LED_7SEG_2D()  do{ P_LED_7SEG_2D = 0; }while(0)
#define     GET_STATUS_LED_7SEG_2D() (P_LED_7SEG_2D)

#define     P_LED_7SEG_2E           P1.6
#define     TURN_ON_LED_7SEG_2E()   do{ P_LED_7SEG_2E = 1; }while(0)
#define     TURN_OFF_LED_7SEG_2E()  do{ P_LED_7SEG_2E = 0; }while(0)
#define     GET_STATUS_LED_7SEG_2E() (P_LED_7SEG_2E)

#define     P_LED_7SEG_2F           P1.7
#define     TURN_ON_LED_7SEG_2F()   do{ P_LED_7SEG_2F = 1; }while(0)
#define     TURN_OFF_LED_7SEG_2F()  do{ P_LED_7SEG_2F = 0; }while(0)
#define     GET_STATUS_LED_7SEG_2F() (P_LED_7SEG_2F)

#define     P_LED_7SEG_2G           P5.5
#define     TURN_ON_LED_7SEG_2G()   do{ P_LED_7SEG_2G = 1; }while(0)
#define     TURN_OFF_LED_7SEG_2G()  do{ P_LED_7SEG_2G = 0; }while(0)
#define     GET_STATUS_LED_7SEG_2G() (P_LED_7SEG_2G)
/***********************************************************************************************************************/
#define     P_LED_7SEG_3A           P5.4
#define     TURN_ON_LED_7SEG_3A()   do{ P_LED_7SEG_3A = 1; }while(0)
#define     TURN_OFF_LED_7SEG_3A()  do{ P_LED_7SEG_3A = 0; }while(0)
#define     GET_STATUS_LED_7SEG_3A() (P_LED_7SEG_3A)

#define     P_LED_7SEG_3B           P5.3
#define     TURN_ON_LED_7SEG_3B()   do{ P_LED_7SEG_3B = 1; }while(0)
#define     TURN_OFF_LED_7SEG_3B()  do{ P_LED_7SEG_3B = 0; }while(0)
#define     GET_STATUS_LED_7SEG_3B() (P_LED_7SEG_3B)

#define     P_LED_7SEG_3C           P5.2
#define     TURN_ON_LED_7SEG_3C()   do{ P_LED_7SEG_3C = 1; }while(0)
#define     TURN_OFF_LED_7SEG_3C()  do{ P_LED_7SEG_3C = 0; }while(0)
#define     GET_STATUS_LED_7SEG_3C() (P_LED_7SEG_3C)

#define     P_LED_7SEG_3D           P5.1
#define     TURN_ON_LED_7SEG_3D()   do{ P_LED_7SEG_3D = 1; }while(0)
#define     TURN_OFF_LED_7SEG_3D()  do{ P_LED_7SEG_3D = 0; }while(0)
#define     GET_STATUS_LED_7SEG_3D() (P_LED_7SEG_3D)

#define     P_LED_7SEG_3E           P5.0
#define     TURN_ON_LED_7SEG_3E()   do{ P_LED_7SEG_3E = 1; }while(0)
#define     TURN_OFF_LED_7SEG_3E()  do{ P_LED_7SEG_3E = 0; }while(0)
#define     GET_STATUS_LED_7SEG_3E() (P_LED_7SEG_3E)

#define     P_LED_7SEG_3F           P0.5
#define     TURN_ON_LED_7SEG_3F()   do{ P_LED_7SEG_3F = 1; }while(0)
#define     TURN_OFF_LED_7SEG_3F()  do{ P_LED_7SEG_3F = 0; }while(0)
#define     GET_STATUS_LED_7SEG_3F() (P_LED_7SEG_3F)

#define     P_LED_7SEG_3G           P3.0
#define     TURN_ON_LED_7SEG_3G()   do{ P_LED_7SEG_3G = 1; }while(0)
#define     TURN_OFF_LED_7SEG_3G()  do{ P_LED_7SEG_3G = 0; }while(0)
#define     GET_STATUS_LED_7SEG_3G() (P_LED_7SEG_3G)
/***********************************************************************************************************************/
#define     P_LED_7SEG_4A           P7.3
#define     TURN_ON_LED_7SEG_4A()   do{ P_LED_7SEG_4A = 1; }while(0)
#define     TURN_OFF_LED_7SEG_4A()  do{ P_LED_7SEG_4A = 0; }while(0)
#define     GET_STATUS_LED_7SEG_4A() (P_LED_7SEG_4A)

#define     P_LED_7SEG_4B           P7.2
#define     TURN_ON_LED_7SEG_4B()   do{ P_LED_7SEG_4B = 1; }while(0)
#define     TURN_OFF_LED_7SEG_4B()  do{ P_LED_7SEG_4B = 0; }while(0)
#define     GET_STATUS_LED_7SEG_4B() (P_LED_7SEG_4B)

#define     P_LED_7SEG_4C           P7.1
#define     TURN_ON_LED_7SEG_4C()   do{ P_LED_7SEG_4C = 1; }while(0)
#define     TURN_OFF_LED_7SEG_4C()  do{ P_LED_7SEG_4C = 0; }while(0)
#define     GET_STATUS_LED_7SEG_4C() (P_LED_7SEG_4C)

#define     P_LED_7SEG_4D           P7.0
#define     TURN_ON_LED_7SEG_4D()   do{ P_LED_7SEG_4D = 1; }while(0)
#define     TURN_OFF_LED_7SEG_4D()  do{ P_LED_7SEG_4D = 0; }while(0)
#define     GET_STATUS_LED_7SEG_4D() (P_LED_7SEG_4D)

#define     P_LED_7SEG_4E           P0.6
#define     TURN_ON_LED_7SEG_4E()   do{ P_LED_7SEG_4E = 1; }while(0)
#define     TURN_OFF_LED_7SEG_4E()  do{ P_LED_7SEG_4E = 0; }while(0)
#define     GET_STATUS_LED_7SEG_4E() (P_LED_7SEG_4E)

#define     P_LED_7SEG_4F           P7.4
#define     TURN_ON_LED_7SEG_4F()   do{ P_LED_7SEG_4F = 1; }while(0)
#define     TURN_OFF_LED_7SEG_4F()  do{ P_LED_7SEG_4F = 0; }while(0)
#define     GET_STATUS_LED_7SEG_4F() (P_LED_7SEG_4F)

#define     P_LED_7SEG_4G           P7.5
#define     TURN_ON_LED_7SEG_4G()   do{ P_LED_7SEG_4G = 1; }while(0)
#define     TURN_OFF_LED_7SEG_4G()  do{ P_LED_7SEG_4G = 0; }while(0)
#define     GET_STATUS_LED_7SEG_4G() (P_LED_7SEG_4G)
/***********************************************************************************************************************/
#define     P_LED_7SEG_5A           P7.6
#define     TURN_ON_LED_7SEG_5A()   do{ P_LED_7SEG_5A = 1; }while(0)
#define     TURN_OFF_LED_7SEG_5A()  do{ P_LED_7SEG_5A = 0; }while(0)
#define     GET_STATUS_LED_7SEG_5A() (P_LED_7SEG_5A)

#define     P_LED_7SEG_5B           P7.7
#define     TURN_ON_LED_7SEG_5B()   do{ P_LED_7SEG_5B = 1; }while(0)
#define     TURN_OFF_LED_7SEG_5B()  do{ P_LED_7SEG_5B = 0; }while(0)
#define     GET_STATUS_LED_7SEG_5B() (P_LED_7SEG_5B)

#define     P_LED_7SEG_5C           P3.1
#define     TURN_ON_LED_7SEG_5C()   do{ P_LED_7SEG_5C = 1; }while(0)
#define     TURN_OFF_LED_7SEG_5C()  do{ P_LED_7SEG_5C = 0; }while(0)
#define     GET_STATUS_LED_7SEG_5C() (P_LED_7SEG_5C)

#define     P_LED_7SEG_5D           P6.3
#define     TURN_ON_LED_7SEG_5D()   do{ P_LED_7SEG_5D = 1; }while(0)
#define     TURN_OFF_LED_7SEG_5D()  do{ P_LED_7SEG_5D = 0; }while(0)
#define     GET_STATUS_LED_7SEG_5D() (P_LED_7SEG_5D)

#define     P_LED_7SEG_5E           P6.2
#define     TURN_ON_LED_7SEG_5E()   do{ P_LED_7SEG_5E = 1; }while(0)
#define     TURN_OFF_LED_7SEG_5E()  do{ P_LED_7SEG_5E = 0; }while(0)
#define     GET_STATUS_LED_7SEG_5E() (P_LED_7SEG_5E)

#define     P_LED_7SEG_5F           P6.1
#define     TURN_ON_LED_7SEG_5F()   do{ P_LED_7SEG_5F = 1; }while(0)
#define     TURN_OFF_LED_7SEG_5F()  do{ P_LED_7SEG_5F = 0; }while(0)
#define     GET_STATUS_LED_7SEG_5F() (P_LED_7SEG_5F)

#define     P_LED_7SEG_5G           P6.0
#define     TURN_ON_LED_7SEG_5G()   do{ P_LED_7SEG_5G = 1; }while(0)
#define     TURN_OFF_LED_7SEG_5G()  do{ P_LED_7SEG_5G = 0; }while(0)
#define     GET_STATUS_LED_7SEG_5G() (P_LED_7SEG_5G)
/***********************************************************************************************************************/
#define     P_LED_7SEG_6A           P4.1
#define     TURN_ON_LED_7SEG_6A()   do{ P_LED_7SEG_6A = 1; }while(0)
#define     TURN_OFF_LED_7SEG_6A()  do{ P_LED_7SEG_6A = 0; }while(0)
#define     GET_STATUS_LED_7SEG_6A() (P_LED_7SEG_6A)

#define     P_LED_7SEG_6B           P4.2
#define     TURN_ON_LED_7SEG_6B()   do{ P_LED_7SEG_6B = 1; }while(0)
#define     TURN_OFF_LED_7SEG_6B()  do{ P_LED_7SEG_6B = 0; }while(0)
#define     GET_STATUS_LED_7SEG_6B() (P_LED_7SEG_6B)

#define     P_LED_7SEG_6C           P4.3
#define     TURN_ON_LED_7SEG_6C()   do{ P_LED_7SEG_6C = 1; }while(0)
#define     TURN_OFF_LED_7SEG_6C()  do{ P_LED_7SEG_6C = 0; }while(0)
#define     GET_STATUS_LED_7SEG_6C() (P_LED_7SEG_6C)

#define     P_LED_7SEG_6D           P12.0
#define     TURN_ON_LED_7SEG_6D()   do{ P_LED_7SEG_6D = 1; }while(0)
#define     TURN_OFF_LED_7SEG_6D()  do{ P_LED_7SEG_6D = 0; }while(0)
#define     GET_STATUS_LED_7SEG_6D() (P_LED_7SEG_6D)

#define     P_LED_7SEG_6E           P14.0
#define     TURN_ON_LED_7SEG_6E()   do{ P_LED_7SEG_6E = 1; }while(0)
#define     TURN_OFF_LED_7SEG_6E()  do{ P_LED_7SEG_6E = 0; }while(0)
#define     GET_STATUS_LED_7SEG_6E() (P_LED_7SEG_6E)

#define     P_LED_7SEG_6F           P14.1
#define     TURN_ON_LED_7SEG_6F()   do{ P_LED_7SEG_6F = 1; }while(0)
#define     TURN_OFF_LED_7SEG_6F()  do{ P_LED_7SEG_6F = 0; }while(0)
#define     GET_STATUS_LED_7SEG_6F() (P_LED_7SEG_6F)

#define     P_LED_7SEG_6G           P0.0
#define     TURN_ON_LED_7SEG_6G()   do{ P_LED_7SEG_6G = 1; }while(0)
#define     TURN_OFF_LED_7SEG_6G()  do{ P_LED_7SEG_6G = 0; }while(0)
#define     GET_STATUS_LED_7SEG_6G() (P_LED_7SEG_6G)

#endif    /* __HW_H__ */
