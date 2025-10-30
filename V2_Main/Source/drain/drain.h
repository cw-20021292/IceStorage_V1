#ifndef __DRAIN_H__
#define __DRAIN_H__

#include "prj_type.h"
#include "level.h"


/* 초기화 */
void  InitDrain(void);

/* 드레인 탱크 수위 상태 */
void  SetDrainLevel(U8 mu8Val );
U8    GetDrainLevel(void);

void  SetInitDrain( U8 mu8Val );
U8    GetInitDrain(void);

void  SetInitDrainReady( U8 mu8Val );
U8    GetInitDrainReady(void);

/* 드레인 펌프 ON, 배수 탱크 수위 확인 후 */
void StartDrainPumpOnLevel(void);

/* 드레인 탱크 수위 제어 @100ms */
void ControlDrainLevel(void);

#endif /* __DRAIN_H__ */
