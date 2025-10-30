#ifndef __CODY_KIT_H__
#define __CODY_KIT_H__

#include "prj_type.h"


#define CODY_STEP_ROOM  0
#define CODY_STEP_COLD  1
typedef struct _cody_kit_
{
    U8 Status;    // Cody mode ( true )
    U8 Step;
} CodyKit_T;

void InitCodyKit(void);

void SetCodyKitStatus(U8 mu8Status);
U8 GetCodyKitStatus(void);

void SetCodyKitStep(U8 mu8Step);
U8 GetCodyKitStep(void);


#endif /* __CODY_KIT_H__ */
