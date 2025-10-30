#ifndef __ERR_COMP_BLDC_COMM_H__
#define __ERR_COMP_BLDC_COMM_H__

#include "prj_type.h"


void InitErrCompBldcComm(void);

void ResetCompBldcCommErrorCount(void);

// ERROR
U8 CheckErrCompBldcComm(U8 mu8Error);

// PROTECT
U8 CheckProtectCompBldcComm(U8 mu8Error);
U8 ReleaseProtectCompBldcComm(U8 mu8Error);

#endif /* __ERR_COMP_BLDC_COMM_H__ */
