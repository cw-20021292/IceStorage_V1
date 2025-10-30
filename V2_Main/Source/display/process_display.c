#include "hw.h"
#include "display.h"
#include "process_display.h"

#include "front.h"
#include "cold_water.h"
#include "hot_water.h"
#include "water_out.h"
#include "water_out_common.h"
#include "tank_clean.h"
#include "ice.h"
#include "ice_full.h"
#include "power_saving.h"

#include "hal_key.h"
#include "hal_uv.h"
#include "temp.h"

#include "error.h"
#include "sound.h"
#include "eeprom.h"
#include "valve.h"
#include "key.h"
#include "key_normal_handler.h"
#include "service.h"
#include "ice_door.h"
#include "uv.h"
#include "drain_water.h"
#include "flush_water_out.h"
#include "hot_water_out.h"
#include "hot_water_out_preheat.h"
#include "filter.h"
#include "cody_kit.h"
#include "voice.h"
#include "empty_cold_tank.h"

#include "smart_check.h"
#include "diagnosis.h"

#include "display_lib.h"
#include "display_eol.h"
#include "display_error.h"
#include "display_clock.h"
#include "fct.h"
#include "WIFI_Control.h"




static void DisplayLockAll(void);
static void DisplayWaterOut(void);
static void DisplayIceOut(void);

static void DisplaySelWater(void);
static void DisplaySelHot(void);
static void DisplaySelAmount(void);
static void Display7Segment(void);
static void DisplayWifiStatus(void);

static void UpdateTimer(void);
static void ProcessDisplayBoot(void);
static void ProcessDisplaySettingMode(void);
static void ProcessDisplayFilterMode(void);
static void ProcessDisplaySoundMode(void);
static void ProcessDisplayCustomerMode(void);
static void ProcessDisplaySavingMode(void);
static void ProcessDisplayNormalMode(void);


typedef struct _display_
{
    U8 Init;            /* FALSE : Ï¥àÍ∏∞???? TRUE : Ï¥àÍ∏∞???ÑÎ£å */
    U8 Version;         /* Î≤ÑÏ†Ñ ?úÏãú FLAG */
    U8 VersionStep;

    //U8 ChangeDimming;
    //U16 ChangeDimmingTimeOut;

    /* Setting Menu */
    U8 Setting;             /* ?§Ï†ï ?îÎ©¥ ?úÏÑ±???ÅÌÉú */

    /* FILTER ALARAM */
    U8 Filter;
    U8 Sound;
    U8 Customer;
    U8 Clock;
    U8 Fota;

    U8 HotPreHeat;
    U8 IceOut;

   /* Memento */
    U8 Memento;         /* Î©îÎ©ò???ôÏûë FLAG */
    U8 MementoStep;     /* Î©îÎ©ò???úÏãú ?ôÏûë STEP */
    U8 MementoTimer;    /* Î©îÎ©ò???úÏãú ?ôÏûë ?úÍ∞Ñ ?Ä?¥Î®∏ */

} Display_T;

Display_T   Disp;


void InitDisplay(void)
{
    // TIMER 
    InitDispTimer();
    SetDispTimer( DISP_TIMER_CONFIG_TIME, TIMER_30SEC );
    StartDispTimerId( DISP_TIMER_SLEEP );

    /* Booting Display - Showing Blink Front Display */
    Disp.Init = FALSE;

    //Disp.ChangeDimming = FALSE;
    //Disp.ChangeDimmingTimeOut = 0; // 1sec..

    Disp.HotPreHeat = 0;

    Disp.Setting     = FALSE;

    Disp.Filter     = FALSE;
    Disp.Sound      = FALSE;
    Disp.Customer   = FALSE;
    Disp.Clock      = FALSE;
    Disp.Fota       = FALSE;

    /* TURN OFF ALL LED */
    TurnOffAllLED();
}


//void RefreshChagneDimming(void)
//{
//    Disp.ChangeDimming = TRUE;
//    Disp.ChangeDimmingTimeOut = 10; // 1sec..
//
//    SetDispTimer( DISP_TIMER_DIMMING, TIMER_1SEC);
//}
//

U8 GetDisplayInit(void)
{
    return Disp.Init;
}

void StartDisplayInit(void)
{
    Disp.Init = FALSE;

    InitDispTimer();
}

typedef struct _disp_id_time_
{
    U8 id;
    U16 mTime;
} DispTimerVal_T;

//static DispTimerVal_T DispTimerValTable[] = 
DispTimerVal_T DispTimerValTable[] = 
{
    { DISP_TIMER_CLOCK,               TIMER_20SEC },  
    { DISP_TIMER_ERROR,               TIMER_3SEC },

    { DISP_TIMER_CONFIG_AMOUNT,       TIMER_2SEC },     
    { DISP_TIMER_ALARM_SETTING,       TIMER_2SEC },
    { DISP_TIMER_ALARM_LOCK_ALL,      TIMER_2SEC },      
    { DISP_TIMER_ALARM_LOCK_HOT,      TIMER_2SEC },      
    { DISP_TIMER_ALARM_LOCK_ICE,      TIMER_2SEC },      
    { DISP_TIMER_ALARM_MAKE_OFF_ICE,  TIMER_2SEC },      
    { DISP_TIMER_ALARM_MAKE_OFF_COLD, TIMER_2SEC },      

    { DISP_TIMER_WIFI_AP_ERROR,       TIMER_5SEC },

    { DISP_TIMER_UP,                  TIMER_1SEC },
    { DISP_TIMER_DOWN,                TIMER_1SEC },
    { DISP_TIMER_NEXT,                TIMER_1SEC },

    { DISP_TIMER_CUSTOMER_MENU,       TIMER_6SEC },
    { DISP_TIMER_SOUND_MENU,          TIMER_6SEC },

    { DISP_TIMER_KEY_ICE,             TIMER_7SEC },  
    { DISP_TIMER_KEY_AMOUNT,          TIMER_7SEC },
    { DISP_TIMER_KEY_HOT,             TIMER_7SEC },
    { DISP_TIMER_KEY_HOT_TEMP,        TIMER_7SEC },

    { DISP_TIMER_SETTING_MENU,        TIMER_20SEC },
    { DISP_TIMER_CLOCK_MENU,          TIMER_20SEC },
    { DISP_TIMER_SLEEP,               TIMER_60MIN },

    { DISP_TIMER_ON,                  TIMER_2SEC },
    { DISP_TIMER_OFF,                 TIMER_2SEC },
    { DISP_TIMER_SETTING_CLOCK,       TIMER_2SEC }
};
#define SZ_DISP_TIMER_TABLE (sizeof(DispTimerValTable)/sizeof(DispTimerVal_T))

static U16 FindTimeVal(U8 id)
{
    U8 i;

    for( i = 0; i < SZ_DISP_TIMER_TABLE; i++ )
    {
        if( id == DispTimerValTable[ i ].id )
        {
            return DispTimerValTable[ i ].mTime;
        }
    }

    return 0;   // timer 0...
}

// Display Timer start/stop
void StartDispTimerId(U8 id)
{
    U16 mTime;

    mTime = FindTimeVal( id );
    SetDispTimer( id, mTime );
}

void StopDispTimerId(U8 id)
{
    SetDispTimer( id, 0);
}


/* Î≤ÑÏ†Ñ ?úÏãú */
void SetVersionDisp(U8 mu8DispSec)
{
    Disp.Version = TRUE;

    SetDispTimer( DISP_TIMER_VERSION, mu8DispSec);
}

/* ?§Ï†ï Î©îÎâ¥ ?†ÌÉù ?ÅÌÉú */
void SetSettingDisp(U8 mu8Conf)
{
    Disp.Setting = mu8Conf;
}

U8 GetSettingDisp(void)
{
    return Disp.Setting;
}


/* ?úÍ∞Ñ ?§Ï†ï Î©îÎâ¥ ?†ÌÉù ?ÅÌÉú */
void SetClockDisp(U8 mu8Conf)
{
    Disp.Clock = mu8Conf;
}

U8 GetClockDisp(void)
{
    return Disp.Clock;
}

/* ?ÑÌÑ∞ ÍµêÏ≤¥ ?ÅÌÉú */
void SetFilterDisp(U8 mu8Conf)
{
    Disp.Filter = mu8Conf;
}

U8 GetFilterDisp(void)
{
    return Disp.Filter;
}

/* ?åÎüâ ?§Ï? ?ÅÌÉú */
void SetSoundDisp(U8 mu8Conf)
{
    Disp.Sound = mu8Conf;
}

U8 GetSoundDisp(void)
{
    return Disp.Sound;
}


/* Í¥ÄÎ¶?Î™®Îìú ?§Ï†ï */
void SetCustomerDisp(U8 mu8Conf)
{
    Disp.Customer = mu8Conf;
}

U8 GetCustomerDisp(void)
{
    return Disp.Customer;
}

/* Î©îÎ©ò???§Ï†ï */
U8 GetMementoDisp(void)
{
    return Disp.Memento;
}


/* FOTA */
U8 GetFotaDisp(void)
{
    return Disp.Fota;
}

void SetFotaDisp(U8 mu8Conf )
{
    Disp.Fota = mu8Conf;
}

void SetMementoDisp(U8 mu8Conf )
{
    Disp.Memento        = mu8Conf;
    Disp.MementoStep    = 0;

    SetDispTimer( DISP_TIMER_MEMENTO, 0);
}


U8 IsExpiredConfigTime(void)
{
    if( IsExpiredDispTimer( DISP_TIMER_CONFIG_TIME ) == FALSE )
    {
        return FALSE;   // ÎßåÎ£å ?àÎê®.
    }

    return TRUE;  // ÎßåÎ£å??
}


static U8 IsWaitHotOut(void)
{
    if( IsSetHotOutMode( HOT_OUT_COOLING ) == TRUE 
            || IsSetHotOutMode( HOT_OUT_PRE_HEATING ) == TRUE 
            || IsSetHotOutMode( HOT_OUT_INIT_DRAIN ) == TRUE )
    {
        return TRUE;
    }

    return FALSE;   // NO wait..
}

// ?ºÏùå Ï∂îÏ∂ú ?îÎ©¥
static void DispIceOutEffect(void)
{
    if( Disp.IceOut == 0 )
    {
        DispSegmentChar( SEGMENT_CHAR_ID_ICE_1 );
    }
    else if( Disp.IceOut == 1 )
    {
        DispSegmentChar( SEGMENT_CHAR_ID_ICE_2 );
    }
    else if( Disp.IceOut == 2 )
    {
        DispSegmentChar( SEGMENT_CHAR_ID_ICE_3 );
    }
    else
    {
        DispSegmentChar( SEGMENT_CHAR_ID_ICE_4 );
    }

    if( IsExpiredDispBlinkTimer( DISP_TIMER_ID_500MS ) == TRUE )
    {
        Disp.IceOut++;
        if( Disp.IceOut > 3 )
        {
            Disp.IceOut = 0;
        }

    }
}

static void Disp7Seg_WaterOut_Debug(void)
{
    U16 mTemp;

    if( IsWaitHotOut() == TRUE )
    {
#if !CONFIG_TEST_PREHEAT
        mTemp =(U16)(GetTemp( TEMP_ID_HEATER ) * 10.0f);
        DispWaterOutAmount( mTemp );
#else
        // ?àÏó¥bar ?¨ÎùºÍ∞Ä?îÍ±∞ ?úÏãú
        U8 mu8Temp;
        U8 mu8HotType;

        mu8HotType  = GetHotSelect();
        mu8Temp     = (U8)GetHotSelectTemp( mu8HotType );
        DispHotWaterPreHeating( GetHotOutPreHeatLevel(), mu8Temp );   // ?®Ïàò ?ÑÎ¶¨?àÌåÖ ?úÏãú
#endif
    }
    else
    {
        mTemp =(U16)(GetTemp( TEMP_ID_HOT_OUT ) * 10.0f);
        DispWaterOutAmount( mTemp );
    }
}

static void Disp7Seg_WaterOut(void)
{
    U16 mu16Val;
    U8 mu8HotType;
    U8 mu8Amount;
    U8 mu8Temp;


    mu16Val     = GetWaterOutAmount();
    mu8Amount   = GetWaterOutAmoutType();
    mu8HotType  = GetHotSelect();
    mu8Temp     = (U8)GetHotSelectTemp( mu8HotType );


    // Ï∂îÏ∂úÏ§ëÏù∏ Î¨ºÏñë ?úÏãú
    // ?†ÌÉù??Ï∂îÏ∂ú??
    // ?®Ïàò?¥Î©¥ ?†ÌÉù???®Ïàò ?Ä??
    if( IsWaitHotOut() == TRUE )
    {
        DispHotWaterPreHeating( GetHotOutPreHeatLevel(), mu8Temp );   // ?®Ïàò ?ÑÎ¶¨?àÌåÖ ?úÏãú
    }
	/* [sean] Î≥ÄÍ≤??úÏñ¥Î°? ?∞ÏÜç Ï∂îÏ∂ú??ml ?úÏãú ?? 1990ml ?¥ÌõÑÎ°úÎäî ?∞ÏÜç?úÏñ¥ ?úÏãú */
	/*
    else if( mu8Amount == FIXED_AMOUNT_INFINITY )
    {
        DispWaterOutAmountInfinity();   // ?∞ÏÜç Ï∂îÏ∂ú ?úÏãú
    }
    else
    {
        DispWaterOutAmount( mu16Val );  // Ï∂îÏ∂ú ?†Îüâ ?úÏãú
    }
	*/
    else
    {
        DispWaterOutAmount( mu16Val );  // Ï∂îÏ∂ú ?†Îüâ ?úÏãú
    }
	
    DispAmountSelOff( mu8Amount );
    DispHotSelOff( GetLockHot() );

#if 0
    if( GetWaterOutSelect() != SEL_WATER_HOT )
    {
        DispHotSelOff( UNLOCK );
    }
    else
    {
        //DispHotSel( mu8HotType, UNLOCK, OFF );
        StartDispTimerId( DISP_TIMER_KEY_HOT );
    }
#endif
}


static void Disp7Seg_IceOut(void)
{
    DispIceOutEffect();
    // ?ºÏùå Ï∂îÏ∂úÍ≥??∞ÏÜç ?†ÌÉù Î≤ÑÌäº??Í≤πÏ≥ê?àÏùÑ ??
    // ?ºÏùå Ï∂îÏ∂ú??Ï¢ÖÎ£å?òÎ©¥ ?∞ÏÜç ?†ÌÉù ?®Í≥ºÎ•?Ï¥àÍ∏∞??
    ResetInfinityCount();
}

static void Disp7Seg_ErrorWifi(void)
{
    U8 mu8Error = 0;

    mu8Error = GetWifiStatusValue(WIFI_STATUS_ERROR);
    DispSegmentWifiError( mu8Error );
}

static void Disp7SegAmountLiter(U8 mu8OnOff)
{
    U8 mu8Amount;

    mu8Amount   = GetWaterOutAmoutType();
    if( mu8OnOff == OFF )
    {
        DispSegmentOff();
    }
    else
    {
        if( mu8Amount == FIXED_AMOUNT_HALF )
        {
            DispWaterOutAmount( 120 );
        }
        else if( mu8Amount == FIXED_AMOUNT_ONE )
        {
            DispWaterOutAmount( 250 );
        }
        else if( mu8Amount == FIXED_AMOUNT_TWO )
        {
            DispWaterOutAmount( 500 );
        }
        else if( mu8Amount == UNFIXED_AMOUNT_HALF )
        {
            DispWaterOutAmount( GetWaterAmountConfig(WATER_AMOUNT_HALF) );
        }
        else if( mu8Amount == UNFIXED_AMOUNT_ONE )
        {
            DispWaterOutAmount( GetWaterAmountConfig(WATER_AMOUNT_ONE) );
        }
        else if( mu8Amount == UNFIXED_AMOUNT_TWO )
        {
            DispWaterOutAmount( GetWaterAmountConfig(WATER_AMOUNT_TWO) );
        }
        else if( mu8Amount == FIXED_AMOUNT_FOUR )
        {
            DispWaterOutAmount( 1000 );
        }
		/*
        else if( mu8Amount == FIXED_AMOUNT_USER )
        {
            U16 mu16Liter;

            mu16Liter = GetWaterOutUserAmountLiter();
            DispWaterOutAmount( mu16Liter );
        }
		*/
        else if( mu8Amount == FIXED_AMOUNT_INFINITY )
        {
            DispWaterOutAmount( 9999 );
        }
    }
}

static void Disp7Seg_Normal(void)
{
    // ?ºÏùå Ï∂îÏ∂ú ?®Í≥º ?úÏûë??Î¶¨ÏÖã
    Disp.IceOut = 0;

    // ?®Ïàò ?àÏó¥ Ï∂îÏ∂ú ?®Í≥º ?úÏûë??Î¶¨ÏÖã
    Disp.HotPreHeat = 0;

    // ?∞ÏÜç Ï∂îÏ∂ú ?®Í≥º ?úÏûë??Î¶¨ÏÖã
    if( GetWaterOut() == FALSE )
    {
        ResetInfinityCount();   
    }


    if( IsExpiredDispTimer( DISP_TIMER_KEY_HOT_TEMP ) == FALSE )
    {
        // ?®Ïàò ?®ÎèÑ
        U8 mu8Temp;
        U8 mu8HotType;

        mu8HotType  = GetHotSelect();
        mu8Temp = (U8)GetHotSelectTemp( mu8HotType );
        DispTemp( mu8Temp );
    }
    else
    {
        // Í∏∞Î≥∏ ?©Îüâ ?§Ï†ï Î≥ÄÍ≤?( 2???êÎ©∏ )
        static U8 mu8OnOff = ON;

        if( IsExpiredDispTimer( DISP_TIMER_CONFIG_AMOUNT ) == TRUE )
        {
            mu8OnOff = ON;
            Disp7SegAmountLiter( ON );
        }
        else
        {
            mu8OnOff = BlinkLED( mu8OnOff, Disp7SegAmountLiter, DISP_TIMER_ID_500MS ); 
        }

    }

}

// Î¨?Ï∂îÏ∂úÏ§ëÏù∏ Í≤ΩÏö∞, Î¨ºÏñë ?úÏãú
static void Display7Segment(void)
{
    U8 mu8SelWater;


    mu8SelWater = GetWaterOutSelect();

#if CONFIG_TEST_HOT_OUT
    if( GET_STATUS_VALVE_HOT_IN() == 1 && 
            GET_STATUS_VALVE_HOT_OUT() == 0 )
    {
        U16 mTemp;
        mTemp =(U16)(GetTemp( TEMP_ID_HEATER ) * 10.0f);
        DispWaterOutAmount( mTemp );

        return;
    }
#endif

    // Î¨?Ï∂îÏ∂ú Ï§? Ï∂îÏ∂ú???úÏãú
    if( GetWaterOut() == TRUE )
    {
        if( GetHotConfigTest() == TRUE )
        {
            Disp7Seg_WaterOut_Debug();
        }
        else
        {
            Disp7Seg_WaterOut();
        }
    }
    else if( IsOpenIceDoor() == TRUE )
    {
        Disp7Seg_IceOut();
    }
    else if( IsExpiredDispTimer( DISP_TIMER_WIFI_AP_ERROR ) == FALSE )
    {
        Disp7Seg_ErrorWifi();
    }
    else
    {
        Disp7Seg_Normal();
    }
}

// WIFI_DISP_APMODE,       // on 0.1s - off 0.1s - on 0.1s - off 0.7s 
#define NUM_WIFI_BLINK_STEP     4
static const U8 mWifiBlinkTime[ NUM_WIFI_BLINK_STEP ] = { 1, 1, 1, 7 };
static U8 WifiApModeBlinkLED( U8 mu8Index, void (*pFun)(U8 mu8OnOff ) )
{
    static U8 mu8Count = 0;

    if( mu8Count != 0 )
    {
        mu8Count--;

        switch( mu8Index )
        {
            case 0:
                pFun( ON );
                break;
            case 1:
                pFun( OFF );
                break;
            case 2:
                pFun( ON );
                break;
            case 3:
                pFun( OFF );
                break;

            default:
                mu8Index = 0;
                pFun( OFF );
                break;
        }

    }

    if( mu8Count == 0 )
    {
        mu8Index++;
        if( mu8Index >= NUM_WIFI_BLINK_STEP )
        {
            mu8Index = 0;
        }
        mu8Count = mWifiBlinkTime[ mu8Index ];
    }


    return mu8Index;
}

U8 dbg_wifi_status = 0;
static void DisplayWifiStatus(void)
{
    static U8 mu8OnOff = OFF;
    static U8 mu8LedIndex = 0;
    E_WIFI_DISPLAY_T mWifiStatus = 0;

    mWifiStatus = GetWifiStatusValue(WIFI_STATUS_DISP);
    dbg_wifi_status = mWifiStatus;
    switch( mWifiStatus )
    {
        case WIFI_DISP_OFF:
            DispWifi( OFF );
            DispWifiBle( OFF );
            mu8OnOff = ON;
            mu8LedIndex = 0;
            break;

        case WIFI_DISP_AP1:
        case WIFI_DISP_AP2:
        case WIFI_DISP_AP3:
        case WIFI_DISP_AP4:
        case WIFI_DISP_AP5:
        case WIFI_DISP_TRY:
            mu8OnOff = BlinkLED( mu8OnOff, DispWifi, DISP_TIMER_ID_500MS ); 
            DispWifiBle( OFF );
            break;

        case WIFI_DISP_APMODE:
        case WIFI_DISP_AP_START:
        case WIFI_DISP_AP_STEP0:
        case WIFI_DISP_AP_STEP1:
        case WIFI_DISP_AP_STEP2:
        case WIFI_DISP_AP_STEP3:
            mu8LedIndex = WifiApModeBlinkLED( mu8LedIndex, DispWifi );
            DispWifiBle( OFF );
            break;

        case WIFI_DISP_CONNECT:
            DispWifi( ON );
            DispWifiBle( OFF );
            break;

        case WIFI_DISP_BLE_START:
        case WIFI_DISP_BLE_STEP0:
        case WIFI_DISP_BLE_STEP1:
        case WIFI_DISP_BLE_STEP2:
        case WIFI_DISP_BLE_STEP3:
            mu8LedIndex = WifiApModeBlinkLED( mu8LedIndex, DispWifi );
            DispWifiBle( OFF );
            break;

    }
}


// WIFI ?ëÏÜç Ï§ëÏù¥Î©??†Ïî® ?úÏãú
// WIFI ÎØ∏Ï†ë???ÅÌÉú?¥Î©¥ ?†Ïî® ?úÏãú OFF
static void DisplayWeather(void)
{
    static U8  mu8Icon = WEATHER_OFF;
    U16 mu16Weather;
    E_WIFI_DISPLAY_T mWifiStatus = 0;


    mWifiStatus = GetWifiStatusValue(WIFI_STATUS_DISP);
    if( mWifiStatus != WIFI_DISP_CONNECT )
    {
        mu8Icon = WEATHER_OFF;
    }
    else
    {
        mu16Weather = GetWifiRequestValue(WIFI_RQST_WEATHER);
        switch( mu16Weather )
        {
            case 1:     // ÎßëÏùå
            case 2:     // Íµ¨Î¶ÑÏ°∞Í∏à
                mu8Icon = WEATHER_SUN;
                break;

            case 3:     // Íµ¨Î¶Ñ ÎßéÏùå
            case 4:     // ?êÎ¶º
            case 11:    // Ï≤úÎë•Î≤àÍ∞ú
            case 12:    // ?àÍ∞ú
                mu8Icon = WEATHER_CLOUD;
                break;

            case 5:     // ?êÎ¶¨Í≥?Îπ?
            case 6:     // ?êÎ¶¨Í≥???
            case 7:     // ÎπÑÏò®??Í∞?
            case 8:     // ?åÎÇòÍ∏?
            case 9:     // Îπ??êÎäî ??
            case 10:    // ???êÎäî Îπ?
                mu8Icon = WEATHER_RAIN;
                break;

            default:
                // ?¥Ï†Ñ Í∞??†Ï?...
                break;
        }
    }

    DispWeather( mu8Icon );
}

static void DisplayLockAll(void)
{
    U8 mu8Lock;

    if( GetLockAll() == LOCK )
    {
        DispAllLockSel( ON );
    }
    else
    {
        DispAllLockSel( OFF );
    }
}


// Ï∂îÏ∂ú LEDÍ∞Ä ?êÎì±?òÎäî Ï°∞Í±¥.
// ?åÎü¨?±Ï? OFF
// ?®Ïàò Ï¥àÍ∏∞ ?åÎü¨?±ÎèÑ OFF
static U8 IsTurnOnWaterOut(void)
{
    if( GetWaterOut() == FALSE )
    {
        return FALSE;
    }

    if( IsFlushOut() == TRUE )
    {
        return FALSE;
    }

    if( IsHotOut() == TRUE 
            && IsSetHotOutMode( HOT_OUT_INIT_DRAIN ) == TRUE 
      )
    {
        return FALSE;
    }

    return TRUE;
}

static void DisplayWaterOut(void)
{
    if( IsTurnOnWaterOut() == TRUE )
    {
        DispWaterOut( ON );
    }
    else
    {
        DispWaterOut( OFF );
    }
}

static void DisplayIceOut(void)
{
    if( IsOpenIceDoor() == FALSE )
    {
        // ?ºÏùå ?∞ÏÜç Ï∂îÏ∂ú???êÎ©∏...
        DispIceOut( OFF );
    }
    else
    {
        DispIceOut( ON );
    }
}


#if 0
#define REGION_COLD_GOOD     0
#define REGION_COLD_BAD      1
static void DisplayColdTemp(void)
{
    static U8 mu8Region = REGION_COLD_GOOD;
    TEMP_T  tCurrentTemp = 0.0f;

    // ?âÏàò OFF?? ?®ÎèÑ??OFF
    if( GetColdWaterConfigMake() == FALSE )
    {
        mu8Region = REGION_COLD_GOOD;
        DispColdTemp( OFF, OFF );
        return ;
    }
    
    // ?®ÎèÑ???∞Î•∏ Region Í≥ÑÏÇ∞ 
    tCurrentTemp = GetTemp( TEMP_ID_COLD_WATER );
    if( mu8Region == REGION_COLD_GOOD )
    {
        if( tCurrentTemp >= GetColdTargetOnTemp() )
        {
            mu8Region = REGION_COLD_BAD;
        }
    }
    else
    {
        if( tCurrentTemp <= GetColdTargetOffTemp() )
        {
            mu8Region = REGION_COLD_GOOD;
        }
    }

    // Region???∞Î•∏ LED ?úÏñ¥
    if( mu8Region == REGION_COLD_BAD )
    {
        DispColdTemp( ON, FALSE );
    }
    else
    {
        DispColdTemp( ON, TRUE );
    }
}
#else
#define REGION_COLD_GOOD     0
#define REGION_COLD_BAD      1
static void DisplayColdTemp(void)
{
    // ?âÏàò OFF?? ?®ÎèÑ??OFF
    if( GetColdWaterConfigMake() == FALSE )
    {
        DispColdTemp( OFF, OFF );
        return ;
    }
    
    if( GetColdWaterTempStatus() == COLD_STATUS_GOOD )
    {
        DispColdTemp( ON, TRUE );
    }
    else
    {
        DispColdTemp( ON, FALSE );
    }
}
#endif

static void DisplaySelIce(void)
{
    U8 mu8Turbo;
    U8 mu8Size;
    U8 mu8Lock;


    mu8Turbo = GetIceTurbo();
    if( GetSystem() == SYSTEM_CP )
    {
        mu8Turbo = FALSE;   // CPÎ™®Îç∏?êÏÑú??Îπ†Î•∏ ?úÎπô Î≤ÑÌäº?ºÎ°ú ?úÏãú
    }
    mu8Lock = GetLockIce();
    mu8Size = GetIceSize();
    if( GetIceConfigMake() == FALSE )
    {
        DispIceSelOff(OFF, OFF, mu8Lock, OFF, mu8Size);
    }
    else if( IsExpiredDispTimer( DISP_TIMER_SLEEP ) == TRUE )
    {
        DispIceSelOff(ON, OFF, mu8Lock, OFF, mu8Size);
    }
    else if( IsExpiredDispTimer( DISP_TIMER_KEY_ICE ) == FALSE )
    {
        if( GetSystem() == SYSTEM_CP )
        {
            DispIceSizeSel( mu8Size, mu8Lock, ON );
        }
        else
        {
            DispIceSel( mu8Turbo, mu8Size, mu8Lock, ON );
        }
    }
    else
    {
        DispIceSelOff(ON, mu8Turbo, mu8Lock, ON, mu8Size);
    }

}

static void DisplaySelIceTurbo(void)
{
    if( GetSystem() == SYSTEM_CP )
    {
        if( GetIceConfigMake() == TRUE )
        {
            DispIceTurbo( GetIceTurbo() );
        }
        else
        {
            DispIceTurboOff();
        }
    }
}

static void DisplaySelWater(void)
{
    U8 mu8Sel;

    //if( GetLockAll() == LOCK )
    //{
    //    DispWaterSel( 0xFF );   // ?ÑÏ≤¥ ÎπÑÌôú?±Ìôî
    //}
    //else
    {
        mu8Sel = GetWaterOutSelect();
        DispWaterSel( mu8Sel, GetColdWaterConfigMake() );
    }
}

static void DisplaySelHot(void)
{
#if 0
    U8 mu8Sel;
    U8 mu8Lock;


    mu8Sel  = GetHotSelect();
    mu8Lock = GetLockHot();
    if( IsExpiredDispTimer( DISP_TIMER_KEY_HOT ) == FALSE )
    {
        DispHotSel( mu8Sel, mu8Lock, ON );
    }
    else
    {
        DispHotSelOff( mu8Lock );
neidf
#else
        DispHotSelOff( GetLockHot() );
#endif
}


#if 0
static void DisplaySelAmount(void)
{
    U8 mu8Sel;


    mu8Sel = GetWaterOutAmoutType();
    if( IsExpiredDispTimer( DISP_TIMER_SLEEP ) == TRUE )
    {
        DispAmount(OFF);
    }
    else if( IsExpiredDispTimer( DISP_TIMER_KEY_AMOUNT ) == FALSE )
    {
        // Î¨ºÏñë ?†ÌÉù ?ÅÌÉú
        DispAmountSel( mu8Sel, ON );
    }
    else
    {
        // ?ÄÍ∏??ÅÌÉú
        DispAmountSelOff( mu8Sel );
    }
}
#endif
static void DisplaySelAmount(void)
{
    U8 mu8Sel;


    mu8Sel = GetWaterOutAmoutType();
    if( IsExpiredDispTimer( DISP_TIMER_SLEEP ) == TRUE )
    {
        DispAmount(ON);
    }
    else if( IsExpiredDispTimer( DISP_TIMER_KEY_AMOUNT ) == FALSE )
    {
        // Î¨ºÏñë ?†ÌÉù ?ÅÌÉú
        DispAmountSel( mu8Sel, ON );
    }
    else
    {
        // ?ÄÍ∏??ÅÌÉú
        DispAmountSelOff( mu8Sel );
    }
}

static void DisplayClock(void)
{
    TimeData_T  mTime;

    GetRtcTime( &mTime );
    DispClock( &mTime );
}



static void DisplayAlarmSetting(void)
{
    // ALARM SETTING BUTTON
    static U8 mu8OnOff[ 7 ] = { ON, };

    if( IsExpiredDispTimer( DISP_TIMER_ALARM_SETTING ) == TRUE )
    {
        mu8OnOff[0] = ON;
    }
    else
    {
        mu8OnOff[0] = BlinkLED( mu8OnOff[0], DispSettingButton, DISP_TIMER_ID_500MS ); 
    }

    if( IsExpiredDispTimer( DISP_TIMER_ALARM_LOCK_HOT ) == TRUE )
    {
        mu8OnOff[1] = ON;
    }
    else
    {
        mu8OnOff[1]  = BlinkLED( mu8OnOff[1], DispAlarmLockHot, DISP_TIMER_ID_500MS ); 
    }

    if( IsExpiredDispTimer( DISP_TIMER_ALARM_LOCK_ICE ) == TRUE )
    {
        mu8OnOff[2] = ON;
    }
    else
    {
        mu8OnOff[2] = BlinkLED( mu8OnOff[2], DispAlarmLockIce, DISP_TIMER_ID_500MS ); 
    }

    if( IsExpiredDispTimer( DISP_TIMER_ALARM_MAKE_OFF_ICE ) == TRUE )
    {
        mu8OnOff[3] = ON;
    }
    else
    {
        if( GetSystem() == SYSTEM_CHP )
        {
            mu8OnOff[3] = BlinkLED( mu8OnOff[3], DispAlarmMakeIce, DISP_TIMER_ID_500MS ); 
        }
        else
        {
            mu8OnOff[3] = BlinkLED( mu8OnOff[3], DispAlarmMakeIceCp, DISP_TIMER_ID_500MS ); 
        }
    }

    if( IsExpiredDispTimer( DISP_TIMER_ALARM_MAKE_OFF_COLD ) == TRUE )
    {
        mu8OnOff[4] = ON;
    }
    else
    {
        mu8OnOff[4] = BlinkLED( mu8OnOff[4], DispAlarmMakeCold, DISP_TIMER_ID_500MS ); 
    }

    if( IsExpiredDispTimer( DISP_TIMER_ALARM_LOCK_ALL ) == TRUE )
    {
        mu8OnOff[5] = ON;
    }
    else
    {
        mu8OnOff[5]  = BlinkLED( mu8OnOff[5], DispAlarmLockAll, DISP_TIMER_ID_500MS ); 
    }

    if( IsExpiredDispTimer( DISP_TIMER_SETTING_CLOCK ) == TRUE )
    {
        mu8OnOff[6] = ON;
    }
    else
    {
        mu8OnOff[6]  = BlinkLED( mu8OnOff[6], DispSettingClockDone, DISP_TIMER_ID_500MS ); 
    }
}


#if CONFIG_UV_MANUAL
extern U8 dbg_uv_ice_door;
extern U8 dbg_uv_water_out;
extern U8 dbg_uv_ice_tray;
extern U8 dbg_uv_ice_tank;
#include "hal_led.h"
static void Test_DisplayUv(void)
{
    HAL_TurnOnOffLED( ICON_UV_ICE,      dbg_uv_ice_door );
    HAL_TurnOnOffLED( ICON_UV_WATER,    dbg_uv_water_out );
    HAL_TurnOnOffLED( ICON_UV_ICE_TRAY, dbg_uv_ice_tray );
    HAL_TurnOnOffLED( ICON_UV_ICE_TANK, dbg_uv_ice_tank );
}
#endif
// UV ?úÏãú ?úÏÑú ?†Í≤Ω?®Ïïº??
// SHOT Î≥¥Îã§ ?ºÎ∞ò UV Ï°∞ÏÇ¨ ?úÍ∞Ñ??Í∏∏Í∏∞ ?åÎ¨∏??
// ?∞ÏÑ† ?ºÎ∞ò ?úÏñ¥Í∞Ä SHOTÎ≥¥Îã§ ?∞ÏÑ† ?úÏúÑ ?íÏùå
// Î¨?Ï∂îÏ∂ú - ?ºÏùå Ï∂îÏ∂ú - ?ºÏùå ?±ÌÅ¨ - ?ºÏùå ?∏Î†à???úÏÑú??
static void DisplayUv(void)
{
    U8 mu8Water = OFF;
    U8 mu8Ice = OFF;
    U8 mu8IceTray = OFF;
    U8 mu8IceTank = OFF;

#if 0
    // WATER 
    if( IsTurnOnUvId( UV_WATER_OUT ) == TRUE 
            || IsTurnOnUvId( UV_WATER_OUT_SHOT ) == TRUE )
    {
        mu8Water = ON;
    }

    // ICE
    if( IsTurnOnUvId( UV_ICE_DOOR ) == TRUE 
            || IsTurnOnUvId( UV_ICE_DOOR_SHOT ) == TRUE )
    {
        mu8Ice = ON;
    }

    // ICE TANK
    if( IsTurnOnUvId( UV_ICE_TANK ) == TRUE )
    {
        mu8IceTank = ON;
    }

    // ICE TRAY
    if( (IsTurnOnUvId( UV_ICE_TRAY ) == TRUE 
                && GetIceFullStatus() == TRUE )
            || IsTurnOnUvId( UV_ICE_TRAY_SHOT ) == TRUE )
    {
        mu8IceTray = ON;
    }
#else
    mu8Water    = HAL_GetUvOnOffStatus( HAL_UV_WATER_OUT );
    mu8Ice      = HAL_GetUvOnOffStatus( HAL_UV_ICE_DOOR );
    mu8IceTray  = HAL_GetUvOnOffStatus( HAL_UV_ICE_TRAY );
    mu8IceTank  = HAL_GetUvOnOffStatus( HAL_UV_ICE_TANK );
#endif

    if( mu8Water == ON || mu8Ice == ON || mu8IceTray == ON || mu8IceTank == ON) 
    {
        DispUvCare( ON, mu8Water, mu8Ice, mu8IceTray, mu8IceTank ); 
    }
    else
    {
        DispUvCare( OFF, mu8Water, mu8Ice, mu8IceTray, mu8IceTank ); 
    }
}

static void DisplayCodyKit(void)
{
    static U8 mu8OnOff = ON;

    if( GetCodyKitStep() == CODY_STEP_ROOM )
    {
        mu8OnOff = BlinkLED( mu8OnOff, DispRoom, DISP_TIMER_ID_500MS );
        DispCold( OFF );
    }
    else
    {
        mu8OnOff = BlinkLED( mu8OnOff, DispCold, DISP_TIMER_ID_500MS );
        DispRoom( OFF );
    }
}


enum
{
    DISP_VERSION_START,
    DISP_VERSION_MODEL,
    DISP_VERSION_VERSION,
    DISP_VERSION_END,
};


static void ProcessDisplayVersionMode(void)
{

    if( IsExpiredDispTimer( DISP_TIMER_VERSION ) == FALSE )
    {
        return ;
    }

    // 1. ALL OFF LED - 1 SEC
    // 2. DISPLAY VERSION - 1 SEC
    // 3. DISPLAY SYSTEM TYPE(CHP or CP) - 1 SEC
    // 1. ALL OFF LED
    switch( Disp.VersionStep )
    {
        case DISP_VERSION_START: 
            /* 1. TURN OFF ALL LED */
            TurnOffAllLED();	

            SetDispTimer( DISP_TIMER_VERSION, TIMER_1SEC );
            Disp.VersionStep++;
            break;

        case DISP_VERSION_MODEL:
            if( GetSystem() == SYSTEM_CHP )
            {
                DispSegmentChar( SEGMENT_CHAR_ID_CHP  );
            }
            else
            {
                DispSegmentChar( SEGMENT_CHAR_ID_CP  );
            }
            SetDispTimer( DISP_TIMER_VERSION, TIMER_1SEC );
            Disp.VersionStep++;
            break;

        case DISP_VERSION_VERSION:
            DispSegmentDigit( VERSION_MAJOR );

            SetDispTimer( DISP_TIMER_VERSION, TIMER_1SEC );
            Disp.VersionStep++;
            break;


        case DISP_VERSION_END:
            TurnOffAllLED();	
            Disp.Version     = FALSE;
            Disp.VersionStep = 0;
            break;

        default:
            Disp.Version     = FALSE;
            Disp.VersionStep = 0;
            break;

    }
}

enum
{
    DISP_JIG_VERSION_START,
    DISP_JIG_VERSION_MODEL,
    DISP_JIG_VERSION_END,
};

static void ProcessDisplayVersionModeFrontJIG(void)
{

    if( IsExpiredDispTimer( DISP_TIMER_VERSION ) == FALSE )
    {
        return ;
    }

    // 1. ALL OFF LED - 1 SEC
    // 2. DISPLAY SYSTEM TYPE(CHP or CP) - 1 SEC
    // 3. ALL OFF LED
    switch( Disp.VersionStep )
    {
        case DISP_JIG_VERSION_START: 
            TurnOffAllLED();	

            SetDispTimer( DISP_TIMER_VERSION, 0 );
            Disp.VersionStep = DISP_VERSION_MODEL;
            break;

        case DISP_JIG_VERSION_MODEL:
            if( GetSystem() == SYSTEM_CHP )
            {
                DispSegmentChar( SEGMENT_CHAR_ID_CHP  );
            }
            else
            {
                DispSegmentChar( SEGMENT_CHAR_ID_CP  );
            }
            SetDispTimer( DISP_TIMER_VERSION, TIMER_1SEC );
            Disp.VersionStep++;
            break;

        case DISP_JIG_VERSION_END:
            TurnOffAllLED();	
            Disp.Version     = FALSE;
            Disp.VersionStep = 0;
            break;

        default:
            Disp.Version     = FALSE;
            Disp.VersionStep = 0;
            break;

    }
}


enum
{
    MMT_LED_ON_ALL,
    MMT_LED_OFF_ALL,
    MMT_ERR_1,
    MMT_ERR_1_OFF,
    MMT_ERR_2,
    MMT_ERR_2_OFF,
    MMT_ERR_3,
    MMT_ERR_3_OFF,
    MMT_ERR_4,
    MMT_ERR_4_OFF,
    MMT_ERR_5,
    MMT_ERR_5_OFF,
    MMT_ERR_DONE
};

static void Mmt_TurnOffLedAll(void)
{
    TurnOffAllLED();	

    SetDispTimer( DISP_TIMER_MEMENTO, TIMER_1SEC );
    Disp.MementoStep++;
}

static void Mmt_TurnOnLedAll(void)
{
    TurnOnAllLED();	

    SetDispTimer( DISP_TIMER_MEMENTO, TIMER_1SEC );
    Disp.MementoStep++;
}

static void Mmt_ErroCode(U8 id, SoundId_T mSoundId )
{
    Memento_T mMemento;
    ErrorId_T  mError;


    GetMementoError( id, &mMemento );
    mError = mMemento.Error;
    if( mError == ERR_NONE )
    {
        Disp.MementoStep = 0;

    }
    else
    {
        DisplayError( mError, ON );
        //Sound( mSoundId );

        SetDispTimer( DISP_TIMER_MEMENTO, TIMER_5SEC );
        Disp.MementoStep++;
    }
}

static void Mmt_Done(void)
{
    Disp.MementoStep = 0;
}


U8 gu8MemtSoundCount = 0;
U8 gu8MementoDelay = 0;
static void SoundMemento(void)
{
    if( gu8MementoDelay != 0 )
    {
        gu8MementoDelay--;
        return ;
    }

    if( gu8MemtSoundCount != 0 )
    {
        gu8MementoDelay = 5;   // 1sec..

        gu8MemtSoundCount--;
        Sound( SOUND_SELECT );
    }

}

static void DisplayMemento(void)
{
    SoundMemento();

    if( IsExpiredDispTimer( DISP_TIMER_MEMENTO ) == FALSE )
    {
        return ;
    }

    // 1. ALL ON LED - 1 SEC
    // 2. ALL OFF LED - 1 SEC
    // 3. DISPLAY ERROR 5SEC & SOUND 1~5
    // 3. DISPLAY ERROR 5SEC & SOUND 1
    switch( Disp.MementoStep )
    {
        case MMT_LED_ON_ALL : 
            Mmt_TurnOnLedAll();
            break;

        case MMT_LED_OFF_ALL:
            Mmt_TurnOffLedAll();
            break;

        case MMT_ERR_1:
            Mmt_ErroCode( 4, SOUND_SELECT );
            gu8MemtSoundCount = 1;
            break;

        case MMT_ERR_1_OFF:
            Mmt_TurnOffLedAll();
            break;

        case MMT_ERR_2:
            Mmt_ErroCode( 3, SOUND_SELECT );
            gu8MemtSoundCount = 2;
            break;

        case MMT_ERR_2_OFF:
            Mmt_TurnOffLedAll();
            break;

        case MMT_ERR_3:
            Mmt_ErroCode( 2, SOUND_SELECT );
            gu8MemtSoundCount = 3;
            break;

        case MMT_ERR_3_OFF:
            Mmt_TurnOffLedAll();
            break;

        case MMT_ERR_4:
            Mmt_ErroCode( 1, SOUND_SELECT );
            gu8MemtSoundCount = 4;
            break;

        case MMT_ERR_4_OFF:
            Mmt_TurnOffLedAll();
            break;

        case MMT_ERR_5:
            Mmt_ErroCode( 0, SOUND_SELECT );
            gu8MemtSoundCount = 5;
            break;

        case MMT_ERR_5_OFF:
            Mmt_TurnOffLedAll();
            break;

        case MMT_ERR_DONE:
            Mmt_Done();
            break;

    }

    return ;
}


static void ProcessDisplayMementoMode(void)
{
    DispLedDuty();

    DisplayMemento();

    DispMementoKey( 0 );
}

/* ?îÏä§?åÎ†à??Ï¥àÍ∏∞???úÏãú */
static void ProcessDisplayBoot(void)
{
    static U8 mu8Count = 7;
    static U8 mu8Toggle = 0;

    if( IsExpiredDispBlinkTimer( DISP_TIMER_ID_500MS ) == TRUE )
    {
        mu8Toggle = !mu8Toggle;

        mu8Count--;
        if( mu8Count == 0 )
        {
            TurnOffAllLED();

            Disp.Init = TRUE;
            mu8Count = 7;
            return;
        }
    }
	if( mu8Count < 3)
	{
        DispSegmentDigit( VERSION_MAJOR );
	}
	else
	{
	    if( mu8Toggle == 0 )
	    {
	        TurnOffAllLED();
	    }
	    else
	    {
	        TurnOnAllLED();
	    }
	}
}


static void ProcessDisplayLockAll(void)
{
    TurnOffAllLED();

    DispAllLock( ON );
    DisplayAlarmSetting();

    return ;
}

static void ProcessDisplaySettingMode(void)
{
    DispLedDuty();

    TurnOffAllLED();
    DispSetting( ON );


    // ?§Ï†ï Î≥ÄÍ≤ΩÏãú 7segment??ON/OFF ?úÏãú
    if( IsExpiredDispTimer( DISP_TIMER_ON ) == FALSE )
    {
        DispSegmentChar( SEGMENT_CHAR_ID_ON );
    }
    else if( IsExpiredDispTimer( DISP_TIMER_OFF ) == FALSE )
    {
        DispSegmentChar( SEGMENT_CHAR_ID_OFF );
    }

    // ?ºÏ†ï ?úÍ∞Ñ Î≤ÑÌäº ?ÖÎ†•???ÜÏúºÎ©??§Ï†ï Î©îÎâ¥?êÏÑú Îπ†Ï†∏?òÏò¥.
    if( IsExpiredDispTimer( DISP_TIMER_SETTING_MENU ) == TRUE )
    {
        SetSettingDisp( FALSE );
    }
}

static void ProcessDisplaySettingClock(void)
{
    MenuTime_T mTime;

    DispLedDuty();

    TurnOffAllLED();

    GetClockTime( &mTime );
    DispSettingClock( 
            GetClockItem(),  // item
            mTime.u8AmPm,   // am/pm
            mTime.u8Hour,   // hour
            mTime.u8Min     // min
            );

    DispSettingClockKey();

    ContySettingClockKey();

    // ?ºÏ†ï ?úÍ∞Ñ Î≤ÑÌäº ?ÖÎ†•???ÜÏúºÎ©??§Ï†ï Î©îÎâ¥?êÏÑú Îπ†Ï†∏?òÏò¥.
    if( IsExpiredDispTimer( DISP_TIMER_CLOCK_MENU ) == TRUE )
    {
        SetClockDisp( FALSE );
    }
}

// ?ÑÌÑ∞ Ïª§Î≤Ñ OPEN???îÎ©¥ ?úÏãú.
// ?ÑÌÑ∞ ?àÎ≤Ñ ÎØ∏Í∞êÏßÄ???êÎ©∏
static void ProcessDisplayFilterMode(void)
{
    DispLedDuty();

    TurnOffAllLED();


    if( GetFilterStatus( ID_FILTER ) == FILTER_OPEN )
    {
        static U8 mu8OnOff;

        mu8OnOff = BlinkLED( mu8OnOff, DispFilterStatus, DISP_TIMER_ID_500MS ); 
    }
    else
    {
        DispFilterStatus( ON );
    }
}

static void ProcessDisplayEmptyCold(void)
{
    DispLedDuty();

    TurnOffAllLED();

    DispWaterOutAmountInfinity();   // ?∞ÏÜç Ï∂îÏ∂ú ?úÏãú

    if( GetEmptyColdTankType() == TYPE_FAUCET )
    {
        DispWaterSelEmptyColdFaucet();
    }
    else
    {
        DispWaterSelEmptyColdDrain();
    }

    DispIceOut( ON );
    DispWaterOut( ON );
}

static void ProcessDisplaySoundMode(void)
{
    U8 mu8Vol;
    U8 mu8Mode;

    DispLedDuty();

    TurnOffAllLED();

    mu8Vol  = GetVoiceVolume();
    mu8Mode = GetVoiceLevel();
    if( mu8Mode == VOICE_LEVEL_OFF )
    {
        mu8Vol = 0xFF;  // OFF
    }

    DispSettingSoundVolume( mu8Vol );

    DispSettingSoundMode( mu8Mode );

    DispSettingSoundKey();

    // Î©îÎâ¥?êÏÑú 6Ï¥??¥ÏÉÅ Î≤ÑÌäº ?ÖÎ†•???ÜÎäî Í≤ΩÏö∞,
    // ?§Ï†ï Î©îÎâ¥?êÏÑú Îπ†Ï†∏?òÏò¥.
    if( IsExpiredDispTimer( DISP_TIMER_SOUND_MENU ) == TRUE )
    {
        SetSoundDisp( FALSE );
    }
}

static void ProcessDisplayCustomerMode(void)
{
    U8 mu8Type;


    DispLedDuty();
    TurnOffAllLED();


    mu8Type = GetCustomerType();
    DispSettingCustomerMode( mu8Type );
    DispSettingKey();

    // Î©îÎâ¥?êÏÑú 6Ï¥??¥ÏÉÅ Î≤ÑÌäº ?ÖÎ†•???ÜÎäî Í≤ΩÏö∞,
    // ?§Ï†ï Î©îÎâ¥?êÏÑú Îπ†Ï†∏?òÏò¥.
    if( IsExpiredDispTimer( DISP_TIMER_CUSTOMER_MENU ) == TRUE )
    {
        SetCustomerDisp( FALSE );
    }
}

static void ProcessDisplayHealth(void)
{
    DispLedDuty();

    TurnOffAllLED();

    // ?§Ï†ï Î©îÎâ¥ OFF
    DispSetting( OFF );

    // Ï∂îÏ∂ú Î≤ÑÌäº
    DisplayWaterOut();
    DisplayIceOut();

    // ?ºÏùå ?†ÌÉù
    DisplaySelIce();

    // ?ºÏùå Îπ†Î•∏ ?úÎπô - Only CP
    DisplaySelIceTurbo();

    // Î¨ºÏÑ†??
    DisplaySelWater();

    // ?®Ïàò ?†ÌÉù ???úÏãú ?¥Ïö©
    DisplaySelHot();

    // Î¨ºÏñë ?†ÌÉù ???úÏãú ?¥Ïö©
    DisplaySelAmount();

    // ?úÎ†à???åÌÑ∞ ?ôÏûë ?úÏãú
    // 10, 20, 30... 100% ?úÏãú.. 10?®ÏúÑÎ°??úÏãú..
    if( IsStartDrainWater() == TRUE )
    {
        U8 mu8Percent;

        // Ï≤´Ïß∏?êÎ¶¨ Í∞íÏ? 0?ºÎ°ú Ï≤òÎ¶¨
        mu8Percent = GetDrainTimePercent() / 10;
        mu8Percent *= 10;
        DispDrainWater( ON, mu8Percent );
    }
}

// Smart Check - Diagnosis
static void ProcessDisplaySmartCheck(void)
{
    DispLedDuty();

    TurnOffAllLED();

    DispPercent( GetDiagnosisProgress() );
}

// CODY KIT - ?¥Í∑† ?úÎπÑ???§Ìä∏
static void ProcessDisplayCodyKit(void)
{
    DispLedDuty();

    TurnOffAllLED();

    DispSegmentChar( SEGMENT_CHAR_ID_STE );

    DisplayWaterOut();
    DisplayIceOut();

    DisplayCodyKit();
}

static void DisplayFlushPercent(void)
{
    if( GetWaterOut() == TRUE )
    {
        U8 mu8Percent;
        U8 mu8Val;

        mu8Percent = GetFlushOutPercent();  
        mu8Val = (mu8Percent / 10U) * 10U;  // Ï≤´Ïß∏?êÎ¶¨ Í∞íÏ? 0?ºÎ°ú Ï≤òÎ¶¨

        DispPercent( mu8Val );
    }
}

// wifi on
// flush led on
// Î≤ÑÌäºÎß??êÎ©∏
static void DisplayFlushSetupInit(void)
{
    static U8 mu8OnOff = ON;
    static U8 mu8SoundGuide = FALSE;
    static U8 mu8Delay = 30;    // 3sec

    // WIFI 
    DisplayWifiStatus();

    // Ï∂îÏ∂ú Î≤ÑÌäº
    if( GetWaterOut() == TRUE )
    {
        DispFlushing( ON );
        DisplayWaterOut();
    }
    else
    {
        mu8OnOff = BlinkLED( mu8OnOff, DispWaterOutCircle, DISP_TIMER_ID_500MS ); 

        if( mu8Delay != 0 )
        {
            mu8Delay--;
        }
        else
        {
            // ÏµúÏ¥à ?§Ïπò ?àÎÇ¥ ?åÏÑ±
            if( mu8SoundGuide == FALSE )
            {
                mu8SoundGuide = TRUE;
                Sound( SOUND_SETUP_FLUSHING_GUIDE );
            }
        }

    }

    DisplayFlushPercent();
}

// wifi off
// flush led on
// Î≤ÑÌäºÎß??êÎ©∏
static void DisplayFlushFilter(void)
{
    static U8 mu8OnOff = ON;

    DisplayWifiStatus();

    // Ï∂îÏ∂ú Î≤ÑÌäº
    if( GetWaterOut() == TRUE )
    {
        DispFlushing( ON );
        DisplayWaterOut();
    }
    else
    {
        mu8OnOff = BlinkLED( mu8OnOff, DispWaterOutCircle, DISP_TIMER_ID_500MS ); 

        DispColdDimming( ON );
    }

    DisplayFlushPercent();
}

// wifi off
// flush led off
// ÏßÑÌñâ???úÏãúÎß?
static void DisplayFlushPowerOn(void)
{
#if 0
    DispFlushing( OFF );

    // Ï∂îÏ∂ú Î≤ÑÌäº
    DisplayWaterOut();

    DisplayFlushPercent();
#else
    static U8 mu8OnOff = ON;


    DisplayWifiStatus();

    // Ï∂îÏ∂ú Î≤ÑÌäº
    if( GetWaterOut() == TRUE )
    {
        DisplayWaterOut();
    }
    else
    {
        mu8OnOff = BlinkLED( mu8OnOff, DispWaterOutCircle, DISP_TIMER_ID_500MS ); 
    }

    DisplayFlushPercent();
#endif
}


static void ProcessDisplayFlushingMode(void)
{
    DispLedDuty();

    TurnOffAllLED();
    StartDispTimerId( DISP_TIMER_SLEEP );

    if( GetFlushSetupInit() == FALSE )
    {
        DisplayFlushSetupInit();
    }
    else if( GetFlushStatus() == FALSE )
    {
        DisplayFlushFilter();
    }
    else if( GetFlushPowerOn() == FALSE )
    {
        DisplayFlushPowerOn();
    }
}


static void ProcessDisplaySavingMode(void)
{
    DispLedDuty();

    TurnOffAllLED();


    DispSaving( GetSavingConfig() );

    // Ï∂îÏ∂ú Î≤ÑÌäº
    DisplayWaterOut();
    DisplayIceOut();
}

static void DispHotOutTest(void)
{
    // ?®Ïàò ?åÏä§??Í∏∞Îä• ?úÏÑ±?îÏ§ë?¥Î©¥ ?®ÎèÑ ?†ÌÉù?ÅÌÉú ?∞ÏÜç ?úÏãú
    if( GetHotConfigTest() == TRUE 
            && GetWaterOutSelect() == SEL_WATER_HOT )
    {
        StartDispTimerId( DISP_TIMER_KEY_HOT_TEMP );
        StartDispTimerId( DISP_TIMER_KEY_HOT );
    }
}

static void ProcessDisplayNormalMode(void)
{
    DispLedDuty();

    TurnOffAllLED();

#if CONFIG_TEST_HOT_OUT
    DispHotOutTest();
#endif

#if CONFIG_TEST_HOT_OUT
    DispSettingButton( (U8)GET_STATUS_VALVE_HOT_IN() );
#else
    // ?§Ï†ï Î©îÎâ¥ OFF
    DispSetting( OFF );
#endif

    // Ï∂îÏ∂ú Î≤ÑÌäº
    DisplayWaterOut();
    DisplayIceOut();

    // ?ºÏùå ?†ÌÉù
    DisplaySelIce();

    // ?ºÏùå Îπ†Î•∏ ?úÎπô - Only CP
    DisplaySelIceTurbo();

    // Î¨ºÏÑ†??
    DisplaySelWater();

    // ?ÑÌÑ∞ ÍµêÏ≤¥ ?åÎûå
    if( GetCustomerType() == CUSTOMER_VISIT )
    {
        DispFilterStatus( OFF );
    }
    else
    {
        DispFilterStatus( GetFilterUsageStatus() );
    }

    // ?âÏàò ?®ÎèÑ ?úÏãú??
    DisplayColdTemp();

    // ?®Ïàò ?†ÌÉù ???úÏãú ?¥Ïö©
    DisplaySelHot();

    // Î¨ºÏñë ?†ÌÉù ???úÏãú ?¥Ïö©
    DisplaySelAmount();

    // Refresh water out...
    if( GetWaterOut() == TRUE || IsOpenIceDoor() == TRUE )
    {
        //StartDisplayClock();
        StartDispTimerId( DISP_TIMER_CLOCK );
    }


#if CONFIG_UV_MANUAL
    Test_DisplayUv();
#else
    DisplayUv();
#endif

    // ICE FULL
    DispIceFull( GetIceFullStatus() );

    // ?àÏ†Ñ ?ôÏûë ?úÏãú
    DispSaving( GetSavingConfig() );

    DisplayWifiStatus();

    // WEATHER
    DisplayWeather();

    // CLOCK 
    if( IsExpiredDispTimer( DISP_TIMER_CLOCK ) == TRUE 
            && GetRtcConfigInit() == TRUE )
    {
        DisplayClock();
    }
    else
    {
        Display7Segment();
    }

    // Alaram Setting
    DisplayAlarmSetting();
}


static void ProcessDisplayErrorMode(void)
{
    static ErrorId_T mu8PrevErr = 0;
    ErrorId_T mu8CurErr = 0;

    static U8 mu8Cmd = ON;
    static U8 mu8Counter = 10;

    if( IsExpiredDispTimer( DISP_TIMER_ERROR ) == TRUE )
    {
        mu8Cmd = ON;
        return ;
    }

    /* Get error code */
    mu8CurErr = GetErrorId();

    /* Display error code */
    if( mu8CurErr == ERR_NONE )
    {
        mu8Cmd = ON;
        mu8Counter = 10;
    }
    else
    {
        mu8Counter--;
        if( mu8Counter == 0 )
        {
            mu8Counter = 10;
        }

        if( mu8Counter <= 5 )
        {
            mu8Cmd = OFF;
        }
        else
        {
            mu8Cmd = ON;
        }

        DisplayError( mu8CurErr, mu8Cmd );
    }

    /* Sound new error code */
    if( mu8PrevErr != mu8CurErr )
    {
        mu8PrevErr = mu8CurErr;
    }
}

void ProcessDisplay(void)
{
    UpdateDisplayTimer();

    /* BOOT */
    if( Disp.Init == FALSE )
    {
        ProcessDisplayBoot();
        return ;
    }

    /* VERSION */
    if( Disp.Version == TRUE )
    {
#if CONFIG_JIG_RBK_FRONT
        ProcessDisplayVersionModeFrontJIG();
#else
        ProcessDisplayVersionMode();
#endif
        return ;
    }

    /* TEST - EOL */
    if( GetEolStatus() == TRUE )
    {
        // Display Ï¥àÍ∏∞??Ï§? EOL Í≤Ä??Î™®Îìú ÏßÑÏûÖ?òÎ©¥ 
        // Ï¶âÏãú Ï¥àÍ∏∞?îÎ? Ï§ëÏ??òÍ≥† EOL Í≤Ä?¨Î™®?úÎ°ú ?ôÏûë?úÎã§.
        Disp.Init = TRUE;
        ProcessDisplayEol();
        return ;
    }

    /* TEST - FCT */
    if( GetFctStatus() == TRUE )
    {
        Disp.Init = TRUE;
        return ;
    }

    /* SMART CHECK */
    if( IsStartDiagnosis() == TRUE )
    {
        // Display Ï¥àÍ∏∞??Ï§? EOL Í≤Ä??Î™®Îìú ÏßÑÏûÖ?òÎ©¥ 
        // Ï¶âÏãú Ï¥àÍ∏∞?îÎ? Ï§ëÏ??òÍ≥† EOL Í≤Ä?¨Î™®?úÎ°ú ?ôÏûë?úÎã§.
        Disp.Init = TRUE;
        ProcessDisplaySmartCheck();
        return ;
    }

    /* CODY KIT */
    if( GetCodyKitStatus() == TRUE )
    {
        ProcessDisplayCodyKit();
        return ;
    }

    /* CODY KIT */
    if( IsStartEmptyColdTank() == TRUE )
    {
        ProcessDisplayEmptyCold();
        return ;
    }

    /* FILTER ALARAM - ?ÑÌÑ∞ ÍµêÏ≤¥ ?åÎûå */
    if( Disp.Filter == TRUE )
    {
        SetSettingDisp( FALSE );
        ProcessDisplayFilterMode();
        return ;
    }

    /* LOCK ALL */
    if( GetLockAll() == LOCK )
    {
        SetSettingDisp( FALSE );
        ProcessDisplayLockAll();
        return ;
    }

    /* MEMENTO */
    if( Disp.Memento == TRUE )
    {
        ProcessDisplayMementoMode();
        return ;
    }

    /* ?ÑÏû¨ ?úÍ∞Ñ ?§Ï†ï */
    if( Disp.Clock == TRUE )
    {
        ProcessDisplaySettingClock();
        return ;
    }

    /* SETTING */
    if( Disp.Setting == TRUE )
    {
        ProcessDisplaySettingMode();
        return ;
    }

    /* ?åÏÑ±/?®Í≥º??Î¨¥Ïùå ?§Ï†ï */
    if( Disp.Sound == TRUE )
    {
        ProcessDisplaySoundMode();
        return ;
    }

    /* ?åÏÑ±/?®Í≥º??Î¨¥Ïùå ?§Ï†ï */
    if( Disp.Customer == TRUE )
    {
        ProcessDisplayCustomerMode();
        return ;
    }

    if( IsStartDrainWater() == TRUE )
    {
        ProcessDisplayHealth();
        return ;
    }

    if( IsDoneFlushInitSetup() == FALSE )
    {
        /* FLUSHING */
        ProcessDisplayFlushingMode();
    }
    else if( GetSavingStatus() == TRUE 
            && IsExpiredDispTimer( DISP_TIMER_CLOCK ) == TRUE )
    {
        ProcessDisplaySavingMode();
    }
    else 
    {
        /* NORMAL MODE */
        ProcessDisplayNormalMode();
    }

    /* ERRROR MODE */
    ProcessDisplayErrorMode();
}
