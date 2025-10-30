
/*******************************************************************************
&Description :
    A1010 ~ A101x
    Function Table
  
&Histiry : 
    ShinHM 2021.04.13 update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_SetCommon.h"
#include "WIFI_TxForm.h"
#include "WIFI_SetFunctionData.h"


/******************************************************************************/
/***** Static Function ********************************************************/
/******************************************************************************/
static void EvtWifiTxFunc ( U16 mId, I8 *pBuf );
static void EvtWifiRxFunc ( U16 mId, I8 *pBuf );


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
#ifdef MODEL_NO_1
static const WifiTxFuncList_T WifiFuncList[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    // {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   }, // 1(ON) / 2(Lock)
    // {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    // {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    // {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0(OFF) / 1(COLD) / 3(ICE)
    // {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    // {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    // {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    // {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    // {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    // {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    // {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    // {   WIFI_FUNC_0013_STERILIZE_TIME,          NULL,           NULL            },
    // {   WIFI_FUNC_0014_STERILIZE_STATUS,        NULL,           NULL            }, // 0 / 1 / 2(Going)
    // {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    // {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    // {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    // {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    // {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    // {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    // {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    // {   WIFI_FUNC_001F_DRAIN_STATUS,            NULL,           NULL            }, // 0 ~ 2
    // {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    // {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            }, // 0 ~ 65535 sec
    // {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            }, // 0 ~ 65535 sec
    // {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    // {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    // {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    // {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    // {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    // {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    // {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    // {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    // {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    // {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    // {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    // {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    // {   WIFI_FUNC_0037_SLEEP_SEL,               NULL,           NULL            }, // 0 / 1 
    // {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    // {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(ON)
    // {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    // {   WIFI_FUNC_003B_HOT_TEMP_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   },
    // {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       NULL,           NULL            }, // 0 / 1
    // {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    // {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     NULL,           NULL            },
    // {   WIFI_FUNC_003F_WATER_COURSE_RESET,      NULL,           NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            }, // 0 / 1
    // {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            }, // 0 / 1
    // {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    // {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    // {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    // {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    // {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          NULL,           NULL            },
    // {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         NULL,           NULL            },
    // {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      NULL,           NULL            },
    // {   WIFI_FUNC_004E_CUP_SENSING_SEL,         NULL,           NULL            },
    // {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    // {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    // {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    // {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    // {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    // {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    // {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	NULL,	        NULL	        },
    {   WIFI_FUNC_0058_ICE_SIZE_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0059_ICE_ONOFF_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_005A_UV_ICE_TANK_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_005B_UV_ICE_TRAY_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_005C_SMART_CHECK_RESULT,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_005D_UV_ICE_FAUCET_STATUS,    EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_005E_SLEEP_MODE_SEL,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_005F_SLEEP_START_HOUR,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0060_SLEEP_START_MIN,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0061_SLEEP_END_HOUR,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0062_SLEEP_END_MIN,           EvtWifiTxFunc,  EvtWifiRxFunc   },   

    // {	WIFI_FUNC_0065_DEFAULT_TEMP_SEL,	    NULL,	NULL   },
    // {	WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL,	NULL,	NULL   },
    // {	WIFI_FUNC_0067_WATER_QUANTITY_USE_SEL0,	NULL,	NULL   },
    // {	WIFI_FUNC_0068_WATER_QUANTITY_USE_SEL1,	NULL,	NULL   },
    // {	WIFI_FUNC_0069_WATER_QUANTITY_USE_SEL2,	NULL,	NULL   },
    // {	WIFI_FUNC_006A_WATER_QUANTITY_USE_SEL3,	NULL,	NULL   },
    // {	WIFI_FUNC_006B_WATER_QUANTITY_USE_SEL4,	NULL,	NULL   },
    // {	WIFI_FUNC_006C_WATER_QUANTITY_USE_SEL5,	NULL,	NULL   },
    // {	WIFI_FUNC_006D_WATER_QUANTITY_USE_SEL6,	NULL,	NULL   },
    // {	WIFI_FUNC_006E_WATER_QUANTITY_USE_SEL7, NULL,	NULL   },
    // {	WIFI_FUNC_006F_WATER_QUANTITY_USE_SEL8, NULL,	NULL   }, 

    // {	WIFI_FUNC_0070_WATER_QUANTITY_USE_SEL9, NULL,	NULL   },
    // {	WIFI_FUNC_0071_WATER_QUANTITY_USE_SEL10, NULL,	NULL   },
    // {	WIFI_FUNC_0072_WATER_QUANTITY_SORT_SEL,	NULL,	NULL   },
    // {	WIFI_FUNC_0073_WATER_TEMP_SORT_SEL,	    NULL,	NULL   },
    // {	WIFI_FUNC_0074_UNUSE_POWER_SAVING_SEL,	NULL,	NULL   },
    // {	WIFI_FUNC_0075_TEMP_UNIT_SEL,	        NULL,	NULL   },
    // {	WIFI_FUNC_0076_WATER_QUANTITY_UNIT_SEL,	NULL,	NULL   },
    // {	WIFI_FUNC_0077_HOT_TARGET_TEMP_TIME,	NULL,	NULL   },
    // {	WIFI_FUNC_0078_COLD_TARGET_TEMP_TIME,	EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_0079_MIXED_WATER_AVE_TEMP,	NULL,	NULL   },
    // {	WIFI_FUNC_007A_MIXED_WATER_MIN_TEMP,	NULL,	NULL   },
    // {	WIFI_FUNC_007B_MIXED_WATER_MAX_TEMP,	NULL,	NULL   },
    // {	WIFI_FUNC_007C_COLDTANK_LOW_WATERLEVEL,	NULL,	NULL   },
    // {	WIFI_FUNC_007D_COLDTANK_HIGH_WATERLEVEL,NULL,	NULL   },
    // {	WIFI_FUNC_007E_HIGH_TEMP_STL_SEL,       NULL,	NULL   },
    // {	WIFI_FUNC_007F_DEFAULT_TEMP_SET,        NULL,	NULL   },

    // {	WIFI_FUNC_0080_HIGH_TEMP_STL_TIME,      NULL,	NULL   },
    // {	WIFI_FUNC_0081_HIGH_TEMP_STL_TIME_M,    NULL,	NULL   },
    // {	WIFI_FUNC_0082_UNUSE_POW_SAVING_STATUS,	NULL,	NULL   },
    // {	WIFI_FUNC_0083_POWER_SAVING_STATUS,	    NULL,	NULL   },
    // {	WIFI_FUNC_0084_HOT_RESTARTING_TEMP,	    NULL,	NULL   },
    // {	WIFI_FUNC_0085_HOT_STOP_TEMP,	        NULL,	NULL   },
    // {	WIFI_FUNC_0086_COLD_RESTARTING_TEMP,	NULL,	NULL   },
    // {	WIFI_FUNC_0087_COLD_STOP_TEMP,	        NULL,	NULL   },
    // {	WIFI_FUNC_0088_COLD_ADDITIONAL_TIME,	NULL,	NULL   },
    // {	WIFI_FUNC_0089_BOOST_PUMP_STATUS,	    NULL,	NULL   },
    // {	WIFI_FUNC_008A_COLDTANK_UV_STL_STATUS,	NULL,	NULL   },
    // {	WIFI_FUNC_008B_HEATER_STATUS,	        NULL,	NULL   },
    // {	WIFI_FUNC_008C_COMP_STATUS,	            EvtWifiTxFunc,	NULL	        },
    // {	WIFI_FUNC_008D_FILTER_WATER_USAGE,	    NULL,	NULL   },
    // {	WIFI_FUNC_008E_FILTER_REMAINING_DAY,	NULL,	NULL   },
    // {	WIFI_FUNC_008F_ICE_FULL_STATUS,	        EvtWifiTxFunc,	NULL            },

    // {	WIFI_FUNC_0090_ICE_TRAY_POSITION_STATUS,NULL,	NULL   },
    // {	WIFI_FUNC_0091_REFRIGERANT_SWC_STATUS,	NULL,	NULL   },
    // {	WIFI_FUNC_0092_DEICING_STAUTS,	        NULL,   NULL            },
    // {	WIFI_FUNC_0093_ICEMAKING_STAUTS,	    EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_0094_DUMMY_DEICING_STAUTS,	NULL,	NULL            },
    // {	WIFI_FUNC_0095_ICEMAKING_COMPLET_TIME,	EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_0096_DEICING_COMPLET_TIME,	EvtWifiTxFunc,  NULL            },
    // {	WIFI_FUNC_0097_TARY_WATER_COMPLET_TIME,	EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_0098_TARY_WATER_QUANTITY,	    EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_0099_COMP_TARGET_RPS,	        NULL,	NULL            },
    // {	WIFI_FUNC_009A_COMP_CURRENT_RPS,	    NULL,	        NULL            },
    // {	WIFI_FUNC_009B_DRAINTANK_LOW_WLEVEL,    NULL,	NULL            },
    // {	WIFI_FUNC_009C_DRAINTANK_HIGH_WLEVEL,	NULL,	NULL            },
    // {	WIFI_FUNC_009D_DRAINTANK_DRAIN_STAUTS,  NULL,	NULL            },
    // {	WIFI_FUNC_009E_COLD_EVA_TEMP,	        NULL,	        NULL            },
    // {	WIFI_FUNC_009F_ICE_EVA_TEMP,	        EvtWifiTxFunc,	NULL            },

    // {	WIFI_FUNC_00A0_COLD_TANK_TEMP,	        EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_00A1_HOT_TANK_TEMP,	        NULL,	NULL            },
    // {	WIFI_FUNC_00A2_CHILD_LOCK,	            NULL,	NULL   },
    // {	WIFI_FUNC_00A3_FLOWPATH_EMPTY,	        NULL,	        NULL            },
    // {	WIFI_FUNC_00A4_HOTTANK_LOW_W_LEVEL,	    NULL,	        NULL            },
    // {	WIFI_FUNC_00A5_HOTTANK_HIGH_W_LEVEL,	NULL,	        NULL            },
    // {	WIFI_FUNC_00A6_PURETANK_LOW_W_LEVEL,	NULL,	        NULL            },
    // {	WIFI_FUNC_00A7_PURETANK_HIGH_W_LEVEL,	NULL,	        NULL            },
    // {	WIFI_FUNC_00A8_COLDTANK_MID_W_LEVEL,        NULL,	        NULL            },
    // {	WIFI_FUNC_00A9_UNUSED_TIME,	            NULL,	    NULL            },
    // {	WIFI_FUNC_00AA_SMART_SAVING_SET_TIME,	NULL,	    NULL   },
    // {	WIFI_FUNC_00AB_SAMRT_SAVING_STATE,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00AC_FILTER1_WATER_USAGE,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00AD_FILTER1_UASGE_DAY,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00AE_FILTER2_WATER_USAGE,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00AF_FILTER2_UASGE_DAY,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00B0_FILTER3_WATER_USAGE,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00B1_FILTER3_UASGE_DAY,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00B2_FILTER_NOTICE1,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00B3_FILTER_NOTICE2,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00B4_FILTER_NOTICE3,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00B5_FILTER_FLUSHING,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00B6_VOICE_LANGUAGE,	        NULL,	    NULL   },
    // {	WIFI_FUNC_00B7_FILTER_SEQ,	            NULL,	    NULL   },
    // {	WIFI_FUNC_00B8_FILTER1_DDAY,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00B9_FILTER2_DDAY,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00BA_FILTER3_DDAY,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00BB_FLOWPATH_STL_STATUS,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00BC_FAUCE_STL_STATUS,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00BD_ICETANK_STL_STATUS,	    NULL,	    NULL            },
    {	WIFI_FUNC_00BE_QUANTITY_HALFCUP,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00BF_QUANTITY_ONECUP,	        EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00C0_QUANTITY_TWOCUPS,	    EvtWifiTxFunc,	EvtWifiRxFunc   },

    {	WIFI_FUNC_00C1_HOTTEMP_USE_SEL0,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00C2_HOTTEMP_USE_SEL1,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00C3_HOTTEMP_USE_SEL2,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00C4_HOTTEMP_USE_SEL3,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00C5_HOTTEMP_USE_SEL4,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00C6_HOTTEMP_USE_SEL5,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00C7_HOTTEMP_USE_SEL6,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00C8_HOTTEMP_USE_SEL7,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00C9_HOTTEMP_USE_SEL8,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00CA_HOTTEMP_USE_SEL9,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00CB_WATER_EXTRACT_STATUS,	EvtWifiTxFunc,	    NULL            },
    // {	WIFI_FUNC_00CC_ICE_EXTRACT_STATUS,	    EvtWifiTxFunc,	    NULL            },
    // {	WIFI_FUNC_00CD_SETUP_MODE_STATUS,	    EvtWifiTxFunc,	    NULL            },
    {	WIFI_FUNC_00CE_HOTTEMP_USE_SEL10,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00CF_HOTTEMP_USE_SEL11,	    EvtWifiTxFunc,	EvtWifiRxFunc   },

};

static const WifiTxFuncList_T WifiFuncListCp[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    // {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    // {   WIFI_FUNC_0003_HOT_LOCK_SEL,            NULL,           NULL            }, // 1(ON) / 2(Lock)
    // {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    // {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    // {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0(OFF) / 1(COLD) / 3(ICE)
    // {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    // {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    // {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    // {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    // {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    // {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    // {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    // {   WIFI_FUNC_0013_STERILIZE_TIME,          NULL,           NULL            },
    // {   WIFI_FUNC_0014_STERILIZE_STATUS,        NULL,           NULL            }, // 0 / 1 / 2(Going)
    // {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    // {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    // {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    // {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    // {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    // {   WIFI_FUNC_001A_HOT_INTEMP,              NULL,           NULL            }, // 0 ~ 99 Temp (H)
    // {   WIFI_FUNC_001B_HOT_MEANTEMP,            NULL,           NULL            }, // 0 ~ 99 Temp (H)
    // {   WIFI_FUNC_001C_HOT_SELTEMP,             NULL,           NULL            }, // 0 ~ 99 Temp (H)
    // {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    // {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    // {   WIFI_FUNC_001F_DRAIN_STATUS,            NULL,           NULL            }, // 0 ~ 2
    // {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    // {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            }, // 0 ~ 65535 sec
    // {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            }, // 0 ~ 65535 sec
    // {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    // {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    // {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    // {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    // {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    // {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    // {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    // {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    // {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    // {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    // {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    // {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    // {   WIFI_FUNC_0037_SLEEP_SEL,               NULL,           NULL            }, // 0 / 1 
    // {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    // {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(ON)
    // {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    // {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,           NULL            },
    // {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       NULL,           NULL            }, // 0 / 1
    // {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    // {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     NULL,           NULL            },
    // {   WIFI_FUNC_003F_WATER_COURSE_RESET,      NULL,           NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            }, // 0 / 1
    // {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            }, // 0 / 1
    // {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    // {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    // {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    // {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    // {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          NULL,           NULL            },
    // {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         NULL,           NULL            },
    // {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      NULL,           NULL            },
    // {   WIFI_FUNC_004E_CUP_SENSING_SEL,         NULL,           NULL            },
    // {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    // {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    // {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    // {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    // {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    // {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    // {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	NULL,	        NULL	        },
    {   WIFI_FUNC_0058_ICE_SIZE_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0059_ICE_ONOFF_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_005A_UV_ICE_TANK_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_005B_UV_ICE_TRAY_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_005C_SMART_CHECK_RESULT,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_005D_UV_ICE_FAUCET_STATUS,    EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_005E_SLEEP_MODE_SEL,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_005F_SLEEP_START_HOUR,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0060_SLEEP_START_MIN,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0061_SLEEP_END_HOUR,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0062_SLEEP_END_MIN,           EvtWifiTxFunc,  EvtWifiRxFunc   },

    // {	WIFI_FUNC_0065_DEFAULT_TEMP_SEL,	    NULL,	NULL   },
    // {	WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL,	NULL,	NULL   },
    // {	WIFI_FUNC_0067_WATER_QUANTITY_USE_SEL0,	NULL,	NULL   },
    // {	WIFI_FUNC_0068_WATER_QUANTITY_USE_SEL1,	NULL,	NULL   },
    // {	WIFI_FUNC_0069_WATER_QUANTITY_USE_SEL2,	NULL,	NULL   },
    // {	WIFI_FUNC_006A_WATER_QUANTITY_USE_SEL3,	NULL,	NULL   },
    // {	WIFI_FUNC_006B_WATER_QUANTITY_USE_SEL4,	NULL,	NULL   },
    // {	WIFI_FUNC_006C_WATER_QUANTITY_USE_SEL5,	NULL,	NULL   },
    // {	WIFI_FUNC_006D_WATER_QUANTITY_USE_SEL6,	NULL,	NULL   },
    // {	WIFI_FUNC_006E_WATER_QUANTITY_USE_SEL7, NULL,	NULL   },
    // {	WIFI_FUNC_006F_WATER_QUANTITY_USE_SEL8, NULL,	NULL   }, 

    // {	WIFI_FUNC_0070_WATER_QUANTITY_USE_SEL9, NULL,	NULL   },
    // {	WIFI_FUNC_0071_WATER_QUANTITY_USE_SEL10, NULL,	NULL   },
    // {	WIFI_FUNC_0072_WATER_QUANTITY_SORT_SEL,	NULL,	NULL   },
    // {	WIFI_FUNC_0073_WATER_TEMP_SORT_SEL,	    NULL,	NULL   },
    // {	WIFI_FUNC_0074_UNUSE_POWER_SAVING_SEL,	NULL,	NULL   },
    // {	WIFI_FUNC_0075_TEMP_UNIT_SEL,	        NULL,	NULL   },
    // {	WIFI_FUNC_0076_WATER_QUANTITY_UNIT_SEL,	NULL,	NULL   },
    // {	WIFI_FUNC_0077_HOT_TARGET_TEMP_TIME,	NULL,	NULL   },
    // {	WIFI_FUNC_0078_COLD_TARGET_TEMP_TIME,	EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_0079_MIXED_WATER_AVE_TEMP,	NULL,	NULL   },
    // {	WIFI_FUNC_007A_MIXED_WATER_MIN_TEMP,	NULL,	NULL   },
    // {	WIFI_FUNC_007B_MIXED_WATER_MAX_TEMP,	NULL,	NULL   },
    // {	WIFI_FUNC_007C_COLDTANK_LOW_WATERLEVEL,	NULL,	NULL   },
    // {	WIFI_FUNC_007D_COLDTANK_HIGH_WATERLEVEL,NULL,	NULL   },
    // {	WIFI_FUNC_007E_HIGH_TEMP_STL_SEL,       NULL,	NULL   },
    // {	WIFI_FUNC_007F_DEFAULT_TEMP_SET,        NULL,	NULL   },

    // {	WIFI_FUNC_0080_HIGH_TEMP_STL_TIME,      NULL,	NULL   },
    // {	WIFI_FUNC_0081_HIGH_TEMP_STL_TIME_M,    NULL,	NULL   },
    // {	WIFI_FUNC_0082_UNUSE_POW_SAVING_STATUS,	NULL,	NULL   },
    // {	WIFI_FUNC_0083_POWER_SAVING_STATUS,	    NULL,	NULL   },
    // {	WIFI_FUNC_0084_HOT_RESTARTING_TEMP,	    NULL,	NULL   },
    // {	WIFI_FUNC_0085_HOT_STOP_TEMP,	        NULL,	NULL   },
    // {	WIFI_FUNC_0086_COLD_RESTARTING_TEMP,	NULL,	NULL   },
    // {	WIFI_FUNC_0087_COLD_STOP_TEMP,	        NULL,	NULL   },
    // {	WIFI_FUNC_0088_COLD_ADDITIONAL_TIME,	NULL,	NULL   },
    // {	WIFI_FUNC_0089_BOOST_PUMP_STATUS,	    NULL,	NULL   },
    // {	WIFI_FUNC_008A_COLDTANK_UV_STL_STATUS,	NULL,	NULL   },
    // {	WIFI_FUNC_008B_HEATER_STATUS,	        NULL,	NULL   },
    // {	WIFI_FUNC_008C_COMP_STATUS,	            EvtWifiTxFunc,	NULL	        },
    // {	WIFI_FUNC_008D_FILTER_WATER_USAGE,	    NULL,	NULL   },
    // {	WIFI_FUNC_008E_FILTER_REMAINING_DAY,	NULL,	NULL   },
    // {	WIFI_FUNC_008F_ICE_FULL_STATUS,	        EvtWifiTxFunc,	NULL            },

    // {	WIFI_FUNC_0090_ICE_TRAY_POSITION_STATUS,NULL,	NULL   },
    // {	WIFI_FUNC_0091_REFRIGERANT_SWC_STATUS,	NULL,	NULL   },
    // {	WIFI_FUNC_0092_DEICING_STAUTS,	        NULL,   NULL            },
    // {	WIFI_FUNC_0093_ICEMAKING_STAUTS,	    EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_0094_DUMMY_DEICING_STAUTS,	NULL,	NULL            },
    // {	WIFI_FUNC_0095_ICEMAKING_COMPLET_TIME,	EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_0096_DEICING_COMPLET_TIME,	EvtWifiTxFunc,  NULL            },
    // {	WIFI_FUNC_0097_TARY_WATER_COMPLET_TIME,	EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_0098_TARY_WATER_QUANTITY,	    EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_0099_COMP_TARGET_RPS,	        NULL,	NULL            },
    // {	WIFI_FUNC_009A_COMP_CURRENT_RPS,	    NULL,	        NULL            },
    // {	WIFI_FUNC_009B_DRAINTANK_LOW_WLEVEL,    NULL,	NULL            },
    // {	WIFI_FUNC_009C_DRAINTANK_HIGH_WLEVEL,	NULL,	NULL            },
    // {	WIFI_FUNC_009D_DRAINTANK_DRAIN_STAUTS,  NULL,	NULL            },
    // {	WIFI_FUNC_009E_COLD_EVA_TEMP,	        NULL,	        NULL            },
    // {	WIFI_FUNC_009F_ICE_EVA_TEMP,	        EvtWifiTxFunc,	NULL            },

    // {	WIFI_FUNC_00A0_COLD_TANK_TEMP,	        EvtWifiTxFunc,	NULL            },
    // {	WIFI_FUNC_00A1_HOT_TANK_TEMP,	        NULL,	NULL            },
    // {	WIFI_FUNC_00A2_CHILD_LOCK,	            NULL,	NULL   },
    // {	WIFI_FUNC_00A3_FLOWPATH_EMPTY,	        NULL,	        NULL            },
    // {	WIFI_FUNC_00A4_HOTTANK_LOW_W_LEVEL,	    NULL,	        NULL            },
    // {	WIFI_FUNC_00A5_HOTTANK_HIGH_W_LEVEL,	NULL,	        NULL            },
    // {	WIFI_FUNC_00A6_PURETANK_LOW_W_LEVEL,	NULL,	        NULL            },
    // {	WIFI_FUNC_00A7_PURETANK_HIGH_W_LEVEL,	NULL,	        NULL            },
    // {	WIFI_FUNC_00A8_COLDTANK_MID_W_LEVEL,        NULL,	        NULL            },
    // {	WIFI_FUNC_00A9_UNUSED_TIME,	            NULL,	    NULL            },
    // {	WIFI_FUNC_00AA_SMART_SAVING_SET_TIME,	NULL,	    NULL   },
    // {	WIFI_FUNC_00AB_SAMRT_SAVING_STATE,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00AC_FILTER1_WATER_USAGE,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00AD_FILTER1_UASGE_DAY,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00AE_FILTER2_WATER_USAGE,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00AF_FILTER2_UASGE_DAY,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00B0_FILTER3_WATER_USAGE,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00B1_FILTER3_UASGE_DAY,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00B2_FILTER_NOTICE1,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00B3_FILTER_NOTICE2,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00B4_FILTER_NOTICE3,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00B5_FILTER_FLUSHING,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00B6_VOICE_LANGUAGE,	        NULL,	    NULL   },
    // {	WIFI_FUNC_00B7_FILTER_SEQ,	            NULL,	    NULL   },
    // {	WIFI_FUNC_00B8_FILTER1_DDAY,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00B9_FILTER2_DDAY,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00BA_FILTER3_DDAY,	        NULL,	    NULL            },
    // {	WIFI_FUNC_00BB_FLOWPATH_STL_STATUS,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00BC_FAUCE_STL_STATUS,	    NULL,	    NULL            },
    // {	WIFI_FUNC_00BD_ICETANK_STL_STATUS,	    NULL,	    NULL            },
    {	WIFI_FUNC_00BE_QUANTITY_HALFCUP,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00BF_QUANTITY_ONECUP,	        EvtWifiTxFunc,	EvtWifiRxFunc   },
    {	WIFI_FUNC_00C0_QUANTITY_TWOCUPS,	    EvtWifiTxFunc,	EvtWifiRxFunc   },

    // {	WIFI_FUNC_00C1_HOTTEMP_USE_SEL0,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00C2_HOTTEMP_USE_SEL1,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00C3_HOTTEMP_USE_SEL2,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00C4_HOTTEMP_USE_SEL3,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00C5_HOTTEMP_USE_SEL4,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00C6_HOTTEMP_USE_SEL5,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00C7_HOTTEMP_USE_SEL6,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00C8_HOTTEMP_USE_SEL7,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00C9_HOTTEMP_USE_SEL8,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00CA_HOTTEMP_USE_SEL9,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00CB_WATER_EXTRACT_STATUS,	EvtWifiTxFunc,	    NULL            },
    // {	WIFI_FUNC_00CC_ICE_EXTRACT_STATUS,	    EvtWifiTxFunc,	    NULL            },
    // {	WIFI_FUNC_00CD_SETUP_MODE_STATUS,	    EvtWifiTxFunc,	    NULL            },
    // {	WIFI_FUNC_00CE_HOTTEMP_USE_SEL10,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
    // {	WIFI_FUNC_00CF_HOTTEMP_USE_SEL11,	    EvtWifiTxFunc,	EvtWifiRxFunc   },
};
#endif

#ifdef MODEL_NO_2
static const WifiTxFuncList_T WifiFuncList[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            EvtWifiTxFunc,  NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               NULL,           NULL            }, // 0 / 1 
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     NULL,           NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET,      NULL,           NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        NULL,           NULL            },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,  		NULL	        },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	EvtWifiTxFunc,  EvtWifiRxFunc   }
};

static const WifiTxFuncList_T WifiFuncListCp[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            NULL,           NULL            }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            EvtWifiTxFunc,  NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               NULL,           NULL            }, // 0 / 1 
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     NULL,           NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET,      NULL,           NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        NULL,           NULL            },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,  		NULL	        },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	EvtWifiTxFunc,  EvtWifiRxFunc   }
};
#endif

#ifdef MODEL_NO_3
static const WifiTxFuncList_T WifiFuncList[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          NULL,           NULL            },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        NULL,           NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            NULL,           NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1  // icon 2.0 rx function ??????_ssg
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           NULL,           NULL            },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         NULL,           NULL            },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         NULL,           NULL            },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	NULL,           NULL            },
    {	WIFI_FUNC_0058_ICE_SIZE_SEL,	        NULL,           NULL            },
    {	WIFI_FUNC_0059_ICE_ONOFF_SEL,	        NULL,           NULL            },
    {   WIFI_FUNC_005A_UV_ICE_TANK_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_005B_UV_ICE_TRAY_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_005C_SMART_CHECK_RESULT,      EvtWifiTxFunc,  EvtWifiRxFunc   }
};

static const WifiTxFuncList_T WifiFuncListCp[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            NULL,           NULL            }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          NULL,           NULL            },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        NULL,           NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            NULL,           NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1 
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           NULL,           NULL            },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         NULL,           NULL            },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         NULL,           NULL            },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	NULL,           NULL            },
    {	WIFI_FUNC_0058_ICE_SIZE_SEL,	        NULL,           NULL            },
    {	WIFI_FUNC_0059_ICE_ONOFF_SEL,	        NULL,           NULL            },
    {   WIFI_FUNC_005A_UV_ICE_TANK_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_005B_UV_ICE_TRAY_STATUS,      NULL,           NULL            },
    {   WIFI_FUNC_005C_SMART_CHECK_RESULT,      EvtWifiTxFunc,  EvtWifiRxFunc   }
};
#endif

#ifdef MODEL_NO_4
static const WifiTxFuncList_T WifiFuncList[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            EvtWifiTxFunc,  EvtWifiRxFunc   }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          NULL,           NULL            },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        NULL,           NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             EvtWifiTxFunc,  NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            EvtWifiTxFunc,  NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               NULL,           NULL            }, // 0 / 1 
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     NULL,           NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET,      NULL,           NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        NULL,           NULL            },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	EvtWifiTxFunc,  EvtWifiRxFunc   }
};

static const WifiTxFuncList_T WifiFuncListCp[] = 
{   /*  ID                                      Tx Function     Rx Function     */
    {   WIFI_FUNC_0001_POWER_SEL,               NULL,           NULL            }, // 0(OFF) / 1(ON)
    {   WIFI_FUNC_0002_COLD_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            NULL,           NULL            }, // 1(ON) / 2(Lock)
    {   WIFI_FUNC_0004_SAVE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0005_LOCK_SEL,                EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_0006_MUTE_SEL,                NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0007_STERILIZE_SEL,           NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0008_ICE_SEL,                 NULL,           NULL            }, // 0(OFF) / 1(COLD) / 3(ICE)
    {   WIFI_FUNC_0009_HOT_TEMP,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000A_COLD_TEMP,               EvtWifiTxFunc,  NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000B_WATER_SEL,               EvtWifiTxFunc,  NULL            }, // 1(A) / 2(C) / 3(H)
    {   WIFI_FUNC_000C_USER_SEL,                NULL,           NULL            }, // 0 ~ 3
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 cc
    {   WIFI_FUNC_000E_TIME_YEAR,               NULL,           NULL            },
    {   WIFI_FUNC_000F_TIME_MONTHDAY,           NULL,           NULL            },
    {   WIFI_FUNC_0010_TIME_MINSEC,             NULL,           NULL            },
    {   WIFI_FUNC_0011_STERILIZE_CYCLE,         NULL,           NULL            },
    {   WIFI_FUNC_0012_STERILIZE_MODE,          NULL,           NULL            },
    {   WIFI_FUNC_0013_STERILIZE_TIME,          NULL,           NULL            },
    {   WIFI_FUNC_0014_STERILIZE_STATUS,        NULL,           NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_0015_TDS_IN,                  NULL,           NULL            },
    {   WIFI_FUNC_0016_TDS_OUT,                 NULL,           NULL            },
    {   WIFI_FUNC_0017_WATER_PRESSURE,          NULL,           NULL            },
    {   WIFI_FUNC_0018_TDS_INTEMP,              NULL,           NULL            },
    {   WIFI_FUNC_0019_TDS_OUTTEMP,             NULL,           NULL            },
    {   WIFI_FUNC_001A_HOT_INTEMP,              NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001B_HOT_MEANTEMP,            NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001C_HOT_SELTEMP,             NULL,           NULL            }, // 0 ~ 99 Temp (H)
    {   WIFI_FUNC_001D_WATER_INQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_001E_AUTODRAIN_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_001F_DRAIN_STATUS,            EvtWifiTxFunc,  NULL            }, // 0 ~ 2
    {   WIFI_FUNC_0020_WATER_REQUANTITY,        NULL,           NULL            },
    {   WIFI_FUNC_0021_HEART_TIME1,             NULL,           NULL            },
    {   WIFI_FUNC_0022_HEART_TIME2,             NULL,           NULL            },
    {   WIFI_FUNC_0023_HEART_TIME3,             NULL,           NULL            },
    {   WIFI_FUNC_0024_HEART_TIME4,             EvtWifiTxFunc,  NULL            }, // 0 ~ 65535 sec
    {   WIFI_FUNC_0025_FILTER_NOTICE1,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0026_FILTER_NOTICE2,          NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0027_FILTER_NOTICE3,          NULL,           NULL            },
    {   WIFI_FUNC_0028_FILTER_NOTICE4,          NULL,           NULL            },
    {   WIFI_FUNC_0029_SERVICE_TIME,            NULL,           NULL            },
    {   WIFI_FUNC_002A_REMOVE_STATE,            NULL,           NULL            },
    {   WIFI_FUNC_002B_SAVE_DETECT,             NULL,           NULL            },
    {   WIFI_FUNC_002C_SILVER_CARE,             EvtWifiTxFunc,  NULL            }, // 0 / 1 / 2(Going)
    {   WIFI_FUNC_002D_DRAIN_QUANTITY,          NULL,           NULL            },
    {   WIFI_FUNC_002E_STERILIZE_QUANTITY,      NULL,           NULL            },
    {   WIFI_FUNC_002F_SPARKLING_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0030_ICE_MAKETIME,            NULL,           NULL            },
    {   WIFI_FUNC_0031_VOICE_SEL,               EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0032_ICELOCK_SEL,             NULL,           NULL            }, // 0 / 1
    {   WIFI_FUNC_0033_VOICE_VOL_SEL,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0034_LIGHT_SEL,               NULL,           NULL            },
    {   WIFI_FUNC_0035_COVER1_OPEN,             EvtWifiTxFunc,  NULL            }, // 0 / 1
    {   WIFI_FUNC_0036_COVER2_OPEN,             NULL,           NULL            },
    {   WIFI_FUNC_0037_SLEEP_SEL,               NULL,           NULL            }, // 0 / 1 
    {   WIFI_FUNC_0038_MASH_CLEAN,              NULL,           NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0039_HOT_SEL,                 NULL,           NULL            },
    {   WIFI_FUNC_003A_FREEZE_BURST,            NULL,           NULL            },
    {   WIFI_FUNC_003B_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_003C_COLD_STRENGTH_SEL,       EvtWifiTxFunc,  EvtWifiRxFunc   }, // 0 / 1
    {   WIFI_FUNC_003D_HEIGHT_SEL,              NULL,           NULL            }, // 1 ~ 4
    {   WIFI_FUNC_003E_WATER_COURSE_STATUS,     NULL,           NULL            },
    {   WIFI_FUNC_003F_WATER_COURSE_RESET       NULL,           NULL            },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        EvtWifiTxFunc,  EvtWifiRxFunc   }, // 
    {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      EvtWifiTxFunc,  NULL            }, // 0 ~ 5
    {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    EvtWifiTxFunc,  NULL            }, // 0 ~ 100 %
    {   WIFI_FUNC_0043_FILTER_STATUS1,          EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0044_FILTER_STATUS2,          NULL,           NULL            },
    {   WIFI_FUNC_0045_FILTER_STATUS3,          NULL,           NULL            },
    {   WIFI_FUNC_0046_FILTER_STATUS4,          NULL,           NULL            },
    {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0048_HOT_TEMP_SEL,            NULL,           NULL            },
    {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        NULL,           NULL            },
    {   WIFI_FUNC_004A_WELCOME_LIGHT,           EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004B_HIDDEN_LCD_SEL,          EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004C_FAUCET_MOVE_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004D_FAUCET_MOVE_STATUS,      EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_004E_CUP_SENSING_SEL,         EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_004F_SOUND_VOL_SEL,           NULL,           NULL            },
    {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        EvtWifiTxFunc,  EvtWifiRxFunc   },
    {   WIFI_FUNC_0052_VOICE_MODE_SEL,          NULL,           NULL            },
    {   WIFI_FUNC_0053_FILTER_RESET1,           EvtWifiTxFunc,  NULL            },
    {   WIFI_FUNC_0054_FILTER_RESET2,           NULL,           NULL            },
    {   WIFI_FUNC_0055_FILTER_RESET3,           NULL,           NULL            },
    {   WIFI_FUNC_0056_FILTER_RESET4,           NULL,           NULL            },
    {	WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,	EvtWifiTxFunc,  EvtWifiRxFunc   }
};
#endif

#define SZ_FN_LIST ( sizeof(WifiFuncList) / sizeof(WifiTxFuncList_T) )
#define SZ_FN_LIST_CP ( sizeof(WifiFuncListCp) / sizeof(WifiTxFuncList_T) )


/******************************************************************************/
/***** Global Function ********************************************************/
/******************************************************************************/
void* GetWifiFuncEventList ( void )
{
    if ( IsUserModelCHP() == TRUE )
    {
        return WifiFuncList;
    }
    else
    {
        return WifiFuncListCp;
    }
}

U16 GetWifiFuncListSize ( void )
{
    if ( IsUserModelCHP() == TRUE )
    {
        return SZ_FN_LIST;
    }
    else
    {
        return SZ_FN_LIST_CP;
    }
}

// Request ID 재사용 예외처리
U8 IsExceptId ( U16 mId )
{
    if ( mId == WIFI_FUNC_0040_SMART_CHECK_RQST )
    {
        return TRUE;
    }
    return FALSE;
}

U8 IsExceptHotTempUseID ( U16 mId )
{
    if ((( mId >= WIFI_FUNC_00C1_HOTTEMP_USE_SEL0 )&&( mId <= WIFI_FUNC_00CA_HOTTEMP_USE_SEL9 ))||
    (( mId >= WIFI_FUNC_00CE_HOTTEMP_USE_SEL10 )&&( mId <= WIFI_FUNC_00CF_HOTTEMP_USE_SEL11 )))
    {
        return TRUE;
    }
    return FALSE;
}
U8 IsExceptSleepTimeID ( U16 mId )
{
    if (( mId >= WIFI_FUNC_005F_SLEEP_START_HOUR )&&( mId <= WIFI_FUNC_0062_SLEEP_END_MIN ))
    {
        return TRUE;
    }
    return FALSE;
}

U8 IsExceptWaterAmountID ( U16 mId )
{
    if ( mId == WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL )
    {
        return TRUE;
    }
    return FALSE;
}
U8 IsExceptWaterAmountID_List ( U16 mId )
{
    if (( mId >= WIFI_FUNC_0067_WATER_QUANTITY_USE_SEL0 )&&( mId <= WIFI_FUNC_0071_WATER_QUANTITY_USE_SEL10 ))
    {
        return TRUE;
    }
    return FALSE;
}

/******************************************************************************/
/***** Local Function *********************************************************/
/******************************************************************************/
static void EvtWifiTxFunc ( U16 mId, I8 *pBuf )
{
    static I8 mai8Buf[LENGTH_WIFI_DATA_ONE];

    _MEMSET_( (void __FAR*) mai8Buf, '\0', sizeof(mai8Buf) );

    ConvertDataHexToAscii( TYPE_FUNC_CMD, mId, mai8Buf );
    
    far_sprintf_s( pBuf, 0, mai8Buf );
}

static void EvtWifiRxFunc ( U16 mId, I8 *pBuf )
{
    ApplyFunctionData( mId, pBuf );
}

