/***********************************************************************************************
 * INCLUDE 
 */
#include "hw.h"
#include "hal_serial.h"
#include "parser.h"
#include "comm.h"
#include "timer.h"

#include "parser_front.h"

#define PARSER_RX_BUF_SZ    (255)
#define PARSER_TX_BUF_SZ    (255)

#if( CONFIG_DEBUG_COMM == 1)
typedef struct _debug_comm_
{
    U32 rxCnt;
    U32 rxErr;

    U32 txCnt;
} SDebugComm_T; 
SDebugComm_T    dbgComm[COMM_ID_MAX];
#endif

/* Receivce buffer, length */
volatile U8 pktRecv[PARSER_RX_BUF_SZ];
I16 pktRecvLen = 0;

/* Send buffer, length */
volatile U8 pktSend[PARSER_TX_BUF_SZ];
I16 pktSendLen = 0;

/***********************************************************************************************
 * RECEIVE PACKET 
 */
typedef I16 (*FPParserRx_T)(U8 *buf, I16 len);
typedef struct _parser_rx_list_
{
    U8 timerId;
    U8 commId;
    FPParserRx_T  isValidPkt;
    FPParserRx_T  parserPkt;
} SParserRxList_T;

static const SParserRxList_T parserRxList[] = 
{
    { TIMER_USER_ID_COMM_FRONT_RX,       COMM_ID_FRONT,       IsValidPktFront,       ParserPktFront },
};
#define SZ_PARSER_RX   ( sizeof(parserRxList) / sizeof(SParserRxList_T) )

void RecvPacketHandler(void)
{
    SParserRxList_T *list = NULL;
    U8 i = 0U;

    for( i=0; i<SZ_PARSER_RX; i++ )
    {
        list = &parserRxList[i];
        if( IsExpiredTimer(TIMER_USER, list->timerId) == TIMER_EXPIRE )
        {
            DisableTimer(TIMER_USER, list->timerId);

            if( (pktRecvLen = RecvCommPacket(list->commId, &pktRecv[0]) ) > 0 )
            {
                InitRecvLength(list->commId);

                if( list->isValidPkt(&pktRecv[0], pktRecvLen) == TRUE )
                {
                    list->parserPkt(&pktRecv[0], pktRecvLen);

#if( CONFIG_DEBUG_COMM == 1 )
                    dbgComm[list->commId].rxCnt++;
#endif
                }
                else
                {
#if( CONFIG_DEBUG_COMM == 1 )
                    dbgComm[list->commId].rxErr++;
#endif
                }
            }
        }
    }
}

/***********************************************************************************************
 * SEND PACKET 
 */

typedef I16 (*FPParserTx_T)(CommHeader_T *p_comm, U8 *buf);
typedef I16 (*FPCrc16_T)(U8 *buf, I16 len);
typedef struct _parser_tx_list_
{
    U8 timerId;
    U8 commId;
    FPParserTx_T makePkt;
    FPCrc16_T crc16;
} SParserTxList_T;

static const SParserTxList_T parserTxList[] = 
{
    { TIMER_USER_ID_COMM_FRONT_TX, COMM_ID_FRONT, MakePktFront, Crc16Main },
};
#define SZ_PARSER_TX   ( sizeof(parserTxList) / sizeof(SParserTxList_T) )

void SendPacketHandler( void )
{
    SParserTxList_T *list = NULL;
    U8 i = 0U;
    CommHeader_T comm;


    for( i=0 ; i<SZ_PARSER_TX; i++ )
    {
        list = &parserTxList[i];

        if( IsExpiredTimer(TIMER_USER, list->timerId) == TIMER_EXPIRE )
        {
            DisableTimer(TIMER_USER, list->timerId);

            comm = GetCommHeader(list->commId);

            /* MAKE PACKET */
            MEMSET( (void __FAR*)pktSend, 0, PARSER_TX_BUF_SZ );
            pktSendLen = list->makePkt((CommHeader_T *)comm, pktSend);
            if( pktSendLen > 0 )
            {
                /* MAKE CRC-16 */
                pktSendLen = list->crc16(pktSend, pktSendLen);

                /* SEND PACKET BY UART */
                SendCommPacket(list->commId, pktSend, pktSendLen);

#if( CONFIG_DEBUG_COMM == 1 )
                dbgComm[list->commId].txCnt++;
#endif
            }
            else
            {
                InitCommId(list->commId);
            }
        }
    }
}

