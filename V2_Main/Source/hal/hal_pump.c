#include "hw.h"
#include "hal_pump.h"


/* 펌프 기능 설정/해제 */
#define  CONFIG_CIRCULATE_PUMP  0   /* 냉수 순환 펌프,  0: 해제, 1: 설정 */
#define  CONFIG_SODA_PUMP       0   /* 소다 추출 펌프,  0: 해제, 1: 설정 */
#define  CONFIG_DRAIN_PUMP      1   /* 드레인 펌프,     0: 해제, 1: 설정 */


// Circulate Pump TIMER 
#define  PUMP_DRAIN_TDR_REG     TDR13

// 배수 펌프 START PWM TIMER
static void Timer_TurnOnDrainPump(void)
{
    TOE1 |= _0008_TAU_CH3_OUTPUT_ENABLE;
    TS1 |= _0004_TAU_CH2_START_TRG_ON | _0008_TAU_CH3_START_TRG_ON;
}

// 배수 펌프 STOP PWM TIMER
static void Timer_TurnOffDrainPump(void)
{
    TOE1 &= ~_0008_TAU_CH3_OUTPUT_ENABLE;
    TT1 |= _0004_TAU_CH2_STOP_TRG_ON | _0008_TAU_CH3_STOP_TRG_ON;
    TO1 &= ~_0008_TAU_CH3_OUTPUT_VALUE_1;
}

U8 HAL_IsTurnOnDrainPump(void)
{
    if( (TOE1 & _0008_TAU_CH3_OUTPUT_ENABLE) != 0 )
    {
        return TRUE;
    }

    return FALSE;
}


#if CONFIG_CIRCULATE_PUMP
#define  PUMP_CIRCULATE_TDR_REG TDR12
// 순환 펌프 START PWM TIMER
static void Timer_TurnOnCirculatePump(void)
{
    TOE1 |=  _0004_TAU_CH2_OUTPUT_ENABLE;
    TS1 |= _0001_TAU_CH0_START_TRG_ON | _0004_TAU_CH2_START_TRG_ON;
}

// 순환 펌프 STOP PWM TIMER
static void Timer_TurnOffCirculatePump(void)
{
    TOE1 &= ~_0004_TAU_CH2_OUTPUT_ENABLE;
    TS1 &= ~_0004_TAU_CH2_START_TRG_ON;
}
#endif// #if CONFIG_CIRCULATE_PUMP


#if CONFIG_SODA_PUMP
#define  PUMP_SODA_TDR_REG      TDR04
// 소다 펌프 START PWM TIMER 
static void Timer_TurnOnSodaPump(void)
{
    TOE0 |= _0010_TAU_CH4_OUTPUT_ENABLE;
    TS0 |= _0004_TAU_CH2_START_TRG_ON | _0010_TAU_CH4_START_TRG_ON;
}

// 소다 펌프 STOP PWM TIMER 
static void Timer_TurnOffSodaPump(void)
{
    TOE0 &= ~_0010_TAU_CH4_OUTPUT_ENABLE;
    TT0 |= _0004_TAU_CH2_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON;
    TO0 &= ~_0010_TAU_CH4_OUTPUT_VALUE_1;
}
#endif// #if CONFIG_SODA_PUMP


// 1600 Register Value : 24V 
// (1600/24) * xV = target register value
//#define DRAIN_MAX_TDR        32U  // 0x0C80
//#define DRAIN_MAX_TDR        32000U  // 0x0020
#define DRAIN_MAX_TDR        0x0C80 
//#define SODA_MAX_TDR         64U  // 0x0040
#define SODA_MAX_TDR         1600U  // 0x0640
static U16 ConvVolt2Reg(U16 mu16MaxTDR, F32 mf32Volt)
{
    F32 mfCalcVal = 0.0f;
    U16 mu16TDR = 0U;

    // TDR 레지스터 계산.
    if( mf32Volt <= 0.0f )
    {
        return 0;   // 0V
    }
    else
    {
        mfCalcVal = ((F32)mu16MaxTDR / 24.0f) * mf32Volt;
        mu16TDR = (U16)mfCalcVal;
    }
    
    if( mu16TDR > mu16MaxTDR )
    {
        mu16TDR = mu16MaxTDR;
    }

    return mu16TDR;
}


// 펌프 ON
void HAL_TurnOnPump(U8 mu8PumpId, F32 mf32Volt)
{
    if( mu8PumpId == HAL_PUMP_DRAIN )
    {
#if CONFIG_DRAIN_PUMP
        Timer_TurnOnDrainPump();
        PUMP_DRAIN_TDR_REG = ConvVolt2Reg( DRAIN_MAX_TDR, mf32Volt );
#endif
    }
#if CONFIG_CIRCULATE_PUMP
    else if( mu8PumpId == HAL_PUMP_CIRCULATE )
    {
        TURN_ON_PUMP_CIRCULATE();

        Timer_TurnOnCirculatePump();
        PUMP_CIRCULATE_TDR_REG = ConvVolt2Reg( SODA_MAX_TDR, mf32Volt );
    }
#endif
#if CONFIG_SODA_PUMP
    else if( mu8PumpId == HAL_PUMP_SODA )
    {
        Timer_TurnOnSodaPump();
        PUMP_SODA_TDR_REG = ConvVolt2Reg( SODA_MAX_TDR, mf32Volt );
    }
#endif
}


// 펌프 OFF 
void HAL_TurnOffPump(U8 mu8PumpId)
{
    if( mu8PumpId == HAL_PUMP_DRAIN )
    {
#if CONFIG_DRAIN_PUMP
        Timer_TurnOffDrainPump();
#endif
    }
#if CONFIG_CIRCULATE_PUMP
    else if( mu8PumpId == HAL_PUMP_CIRCULATE )
    {
        TURN_OFF_PUMP_CIRCULATE();

        Timer_TurnOffCirculatePump();
    }
#endif
#if CONFIG_SODA_PUMP
    else if( mu8PumpId == HAL_PUMP_SODA )
    {
        Timer_TurnOffSodaPump();
    }
#endif
}

// 펌프 POWER POWER ON
#if CONFIG_SODA_PUMP
void HAL_TurnOnPumpPower(U8 mu8PumpId )
{
    if( mu8PumpId == HAL_PUMP_SODA )
    {
        TURN_ON_PUMP_SODA();
    }
}

// 펌프 POWER OFF
void HAL_TurnOffPumpPower(U8 mu8PumpId )
{
    if( mu8PumpId == HAL_PUMP_SODA )
    {
        TURN_ON_PUMP_SODA();
    }
}
#endif// #if CONFIG_SODA_PUMP
