
/* Includes --------------------------------------------------------------------*/
#include "hw.h"
#include "r_cg_serial.h"
#include "hal_i2c.h"
#include "hal_eeprom.h"
#include "util.h"

/* Definitions -----------------------------------------------------------------*/
#define EEP_OP_WR 0x00
#define EEP_OP_RD 0x01

#define DEFAULT_DELAY 35 /* 20ms + 15 */  /* ISL12026 Write Cycle = MAX 20ms */
#define DEFAULT_RETRY_COUNT 3
/* Variables -------------------------------------------------------------------*/
typedef struct _rtc_isl12026_
{
    /* READ/WRITE STATUS */
    U8 WriteProc; 
    U8 ReadProc;

    U32 ReadFail;
    U32 ReadOk;

    U32 WriteFail;
    U32 WriteOk;
} RTC_T;

RTC_T rtc;


/* Functions -------------------------------------------------------------------*/

/**
 * @brief	: EEP processing status setting.
 * @param	: U8 EEP processing start or done.
 * @retval : None.
 */

void HAL_RTC_Init(void)
{
    MEMSET( (void __FAR *)&rtc, 0, sizeof( RTC_T ));
}

/**
 * @brief	: EEP processing status setting.
 * @param	: U8 EEP processing start or done.
 * @retval : None.
 */
void	HAL_RTC_SetWriteProc( U8 _proc )
{
    rtc.WriteProc = _proc;
}

/**
 * @brief	: EEP processing status read.
 * @param	: None.
 * @retval : U8 EEP processing start or done.
 */
U8	HAL_RTC_GetWriteProc( void )
{
    return rtc.WriteProc;
}

/**
 * @brief	: EEPROM processing status check.
 * @param	: None.
 * @retval : True or False.
 */
static BOOL_T	HAL_RTC_WriteProcComplete( void )
{
    U16	wait = 10000;

    do
    {
        if( wait > 0 )
            wait--;
        else
            return FALSE;
    }
    while( !( HAL_RTC_GetWriteProc() == EEP_PROC_DONE ) );

    return TRUE;
}

/**
 * @brief	: EEP processing status setting.
 * @param	: U8 EEP processing start or done.
 * @retval : None.
 */
void	HAL_RTC_SetReadProc( U8 _proc )
{
    rtc.ReadProc = _proc;
}

/**
 * @brief	: EEP processing status read.
 * @param	: None.
 * @retval : U8 EEP processing start or done.
 */
U8	HAL_RTC_GetReadProc( void )
{
    return rtc.ReadProc;
}

/**
 * @brief	: EEPROM processing status check.
 * @param	: None.
 * @retval : True or False.
 */
static BOOL_T	HAL_RTC_ReadProcComplete( void )
{
    U16	wait = 10000;

    do
    {
        if( wait > 0 )
            wait--;
        else
            return FALSE;
    }
    while( !( HAL_RTC_GetReadProc() == EEP_PROC_DONE ) );

    return TRUE;
}

static void CountReadError(void)
{
    rtc.ReadFail++;
}

static void CountWriteError(void)
{
    rtc.WriteFail++;
}

static void i2ca0_StopCondition(void)
{
    R_IICA0_StopCondition();
    //Delay_MS( DEFAULT_DELAY );
}

/**
 * @brief	: Writing a single byte.
 * @param	: U8 memory address, U8 data.
 * @retval : True or False.
 */
static BOOL_T	ByteWrite( U8 _dev, U16 _addr , U8 _data )
{
    U8 buf[ 3 ];

    buf[ 0 ] = GET_HIGH_BYTE(_addr);
    buf[ 1 ] = GET_LOW_BYTE(_addr);
    buf[ 2 ] = _data;


    HAL_RTC_SetWriteProc( EEP_PROC_START );
    if( i2ca0_master_send( _dev | EEP_OP_WR , buf , 3 , 200 ) != MD_OK )
    {
        CountWriteError();

        i2ca0_StopCondition();
        return FALSE;
    }

    if( !HAL_RTC_WriteProcComplete() )
    {
        CountWriteError();
        i2ca0_StopCondition();
        return FALSE;
    }

    i2ca0_StopCondition();
    Delay_MS( DEFAULT_DELAY );
    //Delay_MS(100);

    rtc.WriteOk++;
    return TRUE;
}

BOOL_T	HAL_RTC_ByteWrite( U8 _dev, U16 _addr , U8 _data )
{
    U8 mu8RetryCount = DEFAULT_RETRY_COUNT;


    // Write, ���н� ��õ�
    do
    {
        if( mu8RetryCount == 0 )
        {
            return FALSE;
        }
        mu8RetryCount--;

        RESET_WDT();

    }
    while( ByteWrite( _dev, _addr, _data ) != TRUE );

    RESET_WDT();
    return TRUE;
}


/**
 * @brief	: Writing as the page size.
 * @param	: U8 memory address, U8 data.
 * @retval : True or False.
 */
static BOOL_T	PageWrite( U8 _dev, U16 _addr , U8 *_data, U8 _len )
{
    U8	buf [ EEP_PAGE_SIZE + 2 ];
    U8	rbuf[ EEP_PAGE_SIZE     ];

    buf[ 0 ] = GET_HIGH_BYTE(_addr);
    buf[ 1 ] = GET_LOW_BYTE(_addr);

    MEMCPY( (void __FAR *)&buf[ 2 ], (const void __FAR *)&_data[ 0 ], _len );

    HAL_RTC_SetWriteProc( EEP_PROC_START );

    // Write Page..
    if( i2ca0_master_send( _dev | EEP_OP_WR , &buf[ 0 ] , _len + 2 , 200 ) != MD_OK )
    {
        CountWriteError();
        i2ca0_StopCondition();
        return FALSE;
    }

    if( !HAL_RTC_WriteProcComplete() )
    {
        CountWriteError();
        i2ca0_StopCondition();
        return FALSE;
    }

    i2ca0_StopCondition();
    Delay_MS( DEFAULT_DELAY );
    //Delay_MS( DEFAULT_DELAY );

    // Read.. page
    if( !HAL_RTC_SeqRead( _dev, _addr, &rbuf[ 0 ], _len ) )
    {
        CountWriteError();
        i2ca0_StopCondition();
        return FALSE;
    }

    // Check data..
    if( memcmp( &buf[ 2 ], &rbuf[ 0 ], _len ) != 0 )
    {
        CountWriteError();
        i2ca0_StopCondition();
        return FALSE;
    }

    i2ca0_StopCondition();
    Delay_MS( DEFAULT_DELAY );
    //Delay_MS( DEFAULT_DELAY );
    rtc.WriteOk++;
    return TRUE;
}

BOOL_T	HAL_RTC_PageWrite( U8 _dev, U16 _addr , U8 *_data, U8 _len )
{
    U8 mu8RetryCount = DEFAULT_RETRY_COUNT;


    do
    {
        if( mu8RetryCount == 0 )
        {
            return FALSE;
        }
        mu8RetryCount--;

        RESET_WDT();

    }
    while( PageWrite( _dev, _addr, _data, _len ) != TRUE );

    RESET_WDT();
    return TRUE;
}



/**
 * @brief	: Read the data from the current address.
 * @param	: U8 data.
 * @retval : True or False.
 */
static BOOL_T	CurrentAddrRead( U8 _dev, U8 *_data )
{
    HAL_RTC_SetReadProc( EEP_PROC_START );

    if( i2ca0_master_recv( _dev | EEP_OP_RD , _data , 1 , 10 ) != MD_OK )
    {
        CountReadError();
        i2ca0_StopCondition();
        return FALSE;
    }

    if( !HAL_RTC_ReadProcComplete() )
    {
        CountReadError();
        i2ca0_StopCondition();
        return FALSE;
    }

    i2ca0_StopCondition();
    Delay_MS( DEFAULT_DELAY );
    rtc.ReadOk++;
    return TRUE;
}

BOOL_T	HAL_RTC_CurrentAddrRead( U8 _dev, U8 *_data )
{
    U8 mu8RetryCount = DEFAULT_RETRY_COUNT;



    do
    {
        if( mu8RetryCount == 0 )
        {
            return FALSE;
        }
        mu8RetryCount--;

        RESET_WDT();

    }
    while( CurrentAddrRead( _dev, _data ) != TRUE );

    RESET_WDT();
    return TRUE;
}
/**
 * @brief	: Read one byte.
 * @param	: U8 memory address, U8 * read data.
 * @retval : True or False.
 */
static BOOL_T	ByteRead( U8 _dev, U16 _addr , U8 *_data )
{
    U8 buf[2] = {0};


    buf[0] = GET_HIGH_BYTE( _addr );
    buf[1] = GET_LOW_BYTE( _addr );

    HAL_RTC_SetWriteProc( EEP_PROC_START );
    // Write address..
    if( i2ca0_master_send( _dev | EEP_OP_WR , buf , 2 , 200 ) != MD_OK )
    {
        CountReadError();
        i2ca0_StopCondition();
        return FALSE;
    }

    if( !HAL_RTC_WriteProcComplete() )
    {
        CountReadError();
        i2ca0_StopCondition();
        return FALSE;
    }

    HAL_RTC_SetReadProc( EEP_PROC_START );

    // Read data..
    if( i2ca0_master_recv( _dev | EEP_OP_RD , _data , 1 , 200 ) != MD_OK )
    {
        CountReadError();
        i2ca0_StopCondition();
        return FALSE;
    }


    if( !HAL_RTC_ReadProcComplete() )
    {
        CountReadError();
        i2ca0_StopCondition();
        return FALSE;
    }

    i2ca0_StopCondition();
    Delay_MS( DEFAULT_DELAY );
    rtc.ReadOk++;
    return TRUE;
}

BOOL_T	HAL_RTC_ByteRead( U8 _dev, U16 _addr , U8 *_data )
{
    U8 mu8RetryCount = DEFAULT_RETRY_COUNT;


    do
    {
        if( mu8RetryCount == 0 )
        {
            return FALSE;
        }
        mu8RetryCount--;

        RESET_WDT();

    }
    while ( ByteRead( _dev, _addr , _data ) != TRUE );

    RESET_WDT();
    return TRUE;
}

/**
 * @brief	: Read multiple bytes.
 * @param	: U8 memory address, U8 * read data, U8 read size.
 * @retval : True or False.
 */
static BOOL_T	SeqRead( U8 _dev, U16 _addr , U8 * _data, U8 _len )
{
    U8 buf[2] = {0};


    buf[0] = GET_HIGH_BYTE( _addr );
    buf[1] = GET_LOW_BYTE( _addr );
    HAL_RTC_SetWriteProc( EEP_PROC_START );

    /* Send  Address */
    if( i2ca0_master_send( _dev | EEP_OP_WR , buf , 2 , 200 ) != MD_OK )
    {
        CountReadError();
        i2ca0_StopCondition();
        return FALSE;
    }

    /* Wait */
    if( !HAL_RTC_WriteProcComplete() )
    {
        CountReadError();
        i2ca0_StopCondition();
        return FALSE;
    }

    HAL_RTC_SetReadProc( EEP_PROC_START );

    /* Recv Data */
    if( i2ca0_master_recv( _dev | EEP_OP_RD , _data , _len , 200 ) != MD_OK )
    {
        CountReadError();
        i2ca0_StopCondition();
        return FALSE;
    }

    if( !HAL_RTC_ReadProcComplete() )
    {
        CountReadError();
        i2ca0_StopCondition();
        return FALSE;
    }


    i2ca0_StopCondition();
    Delay_MS( DEFAULT_DELAY );
    rtc.ReadOk++;
    return TRUE;
}

BOOL_T	HAL_RTC_SeqRead( U8 _dev, U16 _addr , U8 * _data, U8 _len )
{
    U8 mu8RetryCount = DEFAULT_RETRY_COUNT;


    do
    {
        if( mu8RetryCount == 0 )
        {
            return FALSE;
        }
        mu8RetryCount--;

        RESET_WDT();

    }
    while( SeqRead( _dev, _addr , _data, _len ) != TRUE );

    RESET_WDT();
    return TRUE;
}

/**
 * @brief	: EEPROM all erase.
 * @param	: None.
 * @retval : True or False.
 */
BOOL_T	HAL_RTC_EEPROM_AllErase( void )
{
    U8	buf[ EEP_PAGE_SIZE + 2 ];
    U16	i;
    U16  addr;

    MEMSET((void __FAR *) &buf[ 2 ], 0x00, EEP_PAGE_SIZE );

    for( i = 0; i < EEP_PAGE_NUM; i++ )
    {
        addr = (i * EEP_PAGE_SIZE);
        buf[ 0 ] = GET_HIGH_BYTE( addr );
        buf[ 1 ] = GET_LOW_BYTE( addr );

        HAL_RTC_SetWriteProc( EEP_PROC_START );

        if( i2ca0_master_send( DEV_ADDR_EEP | EEP_OP_WR , &buf[ 0 ] , EEP_PAGE_SIZE + 2 , 200 ) != MD_OK )
        {
            CountWriteError();
            i2ca0_StopCondition();
            return FALSE;
        }

        if( !HAL_RTC_WriteProcComplete() )
        {
            CountWriteError();
            i2ca0_StopCondition();
            return FALSE;
        }

        i2ca0_StopCondition();
        Delay_MS( DEFAULT_DELAY );
    }

    i2ca0_StopCondition();
    Delay_MS( DEFAULT_DELAY );
    rtc.WriteOk++;
    return TRUE;
}

