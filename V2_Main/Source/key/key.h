#ifndef __KEY_H__
#define __KEY_H__

#include "hal_key.h"

void InitKey(void);

/* SINGLE KEY */
#define  K_NONE             HAL_KEY_NONE

#define  K_SETTING          HAL_KEY_1
#define  K_ICE              HAL_KEY_2       // CP - ICE SIZE
#define  K_HOT              HAL_KEY_3       // CP - ICE_TURBO
#define  K_ROOM             HAL_KEY_4

#define  K_COLD             HAL_KEY_5
#define  K_AMOUNT           HAL_KEY_6
#define  K_ICE_MAKE         HAL_KEY_7       // CP - reserved
#define  K_ICE_LOCK         HAL_KEY_8

#define  K_HOT_LOCK         HAL_KEY_9      // CP - K_ICE_MAKE
#define  K_LOCK_ALL         HAL_KEY_10       
#define  K_COLD_MAKE        HAL_KEY_11
#define  K_SAVE             HAL_KEY_12

// Virtual Key - for CP MODEL
#define  K_CP_ICE_TURBO     HAL_KEY_13      
#define  K_CP_ICE_SIZE      HAL_KEY_14      
#define  K_CP_ICE_MAKE      HAL_KEY_15      

#define  K_ALL              HAL_KEY_ALL


/* FUNCTION KEY */
#define K_HOT_USER      ( K_HOT | K_SETTING )           // 사용자 온수 설정
#define K_HOT_TEST      ( K_HOT | K_SETTING | K_COLD )           // 사용자 온수 설정
#define K_DEICING       ( K_ICE_LOCK | K_SAVE )         // 강제 탈빙
#define K_CODY_KIT      ( K_HOT | K_ROOM | K_AMOUNT )
#define K_CODY_KIT_CP   ( K_CP_ICE_TURBO | K_ROOM | K_AMOUNT )
#define K_EMPTY_COLD    ( K_HOT | K_COLD )              // 냉수 탱크 비우기 - 파우셋 ( CHP )
#define K_EMPTY_COLD_CP ( K_CP_ICE_TURBO | K_COLD )     // 냉수 탱크 비우기 - 파우셋 ( CP )

#define K_EMPTY_COLD_DRAIN    ( K_HOT | K_COLD | K_ROOM )              // 냉수 탱크 비우기 - 배수 밸브 ( CHP )
#define K_EMPTY_COLD_DRAIN_CP ( K_CP_ICE_TURBO | K_COLD | K_ROOM )     // 냉수 탱크 비우기 - 배수 밸브 ( CP )

#define K_DRAIN_COLD    ( K_HOT | K_COLD | K_AMOUNT)              // 냉수 탱크 코일 얼음 녹이기 ( CHP )
#define K_DRAIN_COLD_CP ( K_CP_ICE_TURBO | K_COLD | K_AMOUNT)     // 냉수 탱크 코일 얼음 녹이기 ( CP )

#define K_TEST_ICE_TRAY     ( K_ICE | K_AMOUNT )           // 아이스 트레이 검사(CHP)
#define K_TEST_ICE_TRAY_CP  ( K_CP_ICE_SIZE | K_AMOUNT )   // 아이스 트레이 검사(CP)

//#define K_MUTE          ( K_ICE_LOCK | K_COLD )
//#define K_MANUAL_DRAIN  ( K_SETTING | K_ICE_TURBO )         // 자동배수
//#define K_MANUAL_DRAIN  ( K_SETTING | K_ICE )           // 자동배수

#define K_SETTING_CLOCK     ( K_SETTING | K_ICE )           // 시간 설정 ( CHP )
#define K_SETTING_CLOCK_CP  ( K_SETTING | K_CP_ICE_SIZE )   // 시간 설정 ( CP )
#define K_SETTING_SOUND     ( K_ROOM | K_AMOUNT )           // 음성 모드 변경
#define K_SETTING_CUSTOMER  ( K_SETTING | K_ROOM )          // 관리 모드 변경 ( 자가/방문 )
#define K_CANCEL_FLUSH      ( K_SETTING | K_ICE | K_COLD | K_AMOUNT  )          // 플러싱 취소
#define K_CANCEL_FLUSH_CP   ( K_SETTING | K_CP_ICE_SIZE | K_COLD | K_AMOUNT  )  // 플러싱 취소

/* TIME_SHORT MODE */
#define K_TEST_TS       ( K_SETTING | K_SAVE )          // TEST MODE ( TIME SHORT )
//#define K_VERSION       ( K_COLD | K_HOT )              // TEST(TIME SHORT) ONLY
#define K_MEMENTO       ( K_ICE_LOCK | K_LOCK_ALL )     // TEST(TIME SHORT) ONLY

/* WIFI */
#define K_WIFI_AP        ( K_COLD | K_ROOM )
#define K_WIFI_ONOFF     ( K_COLD | K_AMOUNT )

/* FACTORY MODE */
#define K_TEST_FRONT    ( K_SETTING | K_AMOUNT )      // TEST EOL - FRONT
#define K_TEST_EOL      ( K_HOT    | K_AMOUNT )       // TEST EOL - LOAD 
#define K_TEST_EOL_CP   ( K_CP_ICE_TURBO    | K_AMOUNT )       // TEST EOL - LOAD 
#define K_TEST_FCT      ( K_ICE_LOCK | K_COLD_MAKE )  // TEST MAIN PBA


/* DEVELOP MODE */
#define K_TEST_COMP     ( K_ICE_LOCK | K_AMOUNT  )      // TEST, 압축기 구속 시간 초기화


U32 GetKeyVal( void );
U32 GetKeyPopVal( void );

/* EvnetFlag Index */
#define  KEY_EVENT_PUSHED_SHORT     0x01
#define  KEY_EVENT_PUSHED_LONG_5S   0x02
#define  KEY_EVENT_PUSHED_LONG_3S   0x04
#define  KEY_EVENT_PUSHED_LONG_2S   0x08
#define  KEY_EVENT_PUSHED_LONG_1S   0x10
#define  KEY_EVENT_POP              0x20
#define  KEY_EVENT_ALL              0x3F
void SetKeyEventFlag(U8 mu8Event );
void ClearKeyEventFlag(U8 mu8Event );
U8 IsSetKeyEventFlag( U8 mu8Event );

void ProcessScanKey(void);

#endif /* __KEY_H__ */
