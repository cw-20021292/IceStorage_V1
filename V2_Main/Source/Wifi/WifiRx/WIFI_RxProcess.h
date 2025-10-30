
#ifndef _WIFI_RxProcess_H_
#define _WIFI_RxProcess_H_

/***** Variable ***************************************************************/
typedef struct _wifirx_
{
    U8  u8State;        // Rx�м� (0:Rx���� / 1:�м���)
    U8	u8Wait;         // ������� ������ (0:���� / 1: ����)
    U8  u8CheckModem;   // ������ ù ����Ȯ��
    U8  u8CheckServer;  // ���� ù ����Ȯ��
    U8  u8ConnectCheck; // Uart ����Ȯ��
    U16 u16ConnectCheckCount;  // ������ ����Ȯ�� (5��ī��Ʈ)
    U8  u8DisplayConnectCount;
} WifiRx_T;

typedef U8 (*WifiRxAtFun_T)(void);
typedef struct _wifi_rxat_list_
{
    I8 *Id;
    U8  Disp;
    U8  Err;
    U8  ApWifi;
    U8  ApBle;
    U8  ApStep;
    U8  Connect;
    U8  Sound;
    WifiRxAtFun_T Func;
} WifiRxAtList_T;

/*******************************************************************************
FUNCTION NAME: InitializeWifiProcessRx

DESCRIPTION: Initialize Rx Value

PARAMETERS: X

RETURN VALUE: X

NOTES: $)CCJ1b 1H8 ( Control )
*******************************************************************************/
void InitializeWifiRxProcess ( void );

/*******************************************************************************
FUNCTION NAME: GetWifiUartRxData

DESCRIPTION: Get uart Rx Data

PARAMETERS: mai8UartData

RETURN VALUE: X

NOTES: Int $)CH#Cb ( UartInterrupt )
*******************************************************************************/
void GetWifiUartRxData ( I8* mai8UartData );

/*******************************************************************************
FUNCTION NAME: WifiRxDataProcess

DESCRIPTION: Rx Data Process

PARAMETERS: X

RETURN VALUE: X

NOTES: While ( Control )
*******************************************************************************/
void WifiRxDataProcess ( void );

/*******************************************************************************
FUNCTION NAME: WifiRxAckTimer

DESCRIPTION: Module Connect Check

PARAMETERS: X

RETURN VALUE: X

NOTES: 100ms ( DataCheck )
*******************************************************************************/
// void WifiRxAckTimer ( void );

void ClearRxStatus ( void );

#endif

