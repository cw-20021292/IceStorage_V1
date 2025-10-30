#ifndef __SWING_BAR_H__
#define __SWING_BAR_H__

#include "prj_type.h"


#define  TEST_SWING_BAR    0

#if TEST_SWING_BAR
//// 박정철 부장님 사양 2021.08.23
//U8 the_swing_off = 180;
//U8 the_swing_on  = 20;  

// 2021.01.19
U16 the_swing_off = 180;
U16 the_swing_on  = 20;  
#define  DEFAULT_SWING_BAR_TIME_OFF     the_swing_off
#define  DEFAULT_SWING_BAR_TIME_ON      the_swing_on
#else
#define  DEFAULT_SWING_BAR_TIME_OFF_1   500U   /* 5s @10ms */
#define  DEFAULT_SWING_BAR_TIME_OFF_2  1000U   /* 10s @10ms */
#define  DEFAULT_SWING_BAR_TIME_ON       20U   /* 0.2s @10ms */
#endif

typedef struct _swing_bar_h_
{
    U8 OnOff;      // Swing Bar On/Off command
    U16 OnTime;     // Swing Bar On/Off time
    U16 OffTime;    // Swing Bar On/Off time

    U16 ConfOnTime;    // swing bar off config time
    U16 ConfOffTime;    // swing bar off config time
}SwingBar_T;

void InitSwingBar(void);

void GetSwingBarData( SwingBar_T *pData );

void SetSwingBarConfigOffTime(U16 mu16ConfTime);

void TurnOffSwingBar(void);

void TurnOnSwingBar(void);

void ControlSwingBar(void);

#endif /* __SWING_BAR_H__ */
