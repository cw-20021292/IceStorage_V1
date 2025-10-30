#ifndef __DISPLAY_CLOCK_H__
#define __DISPLAY_CLOCK_H__

#include "prj_type.h"


// 시간 설정 항목
typedef enum
{
    MENU_ITEM_TIME_AMPM,
    MENU_ITEM_TIME_HOUR,
    MENU_ITEM_TIME_MIN
} TimeItem_T;
#define MENU_ITEM_MIN   MENU_ITEM_TIME_AMPM
#define MENU_ITEM_MAX   MENU_ITEM_TIME_MIN

#define TIME_AM     0
#define TIME_PM     1
typedef struct _menu_time_
{
    U8 u8Year;
    U8 u8Month;
    U8 u8Date;
    U8 u8AmPm;
    U8 u8Hour;
    U8 u8Min;
} MenuTime_T;

void InitClock(U8 mu8Init);
TimeItem_T GetClockItem(void);

void GetClockTime(MenuTime_T *pTime);
void NextClock(void);
void UpClock(void);
void DownClock(void);
void SaveClock(void);

void GetTimeHour12(U8 hour24, U8 *pAm, U8 *pHour12);

#endif /* __DISPLAY_CLOCK_H__ */
