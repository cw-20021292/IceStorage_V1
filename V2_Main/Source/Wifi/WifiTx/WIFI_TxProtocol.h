
#ifndef _WIFI_TxProtocol_H_
#define _WIFI_TxProtocol_H_

/*******************************************************************************
FUNCTION NAME: InitializeWifiTxProtocol

DESCRIPTION: Initialize Tx Protocol Value

PARAMETERS: X

RETURN VALUE: X

NOTES: Initialize (WIFI_Control)
*******************************************************************************/
void InitializeWifiTxProtocol ( void );

/*******************************************************************************
FUNCTION NAME: SetRequestId

DESCRIPTION: Setting Request Id

PARAMETERS: mi8RqId

RETURN VALUE: X

NOTES: Call Function (WIFI_RxServer)
*******************************************************************************/
void SetRequestId ( I8* mi8RqId );
void SetRequestIdExcept ( void );

/*******************************************************************************
FUNCTION NAME: WifiSendProtocol

DESCRIPTION: Send Protocol

PARAMETERS: mu8DataType(ToServer/ToModule) / mu8ReId(ReID Usage Status) / mi8Data / mi8AtCmd / mi8Api

RETURN VALUE: X

NOTES: Call Function (WIFI_TxMake)
*******************************************************************************/
void WifiSendProtocol ( U8 mu8Target, U8 mu8ReId, I8* mi8Data, I8* mi8AtCmd, I8* mi8Api );


#endif

