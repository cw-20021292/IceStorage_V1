
#ifndef _WIFI_SetAccumulateData_H_
#define _WIFI_SetAccumulateData_H_

typedef enum
{
    WIFI_ACCM_0000_MONTH_WATER_USER = 0x0000,
    WIFI_ACCM_0001_MONTH_WATER_A,
    WIFI_ACCM_0002_MONTH_WATER_B,
    WIFI_ACCM_0003_MONTH_WATER_C,
    WIFI_ACCM_0004_DAY_WATER_USER,
    WIFI_ACCM_0005_DAY_WATER_A,
    WIFI_ACCM_0006_DAY_WATER_B,
    WIFI_ACCM_0007_DAY_WATER_C,

    WIFI_ACCM_0010_LAST_MONTH_WATT_KRW = 0x0010,
    
    WIFI_ACCUMULATE_NUM
}E_API_A10A0_T;

void* GetWifiAccumulateEventList ( void );
U16 GetWifiAccumulateListSize ( void );


#endif


