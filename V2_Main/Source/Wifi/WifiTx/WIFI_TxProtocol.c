
/*******************************************************************************
&Description :
    Coway Protocol
    Send AT Protocol + Header + Attribute + End Cmd
  
&Histiry : 
    ShinHM 2021.2.22 update
    -. Add the "TO_SCONN" cmd
    -. 2021.05.13 ¿¹¿ÜÁ¶°Ç Ãß°¡
*******************************************************************************/


/***** Header *****************************************************************/
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_TxProtocol.h"
#include "WIFI_TxForm.h"
#include "WIFI_Status.h"
#include "WIFI_UartInterrupt.h"


/***** Define *****************************************************************/
/* Request ID (MCU -> Server) */
// const signed char REQUEST_ID_START[]    = "22MCU2SVR";
// const signed char REQUEST_ID_START[]    = "23MCU2SVR";
const signed char REQUEST_ID_START[]    = "24MCU2SVR";
const signed char REQUEST_ID_END[]      = "-01";


/***** Variable ***************************************************************/
I8 ai8RequestId[LENGTH_REQUEST_ID+1]; // Request ID
I8 ai8RequestIdException[LENGTH_REQUEST_ID+1]; // except Request ID 


/***** Constant ***************************************************************/


/***** Static Function ********************************************************/
static void MakeAtCmd ( I8* pBuf, I8* pAtcmd, U8 mu8To );
static void MakePacket ( I8* pBuf, I8* pApi, I8* pData, U8 mu8ReId );
static void SendPacket ( I8* mi8AtCmd, I8* mai8DataBuf );


/***** Table ******************************************************************/


/***** Global Function ********************************************************/
void InitializeWifiTxProtocol ( void )
{
    _MEMSET_( (void __FAR*) ai8RequestId, '\0', sizeof(ai8RequestId) );
}

void SetRequestId ( I8* mi8RqId )
{
    _MEMSET_( (void __FAR*) ai8RequestId, '\0', sizeof(ai8RequestId) );
    _STRCPY_( (char __FAR*) ai8RequestId, (const char __FAR*) mi8RqId );
}

void SetRequestIdExcept ( void )
{
    _MEMSET_( (void __FAR*) ai8RequestIdException, '\0', sizeof(ai8RequestIdException) );
    _STRCPY_( (char __FAR*) ai8RequestIdException, (const char __FAR*) ai8RequestId );
    _MEMSET_( (void __FAR*) ai8RequestId, '\0', sizeof(ai8RequestId) );
}

void WifiSendProtocol ( U8 mu8Target, U8 mu8ReId, I8* mi8Data, I8* mi8AtCmd, I8* mi8Api )
{
    static I8 mai8TxBuff[LENGTH_WIFI_ALL_MAX_N];
    // static I8 mai8TxBuff[LENGTH_WIFI_ALL_MAX];
    static I8 mai8ATCmd[LENGTH_AT_CMD_MAX];

    _MEMSET_( (void __FAR*) mai8TxBuff, '\0', sizeof(mai8TxBuff) );
    _MEMSET_( (void __FAR*) mai8ATCmd, '\0', sizeof(mai8ATCmd) );
    
    /* Make Data Packet */
    if ( mu8Target == TO_SERVER )
    {
        far_sprintf_s( mai8ATCmd, 0, mi8AtCmd );
    	MakePacket( mai8TxBuff, mi8Api, mi8Data, mu8ReId );
    }
    else
    {
    	MakeAtCmd( mai8ATCmd, mi8AtCmd, mu8Target );
    }
    
    /* Send Packet (Send UART1 TX) */
    SendPacket( mai8ATCmd, mai8TxBuff );
}


/***** Local Function *********************************************************/
static void MakeAtCmd ( I8* pBuf, I8* pAtcmd, U8 mu8Target )
{
    WifiTxCommandList_T *pList = NULL;
    U16 mu16CmdSize = 0U;
    U16 i = 0U;
    U16 mu16Len = 0U;
    U8 mData = 0U;

    mData = GetWifiBLESendStatus(TX_BLE_WIFI);
    pList = (WifiTxCommandList_T *)GetWifiTxCmdEventList(mData);
    mu16CmdSize = GetWifiTxCmdListSize(mData);
    // pList = (WifiTxCommandList_T *)GetWifiTxCmdEventList();
    // mu16CmdSize = GetWifiTxCmdListSize();

    mu16Len = far_sprintf_s( pBuf, 0, pAtcmd );
    
    for ( i = 0 ; i < mu16CmdSize; i++ )
    {
        WifiExFun_T pFun;
        if ( mu8Target == (pList+i)->Target )
    	{
            pFun = (pList+i)->Func;
            if ( pFun != NULL )
            {
                /* AT Cmd $)C?9?\A60G C38. */
                pFun( mu8Target, pBuf, mu16Len );
            }
            break;
    	}
    }
}

static void MakePacket ( I8* pBuf, I8* pApi, I8* pData, U8 mu8ReId )
{
    static U16 mu16RequestCount = 0U;
    U16 mu16Len = 0U;
    U16 mu16BodyLength = 0U;
    U32 mu32Checksum = 0UL;
    U8 mu8Checksum = 0U;
    U16 mu16i = 0U;

    /***** MakePacket *****/
    /* STX (1Byte) */
    pBuf[mu16Len++] = ASCII_STX;
    
    /* Body Length (4Byte) */
    /* length = 5(API) + 16(RequestID) 1('{') + Body(Attribute) + 1('}') + 2(Checksum) + 1(EXT) */
    mu16BodyLength = LENGTH_API_NO + LENGTH_REQUEST_ID + 1 + _STRLEN_( (const char __FAR*) pData ) + 1 + 2 + 1;
    pBuf[mu16Len++] = WIFI_HEX2ASCII((U8)((mu16BodyLength >> 12) & 0x000F));
    pBuf[mu16Len++] = WIFI_HEX2ASCII((U8)((mu16BodyLength >> 8) & 0x000F));
    pBuf[mu16Len++] = WIFI_HEX2ASCII((U8)((mu16BodyLength >> 4) & 0x000F));
    pBuf[mu16Len++] = WIFI_HEX2ASCII((U8)((mu16BodyLength) & 0x000F));
  	
    /* API (5Byte) */
    mu16Len += far_sprintf_s( &pBuf[mu16Len], mu16Len, pApi );
		
    /* Request ID (16Byte) */
    if ( ( mu8ReId == REUSED ) && ( ai8RequestIdException[0] != ASCII_NULL ) )
    {
        mu16Len += far_sprintf_s( &pBuf[mu16Len], mu16Len, ai8RequestIdException );
    	_MEMSET_( (void __FAR*) ai8RequestIdException, '\0', sizeof(ai8RequestIdException) );
    }
    else if ( ( mu8ReId == USED ) && ( ai8RequestId[0] != ASCII_NULL ) )
    {
        mu16Len += far_sprintf_s( &pBuf[mu16Len], mu16Len, ai8RequestId );
    	_MEMSET_( (void __FAR*) ai8RequestId, '\0', sizeof(ai8RequestId) );
    }
    else
    {
        mu16Len += far_sprintf_s( &pBuf[mu16Len], mu16Len, REQUEST_ID_START );
        pBuf[mu16Len++] = WIFI_HEX2ASCII((U8)((mu16RequestCount >> 12) & 0x000F));
        pBuf[mu16Len++] = WIFI_HEX2ASCII((U8)((mu16RequestCount >> 8) & 0x000F));
    	pBuf[mu16Len++] = WIFI_HEX2ASCII((U8)((mu16RequestCount >> 4) & 0x000F));
        pBuf[mu16Len++] = WIFI_HEX2ASCII((U8)((mu16RequestCount) & 0x000F));
    	mu16Len += far_sprintf_s( &pBuf[mu16Len], mu16Len, REQUEST_ID_END );
    	mu16RequestCount++;
    	if ( mu16RequestCount == 0xFFFF )
    	{
    	    mu16RequestCount = 0U;
    	}
    }
    
    /* Attribute (Max ***Byte) */
    pBuf[mu16Len++] = ASCII_OPEN; // '{'
    mu16Len += far_sprintf_s( &pBuf[mu16Len], mu16Len, pData );
    pBuf[mu16Len++] = ASCII_CLOSE; // '}'
    
    /* Check Sum (2Byte) */
    for ( mu16i = 0 ; mu16i < mu16Len ; mu16i++ )
    {
        mu32Checksum += pBuf[mu16i];
    }
    mu8Checksum = (U8)(mu32Checksum);
    pBuf[mu16Len++] = WIFI_HEX2ASCII((U8)((mu8Checksum >> 4) & 0x0F));
    pBuf[mu16Len++] = WIFI_HEX2ASCII((U8)((mu8Checksum) & 0x0F));
    
    /* ETX (1Byte) */
    pBuf[mu16Len++] = ASCII_ETX;
}

static void SendPacket ( I8* mi8AtCmd, I8* mai8DataBuf )
{
    U16 mu16Len = 0U;
    static I8 mai8Data[LENGTH_WIFI_ALL_MAX_N];
    // static I8 mai8Data[LENGTH_WIFI_ALL_MAX];
    
    _MEMSET_( (void __FAR*) mai8Data, '\0', sizeof(mai8Data) );
    
    /* AT Command */
    //mu16Cnt = sprintf( mai8Data, "%s", mi8AtCmd );
    mu16Len = far_sprintf_s( mai8Data, 0, mi8AtCmd );
    
    /* Protocol (Coway Header + Body) */
    //mu16Cnt += sprintf( (__far char*)&mai8Data[mu16Cnt], (const char __far*)"%s", (char __far*)mi8param );
    mu16Len += far_sprintf_s( &mai8Data[mu16Len], mu16Len, mai8DataBuf );
    
    /* Done (0x0D) */
    mai8Data[mu16Len++] = ASCII_DONE;
    
    /* Send Uart */
    SendWifiUart( mu16Len, mai8Data );
}


