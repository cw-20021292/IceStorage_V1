/**
 * File : comm.c
 * 
 * Uart Commnuication
*/
#include "comm.h"

#include "led.h"

CommHeader_T commHeader[COMM_ID_MAX];

#define COMM_RX_ERR_LIMIT_TIMEOUT (1800U)   // 3min, 180sec @100ms

#define COMM_MAIN_RX_BUF_SZ       (150)
#define COMM_MAIN_TX_BUF_SZ       (150)

U8 mainRecvBuf[COMM_MAIN_RX_BUF_SZ];
U8 mainSendBuf[COMM_MAIN_TX_BUF_SZ];

typedef struct _comm_
{
    U8 *recvBuf;
    U8 *sendBuf;
    I16 recvLen;
    I16 sendLen;
    I16 txLen;

    U16 rxErrLimitTimer;
    U8 rxErr;
} SComm_T;

SComm_T comm[COMM_ID_MAX];

void InitComm(void)
{
    MEMSET( (void __FAR *)&comm, 0x00, sizeof(SComm_T) );

    /// MAIN
    comm[COMM_ID_FRONT].recvBuf = &mainRecvBuf[0];
    comm[COMM_ID_FRONT].sendBuf = &mainSendBuf[0];
    comm[COMM_ID_FRONT].rxErrLimitTimer = COMM_RX_ERR_LIMIT_TIMEOUT;
    
    HalUart1Start();
}

void InitCommId(ECommId id)
{
    SComm_T *pComm = NULL;

    pComm = &comm[id];

    if(( id == COMM_ID_FRONT )
    )
    {
        MEMSET( (void __FAR *)&pComm->recvBuf[0], 0x00, COMM_MAIN_RX_BUF_SZ );
        MEMSET( (void __FAR *)&pComm->sendBuf[0], 0x00, COMM_MAIN_TX_BUF_SZ );
    }
    
    pComm->recvLen = 0;
    pComm->sendLen = 0;
    pComm->txLen = 0;
}

static I16 GetRecvBufferLength(ECommId id)
{
    I16 size = 0;

    if(( id == COMM_ID_FRONT )
    )
    {
        size = COMM_MAIN_RX_BUF_SZ;
    }

    return size;
}

U8 IsFullRecvBuffer(ECommId id)
{
    SComm_T *communication = NULL;

    communication = &comm[id];

    if( communication->recvLen >= GetRecvBufferLength(id) )
    {
        return TRUE;
    }

    return FALSE;
}

static IsEmptyRecvBuffer(ECommId id)
{
    SComm_T *communication = NULL;

    communication = &comm[id];

    if( communication->recvLen > 0 )
    {
        return FALSE;
    }

    return TRUE;
}

static I16 GetSendBufferLength(ECommId id)
{
    I16 size = 0;

    if(( id == COMM_ID_FRONT )
    )
    {
        size = COMM_MAIN_TX_BUF_SZ;
    }

    return size;
}

static IsFullSendBuffer(ECommId id)
{
    SComm_T *communication = NULL;

    communication = &comm[id];

    if( communication->sendLen >= GetSendBufferLength(id) )
    {
        return TRUE;
    }

    return FALSE;
}


void InitRecvLength(ECommId id)
{
    comm[id].recvLen = 0;
}

static I16 GetRecvLength(ECommId id)
{
    return comm[id].recvLen;
}

void InitSendLength(ECommId id)
{
    comm[id].sendLen = 0;
}

static I16 GetSendLength(ECommId id)
{
    return comm[id].sendLen;
}

void SetRecvBuffer(ECommId id, U8 data)
{
    SComm_T *communication = NULL;

    communication = &comm[id];
    communication->recvBuf[communication->recvLen] = data;
    communication->recvLen++;
}

static U8 GetRecvBuffer(ECommId id, U16 index)
{
    SComm_T *communicaiton = NULL;

    communicaiton = &comm[id];

    return communicaiton->recvBuf[index];
}

static void SetSendBuffer(ECommId id, U8 data)
{
    SComm_T *communicaiton = NULL;

    if( IsFullSendBuffer(id) == FALSE )
    {
        communicaiton = &comm[id];

        communicaiton->sendBuf[communicaiton->sendLen] = data;
        communicaiton->sendLen++;
    }   
}

/**
 * @ brief           Function For Get Raw Data In Send Buffer
 * @ detail          none
 * @ param    id     User Commnication Id
 * @ param    index  Send Buffer Index
 * @ return          none
**/
static U8 GetSendBuffer(ECommId id, U16 index)
{
    SComm_T *communication = NULL;

    communication = &comm[id];

    return communication->sendBuf[index];
}

/**
 * @ brief           Function For Send Data From Send Buffer
 * @ detail          none
 * @ param    id     User Commnication Id
 * @ return          none
**/
void SendByte(ECommId id)
{
    U8 txData = 0U;
    SComm_T *communication = NULL;

    communication = &comm[id];

    txData = communication->sendBuf[communication->txLen];
    communication->txLen++;

    if( id == COMM_ID_FRONT )
    {
        HalUart1SendByte(txData);
    }
}

/**
 * @ brief           Function For Check if Transmit is complete
 * @ detail          none
 * @ param    id     User Commnication Id
 * @ return          TRUE : Complete, FALSE : NO
**/
U8 IsCompleteTx(ECommId id)
{
    if( comm[id].txLen < comm[id].sendLen )
    {
        return FALSE;
    }

    return TRUE;
}

/**
 * @ brief          Function For Set Protocol Command
 * @ detail         none
 * @ param   id     User Commnication Id
 * @ param   cmd    Protocol Command
 * @ return         none
**/
void SetCommHeader(ECommId id, CommHeader_T cmd)
{
    // Set address of data model 
    commHeader[id] = cmd;
}

/**
 * @ brief          Function For Get Protocol Command
 * @ detail         none
 * @ param   id     User Commnication Id
 * @ return         Protocol Command
**/
CommHeader_T GetCommHeader(ECommId id)
{
    // Get address of data model 
    return commHeader[id];
}

/***********************************************************************************************
 *  SEND PACKET 
 */
I16 SendCommPacket(ECommId id, U8 *sendBuf, I16 len)
{
    U8 i = 0U;

    if( IsFullSendBuffer(id) != TRUE )
    {
        for( i=0; i<len; i++ )
        {
            SetSendBuffer(id, sendBuf[i]);
        }

        SendByte(id);
        return len;     /// send buf size
    }

    return -1; /// error - there is full data
}

/***********************************************************************************************/
/**
 * @brief Front로부터 수신한 패킷을 recvBuf에 copy하는 함수
 */
I8 RecvCommPacket(ECommId id, U8 *recvBuf)
{
    U16 i = 0U;
    I16 len;

    if( IsEmptyRecvBuffer(id) != TRUE )
    {
        len = GetRecvLength(id);
        for( i=0; i<len; i++ )
        {
            recvBuf[i] = GetRecvBuffer(id, i);
        }

        return len; /* received buf size */
    }

    return -1;  /* error - there is no data */
}

void SetFrontRxErr(U8 err)
{
    if( err == TRUE )
    {
        if( comm[COMM_ID_FRONT].rxErrLimitTimer == 0U )
        {
            return;
        }
    }

    comm[COMM_ID_FRONT].rxErr = err;
}

U8 GetMainRxErr(void)
{
    return comm[COMM_ID_FRONT].rxErr;
}

void UpdateFrontRxErrTimer(void)
{
    if( comm[COMM_ID_FRONT].rxErrLimitTimer != 0U )
    {
        comm[COMM_ID_FRONT].rxErrLimitTimer--;
    }
}

void DispFrontRxErr(void)
{
    static U8 blink = 10U;

    if( GetMainRxErr() == FALSE )
    {
        return;
    }

    AllOffLed();

    if( blink < 5U )
    {
        OnOffLed(LED_ID_SEG_1A, PROTOCOL_LED_DUTY_50);
        OnOffLed(LED_ID_SEG_1D, PROTOCOL_LED_DUTY_50);
    }
    else
    {
        OnOffLed(LED_ID_SEG_1A, 0U);
        OnOffLed(LED_ID_SEG_1D, 0U);
    }

    if( blink != 0U )
    {
        blink--;
    }
    else
    {
        blink = 10U;
    }
}

