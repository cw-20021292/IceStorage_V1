#ifndef __CONFIG_H__
#define __CONFIG_H__


/* TEST
 *  -. 압축기 구속시간 초기화 기능 활성화
 *  -. 더미 탈빙 취소 기능 활성화
 */
#define  CONFIG_FOTA_LIB            1  // FOTA 기능 활성화( 표준 라이브러리 xxx_f 사용)
#define  CONFIG_TEST                0  // 냉동, Skip 압축기, 더미탈빙 기능 활성화

#define  CONFIG_TEST_8585           0  // 신뢰성 내구성 테스트  
#define  CONFIG_TEST_8585_ICE       0  // 내구성 - 프로그램 - 얼음 추출 기능
#define  CONFIG_TEST_8585_FILTER    0  // 내구성 - 필터 

#define  CONFIG_TEST_HOT_WATER      0  // 신뢰성, 온수 연속 추출 테스트 프로그램 
#define  CONFIG_TEST_GAS_SWITCH     0  // 냉매 전환 밸브 이동 반복 테스트 
#define  CONFIG_TEST_LED            0  // LED LIGHTING TEST ( 주간/야간 모드 )
#define  CONFIG_TEST_TEMP_READ      0  // 온도 센서 상시 읽기 모드 ( 테스트 모드 )
#define  CONFIG_TEST_FLOW_METER     0  // FLOW METER AUTO COUNTING
#define  CONFIG_TEST_HEATER         0  // 온수 히터 OFF
#define  CONFIG_GAS_FILLING         0  // 냉매 채우기
#define  CONFIG_UV_MANUAL           0  // UV 수동 테스트
#define  CONFIG_TEST_HOT_DRAIN      0  // 온수 배수 밸브 테스트 코드
#define  CONFIG_TEST_FILTER         0  // 필터 레버 테스트 
#define  CONFIG_TANK_REED           0  // 탱크 리드 스위치  ( 1: 1개, 0: 2개 )
#define  CONFIG_TEST_OUT_CONTY      0  // 연속 추출.. 2시간 추출
#define  CONFIG_TEST_HOT_OUT        0  // 온수 추출 온도 표시
#define  CONFIG_TEST_PREHEAT        0  // 온수 예열 온도 표시


#define  CONFIG_COMP_ERROR_CODE     0  // 압축기 에러 코드 표시 50~
#define  CONFIG_COMP_ERROR_ONESHOT  0  // 압축기 에러 코드 표시 50~

// 냉수 센서 상한치/하한치
#define  CONFIG_TEST_COLD_TEMP_UP      0    // 상한치      
#define  CONFIG_TEST_COLD_TEMP_DOWN    0    // 하한치

#define  CONFIG_TEST_AMBI_TEMP_UP      0    // 상한치      
#define  CONFIG_TEST_AMBI_TEMP_DOWN    0    // 하한치

/* CHP or CP */
#define CONFIG_SYSTEM_CHP      0
#define CONFIG_SYSTEM_CP       1
#define CONFIG_SYSTEM_AUTO     2
#define CONFIG_SYSTEM_TYPE     CONFIG_SYSTEM_AUTO  // 0: CHP, 1:CP

/* FRONT JIG - WOORIM, FRONT PBA */
#define  CONFIG_JIG_FRONT_UNUSED_UV     0  // FORNT JIG UV2번 검사
#define  CONFIG_JIG_RBK_FRONT           0  // FRONT JIG PROGRAM( FOR RBK )
#define  CONFIG_JIG_GAS_SWITCH          0  // GAS SIWTCH JIG 

/* 살균 기능 설정/해제 */
#define  CONFIG_STER                0   /* 0: 해제, 1: 설정 */

/* RTC 테스트 - 1sec */
#define  CONFIG_RTC_TEST            0   /* 0: 해제, 1: 설정 */

/* 탈빙 후 얼음탱크의 얼음을 고르게 하기 위한 목적으로 피더모터 역회전 */
#define CONFIG_BACK_FEEDER                1   // 피더모터 역회전

/* 얼음 우선 모드에서 탈빙 중 냉각 기능 활성화 */
#define  CONFIG_ENABLE_DEICING_COOLING    0  /* 0: 해제, 1: 설정 */

/* 에러 감지 기능 설정/해제 */
#define  CONFIG_ERR_ALL             1   /* 0: 해제, 1: 설정 */

/* 통신 송수신 모니터링 기능 설정/해제 */
#define  DEBUG_COMM                 0  /* 0: 해제, 1:설정 */


// 수위 센서 에러 카운트 초기화를 위한 컴파일 옵션
#define TANK_RO     0
#define TANK_NT     1
#define CONFIG_TANK_TYPE  TANK_NT

/* 버전 표시 */
#define VERSION_MAJOR   17
#define VERSION_MINOR   0       

/* 국가 코드 */
#define COUNTRY_CODE_KOR    0

/* WiFi Hot Temp Config */
#define CONFIG_WIFI_HEAT_TEMP		1

/* WiFi Water Amount Config */
#define CONFIG_WIFI_WATER_AMOUNT	1

#endif /* __CONFIG_H__ */

