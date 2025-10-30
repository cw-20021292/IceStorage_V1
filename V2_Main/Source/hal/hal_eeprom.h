
/* Define to prevent recursive inclusion ---------------------------------------*/
#ifndef __HAL_EEPROM_H__
#define __HAL_EEPROM_H__

/* Includes --------------------------------------------------------------------*/
#include "prj_type.h"

/* Exported Definitions --------------------------------------------------------*/
#define DEV_ADDR_EEP	      0xAE  /* Device EEPROM - ISL12026 */
#define DEV_ADDR_RTC	      0xDE  /* Device RTC - ISL12026 */

#define EEP_MAX_SIZE		   512   /* bytes */
#define EEP_PAGE_SIZE	       16    /* 16-byte page write */
#define EEP_PAGE_NUM	       32    /* 512 bytes / 16 page per bytes = 32 pages */
#define RTC_PAGE_SIZE          8     /* 8-byte page write for rtc */


#define EEP_PROC_START		0
#define EEP_PROC_DONE		1

/* Exported Functions ----------------------------------------------------------*/

void HAL_RTC_Init(void);

/**
  * @brief	: EEP processing status setting.
  * @param	: U8 EEP processing start or done.
  * @retval : None.
  */
void	HAL_RTC_SetWriteProc( U8 _proc );

/**
  * @brief	: EEP processing status read.
  * @param	: None.
  * @retval : U8 EEP processing start or done.
  */
U8	HAL_RTC_GetWriteProc( void );

/**
  * @brief	: EEP processing status setting.
  * @param	: U8 EEP processing start or done.
  * @retval : None.
  */
void	HAL_RTC_SetReadProc( U8 _proc );

/**
  * @brief	: EEP processing status read.
  * @param	: None.
  * @retval : U8 EEP processing start or done.
  */
U8	HAL_RTC_GetReadProc( void );

/**
  * @brief	: Writing a single byte.
  * @param	: U16 memory address, U8 data.
  * @retval : True or False.
  */
BOOL_T	HAL_RTC_ByteWrite( U8 _dev, U16 _addr , U8 _data );

/**
  * @brief	: Writing as the page size.
  * @param	: U16 memory address, U8 data.
  * @retval : True or False.
  */
BOOL_T	HAL_RTC_PageWrite( U8 _dev, U16 _addr , U8 *_data, U8 _len );

/**
  * @brief	: Read the data from the current address.
  * @param	: U8 data.
  * @retval : True or False.
  */
BOOL_T	HAL_RTC_CurrentAddrRead( U8 _dev, U8 *_data );

/**
  * @brief	: Read one byte.
  * @param	: U16 memory address, U8 * read data.
  * @retval : True or False.
  */
BOOL_T	HAL_RTC_ByteRead( U8 _dev, U16 _addr , U8 *_data );

/**
  * @brief	: Read multiple bytes.
  * @param	: U16 memory address, U8 * read data, U8 read size.
  * @retval : True or False.
  */
BOOL_T	HAL_RTC_SeqRead( U8 _dev, U16 _addr , U8 * _data, U8 _len );

/**
  * @brief	: EEPROM all erase.
  * @param	: None.
  * @retval : True or False.
  */
BOOL_T	HAL_RTC_EEPROM_AllErase( void );

#endif	/* !__HAL_EEPROM_H__ */

