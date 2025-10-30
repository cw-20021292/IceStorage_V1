/**
 * File : api_led.c
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#include "hw.h"

#include "api_led.h"
#include "hal_timer.h"

void LedInit(void)
{
    HAL_TIMER01_START();
}

/*
 *********************************************************************************
 * UV 및 상태 LED
 *********************************************************************************
 */

void LedOnOffUvHotSterTxt(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_UV_HOT_STER_TXT(); }
    else { TURN_OFF_LED_UV_HOT_STER_TXT(); }
}

void LedOnOffLedUvCareSystem(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_UV_CARE_SYSTEM(); }
    else { TURN_OFF_LED_UV_CARE_SYSTEM(); }
}

void LedOnOffLedUvIceTank(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_UV_ICE_TANK(); }
    else { TURN_OFF_LED_UV_ICE_TANK(); }
}

void LedOnOffLedUvIceTray(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_UV_ICE_TRAY(); }
    else { TURN_OFF_LED_UV_ICE_TRAY(); }
}

void LedOnOffLedUvIceFaucet(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_UV_ICE_FAUCET(); }
    else { TURN_OFF_LED_UV_ICE_FAUCET(); }
}

void LedOnOffLedUvWaterFaucet(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_UV_WATER_FAUCET(); }
    else { TURN_OFF_LED_UV_WATER_FAUCET(); }
}

void LedOnOffLedTempStep1(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_TEMP_STEP_1(); }
    else { TURN_OFF_LED_TEMP_STEP_1(); }
}

void LedOnOffLedTempStep2(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_TEMP_STEP_2(); }
    else { TURN_OFF_LED_TEMP_STEP_2(); }
}

void LedOnOffLedTempStep3(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_TEMP_STEP_3(); }
    else { TURN_OFF_LED_TEMP_STEP_3(); }
}

void LedOnOffLedTempStep4(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_TEMP_STEP_4(); }
    else { TURN_OFF_LED_TEMP_STEP_4(); }
}

void LedOnOffLedTempStep5(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_TEMP_STEP_5(); }
    else { TURN_OFF_LED_TEMP_STEP_5(); }
}

void LedOnOffLedStatusIceFull(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_STATUS_ICE_FULL(); }
    else { TURN_OFF_LED_STATUS_ICE_FULL(); }
}

void LedOnOffLedStatusIceLack(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_STATUS_ICE_LACK(); }
    else { TURN_OFF_LED_STATUS_ICE_LACK(); }
}

void LedOnOffLedStatusUnitCelsius(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_STATUS_UNIT_CELSIUS(); }
    else { TURN_OFF_LED_STATUS_UNIT_CELSIUS(); }
}

void LedOnOffLedStatusBarLow(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_STATUS_BAR_LOW(); }
    else { TURN_OFF_LED_STATUS_BAR_LOW(); }
}

void LedOnOffLedStatusBar2(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_STATUS_BAR_2(); }
    else { TURN_OFF_LED_STATUS_BAR_2(); }
}

void LedOnOffLedStatusBarMiddle(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_STATUS_BAR_MIDDLE(); }
    else { TURN_OFF_LED_STATUS_BAR_MIDDLE(); }
}

void LedOnOffLedStatusBarHigh(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_STATUS_BAR_HIGH(); }
    else { TURN_OFF_LED_STATUS_BAR_HIGH(); }
}

void LedOnOffLedStatusBarInfinite(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_STATUS_BAR_INFINITE(); }
    else { TURN_OFF_LED_STATUS_BAR_INFINITE(); }
}

void LedOnOffLedStatusUnitMl(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_STATUS_UNIT_ML(); }
    else { TURN_OFF_LED_STATUS_UNIT_ML(); }
}

void LedOnOffLedStatusUnitL(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_STATUS_UNIT_L(); }
    else { TURN_OFF_LED_STATUS_UNIT_L(); }
}

void LedOnOffLedAmount(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_AMOUNT(); }
    else { TURN_OFF_LED_AMOUNT(); }
}

void LedOnOffLedCold(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_COLD(); }
    else { TURN_OFF_LED_COLD(); }
}

void LedOnOffLedRoom(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_ROOM(); }
    else { TURN_OFF_LED_ROOM(); }
}

void LedOnOffLedHot(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_HOT(); }
    else { TURN_OFF_LED_HOT(); }
}

void LedOnOffLedIceWater(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_ICE_WATER(); }
    else { TURN_OFF_LED_ICE_WATER(); }
}

void LedOnOffLedIce(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_ICE(); }
    else { TURN_OFF_LED_ICE(); }
}

void LedOnOffLedAllLock(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_ALL_LOCK(); }
    else { TURN_OFF_LED_ALL_LOCK(); }
}

void LedOnOffLedIceOff(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_ICE_OFF(); }
    else { TURN_OFF_LED_ICE_OFF(); }
}

void LedOnOffIceSizeKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_ICE_SIZE_SEL(); }
    else { TURN_OFF_LED_ICE_SIZE_SEL(); }
}

/*
 *********************************************************************************
 * 웰컴 라이팅 (RGBW)
 *********************************************************************************
 */

void LedOnOffLedEXTRACTRightBlue(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_RIGHT_BLUE(); }
    else { TURN_OFF_LED_EXTRACT_RIGHT_BLUE(); }
}

void LedOnOffLedEXTRACTRightGreen(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_RIGHT_GREEN(); }
    else { TURN_OFF_LED_EXTRACT_RIGHT_GREEN(); }
}

void LedOnOffLedEXTRACTRightRed(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_RIGHT_RED(); }
    else { TURN_OFF_LED_EXTRACT_RIGHT_RED(); }
}

void LedOnOffLedEXTRACTRightWhite(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_RIGHT_WHITE(); }
    else { TURN_OFF_LED_EXTRACT_RIGHT_WHITE(); }
}

void LedOnOffLedEXTRACTLeftBlueOuter(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_LEFT_BLUE(); }
    else { TURN_OFF_LED_EXTRACT_LEFT_BLUE(); }
}

void LedOnOffLedEXTRACTLeftGreenOuter(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_LEFT_GREEN(); }
    else { TURN_OFF_LED_EXTRACT_LEFT_GREEN(); }
}

void LedOnOffLedEXTRACTLeftRedOuter(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_LEFT_RED(); }
    else { TURN_OFF_LED_EXTRACT_LEFT_RED(); }
}

void LedOnOffLedEXTRACTLeftWhiteOuter(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_LEFT_WHITE(); }
    else { TURN_OFF_LED_EXTRACT_LEFT_WHITE(); }
}

void LedOnOffLedEXTRACTLeftBlueI(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_LEFT_BLUE_I(); }
    else { TURN_OFF_LED_EXTRACT_LEFT_BLUE_I(); }
}

void LedOnOffLedEXTRACTLeftGreenI(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_LEFT_GREEN_I(); }
    else { TURN_OFF_LED_EXTRACT_LEFT_GREEN_I(); }
}

void LedOnOffLedEXTRACTLeftRedI(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_LEFT_RED_I(); }
    else { TURN_OFF_LED_EXTRACT_LEFT_RED_I(); }
}

void LedOnOffLedEXTRACTLeftWhiteI(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_EXTRACT_LEFT_WHITE_I(); }
    else { TURN_OFF_LED_EXTRACT_LEFT_WHITE_I(); }
}

/*
 *********************************************************************************
 * 아이콘 및 표시 LED
 *********************************************************************************
 */

void LedOnOffFastIce(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_FAST_ICE(); }
    else { TURN_OFF_FAST_ICE(); }
}

void LedOnOffSleepIcon(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_SLEEP_ICON(); }
    else { TURN_OFF_SLEEP_ICON(); }
}

void LedOnOffColon(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_COLON(); }
    else { TURN_OFF_COLON(); }
}

void LedOnOffRamen(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_RAMEN(); }
    else { TURN_OFF_RAMEN(); }
}

void LedOnOffDripCoffee(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_DRIP_COFFEE(); }
    else { TURN_OFF_DRIP_COFFEE(); }
}

void LedOnOffTea(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_TEA(); }
    else { TURN_OFF_TEA(); }
}

void LedOnOffMy1(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_MY1(); }
    else { TURN_OFF_MY1(); }
}

void LedOnOffMy2(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_MY2(); }
    else { TURN_OFF_MY2(); }
}

void LedOnOffMy3(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_MY3(); }
    else { TURN_OFF_MY3(); }
}

void LedOnOffSegDot(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_SEG_DOT(); }
    else { TURN_OFF_SEG_DOT(); }
}

void LedOnOffFilterCleanTxt(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_FILTER_CLEAN_TXT(); }
    else { TURN_OFF_FILTER_CLEAN_TXT(); }
}

void LedOnOffWifi(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_WIFI(); }
    else { TURN_OFF_WIFI(); }
}

void LedOnOffSunny(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_SUNNY(); }
    else { TURN_OFF_SUNNY(); }
}

void LedOnOffCloud(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_CLOUD(); }
    else { TURN_OFF_CLOUD(); }
}

void LedOnOffRain(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_RAIN(); }
    else { TURN_OFF_RAIN(); }
}

void LedOnOffPercent(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_PERCENT(); }
    else { TURN_OFF_PERCENT(); }
}

void LedOnOffIceTypeNormal(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_TYPE_NORMAL(); }
    else { TURN_OFF_ICE_TYPE_NORMAL(); }
}

void LedOnOffIceTypePiece(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_TYPE_PIECE(); }
    else { TURN_OFF_ICE_TYPE_PIECE(); }
}

void LedOnOffIceLocker(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_LOCKER(); }
    else { TURN_OFF_ICE_LOCKER(); }
}

void LedOnOffIceHotWaterLocker(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_HOT_WATER_LOCKER(); }
    else { TURN_OFF_ICE_HOT_WATER_LOCKER(); }
}

void LedOnOffIceCompWhite(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_COMP_WHITE(); }
    else { TURN_OFF_ICE_COMP_WHITE(); }
}

void LedOnOffIceCompBlue(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_COMP_BLUE(); }
    else { TURN_OFF_ICE_COMP_BLUE(); }
}

void LedOnOffIceBigIcon(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_BIG_ICON(); }
    else { TURN_OFF_ICE_BIG_ICON(); }
}

void LedOnOffIceSmallIcon(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_SMALL_ICON(); }
    else { TURN_OFF_ICE_SMALL_ICON(); }
}

void LedOnOffIceLine(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_LINE(); }
    else { TURN_OFF_ICE_LINE(); }
}

void LedOnOffColdTemp(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_COLD_TEMP(); }
    else { TURN_OFF_COLD_TEMP(); }
}

void LedOnOffColdTempWeak(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_COLD_TEMP_WEAK(); }
    else { TURN_OFF_COLD_TEMP_WEAK(); }
}

void LedOnOffColdTempStrong(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_COLD_TEMP_STRONG(); }
    else { TURN_OFF_COLD_TEMP_STRONG(); }
}

void LedOnOffColdTempLine(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_COLD_TEMP_LINE(); }
    else { TURN_OFF_COLD_TEMP_LINE(); }
}

/*
 *********************************************************************************
 * 키 버튼 LED
 *********************************************************************************
 */

void LedOnOffIceWaterKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_WATER_KEY(); }
    else { TURN_OFF_ICE_WATER_KEY(); }
}

void LedOnOffIceKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_KEY(); }
    else { TURN_OFF_ICE_KEY(); }
}

void LedOnOffHotWaterKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_HOT_WATER_KEY(); }
    else { TURN_OFF_HOT_WATER_KEY(); }
}

void LedOnOffRoomWaterKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ROOM_WATER_KEY(); }
    else { TURN_OFF_ROOM_WATER_KEY(); }
}

void LedOnOffColdWaterKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_COLD_WATER_KEY(); }
    else { TURN_OFF_COLD_WATER_KEY(); }
}

void LedOnOffAmountKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_AMOUNT_KEY(); }
    else { TURN_OFF_AMOUNT_KEY(); }
}

void LedOnOffMyKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_MY_KEY(); }
    else { TURN_OFF_MY_KEY(); }
}

void LedOnOffHotWaterLockKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_HOT_WATER_LOCK_KEY(); }
    else { TURN_OFF_HOT_WATER_LOCK_KEY(); }
}

void LedOnOffFastIceKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_FAST_ICE_KEY(); }
    else { TURN_OFF_FAST_ICE_KEY(); }
}

void LedOnOffColdOffKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_COLD_OFF_KEY(); }
    else { TURN_OFF_COLD_OFF_KEY(); }
}

void LedOnOffSleepModeKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_SLEEP_MODE_KEY(); }
    else { TURN_OFF_SLEEP_MODE_KEY(); }
}

void LedOnOffAllLockKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ALL_LOCK_KEY(); }
    else { TURN_OFF_ALL_LOCK_KEY(); }
}

void LedOnOffIceLockKey(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_ICE_LOCK_KEY(); }
    else { TURN_OFF_ICE_LOCK_KEY(); }
}

void LedOnOffWelcomeLedLeft(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_WELCOME_LEFT(); }
    else { TURN_OFF_LED_WELCOME_LEFT(); }
}

void LedOnOffWelcomeLedRight(U8 onOff)
{
    if( onOff == ON ) { TURN_ON_LED_WELCOME_RIGHT(); }
    else { TURN_OFF_LED_WELCOME_RIGHT(); }
}