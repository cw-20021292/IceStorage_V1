#include "cold_water.h"
#include "level.h"
#include "temp.h"
#include "valve.h"
#include "error.h"
#include "power_saving.h"
#include "comp.h"
#include "gas_switch_valve.h"
#include "eeprom.h"
#include "front.h"
#include "tank_clean.h"
#include "ice_maker.h"
#include "water_out.h"
#include "filter.h"
#include "drain_water.h"
#include "flush_water_out.h"
#include "flow_meter.h"
#include "drain_pump.h"
#include "hot_water_out.h"
#include "cody_kit.h"
#include "process_make.h"
#include "ice.h"

/* TARGET OFF TEMP - PROTECTION SUB-COOL */
#define PROTECT_COLD_TEMP   0.0f

#define ICE_TURBO_MAKING_COLD_MAX_TIME	3600

/* TARGET TEMP - ���������� ��� �ð� �µ� ���� */
static const TEMP_T OnTempIceTurboColdMake = 15.0f;
static const TEMP_T OffTempIceTurboColdMake = 7.5f;

/* TARGET TEMP - Power Saving */
static const TEMP_T OnTempPowerSaving    = 10.0f;

// Starting Mode
#define MODE_INIT_STARTING      0
#define MODE_RESTARTING         1
#define TEMP_STARTING           20.0f
#define ON_DELAY_TIME           10      // 10sec @1sec

// Target Rps by Cold Temp Region...
#define REGION_COLD_A           0
#define REGION_COLD_B           1

// COLD MAKE TABLE 
#define COLD_MAKE_TABLE_A       0       // COLD TEMP UNDER 20'C
#define COLD_MAKE_TABLE_B       1       // COLD TEMP OVER 20'C
#define COLD_MAKE_TABLE_NUM     2

// INTI STARTING TEMP ( TEMP COLD WATER )
#define TEMP_INIT_STARTING      20.0f   // Cold water temp, for init starting....

// TEMP REGION ( TEMP AMBIENT )
#define REGION_AMBIENT_A        0       // Ambient temp =< 10
#define REGION_AMBIENT_B        1       // Ambient temp =< 20
#define REGION_AMBIENT_C        2       // Ambient temp =< 30
#define REGION_AMBIENT_D        3       // Ambient temp > 30
#define REGION_NUM              4

// EXTRA MAKE TIME @1sec
#define EXTRA_MAKE_TIME_0MIN    0U         // 0sec
#define EXTRA_MAKE_TIME_5MIN    300U       // 5min = 5 x 60sec 
#define EXTRA_MAKE_TIME_10MIN   600U       // 10min = 10 x 60sec   
#define EXTRA_MAKE_TIME_15MIN   900U       // 15min = 15 x 60sec
#define EXTRA_MAKE_TIME_20MIN   1200U       // 20min = 20 x 60sec


// EXTRA MAKE COLD TEMP
#define TEMP_ON                 7.5f
#define TEMP_OFF_5D8            5.8f
#define TEMP_OFF_6D0            6.0f


// FFED
#define INIT_FULL_WAIT_TIME     70U  // 7sec @100ms
#define MAX_FEED_TIME_OUT       1800U  // 3min = 3 * 60s * 10, @100ms
#define MAX_FEED_AMOUNT         AMOUNT_1000

typedef struct _cold_water_make_table_
{
    U8      RegionAmbi;

    TEMP_T  TempOn;
    TEMP_T  TempOff;

    TEMP_T  TempCompRps;        // cold temp 
    RPS_T   RpsA;               // cold temp lower region comp rps
    RPS_T   RpsB;               // cold temp upper region comp rps

    U16     ExtraMakeTime;      // @1sec
} ColdMakeTable_T;


#if 0
static const ColdMakeTable_T     ColdMakeTableList[ COLD_MAKE_TABLE_NUM ][ REGION_NUM ] = 
{
    /* COLD WATER >= TEMP_INIT_STARTING */
    {
        {   REGION_AMBIENT_A, TEMP_ON,   TEMP_OFF_6D0,   0.0f,   45,   45,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_B, TEMP_ON,   TEMP_OFF_5D8,   0.0f,   45,   45,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_C, TEMP_ON,   TEMP_OFF_5D8,   8.0f,   60,   45,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_D, TEMP_ON,   TEMP_OFF_5D8,   10.0f,  60,   45,    EXTRA_MAKE_TIME_5MIN  }
    },

    /* COLD WATER < TEMP_INIT_STARTING */
    {
        {   REGION_AMBIENT_A, TEMP_ON,   TEMP_OFF_6D0,   0.0f,   40, 40,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_B, TEMP_ON,   TEMP_OFF_5D8,   6.5f,   40, 45,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_C, TEMP_ON,   TEMP_OFF_5D8,   10.0f,  60, 45,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_D, TEMP_ON,   TEMP_OFF_5D8,   10.0f,  60, 45,    EXTRA_MAKE_TIME_5MIN  }
    }
};
#else
static const ColdMakeTable_T     ColdMakeTableList[ COLD_MAKE_TABLE_NUM ][ REGION_NUM ] = 
{
    /* COLD WATER >= TEMP_INIT_STARTING */
    {
        {   REGION_AMBIENT_A, TEMP_ON,   TEMP_OFF_6D0,   0.0f,   45,   45,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_B, TEMP_ON,   TEMP_OFF_5D8,   0.0f,   45,   45,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_C, TEMP_ON,   TEMP_OFF_5D8,   8.0f,   45,   45,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_D, TEMP_ON,   TEMP_OFF_5D8,   10.0f,  45,   45,    EXTRA_MAKE_TIME_5MIN  }
    },

    /* COLD WATER < TEMP_INIT_STARTING */
    {
        {   REGION_AMBIENT_A, TEMP_ON,   TEMP_OFF_6D0,   0.0f,   40, 40,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_B, TEMP_ON,   TEMP_OFF_5D8,   6.5f,   40, 45,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_C, TEMP_ON,   TEMP_OFF_5D8,   10.0f,  45, 45,    EXTRA_MAKE_TIME_0MIN  },
        {   REGION_AMBIENT_D, TEMP_ON,   TEMP_OFF_5D8,   10.0f,  45, 45,    EXTRA_MAKE_TIME_5MIN  }
    }
};
#endif

ColdMakeTable_T *pColdMake = NULL;
ColdWater_T Cold;



static void UpdateColdMakeData(void);

void  InitColdWater(void)
{
    Cold.InitFull       = FALSE;
    Cold.Level          = LEVEL_HIGH;
    Cold.ConfigMake     = TRUE;
    Cold.Make           = FALSE;
    Cold.Mode           = COLD_MODE_INIT;

    // Make
    Cold.StartingMode       = MODE_INIT_STARTING;
    Cold.StartingModeTime   = 0U;

    UpdateColdMakeData();
    Cold.RegionAmbi         = REGION_AMBIENT_C;         // ??�� 24??��??
    Cold.TempTargetOn       = TEMP_ON;
    Cold.TempTargetOff      = TEMP_OFF_5D8;
    Cold.TempCurrent        = GetTemp( TEMP_ID_COLD_WATER );
    Cold.TempStatus         = COLD_STATUS_GOOD;

    Cold.RegionCold         = REGION_COLD_A;
    Cold.TargetRps          = 45;

    Cold.ExtraMake          = FALSE;
    Cold.ExtraMakeTime      = EXTRA_MAKE_TIME_0MIN;

	  Cold.IsCoolingContinueTime = 0;
}

void  GetColdWaterData(ColdWater_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Cold, sizeof( ColdWater_T ));
}

void  SetColdWaterInitFull(U8 mu8Val )
{
    Cold.InitFull = mu8Val;
}

U8    GetColdWaterInitFull(void)
{
    return Cold.InitFull;
}


void  SetColdWaterLevel(U8 mu8Val )
{
    Cold.Level = mu8Val;
}

U8    GetColdWaterLevel(void)
{
    return Cold.Level;
}

void  SetColdWaterConfigMake(U8 mu8Val )
{
    if( mu8Val != TRUE && mu8Val != FALSE )
    {
        return ;
    }

    Cold.ConfigMake = mu8Val;

    SaveEepromId( EEP_ID_CONF_MAKE_COLD );
}

U8    GetColdWaterConfigMake(void)
{
    return Cold.ConfigMake;
}


void  SetColdWaterMake(U8 mu8Val )
{
    Cold.Make = mu8Val;
}

U8    GetColdWaterMake(void)
{
    return Cold.Make;
}

TEMP_T GetColdTargetOffTemp(void)
{
    return Cold.TempTargetOff;
}

TEMP_T GetColdTargetOnTemp(void)
{
    return Cold.TempTargetOn;
}


U8 GetColdWaterTempStatus(void)
{
    return Cold.TempStatus;
}

void CheckColdWaterTempStatus(void)
{
    TEMP_T  tCurrentTemp = 0.0f;

    // �ü� OFF��, �µ��� OFF
    if( GetColdWaterConfigMake() == FALSE )
    {
    //    Cold.TempStatus = COLD_STATUS_GOOD;
        return ;
    }
    
    // �µ��� ���� Region ��� 
    if( Cold.TempStatus == COLD_STATUS_GOOD )
    {
        if( GetTemp( TEMP_ID_COLD_WATER ) >= GetColdTargetOnTemp() )
        {
            Cold.TempStatus = COLD_STATUS_BAD;
        }
    }
    else
    {
        if( GetTemp( TEMP_ID_COLD_WATER ) <= GetColdTargetOffTemp() )
        {
            Cold.TempStatus = COLD_STATUS_GOOD;
        }
    }
}


void SetColdWaterMakeMode(U8 mu8Mode )
{
    Cold.Mode |= mu8Mode;
}

void ClearColdWaterMakeMode(U8 mu8Mode )
{
    Cold.Mode &= ~mu8Mode;
}

U8 IsSetColdWaterMakeMode(U8 mu8Mode )
{
    if( (Cold.Mode & mu8Mode ) == mu8Mode )
    {
        return TRUE;
    }

    return FALSE;
}

U8 GetColdWaterMakeMode(void )
{
    return Cold.Mode;
}

RPS_T   GetColdWaterMakeCompRps(void)
{
    return Cold.TargetRps;
}

void SetIsCoolingContinueTime(U16 time)
{
	Cold.IsCoolingContinueTime = time;
}

U16 GetIsCoolingContinueTime(void)
{
	return Cold.IsCoolingContinueTime;
}

void UpdateIsCoolingContinueTime(void)
{
	if( Cold.IsCoolingContinueTime < ICE_TURBO_MAKING_COLD_MAX_TIME )
	{
		  Cold.IsCoolingContinueTime++;
	}
}

/*
 * �ü� ��ũ ���� ����
 */
void  ControlColdWaterLevel(void)
{
    U8 IsFilterOpen = FALSE;


    /* FILTER COVER OPEN �˻�
     * ��, �ڵ� �˻� �߿��� Skip 
     */
    if( GetCodyKitStatus() == FALSE )
    {
        if(  IsOpenFilter( ID_FILTER) == TRUE 
                || IsOpenFilter( ID_FILTER_COVER) == TRUE )
        {
            IsFilterOpen = TRUE;
        }
    }

    /* ERROR -  LEAK */
    if( IsError( ERR_LEAK ) == TRUE 
            || IsError( ERR_LEAK_FLOW ) == TRUE 
            || IsFilterOpen == TRUE )
    {
        CloseValve( VALVE_NOS );
    }
    else
    {
        OpenValve( VALVE_NOS );
    }
}


static U8 IsValidMake(void)
{
    /* Turn Off make, dont want to make */
    if( Cold.ConfigMake == FALSE )
    {
        return FALSE ;
    }

    if( IsDoneFlushStatus() == FALSE )
    {
        return FALSE;
    }

#if 0
    /* �ü� ��ũ�� ���� �������� ���� ���� ��� */
    if( Cold.InitFull == FALSE )
    {
        return FALSE;
    }
#endif

    /* ���� �߿��� ���� */
#if 0
    if( GetSavingStatus() == TRUE )
    {
        return FALSE;
    }
#endif

    /* ����� ���� �ð� */
    if( IsExpiredCompProtectTime() == FALSE )
    {
        return FALSE;
    }

    /* ����� ��ȣ �ð� �ʰ� */
    if( IsOverCompProtectOnTime() == TRUE )
    {
        return FALSE;
    }

    /* ����� BLDC ��ȣ���� */
    if( IsCompBldcProtectMode() == TRUE )
    {
        return FALSE;
    }
    
    /* Turn Off make, Error */
    if( IsErrorType( ERR_TYPE_COLD_MAKE ) == TRUE )
    {
    	//ClearMakingIceCount();
        return FALSE;
    }

    /* ���� ��� */
    if( IsGoingOnDrainWaterFull() == TRUE )
    {
        return FALSE;
    }

    // �ü� û�� ���
#if 0
    if( GetTankCleanOffLoad() == TRUE )
    {
        return FALSE;
    }

    if( GetCodyService() == TRUE )
    {
        return FALSE;
    }
#endif

    return TRUE;  // Yes, valid
}

static TEMP_T   GetTargetOnTemp(void)
{
	// ���� ���� ������ ��� 3ȸ���� ��, �ð��� �������� ��쿡��, �ð� on �µ��� 15�� ����
	if( GetIceMake() == TRUE
		&& IsMaxMakingIceCount() == TRUE )
	{
		return OnTempIceTurboColdMake;
	}

    // ??��
    if( GetSavingStatus() == STATUS_SLEEP )
    {
        return OnTempPowerSaving;
    }

    return pColdMake->TempOn;
}

static TEMP_T   GetTargetOffTemp(void)
{
	// ���� ���� ������ ��� 3ȸ���� ��, �ð��� �������� ��쿡��, �ð� off �µ��� 7.5�� ����
	//if( IsMaxMakingIceCount() == TRUE )
	if( GetIceMake() == TRUE
	&& IsMaxMakingIceCount() == TRUE )
	{
		return OffTempIceTurboColdMake;
	}

    return pColdMake->TempOff;
}


// �ð� �߰� Ÿ�̸� decounter
static void UpdateExtraMakeTime(void)
{
    if( GetCompOnOff() == OFF )
    {
        return ;
    }

    if( GetGasSwitchStatus() != GAS_SWITCH_COLD )
    {
        return ;
    }

    if( Cold.ExtraMakeTime != 0 )
    {
        Cold.ExtraMakeTime--;
    }
}

// �߰� �ð� ���� Ȯ��
// TRUE ��ȯ�̸� �߰� �ð�
// FALSE �̸� �߰� �ð� ����.
static U8 IsExtraMake(void)
{
    // �ܱ� �µ� ���� ���� �߰� �ð� ����
    if( IsError( ERR_TEMP_AMBIENT ) == TRUE )
    {
        return FALSE;   // NO EXTRA MAKE
    }

    // �ܱ� �µ� ����ġ ���� �߰� �ð� ����
    if( pColdMake->ExtraMakeTime == EXTRA_MAKE_TIME_0MIN )
    {
        return FALSE; // NO EXTRA MAKE;
    }

    return TRUE;   // EXTRA MAKE
}


// Starting Mode ����
// �ð� ���� ����
static U8 CheckStartMake(void)
{
    U8 mu8Make = FALSE;

    // �ü� �µ� 20�� ���ǿ����� ��� �ð�
    if( Cold.TempCurrent >= TEMP_STARTING )
    {
        Cold.StartingMode = MODE_INIT_STARTING;
        mu8Make = TRUE;
    }
    else 
    {
        // �ü� �µ� 20�� ���� ���ǿ����� ���� ���� ��쿡��
        // 10�� ���� �� �ð� ���� ����.
        if( Cold.TempCurrent >= Cold.TempTargetOn )
        {
            // ??�� ���� �߿�????�� ??��??
            if( GetWaterOut() == TRUE 
                    && GetWaterOutSelect() == SEL_WATER_COLD )
            {
                Cold.StartingModeTime = ON_DELAY_TIME;  
            }

            // ��� ���� �ð��� ����Ǹ� �ð� ���� �� 
            if( Cold.StartingModeTime != 0 )
            {
                Cold.StartingModeTime--;
            }
            else
            {
                // ��⵿ ��带 ����
                Cold.StartingMode = MODE_RESTARTING;
                mu8Make = TRUE;
            }
        }
        else
        {
            Cold.StartingModeTime = ON_DELAY_TIME;
        }
    }

    return mu8Make;
}

static U8  CheckStopMake(void)
{
    U8 mu8Make = TRUE;      // ??��


    if( Cold.TempTargetOff >= Cold.TempCurrent )
    {
        if( IsMaxMakingIceCount() == TRUE )
        {
            if( Cold.TempTargetOff == OffTempIceTurboColdMake )
            {
                ClearMakingIceCount();
            }
        }
    
        Cold.ExtraMake      = TRUE;
        Cold.ExtraMakeTime  = pColdMake->ExtraMakeTime;
        if( Cold.ExtraMakeTime == 0 )
        {
            // �߰� �ð� �ð��� �����Ǿ� ���� ������ OFF
            Cold.ExtraMake  = FALSE;    
            mu8Make         = FALSE;    
        }
    }

    return mu8Make;
}


// �ܱ� �µ� ���� ����
static void UpdateColdMakeData(void)
{
    TEMP_T  mTempAmbi;


    mTempAmbi = GetTemp( TEMP_ID_AMBIENT );
    if( mTempAmbi <= 10.0f )
    {
        Cold.RegionAmbi = REGION_AMBIENT_A;
    }
    else if( mTempAmbi <= 20.0f )
    {
        Cold.RegionAmbi = REGION_AMBIENT_B;
    }
    else if( mTempAmbi <= 30.0f )
    {
        Cold.RegionAmbi = REGION_AMBIENT_C;
    }
    else 
    {
        Cold.RegionAmbi = REGION_AMBIENT_D;
    }


    pColdMake = &ColdMakeTableList[ Cold.StartingMode ][ Cold.RegionAmbi ];
}

// �ü� �µ��� ���� ��ǥ RPS�� ����
static RPS_T CheckCompRps(void)
{
    RPS_T   mTargetRps;
    TEMP_T  mTempA;             // Lower
    TEMP_T  mTempB;             // Upper


    // Set Cold Temp Region
    mTempA = pColdMake->TempCompRps;
    mTempB = pColdMake->TempCompRps + 0.5f;
    if( Cold.RegionCold == REGION_COLD_A )
    {
        if( mTempB <= Cold.TempCurrent )
        {
            Cold.RegionCold = REGION_COLD_B;
        }
    }
    else
    {
        if( Cold.TempCurrent < mTempA  )
        {
            Cold.RegionCold = REGION_COLD_A;
        }

    }

    // Set Target RPS
    if( Cold.RegionCold == REGION_COLD_A )
    {
        mTargetRps = pColdMake->RpsA;
    }
    else
    {
        mTargetRps = pColdMake->RpsB;
    }

    return mTargetRps;
}


void  MakeColdWater(void)
{
    U8 mu8Make = FALSE;


    UpdateExtraMakeTime();

    UpdateColdMakeData();

    // Get Current Make Status
    mu8Make = GetColdWaterMake();

    // Get Temperture
    Cold.TempTargetOn  = GetTargetOnTemp();
    Cold.TempTargetOff = GetTargetOffTemp();
    Cold.TempCurrent   = GetTemp( TEMP_ID_COLD_WATER );


    if( Cold.ExtraMake == TRUE )
    {
        // �߰� �ð� ���̸�, �Ϲ� �ð� ������ OFF�� ����
        mu8Make = FALSE;
    }

    if( mu8Make == TRUE )
    {
        mu8Make = CheckStopMake();
    }
    else
    {
        mu8Make = CheckStartMake();
    }

    // Extra Make Cold 
    if( Cold.ExtraMake == TRUE )
    {
        if( Cold.ExtraMakeTime != 0 )
        {
            mu8Make = TRUE;
        }
        else
        {
            Cold.ExtraMake = FALSE;
        }
    }

    // Comp rps
    if( mu8Make == TRUE )
    {
        Cold.TargetRps = CheckCompRps();
    }

	// �������� �ϰ��, 1�ð� �ð� �� �ʱ�ȭ 
	if( IsMaxMakingIceCount() == TRUE )
	{
		if( Cold.IsCoolingContinueTime >= ICE_TURBO_MAKING_COLD_MAX_TIME )
		{
		    SetIsCoolingContinueTime(0);
			mu8Make = FALSE;
	      	Cold.ExtraMakeTime = 0;
			ClearMakingIceCount();
		}
	}
  else
  {
  	SetIsCoolingContinueTime(0);
  }

    // Protection Off
    if( Cold.TempCurrent <= PROTECT_COLD_TEMP )
    {
        mu8Make = FALSE;
        Cold.ExtraMakeTime = 0;
    }

    // Config Off
    if( IsValidMake() == FALSE )
    {
        mu8Make = FALSE;
        Cold.ExtraMakeTime = 0;
    }

	if( mu8Make == FALSE )
	{
		Cold.IsCoolingContinueTime = 0;
	}

    // Set new make status
    SetColdWaterMake( mu8Make );
}
