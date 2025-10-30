#ifndef __KEY_COMMON_HANDLER_H__
#define __KEY_COMMON_HANDLER_H__

#include "prj_type.h"


/* 키 버튼 입력 유효성 조건 검사 함수 인터페이스 */
typedef U8 (*KeyValidFun_T)(U32 mu32Key);

/* 키 이벤트 리스트 인터페이스 */
#define KEY_ACTION_SHORT    0   // Short key
#define KEY_ACTION_2_SEC    1   // Long key 2sec
#define KEY_ACTION_3_SEC    2   // Long key 3sec
#define KEY_ACTION_5_SEC    3   // Long key 5sec
#define KEY_ACTION_POP      4   // Pop key
#define KEY_ACTION_1_SEC    5   // Long key 1sec ( for test mode time short )
#define KEY_ACTION_NUM      6   
typedef U8(*KeyFun_T)(void);
typedef struct _key_event_
{
    U32        Key;
    KeyFun_T   actionEvent[KEY_ACTION_NUM];
} KeyEventList_T;

// KEY EVENT LIST SIZE 
typedef U8 (*KeyEventSize_T)(void);

#endif /* __KEY_COMMON_HANDLER_H__ */
