#include "health_maker.h"

#include "hal_adc.h"
#include "level.h"
#include "valve.h"
#include "drain.h"
#include "drain_pump.h"
#include "cold_water.h"
#include "ice.h"
#include "tank_clean.h"
#include "water_out_common.h"


// ��� �ִ� �ð�, timeout�Ǹ� ��� ���� ���� ����
#define CONFIG_USE_DRAIN_TIMEOUT   1   

/* �ü�, ���� �ܼ� ���� �ð� */
#define DRAIN_MAX_TIMEOUT       6000U  /*100msec 10min x 60sec x 10 = 10min, �ִ� ��� ���� �ð� */

DoDrain_T           DrainStep;

void InitHealthDrain(void)
{
    DrainStep.Step = 0;

#if CONFIG_USE_DRAIN_TIMEOUT
    DrainStep.ExitTime = DRAIN_MAX_TIMEOUT;
#endif
}

void GetHealthDrainData(DoDrain_T *pDrain)
{
    MEMCPY( (void __FAR *)pDrain, (const void __FAR *)&DrainStep, sizeof( DoDrain_T ));
}

void UpdateHealthDrainTime(void)
{
    DrainStep.ExitTime = 0U;
}

void StopHealthDrain(void)
{
    if( DrainStep.Step <= STEP_DRAIN_COLD )
    {
        DrainStep.Step = STEP_DRAIN_CLOSE_IN;
    }
}

HMStatus_T DoHealthDrain(void)
{
    /*��� ������� Step*/
    HMStatus_T mStatus = STATUS_GOING;


    switch(DrainStep.Step)
    {
        case STEP_DRAIN_READY:
            CloseValve( VALVE_COLD_AIR );
            OpenValve( VALVE_COLD_DRAIN );

            DrainStep.ExitTime = DRAIN_MAX_TIMEOUT;

            DrainStep.WaitTime = WAIT_200MS;
            DrainStep.Step++;
            break;

        case STEP_DRAIN:
            if( DrainStep.WaitTime != 0 )
            {
                DrainStep.WaitTime--;
            }
            else
            {
                OpenValve( VALVE_COLD_IN );
                DrainStep.Step++;
            }
            break;

        case STEP_DRAIN_COLD:
            // �ü� ��ũ ����
            CloseValve( VALVE_COLD_AIR );
            CloseValve( VALVE_ROOM_COLD_OUT );

            OpenValve( VALVE_COLD_DRAIN );
            OpenValve( VALVE_COLD_IN );

            // �ִ� ��� �ð� ����
            if( DrainStep.ExitTime != 0 )
            {
                DrainStep.ExitTime--;
            }
            else
            {
                DrainStep.Step++;
            }

            break;

        case STEP_DRAIN_CLOSE_IN:
            CloseValve( VALVE_COLD_IN );
            DrainStep.WaitTime = WAIT_100MS;

            DrainStep.Step++;
            break;

        case STEP_DRAIN_CLOSE_DRAIN:
            if( DrainStep.WaitTime != 0 )
            {
                DrainStep.WaitTime--;
            }
            else
            {
                CloseValve( VALVE_COLD_IN );
                CloseValve( VALVE_ROOM_COLD_OUT );

                OpenValve( VALVE_COLD_AIR );
                CloseValve( VALVE_COLD_DRAIN );

                DrainStep.Step++;
            }
            break;

        case STEP_DRAIN_DONE:

            mStatus = STATUS_DONE;
            break;

        default:
            DrainStep.Step = 0;
            break;
    }


    return mStatus;
}


