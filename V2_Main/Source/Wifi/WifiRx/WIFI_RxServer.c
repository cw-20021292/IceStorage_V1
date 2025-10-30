
/*******************************************************************************
&Description :
    Analysis Server API
  
&Histiry : ShinHM
    2021.04.22 update
    2021.12.10 update

&Histiry : SSG
    2023.05.24 update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_SetCommon.h"
#include "WIFI_DefUser.h"
#include "WIFI_RxServer.h"
#include "WIFI_TxProtocol.h"
#include "WIFI_Initialize.h"
#include "WIFI_Status.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_SetInformationData.h"
#include "WIFI_SetMonitorData.h"
#include "WIFI_UserInterface.h"
#include "WIFI_MonitorFixRAM.h"

/***** Define *****************************************************************/
/* API No (Server -> MCU) */
const signed char API_RX_A1002[] = "A1002"; // Disconnect
const signed char API_RX_A1011[] = "A1011"; // Function Control
const signed char API_RX_A1013[] = "A1013"; // Function Request
const signed char API_RX_A1023[] = "A1023"; // Sensor Request
const signed char API_RX_A1033[] = "A1033"; // Error Request
const signed char API_RX_A1035[] = "A1035"; // Error Selfcheck Request
const signed char API_RX_A1051[] = "A1051"; // Parameter Control
const signed char API_RX_A1053[] = "A1053"; // Parameter Request
const signed char API_RX_A1083[] = "A1083"; // SmartDetect Period Request
const signed char API_RX_A2011[] = "A2011"; // Information Respond
const signed char API_RX_A7011[] = "A7011"; // BLE Cert RX (Request)
const signed char API_RX_A9011[] = "A9011"; // Monitor CMD (Reqeust Write)
const signed char API_RX_A9013[] = "A9013"; // Monitor CMD (Reqeust Read)

/* Wifi Recv Cmd */
#define AT_CMD_RECV "*ICT*AWS_RECV:"
#define AT_CMD_BLE_RECV "*ICT*BLE_RECV:"
/* Length & Position */
#define LENGTH_HEADER 5 // STX + LENGTH
#define POSITION_STX 0 // 0 (1Byte)
#define POSITION_LENGTH 1 // 1~4 (4Byte)
#define POSITION_API 5 // 5~9 (5Byte)
#define POSITION_RQID 10 // 10~25 (16Byte)
#define POSITION_OPEN 26 // 26 (1Byte) '{'
#define POSITION_DATA 27 // 27~ (nByte)


/***** Variable ***************************************************************/
I8 ai8RxProtocol[LENGTH_WIFI_ALL_MAX_N]; // RX DATA (STX ~ ETX)
// I8 ai8RxProtocol[LENGTH_WIFI_DATA_MAX]; // RX DATA (STX ~ ETX)
U16 au16InfoData[WIFI_INFO_NUM];
U16 gu16_RAM_Monitor_RESP_SIZE;         // RAM DATA �б�/���� ��û�� ���� (RAM address ����)
U16 gu16_START_RAM_Address[LENGTH_RAM_ADDRESS_MAX];     // RAM ADDRESS �� (�ִ밹�� ����)
U16 gu16_RAM_Address_len[LENGTH_RAM_ADDRESS_MAX];
/***** Constant ***************************************************************/


/***** Static Function ********************************************************/
static void RunAnalysis ( void );
static U8 IsRxDataOk ( void );

static U8 RxA1011 ( void );
static U8 RxA2011 ( void );
static U8 RxA7011 ( void );
static U8 RxA9011 ( void );
static U8 RxA9013 ( void );

static void DoFuncData ( I8* pBuf );
static void DoInfoData ( I8* pBuf );
static void DoMonitorData (U8 rw_sel, I8* pBuf );


static void MakeCheckId ( I8* pBuf, U16 mLen, U16 mId );


/***** Table ******************************************************************/
typedef U8 (*WifiRxApiFun_T)( void );
typedef struct _wifi_rxapi_list_
{
    I8* Api;
    U32 Tx;
    WifiRxApiFun_T Func;
} WifiRxApiList_T;

/* Wifi Rx Table */
static const WifiRxApiList_T WifiRxApiList[] =
{   /*  API             Tx                      Function */
    {   API_RX_A1002,   WIFI_TX_NULL,               NULL    },
    {   API_RX_A1011,   WIFI_TX_RESP_RECV,          RxA1011 },
    {   API_RX_A1013,   WIFI_TX_RESP_FUNC,          NULL    },
    {   API_RX_A1023,   WIFI_TX_RESP_SENSOR,        NULL    },
    {   API_RX_A1033,   WIFI_TX_RESP_ERR,           NULL    },
    {   API_RX_A1035,   WIFI_TX_SELF_ERR,           NULL    },
    {   API_RX_A1051,   WIFI_TX_SEND_PARA,          NULL    },
    {   API_RX_A1053,   WIFI_TX_RESP_PARA,          NULL    },
    {   API_RX_A1083,   WIFI_TX_RESP_PART,          NULL    },
    {   API_RX_A2011,   WIFI_TX_NULL,               RxA2011 },
    #ifdef USER_EasyPairing20
    {   API_RX_A7011,   WIFI_TX_NULL,               RxA7011 },
    #endif
    {   API_RX_A9011,   WIFI_TX_RESP_MNT_DATA_W,    RxA9011 },
    {   API_RX_A9013,   WIFI_TX_RESP_MNT_DATA_R,    RxA9013 }
};
#define SZ_RX_API_LIST  ( sizeof(WifiRxApiList) / sizeof(WifiRxApiList_T) )


/***** Global Function ********************************************************/
void InitializeWifiRxServer ( void )
{
    _MEMSET_( (void __FAR*) ai8RxProtocol, '\0', sizeof(ai8RxProtocol) );
    _MEMSET_( (void __FAR*) au16InfoData, 0, sizeof(au16InfoData) );
}

void WifiInsertRxAPI ( I8* mai8Recv )
{
    U16 mu16Len = 0U;
    U16 mu16Position = 0U;
    U16 i = 0U;

    _MEMSET_( (void __FAR*) ai8RxProtocol, '\0', sizeof(ai8RxProtocol) );

    /* Check Max Length */
    mu16Len = _STRLEN_( (const char __FAR*) mai8Recv );
    mu16Position = _STRLEN_( (const char __FAR*) AT_CMD_RECV );
    if( mu16Len > LENGTH_WIFI_ALL_MAX_N )
    // if( mu16Len > LENGTH_WIFI_ALL_MAX )
    {
        return;
    }
  	
    /* Insert Data */
    for ( i = 0 ; i < mu16Len ; i++ )
    {   /* STX ~ ETX */
        ai8RxProtocol[i] = mai8Recv[mu16Position + i];
        if ( ai8RxProtocol[i] == ASCII_ETX )
        {
            break;
        }
    }
    
    if ( GetUserInterface( USER_SMART_SENSING_STATUS ) != TRUE ) 
    {
        SetWifiBLESendStatus(TX_BLE_WIFI, WIFIDATA);
    }
    /* Analysis */
    RunAnalysis();
}
/*Rx BLE DATA */
void WifiInsertRxBLEAPI ( I8* mai8Recv )
{
    U16 mu16Len = 0U;
    U16 mu16Position = 0U;
    U16 i = 0U;

    _MEMSET_( (void __FAR*) ai8RxProtocol, '\0', sizeof(ai8RxProtocol) );

    /* Check Max Length */
    mu16Len = _STRLEN_( (const char __FAR*) mai8Recv );
    mu16Position = _STRLEN_( (const char __FAR*) AT_CMD_BLE_RECV );
    if( mu16Len > LENGTH_WIFI_ALL_MAX_N )
    // if( mu16Len > LENGTH_WIFI_ALL_MAX )
    {
        return;
    }
  	
    /* Insert Data */
    for ( i = 0 ; i < mu16Len ; i++ )
    {   /* STX ~ ETX */
        ai8RxProtocol[i] = mai8Recv[mu16Position + i];
        if ( ai8RxProtocol[i] == ASCII_ETX )
        {
            break;
        }
    }

    /* Analysis */
    SetWifiBLESendStatus(TX_BLE_WIFI, BLEDATA);
    SetWifiBLESendStatus(BLE_RECV_CHECK, SET);
    RunAnalysis();
}

U16 GetWifiInfoData ( U16 mId )
{
    return au16InfoData[mId];
}

void SetWifiInfoData ( U16 mId, U16 mData )
{
    au16InfoData[mId] = mData;
    if ( mId == WIFI_INFO_0006_DAY )
    {
        SetUserTimeSetting( &au16InfoData[0] );
    }
    if ( ( mId == WIFI_INFO_0007_PART_PERIOD ) && ( GetWifiIniStatus( STATUS_INI_INFO ) != SET ) )
    {
        SetWifiIniStatus( STATUS_INI_INFO, SET );
    }
}


/***** Local Function *********************************************************/
// <-9v 5%@LEM :P<.
static void RunAnalysis ( void )
{
    WifiRxApiList_T *pList = NULL;
    U8 i = 0U;
    static I8 mai8RxAPINo[LENGTH_API_NO+1];
    static I8 mai8RqId[LENGTH_REQUEST_ID+1];
    
    _MEMSET_( (void __FAR*) mai8RxAPINo, '\0', sizeof(mai8RxAPINo));
    _MEMSET_( (void __FAR*) mai8RqId, '\0', sizeof(mai8RqId));
    
    /* Validity Check */
    if( IsRxDataOk() != TRUE )
    {
        return;
    }
    
    /* API & RequestID */
    for ( i = 0; i < LENGTH_API_NO ; i++ )
    {
        mai8RxAPINo[i] = ai8RxProtocol[POSITION_API + i];
    }
    for ( i = 0; i < LENGTH_REQUEST_ID ; i++ )
    {
        mai8RqId[i] = ai8RxProtocol[POSITION_RQID + i];
    }
    SetRequestId( mai8RqId );
    
    /* Run */
    pList = WifiRxApiList;
#ifdef UNUSED_BLE_A1011_RX
    if (GetWifiBLESendStatus(TX_BLE_WIFI) == BLEDATA ) // BLE DATA ?? ? ??
    {

        if ( _STRNCMP_( (const char __FAR*) (pList+1)->Api, (const char __FAR*) mai8RxAPINo, LENGTH_API_NO ) == N_TRUE ) // BLE : A1011 
        {  
            return;
        
        }
    }
#endif

#ifdef UNUSED_A9011_RX
    if ( _STRNCMP_( (const char __FAR*) (pList+11)->Api, (const char __FAR*) mai8RxAPINo, LENGTH_API_NO ) == N_TRUE ) // BLE : A9011
    {  
        return;
    }
#endif

    if (GetWifiBLESendStatus(TX_BLE_WIFI) == BLEDATA ) // BLE DATA ���� �� ���?
    {
        if ( _STRNCMP_( (const char __FAR*) (pList+10)->Api, (const char __FAR*) mai8RxAPINo, LENGTH_API_NO ) == N_TRUE ) // BLE : A7011 �� ���?
        {  
            SetWifiBLESendStatus(TX_BLE_WIFI, WIFIDATA); 
        }
    }
    
    for ( i = 0; i < SZ_RX_API_LIST; i++ )
    {
        if ( _STRNCMP_( (const char __FAR*) (pList+i)->Api, (const char __FAR*) mai8RxAPINo, LENGTH_API_NO ) == N_TRUE )
        {  
            WifiRxApiFun_T pFun;
            U8 mAck = TRUE;

            pFun = (pList+i)->Func;
            
            if ( pFun != NULL )
            {
                mAck = pFun();
            }
            if ( ( mAck == TRUE ) && ( (pList+i)->Tx != WIFI_TX_NULL ) )
            {
                SetWifiSendStatus( TX_DATA, (pList+i)->Tx );
            }
            break;
        }
    }
}

// @/H?<: 0K;g
static U8 IsRxDataOk ( void )
{
    U16 mu16i = 0U;
    U16 mu16DataLength = 0U;
    U32 mu32CheckSumBuf = 0UL;
    U8 mu8CheckSum = 0U;
    U8 mu8RxCheckSum = 0U;

    /* Check STX */
    if ( ai8RxProtocol[POSITION_STX] != ASCII_STX )
    {
        return FALSE;
    }
    
    /* Check ETX */
    if ( ai8RxProtocol[ _STRLEN_( (const char __FAR*) ai8RxProtocol ) - 1 ] != ASCII_ETX )
    {
        return FALSE;
    }
    
    /* Check Length */
    mu16DataLength = (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_LENGTH]) << 12) & 0xF000;
    mu16DataLength += (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_LENGTH+1]) << 8) & 0x0F00;
    mu16DataLength += (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_LENGTH+2]) << 4) & 0x00F0;
    mu16DataLength += WIFI_ASCII2HEX(ai8RxProtocol[POSITION_LENGTH+3]) & 0x000F;
    
    if ( ( mu16DataLength + LENGTH_HEADER ) != _STRLEN_( (const char __FAR*) ai8RxProtocol ) )
    {
        return FALSE;
    }
    
    /* Check CHECKSUM */
    for ( mu16i = 0 ; mu16i < ( _STRLEN_( (const char __FAR*) ai8RxProtocol ) - 3 ) ; mu16i++ )
    { 
        if ( mu16i > LENGTH_WIFI_ALL_MAX_N )
        // if ( mu16i > LENGTH_WIFI_ALL_MAX )
        {
            break;
        }
        else
        {
            mu32CheckSumBuf += ai8RxProtocol[mu16i];
        }
    }
    mu8CheckSum = (U8)mu32CheckSumBuf;
    mu8RxCheckSum = (U8)((WIFI_ASCII2HEX(ai8RxProtocol[_STRLEN_( (const char __FAR*) ai8RxProtocol)-3]) << 4) & 0xF0);
    mu8RxCheckSum += (U8)(WIFI_ASCII2HEX(ai8RxProtocol[_STRLEN_( (const char __FAR*) ai8RxProtocol)-2]) & 0x0F);
    if ( mu8RxCheckSum != mu8CheckSum )
    {
        return FALSE;
    }
    return TRUE;
}

static U8 IsRecvPossible ( void )
{
    U16 mu16ExceptId = 0U;

    mu16ExceptId = (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_DATA]) << 12) & 0xF000;
    mu16ExceptId += (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_DATA+1]) << 8) & 0x0F00;
    mu16ExceptId += (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_DATA+2]) << 4) & 0x00F0;
    mu16ExceptId += WIFI_ASCII2HEX(ai8RxProtocol[POSITION_DATA+3]) & 0x000F;

    if ( GetUserInterface( USER_SMART_SENSING_STATUS ) == TRUE )
    { 
        if ( IsExceptId( mu16ExceptId ) == TRUE )
        { 
            return TRUE;
        }
    }
    
    if ( GetUserInterface( USER_RX_STATUS ) != TRUE ) 
    {
        if ( IsExceptId( mu16ExceptId ) == TRUE ) 
        {
            if ( GetUserInterface( USER_SMART_POSSIBLE_STATUS ) == TRUE ) 
            {
                return TRUE;
            }
            else
            {
                return FALSE; // �������� ���� ���? �������� ���� ���� �̿� �ٸ� �Է� ����
            }
        }
    }
    else 
    {
        if ( IsExceptId( mu16ExceptId ) == TRUE )
        {
            if ( GetUserInterface( USER_SMART_POSSIBLE_STATUS ) != TRUE ) 
            {
                return FALSE;
            }
        }
        return TRUE;
    }
    return FALSE;
}



static U8 RxA1011 ( void )
{
    U16 i = 0U;
    U8 mData = 0U;
    // static I8 pBuf[LENGTH_WIFI_DATA_ID];
    static I8 pBuf[LENGTH_WIFI_DATA_RX_MAX];

    mData = GetWifiBLESendStatus(TX_BLE_WIFI);

    _MEMSET_( (void __FAR*) pBuf, '\0', sizeof(pBuf) );    

    /* Validity Check */
    if ( IsRecvPossible() != TRUE )
    {
        SetWifiBuzzSound( WIFI_BUZZER_ERROR );
        if (mData != BLEDATA) // BLE DATA TEST��
        {
            return TRUE;
        }
    }

    /* Run */
    // for ( i = 0 ; i < LENGTH_WIFI_DATA_ID ; i++ )
    for ( i = 0 ; i < LENGTH_WIFI_DATA_RX_MAX ; i++ )
    {
        pBuf[i] = ai8RxProtocol[POSITION_DATA + i];
        if ( pBuf[i] == ASCII_CLOSE )
        {
            break;
        }
    }
    DoFuncData( pBuf );
#ifdef USER_USE_ACK
    return TRUE;
#endif
    return FALSE;
}

static U8 RxA2011 ( void )
{
    U16 i = 0U;
    static I8 pBuf[LENGTH_WIFI_INFO_MAX];
    
    _MEMSET_( (void __FAR*) pBuf, '\0', sizeof(pBuf) );

    WifiCheckInitializeInfoAck();
    
	/* Run */
    for ( i = 0 ; i < LENGTH_WIFI_INFO_MAX ; i++ )
    {
        pBuf[i] = ai8RxProtocol[POSITION_DATA + i]; // { ~ }
        if ( pBuf[i] == ASCII_CLOSE )
        {
            break;
        }
    }
    DoInfoData( pBuf );
    return FALSE;
}

// BEL Cert RX
static U8 RxA7011 ( void )
{

    U16 i = 0U;
    I8 mi8Id[LENGTH_WIFI_DATA_ONE];
    U8 mu8i = 0U;
    I8 mi8Data[LENGTH_WIFI_DATA_ID];
    static I8 pBuf[LENGTH_WIFI_DATA_ID];
    _MEMSET_( (void __FAR*) pBuf, '\0', sizeof(pBuf) );    
    /* Run */
    for ( i = 0 ; i < LENGTH_WIFI_DATA_ID ; i++ )
    {
        pBuf[i] = ai8RxProtocol[POSITION_DATA + i];
        if ( pBuf[i] == ASCII_CLOSE )
        {
            break;
        }
    }

    /* Control */
    
    _MEMSET_( (void __FAR*) mi8Id, '\0', sizeof(mi8Id) );

    MakeCheckId( mi8Id, 0, WIFI_BLE_CERT_0001); // A7011.0001 �� ���?

    if ( _STRNCMP_( (const char __FAR*) mi8Id, (const char __FAR*) pBuf, LENGTH_WIFI_ID_ONE ) == N_TRUE ) // ID+:(5Byte) check
    {
        for ( mu8i = 0 ; mu8i < LENGTH_WIFI_DATA_ONE ; mu8i++ )
        {
            if ( ( pBuf[LENGTH_WIFI_ID_ONE+mu8i] == ASCII_COMMA ) || ( pBuf[LENGTH_WIFI_ID_ONE+mu8i] == ASCII_CLOSE ) )
            {
                break;
            }
            else
            {
                mi8Data[mu8i] = pBuf[LENGTH_WIFI_ID_ONE+mu8i];
            }
        }

        if((U8)(mi8Data[0]) == ASCII_NO_1) // Value �� '1'�� ���?
        {
            SetWifiApStatus(STATUS_AP_BLE_CERT,SET);    // �������� ����(���� ���?) ���·� ����
            SetWifiCertID(WIFI_BLE_CERT_0002); // A7010.0002 :
            SetWifiCertStatus(STATUS_CERT_ON); // 0001         
            SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_BLE_CERT); // �������� ���� ���� (A7010)
            SetWifiBuzzSound( WIFI_BUZZER_BLE_CERT ); // �������� �ȳ� ����
        }
        else // ASCII_NO_0 OR ASCII_NO_2 ?
        {
            // SetWifiApStatus(STATUS_AP_BLE_CERT,CLEAR);      // �������� ���� �Ұ� ���·� ����
            SetWifiCertID(WIFI_BLE_CERT_0002); // A7010.0002 :
            SetWifiCertStatus(STATUS_CERT_FAIL); // 0002          
            SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_BLE_CERT); // �������� ���� ���� (A7010)
        }
    }


    return FALSE;
}

// monitor test
static U8 RxA9011 ( void )
{
    U16 i = 0U;
        static I8 pBuf[LENGTH_WIFI_RAM_WRITE]; // Write ?? ?? Size
        
        _MEMSET_( (void __FAR*) pBuf, '\0', sizeof(pBuf) );

        /* Run */
        for ( i = 0 ; i < LENGTH_WIFI_RAM_WRITE ; i++ )
        {
            pBuf[i] = ai8RxProtocol[POSITION_DATA + i]; // { ~ }
            if ( pBuf[i] == ASCII_CLOSE )
            {
                break;
            }
        }
        DoMonitorData(WiFi_M_RAM_Write, pBuf );
        GetRamData();
    #ifdef USER_USE_ACK
        return TRUE;
    #endif
        return FALSE;
}


// Monitor Ram Read
static U8 RxA9013 ( void )
{
    U16 i = 0U;
    static I8 pBuf[LENGTH_RAM_DATA_MAX]; // Read ?? ?? Size
    _MEMSET_( (void __FAR*) pBuf, '\0', sizeof(pBuf) );
    
	/* Run */
    for ( i = 0 ; i < LENGTH_RAM_DATA_MAX ; i++ )
    {
        pBuf[i] = ai8RxProtocol[POSITION_DATA + i]; // { ~ }
        if ( pBuf[i] == ASCII_CLOSE )
        {
            break;
        }
    }
    GetRamData();
    DoMonitorData(WiFi_M_RAM_Read, pBuf );
#ifdef USER_USE_ACK
    return TRUE;
#endif
    return FALSE;
}

static void DoFuncData ( I8* pBuf )
{

    WifiTxFuncList_T *pList = NULL;
    U16 i = 0U;
    U16 mu16Size = 0U;
    U16 mu16FuncCount = 0U;
    // I8 mi8Data[LENGTH_WIFI_DATA_ID];
    U8 mi = 0U;

    #define HOTTEMP_COUNT       12  // �� �¼������µ� DATA ���� 
    #define USE_MAX_HOTTEMP    5   // �ִ� ���� ������ �¼������µ� ����
    #define USE_MIN_HOTTEMP    4   // �ּ� ���� ������ �¼������µ� ����
    I8 mi8HOTTEMP_SEL_LIST_DATA[HOTTEMP_COUNT] = {0};
    U8 mi8HOTTEMP_SEL_LIST_IDNo[HOTTEMP_COUNT] = {0};
    U8 mu8HotTemp_COUNT = 0U;
    U8 mu8HotTemp_Finish = 0U;
    U8 mQExceptionFinish_HOTTEMP = 0U;

    #define SLEEP_COUNT         4  // �� �뷮 DATA ���� (Start H/M, END H/M)
    I8 mi8Sleep_LIST_DATA[SLEEP_COUNT*2] = {0}; // MAX Buf
    U8 mi8Sleep_LIST_IDNo[SLEEP_COUNT] = {0};
    U8 mi8Sleep_LIST_IDSize[SLEEP_COUNT] = {0};
    U8 mu8Sleep_SizeCheck = 0U;
    U8 mu8Sleep_COUNT = 0U;
    U8 mu8Sleep_Size_COUNT = 0U;
    U8 mu8Sleep_Finish = 0U;

    I8 mi8QUANTITY_SEL_Def = 0U;
    U8 mu8QUANTITY_SEL_LIST_Def_IDNo = 0U;

    #define QUANTITY_COUNT 10 // 10�� �뷮
    #define USE_MAX_QUANTITY    5   // �ִ� ���� ������ �뷮 ����
    #define USE_MIN_QUANTITY    2   // �ּ� ���� ������ �뷮 ���� (��������)
    I8 mi8QUANTITY_SEL_LIST_DATA[QUANTITY_COUNT] = {0};
    U8 mi8QUANTITY_SEL_LIST_IDNo[QUANTITY_COUNT] = {0};
    U8 mu8QUANTITY_COUNT = 0U;
    U8 mQi = 0U;
    U8 mQFinish = 0U;
    U8 mQFinish_Defualt = 0U;
    U8 mQExceptionFinish_QUANTITY = 0U;
    U8 mQExceptionFinish_Defualt = 0U;
    U8 mu8FuncListCheck = 0U;
    // _MEMSET_( (void __FAR*) mi8Data, '\0', sizeof(mi8Data) );

    /* Table Matching */
    pList = (WifiTxFuncList_T *)GetWifiFuncEventList(); // EW@L:m 8.=:F.
    mu16Size = GetWifiFuncListSize(); // EW@L:m ;g@LAn
    
    /* Control */
    for ( i = 0 ; i < mu16Size; i++ )
    {
        WifiRxFun_T pFun;
        I8 mi8Id[LENGTH_WIFI_DATA_ONE];
        I8 mId[LENGTH_WIFI_ID_ONE+1]; // Ref
        U8 mu8i = 0U;
        U8 mu8Func = 0U;
        U8 mu8FuncFinish = 0U;
        I8 mi8Func[LENGTH_WIFI_DATA_ONE];

        _MEMSET_( (void __FAR*) mId, '\0', sizeof(mId) );
        _MEMSET_( (void __FAR*) mi8Id, '\0', sizeof(mi8Id) );
        _MEMSET_( (void __FAR*) mi8Func, '\0', sizeof(mi8Func) );

        for ( mu8Func = 0 ; mu8Func < LENGTH_WIFI_ID_ONE ; mu8Func++ )
        {
            mi8Id[mu8Func] = pBuf[mu16FuncCount]; // { ~ }
            mu16FuncCount++;
            if ( mi8Id[mu8Func] == ASCII_COLON )
            {
                break;
            }
        }

        for ( mu8Func = 0 ; mu8Func < LENGTH_WIFI_DATA_ONE ; mu8Func++ )
        {
            mi8Func[mu8Func] = pBuf[mu16FuncCount]; // { ~ }
            mu16FuncCount++;
            if ( mi8Func[mu8Func] == ASCII_COMMA)
            {
                mu8FuncListCheck = SET;
            }
            if (mi8Func[mu8Func] == ASCII_CLOSE)
            {
                mu8FuncFinish = SET;
            }
            if ( mi8Func[mu8Func] == ASCII_COMMA || mi8Func[mu8Func] == ASCII_CLOSE )
            {
                mi8Func[mu8Func] = ASCII_NULL;
                break;
            }
        }
        for ( mi = 0 ; mi < mu16Size; mi++ )
        {
            pFun = (pList+mi)->RxFunc;
            MakeCheckId( mId, 0, (pList+mi)->Id );        
        
            if ( _STRNCMP_( (const char __FAR*) mId, (const char __FAR*) mi8Id, LENGTH_WIFI_ID_ONE ) == N_TRUE ) // ID+:(5Byte) check
            {
                if ( pFun != NULL )
                {
                    /* ------------------------------ �¼������µ� ��Ƽ���� ó�� Start --------------------------------- */
                    if ((mu8FuncListCheck != SET) && (IsExceptHotTempUseID((pList+mi)->Id) == TRUE))
                    {
                        break;
                    }

                    else if ((IsExceptHotTempUseID((pList+mi)->Id) == TRUE))
                    {
                        mi8HOTTEMP_SEL_LIST_DATA[mu8HotTemp_COUNT] = mi8Func[0]; // ASCII
                        mi8HOTTEMP_SEL_LIST_IDNo[mu8HotTemp_COUNT] = mi;
                        mu8HotTemp_COUNT++;
                        if ((mu8FuncFinish == SET) || (mu8HotTemp_COUNT == HOTTEMP_COUNT)) //Check
                        {
                            mu8HotTemp_Finish = SET;
                        }
                    }

                    // �¼������µ� ����ó�� (4~5�� ������ ���? Ȯ��)
                    if (mu8HotTemp_Finish == SET ) 
                    {
                        U8 mu8CountCheck = 0U;
                        for (mQi = 0; mQi < mu8HotTemp_COUNT; mQi++)
                        {
                            if ((mi8HOTTEMP_SEL_LIST_DATA[mQi]-0x30) != 0U)
                            {
                                mu8CountCheck += 1;
                            }
                        }
                        if((mu8CountCheck >= USE_MIN_HOTTEMP ) && (mu8CountCheck <= USE_MAX_HOTTEMP) && (mu8HotTemp_COUNT == HOTTEMP_COUNT) )  // 4�� ~ 5�� 
                        {
                            mQExceptionFinish_HOTTEMP = SET;
                        }
                    }

                    if ((mQExceptionFinish_HOTTEMP == SET)) // �뷮 ���� Check
                    {
                        for (mQi = 0; mQi < mu8HotTemp_COUNT; mQi++)
                        {
                            _MEMSET_( (void __FAR*) mi8Func, '\0', sizeof(mi8Func) );
                            mi8Func[0] = mi8HOTTEMP_SEL_LIST_DATA[mQi];
                            mi8Func[1] = ASCII_NULL;
                            pFun( (pList+mi8HOTTEMP_SEL_LIST_IDNo[mQi])->Id, mi8Func );
                        }
                    }

                    /* ------------------------------  �¼������µ� ��Ƽ���� ó�� end --------------------------------- */


                    /* ------------------------------ ��ħ�ð� ��Ƽ���� ó�� Start --------------------------------- */
                    if ((mu8FuncListCheck != SET) && (IsExceptSleepTimeID((pList+mi)->Id) == TRUE))
                    {
                        break;
                    }

                    else if ((IsExceptSleepTimeID((pList+mi)->Id) == TRUE))
                    {
                        mi8Sleep_LIST_DATA[mu8Sleep_Size_COUNT] = mi8Func[0]; // ASCII
                        mu8Sleep_Size_COUNT++;
                        mi8Sleep_LIST_IDSize[mu8Sleep_COUNT]++;
                        if ((mi8Func[1] >= 0x30) && (mi8Func[1] <= 0x39)) // 2��° �ڸ����� ���� 0 ~ 9 �� ���?
                        {
                            mi8Sleep_LIST_DATA[mu8Sleep_Size_COUNT] = mi8Func[1]; // ASCII
                            mu8Sleep_Size_COUNT++;
                            mi8Sleep_LIST_IDSize[mu8Sleep_COUNT]++;
                        }
                        mi8Sleep_LIST_IDNo[mu8Sleep_COUNT] = mi;
                        mu8Sleep_COUNT++;
                        if ((mu8FuncFinish == SET) || (mu8Sleep_COUNT == SLEEP_COUNT)) // �뷮 ���� Check
                        {
                            mu8Sleep_Finish = SET;
                        }
                    }
                    // ��ħ�ð����� ���� �����? PASS

                    if ((mu8FuncFinish == SET) && (mu8Sleep_Finish == SET)) // (����ó�� üũ �Ϸ�)
                    {
                        // �����ð� ����
                        for (mQi = 0; mQi < mu8Sleep_COUNT; mQi++)
                        {
                            _MEMSET_( (void __FAR*) mi8Func, '\0', sizeof(mi8Func) );
                            if (mi8Sleep_LIST_IDSize[mQi] == 1U)
                            {
                                mi8Func[0] = mi8Sleep_LIST_DATA[mu8Sleep_SizeCheck];
                                mu8Sleep_SizeCheck++;
                                mi8Func[1] = ASCII_NULL;
                            }
                            else if (mi8Sleep_LIST_IDSize[mQi] == 2U)
                            {
                                mi8Func[0] = mi8Sleep_LIST_DATA[mu8Sleep_SizeCheck];
                                mu8Sleep_SizeCheck++;
                                mi8Func[1] = mi8Sleep_LIST_DATA[mu8Sleep_SizeCheck];
                                mu8Sleep_SizeCheck++;
                                mi8Func[2] = ASCII_NULL;
                            }
                            pFun( (pList+mi8Sleep_LIST_IDNo[mQi])->Id, mi8Func );
                        }
                    }
                    /* ------------------------------ ��ħ�ð� ��Ƽ���� ó�� End --------------------------------- */

                    // ------------------------------ �뷮 ��Ƽ����(����)ó�� Start --------------------------------

                    if ((mu8FuncListCheck != SET) && ((IsExceptWaterAmountID_List((pList+mi)->Id) == TRUE)||(IsExceptWaterAmountID((pList+mi)->Id) == TRUE)))
                    {
                        break;  // �뷮 ���� ���� �� PASS 
                    }
                    // LIST�� �������� �� �� �������� �ٽ� ó�� (����Ʈ �뷮 �ӽù��� ����) (0066)
                    // if((mu8FuncFinish != SET) && (IsExceptWaterAmountID((pList+mi)->Id) == TRUE) )
                    else if(IsExceptWaterAmountID((pList+mi)->Id) == TRUE)
                    {
                        mi8QUANTITY_SEL_Def = mi8Func[0]; // ASCII
                        mu8QUANTITY_SEL_LIST_Def_IDNo = mi;
                        mQFinish_Defualt = SET;
                    }
                    // TEST �뷮������ ���۷� ���� (0067~006F)
                    else if((IsExceptWaterAmountID_List((pList+mi)->Id) == TRUE) )
                    {
                        mi8QUANTITY_SEL_LIST_DATA[mu8QUANTITY_COUNT] = mi8Func[0]; // ASCII
                        mi8QUANTITY_SEL_LIST_IDNo[mu8QUANTITY_COUNT] = mi;
                        mu8QUANTITY_COUNT++;
                        if ((mu8FuncFinish == SET) || (mu8QUANTITY_COUNT == QUANTITY_COUNT)) //Check
                        {
                            mQFinish = SET;
                        }
                    }

                    else if((IsExceptHotTempUseID((pList+mi)->Id) != TRUE) && (IsExceptSleepTimeID ((pList+mi)->Id) != TRUE) )
                    {
                        pFun( (pList+mi)->Id, mi8Func );
                    }
                    // �뷮 ����ó�� ---------------------------------(�뷮�����Ϸ� & ����Ʈ�����Ϸ� & �����ͼ���ó�� �Ϸ� & �뷮 9�� ���� �Ϸ�)
                    if ((mQFinish == SET ) && (mQFinish_Defualt == SET ) && (mu8FuncFinish == SET) && (mu8QUANTITY_COUNT == QUANTITY_COUNT)) 
                    {
                        U8 mu8CountCheck = 0U;
                        for (mQi = 0; mQi < mu8QUANTITY_COUNT; mQi++)
                        {
                            if ((mi8QUANTITY_SEL_LIST_DATA[mQi]-0x30) != 0)
                            {
                                mu8CountCheck += 1;
                            }
                            if (((mi8QUANTITY_SEL_Def-0x30) == mQi) && ((mi8QUANTITY_SEL_LIST_DATA[mQi]-0x30) != 0)) // ��ȿ�� ����Ʈ �뷮 �����ΰ� ?
                            {
                                mQExceptionFinish_Defualt = SET;
                            }
                        }

                        if((mu8CountCheck >= USE_MIN_QUANTITY ) && (mu8CountCheck <= USE_MAX_QUANTITY) )  // 2�� ~ 5�� 
                        {
                            mQExceptionFinish_QUANTITY = SET;
                        }
                    }
                    
                    if ((mu8FuncFinish == SET) && (mu8QUANTITY_SEL_LIST_Def_IDNo != 0U) && (mQFinish == SET) && (mQExceptionFinish_Defualt == SET && (mQExceptionFinish_QUANTITY == SET))) // (����ó�� üũ �Ϸ�)
                    {

                        // �뷮 ����
                        for (mQi = 0; mQi < mu8QUANTITY_COUNT; mQi++)
                        {
                            _MEMSET_( (void __FAR*) mi8Func, '\0', sizeof(mi8Func) );
                            mi8Func[0] = mi8QUANTITY_SEL_LIST_DATA[mQi];
                            mi8Func[1] = ASCII_NULL;
                            pFun( (pList+mi8QUANTITY_SEL_LIST_IDNo[mQi])->Id, mi8Func );
                        }
                        // ����Ʈ �뷮 ����
                        _MEMSET_( (void __FAR*) mi8Func, '\0', sizeof(mi8Func) );
                        mi8Func[0] = mi8QUANTITY_SEL_Def;
                        mi8Func[1] = ASCII_NULL;
                        pFun( (pList+mu8QUANTITY_SEL_LIST_Def_IDNo)->Id, mi8Func );
                    }
                    // �뷮 ����ó�� --------------------------------


                    if ( ( IsExceptId( (pList+mi)->Id ) == TRUE ) && ( mi8Func[0] == ASCII_NO_1 ) ) // Smart Diagnosis
                    {
                        SetRequestIdExcept();
                    }
                    break;
                }
            }
        }
        if (mu8FuncFinish == SET)
        {
            break;
        }

    }
}


// 1b4I <3A$ :/0f @{?k
static void DoInfoData ( I8* pBuf )
{
    WifiTxFuncList_T *pList = NULL;
    U16 i = 0U;
    U16 mu16Size = 0U;
    U16 mu16InfoCount = 0U;
    U8 mi = 0U;
	  
    //_MEMSET_( (void __FAR*) mi8Data, '\0', sizeof(mi8Data) );

    pList = (WifiTxFuncList_T *)GetWifiInformationEventList(); // EW@L:m 8.=:F.
    mu16Size = GetWifiInformationListSize(); // EW@L:m ;g@LAn
    
    
    for ( i = 0 ; i < mu16Size; i++ )
    {
        U8 mu8Info = 0U;
        WifiRxFun_T pFun;
        I8 mi8Id[LENGTH_WIFI_ID_ONE+1];
        I8 mId[LENGTH_WIFI_ID_ONE+1]; // Ref
        I8 mi8Info[LENGTH_WIFI_DATA_ONE];
        // *ICT*AWS_RECV:0088A201122MCU2SVR0007-01{0000:2022,0001:7,0002:27,0003:9,0004:32,0005:21,0006:3,0007:1,0010:4,0011:2,0014:28,0015:32,0016:26,0026:65535}
        _MEMSET_( (void __FAR*) mId, '\0', sizeof(mId) );
        _MEMSET_( (void __FAR*) mi8Id, '\0', sizeof(mi8Id) );
        _MEMSET_( (void __FAR*) mi8Info, '\0', sizeof(mi8Info) );

        for ( mu8Info = 0 ; mu8Info < LENGTH_WIFI_ID_ONE ; mu8Info++ )
        {
            mi8Id[mu8Info] = pBuf[mu16InfoCount]; // { ~ }
            mu16InfoCount++;
            if ( mi8Id[mu8Info] == ASCII_COLON )
            {
                break;
            }
        }

        for ( mu8Info = 0 ; mu8Info < LENGTH_WIFI_DATA_ONE ; mu8Info++ )
        {
            mi8Info[mu8Info] = pBuf[mu16InfoCount]; // { ~ }
            mu16InfoCount++;
            if ( mi8Info[mu8Info] == ASCII_COMMA || mi8Info[mu8Info] == ASCII_CLOSE )
            {
                mi8Info[mu8Info] = ASCII_NULL;
                break;
            }
        }

        for ( mi = 0 ; mi < mu16Size; mi++ )
        {
            pFun = (pList+mi)->RxFunc;
            MakeCheckId( mId, 0, (pList+mi)->Id );        
        
            if ( _STRNCMP_( (const char __FAR*) mId, (const char __FAR*) mi8Id, LENGTH_WIFI_ID_ONE ) == N_TRUE ) // ID+:(5Byte) check
            {
                if ( pFun != NULL )
                {
                    pFun( (pList+mi)->Id, mi8Info );
                    break;
                }
            }
        }
    }
}

static void DoMonitorData (U8 rw_sel, I8* pBuf )
{
    WifiTxFuncList_Monitor_T *pList = NULL;
    U16 i = 0U;
    U16 mu16Size = 0U;
    U16 mu16DataCount = 0U;
    U8 mi = 0U;
    U8 mcount = 0U;

    pList = (WifiTxFuncList_Monitor_T *)GetWifiMonitorEventList(); 

    for ( i = 0 ; i < LENGTH_WIFI_ALL_MAX_N; i++ )
    // for ( i = 0 ; i < LENGTH_WIFI_ALL_MAX; i++ )
    {
        if (pBuf[i] == ASCII_COMMA) 
        {
            mcount++;
        }
        else if (pBuf[i] == ASCII_CLOSE)
        {
            mcount++;
            break;
        }
    }
    mu16Size = mcount; // ',' ������ count �Ͽ� ��û DATA�� ����� Ȯ��
    gu16_RAM_Monitor_RESP_SIZE = mu16Size; // ���������� ���� (TxMake ���� cmd size�� ���?)

    for ( i = 0 ; i < mu16Size; i++ ) // RAM Address ã��
    {
        U8 mu8LenCount = 0U;
        WifiRxFun_T pFun;
        I8 mi8Address[LENGTH_WIFI_ID_ONE+1];
        I8 mi8Data[LENGTH_WIFI_RAM_DATA];
        U16 m16Address_HEX = 0U;
        U16 m16Address_HEX_buf;
        U8 mu8WriteFlag = FALSE;
        U8 mu8ReadFlag = FALSE;

        _MEMSET_( (void __FAR*) mi8Address, '\0', sizeof(mi8Address) );
        _MEMSET_( (void __FAR*) mi8Data, '\0', sizeof(mi8Data) );

        for ( mu8LenCount = 0 ; mu8LenCount < LENGTH_WIFI_ID_ONE+1 ; mu8LenCount++ )
        {

            if(pBuf[mu16DataCount] == ASCII_COLON)
            {
                mu16DataCount++;
                break;
            }
            else if (pBuf[mu16DataCount] == ASCII_Z) // [Z1011.] Pass
            {
                mu16DataCount += LENGTH_WIFI_A90xx_PASS;
                mu8LenCount--;
            }
            else{
                mi8Address[mu8LenCount] = pBuf[mu16DataCount]; // {Z1011. ~ }
                mu16DataCount++;
            }
        }

        for ( mu8LenCount = 0 ; mu8LenCount < LENGTH_WIFI_RAM_DATA-1; mu8LenCount++ ) // RAM DATA �� ã��, [RAM DATA �� 10�����θ� ���?}
        {

            mi8Data[mu8LenCount] = pBuf[mu16DataCount]; // { FFFF(adress): ~ }
            mu16DataCount++;
            if ((rw_sel == WiFi_M_RAM_Write) && (mu8WriteFlag == FALSE))
            {
                mi8Data[++mu8LenCount] = 0x31;   // Write A9011 [FFFF(address): ?1(DATA)]
                mu8WriteFlag = TRUE;

            }
            else if ((rw_sel == WiFi_M_RAM_Read) && (mu8ReadFlag == FALSE)) {
                mi8Data[++mu8LenCount] = 0x30;   // Read A9013
                mi8Data[++mu8LenCount] = 0x30;   // Read A9013 [FFFF(address): ?00]
                mu8ReadFlag = TRUE;
            }

            if ( mi8Data[mu8LenCount] == ASCII_COMMA || mi8Data[mu8LenCount] == ASCII_CLOSE )
            {
                mi8Data[mu8LenCount] = ASCII_NULL;
                break;
            }

        }
        mi8Data[LENGTH_WIFI_RAM_DATA-1] = (I8)((mu8LenCount)-2); // Write DATA�� ���ڿ� ���̸� �� ������ ������ ���� 
                                                         // SetUserSystemRamMonitor���� ASCII�� 10������ ��ȯ �� ���?
        gu16_RAM_Address_len[i] = WIFI_ASCII2HEX(mi8Data[0]) & 0x000F; //mi8Data[0]; // ADDRESS �� Len DATA ����

        for ( mi = 0 ; mi < mu16Size; mi++ )
        {   
            m16Address_HEX_buf = (WIFI_ASCII2HEX(mi8Address[0])) ; // ASCII�� 16������ ��ȯ
            m16Address_HEX = (m16Address_HEX_buf << 12) & 0xF000;
            m16Address_HEX_buf = (WIFI_ASCII2HEX(mi8Address[1])) ;
            m16Address_HEX += (m16Address_HEX_buf << 8) & 0x0F00;
            // m16Address_HEX = (WIFI_ASCII2HEX(mi8Address[0]) << 12) & 0xF000; 
            // m16Address_HEX += (WIFI_ASCII2HEX(mi8Address[1]) << 8) & 0x0F00;
            m16Address_HEX += (WIFI_ASCII2HEX(mi8Address[2]) << 4) & 0x00F0;
            m16Address_HEX += WIFI_ASCII2HEX(mi8Address[3]) & 0x000F;

            gu16_START_RAM_Address[i] = m16Address_HEX;

            pFun = (pList)->RxFunc_M; //RAM Address ������ŭ ȣ���ϱ� ������ (pList+i)->RxFunc �� �ƴ�.
            if ( pFun != NULL )
            {
                pFun(m16Address_HEX, mi8Data ); // m8Address_HEX: start ram address(ex: F588)
                break;                         // mi8Data: 2165535 (len(ex: 2), R(0)/W(1), write data:65535)
            }
        }

    }
}

// Make Id (4Byte + 1Byte)
static void MakeCheckId ( I8* pBuf, U16 mLen, U16 mId )
{
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId >> 12) & 0x000F));
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId >> 8) & 0x000F));
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId >> 4) & 0x000F));
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId) & 0x000F));
    pBuf[mLen++] = ASCII_COLON; // 0x3A :
}

