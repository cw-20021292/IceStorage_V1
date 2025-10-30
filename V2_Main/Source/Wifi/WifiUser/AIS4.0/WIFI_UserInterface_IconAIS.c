
#include "WIFI_Common.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_SetSensorData.h"
#include "WIFI_SetErrorData.h"
#include "WIFI_SetPartData.h"
#include "WIFI_SetAccumulateData.h"
#include "WIFI_UserInterface_IconAIS.h"
#include "WIFI_Control.h"

/* USER */
#include "sound.h"
#include "front.h"
#include "eol.h"
#include "fct.h"
#include "rtc.h"
#include "error.h"
#include "filter.h"
#include "flush_water_out.h"
#include "cold_water.h"
#include "ice.h"
#include "process_display.h"
#include "hot_water.h"
#include "hot_water_out.h"
#include "water_out.h"
#include "eeprom.h"
#include "temp.h"
#include "power_saving.h"
#include "energy.h"
#include "user.h"
#include "ice_door.h"
#include "uv.h"
#include "voice.h"
#include "service.h"
#include "smart_check.h"
#include "diagnosis.h"
/*sean [23-04-19] ?îÑÎ°úÌÜ†ÏΩ? Ï∂îÍ???óê ?î∞Î•? ?ó§?çî?åå?ùº Ï∂îÍ??*/
#include "comp.h"
#include "ice_full.h"
#include "ice_maker.h"
#ifdef MODEL_NO_1

/*..sean [24-03-19] ?îÑÎ°úÌÜ†ÏΩ? Ï∂îÍ???óê ?î∞Î•? Íµ¨Ï°∞ Î≥?Í≤?..*/
#if 0
#define PROGRESS_ID_UV_WATER_OUT    0
#define PROGRESS_ID_UV_ICE_OUT      1
#define PROGRESS_ID_UV_ICE_TRAY     2
#define PROGRESS_ID_UV_ICE_TANK     3
#define PROGRESS_ID_SILVER_CARE     4
#define PROGRESS_ID_DRAIN_WATER     5
#define MAX_PROGRESS_NUM            6
#else
typedef enum _progress_id_
{
    PROGRESS_ID_UV_WATER_OUT = 0,			// 0 :
    PROGRESS_ID_UV_ICE_OUT,					// 1 :
    PROGRESS_ID_UV_ICE_TRAY,                // 2 :
    PROGRESS_ID_UV_ICE_TANK,				// 3 :
    PROGRESS_ID_SILVER_CARE,				// 4 :
    PROGRESS_ID_DRAIN_WATER,				// 5 :
    // PROGRESS_ID_COMP,                 		// 6 :
    MAX_PROGRESS_NUM,						// 0 :
} EProgressId_T;

#endif
static U16 GetProgressVal(U8 id);


WIFI_Status_T Wifi;

extern U16 WifiRamWriteCheck_address = 0;
extern U8 WifiSmartCheckResultData = 0;

extern DoDummyDeIcing_T DummyDeIcing;
extern DoReady_T Ready;
extern DoIcing_T Icing;

void SetUserSystemRamWriteStatus ( U16 mu16Address)
{
	WifiRamWriteCheck_address = (U16)mu16Address;
}
U16 GetUserSystemRamWriteStatus ( U16 mu16Address )
{
	if (WifiRamWriteCheck_address == mu16Address)
	{
		return WifiRamWriteCheck_address;
	}
	return CLEAR;
}

void UserWriteEep ( U16 mu16Addr, U8 mu8Val )
{
    if( mu16Addr == EEPROM_ADDR_WIFI_POWER )
    {
        Wifi.OnOff  = mu8Val;
        SaveEepromId ( EEP_ID_WIFI_POWER );
    }
    else if( mu16Addr == EEPROM_ADDR_FIRST_PARING )
    {
        Wifi.FirstParing = mu8Val;
        SaveEepromId ( EEP_ID_WIFI_FIRST_PARING );
    }
    else if( mu16Addr == EEPROM_ADDR_FOTA_MODULE )
    {
        Wifi.FotaModule = mu8Val;
        SaveEepromId ( EEP_ID_WIFI_FOTA_MODULE );
    }
    else if( mu16Addr == EEPROM_ADDR_FOTA_MCU )
    {
        Wifi.FotaMain   = mu8Val;
        SaveEepromId ( EEP_ID_WIFI_FOTA_MCU );
    }
}

U8 UserReadEep ( U16 mu16Addr )
{
    if( mu16Addr == EEPROM_ADDR_WIFI_POWER )
    {
        return Wifi.OnOff;
    }
    else if( mu16Addr == EEPROM_ADDR_FIRST_PARING )
    {
        return Wifi.FirstParing;
    }
    else if( mu16Addr == EEPROM_ADDR_FOTA_MODULE )
    {
        return Wifi.FotaModule;
    }
    else if( mu16Addr == EEPROM_ADDR_FOTA_MCU )
    {
        return Wifi.FotaMain;
    }

    return 0;
}

void UserBuzzSound ( U8 mu8Buzz )
{
    U8 mu8Sound = SOUND_NONE;

    switch( mu8Buzz )
    {
        case WIFI_BUZZER_SELECT:
            mu8Sound = SOUND_SELECT;
            break;

        case WIFI_BUZZER_AP_CONNECT:
            mu8Sound = SOUND_WIFI_CONNECTING;
            break;

        case WIFI_BUZZER_SETUP:
            mu8Sound = SOUND_WIFI_CONNECTING;
            break;

        case WIFI_BUZZER_SERVER_CONNECT:
            mu8Sound = SOUND_WIFI_CONNECTED;
            break;

        case WIFI_BUZZER_AP_FAIL:
            mu8Sound = SOUND_WIFI_FAIL_CONNECTING;
            break;

        case WIFI_BUZZER_CANCEL:
            mu8Sound = SOUND_WIFI_DISCONNECTED;
            break;

        case WIFI_BUZZER_AP_START:
            mu8Sound = SOUND_SETUP;
            break;

        case WIFI_BUZZER_ERROR:
            mu8Sound = SOUND_ERROR;
            break;

        case WIFI_BUZZER_SERVER_FIRST:    
            mu8Sound = SOUND_WIFI_CONNECTED_FIRST;
            break;

        case WIFI_BUZZER_BLE_OFF:
            mu8Sound = SOUND_CANCEL;
            break;
    }

    Sound( mu8Sound );
}

U8 GetUserSystemStatus ( U8 mu8Value )
{
    /*
       mVal = USER_MODEL_TYPE;
0 : CHP
1 : CP
mVal = USER_FACTORY_STATUS;
0 CLEAR : Normal
1 SET : Factory/PCB Test mode
mVal = USER_RX_STATUS;
0 FALSE : App Control X (ÔøΩÔøΩÔøΩÔøΩ/ÔøΩ√∑ÔøΩÔøΩÔøΩ/EWÔøΩÔøΩÔø?/ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔø?/ƒøÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ/ÔøΩÔøΩÔøΩÔøΩ≈ªÔøΩÔøΩ)
1 TRUE : App Control O
mVal = USER_SMART_POSSIBLE_STATUS;
0 FALSE : Smart Check X (ÔøΩÔøΩÔøΩÔøΩ/ÔøΩ√∑ÔøΩÔøΩÔøΩ/EWÔøΩÔøΩÔø?/ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔø?)
1 TRUE : Smart Check O
mVal = USER_ERROR_STATUS;
0 FALSE : Normal
1 TRUE : Error
mVal = USER_FOTA_STATUS;
0 FALSE : Fota X (ÔøΩÔøΩÔøΩÔøΩ/ÔøΩ√∑ÔøΩÔøΩÔøΩ/EWÔøΩÔøΩÔø?)
1 TRUE : Fota O
mVal = USER_SMART_SENSING_STATUS;
0 FALSE : Normal
1 TRUE : Smart Checking
*/

    U8 mu8Return = 0;
    if ( mu8Value == USER_MODEL_TYPE )
    {
        if ( GetSystem() == SYSTEM_CP)
        {
            mu8Return = USER_MODEL_CP;
        }
        else
        {
            mu8Return = USER_MODEL_CHP;
        }
    }
    else if ( mu8Value == USER_FACTORY_STATUS ) 
    { 
        /* ÔøΩÔøΩÔøΩÔøΩÔøΩ◊ΩÔøΩ∆ÆÔøΩÔøΩÔø? »ÆÔøΩÔøΩ */
        if ( GetEolStatus() == TRUE 
                  || GetFctStatus() == TRUE 
           )
        {
            mu8Return = SET; // ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
        }
        else
        {
            mu8Return = CLEAR; // ÔøΩœπ›∏ÔøΩÔø?
        }
    }
    else if ( mu8Value == USER_RX_STATUS )
    { 
        // ÔøΩÔøΩÔøΩÔøΩ, ÔøΩÔøΩÔøΩÔøΩ OPEN, ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ
        if ( GetWaterOut() == TRUE 
                || IsOpenFilter( ID_FILTER_COVER ) == FILTER_OPEN 
                || IsOpenFilter( ID_FILTER  ) == FILTER_OPEN 
                || IsDoneFlushInitSetup() == FALSE
                || IsOpenIceDoor() == TRUE 
           )
        {   
            mu8Return = FALSE; // ÔøΩÔøΩÔøΩ√µÔøΩÔøΩ€±ÔøΩÔøΩÔøΩ
        }
        else
        {
            mu8Return = TRUE; // ÔøΩÔøΩÔøΩ√µÔøΩÔøΩ€∞ÔøΩÔøΩÔøΩ
        }
    }
    else if ( mu8Value == USER_SMART_POSSIBLE_STATUS )
    { 
        // ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ,
        // ÔøΩ√∑ÔøΩÔøΩÔøΩ ÔøΩœ∑ÔøΩ ÔøΩÔøΩ
        if ( IsValidStartSmartCheck() == FALSE )
        {
            mu8Return = FALSE; // ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩ“∞ÔøΩ
        }
        else
        {
            mu8Return = TRUE; // ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ
        }

    }
    else if ( mu8Value == USER_ERROR_STATUS )
    { /* ÔøΩÔøΩ«∞ ÔøΩÔøΩÔøΩÔøΩ»ÆÔøΩÔøΩ */
        if ( GetErrorId() != ERR_NONE )
        {
            mu8Return = TRUE;   // ÔøΩÔøΩÔøΩÔøΩ
        }
        else
        {
            mu8Return = FALSE;  // ÔøΩÔøΩÔøΩÔøΩ
        }
    }
    else if ( mu8Value == USER_FOTA_POSSIBLE_STATUS )
    { // ÔøΩÔøΩÔøΩÔøΩ / ÔøΩ√∑ÔøΩÔøΩÔøΩ / EWÔøΩÔøΩÔø?
        if ( GetWaterOut() == TRUE 
                || IsDoneFlushInitSetup() == FALSE
           )
        {
            mu8Return = FALSE; // ÔøΩÔøΩ≈∏ ÔøΩ“∞ÔøΩ
        }
        else
        {
            mu8Return = TRUE; // ÔøΩÔøΩ≈∏ ÔøΩÔøΩÔøΩÔøΩ
        }
    }
    else if ( mu8Value == USER_SMART_SENSING_STATUS )
    {
        if( IsStartDiagnosis() == TRUE )
        {
            mu8Return = TRUE ; // ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ
        }
        else
        {
            mu8Return = FALSE; // ÔøΩœπÔøΩ
        }
    }
    else if ( mu8Value == USER_FACTORY_DIRECTFOTA_STATUS ) // direct fota mode
    {
        /* Direct FOTA (ÔøΩÔøΩ≈•ÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩÔø? ÔøΩÃªÔøΩÔø?)  */

    }
    else
    {
        mu8Return = 0U;
    }
    return mu8Return;
}

/* User Data Clear */
// ƒøÔøΩÔøΩ OPEN ÔøΩ√∞ÔøΩ
// ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ 
// ÔøΩÔøΩ ÔøΩÔøΩÎ∑?
// ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ≈¨ÔøΩÔøΩÔøΩÔøΩ (ÔøΩÔøΩÔøΩÔøΩÔøΩ«æÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩ ∞ÔøΩÔøΩÔøΩÔøΩÔøΩ)
void ClearUserSystemData ( void ) 
{
    // TANK OPEN ÔøΩ√∞ÔøΩ
    ResetServiceOpenTime();

    // ÔøΩ“∫ÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ
    ClearAllEnergyWatt();

    // ÔøΩÔøΩÔøΩÔøΩ ÔøΩ√∑ÔøΩÔøΩÔøΩ ÔøΩœ∑ÔøΩ FLAG
    SetFilterChangedFlushStatus( FILTER_CHANGED_NONE );

    // ÔøΩÔøΩ ÔøΩÔøΩÎ∑?
    ClearAllUsageWater();
}

/* User Display Setting */
// FOTA ÔøΩÔøΩÔøΩÔøΩ ÔøΩÿ∫ÔøΩ ÔøΩÔøΩÔøΩÔøΩ
//  -. ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ...
//  -. WIFI LEDÔøΩÔøΩ 3ÔøΩÔøΩ ON, 1ÔøΩÔøΩ OFF
void SetUserSystemDisplay ( U8 mu8Display )
{
    if ( mu8Display == USER_DISPLAY_FOTA ) // FOTA
    { // FOTA ÔøΩÔøΩÔøΩÔøΩ 5ÔøΩÔøΩ ÔøΩÔøΩ
        SetFotaDisp( TRUE );
    }
}

/* Time Setting */
void SetUserSystemTime ( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec, U16 mDay )
{
    TimeData_T mTime;

    mTime.Y2K     = 20;
    mTime.DayWeek = (U8)mDay;
    mTime.Month   = (U8)mMonth;
    mTime.Date    = (U8)mDate;
    mTime.Hour    = (U8)mHour;
    mTime.Min     = (U8)mMin;
    mTime.Sec     = (U8)mSec;

    SetRtcTime( &mTime );
    SetRtcConfigInit( TRUE );
}


/* User Error Data */
typedef struct _user_wifi_error_
{
    U16 API_Id;
    ErrorId_T ErrorId;
} WifiError_T;

const static WifiError_T ErrorTable[] = 
{
    { WIFI_ERROR_0006_HOTIN_TEMP,       ERR_TEMP_HOT_WATER_IN   },
    { WIFI_ERROR_0007_HOTOUT_TEMP,      ERR_TEMP_HOT_WATER      },
    { WIFI_ERROR_0008_COLD_TEMP,        ERR_TEMP_COLD_WATER     },
    { WIFI_ERROR_0009_NATURAL_TEMP,     ERR_TEMP_AMBIENT        },
    { WIFI_ERROR_000C_BLDC_COMP,        ERR_COMP_COMM           },
    { WIFI_ERROR_000E_DRAIN_PUMP,       ERR_DRAIN_PUMP          },
    { WIFI_ERROR_0019_STOP_WATER,       ERR_OUTAGE_WATER        },
    { WIFI_ERROR_001B_STOP_HOTWATER,    ERR_OUTAGE_HOT_WATER    },
    { WIFI_ERROR_001C_LEAK_FLOW,        ERR_LEAK_FLOW           },
    { WIFI_ERROR_001D_LEAK_SENSOR,      ERR_LEAK                },
    { WIFI_ERROR_001E_HOT_OVER,         ERR_HEATER_OVERHEAT     },
    { WIFI_ERROR_001F_HEATER_SENSOR,    ERR_TEMP_HEATER         },
    { WIFI_ERROR_0020_ROOM_TEMP,        ERR_TEMP_ROOM_WATER     },
    { WIFI_ERROR_0021_MICRO_SW_DETECT,  ERR_MICRO_SW_DETECT     },
    { WIFI_ERROR_0022_MICRO_SW_MOVE,    ERR_MICRO_SW_MOVE       },
    
    { WIFI_ERROR_0049_COMP_CURRENT_SENSING,			ERR_COMP_CURRENT_SENSING		},
    { WIFI_ERROR_004A_COMP_STARTING_FAIL,			ERR_COMP_STARTING_FAIL			},
    { WIFI_ERROR_004B_COMP_OVER_CURRENT,			ERR_COMP_OVER_CURRENT			},
    { WIFI_ERROR_004C_COMP_OVERHEAT,				ERR_COMP_OVERHEAT				},
    { WIFI_ERROR_004D_COMP_DISCONNECTION_OF_COMM,	ERR_COMP_DISCONNECTION_OF_COMM	},
    { WIFI_ERROR_004E_COMP_ABNORMAL_VOLTAGE,		ERR_COMP_ABNORMAL_VOLTAGE       },

    { WIFI_ERROR_0029_BLDC_STATUS,      ERR_COMP                }
};
#define SZ_ERROR_TABLE  (sizeof(ErrorTable)/sizeof(WifiError_T))
U16 GetUserSystemError ( U16 mu16Err )
{
    U8  i;
    U16 mu16Data = 0U;   // 0:Normal / 1:Error 

    for( i = 0; i < SZ_ERROR_TABLE; i++)
    {
        if( ErrorTable[ i ].API_Id == mu16Err )
        {
            mu16Data = GetErrorStatus( ErrorTable[ i ].ErrorId );
            break;
        }
    }

    return mu16Data;
}

static U16 ConvWifiTemp(TEMP_T mTemp)
{
    U16 mu16Val = 0U;


    /* ROUND UP */
    mTemp = mTemp + 0.5f;
    if( mTemp > 999.0f )
    {
        mTemp = 999.0f;
    }
    else if( mTemp < -999.0f )
    {
        mTemp = -999.0f;
    }

    mu16Val = (U16)mTemp;
    return mu16Val;
}

/* User Sensor Data */
I16 GetUserSystemSensor ( U16 mu16Sen )
{
    I16 mu16Data = 0;
    TEMP_T  mTemp;
    U8 mu8Month;
    U32 mu32Watt;

    switch ( mu16Sen )
    {
        case WIFI_SENSOR_0001_COLD_TEMP : // 0 ~ 99
            mTemp = GetTemp( TEMP_ID_COLD_WATER );
            mu16Data = ConvWifiTemp( mTemp );
            break;

        case WIFI_SENSOR_0003_WATER_TEMP : // 0 ~ 99
            mTemp = GetTemp( TEMP_ID_ROOM_WATER );
            mu16Data = ConvWifiTemp( mTemp );
            break;

        case WIFI_SENSOR_0004_OUT_TEMP : //  0 ~ 99
            mTemp = GetTemp( TEMP_ID_AMBIENT );
            mu16Data = ConvWifiTemp( mTemp );
            break;

        case WIFI_SENSOR_000E_WATT_DATA : // 0.01~250.00 (ex -> 1.78 = 178)

            // Watt hour
            mu32Watt = GetEnergyWattAll();
            mu32Watt /= 10;   // #.### -> #.##

            if( mu32Watt > 65535 )
            {
                mu32Watt = 65535;
            }
            else if( mu32Watt < 1 )
            {
                mu32Watt = 1;
            }

            mu16Data = (U16)mu32Watt;

            // ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ ÔøΩ ±ÔøΩ»≠
            ClearAllEnergyWatt();
            break;

        case WIFI_SENSOR_000F_COMP_DATA : //  0 / 1
            // ÔøΩÔøΩ«• RPSÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ RPSÔøΩÔøΩ 0RPS ÔøΩÃªÔøΩÔøΩÔøΩ ÔøΩÔøΩÏø°ÔøΩÔø? ÔøΩÔøΩÔøΩÔøΩÔø? ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩ¬∑ÔøΩ ÔøΩÔøΩÔøΩÔøΩ.
            // Note. BLDC COMPÔøΩÔøΩ ÔøΩÔøΩÔø? ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩﬂªÔøΩÔøΩ«¥ÔøΩ ÔøΩÔøΩÔø?, 
            //       Current RpsÔøΩÔøΩ ÔøΩ›øÔøΩÔøΩ»µ«¥ÔøΩ ÔøΩÔøΩÏ∞? ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ.
            //       ÔøΩ◊∑ÔøΩÔøΩÔøΩ ÔøΩ›µÔøΩÔø? TargetÔøΩÔøΩ Current ÔøΩÔøΩÔøΩÔøΩ ÔøΩ‘≤ÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ.
            if( GetCompBldcTargetRps() > 0 
                    && GetCompBldcCurrentRps() > 0 )
            {
                mu16Data = ON;
            }
            else
            {
                mu16Data = OFF;
            }
            break;

        default:
            mu16Data = 0U;
            break;
    }

    return mu16Data;
}

/* User Part Data */
// ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ
U16 GetUserSystemPart ( U16 mu16Part )
{
    if( IsValidPartID( mu16Part ) == TRUE )
    {
        return GetSmartCheckDataId( mu16Part );
    }

    return 0;  // no parts
}


U32 GetUserSystemAccumulate ( U16 mu16Acc )
{
    U32 mu32Data = 0U;

    switch ( mu16Acc ) // 0 ~ 999999
    {
        case WIFI_ACCM_0000_MONTH_WATER_USER :
            mu32Data = 1000000;
            break;

        case WIFI_ACCM_0004_DAY_WATER_USER :
            mu32Data = 1010;
            break;

        default:
            mu32Data = 0U;
            break;
    }
    /* Max 999999 */
    if ( mu32Data  >= 999999U )
    {
        mu32Data = 999999U;
    }
    return mu32Data;
}

/* App Control -> User Function Data */
#define W_OFF           0
#define W_ON            1

#define W_CLOSE         0
#define W_OPEN          1

#define W_INSTALLED     0
#define W_REMOVED       1

#define W_HOT_UNLOCK    1
#define W_HOT_LOCK      2

#define W_UNLOCK        0
#define W_LOCK          1

#define W_FILTER_NONE   0
#define W_FILTER_RESET  1

#define W_PROGRESS_OFF      0
#define W_PROGRESS_ON       1
#define W_PROGRESS_GOING    2

//#define W_ICE_OFF           0
#define W_ICE_MODE_COLD     1
//#define W_ICE_MODE_BALANCE  2
#define W_ICE_MODE_ICE      3

#define W_COLD_OFF          0
#define W_COLD_MODE_1       1
#define W_COLD_MODE_2       2

#define W_WATER_ROOM        1
#define W_WATER_COLD        2
#define W_WATER_HOT         3

#define W_ICE_LARGE         0
#define W_ICE_SMALL         2

#define W_ICE_OFF           0
#define W_ICE_ON            1

#define W_MIN_SAVIN_HOUR    0
#define W_MAX_SAVIN_HOUR    23

#define W_MIN_SAVIN_MIN     0
#define W_MAX_SAVIN_MIN     59

#define W_SMART_CHECK_START 1
#define W_SMART_CHECK_STOP  0

#define W_SMART_CHECK_ERROR  1
#define W_SMART_CHECK_OK     0


U8 mu8IsSkipSendData = FALSE;
void SetUserSystemFunction ( U16 mu16Func, U16 mData )
{
    U8 mu8IsSendData = FALSE;


    switch ( mu16Func )
    {
        case WIFI_FUNC_0002_COLD_SEL :
            if(mData == W_ON )
            {
                SetColdWaterConfigMake( TRUE );
                Sound( SOUND_SETUP_COLD );
            }
            else if( mData == W_OFF )
            {
                SetColdWaterConfigMake( FALSE );
                Sound( SOUND_CANCEL_COLD );
            } 
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_0003_HOT_LOCK_SEL :  
            if( GetSystem() == SYSTEM_CHP )
            {
                if (mData == W_HOT_UNLOCK)
                {
                    SetLockHot( UNLOCK );
                    Sound( SOUND_UNLOCK_HOT );
                }
                else if(mData == W_HOT_LOCK )
                {
                    SetLockHot( LOCK );
                    Sound( SOUND_LOCK_HOT );
                }   
                else
                {
                    mu8IsSendData = TRUE;
                }
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_0005_LOCK_SEL :
            if ( mData == W_LOCK )
            {
                SetLockAll( LOCK );
                Sound( SOUND_LOCK_ALL );
            }
            else if( mData == W_UNLOCK )
            {
                SetLockAll( UNLOCK );
                Sound( SOUND_UNLOCK_ALL );
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_0008_ICE_SEL:
            if( mData == W_ICE_MODE_ICE )
            {
                SetIceTurbo( TRUE );
                Sound( SOUND_SETUP );
            }
            else if( mData == W_ICE_MODE_COLD )
            {
                ClearMakingIceCount();
                SetIceTurbo( FALSE );
                Sound( SOUND_SETUP );
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_0031_VOICE_SEL :
            if( mData >= MIN_VOICE_LEVEL && mData <= MAX_VOICE_LEVEL )
            {
                U8 mu8Sound;

                SetVoiceLevel( (U8)mData );
                mu8Sound = SOUND_SETTING_MUTE + ( (SoundId_T)mData - 1 );
                Sound( mu8Sound );
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_0032_ICELOCK_SEL :
            if ( mData == W_LOCK )
            {
                SetLockIce( LOCK );
                Sound( SOUND_LOCK_ICE );
            }
            else if( mData == W_UNLOCK )
            {
                SetLockIce( UNLOCK );
                Sound( SOUND_UNLOCK_ICE );
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_0033_VOICE_VOL_SEL :
            if( mData >= MIN_VOICE_VOLUME && mData <= MAX_VOICE_VOLUME )
            {
                SetVoiceVolume( (U8)mData );
                Sound( SOUND_SETUP );
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

            // ÔøΩÔøΩÔøΩ¬ºÔøΩ ÔøΩÔøΩÔø?
        case WIFI_FUNC_003B_HOT_TEMP_SEL :
/*
            if( mData == W_OFF || mData == W_ON )
            {
                SetHotConfigUser( (U8)mData );
                if( mData == W_ON )
                {
                    Sound( SOUND_SETUP );
                }
                else
                {
                    Sound( SOUND_CANCEL );
                }
            }
            else
			}
            {
                mu8IsSendData = TRUE;

            break;
*/
        case WIFI_FUNC_0040_SMART_CHECK_RQST :
            if( mData == W_SMART_CHECK_START )
            {
                StartSmartCheck();
                Sound( SOUND_SMART_DIAG_START );
            }
            else
            {
                StopSmartCheck();
            }
            break;

        case WIFI_FUNC_0047_WATER_QUANTITY_SEL :
#if 0
            if( mData >= MIN_USER_AMOUNT_LITER 
                    && mData <= MAX_USER_AMOUNT_LITER )
            {
                SetWaterOutUserAmountLiter( mData );
                Sound( SOUND_SETUP_MY_AMOUNT );
            }
            else
            {
                mu8IsSendData = TRUE;
            }
#endif
            break;

        case WIFI_FUNC_0049_OUT_BUTTON_LIGHT :
            if( mData == W_OFF || mData == W_ON )
            {
                SetConfigDeco( DECO_LED_BUTTON, (U8)mData );
                if( mData == W_OFF )
                {
                    Sound( SOUND_CANCEL );
                }
                else
                {
                    Sound( SOUND_SETUP );
                }
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_004A_WELCOME_LIGHT :
            if( mData == W_OFF || mData == W_ON )
            {
                SetConfigDeco( DECO_LED_WELCOME, (U8)mData );
                if( mData == W_OFF )
                {
                    Sound( SOUND_CANCEL );
                }
                else
                {
                    Sound( SOUND_SETUP );
                }
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_0051_QUANTITY_USE_SEL :
#if 0
            if( mData == W_OFF || mData == W_ON )
            {
                SetWaterOutConfigUser( (U8)mData );

                if( mData == W_OFF )
                {
                    Sound( SOUND_CANCEL );
                }
                else
                {
                    Sound( SOUND_SETUP );
                }
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;
#endif

        case WIFI_FUNC_0058_ICE_SIZE_SEL :
            if( mData == W_ICE_LARGE || mData == W_ICE_SMALL )
            {
                if( mData == W_ICE_LARGE )
                {
                    SetIceSize( ICE_LARGE );
                }
                else 
                {
                    SetIceSize( ICE_SMALL );
                }

                Sound( SOUND_SETUP );
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_0059_ICE_ONOFF_SEL :
            if( mData == W_ICE_OFF || mData == W_ICE_ON )
            {
                if( mData == W_ICE_OFF )
                {
                    Sound( SOUND_CANCEL_ICE );
                    SetIceConfigMake( FALSE );
                }
                else
                {
                    Sound( SOUND_SETUP_ICE );
                    SetIceConfigMake( TRUE );
                }
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

            // ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔø?
        case WIFI_FUNC_005C_SMART_CHECK_RESULT :
            WifiSmartCheckResultData = (U8)mData;
            if( mData == W_SMART_CHECK_OK )
            {
                Sound( SOUND_SMART_DIAG_NO_ERR_DONE );
            }
            else
            {
                Sound( SOUND_SMART_DIAG_ERR_DONE );
            }
            break;

        case WIFI_FUNC_005E_SLEEP_MODE_SEL :
            if( mData == W_OFF || mData == W_ON )
            {
                SetSavingConfig( (U8)mData );
                if( mData == W_OFF )
                {
                    Sound( SOUND_CANCEL_SLEEP );
                }
                else
                {
                    Sound( SOUND_SETUP_SLEEP );
                }
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_005F_SLEEP_START_HOUR :
            if( mData >= W_MIN_SAVIN_HOUR
                    && mData <= W_MAX_SAVIN_HOUR )
            {
                SetSavingAlarmHour( ALARM_ON, (U8)mData);
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_0060_SLEEP_START_MIN :
            if( mData >= W_MIN_SAVIN_MIN
                    && mData <= W_MAX_SAVIN_MIN )
            {
                SetSavingAlarmMin( ALARM_ON, (U8)mData);
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_0061_SLEEP_END_HOUR :
            if( mData >= W_MIN_SAVIN_HOUR
                    && mData <= W_MAX_SAVIN_HOUR )
            {
                SetSavingAlarmHour( ALARM_OFF, (U8)mData);
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;

        case WIFI_FUNC_0062_SLEEP_END_MIN :
            if( mData >= W_MIN_SAVIN_MIN
                    && mData <= W_MAX_SAVIN_MIN )
            {
                SetSavingAlarmMin( ALARM_OFF, (U8)mData);
                // SLEEP ÔøΩÔøΩÔøΩÔøΩÔøΩÕ¥ÔøΩ 4ÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩ—πÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩﬁµ«π«∑ÔøΩ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩ—πÔøΩÔøΩÔøΩ ÔøΩÔ∏≤
                Sound( SOUND_SETUP );       
            }
            else
            {
                mu8IsSendData = TRUE;
            }
            break;
		
#ifdef CONFIG_WIFI_WATER_AMOUNT

        case WIFI_FUNC_00BE_QUANTITY_HALFCUP :
            Sound( SOUND_SETUP );  
			SetWifiWaterAmountConfig(WATER_AMOUNT_HALF, mData);
            break;
			
        case WIFI_FUNC_00BF_QUANTITY_ONECUP :
			SetWifiWaterAmountConfig(WATER_AMOUNT_ONE, mData);
            break;
			
        case WIFI_FUNC_00C0_QUANTITY_TWOCUPS :
			SetWifiWaterAmountConfig(WATER_AMOUNT_TWO, mData);
            break;
#endif
		
#ifdef CONFIG_WIFI_HEAT_TEMP
        case WIFI_FUNC_00C1_HOTTEMP_USE_SEL0 :
			Sound( SOUND_SETUP );   
            SetWifiTempConfig(HOT_WATER_ID_HOT_MILK, mData);
            break;
		
        case WIFI_FUNC_00C2_HOTTEMP_USE_SEL1 :
            SetWifiTempConfig(HOT_WATER_ID_DEGREE_50, mData);
            break;
		
        case WIFI_FUNC_00C3_HOTTEMP_USE_SEL2 :
            SetWifiTempConfig(HOT_WATER_ID_DEGREE_55, mData);
            break;
		
        case WIFI_FUNC_00C4_HOTTEMP_USE_SEL3 :
            SetWifiTempConfig(HOT_WATER_ID_DEGREE_60, mData);
            break;
		
        case WIFI_FUNC_00C5_HOTTEMP_USE_SEL4 :
            SetWifiTempConfig(HOT_WATER_ID_DEGREE_65, mData);
            break;
		
        case WIFI_FUNC_00C6_HOTTEMP_USE_SEL5 :
            SetWifiTempConfig(HOT_WATER_ID_HOT_TEA, mData);
            break;
		
        case WIFI_FUNC_00C7_HOTTEMP_USE_SEL6 :
            SetWifiTempConfig(HOT_WATER_ID_DEGREE_75, mData);
            break;
		
        case WIFI_FUNC_00C8_HOTTEMP_USE_SEL7 :
            SetWifiTempConfig(HOT_WATER_ID_DEGREE_80, mData);
            break;
		
        case WIFI_FUNC_00C9_HOTTEMP_USE_SEL8 :
            SetWifiTempConfig(HOT_WATER_ID_HOT_COFFEE, mData);
            break;
		
        case WIFI_FUNC_00CA_HOTTEMP_USE_SEL9 : // 90
            SetWifiTempConfig(HOT_WATER_ID_DEGREE_90, mData);

            break;
        case WIFI_FUNC_00CE_HOTTEMP_USE_SEL10 : // 95
            SetWifiTempConfig(HOT_WATER_ID_DEGREE_95, mData);

            break;
        case WIFI_FUNC_00CF_HOTTEMP_USE_SEL11 : // 100
            SetWifiTempConfig(HOT_WATER_ID_HOT_USER, mData);
            break;
#endif
            
        default:
            break;
    }


    if( mu8IsSendData == TRUE )
    {
        WifiSendData( WIFI_DATA_FUNCTION );
    }
    else
    {
        mu8IsSkipSendData = TRUE;
    }
}


static U8 GetVal( U8 (*pFun)(void), U8 Val, U8 mTrue, U8 mFalse )
{
    if( pFun() == Val )
    {
        return mTrue;
    }

    return mFalse;
}

U32 GetUserSystemRamMonitor ( U16 mu16Func, U16 mLen )
{
	U32 mu32Data = 0U;
	__near U8 *mPRamAddress_read; // 1byte
	U16 mu16temp_2byte_value = 0U;
	U32 mu32temp_4byte_value = 0U;

	if ((mu16Func > Z_rammax) || (mu16Func < Z_rammin))
	{ 
		return mu32Data;
	}

	if ((mLen == 0) && (mLen > 4))
	{
		return mu32Data;
	}
	mPRamAddress_read = (__near U8*)mu16Func;  
	mu32Data = *mPRamAddress_read; 

	if (mLen > 1)
	{
		mPRamAddress_read++;
		mu16temp_2byte_value = *mPRamAddress_read;
		mu32Data += (U16)((mu16temp_2byte_value & 0x00FF) << 8);

	}
	if (mLen > 2)
	{
		mPRamAddress_read++;
		mu32temp_4byte_value = *mPRamAddress_read;
		mPRamAddress_read++;
		mu16temp_2byte_value = *mPRamAddress_read;
		mu32temp_4byte_value += (U16)((mu16temp_2byte_value & 0x00FF) << 8);
		mu32Data += (U32)((mu32temp_4byte_value & 0x0000FFFF) << 16);
	}
	return mu32Data;
}

void  SetUserSystemRamMonitor ( U16 mu16address, I8 *mData )
{
	__near U8 *mPRamAddress;
	U16 mLen = 0U;
	U16 mRWMode = 0U;
	U32 mRamWriteData_4byte = 0U;
	U32 mRamWriteData_4byte_buf = 0U;
	U8 mi = 0U;
	U8 mDec = 0U;
	U32 mDEC_Positional_Number = 1U;

	mLen = WIFI_ASCII2HEX(mData[0]) & 0x000F;	
	mRWMode = WIFI_ASCII2HEX(mData[1]) & 0x000F;  

	if ((mu16address > Z_rammax) || (mu16address < Z_rammin)){ 
		return;
	}
	if (mRWMode != WiFi_M_RAM_Write) 
	{
		return;
	}
	for (mi=0; mi<(mData[LENGTH_WIFI_RAM_DATA-1]-1); mi++) 
	{
		mDEC_Positional_Number *= 10; 
	}
	for (mi=0; mi<mData[LENGTH_WIFI_RAM_DATA-1]; mi++)
	{
		mDec = (U8)((mData[2+mi])-0x30);		
		mRamWriteData_4byte += mDec * (mDEC_Positional_Number); 
		mDEC_Positional_Number /= 10;	
	}

	mPRamAddress = (__near U8*)mu16address;
	mRamWriteData_4byte_buf = mRamWriteData_4byte;
	for (mi = 0; mi < mLen; mi++)
	{
		*mPRamAddress = (U8)(mRamWriteData_4byte_buf & 0x000000FF);	
		mRamWriteData_4byte_buf = mRamWriteData_4byte_buf  >> 8;
		*(mPRamAddress++);	
	}

	SetUserSystemRamWriteStatus((U16)mu16address);
}

////////////////////////////////////////////////////////////////////////////////
// ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ»≠ ÔøΩÔøΩÔøΩÔøΩ 
////////////////////////////////////////////////////////////////////////////////

typedef struct _progress_
{
    U8 Status;      // W_PROGRESS_OFF, ..GOING, ..ON

    U8 PrevVal;     // ON, OFF
    U8 CurVal;      // ON, OFF
    U8 (*pGetStatus)(void);
} Progress_T;

static U8 GetStatusUvWaterOut(void);
static U8 GetStatusUvIceOut(void);
static U8 GetStatusUvIceTray(void);
static U8 GetStatusUvIceTank(void);
static U8 GetStatusSilverCare(void);
static U8 GetStatusDrainWater(void);
static U8 GetStatusComp(void);

Progress_T  ProgressList[ MAX_PROGRESS_NUM ] = 
{
    { W_PROGRESS_OFF,  0,   0, GetStatusUvWaterOut }, 
    { W_PROGRESS_OFF,  0,   0, GetStatusUvIceOut  }, 
    { W_PROGRESS_OFF,  0,   0, GetStatusUvIceTray }, 
    { W_PROGRESS_OFF,  0,   0, GetStatusUvIceTank }, 
    { W_PROGRESS_OFF,  0,   0, GetStatusSilverCare }, 
    { W_PROGRESS_OFF,  0,   0, GetStatusDrainWater }, 
    // { W_PROGRESS_OFF,  0,   0, GetStatusComp }, 
};

static U8 GetProgressStatus(U8 id )
{
    return ProgressList[ id ].Status;
}

static void SetProgressStatus(U8 id, U8 mu8Status)
{
    ProgressList[ id ].Status = mu8Status;
}

static U16 GetProgressVal(U8 id)
{
    U16 mu16Data;

    mu16Data = GetProgressStatus( id );
    if( mu16Data == W_PROGRESS_ON )
    {
        SetProgressStatus( id, W_PROGRESS_GOING );
    }

    return mu16Data;
}

// ÔøΩƒøÔøΩÔø? UV ÔøΩÔøΩÔø?
static U8 GetStatusUvWaterOut(void)
{
    if( IsTurnOnUvId( UV_WATER_OUT ) == TRUE )
    {
        return ON;
    }

    return OFF;
}

static U8 GetStatusUvIceOut(void)
{
    if( IsTurnOnUvId( UV_ICE_DOOR ) == TRUE )
    {
        return ON;
    }

    return OFF;
}

static U8 GetStatusUvIceTray(void)
{
    if( IsTurnOnUvId( UV_ICE_TRAY ) == TRUE )
    {
        return ON;
    }

    return OFF;
}

static U8 GetStatusUvIceTank(void)
{
    if( IsTurnOnUvId( UV_ICE_TANK ) == TRUE )
    {
        return ON;
    }

    return OFF;
}

// ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩ«¥ÔøΩ ÔøΩ√ºÔøΩ/ÔøΩÔøΩÔøΩÔøΩ/ÔøΩ¬ºÔøΩ ÔøΩÔøΩÔøΩÔøΩ
static U8 GetStatusSilverCare(void)
{
    // ÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÃªÔøΩÔø? ÔøΩ√∞ÔøΩÔøΩÔøΩ 48ÔøΩ√∞ÔøΩÔøΩÔøΩ ÔøΩ ∞ÔøΩÔøΩœ∏ÔøΩ...
    if( GetSystemUnusedTime() >= SILVER_CARE_TIME_MIN )
    {
        return ON;  // ÔøΩÔøΩÔø? ÔøΩÔøΩÔøΩÔøΩ
    }

    return OFF; // ÔøΩÔøΩÔø? ÔøΩÔøΩÔøΩÔøΩ.
}

static U8 GetStatusDrainWater(void)
{
    return OFF;
}

static U8 GetStatusComp(void)
{
	// if( GetCompMode() != CLEAR )
    if( GetCompBldcTargetRps() > 0 
        && GetCompBldcCurrentRps() > 0 )
	{
		return ON;
	}
	else
	{
    	return OFF;
	}
}

// ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ»≠ÔøΩÔøΩ ÔøΩ÷∞ÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔø?..
static U8 CheckStateType3(void)
{
    U8 i;
    Progress_T *pList;
    U8 IsChangedStatus = FALSE;

    for( i = 0; i < MAX_PROGRESS_NUM; i++ )
    {
        pList = &ProgressList[ i ];

        pList->CurVal = pList->pGetStatus();
        if( pList->PrevVal != pList->CurVal )
        {
            pList->PrevVal = pList->CurVal;
            if( pList->CurVal == ON )
            {
                if( pList->Status == W_PROGRESS_OFF )
                {
                    pList->Status = W_PROGRESS_ON;
                    IsChangedStatus = TRUE;
                }
            }
            else if( pList->CurVal == OFF )
            {
                pList->Status = W_PROGRESS_OFF;
                IsChangedStatus = TRUE;
            }
        }
    }

    return IsChangedStatus;
}

/////////////////////////////////////////////////////////////////////////////
// ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÃ∫ÔøΩ∆Æ ÔøΩﬂªÔøΩÔøΩÔøΩ √≥ÔøΩÔøΩ
//  1. ÔøΩ√ºÔøΩ ON/OFF
//  2. ÔøΩÔøΩÔøΩÔøΩ ON/OFF
//  ...
//  3. ÔøΩÔøΩÔøΩÔøΩ...
//  4. ÔøΩÔøΩÔøΩÔøΩ...
//  5. ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ/ÔøΩÔøΩÔøΩÔøΩ
//  6. ÔøΩÔøΩÔøΩÔøΩ ƒøÔøΩÔøΩ
//  ...
/////////////////////////////////////////////////////////////////////////////
typedef struct _status_chagend_event_
{
    U16 Count;
    U16 PrevVal;
    U16 CurVal;
    U8 (*pGetStatus)(void);

} StatusEvent_T;

static U8 GetDeco_WelcomLed(void);
static U8 GetDeco_ButtonLed(void);
static U8 GetFilterStatus_Filter(void);
static U8 GetFilterStatus_FilterCover(void);
static U8 GetWaterOutUserAmountLiterEx(void);

StatusEvent_T   StatusEventList[] = 
{
    {  0, 0,   0, GetColdWaterConfigMake }, 

    {  0, 0,   0, GetIceConfigMake     }, 
    {  0, 0,   0, GetIceTurbo          }, 
    {  0, 0,   0, GetIceSize           }, 
    {  0, 0,   0, GetLockIce           }, 
    {  0, 0,   0, IsOpenIceDoor        }, 

//    {  0, 0,   0, GetHotConfigUser     }, 
    {  0, 0,   0, GetLockHot           }, 
    {  0, 0,   0, GetLockAll           }, 
    {  0, 0,   0, GetSavingConfig      }, 

    {  0, 0,   0, GetVoiceLevel        }, 
    {  0, 0,   0, GetVoiceVolume       }, 

    {  0, 0,   0, GetDeco_WelcomLed    },
    {  0, 0,   0, GetDeco_ButtonLed    },

    {  0, 0,   0, GetWaterOut          }, 

    {  0, 0,   0, GetFilterStatus_Filter       }, 
    {  0, 0,   0, GetFilterStatus_FilterCover  }, 
    {  0, 0,   0, GetFilterChangedFlushStatus  }, 

    {  0, 0,   0, GetWaterOutUserAmountLiterEx }, 
//    {  0, 0,   0, GetWaterOutConfigUser },

    {  0, 0,   0, GetSmartCheckStatus   },
    {  0, 0,   0, GetSmartCheck         },
    {  0, 0,   0, GetDiagnosisProgress  },

    {  0, 0,   0, GetServiceCheckTankOpen  }
};
#define SZ_STATUS_EVENT_LIST    (sizeof(StatusEventList)/sizeof(StatusEvent_T))

static U8 GetDeco_WelcomLed(void)
{
    return GetConfigDeco( DECO_LED_WELCOME );
}

static U8 GetDeco_ButtonLed(void)
{
    return GetConfigDeco( DECO_LED_BUTTON );
}

static U8 GetFilterStatus_Filter(void)
{
    return IsOpenFilter( ID_FILTER );
}

static U8 GetFilterStatus_FilterCover(void)
{
    return IsOpenFilter( ID_FILTER_COVER );
}

#define DELAY_TIME  2   // 2sec @1sec
static U8 GetWaterOutUserAmountLiterEx(void)
{
    static U8 InitPowrOn = FALSE;
    static U8 IsChangedStatus = FALSE;
    static U8 mu8DelayTime = DELAY_TIME;
    static U32 mu32PreVal   = 0;
    U32 mu32CurVal = 0;

    // ÔøΩ ±ÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ ÔøΩ›øÔøΩ
    if( InitPowrOn == FALSE )
    {
        InitPowrOn = TRUE;

		/* sean [24-03-27] ?†ú?íà ?û¨?ãú?ûë?ãú, Î∞òÏªµ/?ïúÏª?/?ëêÏª? ?Ç¨?ñë Î∂àÎü¨????Ñú ?Ñ§?†ï */
		//mu32PreVal = GetWaterOutUserAmount();
        return FALSE;
    }

    //mu32CurVal = GetWaterOutUserAmount();
    if( mu32PreVal != mu32CurVal )
    {
        mu32PreVal = mu32CurVal;

        IsChangedStatus = TRUE;
        mu8DelayTime = DELAY_TIME;
    }

    // ÔøΩÔøΩÔøΩÔøΩ ÔøΩ√∞ÔøΩÔøΩÔøΩ ÔøΩÿ¥ÔøΩ.
    // ÔøΩÔøΩ∆∞ ÔøΩÔøΩÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩ‘∑¬µ«¥ÔøΩ ÔøΩÔøΩÔø?...
    if( IsChangedStatus == TRUE )
    {
        if( mu8DelayTime != 0 )
        {
            mu8DelayTime--;
        }
        else
        {
            IsChangedStatus = FALSE;
            return TRUE;
        }
    }

    return FALSE;
}


// ÔøΩÔøΩÔøΩÔøΩ ÔøΩÔøΩ»≠ÔøΩÔøΩ ÔøΩ÷¥ÔøΩ ÔøΩÔøΩÔø?.. 
// ÔøΩÔøΩ»≠ÔøΩÔøΩ ÔøΩ÷∞≈≥ÔøΩ ÔøΩÔøΩÔøΩ≈≥ÔøΩ...
static U8 CheckStateType2(void)
{
    U8 i;
    StatusEvent_T *pList;
    U8 IsChangedStatus = FALSE;

    for( i = 0; i < SZ_STATUS_EVENT_LIST; i++ )
    {
        pList = &StatusEventList[ i ];

        pList->CurVal = pList->pGetStatus();
        if( pList->PrevVal != pList->CurVal )
        {
            pList->PrevVal = pList->CurVal;

            pList->Count++;
            IsChangedStatus = TRUE;
        }
    }

    return IsChangedStatus;
}

U8 dbg_iot_fun = 0; // for dbg...
void ProcessChangedStatus(void)
{
    if( CheckStateType2() == TRUE || CheckStateType3() == TRUE )
    {
        dbg_iot_fun++;

        if( mu8IsSkipSendData == TRUE )
        {
            mu8IsSkipSendData = FALSE;
        }
        else
        {
            WifiSendData( WIFI_DATA_FUNCTION );
        }
    }
}


/* User Function Data */
U16 GetUserSystemFunction ( U16 mu16Func )
{
    U16 mu16Data = 0U;
    TEMP_T  mTemp;

    switch ( mu16Func )
    {        
        case WIFI_FUNC_0002_COLD_SEL :
            mu16Data = GetVal( GetColdWaterConfigMake, TRUE, W_ON, W_OFF );
            break;

        case WIFI_FUNC_0003_HOT_LOCK_SEL :
            mu16Data = GetVal( GetLockHot, LOCK, W_HOT_LOCK, W_HOT_UNLOCK );
            break;

        case WIFI_FUNC_0005_LOCK_SEL : 
            mu16Data = GetVal( GetLockAll, LOCK, W_LOCK, W_UNLOCK );
            break;

        case WIFI_FUNC_0008_ICE_SEL:
            if( GetIceTurbo() == TRUE )
            {
                mu16Data = W_ICE_MODE_ICE;
            }
            else
            {
                mu16Data = W_ICE_MODE_COLD;
            }
            break;

        case WIFI_FUNC_000A_COLD_TEMP : 
#if 0
            mu16Data = GetVal( GetColdWaterConfigMake, FALSE, W_COLD_OFF, W_COLD_MODE_2 );
#else
            if( GetColdWaterConfigMake() == FALSE )
            {
                mu16Data = W_COLD_OFF;
            }
            else
            {
                if( GetColdWaterTempStatus() == COLD_STATUS_GOOD )
                {
                    mu16Data = W_COLD_MODE_2;
                }
                else
                {
                    mu16Data = W_COLD_MODE_1;
                }

            }
#endif

            break;

        case WIFI_FUNC_000B_WATER_SEL : 
            {
                U8 mu8Sel;

                mu8Sel = GetWaterOutSelect();
                if( mu8Sel == SEL_WATER_COLD )
                {
                    mu16Data = W_WATER_COLD;
                }
                else if( mu8Sel == SEL_WATER_HOT )
                {
                    mu16Data = W_WATER_HOT;
                }
                else 
                {
                    mu16Data = W_WATER_ROOM;
                }
            }
            break;        

        case WIFI_FUNC_000D_WATER_OUTQUANTITY :
            mu16Data = GetUserUsageDailyWater( USER_NONE ) * 10U /* 10ml */; 
            ClearAllUsageWater();
            break;

            // ÔøΩÔøΩÔøΩÔøΩ/ÔøΩ√ºÔøΩ ÔøΩÔøΩÔøΩÔøΩ ÔøΩ√øÔøΩÔøΩÔøΩ 0ÔøΩÔøΩÔøΩÔøΩ √≥ÔøΩÔøΩ
        case WIFI_FUNC_001A_HOT_INTEMP :
            if( GetWaterOutLastedSelect() != SEL_WATER_HOT )
            {
                mu16Data = 0;
            }
            else
            {
                TEMP_T mTemp;
                mTemp = GetTemp( TEMP_ID_HOT_IN );
                mu16Data = ConvWifiTemp( mTemp );
            }
            break;

        case WIFI_FUNC_001B_HOT_MEANTEMP :
            if( GetWaterOutLastedSelect() != SEL_WATER_HOT )
            {
                mu16Data = 0;
            }
            else
            {
                TEMP_T mTemp;
                mTemp = GetHotOutAverageOutTemp();
                mu16Data = ConvWifiTemp( mTemp );
            }
            break;

        case WIFI_FUNC_001C_HOT_SELTEMP :
            if( GetWaterOutLastedSelect() != SEL_WATER_HOT )
            {
                mu16Data = 0;
            }
            else
            {
                U8 mu8SelHot;

                mu8SelHot = GetHotSelect();
                mu16Data = GetHotSelectTemp( mu8SelHot );
            }
            break;

        case WIFI_FUNC_001F_DRAIN_STATUS :
            mu16Data = GetProgressVal( PROGRESS_ID_DRAIN_WATER );
            break;

            // TANK OPEN ÔøΩ√∞ÔøΩ... (sec)
        case WIFI_FUNC_0024_HEART_TIME4 :
            mu16Data = GetServiceOpenTime();
            ResetServiceOpenTime();
            break;

        case WIFI_FUNC_002C_SILVER_CARE : 
            mu16Data = GetProgressVal( PROGRESS_ID_SILVER_CARE );
            break;

        case WIFI_FUNC_0030_ICE_MAKETIME:
            mu16Data = GetIceDoorOpeningTimeIot();
            SetIceDoorOpeningTimeIot( 0U );
            break;
            
        case WIFI_FUNC_0031_VOICE_SEL :
            mu16Data = GetVoiceLevel();
            break;

        case WIFI_FUNC_0032_ICELOCK_SEL:
            mu16Data = GetVal( GetLockIce, LOCK, W_LOCK, W_UNLOCK );
            break;

        case WIFI_FUNC_0033_VOICE_VOL_SEL :
            mu16Data = GetVoiceVolume();
            break;

        case WIFI_FUNC_0035_COVER1_OPEN :
            if( IsOpenFilter( ID_FILTER_COVER ) == FILTER_OPEN )
            {
                mu16Data = W_OPEN;
            }
            else
            {
                mu16Data = W_CLOSE;
            }
            break;
	case WIFI_FUNC_0036_COVER2_OPEN :
	    if( GetServiceCheckTankOpen() == W_OPEN )
            {
                mu16Data = W_OPEN;
            }
            else
            {
                mu16Data = W_CLOSE;
            }
	    break;

        case WIFI_FUNC_003B_HOT_TEMP_SEL :
            //mu16Data = GetHotConfigUser();
            break;

        case WIFI_FUNC_0040_SMART_CHECK_RQST :
            mu16Data = GetSmartCheckStatusId(WIFI_FUNC_0040_SMART_CHECK_RQST);
            break;

        case WIFI_FUNC_0041_SMART_CHECK_STATUS :
            mu16Data = GetSmartCheckStatusId(WIFI_FUNC_0041_SMART_CHECK_STATUS);
            break;

        case WIFI_FUNC_0042_SMART_CHECK_PROGRESS :
            mu16Data = GetSmartCheckStatusId(WIFI_FUNC_0042_SMART_CHECK_PROGRESS);
            break;

        case WIFI_FUNC_0043_FILTER_STATUS1 :
            if (IsOpenFilter( ID_FILTER ) == FILTER_OPEN )
            {   
                mu16Data = W_REMOVED;
            }
            else
            {
                mu16Data = W_INSTALLED;
            }
            break;

        case WIFI_FUNC_0047_WATER_QUANTITY_SEL :
            //mu16Data = GetWaterOutUserAmountLiter();
            break;

        case WIFI_FUNC_0049_OUT_BUTTON_LIGHT:
            if( GetConfigDeco( DECO_LED_BUTTON ) == TRUE )
            {
                mu16Data = W_ON;
            }
            else
            {
                mu16Data = W_OFF;
            }
            break;

        case WIFI_FUNC_004A_WELCOME_LIGHT :
            if( GetConfigDeco( DECO_LED_WELCOME ) == TRUE )
            {
                mu16Data = W_ON;
            }
            else
            {
                mu16Data = W_OFF;
            }
            break;

        case WIFI_FUNC_0050_UV_STERILIZE_STATUS :
            mu16Data = GetProgressVal( PROGRESS_ID_UV_WATER_OUT );
            break;

        case WIFI_FUNC_0051_QUANTITY_USE_SEL :
            //mu16Data = GetVal( GetWaterOutConfigUser, TRUE, W_ON, W_OFF );
            break;

        case WIFI_FUNC_0053_FILTER_RESET1 :
            mu16Data = GetVal( GetFilterChangedFlushStatus, FILTER_CHANGED_FLUSH, W_FILTER_RESET, W_FILTER_NONE );
            if( mu16Data == W_FILTER_RESET )
            {
                // Reset Filter 
                ResetFilterUsage();
                SetFilterChangedFlushStatus( FILTER_CHANGED_NONE );
            }
            break;

        case WIFI_FUNC_0058_ICE_SIZE_SEL :
            if( GetIceSize() == ICE_LARGE )
            {
                mu16Data = W_ICE_LARGE;
            }
            else
            {
                mu16Data = W_ICE_SMALL;
            }
            break;

        case WIFI_FUNC_0059_ICE_ONOFF_SEL :
            mu16Data = (U16)GetIceConfigMake();
            break;
            
        case WIFI_FUNC_005A_UV_ICE_TANK_STATUS :
            mu16Data = GetProgressVal( PROGRESS_ID_UV_ICE_TANK );
            break;
            
        case WIFI_FUNC_005B_UV_ICE_TRAY_STATUS :
            mu16Data = GetProgressVal( PROGRESS_ID_UV_ICE_TRAY );
            break;
            
        case WIFI_FUNC_005C_SMART_CHECK_RESULT :
            // No modification required. It is used for public use.
			mu16Data = WifiSmartCheckResultData;
			WifiSmartCheckResultData = 0U;
            break;
            
        case WIFI_FUNC_005D_UV_ICE_FAUCET_STATUS :
            mu16Data = GetProgressVal( PROGRESS_ID_UV_ICE_OUT );
            break;

        case WIFI_FUNC_005E_SLEEP_MODE_SEL :
            mu16Data = (U16)GetSavingConfig();
            break;
            
        case WIFI_FUNC_005F_SLEEP_START_HOUR :
            mu16Data = (U16)GetSavingAlarmHour( ALARM_ON );
            break;
            
        case WIFI_FUNC_0060_SLEEP_START_MIN :
            mu16Data = (U16)GetSavingAlarmMin( ALARM_ON );
            break;
            
        case WIFI_FUNC_0061_SLEEP_END_HOUR :
            mu16Data = (U16)GetSavingAlarmHour( ALARM_OFF );
            break;
            
        case WIFI_FUNC_0062_SLEEP_END_MIN :
            mu16Data = (U16)GetSavingAlarmMin( ALARM_OFF );
            break;

        case WIFI_FUNC_0078_COLD_TARGET_TEMP_TIME :   // ?Éâ?àò Î™©Ìëú ?ò®?èÑ ?èÑ?ã¨ ?ãúÍ∞? - COMP OFF ?ãú ?†Ñ?Ü°(0~9999Ï¥?)
        /*
            if ( GetProgressStatus( PROGRESS_ID_COMP ) == W_PROGRESS_OFF )
            {
                mu16Data = GetWifiCompTime();

                if( mu16Data > 9999U )
                {
                    mu16Data = 9999U;
                }
                ResetWifiCompTime();
            }          
        
            break;
        */
        case WIFI_FUNC_008C_COMP_STATUS :
            // mu16Data = GetProgressVal( PROGRESS_ID_COMP );

            break;
        case WIFI_FUNC_008F_ICE_FULL_STATUS :
			
			if(GetIceFullStatus() == TRUE)
			{
				if(GetInitIceFullStatus() == FALSE)
				{
					SetInitIceFullStatus(TRUE);
					mu16Data = 1;
				}
				else
				{
					mu16Data = 2;
				}
			}
			else
			{
				SetInitIceFullStatus(FALSE);
				mu16Data = 0;
			}

            break;
        case WIFI_FUNC_0093_ICEMAKING_STAUTS :
				mu16Data = GetWifiIceMakeMode();
            break;
        case WIFI_FUNC_0095_ICEMAKING_COMPLET_TIME :
				mu16Data = Icing.IcingTime;
            break;
        case WIFI_FUNC_0096_DEICING_COMPLET_TIME :
				mu16Data = DummyDeIcing.OnOffTime;
            break;
        case WIFI_FUNC_0097_TARY_WATER_COMPLET_TIME :
				mu16Data = 150 - Ready.RoomOutTimeOut; // ROOM_OUT_TIME_OUT_TIME = 150
            break;
        case WIFI_FUNC_0098_TARY_WATER_QUANTITY :
				mu16Data = (U16)Ready.TrayFeedAmount;
            break;
        case WIFI_FUNC_009F_ICE_EVA_TEMP :
            //mTemp = GetTemp( TEMP_ID_EVA_1 );
            ///mu16Data = ConvWifiTemp( mTemp );
            mu16Data = 11;

            break;
        case WIFI_FUNC_00A0_COLD_TANK_TEMP :
            //mTemp = GetTemp( TEMP_ID_COLD_WATER );
            //mu16Data = ConvWifiTemp( mTemp );
			mu16Data = 12;

            break;

#ifdef CONFIG_WIFI_WATER_AMOUNT
        case WIFI_FUNC_00BE_QUANTITY_HALFCUP :
            mu16Data = (U16)GetWaterAmountConfig(FIXED_AMOUNT_HALF);
            break;
        case WIFI_FUNC_00BF_QUANTITY_ONECUP :
            mu16Data = (U16)GetWaterAmountConfig(FIXED_AMOUNT_ONE);
            break;
        case WIFI_FUNC_00C0_QUANTITY_TWOCUPS :
            mu16Data = (U16)GetWaterAmountConfig(FIXED_AMOUNT_TWO);
            break;
#endif   
		
#ifdef CONFIG_WIFI_HEAT_TEMP
        case WIFI_FUNC_00C1_HOTTEMP_USE_SEL0 :
            mu16Data = GetTempConfig(HOT_WATER_ID_HOT_MILK);
            break;
        case WIFI_FUNC_00C2_HOTTEMP_USE_SEL1 :
            mu16Data = GetTempConfig(HOT_WATER_ID_DEGREE_50);
            break;
        case WIFI_FUNC_00C3_HOTTEMP_USE_SEL2 :
            mu16Data = GetTempConfig(HOT_WATER_ID_DEGREE_55);

            break;
        case WIFI_FUNC_00C4_HOTTEMP_USE_SEL3 :
            mu16Data = GetTempConfig(HOT_WATER_ID_DEGREE_60);

            break;
        case WIFI_FUNC_00C5_HOTTEMP_USE_SEL4 :
            mu16Data = GetTempConfig(HOT_WATER_ID_DEGREE_65);

            break;
        case WIFI_FUNC_00C6_HOTTEMP_USE_SEL5 :
            mu16Data = GetTempConfig(HOT_WATER_ID_HOT_TEA);

            break;
        case WIFI_FUNC_00C7_HOTTEMP_USE_SEL6 :
            mu16Data = GetTempConfig(HOT_WATER_ID_DEGREE_75);

            break;
        case WIFI_FUNC_00C8_HOTTEMP_USE_SEL7 :
            mu16Data = GetTempConfig(HOT_WATER_ID_DEGREE_80);

            break;
        case WIFI_FUNC_00C9_HOTTEMP_USE_SEL8 :
            mu16Data = GetTempConfig(HOT_WATER_ID_HOT_COFFEE);

            break;
        case WIFI_FUNC_00CA_HOTTEMP_USE_SEL9 : // 90
            mu16Data = GetTempConfig(HOT_WATER_ID_DEGREE_90);
            
            break;

        case WIFI_FUNC_00CE_HOTTEMP_USE_SEL10 : // 95
            mu16Data = GetTempConfig(HOT_WATER_ID_DEGREE_95);
            
            break;

        case WIFI_FUNC_00CF_HOTTEMP_USE_SEL11 : // 100
            mu16Data = GetTempConfig(HOT_WATER_ID_HOT_USER);

            break;		
#endif         



        default:
            mu16Data = 0U;
            break;
    }

    return mu16Data;
}

#endif //ifdef MODEL_NO_1

