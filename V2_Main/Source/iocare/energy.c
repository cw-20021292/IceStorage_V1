#include "hw.h"
#include "hal_adc.h"

#include "energy.h"
#include "rtc.h"
#include "eeprom.h"
#include "comp_bldc.h"
#include "heater_out.h"
#include "valve.h"
#include "drain_pump.h"

#define CONFIG_USE_EEPROM_ENERGY    0       // ?†Œë¹„ì „? ¥ eeprom ????ž¥ ?—¬ë¶?(0: ë¯¸ì‚¬?š©, 1:?‚¬?š©)

/* ICON AIS  
 *
 * STAND BY : 1.3w
 * COMP COLD - 45RPS : 36.3
 * HEATER RELAY :  2900w 45% = 1305watt  = 
 * HEATER TRIAC :  2900w 55% = 1595watt
 *
 */

// COMP : 100Wh , 100 / 3600 = 0.027777...w
// ICE HEATER : 150Wh , 150 / 3600 = 0.04166667w

/* #.###Wh @ 1sec */
// Example 27 = 0.027 watt per sec
//#define STAND_BY_WH                   21UL       // 0.000w  ( 1.3w )
//#define STAND_BY_BACK_LIGHT_ON_WH   36UL      // 0.036w   @1min  ( ???ê¸? ëª¨ë“œ?Š” ?‚¬?š©?Ÿ‰?´ ?ž‘?•„?„œ 1ë¶„ì´ ?•œ ë²? ? ?‚° )
//#define STAND_BY_BACK_LIGHT_OFF_WH  19UL      // 0.019w   @1min  ( ???ê¸? ëª¨ë“œ?Š” ?‚¬?š©?Ÿ‰?´ ?ž‘?•„?„œ 1ë¶„ì— ?•œ ë²? ? ?‚° )

//#define STAND_BY_BACK_LIGHT_ON_WH   37UL      // 0.036w   @1min  ( ???ê¸? ëª¨ë“œ?Š” ?‚¬?š©?Ÿ‰?´ ?ž‘?•„?„œ 1ë¶„ì´ ?•œ ë²? ? ?‚° )
//#define STAND_BY_BACK_LIGHT_OFF_WH  20UL      // 0.019w   @1min  ( ???ê¸? ëª¨ë“œ?Š” ?‚¬?š©?Ÿ‰?´ ?ž‘?•„?„œ 1ë¶„ì— ?•œ ë²? ? ?‚° )

//#define STAND_BY_BACK_LIGHT_ON_WH   72UL      // 0.036w x 2  @1min  ( ???ê¸? ëª¨ë“œ?Š” ?‚¬?š©?Ÿ‰?´ ?ž‘?•„?„œ 1ë¶„ì´ ?•œ ë²? ? ?‚° )
//#define STAND_BY_BACK_LIGHT_OFF_WH  57UL      // 0.019w x 3  @1min  ( ???ê¸? ëª¨ë“œ?Š” ?‚¬?š©?Ÿ‰?´ ?ž‘?•„?„œ 1ë¶„ì´ ?•œ ë²? ? ?‚° )
//#define ICE_HEATER_WH               42UL      // 0.042w
//#define SODA_PUMP_LOW_WH            40UL      // 0.040w
//#define SODA_PUMP_MID_WH            40UL      // 0.040w
//#define SODA_PUMP_HIGH_WH           40UL      // 0.040w

#define STAND_BY_WH                     31UL      // 0.000w  @1min ( 1.3w )
#define DC_FAN_WH                       21UL      // 0.000w  @1min ( 1.26w )

#define DRAIN_PUMP_WH                    1UL      // 0.000w  @1sec ( 7.0w )
#define VALVE_WH                         2UL      // 0.000w  @1sec ( 7.0w )
#define COMP_WH                         17UL      // 0.010w  @1sec
#define HEATER_WH                      805UL     // 0.000w  @1sec ( 2900w )

#define MAX_WATT                    0xFF000000UL

#define RATE_1KWH                   1879UL   /* 187.9 won */
#define MAX_RATE                    9999UL  /* 9,999 won */

#define MONTHS  12
typedef struct _energy_
{
    U8 u8Month;
    U32 u32WattHour[ MONTHS ];
} Energy_T;


Energy_T Energy; 

void InitEnergy(void)
{
    MEMSET( (void __FAR *)&Energy, 0, sizeof( Energy_T ) );
    Energy.u8Month = 1;
}

#if 0
U8 GetEnergyMonth(void)
{
    return Energy.u8Month;
}

void SetEnergyMonth(U8 mu8Month)
{
    if( mu8Month >= 1 && mu8Month <=12 )
    {
        Energy.u8Month = mu8Month;
    }
}
#endif


U32 GetEnergyRateMonth(U8 mu8Month)
{
    U32 mu32Watt = 0UL;
    U32 mu32Rate = 0UL;


    if( mu8Month >= 1 && mu8Month <= 12 )
    {
        mu32Watt = Energy.u32WattHour[ mu8Month - 1 ];
        //mu32Watt /= 1000;   // ?†Œ?ˆ«?  3?žë¦? ë²„ë¦¼
        mu32Watt /= 1000;   // kw ë³??™˜

        mu32Rate = mu32Watt * RATE_1KWH;    // ? „ê¸°ë£Œ ê³„ì‚°
        mu32Rate /= 10000UL;                  // ?†Œ?ˆ«?  ë²„ë¦¼ 
    }

    if( mu32Rate > MAX_RATE )
    {
        mu32Rate = MAX_RATE;
    }

    return mu32Rate;
}

U32 GetEnergyWattAll(void)
{
    U8 i;
    U32 mu32Watt;

    mu32Watt = Energy.u32WattHour[ 0 ];
    for( i = 1; i < 12 ; i++ )
    {
        mu32Watt += Energy.u32WattHour[ i ];
    }

    return mu32Watt;

}

#if 0
U32 GetEnergyWattMonth(U8 mu8Month)
{
    if( mu8Month >= 1 && mu8Month <= 12 )
    {
        return  Energy.u32WattHour[ mu8Month - 1 ];
    }

    return 0UL;
}

void SetEnergyWattMonth(U8 mu8Month, U32 mu32Watt)
{
    if( mu8Month >= 1 && mu8Month <= 12 )
    {
        Energy.u32WattHour[ mu8Month - 1 ] = mu32Watt;
    }
}
#endif


// 1ë¶„ì— ?•œ ë²? ????ž¥...
static U16 CalcStandBy(void)
{
    static U8 mu8Min = 59;

    if( --mu8Min == 0 )
    {
        mu8Min = 59;

        return STAND_BY_WH;
    }

    return 0;
}

#if 0
// 1ë¶„ì— ?•œ ë²? ????ž¥...
static U16 CalcDcFan(void)
{
    static U8 mu8Min = 59;


    if( GET_STATUS_FAN_MOTOR_1_ONOFF() != 0 )
    {
        if( --mu8Min == 0 )
        {
            mu8Min = 59;
            return DC_FAN_WH;
        }
    }

    return 0;
}


static U16 CalcValves(void)
{
    U16 mu16Sum = 0;

    if( IsOpenValve( VALVE_NOS ) == TRUE )          { mu16Sum += VALVE_WH; }
    if( IsOpenValve( VALVE_ROOM_COLD_OUT ) == TRUE )     { mu16Sum += VALVE_WH; }
    if( IsOpenValve( VALVE_HOT_IN ) == TRUE )       { mu16Sum += VALVE_WH; }
    if( IsOpenValve( VALVE_HOT_OUT ) == TRUE )      { mu16Sum += VALVE_WH; }
    if( IsOpenValve( VALVE_HOT_DRAIN ) == TRUE )    { mu16Sum += VALVE_WH; }
    if( IsOpenValve( VALVE_COLD_AIR ) == TRUE )     { mu16Sum += VALVE_WH; }
    if( IsOpenValve( VALVE_ROOM_COLD_OUT) == TRUE )     { mu16Sum += VALVE_WH; }
    if( IsOpenValve( VALVE_COLD_IN ) == TRUE )      { mu16Sum += VALVE_WH; }
    if( IsOpenValve( VALVE_COLD_DRAIN ) == TRUE )   { mu16Sum += VALVE_WH; }
    if( IsOpenValve( VALVE_COLD_FLUSH ) == TRUE )   { mu16Sum += VALVE_WH; }
    if( IsOpenValve( VALVE_ICE_TRAY_IN ) == TRUE )  { mu16Sum += VALVE_WH; }

    return mu16Sum;
}

static U16 CalcDrainPump(void)
{
    if( IsTurnOnDrainPump() == TRUE )
    {
        return DRAIN_PUMP_WH;
    }

    return 0;
}
#endif

static U16 CalcComp(void)
{
    if( IsTurnOnCompBldc() == TRUE 
            && GetCompBldcCurrentRps() > 0 )
    {
        return COMP_WH;
    }

    return 0UL;
}

static U16 CalcHotHeater(void)
{
    U16 Output;

    Output = GetHeaterOutput();
    if( Output > 0 )
    {
        return ( HEATER_WH * Output ) / 100UL;
    }

    return 0UL;
}


static void ClearEnergyWatt(void)
{
    U8 mu8Index;

    for( mu8Index = 0 ; mu8Index < MONTHS ; mu8Index++ )
    {
        Energy.u32WattHour[ mu8Index ] = 0UL;
    }
}

void ClearAllEnergyWatt(void)
{
    ClearEnergyWatt();
#if CONFIG_USE_EEPROM_ENERGY
    SaveEepromId( EEP_ID_ENERGY );
#endif
}

#define ENERGY_SAVE_TIME_SEC        180U   // 3min
void ProcessEnergy(void)
{
    static U8 mu8Init = FALSE;
    static U8 mu8PrevMonth = 0;
    static U8 mu8PrevYear = 0;
    static U16 mu16SaveTime = ENERGY_SAVE_TIME_SEC;
    U32 u32Watt = 0UL;
    TimeData_T  mTime;


    // Monthê°? ë³?ê²½ë˜ë©?, ?ƒˆë¡œìš´ Month?˜ watt?Š” ì´ˆê¸°?™”
    GetRtcTime( &mTime );
    if( mu8Init == FALSE )
    {
        mu8Init = TRUE;

        mu8PrevMonth = mTime.Month;
        mu8PrevYear = mTime.Year;
        return ;
    }

    // ?‚¬?š©?ž?— ?˜?•´ ?…„?„ê°? ë³?ê²½ë˜?Š” ê²½ìš°
    if( mu8PrevYear != mTime.Year )
    {
        mu8PrevYear = mTime.Year;

        ClearEnergyWatt();
#if CONFIG_USE_EEPROM_ENERGY
        SaveEepromId( EEP_ID_ENERGY );
#endif

        return ;
    }

    if( mTime.Month != Energy.u8Month )
    {
        Energy.u32WattHour[ mTime.Month - 1 ] = 0UL;
        Energy.u8Month = mTime.Month;

#if CONFIG_USE_EEPROM_ENERGY
        SaveEepromId( EEP_ID_ENERGY );
#endif
    }

    // CALC WATT
    u32Watt =  (U32)CalcStandBy();
#if 0
    u32Watt += (U32)CalcDcFan();
    u32Watt += (U32)CalcValves();
    u32Watt += (U32)CalcDrainPump();
#endif
    u32Watt += (U32)CalcComp();
    u32Watt += (U32)CalcHotHeater();

    // ADD WATT
    Energy.u32WattHour[ mTime.Month - 1 ] += u32Watt;
    if( Energy.u32WattHour[ mTime.Month - 1] > MAX_WATT )
    {
        Energy.u32WattHour[ mTime.Month - 1] = MAX_WATT;
    }

    // SAVE EEPROM DATA
    if( --mu16SaveTime == 0 )
    {
        mu16SaveTime = ENERGY_SAVE_TIME_SEC;
#if CONFIG_USE_EEPROM_ENERGY
        SaveEepromId( EEP_ID_ENERGY );
#endif
    }
}

