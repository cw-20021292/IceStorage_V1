#include "err_temp.h"

/*
 * 온도 센서는 하위, 상위 0.2V 수준으로 온도 센서 에러 검출
 * 에러는 OPEN 에러, SHORT 에러 감지
 *
 */

#define ADC_12BIT_TOP      985   /* 4.8V */
#define ADC_12BIT_BOTTOM   41    /* 0.2V */

U8 GetTempErrorType(U16 mu16Adc)
{
    if( mu16Adc > ADC_12BIT_TOP )
    {
        return STATUS_SHORT;    /* ERROR */
    }
    else if( mu16Adc < ADC_12BIT_BOTTOM )
    {
        return STATUS_OPEN;   /* ERROR */
    }

    return STATUS_NORMAL; /* NORMAL */
}


U8 CheckErrorTemp(U8 mu8Error, U16 mu16Adc, U8 *pBuf)
{
    U8 mu8Index;


    mu8Index = GetTempErrorType( mu16Adc );
    if( mu8Index != pBuf[0] )
    {
        pBuf[0] = mu8Index;

        pBuf[1] = COUNT_NUM;   /* NORMAL COUNT */
        pBuf[2] = COUNT_NUM;   /* OPEN ERROR COUNT */
        pBuf[3] = COUNT_NUM;   /* SHORT ERROR COUNT */
        return mu8Error;
    }

    if( pBuf[ mu8Index ] != 0 )
    {
        pBuf[ mu8Index ]--;
    }

    if( pBuf[ mu8Index ] == 0 )
    {
        if( mu8Index == STATUS_NORMAL )
        {
            mu8Error = FALSE; // NORMAL
        }
        else
        {
            mu8Error = TRUE;  // ERROR 
        }
    }

    return mu8Error;
}
