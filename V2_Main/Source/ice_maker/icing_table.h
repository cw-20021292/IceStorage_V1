#ifndef __ICING_TABLE_H__
#define __ICING_TABLE_H__

#include "prj_type.h"


// 제빙 가능 온도 유효 범위 확인
U16 IsValidRangeTemp(TEMP_T tRoom, TEMP_T tAmbi );

// 제빙 시간 반환 ( 단위 : 초 )
U16 GetIcingTime(TEMP_T tRoom, TEMP_T tAmbi );

#endif /* __ICING_TABLE_H__ */
