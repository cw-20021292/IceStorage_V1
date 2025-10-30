#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "prj_type.h"
#include "hal_level.h"


/* LEVEL */
#define  LEVEL_LOW            HAL_LEVEL_LOW        
#define  LEVEL_MID            HAL_LEVEL_MID        
#define  LEVEL_ERR_LOW        HAL_LEVEL_ERR_LOW    
#define  LEVEL_HIGH           HAL_LEVEL_HIGH       
#define  LEVEL_ERR_COMPLEX    HAL_LEVEL_ERR_COMPLEX
#define  LEVEL_ERR_HIGH       HAL_LEVEL_ERR_HIGH   
#define  LEVEL_ERR_OVF_LOW    HAL_LEVEL_ERR_OVF_LOW
#define  LEVEL_OVF            HAL_LEVEL_OVF        

/*ERROR TYPE */
#define  ERR_TYPE_LOW       0
#define  ERR_TYPE_COMPLEX   1
#define  ERR_TYPE_HIGH      2
#define  ERR_TYPE_OVF       3
#define  ERR_TYPE_NUM       4

// 수위 센서 감지 시간 초기화
void InitTankConfDetectTime(void);

// 센서 감지 지연 시간 설정
void SetTankConfDetectTime(U8 mu8Id, U8 mu16DetectTime);

U8 GetTankLevelErrorCount(U8 mu8Id, U8 mu8ErrType);
void SetTankLevelErrorCount(U8 mu8Id, U8 mu8ErrType, U8 mu8Count);
void ClearTankLevelErrorCount(void);

/* TANK LEVEL ID  */
#if 0
#define  LEVEL_ID_ROOM        0
#define  LEVEL_ID_COLD        1
#define  LEVEL_ID_HOT         2
#define  LEVEL_ID_DRAIN       3
#define  LEVEL_ID_ICE_TRAY    4
#define  MAX_LEVEL_ID         5
#else
#define  LEVEL_ID_DRAIN       0
#define  MAX_LEVEL_ID         1
#endif
U8 GetTankLevel(U8 mu8Id);

// 탱크 센서 감지 여부 반환
// Return :
//  TRUE : 감지
//  FALSE : 미감지
U8 IsDetectTankLevel(U8 mu8Id, U8 mu8Level);

// 탱크 만수위 여부 확인
// Return :
//  TRUE : 탱크 가득 
//  FALSE : 탱크 가득 아님
U8 IsTankLevelFull(U8 mu8Id);

// 탱크 센서 에러 상태 검사
// Return
//  TRUE : 에러
//  FALSE ; 정상
U8 IsErrorTankLevel(U8 mu8Level);


void ProcessTankLevel(void);

#endif /* __LEVEL_H__ */
