/**
 * File : timer.h
 * 
 * Process Sys Timer Api
**/
#ifndef __TIMER_H__
#define __TIMER_H__

#include "timer_id.h"

#define DELAY_MS(x) 
#define SEC(x)        ( (x) * 1000UL )

typedef void (*FPUserISR)(void);

typedef enum _timer_type_
{
    TIMER_PROCESS,        /// 0 :
    TIMER_USER,           /// 1 :
} ETimerType_T;

typedef enum _timer_status_
{
    TIMER_EXPIRE,         /// 0 :
    TIMER_NOT_EXPIRE,     /// 1 :
    TIMER_DISABLE,        /// 2 :
} ETimerStatus_T;

void InitTimer(void);
void StartTimer(ETimerType_T type, U8 id, U32 timeOut);
void DisableTimer(ETimerType_T type, U8 id);
void StopTimer(ETimerType_T type, U8 id);
U8 IsExpiredTimer(ETimerType_T type, U8 id);
void UpdateTimer(void);
void RegisterTimerISR(void (*PFUserISR)(void));
FPUserISR GetTimerISR(void);

#endif  /* __TIMER_H__ */
