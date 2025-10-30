#ifndef __EOL_H__
#define __EOL_H__

#include "prj_type.h"


#define  MODEL_CODE_CPSI_8510L      0x23        // AIS 3.0 SODA
#define  MODEL_CODE_CHPI_7400N      0x29        // ICON AIS 4.0
#define  MODEL_CODE                 MODEL_CODE_CHPI_7400N

#define CONFIG_EOL_TEST_WATER       1   // 물검사 여부 (사용:1, 미사용:0) 
#define CONFIG_EOL_TEST_NITROGEN    1   // 질소검사 여부 (사용:1, 미사용:0)

#define  DEFAULT_TEST_TIME_VALUE       54000U   /* 90min, @100msec */
/* EOL STEP ID */

void InitEol(void);

#define EOL_TYPE_LOAD        0
#define EOL_TYPE_FRONT       1
void StartEol(U8 mu8Type);
U8 IsExpiredEolLimitTimer(void);
U8 GetEolStatus(void);
U8 GetEolType(void);

// Return true : start eol
// return false : do not start eol
U8 StartEolLoad(void);

#define EOL_MODE_READY          0
#define EOL_MODE_INIT           1
#define EOL_MODE_CHECK_SENSOR   2
#define EOL_MODE_CHECK_LOAD     3
#define EOL_MODE_DONE           4
#define EOL_MODE_SEALED_INIT    5
#define EOL_MODE_SEALED         6
void SetEolMode(U8 mu8Mode);
U8 GetEolMode(void);


// EOL MODE TEST 
#define EOL_TEST_MODE_0         0   // 테스트 미진행
#define EOL_TEST_MODE_1         1   // 테스트 1단계... 수동검사
#define EOL_TEST_MODE_2         2   // 테스트 2단계... 자동검사
void SetEolTestMode(U8 mu8Mode);
U8 GetEolTestMode(void);

void SetEolTestTimer(U16 mu16Time);
U16 GetEolTestTimer(void);

// EOL - DRAIN WATER 
void StartEolDrainWater(void);
void StopEolDrainWater(void);
U8 GetEolDrainWater(void);

void SetEolExpiredStepTimer(U8 mu8Val);
U16 GetEolExpiredStepTimer(void);

void ReloadEolCommErrTimer(void);
U16 GetEolCommErrTimer(void);

// EOL INPUT TEST MASK BIT
#define  MK_EOL_LAST_STEP        0x80000000UL  // 마지막 단계 
#define  MK_EOL_WIFI             0x40000000UL   // WIFI연결(FRONT)

#define  MK_EOL_KEY_SETTING      0x00000001UL
#define  MK_EOL_KEY_ICE          0x00000002UL
#define  MK_EOL_KEY_HOT          0x00000004UL
#define  MK_EOL_KEY_ROOM         0x00000008UL

#define  MK_EOL_KEY_COLD         0x00000010UL
#define  MK_EOL_KEY_AMOUNT       0x00000020UL
#define  MK_EOL_KEY_SAVE         0x00000040UL
#define  MK_EOL_KEY_HOT_LOCK     0x00000080UL

#define  MK_EOL_KEY_LOCK_ALL     0x00000100UL
#define  MK_EOL_KEY_COLD_MAKE    0x00000200UL
#define  MK_EOL_KEY_ICE_LOCK     0x00000400UL
#define  MK_EOL_KEY_ICE_MAKE     0x00000800UL

#define  MK_EOL_TANK_COVER       0x00001000UL
#define  MK_EOL_FILTER_COVER     0x00002000UL   
#define  MK_EOL_FILTER           0x00004000UL
#define  MK_EOL_FILTER_ONOFF     0x00008000UL       // 필터 핸들 ON->OFF->ON 감지

#define  MK_EOL_LEVER_WATER      0x00010000UL
#define  MK_EOL_LEVER_ICE        0x00020000UL

#define  MK_JIG_FRONT_LAST_CHP   0x00037FFFUL
#define  MK_JIG_FRONT_LAST_CP    0x00037F7FUL

void SetEolTestInputBit(U32 mu32MaskBit);
void ClearEolTestInputBit(U32 mu32MaskBit);
void SetEolTestInputVal(U32 mu32MaksBit);
U32 GetEolTestInputVal(void);

// Error
void SetEolErrorComm(U8 mu8Error);
U8 GetEolErrorComm(void);

// Check Status
enum
{
  EOL_CHK_ID_DRAIN_PUMP,         
  EOL_CHK_ID_EEPROM,             
  EOL_CHK_ID_LEAK,               

  EOL_CHK_ID_UV_ICE_DOOR,           // 3
  EOL_CHK_ID_UV_WATER_OUT,        
  EOL_CHK_ID_UV_ICE_TANK,        
  EOL_CHK_ID_UV_ICE_TRAY,        
  EOL_CHK_ID_ICE_FULL,              // 7
  EOL_CHK_ID_SAVING_PHOTO,
  EOL_CHK_ID_FLOW_METER,            // 9
  EOL_CHK_ID_FILTER_ONOFF,      

  EOL_CHK_ID_HEATER_TRIAC,            // 11
  EOL_CHK_ID_HEATER_RELAY,

  EOL_CHK_ID_ICE_TRAY_DEICING,      // 13
  EOL_CHK_ID_ICE_TRAY_ICING,     
  EOL_CHK_ID_SYSTEM_COLD,           // 15
  EOL_CHK_ID_SYSTEM_ICE,         
  EOL_CHK_ID_NUM                
};

U16 GetEolCheckStatus(U8 mu8Id );
void SetEolCheckStatus(U8 mu8Id, U16 mu16Val);

void UpdateEolTimer(void); // @1sec

#endif /* __EOL_H__ */
