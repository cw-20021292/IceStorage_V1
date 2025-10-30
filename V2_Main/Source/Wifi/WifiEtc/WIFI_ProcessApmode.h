
#ifndef _WIFI_ProcessApmode_H_
#define _WIFI_ProcessApmode_H_

// Initialize
void InitializeWifiAp ( void );

// AP MODE 상태 GET,SET
U8 GetWifiApModeStep ( void ); // Control, Rx
void SetWifiApModeState ( U8 mu8State ); // Control, Rx

// Password GET,SET
I8 GetWifiPassword ( U16 mu16i ); // Initialize
void SetWifiPassword ( U8 mu8i, U8 mu8Data ); // System

// PRODUCT ID GET,SET
void GetWifiPid ( I8 *pBuf ); // Protocol
void SetWifiPid ( U8 mu8i, U8 mu8Data ); // System

// DHCP GET,SET
U8 GetWifiDHCP ( void ); // Initialize
void SetWifiDHCP ( U8 mu8State ); // System

// STATIC IP GET,SET
U16 GetWifiStaticIp ( U16 mu16i ); // Initialize
void SetWifiStaticIp ( U8 mu8i, U8 mu8Data ); // System

// STATIC Gateway GET,SET
U16 GetWifiStaticGateway  ( U16 mu16i ); // Initialize
void SetWifiStaticGateway ( U8 mu8i, U8 mu8Data ); // System

// STATIC Subnet GET,SET
U16 GetWifiStaticSubnet ( U16 mu16i ); // Initialize
void SetWifiStaticSubnet ( U8 mu8i, U8 mu8Data ); // System

// STATIC Subnet GET,SET
U16 GetWifiStaticDns ( U16 mu16i ); // Initialize
void SetWifiStaticDns ( U8 mu8i, U8 mu8Data ); // System

// STATIC Bssid GET,SET
I8 GetWifiBssid ( U16 mu16i ); // Initialize
void SetWifiBssid ( U8 mu8i, U8 mu8Data ); // System

/*******************************************************************************
FUNCTION NAME: WifiRxApProtocol

DESCRIPTION: AP MODE Rx Protocol

PARAMETERS: X
            
RETURN VALUE: X

NOTES: AP모드에서 WIFI Rx Protocol 처리 함수
*******************************************************************************/
void WifiRxApProtocol ( void );

#endif

