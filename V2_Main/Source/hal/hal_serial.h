#ifndef __HAL_SERIAL_H__
#define __HAL_SERIAL_H__

#include "prj_type.h"

/* UART ID */
#define COMM_ID_FRONT         0
#define COMM_ID_WIFI          1
#define COMM_ID_COMP          2
#define COMM_ID_EOL           3
#define MAX_COMM_ID           4


// COMP
#define MAX_COMM_0_RX_BUF_SZ      128
#define MAX_COMM_0_TX_BUF_SZ      128

// WIFI
#define MAX_COMM_1_RX_BUF_SZ      8
#define MAX_COMM_1_TX_BUF_SZ      8

// FRONT
#define MAX_COMM_2_RX_BUF_SZ      30
#define MAX_COMM_2_TX_BUF_SZ      30

// EOL
#define MAX_COMM_3_RX_BUF_SZ      255
#define MAX_COMM_3_TX_BUF_SZ      255


#define MAX_COMM_RX_BUF_SZ        255
#define MAX_COMM_TX_BUF_SZ        255



void    HAL_InitCommId( U8 uart_id );
void    HAL_InitComm( void );

U8  HAL_IsFullRecvBuffer( U8 uart_id );
U8  HAL_IsEmptyRecvBuffer( U8 uart_id );
U8  HAL_IsFullSendBuffer( U8 uart_id );

void HAL_InitRecvLength(U8 uart_id );
I16  HAL_GetRecvLength( U8 uart_id );
I16  HAL_GetSendLength( U8 uart_id );

void    HAL_SetRecvBuffer(U8 uart_id, U8 _byte );
U8    HAL_GetRecvBuffer( U8 uart_id, U16 index );

void    HAL_SetSendBuffer( U8 uart_id, U8 _byte );
U8     HAL_GetSendBuffer( U8 uart_id, U16 index );

void    HAL_SendByte( U8 uart_id );

#endif /* __HAL_SERIAL_H__ */

