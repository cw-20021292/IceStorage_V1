/*************************************************************************************************
 * HEATER TABLE
 *
 *
 *
 */
#include "heater_table.h"

// 유량별 1도를 올리는데 필요한 WATT
WATT_T GetWattByLPM(F32 mLPM)
{
    if( mLPM >= 1.2f )      { return 84; }
    else if( mLPM >= 1.1f ) { return 77; }
    else if( mLPM >= 1.0f ) { return 70; }
    else if( mLPM >= 0.9f ) { return 63; }
    else if( mLPM >= 0.8f ) { return 56; }
    else if( mLPM >= 0.7f ) { return 49; }
    else if( mLPM >= 0.6f ) { return 42; }
    else if( mLPM >= 0.5f ) { return 35; }
    else if( mLPM >= 0.4f ) { return 28; }
    else if( mLPM >= 0.3f ) { return 21; }
    else if( mLPM >= 0.2f ) { return 14; }

    return 7;
}


#if 0
// 예열에 필요한 시간 반환 ( @100ms )
#define AMOUNT_ML   0.095f   // 95mL ( 히터 체적 )
U16 dbg_offset_time = 300;
U16 GetHeatTime(U16 mWatt, TEMP_T tTarget, TEMP_T tIn)
{
    F32 mTime;
    U16 mRetVal = 0;

    mTime = AMOUNT_ML * 4.2f * ( tTarget - tIn ) / ( (F32)mWatt / 1000.0f) ;
    mTime = mTime + 0.005;
    if( mTime > 0.0f )
    {
        mRetVal = (U16)(mTime * 100.0f );
    }

    // 3초 이하인 경우, 최소 3초 이상 동작하도록 수정
    if( mRetVal < dbg_offset_time )
    {
        mRetVal = dbg_offset_time;
    }

    return mRetVal;
}
#endif
