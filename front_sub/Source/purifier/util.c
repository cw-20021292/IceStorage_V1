/**
 * File : util.c
 * 
 * Util Function
**/
#include "mcu.h"
#include "util.h"
#include "hw.h"

/**
 * @brief            Function For Change Hex to Decimal
 * @detail           none
 * @param     hex    Hex Data
 * @return           Decimal Value
**/
U8 Hex2Dec(U8 hex)
{
    U8 dec;

    dec= (hex & 0x0F);
    dec += ((hex & 0xF0) >> 4) * 10;;

    return dec;
}

/**
 * @brief              Function For Change Decimal to Hex
 * @detail             none
 * @param     dec    Decimal Data
 * @return             Hex Value
**/
U8 Dec2Hex(U8 dec)
{
    U8 hex = 0U;

    hex = dec % 10;
    hex += (dec / 10) * 16;

    return hex;
}

/**
 * @brief              Function For Change Ascil to Byte
 * @detail             none
 * @param     upper    Ascil Upper Data
 * @param     lower    Ascil Lower Data
 * @return             Byte Value
**/
U8 Conv_Asc2Byte(U8 upper, U8 lower )
{
    U8 uTemp = 0;
    U8 uVal = 0;

    // Conv upper byte
    if( upper > '9' )
    {
        uTemp = upper - '7';
    }
    else 
    {
        uTemp = upper - '0';
    }
    uVal = uTemp * 16;

    // Conv lower byte
    if( lower > '9' )
    {
        uTemp = lower - '7';
    }
    else 
    {
        uTemp = lower - '0';
    }
    uVal += uTemp;

    return uVal;
}

/**
 * @brief           Function For Delay System
 * @detail          System Shut Down During Time
 * @param     us    Time ( Unit : us )
 * @return          none
**/
void Delay_Us(U8 us)
{
    while( us-- )
    {
        NOP();
        NOP();
        NOP();
    }
}

/**
 * @brief           Function For Delay System
 * @detail          System Shut Down During Time
 * @param     ms    Time ( Unit : ms )
 * @return          none
**/
void Delay_MS(U16 ms)
{
    U16 i;
    U8  j;

    for( i=0U; i<ms; i++ )
    {
        for( j=0U; j<4U; j++ )   // 1ms loop delay
        {
            Delay_Us(250U);
            R_WDT_Restart();
        }
    }
}


/**
 * @brief           Function For ByteOrder
 * @detail          none
 * @param     src   Source Data
 * @return          Changed Data
**/
U16 Conv_ByteOrder(U16 src)
{
    U16 h_to_l = src >> 8;
    U16 l_to_h = ( src & 0x00FF ) << 8;

    return ( l_to_h | h_to_l );
}

/**
 * @brief        Function For Reset System
 * @detail       System Reset using WatchDog Reset
 * @param        none
 * @return       none
**/
void Reset(void)
{
    while(1)
    {
        ; // wait.. watchdog reset
    }
}

/**
 * @brief           Function For Limit Value
 * @detail          none
 * @param     val   none
 * @param     min   none
 * @param     max   none
 * @param     init  none
 * @return          Original Value or Inital Value
**/
U8 CheckLimitValue(U8 val, U8 min, U8 max, U8 init)
{
    if( (val < min) || (val > max) )
    {
        return init;
    }

    return val;
}

/**
 * @brief             Function For Get Min Value
 * @detail            none
 * @param     dataX   Data1
 * @param     dataY   Data2
 * @return            Min Value
**/
I16 GetMin(I16 dataX, I16 dataY)
{
    if( dataX > dataY )
    {
        return dataY;
    }

    return dataX;
}

/**
 * @brief             Function For Get Max Value 
 * @detail            none
 * @param     dataX   Data1
 * @param     dataY   Data2
 * @return            Max Value
**/
I16	GetMax(I16 dataX, I16 dataY)
{
    if( dataX > dataY )
    {
        return dataX;
    }

    return dataY;
}

/**
 * @brief           Function For 
 * @detail          none
 * @param     val   none
 * @param     min   none
 * @param     max   none
 * @return          Value
**/
U8 SetMinMax8(U8 val, U8 min, U8 max )
{
    if( val <= min )
    {
        val = min;
    }
    else if( val >= max )
    {
        val = max;
    }

    return val;
}

/**
 * @brief           Function For 
 * @detail          none
 * @param     val   none
 * @param     min   none
 * @param     max   none
 * @return          Value
**/
U16 SetMinMax16(U16 val, U16 min, U16 max)
{
    if( val <= min )
    {
        val = min;
    }
    else if( val >= max )
    {
        val = max;
    }

    return val;
}

/**
 * @brief           Function For 
 * @detail          none
 * @param     val   none
 * @param     min   none
 * @param     max   none
 * @return          Value
**/
I16 SetMinMaxI16(I16 val, I16 min, I16 max)
{
    if( val <= min )
    {
        val = min;
    }
    else if( val >= max )
    {
        val = max;
    }

    return val;
}

/**
 * @brief           Function For 
 * @detail          none
 * @param     val   none
 * @param     min   none
 * @param     max   none
 * @return          Value
**/
U32 SetMinMax32(U32 val, U32 min, U32 max)
{
    if( val <= min )
    {
        val = min;
    }
    else if( val >= max )
    {
        val = max;
    }

    return val;
}

/**
 * @brief           Function For 
 * @detail          none
 * @param     val   none
 * @param     min   none
 * @param     max   none
 * @return          Value
**/
F32 SetMinMaxF32(F32 val, F32 min, F32 max)
{
    if( val <= min )
    {
        val = min;
    }
    else if( val >= max )
    {
        val = max;
    }

    return val;
}

void SetBit(U8 *bitData, U8 pos)
{
    *bitData |= ((U8)(1U)<<pos); 
}

void ClearBit(U8 *bitData, U8 pos)
{
    *bitData &= ~((U8)(1U)<<pos); 
}

U8 GetBit(U8 *bitData, U8 pos)
{
    U8 data = 0U;

    data = *bitData & ((U8)(1U)<<pos);
    data = data >> pos;

    return data;
}
