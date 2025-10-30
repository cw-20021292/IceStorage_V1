
#ifndef _WIFI_Initialize_H_
#define _WIFI_Initialize_H_

/*******************************************************************************
FUNCTION NAME: WifiStartProcess

DESCRIPTION: 

PARAMETERS: X

RETURN VALUE: X

NOTES: 100ms ( DataCheck )
*******************************************************************************/
void WifiStartProcess ( void );

/*******************************************************************************
FUNCTION NAME: WifiCheckInitializeAck

DESCRIPTION: 

PARAMETERS: X

RETURN VALUE: X

NOTES: ( ProcessRx )
*******************************************************************************/
void WifiCheckInitializeAck ( void );

void WifiCheckInitializeInfoAck ( void );
#endif

