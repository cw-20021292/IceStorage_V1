#include "comm.h"

/***********************************************************************************************
 *  HEADER
 */

CommHeader_T    comm_header[ MAX_COMM_ID ];


CommHeader_T GetCommHeader( U8 id )
{
    // Get address of data model 
    return comm_header[ id ];
}

void    SetCommHeader( U8 id, CommHeader_T val )
{
    // Set address of data model 
    comm_header[ id ] = val;
}



/***********************************************************************************************
 *  SEND PACKET 
 */
I16 CommSendPacket( U8 id,  U8 *send_pkt , I16  len )
{
    U8 i;


    if( HAL_IsFullSendBuffer( id ) != TRUE )
    {
        for( i = 0 ; i < len ; i++ )
        {
            HAL_SetSendBuffer( id, send_pkt[ i ] );
        }

        HAL_SendByte( id );
        return len; /* SENT BUF SIZE */
    }

    return -1;  /* ERROR - THERE IS FULL DATA */
}


/***********************************************************************************************
 *  RECV PACKET 
 */
I8 CommRecvPacket( U8 id , U8 *recv_pkt )
{
    U16  i;
    I16 len;

    if( HAL_IsEmptyRecvBuffer( id ) != TRUE )
    {
        len = HAL_GetRecvLength( id );
        for( i = 0; i < len ; i++ )
        {
            recv_pkt[ i ] = HAL_GetRecvBuffer( id, i );
        }

        return len; /* RECEIVED BUF SIZE */
    }

    return -1;  /* ERROR - THERE IS NO DATA */
}

