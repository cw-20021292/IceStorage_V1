/**
 * File : api_led.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#ifndef __API_LED_H__
#define __API_LED_H__

#include "prj_type.h"

void LedInit(void);
void LedOnOffUvHotSterTxt(U8 onOff);
void LedOnOffLedUvCareSystem(U8 onOff);
void LedOnOffLedUvIceTank(U8 onOff);
void LedOnOffLedUvIceTray(U8 onOff);
void LedOnOffLedUvIceFaucet(U8 onOff);
void LedOnOffLedUvWaterFaucet(U8 onOff);
void LedOnOffLedTempStep1(U8 onOff);
void LedOnOffLedTempStep2(U8 onOff);
void LedOnOffLedTempStep3(U8 onOff);
void LedOnOffLedTempStep4(U8 onOff);
void LedOnOffLedTempStep5(U8 onOff);
void LedOnOffLedStatusIceFull(U8 onOff);
void LedOnOffLedStatusIceLack(U8 onOff);
void LedOnOffLedStatusUnitCelsius(U8 onOff);
void LedOnOffLedStatusBarLow(U8 onOff);
void LedOnOffLedStatusBar2(U8 onOff);
void LedOnOffLedStatusBarMiddle(U8 onOff);
void LedOnOffLedStatusBarHigh(U8 onOff);
void LedOnOffLedStatusBarInfinite(U8 onOff);
void LedOnOffLedStatusUnitMl(U8 onOff);
void LedOnOffLedStatusUnitL(U8 onOff);
void LedOnOffLedAmount(U8 onOff);
void LedOnOffLedCold(U8 onOff);
void LedOnOffLedRoom(U8 onOff);
void LedOnOffLedHot(U8 onOff);
void LedOnOffLedIceWater(U8 onOff);
void LedOnOffLedIce(U8 onOff);
void LedOnOffLedAllLock(U8 onOff);
void LedOnOffLedIceOff(U8 onOff);
/*
 *********************************************************************************
 * 웰컴 라이팅 (RGBW)
 *********************************************************************************
 */
void LedOnOffLedWelcomeRightBlue(U8 onOff);
void LedOnOffLedWelcomeRightGreen(U8 onOff);
void LedOnOffLedWelcomeRightRed(U8 onOff);
void LedOnOffLedWelcomeRightWhite(U8 onOff);
void LedOnOffLedWelcomeLeftBlue(U8 onOff);
void LedOnOffLedWelcomeLeftGreen(U8 onOff);
void LedOnOffLedWelcomeLeftRed(U8 onOff);
void LedOnOffLedWelcomeLeftWhite(U8 onOff);
void LedOnOffLedWelcomeLeftBlueI(U8 onOff);
void LedOnOffLedWelcomeLeftGreenI(U8 onOff);
void LedOnOffLedWelcomeLeftRedI(U8 onOff);
void LedOnOffLedWelcomeLeftWhiteI(U8 onOff);

/*
 *********************************************************************************
 * 아이콘 및 표시 LED
 *********************************************************************************
 */
void LedOnOffFastIce(U8 onOff);
void LedOnOffSleepIcon(U8 onOff);
void LedOnOffColon(U8 onOff);
void LedOnOffRamen(U8 onOff);
void LedOnOffDripCoffee(U8 onOff);
void LedOnOffTea(U8 onOff);
void LedOnOffMy1(U8 onOff);
void LedOnOffMy2(U8 onOff);
void LedOnOffMy3(U8 onOff);
void LedOnOffSegDot(U8 onOff);
void LedOnOffFilterCleanTxt(U8 onOff);
void LedOnOffWifi(U8 onOff);
void LedOnOffSunny(U8 onOff);
void LedOnOffCloud(U8 onOff);
void LedOnOffRain(U8 onOff);
void LedOnOffPercent(U8 onOff);
void LedOnOffIceTypeNormal(U8 onOff);
void LedOnOffIceTypePiece(U8 onOff);
void LedOnOffIceLocker(U8 onOff);
void LedOnOffIceHotWaterLocker(U8 onOff);
void LedOnOffIceCompWhite(U8 onOff);
void LedOnOffIceCompBlue(U8 onOff);
void LedOnOffIceBigIcon(U8 onOff);
void LedOnOffIceSmallIcon(U8 onOff);
void LedOnOffIceLine(U8 onOff);
void LedOnOffColdTemp(U8 onOff);
void LedOnOffColdTempWeak(U8 onOff);
void LedOnOffColdTempStrong(U8 onOff);
void LedOnOffColdTempLine(U8 onOff);

/*
 *********************************************************************************
 * 키 버튼 LED
 *********************************************************************************
 */
void LedOnOffIceWaterKey(U8 onOff);
void LedOnOffIceKey(U8 onOff);
void LedOnOffHotWaterKey(U8 onOff);
void LedOnOffRoomWaterKey(U8 onOff);
void LedOnOffColdWaterKey(U8 onOff);
void LedOnOffAmountKey(U8 onOff);
void LedOnOffMyKey(U8 onOff);
void LedOnOffIceSizeKey(U8 onOff);
void LedOnOffHotWaterLockKey(U8 onOff);
void LedOnOffFastIceKey(U8 onOff);
void LedOnOffColdOffKey(U8 onOff);
void LedOnOffSleepModeKey(U8 onOff);
void LedOnOffAllLockKey(U8 onOff);
void LedOnOffIceLockKey(U8 onOff);

void LedOnOffWelcomeLedLeft(U8 onOff);
void LedOnOffWelcomeLedRight(U8 onOff);

void LedOnOffLedEXTRACTRightBlue(U8 onOff);
void LedOnOffLedEXTRACTRightGreen(U8 onOff);
void LedOnOffLedEXTRACTRightRed(U8 onOff);
void LedOnOffLedEXTRACTRightWhite(U8 onOff);
void LedOnOffLedEXTRACTLeftBlueOuter(U8 onOff);
void LedOnOffLedEXTRACTLeftGreenOuter(U8 onOff);
void LedOnOffLedEXTRACTLeftRedOuter(U8 onOff);
void LedOnOffLedEXTRACTLeftWhiteOuter(U8 onOff);
void LedOnOffLedEXTRACTLeftBlueI(U8 onOff);
void LedOnOffLedEXTRACTLeftGreenI(U8 onOff);
void LedOnOffLedEXTRACTLeftRedI(U8 onOff);
void LedOnOffLedEXTRACTLeftWhiteI(U8 onOff);

#endif  /* __API_LED_H__ */
