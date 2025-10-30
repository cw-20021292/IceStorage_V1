
/*******************************************************************************
&Description :
    AT Cmd / Tx Data Form
    Select Tx Type & Tx Cmd
    
&Histiry : 
    ShinHM 2021.02.22 update
*******************************************************************************/


/***** Header *****************************************************************/
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_TxForm.h"
#include "WIFI_Status.h"
#include "WIFI_UserInterface.h"


/***** Define *****************************************************************/
/* AT Command (MCU -> Wifi Module) */
const signed char AT_CMD_SEND[]         = "AT*ICT*AWS_SEND=";
const signed char AT_CMD_BLE_SEND[]     = "AT*ICT*BLE_SEND=";       // BLE DATA

const signed char AT_CMD_VERSION[]      = "AT*ICT*AWS_SET=11 ";
const signed char AT_CMD_TESTMODE[]     = "AT*ICT*SMODE=99";        // Factory : coway_iot_01 / 12345678
const signed char AT_CMD_DIRECTFOTA[]     = "AT*ICT*SMODE=98";         // Direct FOTA
const signed char AT_CMD_AUTO_OFF[]     = "AT*ICT*AUCONMODE=0 0";
const signed char AT_CMD_AUTO_ON[]      = "AT*ICT*AUCONMODE=0 1";
const signed char AT_CMD_APMODE[]       = "AT*ICT*SMODE=0 ";
const signed char AT_CMD_SCONN[]        = "AT*ICT*SCONN=";
const signed char AT_BLE_APMODE[]       = "AT*ICT*BLE_STATE=2";
const signed char AT_BLE_ADV[]          = "AT*ICT*BLE_ADV=";
const signed char AT_BLE_OFF[]          = "AT*ICT*BLE_STATE=0";

const signed char AT_CMD_FOTA_CHECK[]   = "AT*ICT*OTA_SEND_START";  // "AT*ICT*OTA_SEND_DATA=0";
const signed char AT_CMD_SIZE_ERR[]     = "AT*ICT*OTA_SEND_END=42";
const signed char AT_CMD_TYPE_ERR[]     = "AT*ICT*OTA_SEND_END=41";
const signed char AT_CMD_DEVICE_RUN[]   = "AT*ICT*OTA_SEND_END=43";
const signed char AT_CMD_DEVICE_ERR[]   = "AT*ICT*OTA_SEND_END=44";

/* API No (MCU -> Server) */
const signed char API_TX_STOP_CONNECT[]         = "A1002";  // 서버연결종료
const signed char API_TX_STATUS_SEND[]          = "A1010";  // 상태정보전송/제어응답
const signed char API_TX_STATUS_RESPONSE[]      = "A1012";  // 상태정보응답
const signed char API_TX_STATUS_SEND_CONTROL[]  = "A1014";  // 제어상태정보전송
const signed char API_TX_SENSOR_SEND[]          = "A1020";  // 센서정보전송
const signed char API_TX_SENSOR_RESPONSE[]      = "A1022";  // 센서정보응답
const signed char API_TX_ERROR_SEND[]           = "A1030";  // 에러정보전송
const signed char API_TX_ERROR_RESPONSE[]       = "A1032";  // 에러정보응답
const signed char API_TX_ERROR_SELF_RESPONSE[]  = "A1034";  // 자가진단응답
const signed char API_TX_PARAMETER_SEND[]       = "A1050";  // 파라미터전송
const signed char API_TX_PARAMETER_RESPONSE[]   = "A1052";  // 파라미터응답

const signed char API_TX_PART_SEND[]            = "A1080";  // 고장진단전송
const signed char API_TX_PART_RESPONSE[]        = "A1082";  // 고장진단응답
const signed char API_TX_EXAMINE_SEND[]         = "A1090";  // 정밀진단전송
const signed char API_TX_ACCUMULATE_SEND[]      = "A10A0";  // 누적정보전송
const signed char API_TX_ACCUMULATE_RESPONSE[]  = "A10A2";  // 누적정보응답

const signed char API_TX_INFO_REQUEST[]         = "A2010";  // 부가정보요청
const signed char API_TX_BLE_CERT[]             = "A7010";  // BLE Cert TX (Res)
// const signed char API_TX_MONITOR_RESPONSE[]     = "A9010";  // RAM값 응답
const signed char API_TX_MONITOR_RESPONSE_W[]   = "A9010";  // RAM값 응답 (Write)
const signed char API_TX_MONITOR_RESPONSE_R[]   = "A9012";  // RAM값 응답 (Read)


/***** Variable ***************************************************************/


/***** Constant ***************************************************************/


/***** Static Function ********************************************************/
static void ExceptAtCmd ( U8 mExcept, I8* pBuf, U16 mLen );


/***** Table ******************************************************************/
/* AT CMD & TX DATA 우선순위 설정 및 전송 */
/* To Module (AT CMD) */
static const WifiTxTypeList_T WifiSendAtList[] =
{ /*    Tx ID                       Tx Parameter                Tx Sequence         */
    {   WIFI_AT_APMODE,             PARA_AT_APMODE,             WIFI_TX_SEQ_APMODE  },
    {   WIFI_AT_BLE_AP,             PARA_AT_BLE_AP,             WIFI_TX_SEQ_APMODE  },    
    {   WIFI_AT_VERSION,            PARA_AT_VERSION,            WIFI_TX_SEQ_ATCMD   },
    {   WIFI_AT_TESTMODE,           PARA_AT_TESTMODE,           WIFI_TX_SEQ_ATCMD   },
    {   WIFI_AT_AUTO_OFF,           PARA_AT_AUTO_OFF,           WIFI_TX_SEQ_ATCMD   },
    {   WIFI_AT_AUTO_ON,            PARA_AT_AUTO_ON,            WIFI_TX_SEQ_ATCMD   },
    {   WIFI_AT_BLE_ADV,            PARA_AT_BLE_ADV,            WIFI_TX_SEQ_ATCMD   },
    {   WIFI_AT_SCONN,              PARA_AT_SCONN,              WIFI_TX_SEQ_ATCMD   },
    {   WIFI_AT_BLE_OFF,            PARA_AT_BLE_OFF,            WIFI_TX_SEQ_ATCMD   },
    {   WIFI_AT_DIRECTFOTAMODE,     PARA_AT_DIRECTFOTAMODE,     WIFI_TX_SEQ_ATCMD   }, // direct fota
        
    {   WIFI_AT_FOTA_CHECK,         PARA_AT_FOTA_CHECK,         WIFI_TX_SEQ_ATCMD   },
    {   WIFI_AT_SIZE_ERR,           PARA_AT_SIZE_ERR,           WIFI_TX_SEQ_ATCMD   },
    {   WIFI_AT_TYPE_ERR,           PARA_AT_TYPE_ERR,           WIFI_TX_SEQ_ATCMD   },
    {   WIFI_AT_DEVICE_RUN,         PARA_AT_DEVICE_RUN,         WIFI_TX_SEQ_ATCMD   },
    {   WIFI_AT_DEVICE_ERR,         PARA_AT_DEVICE_ERR,         WIFI_TX_SEQ_ATCMD   }
    
};
#define SZ_ATCMD_LIST  ( sizeof(WifiSendAtList) / sizeof(WifiTxTypeList_T) )

/* To Server */
static const WifiTxTypeList_T WifiSendDataList[] =
{ /*    Tx ID                       Tx Parameter                Tx Sequence         */					
    {   WIFI_TX_SEND_STOP,          PARA_STOP_CONNECT,          WIFI_TX_SEQ_STOP    },
    {   WIFI_TX_RESP_RECV,          PARA_RECV_RESPONSE,         WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_RESP_FUNC,          PARA_STATUS_RESPONSE,       WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_RESP_SENSOR,        PARA_SENSOR_RESPONSE,       WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_RESP_ERR,           PARA_ERROR_RESPONSE,        WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_RESP_PARA,          PARA_PARA_RESPONSE,         WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_SEND_FUNC,          PARA_STATUS_SEND,           WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_SEND_FUNC_CONTROL,  PARA_STATUS_SEND_CONTROL,   WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_SEND_SENSOR,        PARA_SENSOR_SEND,           WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_SEND_ERR,           PARA_ERROR_SEND,            WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_SEND_PARA,          PARA_PARA_SEND,             WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_SELF_ERR,           PARA_ERROR_SELF,            WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_EXCEPT_FUNC,        PARA_FUNC_EXCEPT,           WIFI_TX_SEQ_SEND    },
        
    {   WIFI_TX_SEND_PART,          PARA_PART_SEND,             WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_RESP_PART,          PARA_PART_RESPONSE,         WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_SEND_EXAMINE,       PARA_EXAMINE_SEND,          WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_SEND_ACCUMULATE,    PARA_ACCUMULATE_SEND,       WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_RESP_ACCUMULATE,    PARA_ACCUMULATE_RESPONSE,   WIFI_TX_SEQ_SEND    },
        
    {   WIFI_TX_RQST_INFO,          PARA_INFO_REQUEST,          WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_RQST_INFO_USER,     PARA_INFO_REQUEST_USER,     WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_RQST_INFO_DATA,     PARA_INFO_REQUEST_DATA,     WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_RESP_MNT_DATA_W,    PARA_MONITOR_RESPONSE_W,    WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_RESP_MNT_DATA_R,    PARA_MONITOR_RESPONSE_R,    WIFI_TX_SEQ_SEND    },
    {   WIFI_TX_SEND_BLE_CERT,      PARA_BLE_CERT_TX,           WIFI_TX_SEQ_SEND    }
        
};
#define SZ_DATA_LIST  ( sizeof(WifiSendDataList) / sizeof(WifiTxTypeList_T) )

/* Wifi TxCmd Parameter Table */
static const WifiTxCommandList_T WifiTxCmdList[] =
{ /*    Cmd Parameter Matching ID   Cmd Type(List)          Module/Server   AT Cmd              API                         RequestId   Except      */
    {   PARA_AT_APMODE,             TYPE_AT_CMD,            TO_APMODE,      AT_CMD_APMODE,      NULL,                       UNUSED,     ExceptAtCmd },
    {   PARA_AT_BLE_AP,             TYPE_AT_CMD,            TO_MODULE,      AT_BLE_APMODE,      NULL,                       UNUSED,     NULL        },
    {   PARA_AT_VERSION,            TYPE_AT_CMD,            TO_VERSION,     AT_CMD_VERSION,     NULL,                       UNUSED,     ExceptAtCmd },
    {   PARA_AT_TESTMODE,           TYPE_AT_CMD,            TO_MODULE,      AT_CMD_TESTMODE,    NULL,                       UNUSED,     NULL        },
    {   PARA_AT_AUTO_OFF,           TYPE_AT_CMD,            TO_MODULE,      AT_CMD_AUTO_OFF,    NULL,                       UNUSED,     NULL        },
    {   PARA_AT_AUTO_ON,            TYPE_AT_CMD,            TO_MODULE,      AT_CMD_AUTO_ON,     NULL,                       UNUSED,     NULL        },
    {   PARA_AT_BLE_ADV,            TYPE_AT_CMD,            TO_BLEADV,      AT_BLE_ADV,         NULL,                       UNUSED,     ExceptAtCmd },
    {   PARA_AT_SCONN,              TYPE_AT_CMD,            TO_SCONN,       AT_CMD_SCONN,       NULL,                       UNUSED,     ExceptAtCmd },
    {   PARA_AT_BLE_OFF,            TYPE_AT_CMD,            TO_MODULE,      AT_BLE_OFF,         NULL,                       UNUSED,     NULL        },
        
    {   PARA_AT_FOTA_CHECK,         TYPE_AT_CMD,            TO_MODULE,      AT_CMD_FOTA_CHECK,  NULL,                       UNUSED,     NULL        },
    {   PARA_AT_SIZE_ERR,           TYPE_AT_CMD,            TO_MODULE,      AT_CMD_SIZE_ERR,    NULL,                       UNUSED,     NULL        },
    {   PARA_AT_TYPE_ERR,           TYPE_AT_CMD,            TO_MODULE,      AT_CMD_TYPE_ERR,    NULL,                       UNUSED,     NULL        },
    {   PARA_AT_DEVICE_RUN,         TYPE_AT_CMD,            TO_MODULE,      AT_CMD_DEVICE_RUN,  NULL,                       UNUSED,     NULL        },
    {   PARA_AT_DEVICE_ERR,         TYPE_AT_CMD,            TO_MODULE,      AT_CMD_DEVICE_ERR,  NULL,                       UNUSED,     NULL        },
    {   PARA_AT_DIRECTFOTAMODE,     TYPE_AT_CMD,            TO_MODULE,      AT_CMD_DIRECTFOTA,  NULL,                       UNUSED,     NULL        }, // direct fota
        
    {   PARA_STOP_CONNECT,          TYPE_STOP_CMD,          TO_SERVER,	    AT_CMD_SEND,        API_TX_STOP_CONNECT,        UNUSED,     NULL        },
    {   PARA_RECV_RESPONSE,         TYPE_FUNC_CMD,          TO_SERVER,	    AT_CMD_SEND,        API_TX_STATUS_SEND,         USED,       NULL        },
    {   PARA_STATUS_RESPONSE,       TYPE_FUNC_CMD,          TO_SERVER,	    AT_CMD_SEND,        API_TX_STATUS_RESPONSE,     USED,       NULL        },
    {   PARA_SENSOR_RESPONSE,       TYPE_SENSOR_CMD,        TO_SERVER,      AT_CMD_SEND,        API_TX_SENSOR_RESPONSE,     USED,       NULL        },
    {   PARA_ERROR_RESPONSE,        TYPE_ERROR_CMD,         TO_SERVER, 	    AT_CMD_SEND,        API_TX_ERROR_RESPONSE,      USED,       NULL        },
    {   PARA_PARA_RESPONSE,         TYPE_PARA_CMD,          TO_SERVER, 	    AT_CMD_SEND,        API_TX_PARAMETER_RESPONSE,  USED,       NULL        },
    {   PARA_STATUS_SEND,           TYPE_FUNC_CMD,          TO_SERVER,	    AT_CMD_SEND,        API_TX_STATUS_SEND,         UNUSED,     NULL        },
    {   PARA_STATUS_SEND_CONTROL,   TYPE_FUNC_CONTROL_CMD,  TO_SERVER,	    AT_CMD_SEND,        API_TX_STATUS_SEND_CONTROL, UNUSED,     NULL        },
    {   PARA_SENSOR_SEND,           TYPE_SENSOR_CMD,        TO_SERVER,	    AT_CMD_SEND,        API_TX_SENSOR_SEND,         UNUSED,     NULL        },
    {   PARA_ERROR_SEND,            TYPE_ERROR_CMD,         TO_SERVER, 	    AT_CMD_SEND,        API_TX_ERROR_SEND,          UNUSED,     NULL        },
    {   PARA_PARA_SEND,             TYPE_PARA_CMD,          TO_SERVER, 	    AT_CMD_SEND,        API_TX_PARAMETER_SEND,      USED,       NULL        },
    {   PARA_ERROR_SELF,            TYPE_ERROR_CMD,         TO_SERVER,      AT_CMD_SEND,        API_TX_ERROR_SELF_RESPONSE, USED,       NULL        },
    {   PARA_FUNC_EXCEPT,           TYPE_FUNC_CMD,          TO_SERVER,      AT_CMD_SEND,        API_TX_STATUS_SEND,         UNUSED,     NULL        },
        
    {   PARA_PART_SEND,             TYPE_PART_CMD,          TO_SERVER, 	    AT_CMD_SEND,        API_TX_PART_SEND,           UNUSED,     NULL        },
    {   PARA_PART_RESPONSE,         TYPE_PART_CMD,          TO_SERVER, 	    AT_CMD_SEND,        API_TX_PART_RESPONSE,       USED,       NULL        },
    {   PARA_EXAMINE_SEND,          TYPE_PART_CMD,          TO_SERVER,	    AT_CMD_SEND,        API_TX_EXAMINE_SEND,        REUSED,     NULL        },
    {   PARA_ACCUMULATE_SEND,       TYPE_ACCUMULATE_CMD,    TO_SERVER, 	    AT_CMD_SEND,        API_TX_ACCUMULATE_SEND,     UNUSED,     NULL        },
    {   PARA_ACCUMULATE_RESPONSE,   TYPE_ACCUMULATE_CMD,    TO_SERVER, 	    AT_CMD_SEND,        API_TX_ACCUMULATE_RESPONSE, USED,       NULL        },
        
    {   PARA_INFO_REQUEST,          TYPE_INFO_RQST_CMD,     TO_SERVER, 	    AT_CMD_SEND,        API_TX_INFO_REQUEST,        UNUSED,     NULL        },
    {   PARA_INFO_REQUEST_USER,     TYPE_INFO_RQST_USER_CMD,TO_SERVER,      AT_CMD_SEND,        API_TX_INFO_REQUEST,        UNUSED,     NULL        },
    {   PARA_INFO_REQUEST_DATA,     TYPE_INFO_RQST_DATA_CMD,TO_SERVER,      AT_CMD_SEND,        API_TX_INFO_REQUEST,        UNUSED,     NULL        },
    {   PARA_MONITOR_RESPONSE_W,    TYPE_MONITOR_DATA_CMD,  TO_SERVER,      AT_CMD_SEND,        API_TX_MONITOR_RESPONSE_W,  USED,       NULL        },
    {   PARA_MONITOR_RESPONSE_R,    TYPE_MONITOR_DATA_CMD,  TO_SERVER,      AT_CMD_SEND,        API_TX_MONITOR_RESPONSE_R,  USED,       NULL        },
    {   PARA_BLE_CERT_TX,           TYPE_BLE_CERT_CMD,      TO_SERVER,      AT_CMD_BLE_SEND,    API_TX_BLE_CERT,            USED,       NULL        }
};
#define SZ_TX_CMD_LIST  ( sizeof(WifiTxCmdList) / sizeof(WifiTxCommandList_T) )

/* Wifi(BLE) TxCmd Parameter Table */
static const WifiTxCommandList_T Wifi_BLE_TxCmdList[] =
{ /*    Cmd Parameter Matching ID   Cmd Type(List)          Module/Server   AT Cmd (BLE)             API                         RequestId   Except      */

    {   PARA_AT_APMODE,             TYPE_AT_CMD,            TO_APMODE,      AT_CMD_APMODE,          NULL,                       UNUSED,     ExceptAtCmd },
    {   PARA_AT_BLE_AP,             TYPE_AT_CMD,            TO_MODULE,      AT_BLE_APMODE,          NULL,                       UNUSED,     NULL        },
    {   PARA_AT_VERSION,            TYPE_AT_CMD,            TO_VERSION,     AT_CMD_VERSION,         NULL,                       UNUSED,     ExceptAtCmd },
    {   PARA_AT_TESTMODE,           TYPE_AT_CMD,            TO_MODULE,      AT_CMD_TESTMODE,        NULL,                       UNUSED,     NULL        },
    {   PARA_AT_AUTO_OFF,           TYPE_AT_CMD,            TO_MODULE,      AT_CMD_AUTO_OFF,        NULL,                       UNUSED,     NULL        },
    {   PARA_AT_AUTO_ON,            TYPE_AT_CMD,            TO_MODULE,      AT_CMD_AUTO_ON,         NULL,                       UNUSED,     NULL        },
    {   PARA_AT_BLE_ADV,            TYPE_AT_CMD,            TO_BLEADV,      AT_BLE_ADV,             NULL,                       UNUSED,     ExceptAtCmd },
    {   PARA_AT_SCONN,              TYPE_AT_CMD,            TO_SCONN,       AT_CMD_SCONN,           NULL,                       UNUSED,     ExceptAtCmd },
    {   PARA_AT_BLE_OFF,            TYPE_AT_CMD,            TO_MODULE,      AT_BLE_OFF,             NULL,                       UNUSED,     NULL        },
    {   PARA_AT_DIRECTFOTAMODE,     TYPE_AT_CMD,            TO_MODULE,      AT_CMD_DIRECTFOTA,      NULL,                       UNUSED,     NULL        }, // direct fota

    {   PARA_AT_FOTA_CHECK,         TYPE_AT_CMD,            TO_MODULE,      AT_CMD_FOTA_CHECK,      NULL,                       UNUSED,     NULL        },
    {   PARA_AT_SIZE_ERR,           TYPE_AT_CMD,            TO_MODULE,      AT_CMD_SIZE_ERR,        NULL,                       UNUSED,     NULL        },
    {   PARA_AT_TYPE_ERR,           TYPE_AT_CMD,            TO_MODULE,      AT_CMD_TYPE_ERR,        NULL,                       UNUSED,     NULL        },
    {   PARA_AT_DEVICE_RUN,         TYPE_AT_CMD,            TO_MODULE,      AT_CMD_DEVICE_RUN,      NULL,                       UNUSED,     NULL        },
    {   PARA_AT_DEVICE_ERR,         TYPE_AT_CMD,            TO_MODULE,      AT_CMD_DEVICE_ERR,      NULL,                       UNUSED,     NULL        },

    {   PARA_STOP_CONNECT,          TYPE_STOP_CMD,          TO_SERVER,	    AT_CMD_SEND,            API_TX_STOP_CONNECT,        UNUSED,     NULL        },
    {   PARA_RECV_RESPONSE,         TYPE_FUNC_CMD,          TO_SERVER,	    AT_CMD_SEND,            API_TX_STATUS_SEND,         USED,       NULL        },
    {   PARA_STATUS_RESPONSE,       TYPE_FUNC_CMD,          TO_SERVER,	    AT_CMD_SEND,            API_TX_STATUS_RESPONSE,     USED,       NULL        },
    {   PARA_SENSOR_RESPONSE,       TYPE_SENSOR_CMD,        TO_SERVER,      AT_CMD_SEND,            API_TX_SENSOR_RESPONSE,     USED,       NULL        },
    {   PARA_ERROR_RESPONSE,        TYPE_ERROR_CMD,         TO_SERVER, 	    AT_CMD_SEND,            API_TX_ERROR_RESPONSE,      USED,       NULL        },
    {   PARA_PARA_RESPONSE,         TYPE_PARA_CMD,          TO_SERVER, 	    AT_CMD_SEND,            API_TX_PARAMETER_RESPONSE,  USED,       NULL        },
    {   PARA_STATUS_SEND,           TYPE_FUNC_CMD,          TO_SERVER,	    AT_CMD_SEND,            API_TX_STATUS_SEND,         UNUSED,     NULL        },
    {   PARA_STATUS_SEND_CONTROL,   TYPE_FUNC_CONTROL_CMD,  TO_SERVER,	    AT_CMD_SEND,            API_TX_STATUS_SEND_CONTROL, UNUSED,     NULL        },
    {   PARA_SENSOR_SEND,           TYPE_SENSOR_CMD,        TO_SERVER,	    AT_CMD_SEND,            API_TX_SENSOR_SEND,         UNUSED,     NULL        },
    {   PARA_ERROR_SEND,            TYPE_ERROR_CMD,         TO_SERVER, 	    AT_CMD_SEND,            API_TX_ERROR_SEND,          UNUSED,     NULL        },
    {   PARA_PARA_SEND,             TYPE_PARA_CMD,          TO_SERVER, 	    AT_CMD_SEND,            API_TX_PARAMETER_SEND,      USED,       NULL        },
    {   PARA_ERROR_SELF,            TYPE_ERROR_CMD,         TO_SERVER,      AT_CMD_SEND,            API_TX_ERROR_SELF_RESPONSE, USED,       NULL        },
    {   PARA_FUNC_EXCEPT,           TYPE_FUNC_CMD,          TO_SERVER,      AT_CMD_SEND,            API_TX_STATUS_SEND,         UNUSED,     NULL        },
        
    {   PARA_PART_SEND,             TYPE_PART_CMD,          TO_SERVER, 	    AT_CMD_SEND,            API_TX_PART_SEND,           UNUSED,     NULL        },
    {   PARA_PART_RESPONSE,         TYPE_PART_CMD,          TO_SERVER, 	    AT_CMD_SEND,            API_TX_PART_RESPONSE,       USED,       NULL        },
    {   PARA_EXAMINE_SEND,          TYPE_PART_CMD,          TO_SERVER,	    AT_CMD_SEND,            API_TX_EXAMINE_SEND,        REUSED,     NULL        },
    {   PARA_ACCUMULATE_SEND,       TYPE_ACCUMULATE_CMD,    TO_SERVER, 	    AT_CMD_SEND,            API_TX_ACCUMULATE_SEND,     UNUSED,     NULL        },
    {   PARA_ACCUMULATE_RESPONSE,   TYPE_ACCUMULATE_CMD,    TO_SERVER, 	    AT_CMD_SEND,            API_TX_ACCUMULATE_RESPONSE, USED,       NULL        },
        
    {   PARA_INFO_REQUEST,          TYPE_INFO_RQST_CMD,     TO_SERVER, 	    AT_CMD_SEND,            API_TX_INFO_REQUEST,        UNUSED,     NULL        },
    {   PARA_INFO_REQUEST_USER,     TYPE_INFO_RQST_USER_CMD,TO_SERVER,      AT_CMD_SEND,            API_TX_INFO_REQUEST,        UNUSED,     NULL        },
    {   PARA_INFO_REQUEST_DATA,     TYPE_INFO_RQST_DATA_CMD,TO_SERVER,      AT_CMD_SEND,            API_TX_INFO_REQUEST,        UNUSED,     NULL        },
    {   PARA_MONITOR_RESPONSE_W,    TYPE_MONITOR_DATA_CMD,  TO_SERVER,      AT_CMD_BLE_SEND,        API_TX_MONITOR_RESPONSE_W,  USED,       NULL        },
    {   PARA_MONITOR_RESPONSE_R,    TYPE_MONITOR_DATA_CMD,  TO_SERVER,      AT_CMD_BLE_SEND,        API_TX_MONITOR_RESPONSE_R,  USED,       NULL        },
    {   PARA_BLE_CERT_TX,           TYPE_BLE_CERT_CMD,      TO_SERVER,      AT_CMD_BLE_SEND,        API_TX_BLE_CERT,            USED,       NULL        }
};
#define SZ_BLE_TX_CMD_LIST  ( sizeof(Wifi_BLE_TxCmdList) / sizeof(WifiTxCommandList_T) )

/***** Global Function ********************************************************/
void* GetWifiTxTypeEventList ( U8 mType )
{
    if ( mType == TX_ATCMD )
    {
        return WifiSendAtList;
    }
    else /* ( mType == TX_DATA ) */
    {
        return WifiSendDataList;
    }
}

U16 GetWifiTxTypeListSize ( U8 mType )
{
    if ( mType == TX_ATCMD )
    {
        return SZ_ATCMD_LIST;
    }
    else /* ( mType == TX_DATA ) */
    {
        return SZ_DATA_LIST;
    }
}

void* GetWifiTxCmdEventList ( U8 mType )
{
    if (( mType == BLEDATA ) )
    {
        return Wifi_BLE_TxCmdList;
    }
    else 
    {
        return WifiTxCmdList;
    }
}

U16 GetWifiTxCmdListSize ( U8 mType )
{
    if (( mType == BLEDATA ) )
    {
        return SZ_BLE_TX_CMD_LIST;
    }
    else 
    {
        return SZ_TX_CMD_LIST;
    }
}


/***** Local Function *********************************************************/
static void ExceptAtCmd ( U8 mExcept, I8* pBuf, U16 mLen )
{
    static I8 mai8ModelType[LENGTH_MODEL_TYPE+1];
    static I8 mai8ModelName[LENGTH_MODEL_NAME+1];
    // static I8 mai8ModelNameSSID[LENGTH_MODEL_NAME+1];

    _MEMSET_( (void __FAR*) mai8ModelType, '\0', sizeof(mai8ModelType) );
    _MEMSET_( (void __FAR*) mai8ModelName, '\0', sizeof(mai8ModelName) );
    // _MEMSET_( (void __FAR*) mai8ModelNameSSID, '\0', sizeof(mai8ModelNameSSID) );
    
    /* Model Check */
    if ( GetUserInterface(USER_MODEL_TYPE) == USER_MODEL_CHP )
    {
        far_sprintf_s( mai8ModelType, 0, FOTA_TYPE_CHP );
        far_sprintf_s( mai8ModelName, 0, MODEL_NAME_CHP );
        // far_sprintf_s( mai8ModelNameSSID, 0, MODEL_NAME_CHP_BLESSID );
    }
    else if ( GetUserInterface(USER_MODEL_TYPE) == USER_MODEL_CP )
    {
        far_sprintf_s( mai8ModelType, 0, FOTA_TYPE_CP );
        far_sprintf_s( mai8ModelName, 0, MODEL_NAME_CP );
        // far_sprintf_s( mai8ModelNameSSID, 0, MODEL_NAME_CP_BLESSID );
    }
    else if ( GetUserInterface(USER_MODEL_TYPE) == USER_MODEL_HP )
    {
        far_sprintf_s( mai8ModelType, 0, FOTA_TYPE_HP );
        far_sprintf_s( mai8ModelName, 0, MODEL_NAME_HP );
    }
    else
    {
        far_sprintf_s( mai8ModelType, 0, FOTA_TYPE_CHP );
        far_sprintf_s( mai8ModelName, 0, MODEL_NAME_CHP );
        // far_sprintf_s( mai8ModelNameSSID, 0, MODEL_NAME_CHP_BLESSID );
    }

    /* AT Cmd 예외조건 처리 */
    if ( mExcept == TO_VERSION )
    {
        far_sprintf_s( &pBuf[mLen], 0, WIFI_VERSION );
    }
    else if ( mExcept == TO_APMODE )
    {
        mLen += far_sprintf_s( &pBuf[mLen], 0, COWAY_SSID );
        far_sprintf_s( &pBuf[mLen], 0, mai8ModelName );
    }
    else if ( mExcept == TO_SCONN )
    {
        mLen += far_sprintf_s( &pBuf[mLen], 0, TEST_MODEM_SSID );
        far_sprintf_s( &pBuf[mLen], 0, TEST_MODEM_PW );
    }
    else if ( mExcept == TO_BLEADV )
    {
        mLen += far_sprintf_s( &pBuf[mLen], 0, mai8ModelType );
        pBuf[mLen++] = ASCII_SPACE;
        mLen += far_sprintf_s( &pBuf[mLen], 0, mai8ModelName );
        pBuf[mLen++] = ASCII_UNDERBAR;
    }
}


