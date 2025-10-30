#ifndef __DUMMY_DEICING_TABLE_H__
#define __DUMMY_DEICING_TABLE_H__

#include "prj_type.h"

/* DE-ICING REGION */
#if 0
#define  DUMMY_DEICING_REGION_1   0
#define  DUMMY_DEICING_REGION_2   1
#define  DUMMY_DEICING_REGION_3   2
#define  DUMMY_DEICING_REGION_4   3
U8 GetDummyDeIcingRegion( TEMP_T _tAmbi );

U8 GetDummyDeIcingRepeatNum(U8 mu8Region );

U8 GetDummyDeIcingCmd(U8 mu8Index);

U16 GetDummyDeIcingTime(U8 mu8Region, U8 mu8Index);
#endif

/* 더미 탈빙 압축기 운전 시간
 *
 * @param : mu8Region - 외기 온도 영역
 * @return : 압축기 운전시간 ( sec )
 */
U16 GetDummyDeIcingCompOnTime(U8 mu8Region);

#endif /* __DUMMY_DEICING_TABLE_H__ */
