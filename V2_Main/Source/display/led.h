/**
 * File : led.h
 * 
 * Led
**/
#ifndef __LED_H__
#define __LED_H__

#include "prj_type.h"

#define LED_TYPE_DUTY_DIM   (0x00)
#define LED_TYPE_DUTY       (0x10)

#define LED_DUTY_ON_100     (100U)
#define LED_DUTY_ON_90      (90U)
#define LED_DUTY_ON_80      (80U)
#define LED_DUTY_ON_70      (70U)
#define LED_DUTY_ON_60      (60U)
#define LED_DUTY_ON_50      (50U)
#define LED_DUTY_ON_40      (40U)
#define LED_DUTY_ON_30      (30U)
#define LED_DUTY_ON_20      (20U)
#define LED_DUTY_ON_10      (10U)
#define LED_DUTY_OFF        (0U)

#define PROTOCOL_LED_DUTY_0   (0x00)
#define PROTOCOL_LED_DUTY_10  (0x01)
#define PROTOCOL_LED_DUTY_20  (0x02)
#define PROTOCOL_LED_DUTY_30  (0x03)
#define PROTOCOL_LED_DUTY_40  (0x04)
#define PROTOCOL_LED_DUTY_50  (0x05)
#define PROTOCOL_LED_DUTY_60  (0x06)
#define PROTOCOL_LED_DUTY_70  (0x07)
#define PROTOCOL_LED_DUTY_80  (0x08)
#define PROTOCOL_LED_DUTY_90  (0x09)
#define PROTOCOL_LED_DUTY_100 (0x0A)

typedef enum _led_id_
{
    LED_ID_SEG_1A,                      /// 14 :
    LED_ID_SEG_1B,                      /// 15 :
    LED_ID_SEG_1C,                      /// 16 :
    LED_ID_SEG_1D,                      /// 17 :
    LED_ID_SEG_1E,                      /// 18 :
    LED_ID_SEG_1F,                      /// 19 :
    LED_ID_SEG_1G,                      /// 20 :

    LED_ID_SEG_2A,                      /// 14 :
    LED_ID_SEG_2B,                      /// 15 :
    LED_ID_SEG_2C,                      /// 16 :
    LED_ID_SEG_2D,                      /// 17 :
    LED_ID_SEG_2E,                      /// 18 :
    LED_ID_SEG_2F,                      /// 19 :
    LED_ID_SEG_2G,                      /// 20 :

    LED_ID_SEG_3A,                      /// 21 :
    LED_ID_SEG_3B,                      /// 22 :
    LED_ID_SEG_3C,                      /// 23 :
    LED_ID_SEG_3D,                      /// 24 :
    LED_ID_SEG_3E,                      /// 25 :
    LED_ID_SEG_3F,                      /// 26 :
    LED_ID_SEG_3G,                      /// 27 :

    LED_ID_SEG_4A,                      /// 28 :
    LED_ID_SEG_4B,                      /// 29 :
    LED_ID_SEG_4C,                      /// 30 :
    LED_ID_SEG_4D,                      /// 31 :
    LED_ID_SEG_4E,                      /// 32 :
    LED_ID_SEG_4F,                      /// 33 :
    LED_ID_SEG_4G,                      /// 34 :

    LED_ID_SEG_5A,                      /// 28 :
    LED_ID_SEG_5B,                      /// 29 :
    LED_ID_SEG_5C,                      /// 30 :
    LED_ID_SEG_5D,                      /// 31 :
    LED_ID_SEG_5E,                      /// 32 :
    LED_ID_SEG_5F,                      /// 33 :
    LED_ID_SEG_5G,                      /// 34 :

    LED_ID_SEG_6A,                      /// 28 :
    LED_ID_SEG_6B,                      /// 29 :
    LED_ID_SEG_6C,                      /// 30 :
    LED_ID_SEG_6D,                      /// 31 :
    LED_ID_SEG_6E,                      /// 32 :
    LED_ID_SEG_6F,                      /// 33 :
    LED_ID_SEG_6G,                      /// 34 :
   
    /* 살균부 */
    LED_ID_UV_HOT_STER_TXT,
    LED_ID_UV_STER_TXT,
    LED_ID_UV_WATER_FAUCET,
    LED_ID_UV_ICE_FAUCET,
    LED_ID_UV_ICE_TRAY,
    LED_ID_UV_ICE_STORAGE_BOX,

    /* 좌측 온도설정 단계 */
    LED_ID_TEMP_STEP_1,
    LED_ID_TEMP_STEP_2,
    LED_ID_TEMP_STEP_3,
    LED_ID_TEMP_STEP_4,
    LED_ID_TEMP_STEP_5, 

    /* 얼음가득/부족 */
    LED_ID_ICE_FULL_TXT,
    LED_ID_ICE_LACK_TXT,

    /* 빠른제빙/취침모드/:/'C */
    LED_ID_FAST_ICE_ICON,
    LED_ID_SLEEP_ICON,
    LED_ID_COLON,
    LED_ID_CELCIUS,

    LED_ID_RAMEN,
    LED_ID_DRIP_COFFEE,
    LED_ID_TEA,
    LED_ID_MY1_ICON,
    LED_ID_MY2_ICON,
    LED_ID_MY3_ICON,
    
    LED_ID_SEG_DOT,
    LED_ID_FILTER_CLEAN_TXT,
    
    LED_ID_WIFI_ICON,
    LED_ID_SUNNY_ICON,
    LED_ID_CLOUD_ICON,
    LED_ID_UMBRELLA_ICON,
    
    LED_ID_AMOUNT_INFINITE,
    LED_ID_AMOUNT_STEP_4,
    LED_ID_AMOUNT_STEP_3,
    LED_ID_AMOUNT_STEP_2,
    LED_ID_AMOUNT_STEP_1,

    LED_ID_PERCENT,
    LED_ID_LITER,
    LED_ID_ML,

    LED_ID_ICE_WATER_SEL,
    LED_ID_ICE_SEL,
    LED_ID_HOT_WATER_SEL,
    LED_ID_ROOM_WATER_SEL,
    LED_ID_COLD_WATER_SEL,
    LED_ID_AMOUNT_SEL,
    LED_ID_MY_SEL,
    
    LED_ID_ICE_TYPE_NORMAL,
    LED_ID_ICE_TYPE_PIECE,
    LED_ID_ICE_LOCKER,
    LED_ID_HOT_WATER_LOCKER,

    LED_ID_COLD_WATER_COMP_WHITE,
    LED_ID_COLD_WATER_COMP_BLUE,
    
    LED_ID_ICE_OFF_SEL,
    LED_ID_ICE_SIZE_SEL,
    LED_ID_ICE_BIG,
    LED_ID_ICE_LINE,
    LED_ID_ICE_SMALL,
    LED_ID_HOT_WATER_LOCK_SEL,
    LED_ID_FAST_ICE,
    LED_ID_COLD_WATER_SEL_TEMP,
    LED_ID_COLD_WATER_SEL_TEMP_WEAK,
    LED_ID_COLD_WATER_SEL_TEMP_LINE,
    LED_ID_COLD_WATER_SEL_TEMP_STRONG,
    LED_ID_COLD_WATER_SEL_OFF_SEL,
    LED_ID_SLEEP_SEL,
    
    LED_ID_ICE_OFF_SEL_SEL,
    LED_ID_ALL_LOCK_SEL,

    LED_ID_ICE_EX_IN_WHITE_SEL,
    LED_ID_ICE_EX_IN_RED_SEL,
    LED_ID_ICE_EX_IN_BLUE_SEL,
    LED_ID_ICE_EX_IN_GREEN_SEL,

    LED_ID_ICE_EX_OUTER_WHITE_SEL,
    LED_ID_ICE_EX_OUTER_RED_SEL,
    LED_ID_ICE_EX_OUTER_BLUE_SEL,
    LED_ID_ICE_EX_OUTER_GREEN_SEL,

    LED_ID_WATER_EX_WHITE_SEL,
    LED_ID_WATER_EX_RED_SEL,
    LED_ID_WATER_EX_BLUE_SEL,
    LED_ID_WATER_EX_GREEN_SEL,

    LED_ID_WELCOME_LED_LEFT,
    LED_ID_WELCOME_LED_RIGHT,
    
    LED_ID_MAX,
} ELedId_T;

/* Front Control Function */
void OnOffLed(ELedId_T id, U8 onOff);
void OnAllLed(void);
void OffAllLed(void);

void SetLedType(ELedId_T led, U8 type);
U8 GetLedType(ELedId_T led);

void SetLedOnDuty(ELedId_T led, U8 duty);
void SetLedOffDuty(ELedId_T led, U8 duty);

U8 GetLedDuty(ELedId_T led);
U8 ConvertDuty2Protocol(ELedId_T led);

#endif  /* __LED_H__ */
