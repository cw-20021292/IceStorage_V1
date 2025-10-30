
#ifndef _WIFI_UART_H_
#define _WIFI_UART_H_

/* Initialize Uart Value */
void InitializeWifiUart ( void );

/* Uart Rx Operation */
void IntWifiUartRx ( U8 mu8RxData );

/* Uart Tx Operation */
void IntWifiUartTx ( void );

/* Send / Receive */
void SendWifiUart ( U16 mu16PacketLength, I8* mai8WifiTxFinalData );
void ReceiveWifiUart ( void );

/* Check Rx Time */
void WifiUartRxTimer ( void );

/* Check Tx Time */
void WifiUartTxTimer ( void );

#endif

