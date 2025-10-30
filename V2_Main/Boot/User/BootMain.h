 /*'""FILE COMMENT""****************************************************
@ FILE Name : BootMain.h
@ FILE ID    : 
@ Programmer : Junbeen Son
@ Description : 
/*""FILE COMMENT END ""************************************************/

#pragma sfr
#pragma DI
#pragma EI
#pragma NOP
#pragma HALT
#pragma STOP

typedef unsigned char       U8;
typedef unsigned int        U16;
typedef unsigned long int   U32;

#define SET         1
#define CLEAR       0
#define HIGH		1
#define LOW			0

#define BOOT_RX_NONE	0
#define BOOT_RX_DOING	1
#define BOOT_RX_DONE	2

#include "Fsl.h"
#include "Fsl_types.h"
#include "pfdl.h"         
#include "pfdl_types.h"          
#include "BootWrite.h"
#include "BootDataflash.h"

//====================================================================================//
//=========== 1. Module Setting ======================================================//
//====================================================================================//
//******** define only 1 Module *********//
#define BOOT_MODULE_WIFI  
//#define BOOT_MODULE_LTE   
//====================================================================================// 
 
//====================================================================================//
//=========== 2. Clock Setting =======================================================//
//====================================================================================//
//******** define only 1 Clock *********//
//#define  OCO_CLOCK_16MHz		
#define  OCO_CLOCK_32MHz				// 16MHz base 프로젝트파일이므로 32MHz 사용시, 옵션바이트도 변경 진행 필요 
//====================================================================================//

//====================================================================================//
//=========== 3. Port Setting ========================================================//
//====================================================================================//
#define  BOOT_PM_MODULE_POWER		PM0.4	// Output Only에 해당하는 포트 사용 시, 해당 Define 삭제할 것  
#define  BOOT_P_MODULE_POWER		P0.4	

#ifdef BOOT_MODULE_WIFI
#define  BOOT_PM_MODULE_RESET		PM0.1	// Output Only에 해당하는 포트 사용 시, 해당 Define 삭제할 것  
#define  BOOT_P_MODULE_RESET		P0.1 
#endif

#ifdef BOOT_MODULE_LTE	// LTE 모듈 사용 시, RESET포트는 POWER포트와 동일하게 작성됨
#define  BOOT_PM_MODULE_RESET		BOOT_PM_MODULE_POWER	// Output Only에 해당하는 포트 사용 시, 해당 Define 삭제할 것 
#define  BOOT_P_MODULE_RESET		BOOT_P_MODULE_POWER		// LTE 모듈 사용 시, RESET 포트는 POWER 포트와 동일하게 작성됨 
#endif

//#define  BOOT_PM_MODULE_LED_SINK		PM0.3	// Output Only에 해당하는 포트 사용 시, 해당 Define 삭제할 것 
//#define  BOOT_P_MODULE_LED_SINK			P0.3	// 해당 포트 미사용 시, 해당 Define 삭제 할 것

//#define  BOOT_PM_MODULE_LED_DRIVE		PM0.4	// Output Only에 해당하는 포트 사용 시, 해당 Define 삭제할 것
//#define  BOOT_P_MODULE_LED_DRIVE		P0.4	// 해당 포트 미사용 시, 해당 Define 삭제 할 것
//#define  BOOT_P_MODULE_LED_DRIVE_LEVEL	LOW		// 회로 구성에 따라 HIGH/LOW 선택
//====================================================================================//

#define  BOOT_P_UART_TX					P0.2	// R5F100MG 기준, UART1 Tx Port => P02

#ifdef OCO_CLOCK_16MHz
#define R_PFDL_FDL_FRQ      16      /* Sets the frequency (16 MHz)             */
#endif
#ifdef OCO_CLOCK_32MHz
#define R_PFDL_FDL_FRQ      32      /* Sets the frequency (32 MHz)             */
#endif


 
#define  Watchdog() WDTE = 0xAC  

__near void BOOT_R_CGC_Create(void);
__near void BOOT_R_PORT_Create(void);
__near void BOOT_R_TAU0_Create(void);
__near void BOOT_R_TAU0_Channel0_Start(void);
__near void BOOT_R_SAU0_Create(void);
__near void BOOT_R_UART1_Create(void);
__near void BOOT_R_UART1_Start(void);
__near void BOOT_R_UART1_Stop(void);
__near void BOOT_BaseTimer(void);	
__near void BOOT_FotaLEDControl(void);
__near static U8 BOOT_initModulePort(void);
__near static U8 BOOT_checkOTARequestCondition(__near U8* mu8RxData);
__near static U8 BOOT_checkOldBootErase(__near U8* mu8RxData);
__near void BOOT_setOTARequest(void);  


