
#ifndef _WIFI_CONTROL_H_
#define _WIFI_CONTROL_H_

#include "WIFI_DefUser.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_Status.h"


/***** <User Header> **********************************************************/
/* User �� �Ʒ� 1���� Header �� ���� 
#include "WIFI_Control.h"
*/

/***** <User Function> ********************************************************/
/*
<1> ���� �� �ֱ��Լ�
WifiControlProcess(WIFI_TIME_INI) : �ʱ�ȭ (EEPROM ����) 
WifiControlProcess(WIFI_TIME_SET) : �ʱ���� (EEPROM ���� While ����)
WifiControlProcess(WIFI_TIME_FACTORY) : ������ / �����׽�Ʈ �� �ʱ�ȭ
WifiControlProcess(WIFI_TIME_WHILE) : while 
WifiControlProcess(WIFI_TIME_100MS) : 100ms �ֱ�
WifiControlProcess(WIFI_TIME_1SEC) : 1sec �ֱ�
WifiControlProcess(WIFI_TIME_1MIN) : 1min �ֱ�
*/
void WifiControlProcess ( E_WIFI_TIME_T mu8Time );

/*
<2> Wifi Key
WifiKey(WIFI_KEY_AP) : Wifi Pairing (�ü�+���� 3��)
WifiKey(WIFI_KEY_BLE) : BLE Pairing (���� 3��)
WifiKey(WIFI_KEY_ON)
WifiKey(WIFI_KEY_OFF)
WifiKey(WIFI_KEY_ONOFF) : Power On/Off (�ü�+�뷮 3��)
WifiKey(WIFI_KEY_TEST) : Factory Test

WifiKey(WIFI_KEY_BLE_CERT) : �������� Ű (BLE �������� ������ + ����3��)
WifiKey(WIFI_KEY_BLE_CERT_CANCEL) : �������� ��� (����7��) --> �̻��

*/
void WifiKey ( E_WIFI_KEY_T mu8Key );

/*
<3> Wifi Send Data : �������� ����
WifiSendData(WIFI_DATA_FUNCTION) : ��ɵ����� ���� �� (KEY�Է�/����Ϸ�/���͸���/��ɺ���/�������ܰ���...)
WifiSendData(WIFI_DATA_ERROR) : ���������� ���� �� (�߻�/����)
WifiSendData(WIFI_DATA_PART) : ��� �������� ������ (�ֱ�)
WifiSendData(WIFI_DATA_EXAMINE) : ���� �������� �Ϸ� �� (�̺�Ʈ)
*/
void WifiSendData ( E_WIFI_DATA_T mu8Data );
/*
WifiSendDataControl(WIFI_DATA_FUNCTION_CONTROL, ID) : ��ǰ KEY���� �� (��ǰ ���� �����ʹ� �������� ����)
*/
void WifiSendDataControl ( E_WIFI_DATA_T mu8Data, U8 mu8IdData);
/*
<4> Wifi Get data 
GetWifiStatusValue(WIFI_STATUS_DISP) : WIFI DISPLAY
GetWifiStatusValue(WIFI_STATUS_POWER) : ���� Ȯ�� (0:Off/1:On)
GetWifiStatusValue(WIFI_STATUS_TEST) : TestMode ���� ����Ȯ�� (0:Disconnected/1:Connected)
GetWifiStatusValue(WIFI_STATUS_ERROR) : Wifi Error (0:Normal/1~5:Err)
GetWifiStatusValue(WIFI_STATUS_AP_FIRST) : (0:���ʿ���X/1:���ʿ���Ϸ�)
GetWifiStatusValue(WIFI_STATUS_AP_MODE) : (0:Normal/1:ApMode)
GetWifiStatusValue(WIFI_STATUS_AP_BLE) : (0:Normal/1:BLEApMode)
GetWifiStatusValue(WIFI_STATUS_BLE_CERT) : (0:���������Ұ�����/1:������������ ����/2:���������Ϸ����)  // �������� ���� �߰�!
GetWifiStatusValue(WIFI_STATUS_AP_STEP) : (0:Normal/1~5:ApStep)
GetWifiStatusValue(WIFI_STATUS_AP_ERROR) : AP Error (0:Normal/1~5:Err)
*/
U8 GetWifiStatusValue ( E_WIFI_STATUS_T mType );

/*
<5> Wifi Get Request Info ��û�� ����
GetWifiRequestValue(WIFI_RQST_WEATHER) : ���������� (1 ~ 12)
GetWifiRequestValue(WIFI_RQST_AQI) : AQI (1 ~ 4)
GetWifiRequestValue(WIFI_RQST_CURRENT_TEMP) : ����µ� (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_HIGH_TEMP) : �ְ�µ� (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_LOW_TEMP) : �����µ� (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_FILTER_D_DAY1) : ����1 ��۰���� (0xFFFF �� ��쿡�� �湮���� / ������� ���°�� 9999 )
GetWifiRequestValue(WIFI_RQST_FILTER_D_DAY2) : ����2 ��۰���� (0xFFFF �� ��쿡�� �湮���� / ������� ���°�� 9999 )
GetWifiRequestValue(WIFI_RQST_FILTER_CHANGE_CYCLE) : ���ͱ�ȯ �ֱ�(%) (15Month(�⺻) X 100%(�⺻), 1~999 %)
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
GetWifiServerStatus(STATUS_SVR_CONNECT) : 0  CLEAR  / 1 SET (������) 
*/
#endif

