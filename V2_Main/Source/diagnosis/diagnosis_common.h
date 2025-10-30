#ifndef __DIAGNOSIS_COMMON_H__
#define __DIAGNOSIS_COMMON_H__

#include "prj_type.h"

typedef enum _dstatus_
{
    STATUS_GOING,
    STATUS_DONE,
    STATUS_ERR,
} DStatus_T;


typedef DStatus_T(*DiagnosisDo_T)(void);
typedef void(*DiagnosisInitial_T)(void);
typedef struct _diagnosis_event_
{
    U16                     Part;
    DiagnosisDo_T           DoAction;        //Do Function
    DiagnosisInitial_T      InitAction;      //Initial Function
} DiagnosisEventList_T;

#define STER_WAIT_LOAD_CLEAR     54000U   /*100msec 10 * 60 * 90 = 2hour*/
#define STER_WAIT_LOAD_OP_300MS  3U     /*100msec 10 * 3 = 300ms*/
#define STER_WAIT_LOAD_OP_1S     10U    /*100msec 10 * 1 = 1s*/
#define STER_WAIT_LOAD_OP_2S     20U    /*100msec 10 * 2 = 2s*/
#define STER_WAIT_LOAD_OP_3S     30U    /*100msec 10 * 3 = 3s*/
#define STER_WAIT_LOAD_OP_5S     50U    /*100msec 10 * 5 = 5s*/
#define STER_WAIT_LOAD_OP_180S   1800U   /*100msec 10 * 180 = 180s*/
#define STER_WAIT_LOAD_TRAY_OP   600U   /*100msec 10 * 60 = 60s*/


#endif  /* __DIAGNOSIS_COMMON_H__ */
