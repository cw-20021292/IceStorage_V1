#ifndef __PROCESS_EOL_H__
#define __PROCESS_EOL_H__

#include "prj_type.h"

//void InitCheckIceSystem(void);
//
//void StartEolIceSystem(void);
//
//
//TEMP_T GetEolInitTempIce(void);
//
//TEMP_T GetEolInitTempCold(void);

U8 IsReadyEolInitMode(void);

U8 MovingIceTray(U8 mu8Step );

void ProcessEol(void);  // 1sec event...

#endif /* __PROCESS_EOL_H__ */
