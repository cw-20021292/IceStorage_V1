/**
 * File : timer.c
 * 
 * Process Sys Timer Api
**/
#include "hw.h"
#include "timer.h"

#include "hal_timer.h"

//#include <string.h>

#define	HZ	(1UL)

FPUserISR userISR = NULL;

typedef struct _timer_
{
    U8 Enable;
    U32 TimeOut;
} STimer_T;

LOCAL STimer_T Timer[TIMER_ID_MAX];
LOCAL STimer_T UserTimer[TIMER_USER_ID_MAX];

/**
 * @brief     Function For Initialize Timer
 * @detail    TAU0 / Channel0 / Interval Timer
 * @param     none
 * @return    none
**/
void InitTimer(void)
{
    MEMSET( (void __FAR *)Timer, 0, sizeof(Timer) );
    MEMSET( (void __FAR *)UserTimer, 0, sizeof(UserTimer) );

    HAL_TIMER00_START();   // Start Timer-00
}

/**
 * @brief                Function For Get Address Of TimerInfo Buffer
 * @detail               none
 * @param     xUid       TimerId
 * @param     xUtimeOut  TimeOut
 * @return               none
**/
static STimer_T* GetAddrOfBuf(ETimerType_T type, U8 id)
{
    STimer_T *pTimer = NULL;

    if( type == TIMER_PROCESS )
    {
        pTimer = &Timer[id];
    }
    else  /// TIMER_USER
    {
        pTimer = &UserTimer[id];
    }

    return pTimer;
}

/**
 * @brief                Function For Start Timer
 * @detail               none
 * @param     xEtype     Timer Type
 * @param     xUid       TimerId
 * @param     xUtimeOut  TimeOut
 * @return               none
**/
void StartTimer(ETimerType_T type, U8 id, U32 timeOut)
{
    STimer_T *pTimer = NULL;

    INTS_BYTE_BASE_TIMER();

    ENTER_CRITICAL_SECTION_TIMER();

    pTimer = GetAddrOfBuf(type, id);

    pTimer->Enable = 1U;
    pTimer->TimeOut = timeOut / HZ;
#if (HZ != 1UL)
    // What is the Function?
    if( (timeOut % HZ) >= (U32)5 )
    {
        pTimer->TimeOut++;
    }
#endif

    EXIT_CRITICAL_SECTION_TIMER();
}

/**
 * @brief                Function For Disable Timer
 * @detail               none
 * @param     xEtype     Timer Type
 * @param     xUid       TimerId
 * @return               none
**/
void DisableTimer(ETimerType_T type, U8 id)
{
    STimer_T *pTimer = NULL;

    pTimer = GetAddrOfBuf(type, id);

    pTimer->Enable 	= 0U;
    pTimer->TimeOut	= (U32)-1;
}

/**
 * @brief                Function For Stop Timer
 * @detail               none
 * @param     xEtype     Timer Type
 * @param     xUid       TimerId
 * @return               none
**/
void StopTimer(ETimerType_T type, U8 id)
{
    DisableTimer(type, id);
}

/**
 * @brief                Function For Stop Timer
 * @detail               none
 * @param     xEtype     Timer Type
 * @param     xUid       TimerId
 * @return               TIMER_EXPIRE /TIMER_NOT_EXPIRE / TIMER_DISABLE
**/
U8 IsExpiredTimer(ETimerType_T type, U8 id)
{
    STimer_T *pTimer = NULL;
    U32	timeOut = 0UL;

    INTS_BYTE_BASE_TIMER();

    ENTER_CRITICAL_SECTION_TIMER();

    pTimer = GetAddrOfBuf(type, id);
    
    timeOut = pTimer->TimeOut;
    EXIT_CRITICAL_SECTION_TIMER();

    if( pTimer->Enable == 0 )
    {
        return TIMER_DISABLE;
    }

    if( timeOut > 0 )
    {
        return TIMER_NOT_EXPIRE;
    }

    return TIMER_EXPIRE;
}

/**
 * @brief     Function For Update TimerOut
 * @detail    Unit : 1ms
 * @param     none
 * @return    none
**/
void UpdateTimer(void)
{
    U8  i = 0U;
    U8  j = 0U;

    for( i=0; i<TIMER_ID_MAX; i++ )
    {
        if( Timer[i].Enable == 0 )
        {
            continue;
        }

        if( Timer[i].TimeOut > 0 )
        {
            Timer[i].TimeOut--;
        }
    }

    for( j=0; j<TIMER_USER_ID_MAX; j++ )
    {
        if( UserTimer[j].Enable == 0 )
        {
            continue;
        }

        if( UserTimer[j].TimeOut > 0 )
        {
            UserTimer[j].TimeOut--;
        }
    }
}

/**
 * @brief                Function For User Timer Interrupt
 * @detail               none
 * @param     FPUserISR  User Interrupt Handler
 * @return               TIMER_EXPIRE /TIMER_NOT_EXPIRE / TIMER_DISABLE
**/
void RegisterTimerISR(void (*FPUserISR)(void))
{
    userISR = FPUserISR;
}

FPUserISR GetTimerISR(void)
{
    return userISR;
}
