/**
 * File : hal_serial.h
 * 
 * Hardware Abstraction Layer
 * Depend on Renesas MCU Chip
**/
#ifndef __HAL_SERIAL_H__
#define __HAL_SERIAL_H__

#include "prj_type.h"

/* UART CONFIG */
#define CONFIG_USE_UART_0       (0)   // NONE
#define CONFIG_USE_UART_1       (1)   // With MAIN
#define CONFIG_USE_UART_2       (1)   // With SUB FRONT
#define CONFIG_USE_UART_3       (0)   // NONE

/* UART ID */
typedef enum _comm_id_
{
    COMM_ID_MAIN,         /// 0
    COMM_ID_SUB_FRONT,    /// 1
    COMM_ID_MAX,          /// 2
} ECommId;

void HalUart0Start(void);
void HalUart0Stop(void);
void HalUart0ReceiveByte(U8 *rxData);
void HalUart0SendByte(U8 txData);
void HalUart0EnableInterrupt(U8 enable);

void HalUart1Start(void);
void HalUart1Stop(void);
void HalUart1ReceiveByte(U8 *rxData);
void HalUart1SendByte(U8 txData);
void HalUart1EnableInterrupt(U8 enable);

void HalUart2Start(void);
void HalUart2Stop(void);
void HalUart2ReceiveByte(U8 *rxData);
void HalUart2SendByte(U8 txData);
void HalUart2EnableInterrupt(U8 enable);

void HalUart3Start(void);
void HalUart3Stop(void);
void HalUart3ReceiveByte(U8 *rxData);
void HalUart3SendByte(U8 txData);
void HalUart3EnableInterrupt(U8 enable);

#endif  /* __HAL_SERIAL_H__ */
