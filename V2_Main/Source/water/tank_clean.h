#ifndef __TANK_CLEAN_H__
#define __TANK_CLEAN_H__

#include "prj_type.h"

void InitTankClean(void);

/* 탱크 비우기 모드 설정/해제 */
void SetTankClean(U8 mu8Mode);
U8 GetTankClean(void );
U8 IsTankCleanMode(void);

/* 코디 서비스 모드 */
void SetCodyService(U8 mu8Mode);
U8 GetCodyService(void);

/* 부하 OFF */
U8 GetTankCleanOffLoad(void);
void ClearTankCleanOffLoad(void);

void ProcessTankClean(void);

#endif /* __TANK_CLEAN_H__ */
