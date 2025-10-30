
#ifndef _Wifi_Status_H_
#define _Wifi_Status_H_


void InitializeWifiStatus ( void );

void HAL_SetWifiOnOff ( U8 mu8State );

/***** Module Hal STATUS ******************************************************/
typedef enum
{
    STATUS_MODULE_POWER = 0x00, // on/reset/off 
    STATUS_MODULE_SEQ,          // on/reset/off  Sequence
		
    STATUS_MODULE_NUM
} E_WIFI_STATUS_MODULE_T;
U8 GetWifiModuleStatus ( E_WIFI_STATUS_MODULE_T mId );
void SetWifiModuleStatus ( E_WIFI_STATUS_MODULE_T mId , U8 mu8Status );

// Wifi on/reset/off Sequence
enum
{
    MODULE_NULL = 0x00,
	MODULE_OFF_START = 0x01,
	MODULE_OFF_END,
	MODULE_ON_START = 0x11,
	MODULE_ON_WAIT,
	MODULE_ON_END,
	MODULE_RESET_START = 0x21,
	MODULE_RESET_END
};


/***** Control STATUS *********************************************************/
typedef enum
{
    STATUS_CON_POWER = 0x00,    // WIFI  (ON / OFF)
    STATUS_CON_READY,           // 
		
    STATUS_CON_NUM
} E_WIFI_STATUS_CON_T;

U8 GetWifiControlStatus ( E_WIFI_STATUS_CON_T mId );
void SetWifiControlStatus ( E_WIFI_STATUS_CON_T mId , U8 mu8Status );


/***** Server STATUS **********************************************************/
typedef enum
{
    STATUS_SVR_CONNECT = 0x00,  //  (SET : / CLEAR : )
    STATUS_SVR_END,             //  (SET :  / CLEAR : )
		
    STATUS_SVR_NUM
} E_WIFI_STATUS_SVR_T;

U8 GetWifiServerStatus ( E_WIFI_STATUS_SVR_T mId );
void SetWifiServerStatus ( E_WIFI_STATUS_SVR_T mId , U8 mu8Status );


/***** Initialize STATUS ******************************************************/
typedef enum
{
    STATUS_INI_FINISH = 0x00,   //  Status (SET :  / CLEAR :  )
    STATUS_INI_SEQ,             //  Sequence
    STATUS_INI_INFO,
    
    STATUS_INI_NUM
} E_WIFI_STATUS_INI_T;

U8 GetWifiIniStatus ( E_WIFI_STATUS_INI_T mId );
void SetWifiIniStatus ( E_WIFI_STATUS_INI_T mId , U8 mu8Status );

// Wifi Initial Sequence
enum
{
    INI_SEQ_NULL = 0x00,
    INI_SEQ_SERVER_CONNECT,
    INI_SEQ_RQST_INFO,
    INI_SEQ_ACK_INFO,
    INI_SEQ_SEND_STATUS,
    INI_SEQ_ACK_STATUS,
    INI_SEQ_SEND_SENSOR,
    INI_SEQ_ACK_SENSOR,
    INI_SEQ_SEND_ERROR,
    INI_SEQ_ACK_ERROR,
    INI_SEQ_SEND_PART,
    INI_SEQ_ACK_PART,
    INI_SEQ_SEND_ACCUMULATE,
    INI_SEQ_ACK_ACCUMULATE,
    INI_SEQ_FINISHED
};


/***** Tx STATUS **************************************************************/
typedef enum
{
    STATUS_TX_STATE = 0x00,     // Tx Status (SET : Tx  / CLEAR : Ack )
    STATUS_TX_SEQ,              // TX Sequence
    STATUS_TX_SENSOR_STATE,
		
    STATUS_TX_NUM
} E_WIFI_STATUS_TX_T;

U8 GetWifiTxStatus ( E_WIFI_STATUS_TX_T mId );
void SetWifiTxStatus ( E_WIFI_STATUS_TX_T mId , U8 mu8Status );

// Wifi Tx Data To Server Sequence
enum
{
    WIFI_TX_SEQ_NULL = 0x00,
    WIFI_TX_SEQ_ATCMD,
    WIFI_TX_SEQ_APMODE,
    WIFI_TX_SEQ_STOP,
    WIFI_TX_SEQ_SEND
};


/***** FOTA STATUS ************************************************************/
typedef enum
{
    STATUS_FOTA_SEQ = 0x00, // FOTA $)C4k1b (SET : 4k1b / CLEAR : >x@=)
    STATUS_FOTA_MODULE,     // $)C8p5b FOTA ;sEB (SET : FOTA AxG`A_ / CLEAR : 0|0h>x@=)
    STATUS_FOTA_MCU,
		
    STATUS_FOTA_NUM
} E_WIFI_STATUS_FOTA_T;

U8 GetWifiFotaStatus ( E_WIFI_STATUS_FOTA_T mId );
void SetWifiFotaStatus ( E_WIFI_STATUS_FOTA_T mId , U8 mu8Status );

// Fota Seq
enum
{
    FOTA_NULL = 0x00,
    FOTA_RESPONSE,
    FOTA_CHECK,
    FOTA_DISCONNECT,
    FOTA_WAIT,
    FOTA_START,
    FOTA_RUN
};

/***** AP STATUS **************************************************************/
typedef enum
{
    STATUS_AP_FIRST = 0x00, // $)CCVCJ Fd>n85 (SET : ?O7a / CLEAR : 9L?O7a)
    STATUS_AP_WIFI,         // AP MODE (SET : $)CFd>n858p5e / CLEAR : @O9]8p5e)
    STATUS_AP_BLE,
    STATUS_AP_BLE_CERT,     // ¡°¿Ø¿Œ¡ı KEY∞° ¡∂¿€¿Ã ∞°¥…«— ªÛ≈¬ ¿Œ¡ˆ? (0:¡°¿Ø¿Œ¡ı∫“∞°ªÛ≈¬/1:¡°¿Ø¿Œ¡ı∞°¥…ªÛ≈¬/2:¡°¿Ø¿Œ¡ıøœ∑·ªÛ≈¬)

    STATUS_AP_NUM
} E_WIFI_STATUS_AP_T;

U8 GetWifiApStatus ( E_WIFI_STATUS_AP_T mId );
void SetWifiApStatus ( E_WIFI_STATUS_AP_T mId , U8 mu8Status );


/***** Indicate STATUS ********************************************************/
typedef enum
{
    STATUS_DISPLAY = 0x00,  // Wifi Led Display
    STATUS_FACTORY,
    STATUS_ERROR,           // Wifi Error Display
    STATUS_AP_ERROR,
    STATUS_PARING,
    STATUS_FACTORY_RXTX,
    // STATUS_DISPLAY_COUNT,

    STATUS_IDC_NUM
} E_WIFI_STATUS_IDC_T;

U8 GetWifiIndicateStatus ( E_WIFI_STATUS_IDC_T mId );
void SetWifiIndicateStatus ( E_WIFI_STATUS_IDC_T mId , U8 mu8Status );

/***** Send Tx Data STATUS Control ****************************************************/
U8 GetWifiSendStatusControl ( void );
void SetWifiSendStatusControl ( U8 mu8Status );

/***** Send Tx Data STATUS ****************************************************/
typedef enum
{
    TX_ATCMD = 0x00,    // TX AT CMD (To Module)
    TX_DATA,            // TX DATA (To Server)
		
    STATUS_SEND_NUM
} E_WIFI_STATUS_SEND_T;

U32 GetWifiSendStatus ( E_WIFI_STATUS_SEND_T mId );
void SetWifiSendStatus ( E_WIFI_STATUS_SEND_T mId , U32 mu32Status );
void ClearWifiSendStatus ( E_WIFI_STATUS_SEND_T mId , U32 mu32Status );


/***** Send BLE/Wifi Tx Data STATUS ****************************************************/
typedef enum
{
    TX_BLE_WIFI = 0x00,    // WIFI=0, BLE=1
    BLE_RECV_CHECK,    
    STATUS_SEND_WIFI_BLE_NUM
} E_WIFI_STATUS_SEND_WIFI_BLE_T;

U8 GetWifiBLESendStatus ( E_WIFI_STATUS_SEND_WIFI_BLE_T mId );
void SetWifiBLESendStatus ( E_WIFI_STATUS_SEND_WIFI_BLE_T mId , U8 mu8Status );
void ClearWifi_BLE_SendStatus ( E_WIFI_STATUS_SEND_WIFI_BLE_T mId , U8 mu8Status );

/***** Send Cert Data STATUS NUMBER ****************************************************/

// A7010.0002 ªÛ≈¬
typedef enum
{
    STATUS_CERT_NOTUSED = 0x00,  // πÃªÁøÎ
    STATUS_CERT_ON,              // ¡°¿Ø¿Œ¡ı ON
    STATUS_CERT_FAIL,            // ¡°¿Ø¿Œ¡ı FAIL

    STATUS_CERT_NUM  
};

// A7010.0003 ∞·∞˙
typedef enum
{
    RESULT_CERT_NOTUSED = 0x00,  // πÃªÁøÎ
    RESULT_CERT_SUCCESS,         // ¡°¿Ø¿Œ¡ı º∫∞¯
    RESULT_CERT_TIME_OUT,        // ¡°¿Ø¿Œ¡ı Ω√∞£∏∏∑·
    RESULT_CERT_ERROR,           // ¡°¿Ø¿Œ¡ı ±‚≈∏ ø°∑Ø

    RESULT_CERT_NUM  
};


U8 GetWifiCertID (void);
void SetWifiCertID (U8 mu8ID);
U8 GetWifiCertResult (void);
void SetWifiCertResult (U8 mu8Result);
U8 GetWifiCertStatus (void);
void SetWifiCertStatus (U8 mu8Status);
/* AT Cmd */
#define WIFI_AT_NULL            0x00000000
#define WIFI_AT_APMODE          0x00000001
#define WIFI_AT_BLE_AP          0x00000002
#define WIFI_AT_VERSION         0x00000004
#define WIFI_AT_TESTMODE        0x00000008
#define WIFI_AT_AUTO_OFF        0x00000010
#define WIFI_AT_AUTO_ON         0x00000020
#define WIFI_AT_BLE_ADV         0x00000040
#define WIFI_AT_SCONN           0x00000080
#define WIFI_AT_BLE_OFF         0x00000100
#define WIFI_AT_DIRECTFOTAMODE  0x00000200 // direct fota

#define WIFI_AT_FOTA_CHECK      0x00010000
#define WIFI_AT_SIZE_ERR        0x00020000
#define WIFI_AT_TYPE_ERR        0x00040000
#define WIFI_AT_DEVICE_RUN      0x00080000
#define WIFI_AT_DEVICE_ERR      0x00100000

/* Tx Cmd */
#define WIFI_TX_NULL            0x00000000
#define WIFI_TX_SEND_STOP       0x00000001
#define WIFI_TX_RESP_RECV       0x00000002
#define WIFI_TX_RESP_FUNC       0x00000004
#define WIFI_TX_RESP_SENSOR     0x00000008

#define WIFI_TX_RESP_ERR        0x00000010
#define WIFI_TX_RESP_PARA       0x00000020
#define WIFI_TX_SEND_FUNC       0x00000040
#define WIFI_TX_SEND_SENSOR     0x00000080

#define WIFI_TX_SEND_ERR        0x00000100
#define WIFI_TX_SEND_PARA       0x00000200
#define WIFI_TX_SELF_ERR        0x00000400
#define WIFI_TX_EXCEPT_FUNC     0x00000800

#define WIFI_TX_SEND_PART       0x00001000
#define WIFI_TX_RESP_PART       0x00002000
#define WIFI_TX_SEND_EXAMINE    0x00004000
#define WIFI_TX_SEND_ACCUMULATE 0x00008000

#define WIFI_TX_RESP_ACCUMULATE 0x00010000
#define WIFI_TX_RQST_INFO       0x00020000
#define WIFI_TX_RQST_INFO_USER  0x00040000
#define WIFI_TX_RQST_INFO_DATA  0x00080000
#define WIFI_TX_RESP_MNT_DATA_W 0x00100000
#define WIFI_TX_RESP_MNT_DATA_R 0x00200000
#define WIFI_TX_SEND_FUNC_CONTROL 0x00400000

#define WIFI_TX_SEND_BLE_CERT   0x00800000
    
/***** ETC ********************************************************************/
// WIFI ALL CLEAR
void ClearWifiAllStatus ( void );
void WifiOnOffSetting ( void );
void WifiFactorySetting ( void );
void WifiModuleOnOffSeq ( void );

#endif

