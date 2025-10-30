#ifndef __DEICING_TABLE_H__
#define __DEICING_TABLE_H__

#include "prj_type.h"

/* DE-ICING REGION */
#define  DEICING_REGION_1   0
#define  DEICING_REGION_2   1
#define  DEICING_REGION_3   2
#define  DEICING_REGION_4   3
#define  DEICING_REGION_5   4
#define  DEICING_REGION_6   5
#define  DEICING_REGION_7   6
U8 GetDeIcingRegion( TEMP_T _tAmbi );

U8 GetDeIcingRepeatNum(U8 mu8Region );

U8 GetDeIcingCmd(U8 mu8Index);

U16 GetDeIcingTime(U8 mu8Region, U8 mu8Index);

U8 GetDeIcingCompRps(U8 mu8Region);

U8 GetDeIcingFanOnOff(U8 mu8Region);
#endif /* __DEICING_TABLE_H__ */
