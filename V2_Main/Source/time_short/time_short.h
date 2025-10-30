#ifndef __TIME_SHORT_H__
#define __TIME_SHORT_H__

#include "prj_type.h"

void InitTimeShort(void);

void StartTimeShort(void);
void StopTimeShort(void);
U8 IsExpiredTimeShortLimitTimer(void);
U8 GetTimeShortStatus(void);

void SetTimeShortTestTimer(U16 mu16Time);
U16 GetTimeShortTestTimer(void);


void UpdateTimeShortTimer(void); // @1sec
void ProcessTimeShort(void);

#endif /* __TIME_SHORT_H__ */
