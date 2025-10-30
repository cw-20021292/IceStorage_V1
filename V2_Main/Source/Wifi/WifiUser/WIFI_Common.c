
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"


/***** Common Function ********************************************************/
/// @brief    HEX to Decimal
/// @param    mu8HexData : HEX Data
/// @return   mu8ReturnData : DEC Data
U8 WIFI_HEX2DEC ( U8 mu8HexData )
{
    U8 mu8ReturnData = 0;
    mu8ReturnData = (U8)((((mu8HexData >> 4) & 0x0F) * 10) + (mu8HexData & 0x0F));
    return  mu8ReturnData;
}

/// @brief    Decimal to HEX
/// @param    mu8DecData : DEC Data
/// @return   mu8ReturnData : HEX Data
U8 WIFI_DEC2HEX ( U8 mu8DecData )
{
    U8 mu8ReturnData = 0;
    mu8ReturnData = (U8)((((mu8DecData / 10) << 4) & 0xF0) + (mu8DecData % 10));
    return  mu8ReturnData;
}

/// @brief    HEX to Decimal
/// @param    mu16HexData : HEX Data
/// @return   mu16ReturnData : DEC Data
U16 WIFI_Bit16HEX2DEC ( U16 mu16HexData )
{
    U16 mu16ReturnData = 0;
    mu16ReturnData = ((mu16HexData >> 12) & 0x000F) * 1000;
    mu16ReturnData += ((mu16HexData >> 8) & 0x000F) * 100;
    mu16ReturnData += ((mu16HexData >> 4) & 0x000F) * 10;
    mu16ReturnData += mu16HexData & 0x000F;
    return  mu16ReturnData;
}

/// @brief    Decimal to HEX
/// @param    mu16DecData : DEC Data
/// @return   mu16ReturnData : HEX Data
U16 WIFI_DEC2Bit16HEX ( U16 mu16DecData )
{
    U16 mu16ReturnData = 0;
    mu16ReturnData = ((mu16DecData / 1000) << 12) & 0xF000;
    mu16ReturnData += (((mu16DecData % 1000) / 100) << 8) & 0x0F00;
    mu16ReturnData += (((mu16DecData % 100) / 10) << 4) & 0x00F0;
    mu16ReturnData += (mu16DecData % 10) & 0x000F;
    return  mu16ReturnData;
}

/// @brief    Ascii to HEX
/// @param    mu8HexData : Hex Data
/// @return   mu8ReturnData : Ascii Data
U8 WIFI_HEX2ASCII ( U8 mu8HexData )
{
    U8 mu8ReturnData = 0;
    if (mu8HexData < 0x0A)
    {
        mu8ReturnData = (U8)(mu8HexData | 0x30);
    }
    else
    {
        mu8ReturnData = (U8)((mu8HexData - 9) | 0x40);
    }
    return  mu8ReturnData;
}

/// @brief    Ascii to HEX
/// @param    mu8AsciiData : Ascii Data
/// @return   mu8ReturnData : Hex Data
U8 WIFI_ASCII2HEX ( U8 mu8AsciiData )
{
    U8 mu8ReturnData = 0;
    if (mu8AsciiData >= 0x41)
    {
        mu8ReturnData = (U8)((mu8AsciiData & ~0x40) + 9);
    }
    else
    {
        mu8ReturnData = (U8)(mu8AsciiData & ~0x30);
    }  
    return  mu8ReturnData;
}

/***** Far Function ***********************************************************/
U16 far_sprintf_s ( I8* pBuf, U16 mLen, I8* pData )
{
    U16 i = 0;
    U16 mu16Size = 0;
    U16 mu16Return = 0;
    mu16Size = _STRLEN_( (const char __FAR*) pData );
    for ( i = 0; i < mu16Size; i++ )
    {
        if ( pData[i] == '\0' )
        {
            break;
        }
        pBuf[i] = pData[i];
        mu16Return++;
    }
    return mu16Return;
	/*U16 mu16Return = 0;
	mu16Return = sprintf_f( (char __far*)&pBuf[mLen], (const char __far*)"%s", (char __far*)pData );
	return mu16Return;*/
}

