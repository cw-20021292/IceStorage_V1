#ifndef __ICE_TRAY_H__
#define __ICE_TRAY_H__

#include "prj_type.h"

typedef struct _ice_tray_
{
    U8 Status;    // MOVING / ICING / DEICING
    U8 Mode;      // INIT(0x01) / DEICING(0x02) / ICING(0x04)
    U8 ErrorMoving;        // FALSE(Normal) or TRUE(Error)
    U8 ErrorMicroSwitch;   // FALSE(Normal) or TRUE(Error)
    U8 ErrorIcingCount;    // Error Repeat Count
    U8 ErrorDeIcingCount;  // Error Repeat Count
    U8 ErrorDeIcingAccumCount;  // Error Repeat Accumulative Count

    /* Step */
    U8 IcingStep;
    U8 DeIcingStep;

    /* TIME */
    U32 StartTime;      
    U32 MovingTime;    

    /* POSITION */
    U8 Posi_Icing;
    U8 Posi_DeIcing;
    U16 DeIcingDetectWaitTime;
    U16 IcingDetectWaitTime;
    U16 CheckTimer;     // 제빙 위치로 원점 재이동 하기 위한 타이머

    /* Recovery Reversing */
    U8 RR_Count;
    U32 RR_WaitTime;

    /* Recovery Coil Freezing */
    U8 RCF_Cmd;
    U32 RCF_Time;
    U8 RCF_CmdCount;

    /* Recovery Faild Deicing */
    U32 RFD_WaitTime;

} IceTray_T;

void InitIceTray(void);
void GetIceTrayData( IceTray_T *pData );

// 아이스 트레이 구속 해제 동작 중?
// 0-정상, 1-제빙 위치 에러, 2-탋이 위치 이동 에러
U8 IsRetryIceTray(void);


// 아이스 트레이 구속 해제 동작.
// 탈빙 위치 이동 시 에러 발생 상태인 경우 TRUE 반환
U8 IsErrorIceTrayCoilFrezzing(void);

// 트레이 이동 에러 적산 카운트 초기화
void ResetErrorIceTrayAccumCount(void );

#define  ICE_TRAY_MOVING    0
#define  ICE_TRAY_ICING     1
#define  ICE_TRAY_DEICING   2
U8 GetIceTrayStatus(void);

U8 GetIceTrayErrorMoving(void);
U8 GetIceTrayErrorMicroSwitch(void);

void MoveIceTrayIcingEol(void);
void MoveIceTrayIcing(void);
void MoveIceTrayDeIcing(void);

void ControlIceTray(void);

#endif /* __ICE_TRAY_H__ */
