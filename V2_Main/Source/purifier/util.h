#ifndef __UTIL_H__
#define __UTIL_H__

#include "prj_type.h"

#define GET_HIGH_BYTE(val)     (U8)(((val)&0xFF00) >> 8)
#define GET_LOW_BYTE(val)      (U8)((val)&0x00FF)
#define GET_UINT_WORD(lb, hb)  ((((U16)hb)<<8)|lb)

#define GET_32_BYTE_8(val)          (U8)(((val)&0x000000FF))
#define GET_32_BYTE_16(val)         (U8)(((val)&0x0000FF00)>>8)
#define GET_32_BYTE_24(val)         (U8)(((val)&0x00FF0000)>>16)
#define GET_32_BYTE_32(val)         (U8)(((val)&0xFF000000)>>24)
#define GET_UINT_WORD_32(b32, b24, b16, b8) \
    (\
       ( ((U32)b32)<<24 ) |\
       ( ((U32)b24)<<16 ) |\
       ( ((U32)b16)<<8 ) |\
       ( (U32)b8) \
     )

#define SET_BIT_BYTE(val,bit)     do{ val |= (U8)(bit); }while(0)
#define CLEAR_BIT_BYTE(val,bit)   do{ val &= (U8)(~bit); }while(0)

#define SET_BIT_WORD(val,bit)     do{ val |= (U16)(bit); }while(0)
#define CLEAR_BIT_WORD(val,bit)   do{ val &= (U16)(~bit); }while(0)

#define IS_SET_BIT_32(val,bit)  ((( val & (bit) )==bit)?1:0)

#define DOWN_COUNT(val) \
    do{ \
        if( val != 0 ) { val--; } \
    }while(0)

#define DOWN_COUNT_RELOAD(val,reload) \
    do{ \
        if( val != 0 ){ val--;}\
        else{ val=reload;} \
    }while(0)


I16 GetMin( I16 x, I16 );
I16 GetMax( I16 x, I16 y );

U8 Hex2Dec(U8 mu8Hex);
U8 Dec2Hex(U8 mu8Dec);

U8 ConvAsc2Byte(U8 mu8Upper, U8 mu8Lower );
U16 ByteOrderConv( U16 _src );

void Delay_US( U8 us );
void Delay_MS( U16 ms );

void Reset(void);

// 값을 리미트 제한 한다.
// min, max를 초과하는 경우 Init Value를 반환한다.
// 그렇지 않은 경우 mu8Val를 return으로 반환한다.
U8 CheckLimitValue(U8 mu8Val, U8 mu8Min, U8 mu8Max, U8 mu8Init);


U8 SetMinMax8(U8 mVal, U8 mMin, U8 mMax );

U16 SetMinMax16(U16 mu16Val, U16 mu16Min, U16 mu16Max );

U32 SetMinMax32(U32 mu32Val, U32 mu32Min, U32 mu32Max );

F32 SetMinMaxF32(F32 mf32Val, F32 mf32Min, F32 mf32Max );

#endif /* __UTIL_H__ */
