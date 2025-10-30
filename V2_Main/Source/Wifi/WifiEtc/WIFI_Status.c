
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_Status.h"
#include "WIFI_UserInterface.h"



// WIFI Status Value

typedef struct _wifistatusvalue_
{
    U8  u8Module[STATUS_MODULE_NUM];
    U8  u8Control[STATUS_CON_NUM];
    U8  u8Server[STATUS_SVR_NUM];
    U8  u8Initialize[STATUS_INI_NUM];
    U8  u8Tx[STATUS_TX_NUM];
    U8  u8Fota[STATUS_FOTA_NUM];
    U8  u8Ap[STATUS_AP_NUM];
    U8  u8Indicate[STATUS_IDC_NUM];
    U8  u8ControlId;
    U32 u32Send[STATUS_SEND_NUM];
    U8  u8_Wifi_BLE_Send[STATUS_SEND_WIFI_BLE_NUM];
} WifiStatusValue_T;
WifiStatusValue_T WifiStatusValue;

// WIFI Cert Status Value
typedef struct _wificertstatusvalue_
{
    U8  u8CertValueID;
    U8  u8CertValueResult;
    U8  u8CertValueStatus;

} WifiCertStatusValue_T;
WifiCertStatusValue_T WifiCertStatusValue;

void InitializeWifiStatus ( void )
{
    _MEMSET_( (void __FAR*) &WifiStatusValue, 0, sizeof(WifiStatusValue_T) );
    _MEMSET_( (void __FAR*) &WifiCertStatusValue, 0, sizeof(WifiCertStatusValue_T) );
}

void HAL_SetWifiOnOff ( U8 mu8State )
{
    if ( mu8State == ON )
    {
        ClearWifiAllStatus();
        SetWifiControlStatus(STATUS_CON_POWER,ON);
        SetWifiIndicateStatus(STATUS_DISPLAY,WIFI_DISP_TRY);
        SetWifiModuleStatus(STATUS_MODULE_POWER,SET);
        SetWifiModuleStatus(STATUS_MODULE_SEQ,MODULE_ON_START);
    }
    else if ( mu8State == RESET )
    {
#if ( TEST_B1_OTA == 1 )
    return;
#endif
        ClearWifiAllStatus();
        SetWifiControlStatus(STATUS_CON_POWER,ON);
        SetWifiIndicateStatus(STATUS_DISPLAY,WIFI_DISP_TRY);
        SetWifiModuleStatus(STATUS_MODULE_POWER,SET);
        SetWifiModuleStatus(STATUS_MODULE_SEQ,MODULE_RESET_START);
    }
    else // OFF
    {
        ClearWifiAllStatus();
        WifiWriteEep(EEPROM_ADDR_WIFI_POWER, OFF);
        SetWifiControlStatus(STATUS_CON_POWER,OFF);
        SetWifiModuleStatus(STATUS_MODULE_POWER,SET);
        SetWifiModuleStatus(STATUS_MODULE_SEQ,MODULE_OFF_START);
    }
}


/***** Module STATUS **********************************************************/
U8 GetWifiModuleStatus ( E_WIFI_STATUS_MODULE_T mId )
{
    return WifiStatusValue.u8Module[mId];
}
void SetWifiModuleStatus ( E_WIFI_STATUS_MODULE_T mId , U8 mu8Status )
{
    WifiStatusValue.u8Module[mId] = mu8Status;
}


/***** Control STATUS *********************************************************/
U8 GetWifiControlStatus ( E_WIFI_STATUS_CON_T mId )
{
    return WifiStatusValue.u8Control[mId];
}
void SetWifiControlStatus ( E_WIFI_STATUS_CON_T mId , U8 mu8Status )
{
    WifiStatusValue.u8Control[mId] = mu8Status;
}


/***** Server STATUS **********************************************************/
U8 GetWifiServerStatus ( E_WIFI_STATUS_SVR_T mId )
{
    return WifiStatusValue.u8Server[mId];
}
void SetWifiServerStatus ( E_WIFI_STATUS_SVR_T mId , U8 mu8Status )
{
    WifiStatusValue.u8Server[mId] = mu8Status;
}


/***** Initialize STATUS ******************************************************/
U8 GetWifiIniStatus ( E_WIFI_STATUS_INI_T mId )
{
    return WifiStatusValue.u8Initialize[mId];
}
void SetWifiIniStatus ( E_WIFI_STATUS_INI_T mId , U8 mu8Status )
{
    WifiStatusValue.u8Initialize[mId] = mu8Status;
}


/***** Tx STATUS **************************************************************/
U8 GetWifiTxStatus ( E_WIFI_STATUS_TX_T mId )
{
    return WifiStatusValue.u8Tx[mId];
}
void SetWifiTxStatus ( E_WIFI_STATUS_TX_T mId , U8 mu8Status )
{
    WifiStatusValue.u8Tx[mId] = mu8Status;
}


/***** FOTA STATUS ************************************************************/
U8 GetWifiFotaStatus ( E_WIFI_STATUS_FOTA_T mId )
{
    return WifiStatusValue.u8Fota[mId];
}
void SetWifiFotaStatus ( E_WIFI_STATUS_FOTA_T mId , U8 mu8Status )
{
    WifiStatusValue.u8Fota[mId] = mu8Status;
}


/***** AP STATUS **************************************************************/
U8 GetWifiApStatus ( E_WIFI_STATUS_AP_T mId )
{
    return WifiStatusValue.u8Ap[mId];

}
void SetWifiApStatus ( E_WIFI_STATUS_AP_T mId , U8 mu8Status )
{
    WifiStatusValue.u8Ap[mId] = mu8Status;
}


/***** Indicate STATUS ********************************************************/
U8 GetWifiIndicateStatus ( E_WIFI_STATUS_IDC_T mId )
{
    return WifiStatusValue.u8Indicate[mId];
}
void SetWifiIndicateStatus ( E_WIFI_STATUS_IDC_T mId , U8 mu8Status )
{

    WifiStatusValue.u8Indicate[mId] = mu8Status;
}

/***** Send Tx Data STATUS_CONTROL ****************************************************/
U8 GetWifiSendStatusControl ( void )
{
    return WifiStatusValue.u8ControlId;
}
void SetWifiSendStatusControl ( U8 mu8IdData )
{
    WifiStatusValue.u8ControlId = mu8IdData;
}

/***** Send Tx Data STATUS ****************************************************/
U32 GetWifiSendStatus ( E_WIFI_STATUS_SEND_T mId )
{
    return WifiStatusValue.u32Send[mId];
}
void SetWifiSendStatus ( E_WIFI_STATUS_SEND_T mId , U32 mu32Status )
{
    WifiStatusValue.u32Send[mId] = (WifiStatusValue.u32Send[mId]|mu32Status);
}
void ClearWifiSendStatus ( E_WIFI_STATUS_SEND_T mId , U32 mu32Status )
{
    WifiStatusValue.u32Send[mId] = (WifiStatusValue.u32Send[mId]^mu32Status);
}


/***** Send WIFI_BLE Tx Data STATUS ****************************************************/
U8 GetWifiBLESendStatus ( E_WIFI_STATUS_SEND_WIFI_BLE_T mId )
{
    return WifiStatusValue.u8_Wifi_BLE_Send[mId];
}
void SetWifiBLESendStatus ( E_WIFI_STATUS_SEND_WIFI_BLE_T mId , U8 mu8Status)
{
    WifiStatusValue.u8_Wifi_BLE_Send[mId] = mu8Status;
}
void ClearWifi_BLE_SendStatus ( E_WIFI_STATUS_SEND_WIFI_BLE_T mId , U8 mu8Status)
{
    WifiStatusValue.u8_Wifi_BLE_Send[mId] = mu8Status;
}
/***** CERT ID **********************************************************/
U8 GetWifiCertID ( void )
{
    return WifiCertStatusValue.u8CertValueID;
}
void SetWifiCertID ( U8 mu8ID )
{
    WifiCertStatusValue.u8CertValueID = mu8ID;
}
/***** CERT Result **********************************************************/
U8 GetWifiCertResult ( void )
{
    return WifiCertStatusValue.u8CertValueResult;
}
void SetWifiCertResult ( U8 mu8Result )
{
    WifiCertStatusValue.u8CertValueResult = mu8Result;
}
/***** CERT Status **********************************************************/
U8 GetWifiCertStatus ( void )
{
    return WifiCertStatusValue.u8CertValueStatus;
}
void SetWifiCertStatus ( U8 mu8Status )
{
    WifiCertStatusValue.u8CertValueStatus = mu8Status;
}
/***** ETC ********************************************************************/
/* WIFI  */
void ClearWifiAllStatus ( void )
{
    U8 mu8FirstParing = 0U;
    U8 mu8ErrorDisplay = 0U;
    U8 mu8ApError = 0U;
    U8 mu8FotaMcu = 0U;
    U8 mu8Factory = 0U;
    /* All Initialize except for First pairing & Wifi Error Display */
    mu8FirstParing = GetWifiApStatus(STATUS_AP_FIRST);
    mu8ErrorDisplay = GetWifiIndicateStatus(STATUS_ERROR);
    mu8ApError = GetWifiIndicateStatus(STATUS_AP_ERROR);
    mu8FotaMcu = GetWifiFotaStatus(STATUS_FOTA_MCU);
    mu8Factory= GetWifiIndicateStatus(STATUS_FACTORY);
    _MEMSET_( (void __FAR*) &WifiStatusValue, 0, sizeof(WifiStatusValue_T) );
    #ifdef USER_EasyPairing20
    /* 이지페어링 2.0 FOTA완료 BLE활성화 사용여부 */
    if (mu8FotaMcu == SET) 
    {
        SetWifiApStatus(STATUS_AP_BLE,WAIT);
    }
    #endif
    SetWifiApStatus(STATUS_AP_FIRST,mu8FirstParing);
    SetWifiIndicateStatus(STATUS_ERROR,mu8ErrorDisplay);
    SetWifiIndicateStatus(STATUS_AP_ERROR,mu8ApError);
    SetWifiFotaStatus(STATUS_FOTA_MCU,mu8FotaMcu);
    SetWifiIndicateStatus(STATUS_FACTORY,mu8Factory);
}


void WifiOnOffSetting ( void )
{
    U8 mu8EepData = 0U;
    /* WIFI ON/OFF POWER */
    mu8EepData = WifiReadEep(EEPROM_ADDR_WIFI_POWER);
#if ( TEST_B1_OTA == 1 )
    mu8EepData = SET;
#endif
#if ( TEST_WIFI_PJT == 1 )
    mu8EepData = SET;
#endif
    if ( mu8EepData <= ON )
    {
        SetWifiControlStatus(STATUS_CON_POWER,mu8EepData);
    }
    else
    {
        WifiWriteEep(EEPROM_ADDR_WIFI_POWER,CLEAR);
        SetWifiControlStatus(STATUS_CON_POWER,OFF);
    }
    
    /* WIFI FIRST PARING */
    mu8EepData = WifiReadEep(EEPROM_ADDR_FIRST_PARING);
#if ( TEST_B1_OTA == 1 )
    mu8EepData = SET;
#endif
#if ( TEST_WIFI_PJT == 1 )
    mu8EepData = SET;
#endif
    if ( mu8EepData <= SET )
    {
        SetWifiApStatus(STATUS_AP_FIRST,mu8EepData);
    }
    else
    {
        WifiWriteEep(EEPROM_ADDR_FIRST_PARING,CLEAR);
        SetWifiApStatus(STATUS_AP_FIRST,CLEAR);
    }

    /* WIFI MODULE FOTA */
    mu8EepData = WifiReadEep(EEPROM_ADDR_FOTA_MODULE);
    if ( mu8EepData <= SET )
    {
        SetWifiFotaStatus(STATUS_FOTA_MODULE,mu8EepData);
    }
    else
    {
        WifiWriteEep(EEPROM_ADDR_FOTA_MODULE,CLEAR);
        SetWifiFotaStatus(STATUS_FOTA_MODULE,CLEAR);
    }

    /* WIFI MCU FOTA */
    mu8EepData = WifiReadEep(EEPROM_ADDR_FOTA_MCU);
    if ( mu8EepData <= SET )
    {
        SetWifiFotaStatus(STATUS_FOTA_MCU,mu8EepData);
    }
    else
    {
        WifiWriteEep(EEPROM_ADDR_FOTA_MCU,CLEAR);
        SetWifiFotaStatus(STATUS_FOTA_MCU,CLEAR);
    }

    /* ON/OFF Operation */
    if ( GetWifiControlStatus(STATUS_CON_POWER) == ON )
    {
        HAL_SetWifiOnOff(ON);
    }
    else
    {
        HAL_SetWifiOnOff(OFF);
    }
}

void WifiFactorySetting ( void )
{
    WifiWriteEep(EEPROM_ADDR_WIFI_POWER,CLEAR);
    WifiWriteEep(EEPROM_ADDR_FIRST_PARING,CLEAR);
    WifiWriteEep(EEPROM_ADDR_FOTA_MODULE,CLEAR);
    WifiWriteEep(EEPROM_ADDR_FOTA_MCU,CLEAR);
}

void WifiModuleOnOffSeq ( void )
{
    U8 mu8Seq = 0U;
    
    mu8Seq = GetWifiModuleStatus(STATUS_MODULE_SEQ);

    switch ( mu8Seq )
    {
        case MODULE_OFF_START :
            TURN_OFF_WIFI_RST();
            SetWifiModuleStatus(STATUS_MODULE_SEQ,MODULE_OFF_END);
        break;

        case MODULE_OFF_END :
            TURN_OFF_WIFI_POW();
            SetWifiModuleStatus(STATUS_MODULE_SEQ,MODULE_NULL);
            SetWifiModuleStatus(STATUS_MODULE_POWER,CLEAR);
        break;

        case MODULE_ON_START :
            TURN_ON_WIFI_POW();
            TURN_ON_WIFI_RST();
            SetWifiModuleStatus(STATUS_MODULE_SEQ,MODULE_ON_WAIT);
        break;

        case MODULE_ON_WAIT :
            TURN_OFF_WIFI_RST();
            SetWifiModuleStatus(STATUS_MODULE_SEQ,MODULE_ON_END);
        break;

        case MODULE_ON_END :
            TURN_ON_WIFI_RST();
            SetWifiModuleStatus(STATUS_MODULE_SEQ,MODULE_NULL);
            SetWifiModuleStatus(STATUS_MODULE_POWER,CLEAR);
        break;

        case MODULE_RESET_START :
            TURN_ON_WIFI_POW();
            TURN_OFF_WIFI_RST();
            SetWifiModuleStatus(STATUS_MODULE_SEQ,MODULE_RESET_END);
        break;

        case MODULE_RESET_END :
            TURN_ON_WIFI_RST();
            SetWifiModuleStatus(STATUS_MODULE_SEQ,MODULE_NULL);
            SetWifiModuleStatus(STATUS_MODULE_POWER,CLEAR);
        break;

        default :
                
        break;
    }

}

