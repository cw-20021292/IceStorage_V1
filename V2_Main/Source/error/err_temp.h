#ifndef __ERR_TEMP_H__
#define __ERR_TEMP_H__

#include "prj_type.h"


#define STATUS_NORMAL      1
#define STATUS_OPEN        2 
#define STATUS_SHORT       3 
U8 GetTempErrorType(U16 mu16Adc);

#define COUNT_NUM          30    /* 3sec, @100ms */
U8 CheckErrorTemp(U8 mu8Error, U16 mu16Adc, U8 *pTimerBuf);

#endif /* __ERR_TEMP_H__ */
