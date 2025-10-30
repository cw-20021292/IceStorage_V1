
#ifndef _WIFI_CONTROL_H_
#define _WIFI_CONTROL_H_

#include "WIFI_DefUser.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_Status.h"


/***** <User Header> **********************************************************/
/* User 는 아래 1개의 Header 만 참조 
#include "WIFI_Control.h"
*/

/***** <User Function> ********************************************************/
/*
<1> 셋팅 및 주기함수
WifiControlProcess(WIFI_TIME_INI) : 초기화 (EEPROM 이전) 
WifiControlProcess(WIFI_TIME_SET) : 초기셋팅 (EEPROM 이후 While 이전)
WifiControlProcess(WIFI_TIME_FACTORY) : 공장모드 / 출하테스트 후 초기화
WifiControlProcess(WIFI_TIME_WHILE) : while 
WifiControlProcess(WIFI_TIME_100MS) : 100ms 주기
WifiControlProcess(WIFI_TIME_1SEC) : 1sec 주기
WifiControlProcess(WIFI_TIME_1MIN) : 1min 주기
*/
void WifiControlProcess ( E_WIFI_TIME_T mu8Time );

/*
<2> Wifi Key
WifiKey(WIFI_KEY_AP) : Wifi Pairing (냉수+정수 3초)
WifiKey(WIFI_KEY_BLE) : BLE Pairing (정수 3초)
WifiKey(WIFI_KEY_ON)
WifiKey(WIFI_KEY_OFF)
WifiKey(WIFI_KEY_ONOFF) : Power On/Off (냉수+용량 3초)
WifiKey(WIFI_KEY_TEST) : Factory Test

WifiKey(WIFI_KEY_BLE_CERT) : 점유인증 키 (BLE 점유인증 대기상태 + 정수3초)
WifiKey(WIFI_KEY_BLE_CERT_CANCEL) : 점유인증 취소 (정수7초) --> 미사용

*/
void WifiKey ( E_WIFI_KEY_T mu8Key );

/*
<3> Wifi Send Data : 엑셀파일 참조
WifiSendData(WIFI_DATA_FUNCTION) : 기능데이터 변경 시 (KEY입력/추출완료/필터리셋/기능변경/고장진단관련...)
WifiSendData(WIFI_DATA_ERROR) : 에러데이터 변경 시 (발생/해지)
WifiSendData(WIFI_DATA_PART) : 상시 고장진단 데이터 (주기)
WifiSendData(WIFI_DATA_EXAMINE) : 정밀 고장진단 완료 시 (이벤트)
*/
void WifiSendData ( E_WIFI_DATA_T mu8Data );
/*
WifiSendDataControl(WIFI_DATA_FUNCTION_CONTROL, ID) : 제품 KEY조작 시 (제품 조작 데이터는 프로토콜 참조)
*/
void WifiSendDataControl ( E_WIFI_DATA_T mu8Data, U8 mu8IdData);
/*
<4> Wifi Get data 
GetWifiStatusValue(WIFI_STATUS_DISP) : WIFI DISPLAY
GetWifiStatusValue(WIFI_STATUS_POWER) : 전원 확인 (0:Off/1:On)
GetWifiStatusValue(WIFI_STATUS_TEST) : TestMode 에서 연결확인 (0:Disconnected/1:Connected)
GetWifiStatusValue(WIFI_STATUS_ERROR) : Wifi Error (0:Normal/1~5:Err)
GetWifiStatusValue(WIFI_STATUS_AP_FIRST) : (0:최초연결X/1:최초연결완료)
GetWifiStatusValue(WIFI_STATUS_AP_MODE) : (0:Normal/1:ApMode)
GetWifiStatusValue(WIFI_STATUS_AP_BLE) : (0:Normal/1:BLEApMode)
GetWifiStatusValue(WIFI_STATUS_BLE_CERT) : (0:점유인증불가상태/1:점유인증가능 상태/2:점유인증완료상태)  // 점유인증 조작 추가!
GetWifiStatusValue(WIFI_STATUS_AP_STEP) : (0:Normal/1~5:ApStep)
GetWifiStatusValue(WIFI_STATUS_AP_ERROR) : AP Error (0:Normal/1~5:Err)
*/
U8 GetWifiStatusValue ( E_WIFI_STATUS_T mType );

/*
<5> Wifi Get Request Info 요청값 리턴
GetWifiRequestValue(WIFI_RQST_WEATHER) : 날씨데이터 (1 ~ 12)
GetWifiRequestValue(WIFI_RQST_AQI) : AQI (1 ~ 4)
GetWifiRequestValue(WIFI_RQST_CURRENT_TEMP) : 현재온도 (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_HIGH_TEMP) : 최고온도 (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_LOW_TEMP) : 최저온도 (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_FILTER_D_DAY1) : 필터1 배송경과일 (0xFFFF 인 경우에는 방문관리 / 배송일이 없는경우 9999 )
GetWifiRequestValue(WIFI_RQST_FILTER_D_DAY2) : 필터2 배송경과일 (0xFFFF 인 경우에는 방문관리 / 배송일이 없는경우 9999 )
GetWifiRequestValue(WIFI_RQST_FILTER_CHANGE_CYCLE) : 필터교환 주기(%) (15Month(기본) X 100%(기본), 1~999 %)
*/
U16 GetWifiRequestValue ( E_WIFI_RQST_T mId );

/* r_uart1_interrupt_send */
void InterruptUartWifiTx ( void );
/* r_uart1_interrupt_receive */
void InterruptUartWifiRx ( U8 mu8RxData );
/* r_uart1_interrupt_error */
void InterruptUartWifiError ( void );

void WifiTimerControl ( void );

/**/
// U8 GetWifiServerStatus ( E_WIFI_STATUS_SVR_T mId );

/*
GetWifiServerStatus(STATUS_SVR_CONNECT) : 0  CLEAR  / 1 SET (연결중) 
*/
#endif

