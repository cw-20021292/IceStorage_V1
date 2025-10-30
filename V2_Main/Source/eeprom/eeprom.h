#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "prj_type.h"

typedef enum
{
    /* CONFIG */
    EEP_ID_VESRION_MAJOR,     // VERSION
    EEP_ID_VESRION_MINOR,     // VERSION

    EEP_ID_CONF_MAKE_COLD,    // 냉수 사용 설정
    EEP_ID_CONF_MAKE_HOT,     // 온수 사용 설정
    EEP_ID_CONF_MAKE_ICE,     // 제빙 사용 설정
    EEP_ID_CONF_TURBO,        // 쾌속 제빙 사용 설정
    EEP_ID_CONF_POWER_SAVING,  // 절전 사용 설정

    EEP_ID_CONF_HEALTH,       // 살균 사용 설정
    EEP_ID_CONF_DECO_LED,     // DECO LED ON/OFF
    //EEP_ID_CONF_CONTY,        // 연속 추출
    EEP_ID_CONF_FIXED_AMOUNT, // 연속 추출양
    EEP_ID_CONF_USER_AMOUNT_HALF,	// 사용자 물양 사용 여부
    EEP_ID_CONF_USER_AMOUNT_ONE,    // 사용자 물양
    EEP_ID_CONF_USER_AMOUNT_TWO,	// 사용자 온도 설정
    EEP_ID_CONF_MUTE,         // SOUND MUTE
    EEP_ID_CONF_UNUSED_SAVE,  // 온수 미사용 절전 설정
    EEP_ID_HOT_ALTITUDE,      // 온수 고도 설정 

    EEP_ID_SEL_WATER,         // 물 선택 상태
    EEP_ID_DO_ICING,          // 현재 제빙 상태 ( UNUSED )
    EEP_ID_LOCK_ALL,          // 전체 잠금
    EEP_ID_LOCK_HOT,          // 온수 추출 잠금
    EEP_ID_LOCK_ICE,          // 얼음 추출 잠금
    EEP_ID_ICE_SIZE,          // 얼음 크기 설정(대/중/소)

    EEP_ID_MEMENTO_1,         // 메멘토 에러 1
    EEP_ID_MEMENTO_2,         // 메멘토 에러 2
    EEP_ID_MEMENTO_3,         // 메멘토 에러 3
    EEP_ID_MEMENTO_4,         // 메멘토 에러 4
    EEP_ID_MEMENTO_5,         // 메멘토 에러 5

    EEP_ID_SERVICE_CHECK_DAY,  // 서비스 점검 잔여일

    EEP_ID_FACTORY_SETUP,   
    EEP_ID_TIME,

    // FILTER..
    EEP_ID_FILTER_CHANGED,
    EEP_ID_FILTER_USAGE_TIME,  // 필터 사용 시간 잔여일


    // VOICE
    EEP_ID_VOICE_LEVEL,
    EEP_ID_VOICE_VOLUME,

    // FLUSHING..
    EEP_ID_FLUSH_SETUP_INIT,    
    EEP_ID_FLUSH_STATUS,        
    //EEP_ID_FLUSH_PROGRESS,      
    EEP_ID_CUSTOMER,

    EEP_ID_POWER_SAVING_TIME,

#ifdef CONFIG_WIFI_HEAT_TEMP
	EEP_ID_CONF_HEAT_TEMP_DEGREE_45,
	EEP_ID_CONF_HEAT_TEMP_DEGREE_50,
	EEP_ID_CONF_HEAT_TEMP_DEGREE_55,
	EEP_ID_CONF_HEAT_TEMP_DEGREE_60,
	EEP_ID_CONF_HEAT_TEMP_DEGREE_65,
	EEP_ID_CONF_HEAT_TEMP_DEGREE_70,
	EEP_ID_CONF_HEAT_TEMP_DEGREE_75,
	EEP_ID_CONF_HEAT_TEMP_DEGREE_80,
	EEP_ID_CONF_HEAT_TEMP_DEGREE_85,
	EEP_ID_CONF_HEAT_TEMP_DEGREE_90,
	EEP_ID_CONF_HEAT_TEMP_DEGREE_95,
	EEP_ID_CONF_HEAT_TEMP_DEGREE_100,
#endif

    EEP_ID_USER_A,             // 사용자 음용량
    EEP_ID_USER_B,             // 사용자 음용량
    EEP_ID_USER_C,             // 사용자 음용량
    EEP_ID_USER_NONE,          // 사용자 음용량

    EEP_ID_SODA_PUMP,          // SODA PUMP CALIBRATION
    EEP_ID_CONF_UV_MODE,       // UV 제어 모드
    EEP_ID_ENERGY,             // 소비 전력

    // WIFI
    EEP_ID_WIFI_POWER,
    EEP_ID_WIFI_FIRST_PARING,
    EEP_ID_WIFI_FOTA_MODULE,
    EEP_ID_WIFI_FOTA_MCU,

    /*************************************************************************/
    EEP_ID_STER_DAY_PASS,           /*살균 경과일*/
    EEP_ID_STER_ADJUST_MODE,        /*살균 시간 보정 모드*/
    EEP_ID_STER_PERIOD,             /*살균 주기 3일or 5일*/
    EEP_ID_STER_PERIOD_AUTO_CHANGE, /*살균 주기 자동 변경*/
    EEP_ID_STER_RESERVATION_HOUR,   /*살균 예약 시*/
    EEP_ID_STER_RESERVATION_MIN,    /*살균 예약 분*/
    EEP_ID_STER_ERR_WATER_OUT,      /*살균 중 에러 */
    EEP_ID_STER_DOING,              /*살균수 투입 조건*/
    /*************************************************************************/

    EEP_ID_SODA_INFO,           // 소다 사용량 기본 정보
    EEP_ID_SODA_USAGE_0,        // 소다 사용량 일일 데이터
    EEP_ID_SODA_USAGE_1,        // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_2,        // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_3,        // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_4,        // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_5,        // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_6,        // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_7,        // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_8,        // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_9,        // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_10,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_11,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_12,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_13,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_14,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_15,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_16,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_17,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_18,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_19,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_20,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_21,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_22,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_23,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_24,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_25,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_26,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_27,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_28,       // 소다 사용량 일일 데이터 
    EEP_ID_SODA_USAGE_29,       // 소다 사용량 일일 데이터 

    // NFC
    //EEP_ID_NFC_ERROR_HIS_COUNT,
    //EEP_ID_NFC_USER_HIS_COUNT,
    //EEP_ID_NFC_STER_HIS_COUNT,
    //EEP_ID_NFC_WATER_HIS_COUNT,

    EEP_ID_INIT_END,          // EEPROM INIT END

    EEP_ID_CHKSUM_HIGH,     // CHECK SUM High Byte
    EEP_ID_CHKSUM_LOW,      // CHECK SUM Low Byte

    EEP_ID_NUM
} EepromId_T;

void InitEeprom(void);

void SaveEepromId( EepromId_T mu8Id );

void EraseEeprom(void);

void ProcessEeprom(void);

U8 TestEeprom(void);

#endif /* __EEPROM_H__ */
