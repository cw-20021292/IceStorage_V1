#include "err_outage_hot_water.h"
#include "flow_meter.h"
#include "hot_water.h"


#define AUTO_RELEASE_TIME   300U     // 30sec

// 단수 에러 표시 유지 시간 1분
U16 gu16AutoHotWaterReleaseTime = AUTO_RELEASE_TIME;      // 60sec.. @100ms
U8 CheckErrOutageHotWater(U8 mu8Error)
{
    if( GetOutageStatus( TYPE_HOT_WATER ) == FLOW_OUTAGE )
    {
        SetHotWaterCooling( TRUE );
        return TRUE;    // ERROR
    }

    gu16AutoHotWaterReleaseTime = AUTO_RELEASE_TIME;
    return FALSE;   // NO ERROR 
}

// call back interval : @100ms..
U8 ReleaseErrOutageHotWater(U8 mu8Error)
{
    // 1분 후 단수 에러 자동 해제
    if( gu16AutoHotWaterReleaseTime == 0 )
    {
        SetOutageStatus( TYPE_HOT_WATER, FLOW_SUPPLY );
    }
    else
    {
        gu16AutoHotWaterReleaseTime--;
    }

    return CheckErrOutageHotWater( mu8Error );
}

