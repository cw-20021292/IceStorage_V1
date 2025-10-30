/***********************************************************************************************
 * INCLUDE 
 */
#include <stdio.h>
#include <stdlib.h>

#include "parser_main.h"

#include "api_key.h"

#include "util.h"
#include "timer.h"
#include "purifier.h"
#include "config.h"
#include "led.h"
#include "voice.h"

/***********************************************************************************************
 * DEFINITION 
 */
#define STX                 (0xAA)
#define ETX                 (0x55)

#define MIN_PKT_SZ          (5)

U16 Rx_CRC_CCITT(U8 *puchMsg, U16 usDataLen)
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

I16 IsValidPktMain(U8 *buf, I16 len)
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



static I16 ParserReqMain(U8 *buf);

typedef I16 (*FPAction_t)(U8 *buf);
typedef struct _parser_list_
{
    U8 type;
    FPAction_t pParserPkt;
} SParserList_T;
const static SParserList_T parserList[] = 
{
    { PKT_REQ_MAIN,     ParserReqMain },
};

#define SZ_PARSER_LIST ( sizeof(parserList) / sizeof(SParserList_T))

I16 ParserPktMain(U8 *buf, I16 len)
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

// STX(1) + PKT_ACK_MAIN(1) + LED(69) +  CRC(2) + EXT(1)
// SIZE : 74bytes
/**
 * @brief MAIN으로부터 온 정상 데이터 처리
 * 
 * @param buf : RX DATA
 * @return I16 
 */
static I16 ParserReqMain(U8 *buf)
{
    U32 mu32Val;
    VoiceId_T VoiceId;
    U8 i = 0U;

    /* LED 정보 반영 */
    for( i=0; i< LED_ID_MAX; i++ )
    {
        OnOffLed((ELedId_T)i, buf[i]);
    }

    // Voice ID
    VoiceId = (VoiceId_T)buf[123];
    if( VoiceId != ID_NONE )
    {
        PlayVoice( VoiceId );
    }

    // Voice volume
    SetVoiceVolume( buf[124] );
    
    // Send ACK 
    SetCommHeader(COMM_ID_MAIN, PKT_ACK_MAIN);
    StartTimer(TIMER_USER, TIMER_USER_ID_COMM_MAIN_TX, 1UL);
    return TRUE;
}

typedef struct _make_list_
{
    U8 type;
    FPAction_t makePkt;
} SMakeList_T;

static I16 MakePktAckMain(U8 *buf);
const static SMakeList_T makeList[] = 
{
    { PKT_ACK_MAIN,            MakePktAckMain },
};
#define SZ_MAKE_LIST    ( sizeof(makeList) / sizeof(SMakeList_T) )

I16 MakePktMain(CommHeader_T *comm, U8 *buf)
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
static I16 MakePktAckMain(U8 *buf)
{
    I16 len = 0;
    U16 val_16 = 0;
    U32 val_32 = 0;

    // STX 
    buf[len++] = STX;

    // MESSAGE TYPE
    buf[len++] = PKT_ACK_MAIN;

    // VERSION INFO
    ExportVersion(PROGRAM_VERSION);
    buf[len++] = GetFrontVerMajor();
    buf[len++] = GetFrontVerEvent();
    buf[len++] = GetFrontVerPatch();
    buf[len++] = GetFrontVerMiner();
    
    // KEY
    val_32 = GetKey();
    buf[ len++ ] = GET_32_BYTE_32(val_32);
    buf[ len++ ] = GET_32_BYTE_24(val_32);
    buf[ len++ ] = GET_32_BYTE_16(val_32);
    buf[ len++ ] = GET_32_BYTE_8(val_32);

    buf[ len++ ] = 0;   // UNUSED
    buf[ len++ ] = 0;   // UNUSED

    // CRC-16
    buf[ len++ ] = 0;
    buf[ len++ ] = 0;

    buf[ len++ ] = ETX;

    return len;
}
