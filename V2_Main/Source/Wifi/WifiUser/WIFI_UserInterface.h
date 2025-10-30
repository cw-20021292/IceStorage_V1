
#ifndef _WIFI_UserInterface_H_
#define _WIFI_UserInterface_H_

/* Initialize User Interface Value */
void InitializeWifiUserInterface ( void );

/* EEp Read / Write */
void WifiWriteEep ( U16 mAddr, U8 mVal );
U8 WifiReadEep ( U16 mAddr );

/* Buzzer */
void SetWifiBuzzSound ( U8 mu8Buzz );

/* User Interface Data */
U8 GetUserInterface ( U8 mu8Value );
void SetUserDisplay ( U8 mu8Display );
void ClearUserData ( void );

/* User Data Matching */
U32 GetUserMonitorData ( U16 mu16Func, U16 mu16Len );
U16 GetUserErrorData ( U16 mu16Err );
U16 GetUserFunctionData ( U16 mu16Func );
I16 GetUserSensorData ( U16 mu16Sen );
U16 GetUserPartData ( U16 mu16Part );
U32 GetUserAccumulateData ( U16 mu16Acc );

/* App Control -> User System Function Data */
void SetUserFunctionData ( U16 mu16Func, U16 mData );

/* RAM Monitor */
void SetUserMonitorData ( U16 mu16address, I8 *mData );

/* Time Setting */
void SetUserTimeSetting ( U16* pData );

/* Fota */
void StartFotaOperation ( void );
extern void BOOT_setOTARequest(void);


#endif


