
#ifndef _WIFI_SetCommon_H_
#define _WIFI_SetCommon_H_


typedef enum
{
    WIFI_BLE_CERT_0001 = 0x0001,
    WIFI_BLE_CERT_0002,
    WIFI_BLE_CERT_0003,

}E_API_A7010_T;

void* GetWifiMonitorEventList ( void );

U8 IsUserModelCHP ( void );

void ApplyFunctionData ( U16 mId, I8 *pBuf );

void ApplyInfoData ( U16 mId, I8 *pBuf);

void ApplyMonitorData ( U16 mId, I8 *pBuf);

void ConvertDataHexToAscii ( U8 mType, U16 mu16Id, I8 *pBuf );

void ConvertDataHexToAscii_RAM_Monitor( U8 mType, U16 mu16Address, I8 *pBuf, U16 mLen );
#endif


