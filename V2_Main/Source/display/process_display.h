#ifndef __PROCESS_DISPLAY_H__
#define __PROCESS_DISPLAY_H__

#include "prj_type.h"
#include "display_lib.h"

void InitDisplay(void);


/* 초기 FRONT 초기 점멸 완료 상태 */
U8 GetDisplayInit(void);
void StartDisplayInit(void);

// Display Timer start/stop
void StartDispTimerId(U8 id);
void StopDispTimerId(U8 id);

/* 설정 모드 진입 유효성 타이머 */
U8 IsExpiredConfigTime(void);

/* 버전 표시 */
void SetVersionDisp(U8 mu8DispSec);

/* 설정 메뉴 */
U8 GetSettingDisp(void);
void SetSettingDisp(U8 mu8Conf);

/* 시간 설정 메뉴 */
U8 GetClockDisp(void);
void SetClockDisp(U8 mu8Conf);

/* 필터 메뉴 */
void SetFilterDisp(U8 mu8Conf);
U8 GetFilterDisp(void);

/* 음량 설저 상태 */
void SetSoundDisp(U8 mu8Conf);
U8 GetSoundDisp(void);

/* 관리 모드 설정 */
void SetCustomerDisp(U8 mu8Conf);
U8 GetCustomerDisp(void);

/* 메멘토 */
U8 GetMementoDisp(void);
void SetMementoDisp(U8 mu8Conf );

/* FOTA */
U8 GetFotaDisp(void);
void SetFotaDisp(U8 mu8Conf );

/* 디스플레이 제어 */
void ProcessDisplay(void);

#endif /* __PROCESS_DISPLAY_H__ */
