#include "mcu.h"
#include "util.h"
#include "hw.h"

I16 GetMin( I16 x, I16 y )
{
    if( x > y )
    {
        return y;
    }

    return x;
}

I16	GetMax( I16 x, I16 y )
{
    if( x > y )
    {
        return x;
    }

    return y;
}

/* 0 ~ 99 */
U8 Hex2Dec(U8 mu8Hex)
{
    U8 mu8Dec;

    mu8Dec = (mu8Hex & 0x0F);
    mu8Dec += ((mu8Hex & 0xF0) >> 4) * 10;;

    return mu8Dec;
}

/* 0 ~ 99 */
U8 Dec2Hex(U8 mu8Dec)
{
    U8 mu8Hex;

    mu8Hex = mu8Dec % 10;
    mu8Hex += (mu8Dec / 10) * 16;

    return mu8Hex;
}

U8 ConvAsc2Byte(U8 mu8Upper, U8 mu8Lower )
{
    U8 mu8Temp = 0;
    U8 mu8Val = 0;


    // Conv upper byte
    if( mu8Upper > '9' )
    {
        mu8Temp = mu8Upper - '7';
    }
    else 
    {
        mu8Temp = mu8Upper - '0';
    }
    mu8Val = mu8Temp * 16;

    // Conv lower byte
    if( mu8Lower > '9' )
    {
        mu8Temp = mu8Lower - '7';
    }
    else 
    {
        mu8Temp = mu8Lower - '0';
    }
    mu8Val += mu8Temp;


    return mu8Val;
}

void    Delay_US( U8 us )
{
    while( us-- )
    {
        NOP();
        NOP();
        NOP();
    }
}

U16 ByteOrderConv( U16 _src )
{
    U16 h_to_l = _src >> 8;
    U16 l_to_h = ( _src & 0x00FF ) << 8;

    return ( l_to_h | h_to_l );
}

void    Delay_MS( U16 ms )
{
    U16 i;
    U8      j;

    for( i = 0 ; i < ms ; i++ )
    {
        for( j = 0; j < 4 ; j++ )   // 1ms loop delay
        {
            Delay_US( 250 );
            R_WDT_Restart();
        }
    }
}

void Reset(void)
{
    while(1)
    {
        ; // wait.. watchdog reset
    }
}

U8 CheckLimitValue(U8 mu8Val, U8 mu8Min, U8 mu8Max, U8 mu8Init)
{
    if( mu8Val < mu8Min || mu8Val > mu8Max  )
    {
        return mu8Init;
    }

    return mu8Val;
}

I16 SetMinMaxI16(U16 mi16Val, U16 mi16Min, U16 mi16Max )
{
    if( mi16Val <= mi16Min )
    {
        mi16Val = mi16Min;
    }
    else if( mi16Val >= mi16Max )
    {
        mi16Val = mi16Max;
    }

    return mi16Val;
}

U8 SetMinMax8(U8 mVal, U8 mMin, U8 mMax )
{
    if( mVal <= mMin )
    {
        mVal = mMin;
    }
    else if( mVal >= mMax )
    {
        mVal = mMax;
    }

    return mVal;
}

U16 SetMinMax16(U16 mu16Val, U16 mu16Min, U16 mu16Max )
{
    if( mu16Val <= mu16Min )
    {
        mu16Val = mu16Min;
    }
    else if( mu16Val >= mu16Max )
    {
        mu16Val = mu16Max;
    }

    return mu16Val;
}

U32 SetMinMax32(U32 mu32Val, U32 mu32Min, U32 mu32Max )
{
    if( mu32Val <= mu32Min )
    {
        mu32Val = mu32Min;
    }
    else if( mu32Val >= mu32Max )
    {
        mu32Val = mu32Max;
    }

    return mu32Val;
}

F32 SetMinMaxF32(F32 mf32Val, F32 mf32Min, F32 mf32Max )
{
    if( mf32Val <= mf32Min )
    {
        mf32Val = mf32Min;
    }
    else if( mf32Val >= mf32Max )
    {
        mf32Val = mf32Max;
    }

    return mf32Val;
}
