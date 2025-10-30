
#ifndef _WIFI_COMMON_BUILTIN_H_
#define _WIFI_COMMON_BUILTIN_H_

#ifdef ICON_AIS
/***** User headers ****************************************************************/
#include "mcu.h"
#include "r_cg_serial.h"
#include "WIFI_DefUser.h"


/***** Type Define *****************************************************************/
#if 0
typedef unsigned char       U8;
typedef signed char         I8;
typedef unsigned int        U16;
typedef signed int          I16;
typedef const signed char   CI8;
typedef const signed int    CI16;
typedef unsigned long int   U32;
typedef float               F32;
typedef double              D64;
#endif

/***** library headers ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

/********** HIDDEN_BUILTIN **********/
#define MODEL_NO_1
#define WIFI_VERSION    "V1.0.1.17"
#define MODEL_NAME_CHP  "CHPI-7400NV2"
#define MODEL_NAME_CP   "CPI-7400NV2"
#define MODEL_NAME_HP   "HPI-7400NV2"

// USER WIFI Port & Register
#define R_TXD           TXD1
#define R_RXD           RXD1
#define R_STIF          STIF1
#define P_WIFI_ON       P0.4
#define P_WIFI_RSTN     P0.1

// USER Define (Number of User Data)
#define USER_DATA_MAX_NUM   53  // 사용하는 데이터 총 개수
#define USER_DATA_MAX_NUM_1BYTE   5  // The number of data (DATA Size)
#define USER_DATA_MAX_NUM_3BYTE   5  // The number of data (DATA Size)
#define USER_DATA_MAX_NUM_5BYTE   43  // The number of data (DATA Size)
#define USER_INFO_MAX_NUM   15  // 사용하는 정보 총 개수
//#define USER_USE_ACCUMULATE   // 누적데이터 사용여부
#define USER_USE_WEATHER      // 날씨데이터 사용여부
#define USER_USE_ACK
// #define USER_EasyPairing20    // 이지페어링2.0 사용여부 (점유인증 / 연결끊김 후 BLE자동활성화 / FOTA 후 BLE 자동활성화)

// #define FOTA_SIZE_128K 
// #define FOTA_SIZE_192K 
#define FOTA_SIZE_256K 

// USER Define (RomSize/ProductNum)
#define FOTA            1           // 0:Unused / 1:Used Fota

#ifdef FOTA_SIZE_128K
#define FOTA_SIZE       "513"       // FotaSize = Rom Size * 4 + 1
#endif

#ifdef FOTA_SIZE_192K
#define FOTA_SIZE       "769"       // FotaSize = Rom Size * 4 + 1
#endif

#ifdef FOTA_SIZE_256K
#define FOTA_SIZE       "1025"       // FotaSize = Rom Size * 4 + 1
#endif 

#define FOTA_TYPE_CHP   "113839"    // Product Code Number
#define FOTA_TYPE_CP    "113843"    // Product Code Number
#define FOTA_TYPE_HP    "000000"    // Product Code Number

// USER Define (EEPROM Address)
#define EEPROM_ADDR_WIFI_POWER      0x005E   // WIFI On/Off Status
#define EEPROM_ADDR_FIRST_PARING    0x005F   // WIFI First Paring Status
#define EEPROM_ADDR_FOTA_MODULE     0x0060   // WIFI Module Fota Status
#define EEPROM_ADDR_FOTA_MCU        0x0061   // WIFI MCU Fota Status

#define Z_rammax		(0xFFFF)        //(0xFEDF)				//	MCU RAM Range_MAX ~ less than SFR		
#define Z_rammin		(0xB300)		//	MCU RAM Range_MIN* 256K ->  AF00 ~ B2FF 까지 사용금지 

// USER Define (Buzzer)
#define	WIFI_BUZZER_SELECT          0x01  // 페어링 (선택음 "띵")
#define	WIFI_BUZZER_AP_CONNECT      0x02  // AP연결 (진입음 "띠리링" / 와이파이 연결을 시작합니다)
#define	WIFI_BUZZER_BLE_CONNECT     0x02 // AP연결 (진입음 "띠리링" / BLE연결모드가 활성화 되었습니다.) (웨스턴 추가)
#define	WIFI_BUZZER_SETUP           0x03  // WIFI ON (설정음 "띵동↗" / 와이파이 연결을 시작합니다)
#define	WIFI_BUZZER_SERVER_CONNECT  0x04  // 서버연결 (연결음 "띠리링↗" / 와이파이 연결이 완료되었습니다)
#define	WIFI_BUZZER_AP_FAIL         0x05  // 페어링30분 경과 (해제음 "동띵" / 와이파이 연결이 실패되었습니다. 인터넷 공유기를 확인해주세요. 문제가 반복되면 콜센터에 연락주세요.)
#define	WIFI_BUZZER_CANCEL          0x06  // WIFI OFF (해제음 "동띵" / 와이파이 연결이 해제되었습니다. 다시 연결하시려면 제품 설정 모드에서 와이파이를 켜주세요.)
#define WIFI_BUZZER_AP_START        0x07  // AP시작음 (설정음 "띵동 ↗")
#define	WIFI_BUZZER_ERROR           0x08  // 불가음 (에러음 "띵깡띵깡")
#define WIFI_BUZZER_SERVER_FIRST    0x09  // 서버연결 (연결음 "띠리링↗" / 와이파이 연결이 완료되었습니다. 이제부터 아이오케어 앱을 통해 스마트한 관리를 받으실 수 있습니다.)
#define WIFI_BUZZER_WiFi_OFF	    0x0A // Wi-Fi 페어링 해제음(동띵) / Wi-Fi 연결 모드가 비활성화 되었습니다. (웨스턴 추가)
#define	WIFI_BUZZER_BLE_OFF         0x0A  // 블루투스 패어링 해제음(동띵)
#define WIFI_BUZZER_BLE_CERT        0x01 // (선택음 "띵") / 블루투스 점유인증 활성화 상태 / 제품인증을 진행 중입니다. 앱에서 가이드하는 버튼을 눌러주세요. (웨스턴 추가)
#define WIFI_BUZZER_SMART_CHECK_START       0x03 // (설정음 "띵동 ↗") / 스마트 진단을 시작합니다. 진행 중에는 제품을 사용할 수 없으니 잠시만 기다려 주세요. (웨스턴 추가)
#define WIFI_BUZZER_SMART_CHECK_OK          0x03 // (설정음 "띵동 ↗") / 스마트 진단이 완료 되어 제품을 사용하실 수 있습니다. (웨스턴 추가)
#define WIFI_BUZZER_SMART_CHECK_ERROR       0x08 // 불가음 (에러음 "띵깡띵깡") / 스마트 진단이 완료 되었습니다. (웨스턴 추가)
#endif

#endif

