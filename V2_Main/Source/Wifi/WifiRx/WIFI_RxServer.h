
#ifndef _WIFI_RxServer_H_
#define _WIFI_RxServer_H_

/* Init Rx Value */
void InitializeWifiRxServer ( void );

/* Get Server Data */
void WifiInsertRxAPI ( I8* mai8Recv );
void WifiInsertRxBLEAPI ( I8* mai8Recv );
/* Get/Set Info Data */
U16 GetWifiInfoData ( U16 mId );
void SetWifiInfoData ( U16 mId, U16 mData );


#endif


