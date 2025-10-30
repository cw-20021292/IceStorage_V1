#ifndef __LEVER_COMMON_HANDLER_H__
#define __LEVER_COMMON_HANDLER_H__

#include "prj_type.h"

typedef U8(*LeverFun_T)(void);
typedef struct _lever_event_
{
    LeverFun_T   actionEvent_1;         // short 
    LeverFun_T   actionEvent_2;         // long 2sec
    LeverFun_T   actionEvent_3;         // long 8sec
    LeverFun_T   actionEvent_4;         // pop
} LeverEventList_T;

#endif /* __LEVER_COMMON_HANDLER_H__ */
