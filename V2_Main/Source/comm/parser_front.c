/***********************************************************************************************
 * INCLUDE 
 */
#include <stdio.h>
#include <stdlib.h>

#include "parser_front.h"
#include "util.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_adc.h"
#include "hal_uv.h"
#include "power_saving.h"
#include "timer.h"
#include "voice.h"
#include "front.h"
#include "filter.h"
#include "led.h"
#include "purifier.h"
#include "process_display.h"
/***********************************************************************************************
 * DEFINITION 
 */

#define STX         0xAA
#define ETX         0x55


#define MIN_PKT_SZ      4

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

static U8   check_crc( U8 *buf, I16 len )
{
    U16 crc16 = 0;

    //crc16 = ( ( (U16)buf[ len - 1 ] ) << 8 ) & 0xFF00;
    //crc16 |=    ( buf[ len - 2 ] );
    crc16 = GET_UINT_WORD( buf[len-2], buf[len-3] );

    if( crc16 != Rx_CRC_CCITT( buf, (U16)( len - 3 ) ) )
    {
        return FALSE;
    }

    return TRUE;
}

I16 IsValidPkt_Front( U8 *buf, I16 len )
{
    if( buf == NULL )
    {
        return FALSE;
    }

    if( len < MIN_PKT_SZ )
    {
        return FALSE;
    }

    if( check_crc( buf, len ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}


static I16 ParserAckLed(U8 *buf);
static I16 ParserAckVoice(U8 *buf);

typedef I16 (*action_t)( U8 *buf );
typedef struct _parser_list_t
{
    U8 Type;
    action_t pParserPkt;
} parser_list_t;
const static parser_list_t parser_list[] = 
{
    //{ PKT_FRONT_ACK_LED,    ParserAckLed },
    //{ PKT_FRONT_ACK_LED_2,  ParserAckLed },
    { PKT_FRONT_ACK_LED_3,  ParserAckLed },
    { PKT_FRONT_ACK_VOICE,  ParserAckVoice }
};

#define SZ_PS_TABLE ( sizeof( parser_list ) / sizeof( parser_list_t ))

I16 ParserPkt_Front( U8 *buf, I16 len)
{
    U8 mu8Type;
    action_t	pParser;
    U8 i;


    for( i = 0; i < SZ_PS_TABLE; i++ )
    {
        mu8Type = parser_list[ i ].Type;
        pParser = parser_list[ i ].pParserPkt;

        if( mu8Type == buf[1] )
        {
            if( pParser != NULL )
            {
                len = pParser( &buf[2] );
            }
            break;
        }
    }

    return len;
}

I16 Crc16_Front( U8 *buf, I16 len )
{
    U16 mu16Chksum = 0;
    

    if( len < MIN_PKT_SZ )
    {
        return 0; // error..
    }

    mu16Chksum = Rx_CRC_CCITT( buf, (U16)(len - 3));
    buf[ len - 3 ] = GET_HIGH_BYTE(mu16Chksum);
    buf[ len - 2 ] = GET_LOW_BYTE(mu16Chksum);

    return len;
}


// CHPx KEY MAP -> CPx KEY MAP
static U32 MappingChp2Cp(U32 mu32Key)
{
    U32 mu32NewKey = 0UL;

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_1 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_1;
    }

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_2 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_14;
    }

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_3 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_13;
    }

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_4 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_4;
    }

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_5 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_5;
    }

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_6 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_6;
    }

    // CHP?óê?Ñú?äî ?ñº?ùå OFF, CP?óê?Ñú?äî ÎØ∏ÏÇ¨?ö©
    //if( IS_SET_BIT_32( mu32Key, HAL_KEY_7 ) != 0 )
    //{
    //    mu32NewKey |= HAL_KEY_7;
    //}

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_8 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_8;
    }

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_9 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_15;
    }

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_10 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_10;
    }

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_11 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_11;
    }

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_12 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_12;
    }


    if( IS_SET_BIT_32( mu32Key, HAL_KEY_19 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_19;
    }

    if( IS_SET_BIT_32( mu32Key, HAL_KEY_20 ) != 0 )
    {
        mu32NewKey |= HAL_KEY_20;
    }


    return mu32NewKey;
}


// SYSTEM Íµ¨ÏÑ±?óê ?î∞?ùº KEY MASK Í∞íÏùÑ ?ã§Î•¥Í≤å Í∞??†∏Í∞ÑÎã§.
static U32 GetKeyValMask(U32 mu32Key)
{
    if( GetSystem() == SYSTEM_CP )
    {
        mu32Key = MappingChp2Cp( mu32Key );
    }

    return mu32Key;
}



#define HAL_KEY_WATER_OUT   HAL_KEY_19
#define HAL_KEY_ICE_OUT     HAL_KEY_20

// INPUT MASK
#define MK_MODEL            0x01
#define MK_FILTER_COVER     0x02
U32 dbg_the_key_1 = 0UL;
U32 dbg_the_key_2 = 0UL;
/**
 * @brief Front∑Œ∫Œ≈Õ πﬁ¿∫ ∆–≈∂ √≥∏Æ (Front °Ê Main)
 * 
 * @param buf 
 * @return I16 
 */
static I16 ParserAckLed(U8 *buf)
{
    U32 mu32Key;
    U32 mu32Val;
    U16 mu16Val;

    /// Front Version
    SetFrontVerMajor(buf[0]);
    SetFrontVerEvent(buf[1]);
    SetFrontVerPatch(buf[2]);
    SetFrontVerMiner(buf[3]);
    
    // if( (buf[4] & MK_MODEL ) != 0 )
    // {
    //     SetFrontSystem( SYSTEM_CHP );
    // }
    // else
    // {
    //     SetFrontSystem( SYSTEM_CP );
    // }

    // if( (buf[5] & MK_FILTER_COVER ) == 0 )
    // {
    //     SetFilterStatus( ID_FILTER_COVER, FILTER_CLOSE );
    // }
    // else
    // {
    //     SetFilterStatus( ID_FILTER_COVER, FILTER_OPEN );
    // }

    // KEY 
    mu32Val = GET_UINT_WORD_32( buf[4], buf[5], buf[6], buf[7] );
    dbg_the_key_1 = mu32Val;

    mu32Key = GetKeyValMask( mu32Val );
    dbg_the_key_2 = mu32Key;

    mu32Key = (mu32Key & (~(HAL_KEY_WATER_OUT | HAL_KEY_ICE_OUT))); // MASK ICE OUT & WATER OUT KEY
    HAL_SetKeyVal( mu32Key );

    // ICE OUT 
    if( (mu32Val & HAL_KEY_ICE_OUT) != 0 ) 
    {
        HAL_SetLeverIce( 1 );
    }
    else
    {
        HAL_SetLeverIce( 0 );
    }

    // WATER OUT 
    if( (mu32Val & HAL_KEY_WATER_OUT) != 0 ) 
    {
        HAL_SetLeverWater( 1 );
    }
    else
    {
        HAL_SetLeverWater( 0 );
    }


    /* ADC∞™¿ª Frontø°º≠ Main¿∏∑Œ ∫∏≥ª¡‡æﬂ «“ ∂ß ø©±‚ √ﬂ∞° */
    // // ADC - UV WATER
    // mu16Val = GET_UINT_WORD( buf[11], buf[10] );
    // // HAL_SetAdcValue( ANI_UV_WATER_OUT, mu16Val ); 

    // // ADC - UV ICE
    // mu16Val = GET_UINT_WORD( buf[13], buf[12] );
    // // HAL_SetAdcValue( ANI_UV_ICE_DOOR, mu16Val ); 

    // // ADC - UNUSED...
    // mu16Val = GET_UINT_WORD( buf[15], buf[14] );

    // // ADC - UNUSED...
    // mu16Val = GET_UINT_WORD( buf[17], buf[16] );

    return TRUE;
}

static I16 ParserAckVoice(U8 *buf)
{

    return TRUE;
}


typedef struct _make_list_t
{
    U8  Type;
    action_t    MakePkt;
} make_list_t;

static I16 MakePktReqLed3( U8 *buf );
static I16 MakePktReqVoice( U8 *buf );
const static make_list_t make_list[] = 
{
    { PKT_FRONT_REQ_LED_3,  MakePktReqLed3  },
    //{ PKT_FRONT_REQ_VOICE,  MakePktReqVoice }
};
#define SZ_TABLE    ( sizeof( make_list ) / sizeof( make_list_t ))

I16 MakePkt_Front( CommHeader_T *p_comm, U8 *buf )
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


U8 the_mode = 0;
/**
 * @brief Main °Ê Front πÊ«‚ ∆–≈∂
 * 
 * @param buf 
 * @return I16 
 */
static I16 MakePktReqLed3( U8 *buf )
{
    I16 mi16Len = 0;
    U8  mu8Buf[MAX_LED_BUF];
    U8 i;
    U8 mVoiceVol;      
    VoiceId_T   mVoiceId;

    // STX 
    buf[ mi16Len++ ] = STX;

    // MESSAGE TYPE
    buf[ mi16Len++ ] = PKT_FRONT_REQ_LED_3;
    
    /// LEDs (2 ~ 122)
    for( i=0; i < LED_ID_MAX; i++ )
    {
        buf[mi16Len++] = ConvertDuty2Protocol(i);
    }

    // VOICE
    mVoiceId = GetVoicePlayId();
    if( mVoiceId != ID_NONE )
    {
        PlayVoice( ID_NONE );               // Clear Voice Id ( for one-shot )
    }
    buf[ mi16Len++ ] = mVoiceId;            // 123

    if( IsVoiceMaxVol( mVoiceId ) == TRUE )
    {
        mVoiceVol = MAX_VOICE_VOLUME;
    }
    else
    {
        mVoiceVol = GetVoiceVolume();   
    }
    // VOICE VOLUME
    buf[ mi16Len++ ] = mVoiceVol;       // 124

    // CRC-16
    buf[ mi16Len++ ] = 0;               // 125
    buf[ mi16Len++ ] = 0;               // 126

    buf[ mi16Len++ ] = ETX;             // 127
    return mi16Len;
}


#if 0
static I16 MakePktReqVoice( U8 *buf )
{
    I16 mi16Len = 0;


    // STX 
    buf[ mi16Len++ ] = STX;

    // MESSAGE TYPE
    buf[ mi16Len++ ] = PKT_FRONT_REQ_VOICE;
    

    // VOICE
    buf[ mi16Len++ ] = GetVoicePlayId();


    // CRC-16
    buf[ mi16Len++ ] = 0;
    buf[ mi16Len++ ] = 0;

    buf[ mi16Len++ ] = ETX;
    return mi16Len;
}

#endif
