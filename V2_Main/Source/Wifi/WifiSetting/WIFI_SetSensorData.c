
/*******************************************************************************
&Description :
    A1020 ~ A102x
    Sensor Table
  
&Histiry : 
    ShinHM 2021.04.14 update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_SetCommon.h"
#include "WIFI_TxForm.h"
#include "WIFI_SetSensorData.h"


/******************************************************************************/
/***** Static Function ********************************************************/
/******************************************************************************/
static void EvtWifiSensor ( U16 mId, I8 *pBuf );


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
#ifdef MODEL_NO_1
static const WifiTxFuncList_T WifiSensorList[] = 
{   /*  ID                                  Tx Function     Rx Function */
    {   WIFI_SENSOR_0001_COLD_TEMP,         EvtWifiSensor,  NULL    }, 
    {   WIFI_SENSOR_0002_HOT_TEMP,          NULL,           NULL    },
    {   WIFI_SENSOR_0003_WATER_TEMP,        EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_0004_OUT_TEMP,          EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_0005_LUX_DATA,          NULL,           NULL    },
    {   WIFI_SENSOR_0006_FILTER1_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0007_FILTER2_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0008_FILTER3_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0009_FILTER4_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000A_VOLTAGE_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000B_CURRENT_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000C_FACTOR_DATA,       NULL,           NULL    },
    {   WIFI_SENSOR_000D_RSSI_DATA,         NULL,           NULL    },
    {   WIFI_SENSOR_000E_WATT_DATA,         EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_000F_COMP_DATA,         EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_0010_TEM_TEMP,          NULL,           NULL    },
    {   WIFI_SENSOR_0011_WATT_KRW,          NULL,           NULL    },
    {   WIFI_SENSOR_0012_COLD_UP_TEMP,      NULL,           NULL    },
    {   WIFI_SENSOR_0013_COLD_DOWN_TEMP,    NULL,           NULL    }
};
#endif

#ifdef MODEL_NO_2
static const WifiTxFuncList_T WifiSensorList[] = 
{   /*  ID                                  Tx Function     Rx Function */
    {   WIFI_SENSOR_0001_COLD_TEMP,         EvtWifiSensor,  NULL    }, 
    {   WIFI_SENSOR_0002_HOT_TEMP,          NULL,           NULL    },
    {   WIFI_SENSOR_0003_WATER_TEMP,        NULL,           NULL    },
    {   WIFI_SENSOR_0004_OUT_TEMP,          EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_0005_LUX_DATA,          NULL,           NULL    },
    {   WIFI_SENSOR_0006_FILTER1_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0007_FILTER2_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0008_FILTER3_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0009_FILTER4_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000A_VOLTAGE_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000B_CURRENT_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000C_FACTOR_DATA,       NULL,           NULL    },
    {   WIFI_SENSOR_000D_RSSI_DATA,         NULL,           NULL    },
    {   WIFI_SENSOR_000E_WATT_DATA,         EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_000F_COMP_DATA,         EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_0010_TEM_TEMP,          NULL,           NULL    },
    {   WIFI_SENSOR_0011_WATT_KRW,          NULL,           NULL    },
    {   WIFI_SENSOR_0012_COLD_UP_TEMP,      NULL,           NULL    },
    {   WIFI_SENSOR_0013_COLD_DOWN_TEMP,    NULL,           NULL    }
};
#endif

#ifdef MODEL_NO_3
static const WifiTxFuncList_T WifiSensorList[] = 
{   /*  ID                                  Tx Function     Rx Function */
    {   WIFI_SENSOR_0001_COLD_TEMP,         EvtWifiSensor,  NULL    }, 
    {   WIFI_SENSOR_0002_HOT_TEMP,          NULL,           NULL    },
    {   WIFI_SENSOR_0003_WATER_TEMP,        NULL,           NULL    },
    {   WIFI_SENSOR_0004_OUT_TEMP,          EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_0005_LUX_DATA,          NULL,           NULL    },
    {   WIFI_SENSOR_0006_FILTER1_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0007_FILTER2_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0008_FILTER3_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0009_FILTER4_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000A_VOLTAGE_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000B_CURRENT_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000C_FACTOR_DATA,       NULL,           NULL    },
    {   WIFI_SENSOR_000D_RSSI_DATA,         NULL,           NULL    },
    {   WIFI_SENSOR_000E_WATT_DATA,         EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_000F_COMP_DATA,         NULL,           NULL    },
    {   WIFI_SENSOR_0010_TEM_TEMP,          EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_0011_WATT_KRW,          NULL,           NULL    },
    {   WIFI_SENSOR_0012_COLD_UP_TEMP,      NULL,           NULL    },
    {   WIFI_SENSOR_0013_COLD_DOWN_TEMP,    NULL,           NULL    }
};
#endif

#ifdef MODEL_NO_4
static const WifiTxFuncList_T WifiSensorList[] = 
{   /*  ID                                  Tx Function     Rx Function */
    {   WIFI_SENSOR_0001_COLD_TEMP,         EvtWifiSensor,  NULL    }, 
    {   WIFI_SENSOR_0002_HOT_TEMP,          NULL,           NULL    },
    {   WIFI_SENSOR_0003_WATER_TEMP,        NULL,           NULL    },
    {   WIFI_SENSOR_0004_OUT_TEMP,          EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_0005_LUX_DATA,          NULL,           NULL    },
    {   WIFI_SENSOR_0006_FILTER1_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0007_FILTER2_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0008_FILTER3_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_0009_FILTER4_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000A_VOLTAGE_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000B_CURRENT_DATA,      NULL,           NULL    },
    {   WIFI_SENSOR_000C_FACTOR_DATA,       NULL,           NULL    },
    {   WIFI_SENSOR_000D_RSSI_DATA,         NULL,           NULL    },
    {   WIFI_SENSOR_000E_WATT_DATA,         EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_000F_COMP_DATA,         NULL,           NULL    },
    {   WIFI_SENSOR_0010_TEM_TEMP,          EvtWifiSensor,  NULL    },
    {   WIFI_SENSOR_0011_WATT_KRW,          NULL,           NULL    },
    {   WIFI_SENSOR_0012_COLD_UP_TEMP,      NULL,           NULL    },
    {   WIFI_SENSOR_0013_COLD_DOWN_TEMP,    NULL,           NULL    }
};

#endif
    
#define SZ_SS_LIST ( sizeof(WifiSensorList) / sizeof(WifiTxFuncList_T) )


/******************************************************************************/
/***** Global Function ********************************************************/
/******************************************************************************/
void* GetWifiSensorEventList ( void )
{
    return WifiSensorList;
}

U16 GetWifiSensorListSize ( void )
{
    return SZ_SS_LIST;
}

// 소수데이터 예외처리
U8 IsPointData ( U8 mType, U16 mId )
{
    if ( mType == TYPE_SENSOR_CMD && mId == WIFI_SENSOR_000E_WATT_DATA )
    {
        return TRUE; // 소수
    }
    return FALSE; // 정수
}


/******************************************************************************/
/***** Local Function *********************************************************/
/******************************************************************************/
static void EvtWifiSensor ( U16 mId, I8 *pBuf )
{
    static I8 mai8Buf[LENGTH_WIFI_DATA_ONE];
    
    _MEMSET_( (void __FAR*) mai8Buf, '\0', sizeof(mai8Buf) );
    
    ConvertDataHexToAscii( TYPE_SENSOR_CMD, mId, mai8Buf );
    
    far_sprintf_s( pBuf, 0, mai8Buf );
}


