
/* UART0 : FRONT */
#pragma interrupt INTST2    r_uart2_interrupt_send
#pragma interrupt INTSR2    r_uart2_interrupt_receive

#if 0
/* UART0 : FRONT */
#pragma interrupt INTST1    r_uart1_interrupt_send
#pragma interrupt INTSR1    r_uart1_interrupt_receive

/* UART1 : WIFI */
#pragma interrupt INTST0    r_uart0_interrupt_send
#pragma interrupt INTSR0    r_uart0_interrupt_receive

/* UART2 : LINE TEST / DEBUG */

/* UART3 : LCD */
#pragma interrupt INTST3    r_uart3_interrupt_send
#pragma interrupt INTSR3    r_uart3_interrupt_receive
#endif


#include "hw.h"
#include "hal_serial.h"
#include <string.h>
#include "timer.h"


#define  MAIN_TXD TXD2
#define  MAIN_RXD RXD2

#if 0
#define  WIFI_TXD TXD1
#define  WIFI_RXD RXD1

#define  EOL_TXD TXD2
#define  EOL_RXD RXD2

#define  LCD_TXD TXD3
#define  LCD_RXD RXD3
#endif


typedef struct _comm_
{
    U8 recv_buf[ MAX_COMM_RX_BUF_SZ ];
    I16 recv_len;
    U8 send_buf[ MAX_COMM_TX_BUF_SZ ];
    I16 send_len;
    I16 tx_len;
} comm_t;

comm_t  comm[ MAX_COMM_ID ];



void    HAL_InitCommId( U8 uart_id )
{
    memset( &comm[ uart_id ], 0, sizeof( comm_t ) );
}

void    HAL_InitComm( void )
{
    HAL_InitCommId( COMM_ID_MAIN );
    R_UART2_Start();
#if 0
    HAL_InitCommId( COMM_ID_WIFI );
    R_UART1_Start();

    HAL_InitCommId( COMM_ID_EOL );
    R_UART2_Start();

    HAL_InitCommId( COMM_ID_LCD );
    R_UART3_Start();
#endif
}


U8  HAL_IsFullRecvBuffer( U8 uart_id )
{
    if( comm[ uart_id ].recv_len >= MAX_COMM_RX_BUF_SZ )
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

U8  HAL_IsFullSendBuffer( U8 uart_id )
{
    if( comm[ uart_id ].send_len >= MAX_COMM_TX_BUF_SZ )
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
    p_comm->recv_buf[ p_comm->recv_len ] = _byte;
    p_comm->recv_len++;

}

U8 HAL_GetRecvBuffer( U8 uart_id, U16 index )
{
    return comm[ uart_id ].recv_buf[ index ];
}


void    HAL_SetSendBuffer( U8 uart_id, U8 _byte )
{
    comm_t *p_comm;
    
    if( HAL_IsFullSendBuffer( uart_id ) == FALSE )
    {
        p_comm = &comm[ uart_id ];
        p_comm->send_buf[ p_comm->send_len ] = _byte;
        p_comm->send_len++;
    }
}

U8  HAL_GetSendBuffer( U8 uart_id, U16 index )
{
    return comm[ uart_id ].send_buf[ index ];
}


void    HAL_SendByte( U8 uart_id )
{
    U8 ch;
    comm_t *p_comm;


    p_comm  = &comm[ uart_id ];
    ch      = p_comm->send_buf[ p_comm->tx_len ];
    p_comm->tx_len++;

#if 0
    if( uart_id == COMM_ID_EOL )
    {
       EOL_TXD = ch;
    }
    else if( uart_id == COMM_ID_WIFI )
    {
       WIFI_TXD = ch;
    }
    else if( uart_id == COMM_ID_LCD )
    {
       LCD_TXD = ch;
    }

#endif
    if( uart_id == COMM_ID_MAIN )
    {
       MAIN_TXD = ch;
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

#if 0
/* UART 1 */
__interrupt static void r_uart1_interrupt_receive(void)
{
    volatile U8 rx_data;
    volatile U8 err_type;

    err_type = (U8)(SSR03 & 0x0007U);
    SIR03 = (U16)err_type;

    rx_data = MAIN_RXD;
    
    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( COMM_ID_MAIN ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_MAIN, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_MAIN );
        }
    }

    StartTimer( TIMER_ID_COMM_MAIN_RX, 5 );
}

__interrupt static void r_uart1_interrupt_send(void)
{
    if( IsCompleteTx( COMM_ID_MAIN ) == FALSE )
    {
        HAL_SendByte( COMM_ID_MAIN );
    }
    else
    {
        HAL_InitCommId( COMM_ID_MAIN );
    }
}
#endif

/* UART 0 */
#if 0
__interrupt static void r_uart0_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;

    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    rx_data = MAIN_RXD;

    if( err_type == 0 )
    {
        StartTimer( TIMER_ID_COMM_MAIN_RX, 2 );
        if( HAL_IsFullRecvBuffer( COMM_ID_MAIN ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_MAIN, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_MAIN );
        }


    }
}

__interrupt static void r_uart0_interrupt_send(void)
{
    if( IsCompleteTx( COMM_ID_MAIN ) == FALSE )
    {
        HAL_SendByte( COMM_ID_MAIN );
    }
    else
    {
        HAL_InitCommId( COMM_ID_MAIN );
    }
}
#endif


#if 1

/* UART 3 */

__interrupt static void r_uart2_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;

    err_type = (uint8_t)(SSR11 & 0x0007U);
    SIR11 = (uint16_t)err_type;

    rx_data = MAIN_RXD;

    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( COMM_ID_MAIN ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_MAIN, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_MAIN );
        }

        StartTimer( TIMER_ID_COMM_MAIN_RX, 3 );
    }

}

__interrupt static void r_uart2_interrupt_send(void)
{
    if( IsCompleteTx( COMM_ID_MAIN ) == FALSE )
    {
        HAL_SendByte( COMM_ID_MAIN );
    }
    else
    {
        HAL_InitCommId( COMM_ID_MAIN );
    }
}
#endif


#if 0
/* UART 4 */
__interrupt static void r_uart3_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;

    err_type = (uint8_t)(SSR13 & 0x0007U);
    SIR13 = (uint16_t)err_type;

    rx_data = EOL_RXD;

    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( COMM_ID_LCD ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_LCD, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_LCD );
        }

        StartTimer( TIMER_ID_COMM_LCD_RX, 20 );
    }
}

__interrupt static void r_uart3_interrupt_send(void)
{
    if( IsCompleteTx( COMM_ID_LCD ) == FALSE )
    {
        HAL_SendByte( COMM_ID_LCD );
    }
    else
    {
        HAL_InitCommId( COMM_ID_LCD );
    }
}
#endif
