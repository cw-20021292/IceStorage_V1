
#ifndef _WIFI_TxForm_H_
#define _WIFI_TxForm_H_

/***** Tx Parameter ***********************************************************/
typedef enum
{
    TYPE_AT_CMD = 0x01,
    TYPE_STOP_CMD,
    TYPE_FUNC_CMD,
    TYPE_FUNC_CONTROL_CMD,
    TYPE_SENSOR_CMD,
    TYPE_ERROR_CMD,
    TYPE_PARA_CMD,
    TYPE_PART_CMD,
    TYPE_EXAMINE_CMD,
    TYPE_ACCUMULATE_CMD,
    TYPE_INFO_RQST_CMD,
    TYPE_INFO_RQST_USER_CMD,
    TYPE_INFO_RQST_DATA_CMD,
    TYPE_MONITOR_DATA_CMD,
    TYPE_BLE_CERT_CMD

}E_WIFI_TX_TYPE_T;

typedef struct _wifi_send_list_S
{
    U32 Id;
    U8 mu8Para;
    U8 mu8Seq;
} WifiTxTypeList_T;

typedef struct _wifi_txcmd_list_
{
    U8  Para;               // Matching Parameter
    E_WIFI_TX_TYPE_T  Type; // Command Type : table matching (Func/Err/Sensor/Para/Part/Accumulat/Info)
    U8  Target;             // To Module / To Server
    I8* AtCmd;              // AT Cmd
    I8* Api;                // API No
    U8	ReId;               // Request ID Usage Status
    WifiExFun_T Func;
} WifiTxCommandList_T;


// Device -> Server Protocol Packet Parameter ±¸ºÐÀÚ
enum
{
    PARA_NULL           = 0x00,
    PARA_AT_APMODE,
    PARA_AT_BLE_AP,
    PARA_AT_VERSION,
    PARA_AT_TESTMODE,
    PARA_AT_AUTO_OFF,
    PARA_AT_AUTO_ON,
    PARA_AT_BLE_ADV,
    PARA_AT_SCONN,
    PARA_AT_BLE_OFF,
    PARA_AT_DIRECTFOTAMODE, // direct fota
    
    PARA_AT_FOTA_CHECK,
    PARA_AT_SIZE_ERR,
    PARA_AT_TYPE_ERR,
    PARA_AT_DEVICE_RUN,
    PARA_AT_DEVICE_ERR, // 15
    
    PARA_STOP_CONNECT, // 16
    PARA_RECV_RESPONSE,
    PARA_STATUS_RESPONSE,
    PARA_SENSOR_RESPONSE,
    PARA_ERROR_RESPONSE,
    PARA_PARA_RESPONSE,
    PARA_STATUS_SEND,
    PARA_STATUS_SEND_CONTROL,
    PARA_SENSOR_SEND,
    PARA_ERROR_SEND,
    PARA_PARA_SEND,
    PARA_ERROR_SELF,
    PARA_FUNC_EXCEPT, // 28
    
    PARA_PART_SEND,
    PARA_PART_RESPONSE,
    PARA_EXAMINE_SEND,
    PARA_ACCUMULATE_SEND,
    PARA_ACCUMULATE_RESPONSE, // 33

    PARA_INFO_REQUEST,
    PARA_INFO_REQUEST_USER,
    PARA_INFO_REQUEST_DATA,
    PARA_MONITOR_RESPONSE_W,
    PARA_MONITOR_RESPONSE_R,

    PARA_BLE_CERT_TX,
};

/*******************************************************************************
FUNCTION NAME: GetWifiTxTypeEventList / GetWifiTxTypeListSize

DESCRIPTION: Return Table & Size

PARAMETERS: mType (Tx Matching Parameter)

RETURN VALUE: WifiSendAtList / WifiSendDataList

NOTES: Call Function (WIFI_TxMake)
*******************************************************************************/
void* GetWifiTxTypeEventList ( U8 mType );
U16 GetWifiTxTypeListSize ( U8 mType );

/*******************************************************************************
FUNCTION NAME: GetWifiTxCmdEventList / GetWifiTxCmdListSize

DESCRIPTION: Return Table & Size

PARAMETERS: Void

RETURN VALUE: WifiTxCmdList

NOTES: Call Function (WIFI_TxMake)
*******************************************************************************/
// void* GetWifiTxCmdEventList ( void );
// U16 GetWifiTxCmdListSize ( void );

void* GetWifiTxCmdEventList ( U8 mType  );
U16 GetWifiTxCmdListSize ( U8 mType  );

#endif

