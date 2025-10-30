#ifndef __KEY_EVENT_LIB_H__
#define __KEY_EVENT_LIB_H__

#include "prj_type.h"


// 연속키 입력 처리
#define DEFAULT_CONT_TICK   2      // 500ms @100ms
void InitContKey(U8 mu8Tick);
void ControlContKey( U8 (*pfKeyDown)(void), U8 (*pfKeyUp)(void));
U8 KeyContDown(void);
U8 KeyContUp(void);
U8 KeyStopCont(void);


#endif /* __KEY_EVENT_LIB_H__ */
