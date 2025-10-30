#include "water_out_common.h"
#include "flow_meter.h"
#include "water_out.h"



#define DEFAULT_AMOUNT  0UL

// @10ms...
const static WaterOutTable_T OutTableList[] = 
{
    { OUT_AMOUNT_0,          0UL },
    { OUT_AMOUNT_10,         AMOUNT_10 },
    { OUT_AMOUNT_120,        AMOUNT_120 },
    { OUT_AMOUNT_250,        AMOUNT_250 },
    { OUT_AMOUNT_500,        AMOUNT_500 },
    { OUT_AMOUNT_1000,       AMOUNT_1000 },
    { OUT_AMOUNT_ICE_TRAY,   AMOUNT_235 },
#if CONFIG_TEST_OUT_CONTY
    { OUT_AMOUNT_INFINITY,   720000UL },     // 120min
#else
    { OUT_AMOUNT_INFINITY,    AMOUNT_10000 },   // 10L ( 3min )
#endif
    { OUT_AMOUNT_CLEAN,      30000UL }      // 300sec (5min)
};

#define SZ_OUT_TABLE   (sizeof(OutTableList)/sizeof(WaterOutTable_T))

// ?�량�?추출 ?�간??반환?�다.
U32 GetFindOutTargetTime(WaterOutType_T mType)
{
    U8 i;

    for( i = 0; i < SZ_OUT_TABLE ; i++ )
    {
        if( OutTableList[ i ].Type == mType )
        {
            return OutTableList[ i ].Amount;
        }
    }
	//WIFI SETTING AMOUNT
	if( mType == OUT_AMOUNT_HALF_WIFI )
	{
		return GetWaterAmountConv(UNFIXED_AMOUNT_HALF);
	}
	else if( mType == OUT_AMOUNT_ONE_WIFI )
	{
		return GetWaterAmountConv(UNFIXED_AMOUNT_ONE);
	}
	else if( mType == OUT_AMOUNT_TWO_WIFI )
	{
		return GetWaterAmountConv(UNFIXED_AMOUNT_TWO);
	}

/*
    // USER AMOUNT
    if( mType == OUT_AMOUNT_USER )
    {
        return GetWaterOutUserAmount();
    }
*/
    return DEFAULT_AMOUNT;
}


// 추출??반환 ( percent )
U8 CalcPercent(U32 mu32Target, U32 mu32Current)
{
    return (U8)(( (F32)mu32Current / (F32)mu32Target ) * 100.0f);
}



// 추출??ml ?�위 반환
U16 CalcLiter(U32 mu32Amount)
{
    U32 mu32TargetAmount;


    mu32TargetAmount = GetFindOutTargetTime( OUT_AMOUNT_10 );
    if( mu32Amount >= mu32TargetAmount )
    {
        mu32Amount = mu32Amount / mu32TargetAmount;
        mu32Amount *= ML_UNIT;   
    }
    else
    {
        mu32Amount = 0UL;
    }

    return (U16)(mu32Amount);

}

// ml�?추출?�으�?변??
U32 CalcAmount(U16 mLiter)
{
    U32 mu32Amount;

    mLiter /= ML_UNIT;
    mu32Amount = mLiter * GetFindOutTargetTime( OUT_AMOUNT_10 );

    return mu32Amount;
}

