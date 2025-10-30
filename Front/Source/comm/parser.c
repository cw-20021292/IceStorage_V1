/***********************************************************************************************
 * INCLUDE 
 */
#include "hw.h"
#include "hal_serial.h"
#include "parser.h"
#include "comm.h"
#include "timer.h"

#include "parser_main.h"

#define DEBUG_COMM 0

#if DEBUG_COMM
typedef struct _debug_comm_
{
    U32 rx_cnt;
    U32 rx_err;

    U32 tx_cnt;
} debug_comm_t; 
debug_comm_t    d_comm[ MAX_COMM_ID ];
#endif

/* Receivce buffer, length */
volatile U8 pkt_recv[ MAX_COMM_RX_BUF_SZ ];
I16 pkt_recv_len = 0;

/* Send buffer, length */
volatile U8 pkt_send[ MAX_COMM_TX_BUF_SZ ];
I16 pkt_send_len = 0;

/***********************************************************************************************
 * RECEIVE PACKET 
 */
typedef I16 (*fn_parser_rx_t)( U8 *buf, I16 len );
typedef struct _parser_list_
{
    U8 TimerId;
    U8 CommId;
    fn_parser_rx_t  IsValidPkt;
    fn_parser_rx_t  ParserPkt;
} parser_rx_list_t;

static const parser_rx_list_t parser_rx_list[] = 
{
    { TIMER_ID_COMM_MAIN_RX,   COMM_ID_MAIN   ,  IsValidPkt_Main,   ParserPkt_Main  }
};
#define MAX_PARSER_RX_NUM   ( sizeof( parser_rx_list) / sizeof( parser_rx_list_t ) )

void RecvPacketHandler( void )
{
    parser_rx_list_t    *p_list;
    U8  i;

    for( i = 0 ; i < MAX_PARSER_RX_NUM ; i++ )
    {
        p_list = &parser_rx_list[ i ];
        if( IsExpiredTimer( p_list->TimerId ) == TIMER_EXPIRE )
        {
            DisableTimer( p_list->TimerId );

            if( ( pkt_recv_len = CommRecvPacket( p_list->CommId, &pkt_recv[0] ) ) > 0 )
            {
                HAL_InitRecvLength( p_list->CommId );

                if( p_list->IsValidPkt( &pkt_recv[0], pkt_recv_len ) == TRUE )
                {
                    p_list->ParserPkt( &pkt_recv[0], pkt_recv_len );

                    switch( p_list->CommId )
                    {
                        case COMM_ID_MAIN :
                            break;
                        default :
                            break;
                    }

#if DEBUG_COMM
                    d_comm[ p_list->CommId ].rx_cnt++;
#endif
                }
                else
                {


#if DEBUG_COMM
                    d_comm[ p_list->CommId ].rx_err++;
#endif
                }
            }
        }
    }
}

/***********************************************************************************************
 * SEND PACKET 
 */

typedef I16 (*fn_parser_tx_t)( CommHeader_T *p_comm, U8 *buf );
typedef I16 (*fn_crc16_t)( U8 *buf, I16 len  );
typedef struct _parser_tx_list_
{
    U8 TimerId;
    U8 CommId;
    fn_parser_tx_t make_pkt;
    fn_crc16_t crc16;

} parser_tx_list_t;

static const parser_tx_list_t parser_tx_list[] = 
{
    { TIMER_ID_COMM_MAIN_TX,   COMM_ID_MAIN,     MakePkt_Main,    Crc16_Main    }
};
#define MAX_PARSER_TX_NUM   ( sizeof( parser_tx_list) / sizeof( parser_tx_list_t ) )

void SendPacketHandler( void )
{
    parser_tx_list_t    *p_list;
    U8  i;
    CommHeader_T        p_comm;


    for( i = 0 ; i < MAX_PARSER_TX_NUM ; i++ )
    {
        p_list = &parser_tx_list[ i ];

        if( IsExpiredTimer( p_list->TimerId ) == TIMER_EXPIRE )
        {
            DisableTimer( p_list->TimerId );

            p_comm = GetCommHeader( p_list->CommId );

            /* MAKE PACKET */
            memset( pkt_send, 0, MAX_COMM_TX_BUF_SZ );
            pkt_send_len = p_list->make_pkt( (CommHeader_T *)p_comm, pkt_send );
            if( pkt_send_len > 0 )
            {
                /* MAKE CRC-16 */
                pkt_send_len = p_list->crc16( pkt_send, pkt_send_len );

                /* SEND PACKET BY UART */
                CommSendPacket( p_list->CommId, pkt_send, pkt_send_len );

#if DEBUG_COMM
                d_comm[ p_list->CommId ].tx_cnt++;
#endif
            }
            else
            {
                switch( p_list->TimerId )
                {
                    case TIMER_ID_COMM_MAIN_TX :
                        HAL_InitCommId( COMM_ID_MAIN );
                        break;

                    default :
                        break;
                }
            }
        }
    }
}

