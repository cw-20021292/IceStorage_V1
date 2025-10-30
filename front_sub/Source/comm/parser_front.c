/***********************************************************************************************
 * INCLUDE 
 */
#include <stdio.h>
#include <stdlib.h>

#include "parser_front.h"

#include "util.h"
#include "timer.h"
#include "purifier.h"
#include "config.h"
#include "led.h"

/***********************************************************************************************
 * DEFINITION 
 */
#define STX                 (0xAA)
#define ETX                 (0x55)

#define PKT_REQ_FRONT       (0x02)

#define PKT_ACK_FRONT       (0x12)

#define MIN_PKT_SZ          (5)

static U16 Rx_CRC_CCITT(U8 *puchMsg, U16 usDataLen)
{
    U8 i = 0;
    U16 wCRCin = 0x0000;
    U16 wCPoly = 0x1021;
    U8 wChar = 0;

    while(usDataLen--)
    {
        wChar = *(puchMsg++);
        wCRCin ^= ((U16)wChar << 8);
        for(i = 0; i < 8; i++)
        {
            if (wCRCin & 0x8000)
            {
                wCRCin = (wCRCin << 1) ^ wCPoly;
            }
            else
            {
                wCRCin = wCRCin << 1;
            }
        }
    }

    return (wCRCin);
}

I16 Crc16Main(U8 *buf, I16 len)
{
    U16 checksum = 0;

    if( len < MIN_PKT_SZ )
    {
        return 0; // error..
    }

    checksum = Rx_CRC_CCITT(buf, (U16)(len - 3));
    buf[ len - 3 ] = GET_16_HIGH_BYTE(checksum);
    buf[ len - 2 ] = GET_16_LOW_BYTE(checksum);

    return len;
}

/*****************************************************************************/
// STX(1) + PKT_ACK_MAIN(1) + LED(69) +  CRC(2) + EXT(1)
// SIZE : 74bytes
/**
 * @brief Master로부터 전달받은 패킷에 대한 처리 및 응답 처리
 * 
 */
static I16 ParserReqFront(U8 *buf)
{
    U8 i = 0U;
    //U16 val_16 = 0U;
    //U32 val_32 = 0U;

    /* MAIN한테 온 ON/OFF 데이터를  */
    for( i=0; i< LED_ID_MAX; i++ )
    {
        OnOffLed((ELedId_T)i, buf[i]);
    }
    
    // Send ACK 
    SetCommHeader(COMM_ID_FRONT, PKT_ACK_FRONT);
    StartTimer(TIMER_USER, TIMER_USER_ID_COMM_FRONT_TX, 1UL);
    return TRUE;
}

static U8 Check_Crc(U8 *buf, I16 len)
{
    U16 crc16 = 0;

    crc16 = ( ( (U16)buf[ len - 3 ] ) << 8 ) & 0xFF00;
    crc16 |=    (U16)( buf[ len - 2 ] );

    if( crc16 != Rx_CRC_CCITT( buf, (U16)( len - 3 ) ) )
    {
        return FALSE;
    }

    return TRUE;
}

I16 IsValidPktFront(U8 *buf, I16 len)
{
    if( buf == NULL )
    {
        return FALSE;
    }

    if( len < MIN_PKT_SZ )
    {
        return FALSE;
    }

    if( Check_Crc( buf, len ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

typedef I16 (*FPAction_t)(U8 *buf);
typedef struct _parser_list_
{
    U8 type;
    FPAction_t pParserPkt;
} SParserList_T;
const static SParserList_T parserList[] = 
{
    { PKT_REQ_FRONT, ParserReqFront },
};

#define SZ_PARSER_LIST ( sizeof(parserList) / sizeof(SParserList_T))

I16 ParserPktFront(U8 *buf, I16 len)
{
    U8 type = 0U;
    FPAction_t pParser = NULL;
    U8 i = 0U;

    for( i=0; i < SZ_PARSER_LIST; i++ )
    {
        type = parserList[i].type;
        pParser = parserList[i].pParserPkt;

        if( type == buf[1] )
        {
            if( pParser != NULL )
            {
                len = pParser(&buf[2]);
            }
            break;
        }
    }

    return len;
}

typedef struct _make_list_
{
    U8 type;                // WHO?
    FPAction_t makePkt;     // HOW?
} SMakeList_T;

static I16 MakePktAckFront(U8 *buf);
const static SMakeList_T makeList[] = 
{
    { PKT_ACK_FRONT,       MakePktAckFront },
};
#define SZ_MAKE_LIST    ( sizeof(makeList) / sizeof(SMakeList_T) )

I16 MakePktFront(CommHeader_T *comm, U8 *buf)
{
    U8 type = 0U;
    FPAction_t pMakePkt = NULL;
    U8 i = 0U;
    I16	len	= -1;

    for( i=0; i<SZ_MAKE_LIST; i++ )
    {
        type = makeList[i].type;
        pMakePkt = makeList[i].makePkt;

        if( type == (U8)comm )
        {
            if( pMakePkt != NULL )
            {
                len = pMakePkt(buf);
            }
            break;
        }
    }

    return len;
}

// STX(1) + PKT_ACK_MAIN(1) + F_VERSION(4) + KEY(4) + UNUSED1(1) + UNUSED1(1) + CRC(2) + EXT(1)
// SIZE : 15bytes
static I16 MakePktAckFront(U8 *buf)
{
    I16 len = 0;
    ELedId_T LedId = 0;

    // STX 
    buf[len++] = STX;

    // MESSAGE TYPE
    buf[len++] = PKT_ACK_FRONT;

    for (LedId = 0 ; LedId < LED_ID_MAX ; LedId++)
    {
        // 각 정해진 LED별로 Duty값 전송
        buf[len++] = ConvertDuty2Protocol(LedId);
    }

    buf[ len++ ] = 0;   // UNUSED

    // CRC-16
    buf[ len++ ] = 0;
    buf[ len++ ] = 0;

    buf[ len++ ] = ETX;

    return len;
}
