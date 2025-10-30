#include "hot_water.h"
#include "error.h"
#include "water_out.h"
#include "flush_water_out.h"
#include "eeprom.h"
#include "hot_water_out.h"
#include "heater_out.h"
#include "relay.h"



// TEMP HOT SELECT ('C)
#define TEMP_HOT_USER       100
#define	TEMP_DEGREE_95		95
#define	TEMP_DEGREE_90		90
#define TEMP_HOT_COFFEE     85     
#define	TEMP_DEGREE_80		80
#define	TEMP_DEGREE_75		75  
#define TEMP_HOT_TEA        70
#define	TEMP_DEGREE_65		65
#define	TEMP_DEGREE_60		60
#define	TEMP_DEGREE_55		55
#define	TEMP_DEGREE_50		50
#define TEMP_HOT_MILK       45

#ifdef CONFIG_WIFI_HEAT_TEMP

typedef struct _Temp_conf_info_
{
    U8 TempConfigNum;                         

    U8 TempList[SEL_HOT_MAX_NUM];                 
} TempConfInfo_T;

TempConfInfo_T TempConfInfo;

typedef struct _amount_config_
{
    TempId_T		mEid;
    EepromId_T		mEeepromId;
    U16				mUconfig;
} TempConfig_T;

TempConfig_T TempConfigList[ ] = 
{
    { SEL_HOT_MILK,			EEP_ID_CONF_HEAT_TEMP_DEGREE_45,	TRUE	},
	{ SEL_HOT_DEGREE_50,	EEP_ID_CONF_HEAT_TEMP_DEGREE_50,	FALSE	},
	{ SEL_HOT_DEGREE_55,	EEP_ID_CONF_HEAT_TEMP_DEGREE_55,	FALSE	},
	{ SEL_HOT_DEGREE_60,	EEP_ID_CONF_HEAT_TEMP_DEGREE_60,	FALSE	},
	{ SEL_HOT_DEGREE_65,	EEP_ID_CONF_HEAT_TEMP_DEGREE_65,	FALSE	},
    { SEL_HOT_TEA,			EEP_ID_CONF_HEAT_TEMP_DEGREE_70,	TRUE	},
	{ SEL_HOT_DEGREE_75,	EEP_ID_CONF_HEAT_TEMP_DEGREE_75,	FALSE	},
	{ SEL_HOT_DEGREE_80,	EEP_ID_CONF_HEAT_TEMP_DEGREE_80,	FALSE	},
    { SEL_HOT_COFFEE,		EEP_ID_CONF_HEAT_TEMP_DEGREE_85,	TRUE	},
	{ SEL_HOT_DEGREE_90,	EEP_ID_CONF_HEAT_TEMP_DEGREE_90,	FALSE	},
	{ SEL_HOT_DEGREE_95,	EEP_ID_CONF_HEAT_TEMP_DEGREE_95,	FALSE	},
    { SEL_HOT_USER,			EEP_ID_CONF_HEAT_TEMP_DEGREE_100,	TRUE	},
};
//#else
const static U16 gu16TempList[ SEL_HOT_NUM ] = 
{
    TEMP_HOT_MILK,
	TEMP_DEGREE_50,
	TEMP_DEGREE_55,
	TEMP_DEGREE_60,
	TEMP_DEGREE_65,
    TEMP_HOT_TEA,
	TEMP_DEGREE_75,
	TEMP_DEGREE_80,
    TEMP_HOT_COFFEE,
	TEMP_DEGREE_90,
	TEMP_DEGREE_95,
    TEMP_HOT_USER,
};
#endif // end of ifndef CONFIG_WIFI_WATER_AMOUNT

// 전원 RESET 후, 온수 자동 드레인 동작 전 대기 시간
//#define POWER_ON_TIME           20U  // 3sec @100ms

// 온수 자동 드레인 대기 시간..
//#define INIT_FULL_WAIT_TIME     30U  // 3sec @100ms

#define HEATER_ON_TIME          20U // 2sec @100ms

HotWater_T Hot;


void  InitHotWater(void)
{
    Hot.InitFull            = FALSE;
    Hot.Select              = SEL_HOT_USER;
    Hot.HeaterOnTime        = HEATER_ON_TIME;
    Hot.ConfUser            = TRUE;
    Hot.Cooling             = FALSE;
#if CONFIG_TEST_HOT_OUT
    Hot.Test                = TRUE;
#endif

#ifdef	CONFIG_WIFI_HEAT_TEMP

	TempConfInfo.TempList[0] = SEL_HOT_MILK;
	TempConfInfo.TempList[1] = SEL_HOT_TEA;
	TempConfInfo.TempList[2] = SEL_HOT_COFFEE;
	TempConfInfo.TempList[3] = SEL_HOT_USER;
	
	TempConfInfo.TempConfigNum = 0;
#endif
}

void  GetHotWaterData(HotWater_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Hot, sizeof( HotWater_T ));
}

void  SetHotWaterInitFull(U8 mu8Val )
{
    Hot.InitFull = mu8Val;
}

U8    GetHotWaterInitFull(void)
{
    return Hot.InitFull;
}

void  SetHotWaterCooling(U8 mu8Val )
{
    Hot.Cooling = mu8Val;
}

U8    GetHotWaterCooling(void)
{
    return Hot.Cooling;
}

void SetHotSelect(U8 mu8Sel )
{
    Hot.Select = mu8Sel;
}

U8   GetHotSelect(void)
{
    return Hot.Select;
}

void ClearHotHeaterOnTime(void)
{
    Hot.HeaterOnTime = 0;
}

#if 0
// 온수 사용자 물양 
void SetHotConfigUser(U8 mu8Conf )
{
    Hot.ConfUser = mu8Conf;
    SaveEepromId( EEP_ID_CONF_USER_HOT );
}

U8   GetHotConfigUser(void)
{
    return Hot.ConfUser;
}
#endif

#ifndef CONFIG_WIFI_HEAT_TEMP
U16  GetHotSelectTemp(U8 mu8Sel)
{
    if( mu8Sel > SEL_HOT_NUM )
    {
        mu8Sel = SEL_HOT_COFFEE;
    }

    return gu16TempList[ mu8Sel ];
}
#else
	U8 ListIndex;
U16  GetHotSelectTemp(U8 mu8Sel)
{
	
	// 메뉴 순환
    if( mu8Sel > SEL_HOT_NUM )
    {
        mu8Sel = 0; 
    }
	//ListIndex = TempConfInfo.TempList[mu8Sel];
	
    return gu16TempList[mu8Sel];
}
#endif

void  SetHotConfigTest(U8 mu8Test)
{
    Hot.Test = mu8Test;
}

U16  GetHotConfigTest(void)
{
    return Hot.Test;
}


U8 IsExpiredHeaterOnTime(void)
{
    if( Hot.HeaterOnTime == 0 )
    {
        return TRUE;
    }

    return FALSE;
}

void CheckHotOutHeaterOnTime(void)
{
    // 온수 추출 중에 압축기 RELAY가 ON 되는 것을 막는다.
    // 온수 히터 소비 전류가 높은 상태에서 
    // 압축기 RELAY ON 시점에 발생되는 inrush current가 더해지면
    // over current 되는 문제를 방지 하기 위한 동작이다.
    if( IsHotOut() == TRUE 
            || GetHeaterOutput() != 0 )
    {
        if( IsTurnOnRelay( RELAY_COMP ) == FALSE )
        {
            Hot.HeaterOnTime = HEATER_ON_TIME;
        }
    }
    
    if( Hot.HeaterOnTime != 0 )
    {
        Hot.HeaterOnTime--;
    }
}

#ifdef CONFIG_WIFI_HEAT_TEMP

void SetWifiTempConfig(U8 TempConfigId, U16 SettingStatus)
{
	U8 mu8TempMenuCount;
	U8 index;
	U8 ListIndex;
	
    if( TempConfigId >= SEL_HOT_NUM )
    {
        return;
    }
	else if( TempConfigId == 0 )
	{
		for(index = 0; index < SEL_HOT_MAX_NUM; index++)
		{
			TempConfInfo.TempList[index] = 0;
		}
		TempConfInfo.TempConfigNum = 0;
	}
	else {}

	//eeprom 저장 내용 수정
    if( TempConfigList[TempConfigId].mUconfig != SettingStatus )
    {
        SaveEepromId(TempConfigList[TempConfigId].mEeepromId);
    }
	else {}

    TempConfigList[TempConfigId].mUconfig = SettingStatus;

	// 갯수 확인
	if(SettingStatus == TRUE)
	{
		//TRUE 일 경우, 대입
		//TempConfInfo.TempList[ListIndex++] = TempConfigList[TempConfigId].mEid;
		TempConfInfo.TempList[TempConfInfo.TempConfigNum++] = TempConfigList[TempConfigId].mEid;

		if(TempConfInfo.TempList[4] != 0)
		{	
			if(TempConfInfo.TempConfigNum > SEL_HOT_MAX_NUM)
			{
				TempConfInfo.TempConfigNum = 0;
			}
			else {}
		}
		else
		{
			if(TempConfInfo.TempConfigNum > SEL_DEFAULT_HOT_NUM)
			{
				TempConfInfo.TempConfigNum = 0;
			}
			else {}
		}

	}
	else {}

	
}

U8	GetTempConfInfoId(U8 TempConfigId)
{
    return TempConfInfo.TempConfigNum;
}

U8	GetTempConfigId(U8 TempConfigId)
{
    return TempConfigList[TempConfigId].mEid;
}

U8	GetTempConfig(U8 TempConfigId)
{
    return TempConfigList[TempConfigId].mUconfig;
}

U8 GetTempConfigList(U8 ConfigNum)
{
    return TempConfInfo.TempList[ConfigNum];
}

U8 GetHotTempList(U8 ConfigNum)
{
    return gu16TempList[ConfigNum];
}
#endif // end of CONFIG_WIFI_HEAT_TEMP
