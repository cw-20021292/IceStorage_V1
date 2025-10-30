
#ifndef _WIFI_Fota_H_
#define _WIFI_Fota_H_

/*******************************************************************************
FUNCTION NAME: InitializeWifiProcessRx

DESCRIPTION: Initialize Rx Value

PARAMETERS: X

RETURN VALUE: X

NOTES: √ ±‚ 1»∏ ( Control )
*******************************************************************************/
void InitializeWifiFota ( void );

void WifiFotaProcess ( void );

void WifiCheckFotaSize ( I8* mi8FotaData );
void WifiCheckFotaType ( I8* mi8FotaData );



#endif

