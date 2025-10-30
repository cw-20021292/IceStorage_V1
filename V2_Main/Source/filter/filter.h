#ifndef __FILTER_H__
#define __FILTER_H__

#include "prj_type.h"


/* FILTER ID */
#define ID_FILTER_COVER    0
#define ID_FILTER          1
#define MAX_FILTER_NUM     2

typedef struct _filter_
{
    U8 Status[ MAX_FILTER_NUM ];
    U8 ChangedStatus;
    U8 ChangedFlushStatus;
    
    U8 UsageStatus;     // NORMAL or TIMEOUT
    U32 UsageTime;      // @ 1min
    U16 SaveTime;       // @ 1min
} Filter_T;


void InitFilter(void);

/* 필터 교체 후, 플러싱 종료 여부 */
#define FILTER_CHANGED_NONE     0   // none
#define FILTER_CHANGED_FLUSH    1   // 필터 교체 플러싱 완료
U8 GetFilterChangedFlushStatus(void);
void SetFilterChangedFlushStatus(U8 mu8Flush);

#define FILTER_MOUNTED      0
#define FILTER_CHANGED      1
U8 GetFilterChangedStatus(void);
void SetFilterChangedStatus(U8 mu8Changed);


/* FILTER STATUS - OPEN or CLOSE */
U8 IsOpenFilter(U8 mu8Id);

#define FILTER_OPEN     1
#define FILTER_CLOSE    0
void SetFilterStatus(U8 mu8Id, U8 mu8Status);
U8 GetFilterStatus(U8 mu8Id);

// 필터 사용 상태
#define FILTER_USAGE_NORMAL     0
#define FILTER_USAGE_TIME_OUT   1
void SetFilterUsageStatus(U8 mu8Status);
U8 GetFilterUsageStatus(void);

// 필터 사용 시간 
#define	FILTER_USAGE_TIME ( 172800UL )	/* 60min x 24hour x 120days = 172800 min */
void SetFilterUsageTime(U32 mu32Time);
U32 GetFilterUsageTime(void);

// 필터 리셋 ( 상태, 시간 )
void ResetFilterUsage(void);

void CountFilterUsage(void);


void CheckFilterStatus(void);

void ProcessFilter(void);

#endif /* __FILTER_H__ */
