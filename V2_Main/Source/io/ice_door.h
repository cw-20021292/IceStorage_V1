#ifndef __ICE_DOOR_H__
#define __ICE_DOOR_H__

#include "prj_type.h"


#define ICE_DOOR_OPEN_ALARAM_TIME         3      // 3sec 
#define ICE_DOOR_OPEN_BACK_FEEDER_TIME    8      // 8sec 
#define ICE_DOOR_CLEAR_BACK_FEEDER_TIME   20     // 20sec 

void InitIceDoor(void);


// 일반 제어용 열림 감지 시간
U16 GetIceDoorOpeningTime(void);
void SetIceDoorOpeningTime(U16 mu16Time);

// IOT용 열림 감지 시간
U16 GetIceDoorOpeningTimeIot(void);
void SetIceDoorOpeningTimeIot(U16 mu16Time);

void CountingIceDoorOpeningTime(void);

/* FORCED CLOSE DOOR */
#define  ICE_CLOSE_TIME    86400UL /* @1sec,  60sec x 60min x 24hour */
void SetIceDoorConfClosedTime(U32 mu16Time);
U32 GetIceDoorConfClosedTime(void);


#define  ICE_DOOR_OPEN     0     // FULL OPEND
#define  ICE_DOOR_OPENING  1     // OPEING..
#define  ICE_DOOR_CLOSE    2     // FULL CLOSED
#define  ICE_DOOR_CLOSING  3     // CLSOING...
U8 GetIceDoorStatus(void);
// 아이스 도어 추출 여부
// RETURN
// TRUE - 추출 중
// FALSE - 추출 중지
U8 IsOpenIceDoor(void);

void OpenIceDoor(void);
void CloseIceDoor(void);
void ForcedCloseIceDoor(void);

void ControlForcedCloseIceDoor(void);
void ControlIceDoor(void);

void TEST_StartIceDoor(void);
void TEST_StopIceDoor(void);
void TEST_RepeatIceDoor(void);

#endif /* __ICE_DOOR_H__ */
