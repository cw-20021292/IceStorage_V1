#ifndef __DISPLAY_LIB_H__
#define __DISPLAY_LIB_H__

#include "prj_type.h"



void InitDispTimer(void);

// TIMER COUNTER 
#define TIMER_100MS    1U
#define TIMER_500MS    5U
#define TIMER_1SEC     10U
#define TIMER_2SEC     20U
#define TIMER_3SEC     30U
#define TIMER_4SEC     40U
#define TIMER_5SEC     50U
#define TIMER_6SEC     60U
#define TIMER_7SEC     70U
#define TIMER_20SEC    200U
#define TIMER_30SEC    300U
#define TIMER_20MIN    12000U
#define TIMER_60MIN    36000U

// TIMER INDEX
enum
{
    DISP_TIMER_KEY_ICE,            /* 얼음 선택 */
    DISP_TIMER_KEY_AMOUNT,         /* 물양 선택 */
    DISP_TIMER_KEY_HOT,            /* 온수 선택 */
    DISP_TIMER_KEY_HOT_TEMP,       /* 온수 선택 */
    DISP_TIMER_CONFIG_TIME,        /* 설정 진입 금지 타이머 */

    DISP_TIMER_ALARM_LOCK_ALL,          /* 버튼 알람 - 전체 잠금 */
    DISP_TIMER_ALARM_LOCK_HOT,          /* 버튼 알람 - 온수 잠금 */
    DISP_TIMER_ALARM_LOCK_ICE,          /* 버튼 알람 - 얼음 잠금 */
    DISP_TIMER_ALARM_MAKE_OFF_ICE,      /* 버튼 알람 - 얼음 OFF */
    DISP_TIMER_ALARM_MAKE_OFF_COLD,     /* 버튼 알람 - 냉수 OFF */

    DISP_TIMER_ALARM_SETTING,
    DISP_TIMER_CONFIG_AMOUNT,       /* 물양 설정 */
    DISP_TIMER_VERSION,             /* 버전 표시 시간 */
    DISP_TIMER_MEMENTO,             /* 메멘토 타이머 */
    DISP_TIMER_SLEEP,               /* 대기 모드 */

    DISP_TIMER_CLOCK,              
    DISP_TIMER_ERROR,             
    DISP_TIMER_SETTING_MENU,        /* 설정 메뉴 EXIT 타이머 */
    DISP_TIMER_CUSTOMER_MENU,       
    DISP_TIMER_SOUND_MENU,          /* 사운드 설정 EXIT 타이머 */
    DISP_TIMER_CLOCK_MENU,       
    DISP_TIMER_UP,                  /* SETTING - UP */
    DISP_TIMER_DOWN,                /* SETTING - DOWN */
    DISP_TIMER_NEXT,                /* SETTING - DOWN */

    DISP_TIMER_WIFI_AP_ERROR,       /* WIFI - AP ERROR CODE */
    DISP_TIMER_ON,  
    DISP_TIMER_OFF, 
    DISP_TIMER_SETTING_CLOCK,       /* 시간 설정 완료 */

    MAX_DISP_KEY_TIMER_ID       
};

U8 IsExpiredDispTimer(U8 id);
void SetDispTimer(U8 id, U16 time);


// TIMER BLINK INDEX 
#define DISP_TIMER_ID_500MS        0    /* LED 점멸 타이머 0.5sec */
#define DISP_TIMER_ID_1SEC         1    /* LED 점멸 타이머 1sec */
#define MAX_DISP_TIMER_BLINK_ID    2
U8 IsExpiredDispBlinkTimer(U8 id);
U8 BlinkLED( U8 mu8OnOff, void (*pFun)(U8 mu8OnOff), U8 mu8TimerId );

void UpdateDisplayTimer(void);

#endif /* __DISPLAY_LIB_H__ */
