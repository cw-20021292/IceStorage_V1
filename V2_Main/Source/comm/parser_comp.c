/***********************************************************************************************
 * INCLUDE 
 */
#include <stdio.h>
#include <stdlib.h>

#include "parser_comp.h"
#include "util.h"
#include "comp_bldc.h"
#include "err_comp_bldc_comm.h"
#include "timer.h"

/***********************************************************************************************
 * DEFINITION 
 */

#define CHKSUM_MAGIC_CODE       0x55

#define FIELD_ERR_CODE          0
#define FIELD_FEEDBACK_RPS      1
#define FIELD_CHECKSUM          2

#define MIN_RX_PKT_SZ           3
#define MIN_TX_PKT_SZ           2


static U8   check_crc( U8 *buf, I16 len )
{
    U8 mu8Chksum = 0;

    mu8Chksum = (buf[ FIELD_ERR_CODE ] + buf[ FIELD_FEEDBACK_RPS ]) ^ CHKSUM_MAGIC_CODE;
    if( mu8Chksum != buf[ FIELD_CHECKSUM ] )
    {
        return FALSE;
    }

    return TRUE;
}

I16 IsValidPkt_Comp( U8 *buf, I16 len )
{
    if( buf == NULL )
    {
        return FALSE;
    }

    if( len < MIN_RX_PKT_SZ )
    {
        return FALSE;
    }

    if( check_crc( buf, len ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}


static I16 ParserAck(U8 *buf);

typedef I16 (*action_t)( U8 *buf );

I16 ParserPkt_Comp( U8 *buf, I16 len)
{
    ParserAck( &buf[0] );

    StartTimer( TIMER_ID_COMM_COMP_RX_ERR, SEC( ERROR_COMP_COMM_TIMEOUT ));
    ResetCompBldcCommErrorCount();

    SetCompBldcCommTest( TRUE );

    return len;
}

I16 Crc16_Comp( U8 *buf, I16 len )
{
    U16 mu16Chksum = 0;
    

    if( len < MIN_TX_PKT_SZ )
    {
        return 0; // error..
    }

    buf[ len - 1 ] = buf[0] ^ CHKSUM_MAGIC_CODE;

    return len;
}

U8 gDbgCompErr = 0U;
U8 gDbgCompErrType = 0U;
static I16 ParserAck(U8 *buf)
{
    if( gDbgCompErr == TRUE )
    {
        SetCompBldcErrorCode( gDbgCompErrType );
    }
    else
    {  
        SetCompBldcErrorCode( buf[ FIELD_ERR_CODE ] );
    }
#if 1
    SetCompBldcCurrentRps( buf[ FIELD_FEEDBACK_RPS ] );
#else
    SetCompBldcErrorCode( dbg_comp_err );
    SetCompBldcCurrentRps( buf[ FIELD_FEEDBACK_RPS ] );
#endif

    return TRUE;
}


typedef struct _make_list_t
{
    U8  Type;
    action_t    MakePkt;
} make_list_t;

static I16 MakePktReq( U8 *buf );
const static make_list_t make_list[] = 
{
    { PKT_COMP_REQ,             MakePktReq },
};
#define SZ_TABLE    ( sizeof( make_list ) / sizeof( make_list_t ))

I16 MakePkt_Comp( CommHeader_T *p_comm, U8 *buf )
{
    U8 mu8Type;
    action_t	p_make_pkt;
    U8 i;
    I16	len	= -1;


    for( i = 0; i < SZ_TABLE; i++ )
    {
        mu8Type = make_list[ i ].Type;
        p_make_pkt = make_list[ i ].MakePkt;

        if( mu8Type == (U8)p_comm )
        {
            if( p_make_pkt != NULL )
            {
                len = p_make_pkt( buf );
            }
            break;
        }
    }

    return len;
}


static I16 MakePktReq( U8 *buf )
{
    I16 mi16Len = 0;

    buf[ mi16Len++ ] = (U8)GetCompBldcTargetRps();  // TARGET RPS
    buf[ mi16Len++ ] = 0;                           // CHKSUM 

    return mi16Len;
}

