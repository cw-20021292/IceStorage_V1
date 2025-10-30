#ifndef __ICE_FULL_H__
#define __ICE_FULL_H__

#include "prj_type.h"

typedef struct _ice_full_
{
    /* Status */
    U8 Full;         // 만빙 상태 
    U8 Out;          // 만빙 이후, 추출이 한번이라도 있었는지 확인
    
    /* CHECK */
    U8 Start;        // 만빙 검사 시작 상태 FLAG
    U8 CheckCount;   // 검사 시간 내에 기준치 초과 횟수
    U16 CheckTime;   // 검사 시간

    U8 PowerOnCheck;  // 전원 RESET 후, 최초 검사 실행 여부

	U8 initFull;
} IceFull_T;

void InitIceFull(void);

void GetIceFullData( IceFull_T *pIceFull );

U8 GetIceFullStatus(void);

U8 GetIceFullOutStatus(void);
void ClearIceFullOutStatus(void);

U8 GetIceFullPowerOnCheck(void);

void StartCheckIceFull(void);
U8 IsDoneCheckIceFull(void);

void CheckIceFull(void);

U8 GetInitIceFullStatus(void);
void SetInitIceFullStatus(U8 initIceFullStatus);

#endif /* __ICE_FULL_H__ */
