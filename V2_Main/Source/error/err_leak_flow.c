#include "err_leak_flow.h"
#include "flow_meter.h"
#include "valve.h"


#define DETECT_TIME             100U    // 10sec @100ms
#define RELEASE_TIME            600U    // 60sec @100ms

#define LEAK_FLOW_HZ            5       // 5Hz

typedef struct _err_leak_flow_
{
    U16 DetectTime;
    U16 ReleaseTime;
} ErrLeakFlow_T;


ErrLeakFlow_T   LeakFlow;

static void ClearLeakFlow(void)
{
    LeakFlow.DetectTime     = DETECT_TIME;
    LeakFlow.ReleaseTime    = RELEASE_TIME;
}

static U8 IsOpenValves(void)
{
    if( IsOpenValve( VALVE_ROOM_COLD_OUT ) == TRUE               // 냉/정수 추출
    || IsOpenValve( VALVE_COLD_IN ) == TRUE         // 냉수 채우기
    || IsOpenValve( VALVE_HOT_OUT ) == TRUE         // 온수 추출
    || IsOpenValve( VALVE_HOT_IN ) == TRUE          // 온수 드레인
    || IsOpenValve( VALVE_ICE_TRAY_IN ) == TRUE     // 아이스 트레이 입수
    || IsOpenValve( VALVE_COLD_FLUSH) == TRUE       // 냉수 플러싱
      )
    {
        return TRUE;
    }

    return FALSE;
}

U8 CheckErrLeakFlow(U8 mu8Error)
{
    // 추출 중 또는 얼음 트레이 입수 중이면
    // 에러 검사 조건이 아님.
    if( IsOpenValves() == TRUE )
    {
        ClearLeakFlow();

        return FALSE;
    }

    // 에러 검사 조건...
    if( GetFlowMeterHz() > LEAK_FLOW_HZ )
    {
        if( LeakFlow.DetectTime != 0 )
        {
            LeakFlow.DetectTime--;
        }
        else
        {
            ClearLeakFlow();
            return TRUE;    // 에러 발생
        }

    }
    else
    {
        LeakFlow.DetectTime = DETECT_TIME;
    }

    return FALSE;
}

// unused... 
#if 0   
U8 ReleaseErrLeakFlow(U8 mu8Error)
{
    // 추출 중 또는 얼음 트레이 입수 중이면
    // 에러 검사 조건이 아님.
    if( IsOpenValves() == TRUE )
    {
        ClearLeakFlow();

        return TRUE; // 에러.. 현재 에러 상태이므로 에러 유지..
    }


    // 에러 검사 조건...
    if( GetFlowMeterHz() < LEAK_FLOW_HZ )
    {
        if( LeakFlow.ReleaseTime != 0 )
        {
            LeakFlow.ReleaseTime--;
        }
        else
        {
            ClearLeakFlow();
            return FALSE;   // 해제
        }

    }
    else
    {
        LeakFlow.ReleaseTime = RELEASE_TIME;
    }

    return TRUE; //에러
}
#endif
