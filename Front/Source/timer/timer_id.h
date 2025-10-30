/**
 * File : timer_id.h
 * 
 * Process Sys Timer Id
**/
#ifndef __TIMER_ID_H__
#define __TIMER_ID_H__

typedef enum _timer_id_
{
    TIMER_ID_1MS,              // 0 :
    TIMER_ID_10MS,             // 1 :
    TIMER_ID_100MS,            // 2 :
    TIMER_ID_1SEC,             // 3 :
    TIMER_ID_10SEC,            // 4 :
    TIMER_ID_40SEC,            // 5 :
    TIMER_ID_1MIN,             // 6 :

    TIMER_ID_MAIN_TX_ERR,      // 7 : 
    
    TIMER_ID_SUB_FRONT,        // 8 : 
    TIMER_ID_SUB_FRONT_RX_ERR, // 9 :

    TIMER_ID_MAX,              // 10 :
} ETimerId_T;

typedef enum _timer_user_id_
{
    TIMER_USER_ID_COMM_MAIN_RX,    // 0 :
    TIMER_USER_ID_COMM_MAIN_TX,    // 1 :

    TIMER_USER_ID_COMM_FRONT_SUB_RX,
    TIMER_USER_ID_COMM_FRONT_SUB_TX,

    TIMER_USER_ID_MAX,             // 2 :
} ETimerUserId_T;

#endif  /* __TIMER_ID_H__ */
