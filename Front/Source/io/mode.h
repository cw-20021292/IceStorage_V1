#ifndef __MODE_H__
#define __MODE_H__

#include "prj_type.h"



#define MODE_NONE   0
#define MODE_FOTA   1

void InitMode(void);


void SetMode(U8 mu8Mode);
U8 GetMode(void);


void ProcessMode(void);


#endif
