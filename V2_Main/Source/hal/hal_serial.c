#define CONFIG_UART_0   1
#define CONFIG_UART_1   1
#define CONFIG_UART_2   1
#define CONFIG_UART_3   1  

/* UART0 : FRONT */
#if CONFIG_UART_0 
#pragma interrupt INTST0    r_uart0_interrupt_send
#pragma interrupt INTSR0    r_uart0_interrupt_receive
#endif

/* UART1 : WIFI */
#if CONFIG_UART_1
#pragma interrupt INTST1    r_uart1_interrupt_send
#pragma interrupt INTSR1    r_uart1_interrupt_receive
#endif

/* UART2 : BLDC COMP */
#if CONFIG_UART_2 
#pragma interrupt INTST2    r_uart2_interrupt_send
#pragma interrupt INTSR2    r_uart2_interrupt_receive
#endif

/* EOL (OK) */
#if CONFIG_UART_3 
#pragma interrupt INTST3    r_uart3_interrupt_send
#pragma interrupt INTSR3    r_uart3_interrupt_receive
#endif


#include "hw.h"
#include "hal_serial.h"
#include <string.h>
#include "timer.h"
#include "WIFI_Control.h"


#if CONFIG_UART_0
#define  FRONT_TXD TXD0
#define  FRONT_RXD RXD0
#endif

#if CONFIG_UART_1
#define  WIFI_TXD TXD1
#define  WIFI_RXD RXD1
#endif

#if CONFIG_UART_2
#define  COMP_TXD TXD2
#define  COMP_RXD RXD2
#endif

#if CONFIG_UART_3
#define  EOL_TXD TXD3
#define  EOL_RXD RXD3
#endif

// @ms
#define UART0_RX_TIME_STAMP   100
#define UART1_RX_TIME_STAMP   10
#define UART2_RX_TIME_STAMP   10
#define UART3_RX_TIME_STAMP   10


#if CONFIG_UART_0
U8 recv_0_buf[ MAX_COMM_0_RX_BUF_SZ ];
U8 send_0_buf[ MAX_COMM_0_TX_BUF_SZ ];
#endif

#if CONFIG_UART_1
U8 recv_1_buf[ MAX_COMM_1_RX_BUF_SZ ];
U8 send_1_buf[ MAX_COMM_1_TX_BUF_SZ ];
#endif

#if CONFIG_UART_2
U8 recv_2_buf[ MAX_COMM_2_RX_BUF_SZ ];
U8 send_2_buf[ MAX_COMM_2_TX_BUF_SZ ];
#endif

#if CONFIG_UART_3
U8 recv_3_buf[ MAX_COMM_3_RX_BUF_SZ ];
U8 send_3_buf[ MAX_COMM_3_TX_BUF_SZ ];
#endif

typedef struct _comm_
{
    //U8 recv_buf[ MAX_COMM_RX_BUF_SZ ];
    I16 recv_len;
    //U8 send_buf[ MAX_COMM_TX_BUF_SZ ];
    I16 send_len;
    I16 tx_len;
} comm_t;

comm_t  comm[ MAX_COMM_ID ];



void    HAL_InitCommId( U8 uart_id )
{
    if( uart_id == COMM_ID_FRONT)
    {
#if CONFIG_UART_0
        MEMSET( (void __FAR *)&recv_0_buf[0], 0, MAX_COMM_0_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_0_buf[0], 0, MAX_COMM_0_TX_BUF_SZ );
#endif
    }
    else if( uart_id == COMM_ID_WIFI )
    {
#if CONFIG_UART_1
        MEMSET( (void __FAR *)&recv_1_buf[0], 0, MAX_COMM_1_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_1_buf[0], 0, MAX_COMM_1_TX_BUF_SZ );
#endif
    }
    
    else if( uart_id == COMM_ID_COMP )
    {
#if CONFIG_UART_2
        MEMSET( (void __FAR *)&recv_2_buf[0], 0, MAX_COMM_2_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_2_buf[0], 0, MAX_COMM_2_TX_BUF_SZ );
#endif
    }

    else //if( uart_id == COMM_ID_EOL)
    {
#if CONFIG_UART_3
        MEMSET( (void __FAR *)&recv_3_buf[0], 0, MAX_COMM_3_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_3_buf[0], 0, MAX_COMM_3_TX_BUF_SZ );
#endif
    }

    MEMSET( (void __FAR *)&comm[ uart_id ], 0, sizeof( comm_t ) );
}

void    HAL_InitComm( void )
{
#if CONFIG_UART_0
    HAL_InitCommId( COMM_ID_FRONT );
    R_UART0_Start();
#endif

#if CONFIG_UART_1
    HAL_InitCommId( COMM_ID_WIFI );
    R_UART1_Start();
#endif

#if CONFIG_UART_2
    HAL_InitCommId( COMM_ID_COMP );
    R_UART2_Start();
#endif

#if CONFIG_UART_3
    HAL_InitCommId( COMM_ID_EOL );
    R_UART3_Start();
#endif

}

static I16 HAL_GetMaxRecvBufferLength(U8 uart_id)
{
    if( uart_id == 0 )
    {
        return MAX_COMM_0_RX_BUF_SZ;
    }
    else if( uart_id == 1 )
    {
        return MAX_COMM_1_RX_BUF_SZ;
    }
    else if( uart_id == 2 )
    {
        return MAX_COMM_2_RX_BUF_SZ;
    }

    return MAX_COMM_3_RX_BUF_SZ;
}

U8  HAL_IsFullRecvBuffer( U8 uart_id )
{
    if( comm[ uart_id ].recv_len >= HAL_GetMaxRecvBufferLength( uart_id ) )
    {
        return TRUE;
    }

    return FALSE;
}

U8  HAL_IsEmptyRecvBuffer( U8 uart_id )
{
    if( comm[ uart_id ].recv_len > 0 )
    {
        return FALSE;
    }

    return TRUE;

}

static I16 HAL_GetMaxSendBufferLength(U8 uart_id)
{
    if( uart_id == 0 )
    {
        return MAX_COMM_0_TX_BUF_SZ;
    }
    else if( uart_id == 1 )
    {
        return MAX_COMM_1_TX_BUF_SZ;
    }
    else if( uart_id == 2 )
    {
        return MAX_COMM_2_TX_BUF_SZ;
    }

    return MAX_COMM_3_TX_BUF_SZ;
}

U8  HAL_IsFullSendBuffer( U8 uart_id )
{
    if( comm[ uart_id ].send_len >= HAL_GetMaxSendBufferLength( uart_id ) )
    {
        return TRUE;
    }

    return FALSE;
}

void HAL_InitRecvLength(U8 uart_id )
{
    comm[ uart_id ].recv_len = 0;
}

I16  HAL_GetRecvLength( U8 uart_id )
{
    return comm[ uart_id ].recv_len;
}

I16  HAL_GetSendLength( U8 uart_id )
{
    return comm[ uart_id].send_len;
}


void    HAL_SetRecvBuffer(U8 uart_id, U8 _byte )
{
    comm_t *p_comm;

    p_comm = &comm[ uart_id ];
#if 0
    p_comm->recv_buf[ p_comm->recv_len ] = _byte;
#else
    if( uart_id == COMM_ID_FRONT)
    {
        recv_0_buf[ p_comm->recv_len ] = _byte;        
    }
    else if( uart_id == COMM_ID_WIFI )
    {
        recv_1_buf[ p_comm->recv_len ] = _byte;
    }
    else if( uart_id == COMM_ID_COMP )
    {
        recv_2_buf[ p_comm->recv_len ] = _byte;
    }
    else //if( uart_id == COMM_ID_EOL)
    {
        recv_3_buf[ p_comm->recv_len ] = _byte;
    }
#endif
    p_comm->recv_len++;

}

U8 HAL_GetRecvBuffer( U8 uart_id, U16 index )
{
#if 0
    return comm[ uart_id ].recv_buf[ index ];
#else
    if( uart_id == COMM_ID_FRONT)
    {
        return recv_0_buf[ index ];
    }
    else if( uart_id == COMM_ID_WIFI )
    {
        return recv_1_buf[ index ];
    }
    else if( uart_id == COMM_ID_COMP )
    {
        return recv_2_buf[ index ];
    }
    else //if( uart_id == COMM_ID_EOL)
    {
        return recv_3_buf[ index ];
    }
#endif
}


void    HAL_SetSendBuffer( U8 uart_id, U8 _byte )
{
    comm_t *p_comm;

    if( HAL_IsFullSendBuffer( uart_id ) == FALSE )
    {
        p_comm = &comm[ uart_id ];
#if 0
        p_comm->send_buf[ p_comm->send_len ] = _byte;
#else
        if( uart_id == COMM_ID_FRONT)
        {
            send_0_buf[ p_comm->send_len ] = _byte;
        }
        else if( uart_id == COMM_ID_WIFI )
        {
            send_1_buf[ p_comm->send_len ] = _byte;
        }
        else if( uart_id == COMM_ID_COMP )
        {
            send_2_buf[ p_comm->send_len ] = _byte;
        }
        else //if( uart_id == COMM_ID_EOL)
        {
            send_3_buf[ p_comm->send_len ] = _byte;
        }
#endif
        p_comm->send_len++;
    }
}

U8  HAL_GetSendBuffer( U8 uart_id, U16 index )
{
#if 0
    return comm[ uart_id ].send_buf[ index ];
#else
    if( uart_id == COMM_ID_COMP )
    {
        return send_2_buf[index];
    }
    else if( uart_id == COMM_ID_WIFI )
    {
        return send_1_buf[index];
    }
    else if( uart_id == COMM_ID_FRONT)
    {
        return send_0_buf[index];
    }
    else //if( uart_id == COMM_ID_EOL)
    {
        return send_3_buf[index];
    }
#endif
}


void    HAL_SendByte( U8 uart_id )
{
    U8 ch;
    comm_t *p_comm;


    p_comm  = &comm[ uart_id ];
#if 0
    ch      = p_comm->send_buf[ p_comm->tx_len ];
#else
    if( uart_id == COMM_ID_FRONT)
    {
        ch = send_0_buf[p_comm->tx_len];
    }
    else if( uart_id == COMM_ID_WIFI )
    {
        ch = send_1_buf[p_comm->tx_len];
    }
    else if( uart_id == COMM_ID_COMP )
    {
        ch = send_2_buf[p_comm->tx_len];
    }
    else //if( uart_id == COMM_ID_EOL)
    {
        ch = send_3_buf[p_comm->tx_len];
    }
#endif

    p_comm->tx_len++;

    if( uart_id == COMM_ID_FRONT )
    {
#if CONFIG_UART_0
        FRONT_TXD = ch;
#endif
    }
    else if( uart_id == COMM_ID_WIFI )
    {
#if CONFIG_UART_1
        WIFI_TXD = ch;
#endif
    }
    else if( uart_id == COMM_ID_COMP )
    {
#if CONFIG_UART_2
        COMP_TXD = ch;
#endif
    }
    else if( uart_id == COMM_ID_EOL )
    {
#if CONFIG_UART_3
        EOL_TXD = ch;
#endif
    }
}

static U8   IsCompleteTx( U8 uart_id )
{
    if( comm[uart_id].tx_len < comm[ uart_id ].send_len )
    {
        return FALSE;
    }

    return TRUE;

}

/* INTERRUPT */

/* UART 0 */
#if CONFIG_UART_0
__interrupt static void r_uart0_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;


    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    rx_data = FRONT_RXD;

    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( COMM_ID_FRONT ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_FRONT, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_FRONT );
        }

        StartTimer( TIMER_ID_COMM_FRONT_RX, UART2_RX_TIME_STAMP );
    }
}

__interrupt static void r_uart0_interrupt_send(void)
{
    if( IsCompleteTx( COMM_ID_FRONT ) == FALSE )
    {
        HAL_SendByte( COMM_ID_FRONT );
    }
    else
    {
        HAL_InitCommId( COMM_ID_FRONT );
    }
}
#endif

/* UART 1 */
#if CONFIG_UART_1
__interrupt static void r_uart1_interrupt_receive(void)
{
#if 0
    volatile U8 rx_data;
    volatile U8 err_type;

    err_type = (U8)(SSR03 & 0x0007U);
    SIR03 = (U16)err_type;

    rx_data = WIFI_RXD;

    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( COMM_ID_WIFI ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_WIFI, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_WIFI );
        }
    }

    StartTimer( TIMER_ID_COMM_WIFI_RX, UART1_RX_TIME_STAMP );
#endif

    volatile U8 rx_data;

    rx_data = WIFI_RXD;

    InterruptUartWifiError();
    InterruptUartWifiRx( rx_data );
}

__interrupt static void r_uart1_interrupt_send(void)
{
#if 0
    if( IsCompleteTx( COMM_ID_WIFI ) == FALSE )
    {
        HAL_SendByte( COMM_ID_WIFI );
    }
    else
    {
        HAL_InitCommId( COMM_ID_WIFI );
    }
#endif
    InterruptUartWifiTx();
}
#endif

/* UART 2 */
#if CONFIG_UART_2
U16 the_rx_err_count = 0;
__interrupt static void r_uart2_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;

    err_type = (uint8_t)(SSR11 & 0x0007U);
    SIR11 = (uint16_t)err_type;

    rx_data = COMP_RXD;

    EI();
    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( COMM_ID_COMP ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_COMP, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_COMP );
        }

        StartTimer( TIMER_ID_COMM_COMP_RX, UART0_RX_TIME_STAMP );
    }
    else
    {
        the_rx_err_count++;
    }
}

__interrupt static void r_uart2_interrupt_send(void)
{
    EI();
    if( IsCompleteTx( COMM_ID_COMP ) == FALSE )
    {
        HAL_SendByte( COMM_ID_COMP );
    }
    else
    {
        HAL_InitCommId( COMM_ID_COMP );
    }
}
#endif


/* UART 3 */
#if CONFIG_UART_3
U16 the_isr_err_count = 0;
__interrupt static void r_uart3_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;

    err_type = (uint8_t)(SSR13 & 0x0007U);
    SIR13 = (uint16_t)err_type;

    rx_data = EOL_RXD;

    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( COMM_ID_EOL ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_EOL, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_EOL );
        }

        StartTimer( TIMER_ID_COMM_EOL_RX, UART3_RX_TIME_STAMP );
    }
    else
    {
        the_isr_err_count++;
    }
}


__interrupt static void r_uart3_interrupt_send(void)
{
    if( IsCompleteTx( COMM_ID_EOL ) == FALSE )
    {
        HAL_SendByte( COMM_ID_EOL );
    }
    else
    {
        HAL_InitCommId( COMM_ID_EOL );
    }
}
#endif
