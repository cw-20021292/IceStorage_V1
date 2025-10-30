#ifndef __HEALTH_MAKER_H__
#define __HEALTH_MAKER_H__

#include "prj_type.h"



/* Health Maker Status */
typedef enum
{
    STATUS_GOING,
    STATUS_DONE,
    STATUS_ERR,
} HMStatus_T;  

/*Drain - ��� */
typedef enum
{
    STEP_DRAIN_READY,      // ��� �غ�
    STEP_DRAIN,
    STEP_DRAIN_COLD,
    STEP_DRAIN_CLOSE_IN,
    STEP_DRAIN_CLOSE_DRAIN,
    STEP_DRAIN_DONE,
} DrainStep_T;

typedef struct _do_drain_
{
    DrainStep_T Step;
    U16 WaitTime;         // �ü� �Լ� ��� ���� �ð�
    U16 ExitTime;         // ��ü ��� ���� ����Ʈ Ÿ�̸�
    //U16 ResidualTime;     // ���� ��ũ �ܼ� ���� Ÿ�̸�
    //U16 StayTime;         // �ü� ������ ���� ���� Ȯ�� �ð�
} DoDrain_T;

// ���
void InitHealthDrain(void);
void GetHealthDrainData(DoDrain_T *pDrain);
void UpdateHealthDrainTime(void);

void StopHealthDrain(void);

// ��� �� ���
HMStatus_T DoHealthDrain(void);

#endif /* __HEALTH_MAKER_H__ */
