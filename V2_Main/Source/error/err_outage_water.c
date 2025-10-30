#include "err_outage_water.h"
#include "flow_meter.h"

#define AUTO_RELEASE_TIME   600U     // 60sec

// 단수 에러 표시 유지 시간 1분
U16 gu16AutoReleaseTime = AUTO_RELEASE_TIME;      // 60sec.. @100ms
U8 CheckErrOutageWater(U8 mu8Error)
{
    if( GetOutageStatus( TYPE_WATER ) == FLOW_OUTAGE )
    {
        return TRUE;    // ERROR
    }

    gu16AutoReleaseTime = AUTO_RELEASE_TIME;
    return FALSE;   // NO ERROR 
}

// call back interval : @100ms..
U8 ReleaseErrOutageWater(U8 mu8Error)
{
    // 1분 후 단수 에러 자동 해제
    if( gu16AutoReleaseTime == 0 )
    {
        SetOutageStatus( TYPE_WATER, FLOW_SUPPLY );
    }
    else
    {
        gu16AutoReleaseTime--;
    }

    return CheckErrOutageWater( mu8Error );
}

