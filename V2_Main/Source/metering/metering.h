#ifndef __METERING__
#define __METERING__

#include "prj_type.h"


/* FILTER ID */
#define ID_METERING_12V    0
#define ID_METERING_24V    1
#define MAX_METERING_NUM   2


typedef double  Current_T;


typedef struct _metering_
{
    Current_T Current[ MAX_METERING_NUM ];
//    Current_T Peak[ MAX_METERING_NUM ];
//    Current_T RMS[ MAX_METERING_NUM ];
} Meter_T;

void InitMetering(void);

Current_T GetMeteringCurrent(U8 id);

void ProcessMetering(void);


#endif /* __METERING__ */
