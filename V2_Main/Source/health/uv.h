#ifndef __UV_H__
#define __UV_H__
  
#include "prj_type.h"
  
// UV ID 
typedef enum 
{
    UV_ICE_DOOR,            // 정기 살균
    UV_ICE_DOOR_SHOT,       // 얼음 추출 종료 후

    UV_WATER_OUT,           // 정기 살균
    UV_WATER_OUT_SHOT,      // 물 추출 종료 후

    UV_ICE_TRAY,            // 만빙 일 때,
    UV_ICE_TRAY_SHOT,       // 탈빙 중이 때,

    UV_ICE_TANK,            // 정기 살균

    MAX_UV_NUM  
}UvId_T;


// config
void SetConfigUvId(UvId_T id, U8 config);
U8 GetConfigUvId(UvId_T id);

// @100ms
void TurnOnUvId(UvId_T id, U32 OffTime, U32 OnTime );
void TurnOffUvId(UvId_T id);
U8  IsTurnOnUvId(UvId_T id);

// status 
// - true : pause,
// - false : run
void SetPauseUvId(UvId_T id, U8 status);

U8 GetUvOnPercent(UvId_T id);

// IsLimited
#define UNLIMITED   0
#define LIMITED     1
void SetLimitConfUvId(UvId_T id, U8 IsLimited);

void SetLimitCountUvId(UvId_T id, U8 mu8Count);

void ControlUv(void);

#endif   /* __UV_H__ */
