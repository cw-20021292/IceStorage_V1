#ifndef __HAL_SERIAL_H__
#define __HAL_SERIAL_H__

#include "prj_type.h"

/* UART ID */
#define COMM_ID_MAIN         0
#define MAX_COMM_ID           1
#if 0
#define COMM_ID_WIFI          1
#define COMM_ID_EOL           2
#define COMM_ID_LCD           3
#define MAX_COMM_ID           4
#endif

#define MAX_COMM_RX_BUF_SZ      256
#define MAX_COMM_TX_BUF_SZ      256

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

