#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "prj_type.h"

typedef struct _service_check_
{
    U8   TankOpen;   /* 탱크 탑 커버가 OPEN 여부 */

    U8   Status;     /* 서비스 점검 표시 여부 */
    U32  Time;       /* 서비스 점검 Time out 시간 */
    U32  SaveTime;   /* 서비스 점검 Time EEPROM 저장 주기 */

    U16  OpenTime;   /* 서비스 점검 TANK OPEN 시간 @sec */
} Service_T;


void InitServiceCheck(void);

void GetServiceCheckData(Service_T *pData );

U8 GetServiceCheckTankOpen(void);

U8 GetServiceCheckStatus(void);

#define	SERVICE_TIME_MIN ( 129600UL )	/* 60min x 24hour x 90days = 129600 min */
void SetServiceTime(U32 mu32Time);
U32 GetServiceTime(void);

void SetServiceCheckDay(U8 mu8Days);
U8 GetServiceCheckDay(void);

// TANK OPENING TIME.. @sec
void SetServiceOpenTime(U16 mu32Time);
U16 GetServiceOpenTime(void);
void ResetServiceOpenTime(void);

// @100ms
void ProcessCheckTankOpen(void); 

// @1min
void ProcessServiceCheck(void);
#endif /* __SERVICE_H__ */
