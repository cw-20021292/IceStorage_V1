#ifndef __HAL_LED_H__
#define __HAL_LED_H__


#define CONFIG_LED_LPP  1

#define MAX_LED_BUF        12               /* 12bytes x 8bits = 96bits */


/* DEFAULT DUTY */
//#define DEFAULT_ONOFF_DUTY          90U
//#define DEFAULT_DIMMING_DUTY        20U
//#define DEFAULT_ONOFF_DUTY_DARK     60U
//#define DEFAULT_DIMMING_DUTY_DARK   10U

//  2022.05.04
//#define DEFAULT_ONOFF_DUTY          90U
//#define DEFAULT_DIMMING_DUTY        30U
//#define DEFAULT_ONOFF_DUTY_DARK     40U
//#define DEFAULT_DIMMING_DUTY_DARK   10U

//  2022.05.23
//#define DEFAULT_ONOFF_DUTY          90U
//#define DEFAULT_DIMMING_DUTY        40U
//#define DEFAULT_ONOFF_DUTY_DARK     50U
//#define DEFAULT_DIMMING_DUTY_DARK   20U

//  2022.06.30
#define DEFAULT_ONOFF_DUTY          90U
#define DEFAULT_DIMMING_DUTY        30U
#define DEFAULT_ONOFF_DUTY_DARK     50U
#define DEFAULT_DIMMING_DUTY_DARK   20U
typedef enum
{
    SEG_1_G,
    SEG_1_F,
    SEG_1_E,
    SEG_1_D,
    SEG_1_C,
    SEG_1_B,
    SEG_1_A,

    SEG_2_G,
    SEG_2_F,
    SEG_2_E,
    SEG_2_D,
    SEG_2_C,
    SEG_2_B,
    SEG_2_A,

    SEG_3_G,
    SEG_3_F,
    SEG_3_E,
    SEG_3_D,
    SEG_3_C,
    SEG_3_B,
    SEG_3_A,

    ICON_FLUSHING,      // 상단 - 필터 세척
    ICON_DRAIN,     // 유로 배수 
    SETTING,
    ICE_MAKE,
    ICE_OUT_2,
    ICE_OUT_1,
    ICON_ICE_FULL,

    ICE_LOCK,      
    ICON_ICE_LOCK,
    ICE_MODE,
    ICE_SIZE_SMALL,       // ICE SIZE - SMALL
    ICE_SIZE_BAR,         // ICE SIZE - |
    ICE_SIZE_LARGE,       // ICE SIZE - LARGE
    ICE_TURBO,


#if !CONFIG_LED_LPP
    HOT_COFFE,          // CHPI, -> RSERVED
    HOT_TEA,            // CHPI, -> RSERVED
    HOT_MILK,           // CHPI, -> RSERVED
#else
    HOT_MILK,          // CHPI, -> RSERVED
    HOT_TEA,            // CHPI, -> RSERVED
    HOT_COFFE,           // CHPI, -> RSERVED
#endif
    HOT_WATER,          // CHPI  ( CP, -> LOCK_ALL )
    ICON_HOT_LOCK,      // CHPI (CP, -> ICE_LOCK )
    HOT_LOCK,           // CHPI (CP, -> ICE_LOCK )
    CP_ICE_TURBO_2,     // CHPI  ( CP, -> LOCK_ALL )


    COLON,
    SEG_0_C,
    SEG_0_B,
    ICON_PM,
    ICON_UV_SMART_CARE_TM,
    ICON_AM,            
    ICON_FILTER,    // 필터 세척


    ICON_CLOUD,
    ICON_SUN,
    ICON_WIFI_BLE,
    ICON_WIFI,
    ICON_PERCENT,
    ICON_CELSIUS,
    ICON_SAVING,


    ICON_UV_ICE_TANK,
    ICON_UV_ICE_TRAY,
    ICON_UV_ICE,        
    ICON_UV_WATER,           
    ICON_UV_SMART_CARE,
    ICON_RAIN,
    ICON_ML,


    AMOUNT_500,        // 두컵
    AMOUNT_250,        // 한컵
    AMOUNT_120,        // 반컵
    AMOUNT,            // 버튼 - AMOUNT
    SAVING,            // CP, -> RESERVED
    WATER_OUT_1,
    WATER_OUT_2,


    AMOUNT_INFINITY,    // 연속
    COLD_TEMP_WHITE,
    COLD_TEMP_BLUE,
    COLD_WATER,
    COLD_MAKE,          // SETTING - 냉수켜짐
    ROOM_WATER,
    LOCK_ALL,           //empty...


    WELCOM_ICE,
    WELCOM_WATER,


    // Virtual
    CP_ICE_TURBO,
    CP_ICE_MAKE,
    CP_ICE_SIZE,

    //ICE_SIZE,
    //EMPTY_PORT_2,   // empty..
    //FILTER_RESET,
    //BAR_5,
    //BAR_4,
    //BAR_3,
    //BAR_2,
    //BAR_1,
    //AMOUNT_USER,    // AMOUNT - USER
    //AMOUNT_SETTING, // USER AMOUNT SETTING - 맞춤용량

    LED_ALL,

    MAX_LED_NUM

} LedId_T;



void HAL_InitLed(void);

// LED ON/OFF 
void HAL_TurnOnOffLEDAll(U8 mu8OnOff);
void HAL_TurnOnOffLED(LedId_T led, U8 mu8OnOff);
void HAL_TurnOnOffLEDEx(LedId_T led, U8 mu8OnOff);
void HAL_GetLedOnOffStatus(U8 *pBuf);


// LED DUTY 
void HAL_SetLedDuty(U8 mu8Duty);
U8 HAL_GetLedDuty(void);

// DIMMING ON/OFF
void HAL_DimmingLedAll(U8 mu8OnOff);
void HAL_DimmingLed(LedId_T led, U8 mu8OnOff);
void HAL_GetLedDimmingStatus(U8 *pBuf);

// DIMMING DUTY 
void HAL_SetLedDimmingDuty(U8 mu8Duty);
U8 HAL_GetLedDimmingDuty(void);
#endif /* __HAL_LED_H__ */
