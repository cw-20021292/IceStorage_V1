
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
#define USER_DATA_MAX_NUM   53  // ����ϴ� ������ �� ����
#define USER_DATA_MAX_NUM_1BYTE   5  // The number of data (DATA Size)
#define USER_DATA_MAX_NUM_3BYTE   5  // The number of data (DATA Size)
#define USER_DATA_MAX_NUM_5BYTE   43  // The number of data (DATA Size)
#define USER_INFO_MAX_NUM   15  // ����ϴ� ���� �� ����
//#define USER_USE_ACCUMULATE   // ���������� ��뿩��
#define USER_USE_WEATHER      // ���������� ��뿩��
#define USER_USE_ACK
// #define USER_EasyPairing20    // ������2.0 ��뿩�� (�������� / ������� �� BLE�ڵ�Ȱ��ȭ / FOTA �� BLE �ڵ�Ȱ��ȭ)

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
#define Z_rammin		(0xB300)		//	MCU RAM Range_MIN* 256K ->  AF00 ~ B2FF ���� ������ 

// USER Define (Buzzer)
#define	WIFI_BUZZER_SELECT          0x01  // �� (������ "��")
#define	WIFI_BUZZER_AP_CONNECT      0x02  // AP���� (������ "�츮��" / �������� ������ �����մϴ�)
#define	WIFI_BUZZER_BLE_CONNECT     0x02 // AP���� (������ "�츮��" / BLE�����尡 Ȱ��ȭ �Ǿ����ϴ�.) (������ �߰�)
#define	WIFI_BUZZER_SETUP           0x03  // WIFI ON (������ "�򵿢�" / �������� ������ �����մϴ�)
#define	WIFI_BUZZER_SERVER_CONNECT  0x04  // �������� (������ "�츮����" / �������� ������ �Ϸ�Ǿ����ϴ�)
#define	WIFI_BUZZER_AP_FAIL         0x05  // ��30�� ��� (������ "����" / �������� ������ ���еǾ����ϴ�. ���ͳ� �����⸦ Ȯ�����ּ���. ������ �ݺ��Ǹ� �ݼ��Ϳ� �����ּ���.)
#define	WIFI_BUZZER_CANCEL          0x06  // WIFI OFF (������ "����" / �������� ������ �����Ǿ����ϴ�. �ٽ� �����Ͻ÷��� ��ǰ ���� ��忡�� �������̸� ���ּ���.)
#define WIFI_BUZZER_AP_START        0x07  // AP������ (������ "�� ��")
#define	WIFI_BUZZER_ERROR           0x08  // �Ұ��� (������ "������")
#define WIFI_BUZZER_SERVER_FIRST    0x09  // �������� (������ "�츮����" / �������� ������ �Ϸ�Ǿ����ϴ�. �������� ���̿��ɾ� ���� ���� ����Ʈ�� ������ ������ �� �ֽ��ϴ�.)
#define WIFI_BUZZER_WiFi_OFF	    0x0A // Wi-Fi �� ������(����) / Wi-Fi ���� ��尡 ��Ȱ��ȭ �Ǿ����ϴ�. (������ �߰�)
#define	WIFI_BUZZER_BLE_OFF         0x0A  // ������� �о ������(����)
#define WIFI_BUZZER_BLE_CERT        0x01 // (������ "��") / ������� �������� Ȱ��ȭ ���� / ��ǰ������ ���� ���Դϴ�. �ۿ��� ���̵��ϴ� ��ư�� �����ּ���. (������ �߰�)
#define WIFI_BUZZER_SMART_CHECK_START       0x03 // (������ "�� ��") / ����Ʈ ������ �����մϴ�. ���� �߿��� ��ǰ�� ����� �� ������ ��ø� ��ٷ� �ּ���. (������ �߰�)
#define WIFI_BUZZER_SMART_CHECK_OK          0x03 // (������ "�� ��") / ����Ʈ ������ �Ϸ� �Ǿ� ��ǰ�� ����Ͻ� �� �ֽ��ϴ�. (������ �߰�)
#define WIFI_BUZZER_SMART_CHECK_ERROR       0x08 // �Ұ��� (������ "������") / ����Ʈ ������ �Ϸ� �Ǿ����ϴ�. (������ �߰�)
#endif

#endif

