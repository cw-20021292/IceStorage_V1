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
    if( IsOpenValve( VALVE_ROOM_COLD_OUT ) == TRUE               // ��/���� ����
    || IsOpenValve( VALVE_COLD_IN ) == TRUE         // �ü� ä���
    || IsOpenValve( VALVE_HOT_OUT ) == TRUE         // �¼� ����
    || IsOpenValve( VALVE_HOT_IN ) == TRUE          // �¼� �巹��
    || IsOpenValve( VALVE_ICE_TRAY_IN ) == TRUE     // ���̽� Ʈ���� �Լ�
    || IsOpenValve( VALVE_COLD_FLUSH) == TRUE       // �ü� �÷���
      )
    {
        return TRUE;
    }

    return FALSE;
}

U8 CheckErrLeakFlow(U8 mu8Error)
{
    // ���� �� �Ǵ� ���� Ʈ���� �Լ� ���̸�
    // ���� �˻� ������ �ƴ�.
    if( IsOpenValves() == TRUE )
    {
        ClearLeakFlow();

        return FALSE;
    }

    // ���� �˻� ����...
    if( GetFlowMeterHz() > LEAK_FLOW_HZ )
    {
        if( LeakFlow.DetectTime != 0 )
        {
            LeakFlow.DetectTime--;
        }
        else
        {
            ClearLeakFlow();
            return TRUE;    // ���� �߻�
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
    // ���� �� �Ǵ� ���� Ʈ���� �Լ� ���̸�
    // ���� �˻� ������ �ƴ�.
    if( IsOpenValves() == TRUE )
    {
        ClearLeakFlow();

        return TRUE; // ����.. ���� ���� �����̹Ƿ� ���� ����..
    }


    // ���� �˻� ����...
    if( GetFlowMeterHz() < LEAK_FLOW_HZ )
    {
        if( LeakFlow.ReleaseTime != 0 )
        {
            LeakFlow.ReleaseTime--;
        }
        else
        {
            ClearLeakFlow();
            return FALSE;   // ����
        }

    }
    else
    {
        LeakFlow.ReleaseTime = RELEASE_TIME;
    }

    return TRUE; //����
}
#endif
