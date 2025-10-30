    
/*******************************************************************************
&Description :
    A1080 ~ A108x
    A1090 ~ A109x
    Part Table
  
&Histiry : 
    ShinHM 2021.04.14 update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_SetCommon.h"
#include "WIFI_TxForm.h"
#include "WIFI_SetPartData.h"


/******************************************************************************/
/***** Static Function ********************************************************/
/******************************************************************************/
static void EvtWifiTxPart ( U16 mId, I8 *pBuf );


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
#ifdef MODEL_NO_1
static const WifiTxFuncList_T WifiPartList[] = 
{   /* PART LIST ID                                     Tx Function         Rx Function */
    {   WIFI_PART_0000_VALVE_FEED,                      NULL,               NULL    },
    {   WIFI_PART_0001_VALVE_NOS,                       EvtWifiTxPart,      NULL    },   
    {   WIFI_PART_0002_VALVE_HOT_OUT,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0003_VALVE_PURE_OUT,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0004_VALVE_COLD_OUT,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0005_VALVE_CP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0006_VALVE_CHP_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0007_VALVE_PURE_IN,                   NULL,               NULL    },
    {   WIFI_PART_0008_VALVE_HOT_AIR,                   NULL,               NULL    },
    {   WIFI_PART_0009_VALVE_HOT_IN,                    EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000A_VALVE_HOT_DRAIN,                 EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000B_VALVE_COLD_AIR,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000C_VALVE_COLD_IN,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000D_VALVE_COLD_DRAIN,                EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000E_VALVE_ICE_STORAGE_IN,            NULL,               NULL    },
    {   WIFI_PART_000F_VALVE_PRESSURE_IN,               NULL,               NULL    },        
    {   WIFI_PART_0010_VALVE_PRESSURE_DRAIN,            NULL,               NULL    },
    {   WIFI_PART_0011_VALVE_STERILIZE,                 NULL,               NULL    },
    {   WIFI_PART_0012_VALVE_DRAIN_NOS,                 NULL,               NULL    },
    {   WIFI_PART_0013_VALVE_ION_CHANGE,                NULL,               NULL    },
    {   WIFI_PART_0014_VALVE_INNO_CENCE,                NULL,               NULL    },
    {   WIFI_PART_0015_VALVE_ICE_TRAY_IN,               EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0016_VALVE_COLD_ROTATE,               NULL,               NULL    },
    {   WIFI_PART_0017_VALVE_EW_IN,                     NULL,               NULL    },
    {   WIFI_PART_0018_VALVE_FLUSHING,                  NULL,               NULL    },
   
    {   WIFI_PART_0100_LEVEL_COLD_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0101_LEVEL_COLD_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0102_LEVEL_COLD_MID,                  NULL,               NULL    },
    {   WIFI_PART_0103_LEVEL_COLD_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0104_LEVEL_ICE_STORAGE_HIGH,          NULL,               NULL    },
    {   WIFI_PART_0105_LEVEL_PURE_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0106_LEVEL_PURE_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0107_LEVEL_PURE_MID,                  NULL,               NULL    },
    {   WIFI_PART_0108_LEVEL_PURE_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0109_LEVEL_HOT_HIGH,                  NULL,               NULL    },
    {   WIFI_PART_010A_LEVEL_DRAIN_OVER,                NULL,               NULL    },
    {   WIFI_PART_010B_LEVEL_DRAIN_HIGH,                EvtWifiTxPart,      NULL    },
    {   WIFI_PART_010C_LEVEL_DRAIN_MID,                 NULL,               NULL    },
    {   WIFI_PART_010D_LEVEL_DRAIN_LOW,                 EvtWifiTxPart,      NULL    },

    {   WIFI_PART_0200_COLD_COMP,                       NULL,               NULL    },
    {   WIFI_PART_0201_COLD_BLDC,                       EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0202_COLD_TEM,                        NULL,               NULL    },
    {   WIFI_PART_0203_COLD_TEM_TEMP,                   NULL,               NULL    },
    {   WIFI_PART_0204_COLD_FAN,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0205_COLD_STIRRING_MOTOR,             NULL,               NULL    },
    {   WIFI_PART_0206_COLD_TEMP_1,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0207_COLD_TEMP_2,                     NULL,               NULL    },
    {   WIFI_PART_0208_COLD_ROOM_TEMP,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0209_COLD_ROTATE_PUMP,                NULL,               NULL    },
        
    {   WIFI_PART_0300_HOT_TANK_HEATER,                 NULL,               NULL    },
    {   WIFI_PART_0301_HOT_TEMP,                        NULL,               NULL    },
    {   WIFI_PART_0302_HOT_INSTANT_HEATER_1,            EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0303_HOT_INSTANT_HEATER_2,            EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0304_HOT_FLOW_MOTOR,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0305_HOT_TEMP_IN,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0306_HOT_TEMP_OUT,                    EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0307_HOT_HEATER_TEMP,                 EvtWifiTxPart,      NULL    },
        
    {   WIFI_PART_0400_SENSOR_PRESSURE_1,               NULL,               NULL    },
    {   WIFI_PART_0401_SENSOR_PRESSURE_2,               NULL,               NULL    },
    {   WIFI_PART_0402_SENSOR_LEAK,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0403_SENSOR_FLOW,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0404_SENSOR_FLOW_REED,                NULL,               NULL    },
    {   WIFI_PART_0405_SENSOR_FILTER_REED,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0406_SENSOR_FILTER_SW_1,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0407_SENSOR_FILTER_SW_2,              NULL,               NULL    },
    {   WIFI_PART_0408_SENSOR_FILTER_SW_3,              NULL,               NULL    },
    {   WIFI_PART_0409_SENSOR_TANK_REED,                EvtWifiTxPart,      NULL    },
    {   WIFI_PART_040A_SENSOR_LIGHT,                    NULL,               NULL    },
    {   WIFI_PART_040B_SENSOR_WATER_COURSE_REED,        NULL,               NULL    },

    {   WIFI_PART_0500_STER_CSI_MODULE,                 NULL,               NULL    },
    {   WIFI_PART_0501_STER_UV_FAUCET,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0502_STER_UV_FAUCET_ICE,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0503_STER_UV_ICE_TANK,                EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0504_STER_UV_ICE_TRAY,                EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0505_STER_UV_COLD_TANK,               NULL,               NULL    },

    {   WIFI_PART_0600_TDS_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0601_TDS_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0602_TDS_SENSOR_IN,                   NULL,               NULL    },
    {   WIFI_PART_0603_TDS_SENSOR_OUT,                  NULL,               NULL    },

    {   WIFI_PART_0700_FAUCET_REED_1,                   NULL,               NULL    },
    {   WIFI_PART_0701_FAUCET_REED_2,                   NULL,               NULL    },
    {   WIFI_PART_0702_FAUCET_CUP_SENSOR,               NULL,               NULL    },
    {   WIFI_PART_0703_FAUCET_MOTOR,                    NULL,               NULL    },

    {   WIFI_PART_0800_ICE_PURE_TEMP,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0801_ICE_FULL_SENSOR,                 EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0802_ICE_SWING_BAR,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0803_ICE_TRAY_AC,                     NULL,               NULL    },
    {   WIFI_PART_0804_ICE_TRAY_STEPMOTOR,              NULL,               NULL    },
    {   WIFI_PART_0805_ICE_TRAY_SENSING_SW,             EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0806_ICE_MOTOR_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0807_ICE_DOOR_STEPMOTOR,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0808_ICE_COURSE_CHANGE_VV,            EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0809_ICE_HOT_GAS_VV,                  NULL,               NULL    },
    {   WIFI_PART_080A_ICE_DRAIN_PUMP,                  EvtWifiTxPart,      NULL    }
};

static const WifiTxFuncList_T WifiPartListCp[] = 
{
    {   WIFI_PART_0000_VALVE_FEED,                      NULL,               NULL    },
    {   WIFI_PART_0001_VALVE_NOS,                       EvtWifiTxPart,      NULL    },   
    {   WIFI_PART_0002_VALVE_HOT_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0003_VALVE_PURE_OUT,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0004_VALVE_COLD_OUT,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0005_VALVE_CP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0006_VALVE_CHP_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0007_VALVE_PURE_IN,                   NULL,               NULL    },
    {   WIFI_PART_0008_VALVE_HOT_AIR,                   NULL,               NULL    },
    {   WIFI_PART_0009_VALVE_HOT_IN,                    NULL,               NULL    },
    {   WIFI_PART_000A_VALVE_HOT_DRAIN,                 NULL,               NULL    },
    {   WIFI_PART_000B_VALVE_COLD_AIR,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000C_VALVE_COLD_IN,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000D_VALVE_COLD_DRAIN,                EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000E_VALVE_ICE_STORAGE_IN,            NULL,               NULL    },
    {   WIFI_PART_000F_VALVE_PRESSURE_IN,               NULL,               NULL    },        
    {   WIFI_PART_0010_VALVE_PRESSURE_DRAIN,            NULL,               NULL    },
    {   WIFI_PART_0011_VALVE_STERILIZE,                 NULL,               NULL    },
    {   WIFI_PART_0012_VALVE_DRAIN_NOS,                 NULL,               NULL    },
    {   WIFI_PART_0013_VALVE_ION_CHANGE,                NULL,               NULL    },
    {   WIFI_PART_0014_VALVE_INNO_CENCE,                NULL,               NULL    },
    {   WIFI_PART_0015_VALVE_ICE_TRAY_IN,               EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0016_VALVE_COLD_ROTATE,               NULL,               NULL    },
    {   WIFI_PART_0017_VALVE_EW_IN,                     NULL,               NULL    },
    {   WIFI_PART_0018_VALVE_FLUSHING,                  EvtWifiTxPart,      NULL    },
   
    {   WIFI_PART_0100_LEVEL_COLD_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0101_LEVEL_COLD_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0102_LEVEL_COLD_MID,                  NULL,               NULL    },
    {   WIFI_PART_0103_LEVEL_COLD_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0104_LEVEL_ICE_STORAGE_HIGH,          NULL,               NULL    },
    {   WIFI_PART_0105_LEVEL_PURE_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0106_LEVEL_PURE_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0107_LEVEL_PURE_MID,                  NULL,               NULL    },
    {   WIFI_PART_0108_LEVEL_PURE_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0109_LEVEL_HOT_HIGH,                  NULL,               NULL    },
    {   WIFI_PART_010A_LEVEL_DRAIN_OVER,                NULL,               NULL    },
    {   WIFI_PART_010B_LEVEL_DRAIN_HIGH,                EvtWifiTxPart,      NULL    },
    {   WIFI_PART_010C_LEVEL_DRAIN_MID,                 NULL,               NULL    },
    {   WIFI_PART_010D_LEVEL_DRAIN_LOW,                 EvtWifiTxPart,      NULL    },

    {   WIFI_PART_0200_COLD_COMP,                       NULL,               NULL    },
    {   WIFI_PART_0201_COLD_BLDC,                       EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0202_COLD_TEM,                        NULL,               NULL    },
    {   WIFI_PART_0203_COLD_TEM_TEMP,                   NULL,               NULL    },
    {   WIFI_PART_0204_COLD_FAN,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0205_COLD_STIRRING_MOTOR,             NULL,               NULL    },
    {   WIFI_PART_0206_COLD_TEMP_1,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0207_COLD_TEMP_2,                     NULL,               NULL    },
    {   WIFI_PART_0208_COLD_ROOM_TEMP,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0209_COLD_ROTATE_PUMP,                NULL,               NULL    },
        
    {   WIFI_PART_0300_HOT_TANK_HEATER,                 NULL,               NULL    },
    {   WIFI_PART_0301_HOT_TEMP,                        NULL,               NULL    },
    {   WIFI_PART_0302_HOT_INSTANT_HEATER_1,            NULL,               NULL    },
    {   WIFI_PART_0303_HOT_INSTANT_HEATER_2,            NULL,               NULL    },
    {   WIFI_PART_0304_HOT_FLOW_MOTOR,                  NULL,               NULL    },
    {   WIFI_PART_0305_HOT_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0306_HOT_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0307_HOT_HEATER_TEMP,                 NULL,               NULL    },
        
    {   WIFI_PART_0400_SENSOR_PRESSURE_1,               NULL,               NULL    },
    {   WIFI_PART_0401_SENSOR_PRESSURE_2,               NULL,               NULL    },
    {   WIFI_PART_0402_SENSOR_LEAK,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0403_SENSOR_FLOW,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0404_SENSOR_FLOW_REED,                NULL,               NULL    },
    {   WIFI_PART_0405_SENSOR_FILTER_REED,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0406_SENSOR_FILTER_SW_1,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0407_SENSOR_FILTER_SW_2,              NULL,               NULL    },
    {   WIFI_PART_0408_SENSOR_FILTER_SW_3,              NULL,               NULL    },
    {   WIFI_PART_0409_SENSOR_TANK_REED,                EvtWifiTxPart,      NULL    },
    {   WIFI_PART_040A_SENSOR_LIGHT,                    NULL,               NULL    },
    {   WIFI_PART_040B_SENSOR_WATER_COURSE_REED,        NULL,               NULL    },

    {   WIFI_PART_0500_STER_CSI_MODULE,                 NULL,               NULL    },
    {   WIFI_PART_0501_STER_UV_FAUCET,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0502_STER_UV_FAUCET_ICE,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0503_STER_UV_ICE_TANK,                EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0504_STER_UV_ICE_TRAY,                EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0505_STER_UV_COLD_TANK,               NULL,               NULL    },

    {   WIFI_PART_0600_TDS_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0601_TDS_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0602_TDS_SENSOR_IN,                   NULL,               NULL    },
    {   WIFI_PART_0603_TDS_SENSOR_OUT,                  NULL,               NULL    },

    {   WIFI_PART_0700_FAUCET_REED_1,                   NULL,               NULL    },
    {   WIFI_PART_0701_FAUCET_REED_2,                   NULL,               NULL    },
    {   WIFI_PART_0702_FAUCET_CUP_SENSOR,               NULL,               NULL    },
    {   WIFI_PART_0703_FAUCET_MOTOR,                    NULL,               NULL    },

    {   WIFI_PART_0800_ICE_PURE_TEMP,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0801_ICE_FULL_SENSOR,                 EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0802_ICE_SWING_BAR,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0803_ICE_TRAY_AC,                     NULL,               NULL    },
    {   WIFI_PART_0804_ICE_TRAY_STEPMOTOR,              NULL,               NULL    },
    {   WIFI_PART_0805_ICE_TRAY_SENSING_SW,             EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0806_ICE_MOTOR_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0807_ICE_DOOR_STEPMOTOR,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0808_ICE_COURSE_CHANGE_VV,            EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0809_ICE_HOT_GAS_VV,                  NULL,               NULL    },
    {   WIFI_PART_080A_ICE_DRAIN_PUMP,                  EvtWifiTxPart,      NULL    }
};
#endif

#ifdef MODEL_NO_2
static const WifiTxFuncList_T WifiPartList[] = 
{   /* PART LIST ID                                     Tx Function         Rx Function */
    {   WIFI_PART_0000_VALVE_FEED,                      NULL,               NULL    },
    {   WIFI_PART_0001_VALVE_NOS,                       EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0002_VALVE_HOT_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0003_VALVE_PURE_OUT,                  NULL,               NULL    },
    {   WIFI_PART_0004_VALVE_COLD_OUT,                  NULL,               NULL    },
    {   WIFI_PART_0005_VALVE_CP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0006_VALVE_CHP_OUT,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0007_VALVE_PURE_IN,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0008_VALVE_HOT_AIR,                   NULL,               NULL    },
    {   WIFI_PART_0009_VALVE_HOT_IN,                    EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000A_VALVE_HOT_DRAIN,                 NULL,               NULL    },
    {   WIFI_PART_000B_VALVE_COLD_AIR,                  NULL,               NULL    },
    {   WIFI_PART_000C_VALVE_COLD_IN,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000D_VALVE_COLD_DRAIN,                NULL,               NULL    },
    {   WIFI_PART_000E_VALVE_ICE_STORAGE_IN,            NULL,               NULL    },
    {   WIFI_PART_000F_VALVE_PRESSURE_IN,               NULL,               NULL    },
        
    {   WIFI_PART_0010_VALVE_PRESSURE_DRAIN,            NULL,               NULL    },
    {   WIFI_PART_0011_VALVE_STERILIZE,                 EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0012_VALVE_DRAIN_NOS,                 EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0013_VALVE_ION_CHANGE,                NULL,               NULL    },
    {   WIFI_PART_0014_VALVE_INNO_CENCE,                NULL,               NULL    },
    {   WIFI_PART_0015_VALVE_ICE_TRAY_IN,               NULL,               NULL    },
   
    {   WIFI_PART_0100_LEVEL_COLD_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0101_LEVEL_COLD_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0102_LEVEL_COLD_MID,                  NULL,               NULL    },
    {   WIFI_PART_0103_LEVEL_COLD_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0104_LEVEL_ICE_STORAGE_HIGH,          EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0105_LEVEL_PURE_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0106_LEVEL_PURE_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0107_LEVEL_PURE_MID,                  NULL,               NULL    },
    {   WIFI_PART_0108_LEVEL_PURE_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0109_LEVEL_HOT_HIGH,                  NULL,               NULL    },

    {   WIFI_PART_0200_COLD_COMP,                       NULL,               NULL    },
    {   WIFI_PART_0201_COLD_BLDC,                       EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0202_COLD_TEM,                        NULL,               NULL    },
    {   WIFI_PART_0203_COLD_TEM_TEMP,                   NULL,               NULL    },
    {   WIFI_PART_0204_COLD_FAN,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0205_COLD_STIRRING_MOTOR,             EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0206_COLD_TEMP_1,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0207_COLD_TEMP_2,                     NULL,               NULL    },
    {   WIFI_PART_0208_COLD_ROOM_TEMP,                  EvtWifiTxPart,      NULL    },
        
    {   WIFI_PART_0300_HOT_TANK_HEATER,                 NULL,               NULL    },
    {   WIFI_PART_0301_HOT_TEMP,                        NULL,               NULL    },
    {   WIFI_PART_0302_HOT_INSTANT_HEATER_1,            EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0303_HOT_INSTANT_HEATER_2,            EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0304_HOT_FLOW_MOTOR,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0305_HOT_TEMP_IN,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0306_HOT_TEMP_OUT,                    EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0307_HOT_HEATER_TEMP,                 NULL,               NULL    },
        
    {   WIFI_PART_0400_SENSOR_PRESSURE_1,               NULL,               NULL    },
    {   WIFI_PART_0401_SENSOR_PRESSURE_2,               NULL,               NULL    },
    {   WIFI_PART_0402_SENSOR_LEAK,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0403_SENSOR_FLOW,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0404_SENSOR_FLOW_REED,                NULL,               NULL    },
    {   WIFI_PART_0405_SENSOR_FILTER_REED,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0406_SENSOR_FILTER_SW_1,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0407_SENSOR_FILTER_SW_2,              NULL,               NULL    },
    {   WIFI_PART_0408_SENSOR_FILTER_SW_3,              NULL,               NULL    },

    {   WIFI_PART_0500_STER_CSI_MODULE,                 EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0501_STER_UV_FAUCET,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0502_STER_UV_FAUCET_ICE,              NULL,               NULL    },
    {   WIFI_PART_0503_STER_UV_ICE_TANK,                NULL,               NULL    },
    {   WIFI_PART_0504_STER_UV_ICE_TRAY,                NULL,               NULL    },
    {   WIFI_PART_0505_STER_UV_COLD_TANK,               NULL,               NULL    },

    {   WIFI_PART_0600_TDS_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0601_TDS_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0602_TDS_SENSOR_IN,                   NULL,               NULL    },
    {   WIFI_PART_0603_TDS_SENSOR_OUT,                  NULL,               NULL    },

    {   WIFI_PART_0700_FAUCET_REED_1,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0701_FAUCET_REED_2,                   NULL,               NULL    },
    {   WIFI_PART_0702_FAUCET_CUP_SENSOR,               EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0703_FAUCET_MOTOR,                    NULL,               NULL    }
};
    
static const WifiTxFuncList_T WifiPartListCp[] = 
{   /* PART LIST ID                                     Tx Function         Rx Function */
    {   WIFI_PART_0000_VALVE_FEED,                      NULL,               NULL    },
    {   WIFI_PART_0001_VALVE_NOS,                       EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0002_VALVE_HOT_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0003_VALVE_PURE_OUT,                  NULL,               NULL    },
    {   WIFI_PART_0004_VALVE_COLD_OUT,                  NULL,               NULL    },
    {   WIFI_PART_0005_VALVE_CP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0006_VALVE_CHP_OUT,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0007_VALVE_PURE_IN,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0008_VALVE_HOT_AIR,                   NULL,               NULL    },
    {   WIFI_PART_0009_VALVE_HOT_IN,                    NULL,               NULL    },
    {   WIFI_PART_000A_VALVE_HOT_DRAIN,                 NULL,               NULL    },
    {   WIFI_PART_000B_VALVE_COLD_AIR,                  NULL,               NULL    },
    {   WIFI_PART_000C_VALVE_COLD_IN,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000D_VALVE_COLD_DRAIN,                NULL,               NULL    },
    {   WIFI_PART_000E_VALVE_ICE_STORAGE_IN,            NULL,               NULL    },
    {   WIFI_PART_000F_VALVE_PRESSURE_IN,               NULL,               NULL    },
        
    {   WIFI_PART_0010_VALVE_PRESSURE_DRAIN,            NULL,               NULL    },
    {   WIFI_PART_0011_VALVE_STERILIZE,                 EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0012_VALVE_DRAIN_NOS,                 EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0013_VALVE_ION_CHANGE,                NULL,               NULL    },
    {   WIFI_PART_0014_VALVE_INNO_CENCE,                NULL,               NULL    },
    {   WIFI_PART_0015_VALVE_ICE_TRAY_IN,               NULL,               NULL    },
   
    {   WIFI_PART_0100_LEVEL_COLD_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0101_LEVEL_COLD_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0102_LEVEL_COLD_MID,                  NULL,               NULL    },
    {   WIFI_PART_0103_LEVEL_COLD_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0104_LEVEL_ICE_STORAGE_HIGH,          EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0105_LEVEL_PURE_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0106_LEVEL_PURE_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0107_LEVEL_PURE_MID,                  NULL,               NULL    },
    {   WIFI_PART_0108_LEVEL_PURE_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0109_LEVEL_HOT_HIGH,                  NULL,               NULL    },

    {   WIFI_PART_0200_COLD_COMP,                       NULL,               NULL    },
    {   WIFI_PART_0201_COLD_BLDC,                       EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0202_COLD_TEM,                        NULL,               NULL    },
    {   WIFI_PART_0203_COLD_TEM_TEMP,                   NULL,               NULL    },
    {   WIFI_PART_0204_COLD_FAN,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0205_COLD_STIRRING_MOTOR,             EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0206_COLD_TEMP_1,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0207_COLD_TEMP_2,                     NULL,               NULL    },
    {   WIFI_PART_0208_COLD_ROOM_TEMP,                  EvtWifiTxPart,      NULL    },
        
    {   WIFI_PART_0300_HOT_TANK_HEATER,                 NULL,               NULL    },
    {   WIFI_PART_0301_HOT_TEMP,                        NULL,               NULL    },
    {   WIFI_PART_0302_HOT_INSTANT_HEATER_1,            NULL,               NULL    },
    {   WIFI_PART_0303_HOT_INSTANT_HEATER_2,            NULL,               NULL    },
    {   WIFI_PART_0304_HOT_FLOW_MOTOR,                  NULL,               NULL    },
    {   WIFI_PART_0305_HOT_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0306_HOT_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0307_HOT_HEATER_TEMP,                 NULL,               NULL    },
        
    {   WIFI_PART_0400_SENSOR_PRESSURE_1,               NULL,               NULL    },
    {   WIFI_PART_0401_SENSOR_PRESSURE_2,               NULL,               NULL    },
    {   WIFI_PART_0402_SENSOR_LEAK,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0403_SENSOR_FLOW,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0404_SENSOR_FLOW_REED,                NULL,               NULL    },
    {   WIFI_PART_0405_SENSOR_FILTER_REED,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0406_SENSOR_FILTER_SW_1,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0407_SENSOR_FILTER_SW_2,              NULL,               NULL    },
    {   WIFI_PART_0408_SENSOR_FILTER_SW_3,              NULL,               NULL    },

    {   WIFI_PART_0500_STER_CSI_MODULE,                 EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0501_STER_UV_FAUCET,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0502_STER_UV_FAUCET_ICE,              NULL,               NULL    },
    {   WIFI_PART_0503_STER_UV_ICE_TANK,                NULL,               NULL    },
    {   WIFI_PART_0504_STER_UV_ICE_TRAY,                NULL,               NULL    },
    {   WIFI_PART_0505_STER_UV_COLD_TANK,               NULL,               NULL    },

    {   WIFI_PART_0600_TDS_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0601_TDS_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0602_TDS_SENSOR_IN,                   NULL,               NULL    },
    {   WIFI_PART_0603_TDS_SENSOR_OUT,                  NULL,               NULL    },

    {   WIFI_PART_0700_FAUCET_REED_1,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0701_FAUCET_REED_2,                   NULL,               NULL    },
    {   WIFI_PART_0702_FAUCET_CUP_SENSOR,               EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0703_FAUCET_MOTOR,                    NULL,               NULL    }
};
#endif

#ifdef MODEL_NO_3
static const WifiTxFuncList_T WifiPartList[] = 
{
    {   WIFI_PART_0000_VALVE_FEED,                      NULL,               NULL    },
    {   WIFI_PART_0001_VALVE_NOS,                       EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0002_VALVE_HOT_OUT,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0003_VALVE_PURE_OUT,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0004_VALVE_COLD_OUT,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0005_VALVE_CP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0006_VALVE_CHP_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0007_VALVE_PURE_IN,                   NULL,               NULL    },
    {   WIFI_PART_0008_VALVE_HOT_AIR,                   NULL,               NULL    },
    {   WIFI_PART_0009_VALVE_HOT_IN,                    EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000A_VALVE_HOT_DRAIN,                 NULL,               NULL    }, // 삭제 _ icon 2.0 온수배수밸브 없음._ssg
    {   WIFI_PART_000B_VALVE_COLD_AIR,                  NULL,               NULL    },
    {   WIFI_PART_000C_VALVE_COLD_IN,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000D_VALVE_COLD_DRAIN,                NULL,               NULL    },
    {   WIFI_PART_000E_VALVE_ICE_STORAGE_IN,            NULL,               NULL    },
    {   WIFI_PART_000F_VALVE_PRESSURE_IN,               NULL,               NULL    },
    {   WIFI_PART_0010_VALVE_PRESSURE_DRAIN,            NULL,               NULL    },
    {   WIFI_PART_0011_VALVE_STERILIZE,                 NULL,               NULL    },
    {   WIFI_PART_0012_VALVE_DRAIN_NOS,                 NULL,               NULL    },
    {   WIFI_PART_0013_VALVE_ION_CHANGE,                NULL,               NULL    },
    {   WIFI_PART_0014_VALVE_INNO_CENCE,                NULL,               NULL    },
    {   WIFI_PART_0015_VALVE_ICE_TRAY_IN,               NULL,               NULL    },
    {   WIFI_PART_0016_VALVE_COLD_ROTATE,               NULL,               NULL    },
    {   WIFI_PART_0017_VALVE_EW_IN,                     NULL,               NULL    },    
   
    {   WIFI_PART_0100_LEVEL_COLD_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0101_LEVEL_COLD_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0102_LEVEL_COLD_MID,                  NULL,               NULL    },
    {   WIFI_PART_0103_LEVEL_COLD_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0104_LEVEL_ICE_STORAGE_HIGH,          NULL,               NULL    },
    {   WIFI_PART_0105_LEVEL_PURE_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0106_LEVEL_PURE_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0107_LEVEL_PURE_MID,                  NULL,               NULL    },
    {   WIFI_PART_0108_LEVEL_PURE_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0109_LEVEL_HOT_HIGH,                  NULL,               NULL    },
    {   WIFI_PART_010A_LEVEL_DRAIN_OVER,                NULL,               NULL    },
    {   WIFI_PART_010B_LEVEL_DRAIN_HIGH,                NULL,               NULL    },
    {   WIFI_PART_010C_LEVEL_DRAIN_MID,                 NULL,               NULL    },
    {   WIFI_PART_010D_LEVEL_DRAIN_LOW,                 NULL,               NULL    },
    
    {   WIFI_PART_0200_COLD_COMP,                       NULL,               NULL    },
    {   WIFI_PART_0201_COLD_BLDC,                       NULL,               NULL    },
    {   WIFI_PART_0202_COLD_TEM,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0203_COLD_TEM_TEMP,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0204_COLD_FAN,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0205_COLD_STIRRING_MOTOR,             NULL,               NULL    },
    {   WIFI_PART_0206_COLD_TEMP_1,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0207_COLD_TEMP_2,                     NULL,               NULL    },
    {   WIFI_PART_0208_COLD_ROOM_TEMP,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0209_COLD_ROTATE_PUMP,                NULL,               NULL    },
        
    {   WIFI_PART_0300_HOT_TANK_HEATER,                 NULL,               NULL    },
    {   WIFI_PART_0301_HOT_TEMP,                        NULL,               NULL    },
    {   WIFI_PART_0302_HOT_INSTANT_HEATER_1,            EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0303_HOT_INSTANT_HEATER_2,            EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0304_HOT_FLOW_MOTOR,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0305_HOT_TEMP_IN,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0306_HOT_TEMP_OUT,                    EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0307_HOT_HEATER_TEMP,                 EvtWifiTxPart,      NULL    },
        
    {   WIFI_PART_0400_SENSOR_PRESSURE_1,               NULL,               NULL    },
    {   WIFI_PART_0401_SENSOR_PRESSURE_2,               NULL,               NULL    },
    {   WIFI_PART_0402_SENSOR_LEAK,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0403_SENSOR_FLOW,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0404_SENSOR_FLOW_REED,                NULL,               NULL    },
    {   WIFI_PART_0405_SENSOR_FILTER_REED,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0406_SENSOR_FILTER_SW_1,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0407_SENSOR_FILTER_SW_2,              NULL,               NULL    },
    {   WIFI_PART_0408_SENSOR_FILTER_SW_3,              NULL,               NULL    },
    {   WIFI_PART_0409_SENSOR_TANK_REED,                NULL,               NULL    },
    {   WIFI_PART_040A_SENSOR_LIGHT,                    NULL,               NULL    },
    {   WIFI_PART_040B_SENSOR_WATER_COURSE_REED,        EvtWifiTxPart,      NULL    },
        
    {   WIFI_PART_0500_STER_CSI_MODULE,                 NULL,               NULL    },
    {   WIFI_PART_0501_STER_UV_FAUCET,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0502_STER_UV_FAUCET_ICE,              NULL,               NULL    },
    {   WIFI_PART_0503_STER_UV_ICE_TANK,                NULL,               NULL    },
    {   WIFI_PART_0504_STER_UV_ICE_TRAY,                NULL,               NULL    },
    {   WIFI_PART_0505_STER_UV_COLD_TANK,               NULL,               NULL    },

    {   WIFI_PART_0600_TDS_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0601_TDS_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0602_TDS_SENSOR_IN,                   NULL,               NULL    },
    {   WIFI_PART_0603_TDS_SENSOR_OUT,                  NULL,               NULL    },

    {   WIFI_PART_0700_FAUCET_REED_1,                   NULL,               NULL    },
    {   WIFI_PART_0701_FAUCET_REED_2,                   NULL,               NULL    },
    {   WIFI_PART_0702_FAUCET_CUP_SENSOR,               NULL,               NULL    },
    {   WIFI_PART_0703_FAUCET_MOTOR,                    NULL,               NULL    },

    {   WIFI_PART_0800_ICE_PURE_TEMP,                   NULL,               NULL    },
    {   WIFI_PART_0801_ICE_TRAY_TEMP_IN,                NULL,               NULL    },
    {   WIFI_PART_0802_ICE_FULL_SENSOR,                 NULL,               NULL    },
    {   WIFI_PART_0803_ICE_SWING_BAR,                   NULL,               NULL    },
    {   WIFI_PART_0804_ICE_TRAY_AC,                     NULL,               NULL    },
    {   WIFI_PART_0805_ICE_TRAY_STEPMOTOR,              NULL,               NULL    },
    {   WIFI_PART_0806_ICE_TRAY_SENSING_SW,             NULL,               NULL    },
    {   WIFI_PART_0807_DEICE_TRAY_SENSING_SW,           NULL,               NULL    },
    {   WIFI_PART_0808_ICE_MOTOR_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0809_ICE_DOOR_STEPMOTOR,              NULL,               NULL    },
    {   WIFI_PART_080A_ICE_COURSE_CHANGE_VV,            NULL,               NULL    },
    {   WIFI_PART_080B_ICE_HOT_GAS_VV,                  NULL,               NULL    },
    {   WIFI_PART_080C_ICE_DRAIN_PUMP,                  NULL,               NULL    }    
};

static const WifiTxFuncList_T WifiPartListCp[] = 
{   /* PART LIST ID                                     Tx Function         Rx Function */
    {   WIFI_PART_0000_VALVE_FEED,                      NULL,               NULL    },
    {   WIFI_PART_0001_VALVE_NOS,                       EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0002_VALVE_HOT_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0003_VALVE_PURE_OUT,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0004_VALVE_COLD_OUT,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0005_VALVE_CP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0006_VALVE_CHP_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0007_VALVE_PURE_IN,                   NULL,               NULL    },
    {   WIFI_PART_0008_VALVE_HOT_AIR,                   NULL,               NULL    },
    {   WIFI_PART_0009_VALVE_HOT_IN,                    NULL,               NULL    },
    {   WIFI_PART_000A_VALVE_HOT_DRAIN,                 NULL,               NULL    },
    {   WIFI_PART_000B_VALVE_COLD_AIR,                  NULL,               NULL    },
    {   WIFI_PART_000C_VALVE_COLD_IN,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000D_VALVE_COLD_DRAIN,                NULL,               NULL    },
    {   WIFI_PART_000E_VALVE_ICE_STORAGE_IN,            NULL,               NULL    },
    {   WIFI_PART_000F_VALVE_PRESSURE_IN,               NULL,               NULL    },
    {   WIFI_PART_0010_VALVE_PRESSURE_DRAIN,            NULL,               NULL    },
    {   WIFI_PART_0011_VALVE_STERILIZE,                 NULL,               NULL    },
    {   WIFI_PART_0012_VALVE_DRAIN_NOS,                 NULL,               NULL    },
    {   WIFI_PART_0013_VALVE_ION_CHANGE,                NULL,               NULL    },
    {   WIFI_PART_0014_VALVE_INNO_CENCE,                NULL,               NULL    },
    {   WIFI_PART_0015_VALVE_ICE_TRAY_IN,               NULL,               NULL    },
    {   WIFI_PART_0016_VALVE_COLD_ROTATE,               NULL,               NULL    },
    {   WIFI_PART_0017_VALVE_EW_IN,                     NULL,               NULL    },    
   
    {   WIFI_PART_0100_LEVEL_COLD_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0101_LEVEL_COLD_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0102_LEVEL_COLD_MID,                  NULL,               NULL    },
    {   WIFI_PART_0103_LEVEL_COLD_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0104_LEVEL_ICE_STORAGE_HIGH,          NULL,               NULL    },
    {   WIFI_PART_0105_LEVEL_PURE_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0106_LEVEL_PURE_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0107_LEVEL_PURE_MID,                  NULL,               NULL    },
    {   WIFI_PART_0108_LEVEL_PURE_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0109_LEVEL_HOT_HIGH,                  NULL,               NULL    },
    {   WIFI_PART_010A_LEVEL_DRAIN_OVER,                NULL,               NULL    },
    {   WIFI_PART_010B_LEVEL_DRAIN_HIGH,                NULL,               NULL    },
    {   WIFI_PART_010C_LEVEL_DRAIN_MID,                 NULL,               NULL    },
    {   WIFI_PART_010D_LEVEL_DRAIN_LOW,                 NULL,               NULL    },
    
    {   WIFI_PART_0200_COLD_COMP,                       NULL,               NULL    },
    {   WIFI_PART_0201_COLD_BLDC,                       NULL,               NULL    },
    {   WIFI_PART_0202_COLD_TEM,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0203_COLD_TEM_TEMP,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0204_COLD_FAN,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0205_COLD_STIRRING_MOTOR,             NULL,               NULL    },
    {   WIFI_PART_0206_COLD_TEMP_1,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0207_COLD_TEMP_2,                     NULL,               NULL    },
    {   WIFI_PART_0208_COLD_ROOM_TEMP,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0209_COLD_ROTATE_PUMP,                NULL,               NULL    },
        
    {   WIFI_PART_0300_HOT_TANK_HEATER,                 NULL,               NULL    },
    {   WIFI_PART_0301_HOT_TEMP,                        NULL,               NULL    },
    {   WIFI_PART_0302_HOT_INSTANT_HEATER_1,            NULL,               NULL    },
    {   WIFI_PART_0303_HOT_INSTANT_HEATER_2,            NULL,               NULL    },
    {   WIFI_PART_0304_HOT_FLOW_MOTOR,                  NULL,               NULL    },
    {   WIFI_PART_0305_HOT_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0306_HOT_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0307_HOT_HEATER_TEMP,                 NULL,               NULL    },
        
    {   WIFI_PART_0400_SENSOR_PRESSURE_1,               NULL,               NULL    },
    {   WIFI_PART_0401_SENSOR_PRESSURE_2,               NULL,               NULL    },
    {   WIFI_PART_0402_SENSOR_LEAK,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0403_SENSOR_FLOW,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0404_SENSOR_FLOW_REED,                NULL,               NULL    },
    {   WIFI_PART_0405_SENSOR_FILTER_REED,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0406_SENSOR_FILTER_SW_1,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0407_SENSOR_FILTER_SW_2,              NULL,               NULL    },
    {   WIFI_PART_0408_SENSOR_FILTER_SW_3,              NULL,               NULL    },
    {   WIFI_PART_0409_SENSOR_TANK_REED,                NULL,               NULL    },
    {   WIFI_PART_040A_SENSOR_LIGHT,                    NULL,               NULL    },
    {   WIFI_PART_040B_SENSOR_WATER_COURSE_REED,        EvtWifiTxPart,      NULL    },
        
    {   WIFI_PART_0500_STER_CSI_MODULE,                 NULL,               NULL    },
    {   WIFI_PART_0501_STER_UV_FAUCET,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0502_STER_UV_FAUCET_ICE,              NULL,               NULL    },
    {   WIFI_PART_0503_STER_UV_ICE_TANK,                NULL,               NULL    },
    {   WIFI_PART_0504_STER_UV_ICE_TRAY,                NULL,               NULL    },
    {   WIFI_PART_0505_STER_UV_COLD_TANK,               NULL,               NULL    },

    {   WIFI_PART_0600_TDS_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0601_TDS_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0602_TDS_SENSOR_IN,                   NULL,               NULL    },
    {   WIFI_PART_0603_TDS_SENSOR_OUT,                  NULL,               NULL    },

    {   WIFI_PART_0700_FAUCET_REED_1,                   NULL,               NULL    },
    {   WIFI_PART_0701_FAUCET_REED_2,                   NULL,               NULL    },
    {   WIFI_PART_0702_FAUCET_CUP_SENSOR,               NULL,               NULL    },
    {   WIFI_PART_0703_FAUCET_MOTOR,                    NULL,               NULL    },

    {   WIFI_PART_0800_ICE_PURE_TEMP,                   NULL,               NULL    },
    {   WIFI_PART_0801_ICE_TRAY_TEMP_IN,                NULL,               NULL    },
    {   WIFI_PART_0802_ICE_FULL_SENSOR,                 NULL,               NULL    },
    {   WIFI_PART_0803_ICE_SWING_BAR,                   NULL,               NULL    },
    {   WIFI_PART_0804_ICE_TRAY_AC,                     NULL,               NULL    },
    {   WIFI_PART_0805_ICE_TRAY_STEPMOTOR,              NULL,               NULL    },
    {   WIFI_PART_0806_ICE_TRAY_SENSING_SW,             NULL,               NULL    },
    {   WIFI_PART_0807_DEICE_TRAY_SENSING_SW,           NULL,               NULL    },
    {   WIFI_PART_0808_ICE_MOTOR_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0809_ICE_DOOR_STEPMOTOR,              NULL,               NULL    },
    {   WIFI_PART_080A_ICE_COURSE_CHANGE_VV,            NULL,               NULL    },
    {   WIFI_PART_080B_ICE_HOT_GAS_VV,                  NULL,               NULL    },
    {   WIFI_PART_080C_ICE_DRAIN_PUMP,                  NULL,               NULL    }      
};
#endif

#ifdef MODEL_NO_4
static const WifiTxFuncList_T WifiPartList[] = 
{   /* PART LIST ID                                     Tx Function         Rx Function */
    {   WIFI_PART_0000_VALVE_FEED,                      NULL,               NULL    },
    {   WIFI_PART_0001_VALVE_NOS,                       EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0002_VALVE_HOT_OUT,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0003_VALVE_PURE_OUT,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0004_VALVE_COLD_OUT,                  NULL,               NULL    },
    {   WIFI_PART_0005_VALVE_CP_OUT,                    EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0006_VALVE_CHP_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0007_VALVE_PURE_IN,                   NULL,               NULL    },
    {   WIFI_PART_0008_VALVE_HOT_AIR,                   NULL,               NULL    },
    {   WIFI_PART_0009_VALVE_HOT_IN,                    EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000A_VALVE_HOT_DRAIN,                 EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000B_VALVE_COLD_AIR,                  NULL,               NULL    },
    {   WIFI_PART_000C_VALVE_COLD_IN,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000D_VALVE_COLD_DRAIN,                NULL,               NULL    },
    {   WIFI_PART_000E_VALVE_ICE_STORAGE_IN,            NULL,               NULL    },
    {   WIFI_PART_000F_VALVE_PRESSURE_IN,               NULL,               NULL    },
    {   WIFI_PART_0010_VALVE_PRESSURE_DRAIN,            NULL,               NULL    },
    {   WIFI_PART_0011_VALVE_STERILIZE,                 NULL,               NULL    },
    {   WIFI_PART_0012_VALVE_DRAIN_NOS,                 NULL,               NULL    },
    {   WIFI_PART_0013_VALVE_ION_CHANGE,                NULL,               NULL    },
    {   WIFI_PART_0014_VALVE_INNO_CENCE,                NULL,               NULL    },
    {   WIFI_PART_0015_VALVE_ICE_TRAY_IN,               NULL,               NULL    },
   
    {   WIFI_PART_0100_LEVEL_COLD_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0101_LEVEL_COLD_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0102_LEVEL_COLD_MID,                  NULL,               NULL    },
    {   WIFI_PART_0103_LEVEL_COLD_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0104_LEVEL_ICE_STORAGE_HIGH,          NULL,               NULL    },
    {   WIFI_PART_0105_LEVEL_PURE_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0106_LEVEL_PURE_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0107_LEVEL_PURE_MID,                  NULL,               NULL    },
    {   WIFI_PART_0108_LEVEL_PURE_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0109_LEVEL_HOT_HIGH,                  NULL,               NULL    },

    {   WIFI_PART_0200_COLD_COMP,                       NULL,               NULL    },
    {   WIFI_PART_0201_COLD_BLDC,                       NULL,               NULL    },
    {   WIFI_PART_0202_COLD_TEM,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0203_COLD_TEM_TEMP,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0204_COLD_FAN,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0205_COLD_STIRRING_MOTOR,             NULL,               NULL    },
    {   WIFI_PART_0206_COLD_TEMP_1,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0207_COLD_TEMP_2,                     NULL,               NULL    },
    {   WIFI_PART_0208_COLD_ROOM_TEMP,                  EvtWifiTxPart,      NULL    },
        
    {   WIFI_PART_0300_HOT_TANK_HEATER,                 NULL,               NULL    },
    {   WIFI_PART_0301_HOT_TEMP,                        NULL,               NULL    },
    {   WIFI_PART_0302_HOT_INSTANT_HEATER_1,            EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0303_HOT_INSTANT_HEATER_2,            EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0304_HOT_FLOW_MOTOR,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0305_HOT_TEMP_IN,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0306_HOT_TEMP_OUT,                    EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0307_HOT_HEATER_TEMP,                 EvtWifiTxPart,      NULL    },
        
    {   WIFI_PART_0400_SENSOR_PRESSURE_1,               NULL,               NULL    },
    {   WIFI_PART_0401_SENSOR_PRESSURE_2,               NULL,               NULL    },
    {   WIFI_PART_0402_SENSOR_LEAK,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0403_SENSOR_FLOW,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0404_SENSOR_FLOW_REED,                NULL,               NULL    },
    {   WIFI_PART_0405_SENSOR_FILTER_REED,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0406_SENSOR_FILTER_SW_1,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0407_SENSOR_FILTER_SW_2,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0408_SENSOR_FILTER_SW_3,              NULL,               NULL    },

    {   WIFI_PART_0500_STER_CSI_MODULE,                 NULL,               NULL    },
    {   WIFI_PART_0501_STER_UV_FAUCET,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0502_STER_UV_FAUCET_ICE,              NULL,               NULL    },
    {   WIFI_PART_0503_STER_UV_ICE_TANK,                NULL,               NULL    },
    {   WIFI_PART_0504_STER_UV_ICE_TRAY,                NULL,               NULL    },
    {   WIFI_PART_0505_STER_UV_COLD_TANK,               NULL,               NULL    },

    {   WIFI_PART_0600_TDS_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0601_TDS_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0602_TDS_SENSOR_IN,                   NULL,               NULL    },
    {   WIFI_PART_0603_TDS_SENSOR_OUT,                  NULL,               NULL    },

    {   WIFI_PART_0700_FAUCET_REED_1,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0701_FAUCET_REED_2,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0702_FAUCET_CUP_SENSOR,               EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0703_FAUCET_MOTOR,                    EvtWifiTxPart,      NULL    }
};
static const WifiTxFuncList_T WifiPartListCp[] = 
{
    {   WIFI_PART_0000_VALVE_FEED,                      NULL,               NULL    },
    {   WIFI_PART_0001_VALVE_NOS,                       EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0002_VALVE_HOT_OUT,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0003_VALVE_PURE_OUT,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0004_VALVE_COLD_OUT,                  NULL,               NULL    },
    {   WIFI_PART_0005_VALVE_CP_OUT,                    EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0006_VALVE_CHP_OUT,                   NULL,               NULL    },
    {   WIFI_PART_0007_VALVE_PURE_IN,                   NULL,               NULL    },
    {   WIFI_PART_0008_VALVE_HOT_AIR,                   NULL,               NULL    },
    {   WIFI_PART_0009_VALVE_HOT_IN,                    NULL,               NULL    },
    {   WIFI_PART_000A_VALVE_HOT_DRAIN,                 NULL,               NULL    },
    {   WIFI_PART_000B_VALVE_COLD_AIR,                  NULL,               NULL    },
    {   WIFI_PART_000C_VALVE_COLD_IN,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_000D_VALVE_COLD_DRAIN,                NULL,               NULL    },
    {   WIFI_PART_000E_VALVE_ICE_STORAGE_IN,            NULL,               NULL    },
    {   WIFI_PART_000F_VALVE_PRESSURE_IN,               NULL,               NULL    },
    {   WIFI_PART_0010_VALVE_PRESSURE_DRAIN,            NULL,               NULL    },
    {   WIFI_PART_0011_VALVE_STERILIZE,                 NULL,               NULL    },
    {   WIFI_PART_0012_VALVE_DRAIN_NOS,                 NULL,               NULL    },
    {   WIFI_PART_0013_VALVE_ION_CHANGE,                NULL,               NULL    },
    {   WIFI_PART_0014_VALVE_INNO_CENCE,                NULL,               NULL    },
    {   WIFI_PART_0015_VALVE_ICE_TRAY_IN,               NULL,               NULL    },
   
    {   WIFI_PART_0100_LEVEL_COLD_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0101_LEVEL_COLD_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0102_LEVEL_COLD_MID,                  NULL,               NULL    },
    {   WIFI_PART_0103_LEVEL_COLD_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0104_LEVEL_ICE_STORAGE_HIGH,          NULL,               NULL    },
    {   WIFI_PART_0105_LEVEL_PURE_OVER,                 NULL,               NULL    },
    {   WIFI_PART_0106_LEVEL_PURE_HIGH,                 NULL,               NULL    },
    {   WIFI_PART_0107_LEVEL_PURE_MID,                  NULL,               NULL    },
    {   WIFI_PART_0108_LEVEL_PURE_LOW,                  NULL,               NULL    },
    {   WIFI_PART_0109_LEVEL_HOT_HIGH,                  NULL,               NULL    },

    {   WIFI_PART_0200_COLD_COMP,                       NULL,               NULL    },
    {   WIFI_PART_0201_COLD_BLDC,                       NULL,               NULL    },
    {   WIFI_PART_0202_COLD_TEM,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0203_COLD_TEM_TEMP,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0204_COLD_FAN,                        EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0205_COLD_STIRRING_MOTOR,             NULL,               NULL    },
    {   WIFI_PART_0206_COLD_TEMP_1,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0207_COLD_TEMP_2,                     NULL,               NULL    },
    {   WIFI_PART_0208_COLD_ROOM_TEMP,                  EvtWifiTxPart,      NULL    },
        
    {   WIFI_PART_0300_HOT_TANK_HEATER,                 NULL,               NULL    },
    {   WIFI_PART_0301_HOT_TEMP,                        NULL,               NULL    },
    {   WIFI_PART_0302_HOT_INSTANT_HEATER_1,            NULL,               NULL    },
    {   WIFI_PART_0303_HOT_INSTANT_HEATER_2,            NULL,               NULL    },
    {   WIFI_PART_0304_HOT_FLOW_MOTOR,                  NULL,               NULL    },
    {   WIFI_PART_0305_HOT_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0306_HOT_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0307_HOT_HEATER_TEMP,                 NULL,               NULL    },
        
    {   WIFI_PART_0400_SENSOR_PRESSURE_1,               NULL,               NULL    },
    {   WIFI_PART_0401_SENSOR_PRESSURE_2,               NULL,               NULL    },
    {   WIFI_PART_0402_SENSOR_LEAK,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0403_SENSOR_FLOW,                     EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0404_SENSOR_FLOW_REED,                NULL,               NULL    },
    {   WIFI_PART_0405_SENSOR_FILTER_REED,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0406_SENSOR_FILTER_SW_1,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0407_SENSOR_FILTER_SW_2,              EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0408_SENSOR_FILTER_SW_3,              NULL,               NULL    },

    {   WIFI_PART_0500_STER_CSI_MODULE,                 NULL,               NULL    },
    {   WIFI_PART_0501_STER_UV_FAUCET,                  EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0502_STER_UV_FAUCET_ICE,              NULL,               NULL    },
    {   WIFI_PART_0503_STER_UV_ICE_TANK,                NULL,               NULL    },
    {   WIFI_PART_0504_STER_UV_ICE_TRAY,                NULL,               NULL    },
    {   WIFI_PART_0505_STER_UV_COLD_TANK,               NULL,               NULL    },

    {   WIFI_PART_0600_TDS_TEMP_IN,                     NULL,               NULL    },
    {   WIFI_PART_0601_TDS_TEMP_OUT,                    NULL,               NULL    },
    {   WIFI_PART_0602_TDS_SENSOR_IN,                   NULL,               NULL    },
    {   WIFI_PART_0603_TDS_SENSOR_OUT,                  NULL,               NULL    },

    {   WIFI_PART_0700_FAUCET_REED_1,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0701_FAUCET_REED_2,                   EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0702_FAUCET_CUP_SENSOR,               EvtWifiTxPart,      NULL    },
    {   WIFI_PART_0703_FAUCET_MOTOR,                    EvtWifiTxPart,      NULL    }
};
#endif

#define SZ_PT_LIST ( sizeof(WifiPartList) / sizeof(WifiTxFuncList_T) )


/******************************************************************************/
/***** Global Function ********************************************************/
/******************************************************************************/
void* GetWifiPartEventList ( void )
{
    if ( IsUserModelCHP() == TRUE )
    {
        return WifiPartList;
    }
    else
    {
        return WifiPartListCp;
    }
}

U16 GetWifiPartListSize ( void )
{
    return SZ_PT_LIST;
}


/******************************************************************************/
/***** Local Function *********************************************************/
/******************************************************************************/
static void EvtWifiTxPart ( U16 mId, I8 *pBuf )
{
    static I8 mai8Buf[LENGTH_WIFI_DATA_ONE];

    _MEMSET_( (void __FAR*) mai8Buf, '\0', sizeof(mai8Buf));

    ConvertDataHexToAscii( TYPE_PART_CMD, mId, mai8Buf );
    
    far_sprintf_s( pBuf, 0, mai8Buf );
}


