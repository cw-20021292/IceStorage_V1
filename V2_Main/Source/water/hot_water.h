#ifndef __HOT_WATER_H__
#define __HOT_WATER_H__

#include "prj_type.h"

typedef struct _hot_water_
{
    U8 InitFull;    // TRUE : 초기 배수 완료
    U8 Select;      // 온수 추출 레벨 (MILK, TEA, COFFE)

    //U16 PowerOnWaitTime;    // 전원 RESET 후 대기 시간..
    //U16 InitFullWaitTime;   // 자동 초기 배수 동작 대기 시간

    U16 HeaterOnTime;
    U8  ConfUser;           // 사용자 온도 ( 95도 )

    U8 Cooling;                // 온수 추출 테스트 기능
    U8 Test;                // 온수 추출 테스트 기능
} HotWater_T;

// 초기화
void  InitHotWater(void);
void  GetHotWaterData(HotWater_T *pData);

// 최초 만수위( 직수에서는 초기 탱크 채우기 )
void  SetHotWaterInitFull(U8 mu8Val );
U8    GetHotWaterInitFull(void);

// COOLING 
void  SetHotWaterCooling(U8 mu8Val );
U8    GetHotWaterCooling(void);

// 온수 온도
#define SEL_HOT_MILK    	0U       // MILK (45'C)
#define SEL_HOT_DEGREE_50	1U
#define SEL_HOT_DEGREE_55	2U
#define SEL_HOT_DEGREE_60	3U
#define SEL_HOT_DEGREE_65	4U
#define SEL_HOT_TEA     	5U       // TEA (70'C)
#define SEL_HOT_DEGREE_75	6U
#define SEL_HOT_DEGREE_80	7U
#define SEL_HOT_COFFEE  	8U       // COFFEE (85'C)
#define SEL_HOT_DEGREE_90  	9U      
#define SEL_HOT_DEGREE_95  	10U		    
#define SEL_HOT_USER    	11U       // USER (100'C)
#define SEL_HOT_NUM     	12U   

#define SEL_HOT_MAX_NUM		5U
#define SEL_DEFAULT_HOT_NUM	4U

void SetHotSelect(U8 mu8Sel );
U8   GetHotSelect(void);

void ClearHotHeaterOnTime(void);

void SetHotConfigUser(U8 mu8Conf );
U8   GetHotConfigUser(void);

U16  GetHotSelectTemp(U8 mu8Sel);

void  SetHotConfigTest(U8 mu8Test);
U16  GetHotConfigTest(void);

U8 IsExpiredHeaterOnTime(void);
void CheckHotOutHeaterOnTime(void);

#ifdef CONFIG_WIFI_HEAT_TEMP
void SetWifiTempConfig(U8 TempConfigId, U16 SettingStatus);
U8	GetTempConfInfoId(U8 TempConfigId);
U8	GetTempConfigId(U8 TempConfigId);
U8	GetTempConfig(U8 TempConfigId);
U8 GetTempConfigList(U8 ConfigNum);
U8 GetHotTempList(U8 ConfigNum);

typedef enum _temp_id_
{
    HOT_WATER_ID_HOT_MILK = 0,
	HOT_WATER_ID_DEGREE_50,
	HOT_WATER_ID_DEGREE_55,
	HOT_WATER_ID_DEGREE_60,
	HOT_WATER_ID_DEGREE_65,
    HOT_WATER_ID_HOT_TEA,
	HOT_WATER_ID_DEGREE_75,
	HOT_WATER_ID_DEGREE_80,
    HOT_WATER_ID_HOT_COFFEE,
	HOT_WATER_ID_DEGREE_90,
	HOT_WATER_ID_DEGREE_95,
    HOT_WATER_ID_HOT_USER,
} TempId_T;

typedef enum _temp_conf_num_
{
    HOT_WATER_CONF_NUM_FIRST,
    HOT_WATER_CONF_NUM_SECOND,
    HOT_WATER_CONF_NUM_THIRD,
    HOT_WATER_CONF_NUM_FOURTH,
    HOT_WATER_CONF_NUM_FIFTH,
} TempConfNum_T;

#endif

#endif /* __HOT_WATER_H__ */
