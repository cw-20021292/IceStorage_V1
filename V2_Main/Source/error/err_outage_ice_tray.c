#include "err_outage_ice_tray.h"
# include "flow_meter.h"

#define AUTO_RELEASE_TIME   600U     // 60sec

// 단수 에러 표시 유지 시간 1분
U16 gu16IceTrayReleaseTime = AUTO_RELEASE_TIME;      // 60sec.. @100ms
U8 CheckErrOutageIceTray(U8 mu8Error)
{
    if( GetOutageStatus( TYPE_ICE_TRAY ) == FLOW_OUTAGE )
    {
        return TRUE;    // ERROR
    }

    gu16IceTrayReleaseTime = AUTO_RELEASE_TIME;
    return FALSE;   // NO ERROR 
}

// call back interval : @100ms..
U8 ReleaseErrOutageIceTray(U8 mu8Error)
{
    // 1분 후 단수 에러 자동 해제
    if( gu16IceTrayReleaseTime == 0 )
    {
        SetOutageStatus( TYPE_ICE_TRAY, FLOW_SUPPLY );
    }
    else
    {
        gu16IceTrayReleaseTime--;
    }

    return CheckErrOutageIceTray( mu8Error );
}

