#ifndef __PROCESS_MAKE_H__
#define __PROCESS_MAKE_H__

#include "prj_type.h"

void InitProcessMake(void);

void ProcessMake(void);

/* Status */
#if 0
typedef enum
{
    ICE_TURBO_NONE,
    ICE_TURBO_ONGOING,
	ICE_TURBO_PAUSE,
    ICE_TURBO_START,
} IceTurboStatus_T;

typedef struct _Ice_Turbo_mode_
{
    IceTurboStatus_T Status;          

    U16  IsCoolingContinueTime;
    U8  MakingIceCount;

} Ice_Turbo_T;
#endif


//U8 GetIceTurboStatus(void);
//void SetIceTurboStatus(U8 mu8status);


#endif /* __PROCESS_MAKE_H__ */
