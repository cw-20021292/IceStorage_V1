#ifndef __FRONT_H__
#define __FRONT_H__

#include "prj_type.h"

void InitFront(void);

void SetFactroySetup(U8 mu8Setup);
U8 GetFactorySetup(void);
void CheckFactroySetup(void);


#define DECO_LED_BUTTON     0
#define DECO_LED_WELCOME    1
#define MAX_DECO_NUM        2
void SetConfigDeco(U8 mu8Id, U8 mu8OnOff );
U8 GetConfigDeco(U8 mu8Id);

#define SYSTEM_CHP     0
#define SYSTEM_CP      1
void SetSystem(U8 mu8Type );
U8 GetSystem(void);

void SetFrontSystem(U8 mu8Type );
U8 GetFrontSystem(void);

void CheckSystem( void );

// 서비스 관리 타입
#define CUSTOMER_SELF       0
#define CUSTOMER_VISIT      1
void SetCustomerType(U8 mu8Type);
U8 GetCustomerType(void);

#define  LOCK     1
#define  UNLOCK   0
void SetLockAll(U8 mu8Lock );
U8 GetLockAll(void);

void SetLockHot(U8 mu8Lock );
U8 GetLockHot(void);

void SetLockIce(U8 mu8Lock );
U8 GetLockIce(void);

// 정수기 미사용 시간 ( 실버케어)
// 미사용 시간(분)을 반환
#define SILVER_CARE_TIME_MIN    (2880UL) /* 48hour x 60min */
U32 GetSystemUnusedTime(void);  
void ResetUnusedTime(void);
void CountingUnusedTime(void);


U8 GetFotaReboot(void);
void SetFotaReboot(U8 mu8Fota);


// LIMIT TIMER ( 특수 키 입력 제한 타이머 )
U8 IsExpiredTestKeyLimitTimer(void);
void UpdateTestKeyLimitTimer(void);

#endif /* __FRONT_H__ */
