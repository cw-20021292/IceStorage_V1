
/* Includes --------------------------------------------------------------------*/
#pragma interrupt INTIICA0 r_i2ca0_interrupt
#pragma interrupt INTIICA1 r_i2ca1_interrupt

#define TEST_EEP_NO_STOP    1

#include "hal_i2c.h"
#include "r_cg_serial.h"
#include "hal_eeprom.h"


/* Definitions -----------------------------------------------------------------*/

/* Variables -------------------------------------------------------------------*/
/* I2C A0 -------------------------------------------------------------------*/
static	volatile uint8_t	i2ca0_mstatus	= 0;
static	volatile uint8_t *i2ca0_rx_pbuf	= 0;
static	volatile uint16_t	i2ca0_rx_len   = 0;
static	volatile uint16_t	i2ca0_rx_cnt   = 0;

static	volatile uint8_t *i2ca0_tx_pbuf	= 0;
static	volatile uint16_t	i2ca0_tx_cnt	= 0;

/* I2C A1 -------------------------------------------------------------------*/
static	volatile uint8_t	i2ca1_mstatus	= 0;
static	volatile uint8_t *i2ca1_rx_pbuf	= 0;
static	volatile uint16_t	i2ca1_rx_len	= 0;
static	volatile uint16_t	i2ca1_rx_cnt	= 0;

static	volatile uint8_t *i2ca1_tx_pbuf	= 0;
static	volatile uint16_t	i2ca1_tx_cnt	= 0;

/* I2C A0 -------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------*/
/**
 * @brief	: This function starts to send data as master mode.
 * @param	: Transfer address, transfer buffer pointer, buffer size, wait for start condition.
 * @retval : Status.
 */
MD_STATUS	i2ca0_master_send( uint8_t _addr, uint8_t * _tx_buf, uint16_t _tx_num, uint8_t _wait )
{
    MD_STATUS status = MD_OK;

    IICAMK0 = 1U;									/* disable INTIICA0 interrupt */

    /* Check bus busy */
    if( 1U == IICBSY0 )
    {
        IICAMK0	= 0U;								/* enable INTIICA0 interrupt */
        status	= MD_ERROR1;
    } 
    /* Check trigger */
    else if( ( 1U == SPT0 ) || ( 1U == STT0 ) ) 
    {
        IICAMK0	= 0U;								/* enable INTIICA0 interrupt */  
        status	= MD_ERROR2;
    }
    else
    {
        STT0	= 1U;								/* send IICA0 start condition */
        IICAMK0	= 0U;								/* enable INTIICA0 interrupt */

        /* Wait */
        while( _wait-- )
        {
            ;
        }

        if( 0U == STD0 )
        {
            status = MD_ERROR3;
        }

        /* Set parameter */
        i2ca0_tx_cnt	= _tx_num;
        i2ca0_tx_pbuf	= _tx_buf;
        i2ca0_mstatus	= _00_IICA_MASTER_FLAG_CLEAR;
        _addr		&= (uint8_t)~0x01U;				/* set send mode */
        IICA0		= _addr;						/* send address */
    }

    return ( status );
}

/**
 * @brief	: This function starts to receive data as master mode.
 * @param	: Receive address, receive buffer pointer, buffer size, wait for start condition.
 * @retval : Status.
 */
MD_STATUS	i2ca0_master_recv( uint8_t _addr, uint8_t * _rx_buf, uint16_t _rx_num, uint8_t _wait )
{
    MD_STATUS status = MD_OK;


#if !TEST_EEP_NO_STOP
    IICAMK0 = 1U;									/* disable INTIIA0 interrupt */

    /* Check bus busy */
    if( 1U == IICBSY0 )
    {
        IICAMK0	= 0U;								/* enable INTIIA0 interrupt */
        status	= MD_ERROR1;
    }
    /* Check trigger */
    else if( ( 1U == SPT0 ) || ( 1U == STT0 ) )
    {
        IICAMK0	= 0U;								/* enable INTIICA0 interrupt */
        status	= MD_ERROR2;
    }
    else
#endif
    {
        STT0	= 1U;								/* set IICA0 start condition */
        IICAMK0	= 0U;								/* enable INTIIA0 interrupt */

        /* Wait */
        while( _wait-- )
        {
            ;
        }

        if( 0U == STD0 )
        {
            status = MD_ERROR3;
        }

        /* Set parameter */
        i2ca0_rx_cnt	= 0U;
        i2ca0_rx_len	= _rx_num;
        i2ca0_rx_pbuf	= _rx_buf;
        i2ca0_mstatus	= _00_IICA_MASTER_FLAG_CLEAR;
        _addr		|= 0x01U;						/* set receive mode */
        IICA0		= _addr;						/* receive address */
    }

    return ( status );
}


static void i2ca0_callback_master_error(MD_STATUS flag);
static void i2ca0_callback_master_receiveend(void);
static void i2ca0_callback_master_sendend(void);

/**
 * @brief	: I2C master handler.
 * @param	: None.
 * @retval : None.
 */
static void	i2ca0_master_handler( void )
{
    /* Control for communication */
    if( ( 0U == IICBSY0 ) && ( i2ca0_tx_cnt != 0U ) )
    {
        i2ca0_callback_master_error( MD_SPT );
    }
    /* Control for sended address */
    else
    {
        if( ( i2ca0_mstatus & _80_IICA_ADDRESS_COMPLETE ) == 0U )
        {
            if( 1U == ACKD0 )
            {
                i2ca0_mstatus |= _80_IICA_ADDRESS_COMPLETE;

                if( 1U == TRC0 )
                {
                    WTIM0 = 1U;

                    if( i2ca0_tx_cnt > 0U )
                    {
                        IICA0 = *i2ca0_tx_pbuf;
                        i2ca0_tx_pbuf++;
                        i2ca0_tx_cnt--;
                    }
                    else
                    {
                        i2ca0_callback_master_sendend();
                    }
                }
                else
                {
                    ACKE0 = 1U;
                    WTIM0 = 0U;
                    WREL0 = 1U;
                }
            }
            else
            {
                i2ca0_callback_master_error( MD_NACK );
            }
        }
        else
        {
            /* Master send control */
            if( 1U == TRC0 )
            {
                if( ( 0U == ACKD0 ) && ( i2ca0_tx_cnt != 0U ) )
                {
                    i2ca0_callback_master_error( MD_NACK );

                }
                else
                {
                    if( i2ca0_tx_cnt > 0U )
                    {
                        IICA0 = *i2ca0_tx_pbuf;
                        i2ca0_tx_pbuf++;
                        i2ca0_tx_cnt--;
                    }
                    else
                    {
                        i2ca0_callback_master_sendend();
                    }
                }
            }
            /* Master receive control */
            else
            {
                if( i2ca0_rx_cnt < i2ca0_rx_len )
                {
                    *i2ca0_rx_pbuf = IICA0;
                    i2ca0_rx_pbuf++;
                    i2ca0_rx_cnt++;

                    if( i2ca0_rx_cnt == i2ca0_rx_len )
                    {
                        ACKE0 = 0U;
                        WREL0 = 1U;
                        WTIM0 = 1U;
                    }
                    else
                    {
                        WREL0 = 1U;
                    }
                }
                else
                {
                    i2ca0_callback_master_receiveend();
                }
            }
        }
    }
}

/**
 * @brief	: Callback function when master error occurs.
 * @param	: Error flag.
 * @retval : None.
 */
static void i2ca0_callback_master_error( MD_STATUS flag )
{

    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/**
 * @brief	: Callback function when finishes master reception.
 * @param	: None.
 * @retval : None.
 */
static void i2ca0_callback_master_receiveend( void )
{
#if !TEST_EEP_NO_STOP
    SPT0 = 1U;
#endif

    HAL_RTC_SetReadProc( EEP_PROC_DONE );
}

/**
 * @brief	: Callback function when finishes master transmission.
 * @param	: None.
 * @retval : None.
 */
static void i2ca0_callback_master_sendend( void )
{
#if !TEST_EEP_NO_STOP
    SPT0 = 1U;
#endif

    HAL_RTC_SetWriteProc( EEP_PROC_DONE );
}

__interrupt static void r_i2ca0_interrupt(void)
{
    if ((IICS0 & _80_IICA_STATUS_MASTER) == 0x80U)
    {
        i2ca0_master_handler();
    }
}

/* I2C A1 -------------------------------------------------------------------*/
/* Functions -------------------------------------------------------------------*/
/**
 * @brief	: This function starts to send data as master mode.
 * @param	: Transfer address, transfer buffer pointer, buffer size, wait for start condition.
 * @retval : Status.
 */
MD_STATUS	i2ca1_master_send( uint8_t _addr, uint8_t * _tx_buf, uint16_t _tx_num, uint8_t _wait )
{
    MD_STATUS status = MD_OK;


    IICAMK1 = 1U;									/* disable INTIICA0 interrupt */

    /* Check bus busy */
    if( 1U == IICBSY1 )
    {
        IICAMK1	= 0U;								/* enable INTIICA0 interrupt */
        status	= MD_ERROR1;
    } 
    /* Check trigger */
    else if( ( 1U == SPT1 ) || ( 1U == STT1 ) ) 
    {
        IICAMK1	= 0U;								/* enable INTIICA0 interrupt */  
        status	= MD_ERROR2;
    }
    else
    {
        STT1	= 1U;								/* send IICA0 start condition */
        IICAMK1	= 0U;								/* enable INTIICA0 interrupt */

        /* Wait */
        while( _wait-- )
        {
            ;
        }

        if( 0U == STD1 )
        {
            status = MD_ERROR3;
        }

        /* Set parameter */
        i2ca1_tx_cnt	= _tx_num;
        i2ca1_tx_pbuf	= _tx_buf;
        i2ca1_mstatus	= _00_IICA_MASTER_FLAG_CLEAR;
        _addr		&= (uint8_t)~0x01U;				/* set send mode */
        IICA1		= _addr;						/* send address */
    }

    return ( status );
}

/**
 * @brief	: This function starts to receive data as master mode.
 * @param	: Receive address, receive buffer pointer, buffer size, wait for start condition.
 * @retval : Status.
 */
MD_STATUS	i2ca1_master_recv( uint8_t _addr, uint8_t * _rx_buf, uint16_t _rx_num, uint8_t _wait )
{
    MD_STATUS status = MD_OK;

    IICAMK1 = 1U;									/* disable INTIIA1 interrupt */

    /* Check bus busy */
    if( 1U == IICBSY1 )
    {
        IICAMK1	= 0U;								/* enable INTIIA1 interrupt */
        status	= MD_ERROR1;
    }
    /* Check trigger */
    else if( ( 1U == SPT1 ) || ( 1U == STT1 ) )
    {
        IICAMK1	= 0U;								/* enable INTIICA1 interrupt */
        status	= MD_ERROR2;
    }
    else
    {
        STT1	= 1U;								/* set IICA1 start condition */
        IICAMK1	= 0U;								/* enable INTIIA1 interrupt */

        /* Wait */
        while( _wait-- )
        {
            ;
        }

        if( 0U == STD1 )
        {
            status = MD_ERROR3;
        }

        /* Set parameter */
        i2ca1_rx_cnt	= 0U;
        i2ca1_rx_len	= _rx_num;
        i2ca1_rx_pbuf	= _rx_buf;
        i2ca1_mstatus	= _00_IICA_MASTER_FLAG_CLEAR;
        _addr		|= 0x01U;						/* set receive mode */
        IICA1		= _addr;						/* receive address */
    }

    return ( status );
}


static void i2ca1_callback_master_error(MD_STATUS flag);
static void i2ca1_callback_master_receiveend(void);
static void i2ca1_callback_master_sendend(void);

/**
 * @brief	: I2C master handler.
 * @param	: None.
 * @retval : None.
 */
static void	i2ca1_master_handler( void )
{
    /* Control for communication */
    if( ( 0U == IICBSY1 ) && ( i2ca1_tx_cnt != 0U ) )
    {
        i2ca1_callback_master_error( MD_SPT );
    }
    /* Control for sended address */
    else
    {
        if( ( i2ca1_mstatus & _80_IICA_ADDRESS_COMPLETE ) == 0U )
        {
            if( 1U == ACKD1 )
            {
                i2ca1_mstatus |= _80_IICA_ADDRESS_COMPLETE;

                if( 1U == TRC1 )
                {
                    WTIM1 = 1U;

                    if( i2ca1_tx_cnt > 0U )
                    {
                        IICA1 = *i2ca1_tx_pbuf;
                        i2ca1_tx_pbuf++;
                        i2ca1_tx_cnt--;
                    }
                    else
                    {
                        i2ca1_callback_master_sendend();
                    }
                }
                else
                {
                    ACKE1 = 1U;
                    WTIM1 = 0U;
                    WREL1 = 1U;
                }
            }
            else
            {
                i2ca1_callback_master_error( MD_NACK );
            }
        }
        else
        {
            /* Master send control */
            if( 1U == TRC1 )
            {
                if( ( 0U == ACKD1 ) && ( i2ca1_tx_cnt != 0U ) )
                {
                    i2ca1_callback_master_error( MD_NACK );
                }
                else
                {
                    if( i2ca1_tx_cnt > 0U )
                    {
                        IICA1 = *i2ca1_tx_pbuf;
                        i2ca1_tx_pbuf++;
                        i2ca1_tx_cnt--;
                    }
                    else
                    {
                        i2ca1_callback_master_sendend();
                    }
                }
            }
            /* Master receive control */
            else
            {
                if( i2ca1_rx_cnt < i2ca1_rx_len )
                {
                    *i2ca1_rx_pbuf = IICA1;
                    i2ca1_rx_pbuf++;
                    i2ca1_rx_cnt++;

                    if( i2ca1_rx_cnt == i2ca1_rx_len )
                    {
                        ACKE1 = 0U;
                        WREL1 = 1U;
                        WTIM1 = 1U;
                    }
                    else
                    {
                        WREL1 = 1U;
                    }
                }
                else
                {
                    i2ca1_callback_master_receiveend();
                }
            }
        }
    }
}

/**
 * @brief	: Callback function when master error occurs.
 * @param	: Error flag.
 * @retval : None.
 */
static void i2ca1_callback_master_error( MD_STATUS flag )
{

    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/**
 * @brief	: Callback function when finishes master reception.
 * @param	: None.
 * @retval : None.
 */
static void i2ca1_callback_master_receiveend( void )
{
    SPT1 = 1U;

    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/**
 * @brief	: Callback function when finishes master transmission.
 * @param	: None.
 * @retval : None.
 */
static void i2ca1_callback_master_sendend( void )
{
    SPT1 = 1U;

    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

__interrupt static void r_i2ca1_interrupt(void)
{
    if ((IICS1 & _80_IICA_STATUS_MASTER) == 0x80U)
    {
        i2ca1_master_handler();
    }
}
