#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "prj_type.h"
#include "rtc.h"


// 7세그먼트 숫자 표시( 0 ~ 999 )
void DispSegmentDigit(U16 mu16Val);

// 7세그먼트 문자 표시 ID
enum
{
    SEGMENT_CHAR_ID_ON,           /* On */
    SEGMENT_CHAR_ID_OFF,          /* OFF */
    SEGMENT_CHAR_ID_CON,          /* Con */
    SEGMENT_CHAR_ID_STACK_BAR_CENTER,   /* --- */
    SEGMENT_CHAR_ID_STACK_BAR_TOP,      /* ``` */
    SEGMENT_CHAR_ID_BAR_BOTTOM,   /* ___ */
    SEGMENT_CHAR_ID_BAR_CENTER,   /* --- */
    SEGMENT_CHAR_ID_BAR_TOP,      /* ``` */
    SEGMENT_CHAR_ID_CHP,          /* CHP */
    SEGMENT_CHAR_ID_CP,           /* CP  */
    SEGMENT_CHAR_ID_CIRCLE,       /* CIRCLE  */
    SEGMENT_CHAR_ID_SPACE,        /* SPACE  */
    SEGMENT_CHAR_ID_ICE_1,         /* ICE OUT-EFFECT 1  */
    SEGMENT_CHAR_ID_ICE_2,         /* ICE OUT-EFFECT 2 */
    SEGMENT_CHAR_ID_ICE_3,         /* ICE OUT-EFFECT 3 */
    SEGMENT_CHAR_ID_ICE_4,         /* ICE OUT-EFFECT 4 */
    SEGMENT_CHAR_ID_CENTER_1,      /* SOUND VOL -1- */
    SEGMENT_CHAR_ID_CENTER_2,      /* SOUND VOL -2- */
    SEGMENT_CHAR_ID_CENTER_3,      /* SOUND VOL -3- */
    SEGMENT_CHAR_ID_SE,             /* CUSTOMER MODE - SELF */
    SEGMENT_CHAR_ID_CO,             /* CUSTOMER MODE - COWAY(?) - VISIT */
    SEGMENT_CHAR_ID_STE,            /* 살균 - StE */
    MAX_SEGMENT_CHAR_ID,        
};

void DispSegmentChar(U8 mu8CharId);

void DispSegmentOff(void);

// 에러 코드 표시 ( E01 ~ E99 )
void DispSegmentError(U8 mu8ErrCode );

// WIFI 에러 코드 표시 (AP1 ~ AP5 )
void DispSegmentWifiError(U8 mu8ErrCode );

// 연속 추출 화면 표시
void DispSegmentCon(U8 mu8Step);

/* Functinos */
void TurnOffAllLED(void);
void TurnOnAllLED(void);

void DispLedDuty(void);

// 얼음 추출
#if 0
void OffDispIceOut(void);
#endif
void DispIceOut( U8 mu8OnOff );

// 물 추출
void DispWaterOutCircle(U8 mu8OnOff);
//void OffDispWaterOut(void);
void DispWaterOut( U8 mu8OnOff );

// 냉각 온도 상태 표시
// 냉수 온도가 높으면 WHITE LED
// 냉수 온도가 낮으면 BLUE LED
// mu8OnOff : ON, OFF
// mu8ColdGood : TRUE, FALSE
void DispColdTemp(U8 mu8OnOff, U8 mu8ColdGood);


// 얼음 
void DispIceSizeSel(U8 mu8Size, U8 mu8Lock, U8 mu8Dimming);
void DispIceSel(U8 mu8Turbo, U8 mu8Size, U8 mu8Lock, U8 mu8Dimming);

// 얼음 선택 표시 OFF
void DispIceSelOff(U8 mu8OnOff, U8 mu8Turbo, U8 mu8Lock, U8 mu8SizeOnOff, U8 mu8Size);
//void DispIceSelOff(U8 mu8OnOff, U8 mu8Turbo, U8 mu8Lock, U8 mu8Size);

// 얼음 빠른제빙 - Only CP
void DispIceTurbo(U8 mu8OnOff);
void DispIceTurboOff(void);

// 냉온정 선택
void DispWater(U8 mu8Sel);
void DispWaterSel(U8 mu8Sel, U8 mu8ConfCold);

// 냉수 비우기 모드 버튼 표시
void DispWaterSelEmptyColdFaucet(void);
void DispWaterSelEmptyColdDrain(void);


// 설정 버튼
void DispSettingButton(U8 mu8OnOff);
void DispAlarmLockAll(U8 mu8OnOff);
void DispAlarmLockHot(U8 mu8OnOff);
void DispAlarmLockIce(U8 mu8OnOff);
void DispAlarmMakeIceCp(U8 mu8OnOff);
void DispAlarmMakeIce(U8 mu8OnOff);
void DispAlarmMakeCold(U8 mu8OnOff);

// 설정 
void DispSetting(U8 mu8OnOff );

// WELCOM LED - WATER 
void DispWelcomWater( U8 mu8OnOff );

// WELCOM LED - ICE 
void DispWelcomIce( U8 mu8OnOff );

// 물양
void DispAmountSel(U8 mu8Amount, U8 mu8Dimming );
void DispAmountSelOff(U8 mu8Amount );
void DispAmount(U8 mu8OnOff);

void ResetInfinityCount(void);
void DispWaterOutAmountInfinity(void);

// 온수 예열
void DispHotWaterPreHeating(U8 mu8Step, U8 mu8Temp);

// 온수 잠김
//void DispHotLock(U8 mu8OnOff);

// 온수 종류 - 변경 중
//void DispHotSel(U8 mu8Sel, U8 mu8Lock, U8 mu8Dimming);

// 온수 선택 표시 OFF
void DispHotSelOff(U8 mu8Lock);


void DispUvCare(U8 mu8OnOff, U8 mu8WaterOut, U8 mu8IceOut, U8 mu8IceTray, U8 mu8IceTank);

// VERSION
void DispVersion( U8 mu8Version);

//// VERSION ON time 2sec..
//// return ture : time out
//// return false : goning on..
//U8 DispVersionOnTime(void);

// WIFI
void DispWifi(U8 mu8OnOff);

void DispWifiBle(U8 mu8OnOff);

// FILTER FLUSHING
void DispFlushing(U8 mu8OnOff);

// FILTER USAGE STATUS
void DispFilterStatus(U8 mu8OnOff);

// ICE FULL 
void DispIceFull(U8 mu8OnOff);

// SAVING ICON
void DispSaving(U8 mu8OnOff);

// ICE MAKE
//void DispIceMake(U8 mu8OnOff);

// COLD MAKE
//void DispColdMake(U8 mu8OnOff);

// LOCK - ALL 
void DispAllLock(U8 mu8OnOff);
void DispAllLockSel(U8 mu8OnOff);

// LOCK - ICE
//void DispIceLock(U8 mu8OnOff);


// Water outing...
void DispWaterOutAmount(U16 mu16Val);

// 퍼센트 표시 0% ~ 100%
void DispPercent(U16 mu16Val);


// 사운드 볼륨 설정 
void DispSettingSoundVolume(U8 mu8Vol);

// 사운드 모드 설정
void DispSettingSoundMode(U8 mu8Mode);

// 관리 모드 설정 
void DispSettingCustomerMode(U8 mu8Mode);

// 시간 설정 버튼 
void DispSettingClockKey(void);

// 사운드 설정 버튼
void DispSettingSoundKey(void);

// 사운드 모드에서의 주요 버튼
void DispSettingKey(void);

// 온도 표시
void DispTemp(U16 mu16Val);

// 현재 시간 표시
void DispClock(TimeData_T *pTime);

// 현재 시간 설정 완료
void DispSettingClockDone(U8 mu8OnOff);

// 현재 시간 설정 
void DispSettingClock(U8 mItem, U8 mAmPm, U8 mHour, U8 mMin);

// 날씨 표시
#define WEATHER_OFF     0
#define WEATHER_SUN     1
#define WEATHER_CLOUD   2
#define WEATHER_RAIN    3
void DispWeather(U8 mu8Status );

// 자동배수
void DispDrainWater(U8 mu8OnOff, U8 mu8Percent);


// CODY KIT
void DispRoom(U8 mu8OnOff);

void DispColdDimming(U8 mu8OnOff);

void DispCold(U8 mu8OnOff);

#if 0
// STATUS 
#define STATUS_STOP     0       // 표시 없음
#define STATUS_OFF      1       // OFF 표시
#define STATUS_MAKE     2       // 진행 상태 표시
#define STATUS_DEICING  3       // 진행 ( 탈빙) 표시
U8 GetIceStatus(void);
U8 GetColdStatus(void);
U8 GetDispIceFullStatus(void);
#endif


// 메멘토 버튼 표시
void DispMementoKey( mu8Sel );

#endif /* __DISPLAY_H__ */
