#include "hw.h"
#include "metering.h"
#include "hal_adc.h"

#include <stdio.h>
#include <string.h>



Meter_T Metering;

void InitMetering(void)
{
    Metering.Current[ ID_METERING_12V ] = 0.0f;
    Metering.Current[ ID_METERING_24V ] = 0.0f;
}

Current_T GetMeteringCurrent(U8 id)
{
    return Metering.Current[ id ];
}


/*
 *1. DC 저전류 계산식 (mA 단위, 10mA~500mA)
 *   OP AMP 출력 전압 기준 
 *    - 0.05V 미만 : 0mA 적용
 *    - 0.05~3.50V : ((OP AMP 출력전압+0.025) X 1000) / (0.3 X 24)
 *    - 3.50V 초과 : 미사용 영역으로 고전류 부분을 이용

 *2. DC 고전류 계산 (mA 단위, 300mA~2,000mA)
 *   OP AMP 출력 전압 기준
 *    - 0.30 미만 : 미사용 영역
 *    - 0.30~3.50V : (OP AMP 출력전압 X 1000) / (0.3 X 5.76)
 *    - 3.50V 초과 : 미사용 영역
 */
// TBD
static Current_T CalcCurrent_DC_12V(U16 mu16RawAdc)
{
    
    F32 mVolt;
    Current_T mCurrent = 0.0f;

    mVolt = (5.0f/1024.0f) * (F32)mu16RawAdc;

    if( mVolt < 0.05f )
    {
        mCurrent = 0.0f;
    }
    else if( mVolt > 3.5f )
    {
        mCurrent = (mVolt + 0.025f ) * 1000.0f;
        mCurrent = mCurrent / ( 0.3f * 24.0f );
    }

    return mCurrent;
}

// TBD
static Current_T CalcCurrent_DC_24V(U16 mu16RawAdc)
{
    F32 mVolt;
    Current_T mCurrent = 0.0f;

    mVolt = (5.0f/1024.0f) * (F32)mu16RawAdc;

    if( mVolt < 0.05f )
    {
        mCurrent = 0.0f;
    }
    else if( mVolt > 3.5f )
    {
        mCurrent = (mVolt + 0.025f ) * 1000.0f;
        mCurrent = mCurrent / ( 0.3f * 24.0f );
    }

    return mCurrent;

}

// 100ms...
void ProcessMetering(void)
{
    U16 mRawAdc = 0U;


    // Current 12V
    if( GET_STATUS_SMPS_12V() == 0 /* 12V ACTIVE */ )
    {
        mRawAdc = HAL_GetAdcValue( ANI_12V_FB );
        Metering.Current[ ID_METERING_12V ] = CalcCurrent_DC_12V( mRawAdc );
    }

    // Curernt DC24V
    mRawAdc = HAL_GetAdcValue( ANI_24V_FB );
    Metering.Current[ ID_METERING_24V ] = CalcCurrent_DC_24V( mRawAdc );
}

