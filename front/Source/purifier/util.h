/**
 * File : util.h
 * 
 * Util Api
*/
#ifndef __UTIL_H__
#define __UTIL_H__

#include "prj_type.h"

/**
 * Macro
**/
#define UNUSED(X)                   ((void)(x))

#define GET_16_HIGH_BYTE(val)       (U8)(((val)&0xFF00) >> 8)
#define GET_16_LOW_BYTE(val)        (U8)((val)&0x00FF)
#define GET_16_UINT_WORD(lb, hb)    ((((U16)hb)<<8)|lb)

#define GET_32_BYTE_8(val)          (U8)(((val)&0x000000FF))
#define GET_32_BYTE_16(val)         (U8)(((val)&0x0000FF00)>>8)
#define GET_32_BYTE_24(val)         (U8)(((val)&0x00FF0000)>>16)
#define GET_32_BYTE_32(val)         (U8)(((val)&0xFF000000)>>24)
#define GET_32_UINT_WORD(b32, b24, b16, b8) \
    (\
        ( ((U32)b32)<<24 ) |\
        ( ((U32)b24)<<16 ) |\
        ( ((U32)b16)<<8 ) |\
        ( (U32)b8) \
    )

#define SET_BIT_BYTE(val,bit)     do{ val |= (U8)(bit); }while(0)
#define CLEAR_BIT_BYTE(val,bit)   do{ val &= (U8)(~bit); }while(0

#define SET_BIT_WORD(val,bit)     do{ val |= (U16)(bit); }while(0)
#define CLEAR_BIT_WORD(val,bit)   do{ val &= (U16)(~bit); }while(0)

#define IS_SET_BIT_32(val,bit)    ((( val & (bit) )==bit)?1:0)

#define DOWN_COUNT(val) \
    do{ \
        if( val != 0 ) { val--; } \
    }while(0)

#define DOWN_COUNT_RELOAD(val, reload) \
    do{ \
        if( val != 0 ){ val--;}\
        else{ val=reload;} \
    }while(0)

U8 Hex2Dec(U8 hex);
U8 Dec2Hex(U8 dec);
U8 Conv_Asc2Byte(U8 upper, U8 lower);

void Delay_Us(U8 us);
void Delay_MS(U16 ms);

U16 Conv_ByteOrder(U16 src);

void Reset(void);

I16 GetMin(I16 dataX, I16 dataY);
I16	GetMax(I16 dataX, I16 dataY);
U8 CheckLimitValue(U8 val, U8 min, U8 max, U8 init);
U8 SetMinMax8(U8 val, U8 min, U8 max);
U16 SetMinMax16(U16 val, U16 min, U16 max);
I16 SetMinMaxI16(I16 val, I16 min, I16 max);
U32 SetMinMax32(U32 val, U32 min, U32 max);
F32 SetMinMaxF32(F32 val, F32 min, F32 max);

void SetBit(U8 *bitData, U8 pos);
void ClearBit(U8 *bitData, U8 pos);
U8 GetBit(U8 *bitData, U8 pos);

#endif  /* __UTIL_H__ */
