#ifndef __KEY_HANDLER_H__
#define __KEY_HANDLER_H__

#include "prj_type.h"
#include "key_common_handler.h"

// 키 이벤트 리스트 등록 함수
void RegisterKeyEvent( KeyValidFun_T pValid, KeyEventList_T *pList, U8 mu8Size );
U8 IsValidKeyCondition(U32 mu32Key);
void* GetKeyEventList(void);
U8 GetKeyEventListSize(void);

#endif /* __KEY_HANDLER_H__ */
