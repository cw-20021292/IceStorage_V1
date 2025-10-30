/**
 * File : process_sys_event.h
 * 
 * Process Time Scheduler Header
**/
#ifndef __PROCESS_SYS_EVENT__
#define __PROCESS_SYS_EVENT__

#include "prj_type.h"

void InitSystem(void);
void InitStartTimer(void);

void ProcessEventHandler(void);

#endif  /* __PROCESS_SYS_EVENT__ */
