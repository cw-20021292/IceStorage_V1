#ifndef __TIMER_H__
#define __TIMER_H__ 

#include "timer_id.h"

#define DELAY_MS(x) 
#define SEC( x )        ( ( x ) * 1000UL )


#define INTS_BYTE_BASE_TIMER()     unsigned char _ints_timer_byte
#define MASK_BASE_TIMER                       TMMK00
#define DISABLE_INT_MASK_BASE_TIMER()          do { MASK_BASE_TIMER = 1; }while(0)
#define ENABLE_INT_MASK_BASE_TIMER()           do { MASK_BASE_TIMER = 0; }while(0)

#define ENTER_CRITICAL_SECTION_TIMER()  \
    do{ \
        _ints_timer_byte = MASK_BASE_TIMER; \
        DISABLE_INT_MASK_BASE_TIMER(); \
    }while(0)

#define EXIT_CRITICAL_SECTION_TIMER()   \
    do{ \
        MASK_BASE_TIMER = _ints_timer_byte; \
    } while(0)


void InitTimer( void );

void StartTimer( U8 id, U32 time_out );

void DisableTimer( U8 id );

void StopTimer( U8 id );

#define TIMER_EXPIRE        0
#define TIMER_NOT_EXPIRE    1
#define TIMER_DISABLE       2
U8 IsExpiredTimer( U8 id );


/* 타이머 입터럽트 사용자 ISR 함수 등록 */
void RegisterTimerISR( void (*pUserISR)(void) );

#endif /* __TIMER_H__ */
