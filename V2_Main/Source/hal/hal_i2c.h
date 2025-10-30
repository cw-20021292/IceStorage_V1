/* Define to prevent recursive inclusion ---------------------------------------*/
#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__

/* Includes --------------------------------------------------------------------*/
#include "r_cg_macrodriver.h"
#include "prj_type.h"

/* Exported Definitions --------------------------------------------------------*/

/* Exported Variables ----------------------------------------------------------*/

/* Exported Functions ----------------------------------------------------------*/
/* I2C A0 -------------------------------------------------------------------*/
/**
  * @brief	: This function starts to send data as master mode.
  * @param	: Transfer address, transfer buffer pointer, buffer size, wait for start condition.
  * @retval : Status.
  */
MD_STATUS	i2ca0_master_send( uint8_t _addr, uint8_t * _tx_buf, uint16_t _tx_num, uint8_t _wait );

/**
  * @brief	: This function starts to receive data as master mode.
  * @param	: Receive address, receive buffer pointer, buffer size, wait for start condition.
  * @retval : Status.
  */
MD_STATUS	i2ca0_master_recv( uint8_t _addr, uint8_t * _rx_buf, uint16_t _rx_num, uint8_t _wait );

/* I2C A1 -------------------------------------------------------------------*/
/**
  * @brief	: This function starts to send data as master mode.
  * @param	: Transfer address, transfer buffer pointer, buffer size, wait for start condition.
  * @retval : Status.
  */
MD_STATUS	i2ca1_master_send( uint8_t _addr, uint8_t * _tx_buf, uint16_t _tx_num, uint8_t _wait );

/**
  * @brief	: This function starts to receive data as master mode.
  * @param	: Receive address, receive buffer pointer, buffer size, wait for start condition.
  * @retval : Status.
  */
MD_STATUS	i2ca1_master_recv( uint8_t _addr, uint8_t * _rx_buf, uint16_t _rx_num, uint8_t _wait );


#endif /* __HAL_I2C_H__ */


