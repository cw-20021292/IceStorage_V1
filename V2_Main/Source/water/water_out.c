#include "water_out.h"
#include "sound.h"
#include "eeprom.h"
#include "tank_clean.h"
#include "key_event_lib.h"
#include "error.h"
#include "valve.h"

#include "room_water_out.h"
#include "cold_water.h"
#include "cold_water_out.h"
#include "hot_water.h"
#include "hot_water_out.h"
#include "flush_water_out.h"
#include "cody_water_out.h"
#include "flow_meter.h"
#include "water_out_common.h"
#include "process_display.h"
#include "sound.h"

#include "water_out_type.h"
// USER AMOUNT 
#define ONE_USER_AMOUNT         AMOUNT_10
#define DEFAULT_USER_AMOUNT_LITER   550
#define DEFAULT_USER_AMOUNT         ( (DEFAULT_USER_AMOUNT_LITER / ML_UNIT) * AMOUNT_10 )   //AMOUNT_300
#define MAX_CONF_USER_AMOUNT    ( (MAX_USER_AMOUNT_LITER / ML_UNIT) * AMOUNT_10 ) //AMOUNT_1000 

#define  RETURN_TIME_AFTER_WATER_OUT      500 // 5sec @10ms
#define  DEFAULT_RETURN_TIME              700 // 7sec @10ms
#define  DEFAULT_CONTINUE_RETURN_TIME     0   // 0sec @10ms
#define  RETURN_TIME_AMOUNT               20  // 200ms @10ms
#define  OUT_PROTECT_TIME                 50  // 500ms @10ms

#define  LIMIT_TIME  (U32)-1
typedef struct _water_out_
{
    /* Select */
    U8   Select;
    U8   LastedSelect;
    U8   PrevSelect;
    U16  ReturnTime;
    U16  ReturnTimeAmount;
    //U16  ReturnTimeUser;

    /* Water Out Mode */
    U8   WaterOut;          // TRUE or FALSE
    U8   ConfAmount;        // FIXED_AMOUINT_HALF ~ TWO
    U8   ProtectTime;       // 재추출 금지 시간
#if 0
    /* User Mode */
    U8  ConfUser;           // TRUE or FALSE
    U32  ConfUserAmount;    // USER AMOUNT
#endif    
    /* Continue - unused for cpsi-8510 */
    U8   Continue;               // 연속 추출 (TRUE or FALSE)
    U16  ContinueReturnTime;     // 연속 선택 해제 시간 

    /* Fixed Amouted */
    U8   LongKeyOut;             // 연속 추출 설정 Long key 
    U8   LongKeyBackUp;          // 연속 추출 설정전 FixedAmountType의 이전 상태값
    U8   InfinityAmout;          // 연속 추출 설정/해제 ( EEPROM )
    U8   FixedAmountType;        //  FIXED_AMOUNT_xxxxx

    U32  InfinityTimeOut;        // 연속 추출 종료 시간(3분)
    U32  ConfInfinityTimeOut;        // 연속 추출 종료 시간(3분)

    /* Water Out Time */
    U32  OutWaterTime[MAX_TIME_ID][MAX_WATER_ID];  // 마지막 추출 시간 @10msec
#ifdef CONFIG_WIFI_WATER_AMOUNT

    /* Wifi Mode */
    U8  WiFiConfUserHalf;           // TRUE or FALSE
    U8  WiFiConfUserOne;           // TRUE or FALSE
    U8  WiFiConfUserTwo;           // TRUE or FALSE

	/* Water Amount setting */
	U16 WaterAmountHalf;
	U16 WaterAmountOne;
	U16 WaterAmountTwo;	
	
	U8 WaterAmountInitConf;
#if 0
	/* Water Amount Conv*/
	U32 ConvWaterAmountHalf;
	U32 ConvWaterAmountOne;
	U32 ConvWaterAmountTwo;	
#endif
	
#endif
} WaterOutStatus_T;

WaterOutStatus_T Out;

#define SEL_WATER_CONF_NUM	3
typedef struct _water_amount_conf_info_
{
    U8 WaterAmountConfigNum;                         

    U8 WaterAmountList[SEL_WATER_CONF_NUM];                 
} WAterAmountConfInfo_T;

WAterAmountConfInfo_T WAterAmountConfInfo;

// 상황에 따라 내부 변수로 들어갈 수 있음.
WaterOutType_T mAmount_half_type;
WaterOutType_T mAmount_one_type;
WaterOutType_T mAmount_two_type;


void  InitWaterOut(void)
{
    U8 i;

    Out.Select          = SEL_WATER_ROOM;
    Out.LastedSelect    = SEL_WATER_ROOM;
    Out.PrevSelect      = SEL_WATER_ROOM;
    Out.ReturnTime      = 0;
    Out.ReturnTimeAmount = 0;
    //Out.ReturnTimeUser = 0;

    Out.WaterOut        = FALSE;
    Out.ConfAmount      = FIXED_AMOUNT_HALF;
#if 0
    Out.ConfUser        = FALSE;
    Out.ConfUserAmount  = DEFAULT_USER_AMOUNT;
#endif
    Out.Continue            = FALSE;
    Out.ContinueReturnTime  = 0;

    Out.LongKeyOut           = FALSE;
    Out.LongKeyBackUp        = FIXED_AMOUNT_HALF;
    Out.InfinityAmout        = FALSE;
    Out.FixedAmountType      = FIXED_AMOUNT_HALF;

#if CONFIG_TEST_OUT_CONTY
    Out.InfinityTimeOut      = INFINITY_120MIN_TIMEOUT;
    Out.ConfInfinityTimeOut  = INFINITY_120MIN_TIMEOUT;
#else
    Out.InfinityTimeOut      = INFINITY_3MIN_TIMEOUT;
    Out.ConfInfinityTimeOut  = INFINITY_3MIN_TIMEOUT;
#endif
    Out.ProtectTime          = OUT_PROTECT_TIME;

    for( i = 0; i < MAX_WATER_ID ; i++ )
    {
        Out.OutWaterTime[LASTED_TIME_ID][ i ]   = 0UL;
        Out.OutWaterTime[DAILY_TIME_ID][ i ]    = 0UL;
        Out.OutWaterTime[TOTAL_TIME_ID][ i ]    = 0UL;
    }
    // Init Out modules...
    InitFlushOut();
	
	/* sean [24-03-29] wifi 설정하면서 필요한 변수 전부 초기화 */
	Out.WaterAmountInitConf = FALSE;

	Out.WiFiConfUserHalf = 0;
	Out.WiFiConfUserOne = 0;
	Out.WiFiConfUserTwo = 0;

	Out.WaterAmountHalf = 120;
	Out.WaterAmountOne = 250;
	Out.WaterAmountTwo = 500;
	
}



void  SetWaterOutSelect(U8 mu8Id )
{
    if( mu8Id >= MAX_WATER_ID )
    {
        return ;
    }

    if( Out.Select != mu8Id )
    {
        Out.PrevSelect = Out.Select;
    }

    Out.ReturnTime = DEFAULT_RETURN_TIME; // 7sec@100ms
    Out.Select     = mu8Id;

    SaveEepromId( EEP_ID_SEL_WATER );
}

U8    GetWaterOutSelect(void)
{
    return Out.Select;
}

U8    GetWaterOutLastedSelect(void)
{
    return Out.LastedSelect;
}

void UpdateWaterOutReturnTime(void)
{
    Out.ReturnTime = DEFAULT_RETURN_TIME;
}

void ResetWaterOutReturnTime(void)
{
    Out.ReturnTime = 0;
    Out.ReturnTimeAmount = RETURN_TIME_AMOUNT;
}

#if 0
void UpdateWaterOutReturnTimeUser(void)
{
    Out.ReturnTimeUser = DEFAULT_RETURN_TIME;
}
#endif

U16 GetWaterOutReturnTime(void)
{
    return Out.ReturnTime;
}

U16 GetWaterOutReturnTimeAmount(void)
{
    return Out.ReturnTimeAmount;
}

#if 0
U16 GetWaterOutReturnTimeUser(void)
{
    return Out.ReturnTimeUser;
}
#endif

static U8 SoundFlushinStart(void)
{
    U8 mu8Sound;

    // 플러싱 종료 안내
    if( GetFlushSetupInit() == FALSE )
    {
        mu8Sound = SOUND_SETUP_FLUSHING;
    }
    else
    {
        mu8Sound = SOUND_FILTER_FLUSHING;
    }

    return mu8Sound;
}

U8 StartWaterOutFlush(U8 mu8Sound)
{
    // IF IS NOT DONE FLUSHING.... DO FLUSHING..
    if( IsDoneFlushStatus() == FALSE )
    {
        SetWaterOutSelect( SEL_WATER_FLUSH );
        mu8Sound = SoundFlushinStart();
    }
    else if( GetFlushPowerOn() == FLUSH_STATUS_INIT )
    {
        SetWaterOutSelect( SEL_WATER_FLUSH );
    }

    StartWaterOut();
    return mu8Sound;
}

/* 추출 시작 - 전원 리셋 */
void StartWaterOutFlushPowerOn(void)
{
    if( IsDoneFlushStatus() == TRUE )
    {
        if( GetFlushPowerOn() == FLUSH_STATUS_INIT )
        {
            SetWaterOutSelect( SEL_WATER_FLUSH );
            StartWaterOut();
        }
    }
}

/* 추출 시작 */
void  StartWaterOut(void)
{
    if( Out.WaterOut == FALSE )
    {
        Out.OutWaterTime[LASTED_TIME_ID][Out.Select] = 0UL;
    }
    Out.WaterOut = TRUE;
}

/* 추출 시작 - 프러싱 동작 포함 */
/* 추출 중지 */
void  StopWaterOut(void)
{
    Out.WaterOut = FALSE;

#if 0
    SetTankClean( FALSE );
#endif
}

/* 추출 상태 반환
 * Return
 *  -TRUE : 추출 중
 *  -FALSE : 추출 중지 상태
 * */
U8 GetWaterOut(void)
{
    return Out.WaterOut;
}

/* 연속 추출 설정/해제 */
void  SetWaterOutContinue(U8 mu8Mode )
{
    if( mu8Mode != TRUE && mu8Mode != FALSE )
    {
        return ;
    }

    Out.Continue = mu8Mode;
    if( mu8Mode == TRUE )
    {
        Out.ContinueReturnTime = DEFAULT_CONTINUE_RETURN_TIME;
    }
}

U8    GetWaterOutContinue(void)
{
    return Out.Continue;
}

U8 IsExpiredOutProtectTime(void)
{
    if( Out.ProtectTime != 0 )
    {
        return FALSE;   // 보호 시간 동작 중
    }

    return TRUE;    // 보호 시간 끝
}

void SetWaterOutInfinityLongKey(void)
{
    Out.LongKeyOut = TRUE;

    Out.LongKeyBackUp = GetWaterOutFixedAmoutType();
    SetWaterOutFixedAmountType( FIXED_AMOUNT_INFINITY );
}


void ClearWaterOutInfinityLongKey(void)
{
    if( Out.WaterOut == FALSE && Out.LongKeyOut == TRUE )
    {
        Out.LongKeyOut = FALSE;

        SetWaterOutFixedAmountType( Out.LongKeyBackUp );
    }
}

// 추출량 설정
void  SetWaterOutFixedAmountType(U8 mType )
{
    if( mType >= OUT_AMOUNT_NUM )
    {
        return ;
    }

    if( mType == FIXED_AMOUNT_INFINITY )
    {
        Out.InfinityAmout = TRUE;
    }
    else
    {
        Out.InfinityAmout  = FALSE;
        Out.FixedAmountType  = mType;
    }

    Out.ReturnTime = DEFAULT_RETURN_TIME; // 7sec@100ms
}

U8    GetWaterOutFixedAmoutType(void)
{
    return Out.FixedAmountType;
}


U8 GetWaterOutAmoutType(void)
{
    if( Out.InfinityAmout == TRUE )
    {
        return FIXED_AMOUNT_INFINITY;
    }

    return GetWaterOutFixedAmoutType();
}

void ClearWaterOutInfinityAmout(void)
{
    Out.InfinityAmout = FALSE;
}


U8 GetWaterOutConfFixedAmountType(void)
{
    return Out.ConfAmount;
}

void SetWaterOutConfFixedAmountType(U8 mu8Type)
{
    Out.ConfAmount = mu8Type;

    SaveEepromId( EEP_ID_CONF_FIXED_AMOUNT );
}

void SetWaterOutConfInfinityTimeOut(U32 mu32TimeOut)
{
    Out.ConfInfinityTimeOut = mu32TimeOut;
}

#if 0
void SetWaterOutConfigUser(U8 mu8Config)
{
    Out.ConfUser = mu8Config;

    SaveEepromId( EEP_ID_CONF_USER );
}

U8 GetWaterOutConfigUser(void)
{
    return Out.ConfUser;
}
#endif

#if 0
U16 GetWaterOutUserAmountLiter(void)
{
    return CalcLiter( Out.ConfUserAmount );
}

void SetWaterOutUserAmountLiter(U16 mu16Liter)
{
    U32 mu32Val;

    mu32Val = CalcAmount( mu16Liter );

    SetWaterOutUserAmount( mu32Val );
}
#endif

#if 0
U32 GetWaterOutUserAmount(void)
{
    return Out.ConfUserAmount;
}

void SetWaterOutUserAmount(U32 mu32UserAmount)
{
    Out.ConfUserAmount = mu32UserAmount;
    SaveEepromId( EEP_ID_CONF_USER_AMOUNT );
}

void IncrementWaterOutAmount(void)
{
    Out.ConfUserAmount += ONE_USER_AMOUNT; 
    if( Out.ConfUserAmount > MAX_CONF_USER_AMOUNT )
    {
        Out.ConfUserAmount = DEFAULT_USER_AMOUNT;
    }

    SaveEepromId( EEP_ID_CONF_USER_AMOUNT );
}
void IncrementWaterOutAmountLimit(void)
{
    if( Out.ConfUserAmount < MAX_CONF_USER_AMOUNT )
    {
        Out.ConfUserAmount += ONE_USER_AMOUNT; 
    }

    SaveEepromId( EEP_ID_CONF_USER_AMOUNT );
}
#endif
// 추출 시간 설정
// mu8Type : TIME_ID
// mu8Id : WATER_ID
// mu32Time : 시간.
void  SetWaterOutTime(U8 mu8Type, U8 mu8Id, U32 mu32Time )
{
    Out.OutWaterTime[mu8Type][mu8Id] = mu32Time;
}

// 추출 시간 반환
// mu8Type : TIME_ID
// mu8Id : WATER_ID
U32  GetWaterOutTime(U8 mu8Type, U8 mu8Id )
{
    return Out.OutWaterTime[mu8Type][mu8Id];
}


// 추출 시간 업데이트
static void UpdateOutTime(U8 mu8Sel)
{
    U8 i;

    for( i = 0; i < MAX_TIME_ID; i++ )
    {
        if( Out.OutWaterTime[i][mu8Sel] < LIMIT_TIME )
        {
            Out.OutWaterTime[i][mu8Sel]++;
        }
    }
}


#if 0
static U8 ConvWaterOutTime2Cup(U32 mu32Time, U32 mu32CupTime)
{
    U32 mu32CupSize;


    if( mu32Time < (mu32CupTime/3) )
    {
        mu32CupSize = 0;    // 0-cup under 1/3
    }
    else if( mu32Time <= mu32CupTime )
    {
        mu32CupSize = 1;
        return 1;    // 1-cup over 1/3
    }
    else
    {
        // Round Up
        mu32Time += (mu32CupTime / 2);    
        mu32CupSize = mu32Time / mu32CupTime;
    }

    return (U8)mu32CupSize;
}
#endif


// 추출 시간을 컵 사이즈로 반환
#if 0
U8 GetWaterOutCupNum(U8 mu8Type, U8 mu8Id)
{
    U8 mu8Num;
    U32 mu32Time;
    U32 mu32CupTime;

    mu32Time = Out.OutWaterTime[mu8Type][mu8Id];
    mu32CupTime = gu32TimePerCup[ mu8Id ];

    mu8Num = ConvWaterOutTime2Cup( mu32Time, mu32CupTime );
    return mu8Num;
}
#endif

static void  ControlWaterOutSelect(void)
{
#if 0
    MenuId_T mMenuId;

    /* 1. 온수 선택 후  7sec 후에 다시 냉수 또는 정수로 물 선택을 복귀 시킨다.
     * 2. 단, 냉수 사용이 해제되어 있는 경우에는 정수를 선택한다.
     * 3. 냉수 선택 시 냉수 사용이 해제되어 있는 경우에는 정수로 물 선택을 복귀 시킨다.
     *
     * 물이 추출 중인 경우에는, 물 추출 이후에 5초 타이머가 적용될 수 있도록
     * 타이머를 reset 한다.
     */

    mMenuId = GetCurrentMenu();
    
    if( mMenuId != MENU_MAIN 
            && mMenuId != MENU_WATER_OUT
            && mMenuId != MENU_STANDBY)
    {
        Out.ReturnTime  = DEFAULT_RETURN_TIME;    
    }
#endif

#if CONFIG_TEST_HOT_WATER
    if( Out.Select == SEL_WATER_COLD )
#else
    if( Out.Select == SEL_WATER_HOT
            || Out.Select == SEL_WATER_COLD )
#endif
    {
        if( Out.WaterOut == TRUE )
        {
            Out.ReturnTime  = DEFAULT_RETURN_TIME;    

#if 0   
            // 2020.01.20, 7초로 통일
            /* Make Cold Off, Select Cold */
            if( Out.Select == SEL_WATER_COLD 
                    && GetColdWaterConfigMake() == FALSE )
            {
                Out.ReturnTime = 1;
            }
#endif
        }

        if( Out.ReturnTime != 0 )
        {
            Out.ReturnTimeAmount = RETURN_TIME_AMOUNT;
        }
        else
        {
            // 이전 선택 상태로 이동
#if 0
            if( Out.Select == SEL_WATER_SODA )
            {
                Out.Select = Out.PrevSelect;
                SetSodaSelect( SEL_SODA_HIGH );
            }
#else
            if( Out.Select == SEL_WATER_HOT )
            {
                if( GetHotConfigTest() == FALSE )
                {
                    Out.Select = Out.PrevSelect;
                }
                //SetSodaSelect( SEL_SODA_HIGH );
            }
#endif
            // 정수 선택 상태로 이동
            if( Out.Select == SEL_WATER_COLD 
                    && ( GetColdWaterConfigMake() == FALSE 
                        || IsErrorType( ERR_TYPE_COLD_MAKE )) 
              )
            {
                Out.Select = SEL_WATER_ROOM;
            }
        }
    }
    else
    {
        // ROOM OUT
        if( Out.WaterOut == TRUE )
        {
            Out.ReturnTime = DEFAULT_RETURN_TIME;
        }

        if( Out.ReturnTime != 0 )
        {
            Out.ReturnTimeAmount = RETURN_TIME_AMOUNT;
        }

    }
	if( (Out.WiFiConfUserHalf == TRUE) && (Out.WaterAmountInitConf == FALSE) )
	{
		Out.LongKeyBackUp	= UNFIXED_AMOUNT_HALF;
		Out.ConfAmount		= UNFIXED_AMOUNT_HALF;
		Out.FixedAmountType = UNFIXED_AMOUNT_HALF;
		Out.WaterAmountInitConf = TRUE;
	}

    if( Out.ReturnTime == 0 && Out.ReturnTimeAmount == 0 )
    {
        // 정량 추출양 재설정
        Out.InfinityAmout = FALSE;
        Out.FixedAmountType = Out.ConfAmount;
    }

    if( Out.ReturnTimeAmount != 0 )
    {
        Out.ReturnTimeAmount--;
    }
    
    if( Out.ReturnTime != 0 )
    {
        Out.ReturnTime--;
    }
}


// 연속 선택 표시부 제어 
static void ControlContinueSelect(void)
{
    // 추출 중에는 연속 추출 선택 버튼 해제 시간을 유지
    if( Out.WaterOut == TRUE && Out.Continue == TRUE )
    {
        Out.ContinueReturnTime = DEFAULT_CONTINUE_RETURN_TIME;
    }

    // 추출 중이 아닐 때에는 연속 선택 버튼을 일정 시간 이후 해제 한다.
    if( Out.Continue == TRUE && Out.ContinueReturnTime == 0 )
    {
        SetWaterOutContinue( FALSE );
        ClearWaterOutInfinityAmout();
    }
    else
    {
        if( Out.ContinueReturnTime != 0 )
        {
            Out.ContinueReturnTime--;
        }
    }

    // 청소 모드
#if 0
    if( GetTankClean() == TRUE 
            || GetCodyService() == TRUE )
    {
        SetWaterOutFixedAmountType( FIXED_AMOUNT_TANK_CLEAN );
    }
#endif
}



// 추출 중지 조건 검사
// Precondition 
//  -. 탱크 비움 기능이 아닐 때,
//  -. 물 추출 중일 떄,
static void CheckWaterOutStopCondition(void)
{
    // 물 추출 중일 때에만 동작한다.
    if( Out.WaterOut == FALSE )
    {
        Out.InfinityTimeOut = Out.ConfInfinityTimeOut;
        return ;    
    }

#if 0
    // 탱크 비움 기능이 아닐 때에만 동작한다.
    if( GetTankClean() == TRUE )
    {
        return ;
    }

    if( GetCodyService() == TRUE )
    {
        return ;
    }
#endif

    // 연속 추출 시간 초과
    if( GetWaterOutAmoutType() == FIXED_AMOUNT_INFINITY )
    {
        if( Out.InfinityTimeOut != 0 )
        {
#if !CONFIG_TEST_8585_FILTER
            Out.InfinityTimeOut--;
#endif
        }
        else
        {
            StopWaterOut();
            StartDispTimerId( DISP_TIMER_ERROR );
            Sound( SOUND_EFFLUENT_END );
        }
    }

#if 0
    // 추출 중, 에러 발생시 추출 중지..
    if( ( GetWaterOutSelect() == SEL_WATER_ROOM 
                &&  IsErrorType( ERR_TYPE_ROOM_WATER_OUT ) == TRUE )
            || ( GetWaterOutSelect() == SEL_WATER_COLD 
                &&  IsErrorType( ERR_TYPE_COLD_WATER_OUT ) == TRUE )
            || ( GetWaterOutSelect() == SEL_WATER_HOT 
                &&  IsErrorType( ERR_TYPE_HOT_WATER_OUT ) == TRUE )
      )
    {
        StopWaterOut();
        Sound( SOUND_EFFLUENT_END );
    }
#endif
}

#if 0
// 탱크 비움 기능에서 바닥 잔수 제거 시간 
//#define RESIDUAL_TIME     1300UL // 13sec @10ms
//#define RESIDUAL_TIME     2500UL // 25sec @10ms
#define RESIDUAL_TIME       1800UL // 18sec @10ms   
static void ControlTankClean(void)
{
    static U32 mu32ResidualTime;

    if( GetTankClean() != TRUE )
    {
        mu32ResidualTime = RESIDUAL_TIME;
        return ;
    }

    if( GetColdWaterLevel() == LEVEL_LOW )
    {
        if( mu32ResidualTime != 0 )
        {
            mu32ResidualTime--;
        }
        else
        {
            StopWaterOut();
            Sound( SOUND_EFFLUENT_END );
        }
    }
}
#endif

static WaterOutType_T ConvOutType(U8 mu8Type)
{
#if 1
    WaterOutType_T mAmount;

    if( mu8Type == FIXED_AMOUNT_ONE )
    {
        mAmount = OUT_AMOUNT_250;
    }
    else if( mu8Type == FIXED_AMOUNT_TWO )
    {
        mAmount =  OUT_AMOUNT_500;
    }
    else if( mu8Type == FIXED_AMOUNT_FOUR )
    {
        mAmount =  OUT_AMOUNT_1000;
    }
	
	else if( mu8Type == UNFIXED_AMOUNT_HALF )
    {
        mAmount =  OUT_AMOUNT_HALF_WIFI;
    }
	
	else if( mu8Type == UNFIXED_AMOUNT_ONE )
    {
        mAmount =  OUT_AMOUNT_ONE_WIFI;
    }
	
	else if( mu8Type == UNFIXED_AMOUNT_TWO )
    {
        mAmount =  OUT_AMOUNT_TWO_WIFI;
    }
	
    else if( mu8Type == FIXED_AMOUNT_INFINITY )
    {
        mAmount =  OUT_AMOUNT_INFINITY;
    }
/*
    else if( mu8Type == FIXED_AMOUNT_USER )
    {
        mAmount =  OUT_AMOUNT_USER;
    }
*/
	else if( mu8Type == FIXED_AMOUNT_TANK_CLEAN )
    {
        mAmount =  OUT_AMOUNT_CLEAN;
    }
    else
    {
        // FIXED AMOUNT_HALF
        mAmount =  OUT_AMOUNT_120;
    }
#else 
    WaterOutType_T mAmount;

	Out.WaterAmountHalf = GetWaterAmountConfig(FIXED_AMOUNT_HALF);
	Out.WaterAmountOne = GetWaterAmountConfig(FIXED_AMOUNT_ONE);
	Out.WaterAmountTwo = GetWaterAmountConfig(FIXED_AMOUNT_TWO);
	
    if( mu8Type == FIXED_AMOUNT_ONE )
    {
        mAmount = Out.WaterAmountOne;
    }
    else if( mu8Type == FIXED_AMOUNT_TWO )
    {
        mAmount =  Out.WaterAmountTwo;
    }
    else if( mu8Type == FIXED_AMOUNT_FOUR )
    {
        mAmount =  OUT_AMOUNT_1000;
    }
    else if( mu8Type == FIXED_AMOUNT_INFINITY )
    {
        mAmount =  OUT_AMOUNT_INFINITY;
    }
/*
    else if( mu8Type == FIXED_AMOUNT_USER )
    {
        mAmount =  OUT_AMOUNT_USER;
    }
*/
    else if( mu8Type == FIXED_AMOUNT_TANK_CLEAN )
    {
        mAmount =  OUT_AMOUNT_CLEAN;
    }
    else
    {
        // FIXED AMOUNT_HALF
        mAmount = Out.WaterAmountHalf;
    }
	
#endif

    return mAmount;
}


static void  ControlWaterOutCmd(void)
{
    U8 mu8Select;
    U8 mu8Amount;
    WaterOutType_T  mOutType;


    /* TURN OFF ALL WATER OUT */
    if( Out.WaterOut == FALSE )
    {
        TurnOffRoomOut();
        TurnOffColdOut();
        TurnOffHotOut();
        TurnOffFlushOut();
        TurnOffCodyOut();
        //TurnOffSodaOut();
        return ;
    }

    mu8Select = Out.Select;
    mu8Amount = GetWaterOutAmoutType();
    mOutType = ConvOutType( mu8Amount );

    /* Update Time information */
    UpdateOutTime( mu8Select );


    /* TURN ON WATER OUT - COLD */
    if( mu8Select == SEL_WATER_COLD )
    {
        TurnOnColdOut(mOutType);
    }
    /* TURN ON WATER OUT - HOT */
    else if( mu8Select == SEL_WATER_HOT )
    {
        TurnOnHotOut(mOutType);
    }
    else if( mu8Select == SEL_WATER_FLUSH )
    {
        TurnOnFlushOut(mOutType);
    }
    else if( mu8Select == SEL_WATER_CODY )
    {
        TurnOnCodyOut(mOutType);
    }
#if 0
    /* TURN ON WATER OUT - SODA */
    else if( mu8Select == SEL_WATER_SODA )
    {
        TurnOnSodaOut(mOutType);
    }
#endif
    /* TURN ON WATER OUT - ROOM */
    else
    {
        TurnOnRoomOut(mOutType);
    }
    
    /* Updated Lasted Selected Out */
    Out.LastedSelect = mu8Select;
}

U8  GetWaterOutPercent(void)
{
    U8 mu8Percent = 0;
    U8 mu8Sel = 0;


    mu8Sel = GetWaterOutSelect();
    if( mu8Sel == SEL_WATER_ROOM )
    {
        mu8Percent = GetRoomOutPercent();
    }
    else if( mu8Sel == SEL_WATER_COLD )
    {
        mu8Percent = GetColdOutPercent();
    }
    else if( mu8Sel == SEL_WATER_HOT )
    {
        mu8Percent = GetHotOutPercent();
    }
    else if( mu8Sel == SEL_WATER_FLUSH )
    {
        mu8Percent = GetFlushOutPercent();
    }
    else if( mu8Sel == SEL_WATER_FLUSH )
    {
        mu8Percent = GetCodyOutPercent();
    }
#if 0
    else    // if( SEL_WATER_SODA )
    {
        // 소다 준비 중 표시..
        if( IsSetSodaOutMode( SODA_OUT_MIXING ) == TRUE 
                || IsSetSodaOutMode( SODA_OUT_MIXING_REPEAT ) == TRUE )
        {
            mu8Percent = 102; // text : 준비 중
        }
        else
        {
            mu8Percent = GetSodaOutPercent();
        }
    }
#endif

    mu8Percent += 5;
    mu8Percent /= 10;
    mu8Percent *= 10;

    return mu8Percent;
}

U16  GetWaterOutAmount(void)
{
    U16 mu16Amount = 0;
    U8 mu8Sel = 0;


    mu8Sel = GetWaterOutSelect();
    if( mu8Sel == SEL_WATER_ROOM )
    {
        mu16Amount = GetRoomOutAmount();
    }
    else if( mu8Sel == SEL_WATER_COLD )
    {
        mu16Amount = GetColdOutAmount();
    }
    else if( mu8Sel == SEL_WATER_HOT )
    {
        mu16Amount = GetHotOutAmount();
    }
    else if( mu8Sel == SEL_WATER_FLUSH )
    {
        mu16Amount = GetFlushOutAmount();
    }
    else if( mu8Sel == SEL_WATER_FLUSH )
    {
        mu16Amount = GetCodyOutAmount();
    }
#if 0
    else    // if( SEL_WATER_SODA )
    {
        // 소다 준비 중 표시..
        if( IsSetSodaOutMode( SODA_OUT_MIXING ) == TRUE 
                || IsSetSodaOutMode( SODA_OUT_MIXING_REPEAT ) == TRUE )
        {
            mu16Amount = 102; // text : 준비 중
        }
        else
        {
            mu16Amount = GetSodaOutPercent();
        }
    }
#endif

    return mu16Amount;
}


// 디폴트 물양 설정 검사
static void CheckFixedAmountType(void)
{
    // 디폴트 물양이 USER인 경우, 사용자 물양이 해제되면
    // 디폴트 물양을 HALF로 강제 변경 
   /*
    if( Out.ConfUser == FALSE 
            && Out.ConfAmount == FIXED_AMOUNT_USER )
    {
        SetWaterOutConfFixedAmountType( FIXED_AMOUNT_HALF );
    }
    */
}

void  ControlWaterOut(void)
{  
    U8 mu8OutDone = WATER_OUT_GOING;

	/* sean [24-03-21] my 물량이 사라지면서 기능 내용 삭제 */
    CheckFixedAmountType();

    // 표시부 기능 
    ControlWaterOutSelect();        // 냉/온/정 선택
    ControlContinueSelect();        // 연속(정량) 선택

    // 추출 정지 조건
    CheckWaterOutStopCondition();   // 추출 정지 조건 검사
    //ControlTankClean();             // 청소 기능 

    // 추출 제어 
    ControlWaterOutCmd();         // 추출 동작 제어

    // 물 종류별 추출 제어
    //mu8OutDone |= ControlWaterSodaOut();
    mu8OutDone |= ControlWaterHotOut();
    mu8OutDone |= ControlWaterRoomOut();
    mu8OutDone |= ControlWaterColdOut();
    mu8OutDone |= ControlWaterFlushOut();
    mu8OutDone |= ControlWaterCodyOut();
    if( mu8OutDone == WATER_OUT_DONE )
    {
        // 재출 방지 시간 설정
        Out.ProtectTime = OUT_PROTECT_TIME;
        StopWaterOut();

        StartDispTimerId( DISP_TIMER_ERROR );
        //Sound(SOUND_EFFLUENT_END);
    }

    if( Out.ProtectTime != 0 )
    {
        Out.ProtectTime--;
    }
}

#ifdef CONFIG_WIFI_WATER_AMOUNT

U32 CalcWaterAmountLitter(U16 mu16litter) // setting 형태로 바꿔줘야함 
{
	return CalcAmount( mu16litter );
}

U32 GetWaterAmountConv(U8 mu8type)
{
	U32 mu32Val;

	if( mu8type == UNFIXED_AMOUNT_HALF)
	{
		mu32Val = CalcWaterAmountLitter(Out.WaterAmountHalf);
	}
	else if( mu8type == UNFIXED_AMOUNT_ONE)
	{
		mu32Val = CalcWaterAmountLitter(Out.WaterAmountOne);
	}
	else //if( mu8type == UNFIXED_AMOUNT_HALF)
	{
		mu32Val = CalcWaterAmountLitter(Out.WaterAmountTwo);
	}

	return mu32Val;
}


U8 GetWaterAmountConf(U8 mu8type)
{
	U8 mu8Val;

	if( mu8type == UNFIXED_AMOUNT_HALF)
	{
		mu8Val = Out.WiFiConfUserHalf;
	}
	else if( mu8type == UNFIXED_AMOUNT_ONE)
	{
		mu8Val = Out.WiFiConfUserHalf;
	}
	else //if( mu8type == UNFIXED_AMOUNT_HALF)
	{
		mu8Val = Out.WiFiConfUserHalf;
	}

	return mu8Val;
}

U16 GetWaterAmountConfig(U16 mu8type)
{
	if(mu8type == WATER_AMOUNT_HALF)
	{
		mu8type = Out.WaterAmountHalf;
	}
	else if(mu8type == WATER_AMOUNT_ONE)
	{
		mu8type = Out.WaterAmountOne;
	}
	else// if(mu8type == FIXED_AMOUNT_TWO)
	{
		mu8type = Out.WaterAmountTwo;
	}
	return mu8type;
}

void SetWaterAmountConfig(U8 mu8waterAmountID, U16 mu16type)
{
	if (mu16type == 120)
	{
		if( mu8waterAmountID == WATER_AMOUNT_HALF )
		{
			Out.WaterAmountHalf = 120;			
		}		
	}
	else
	{
		if( mu8waterAmountID == WATER_AMOUNT_HALF )
		{
			Out.WaterAmountHalf = mu16type;	
			Out.WiFiConfUserHalf = TRUE;
		}		
	}
	
	if (mu16type == 250)
	{
		if( mu8waterAmountID == WATER_AMOUNT_ONE )
		{
			Out.WaterAmountOne = 250;			
		}		
	}
	else
	{
		if( mu8waterAmountID == WATER_AMOUNT_ONE )
		{
			Out.WaterAmountOne = mu16type;	
			Out.WiFiConfUserOne = TRUE;
		}		
	}
	
	if (mu16type == 500)
	{
		if( mu8waterAmountID == WATER_AMOUNT_TWO )
		{
			Out.WaterAmountTwo = mu16type;			
		}		
	}
	else
	{
		if( mu8waterAmountID == WATER_AMOUNT_TWO )
		{
			Out.WaterAmountTwo = mu16type;	
			Out.WiFiConfUserTwo = TRUE;
		}		
	}
	/*
	if( ( mu16type != 120 ) || (mu16type != 250 ) || ( mu16type != 500 ) )
	{
		if( mu8waterAmountID == WATER_AMOUNT_HALF )
		{
			Out.WaterAmountHalf = mu16type;
			
		}
		else if( mu8waterAmountID == WATER_AMOUNT_ONE )
		{
			Out.WaterAmountOne = mu16type;
		}
		else // if( WaterAmountindex == WATER_AMOUNT_TWO )
		{
			Out.WaterAmountTwo = mu16type;
		}
	}
	else
	{
		if( mu8waterAmountID == WATER_AMOUNT_HALF )
		{
			Out.WaterAmountHalf = 120;
			
		}
		else if( mu8waterAmountID == WATER_AMOUNT_ONE )
		{
			Out.WaterAmountOne = 250;
		}
		else // if( WaterAmountindex == WATER_AMOUNT_TWO )
		{
			Out.WaterAmountTwo = 500;
		}
	}
	*/
}

void SetWifiWaterAmountConfig(U8 mu8waterAmountID, U16 mu16type)
{
	//static U8 WaterAmountindex;
	if ( (mu16type < 120 ) || (mu16type > 990 ) )
	{
		return;
	}
	else {}
	
	if( mu8waterAmountID == WATER_AMOUNT_HALF )
	{
		Out.WaterAmountHalf = mu16type;
		Out.WiFiConfUserHalf = TRUE;
		//Out.ConvWaterAmountHalf = CalcWaterAmountLitter(Out.WaterAmountHalf);
		//eeprom 저장 확인
		SaveEepromId(EEP_ID_CONF_USER_AMOUNT_HALF);
		
	}
	else if( mu8waterAmountID == WATER_AMOUNT_ONE )
	{
		Out.WaterAmountOne = mu16type;
		Out.WiFiConfUserOne = TRUE;
		//Out.ConvWaterAmountHalf = CalcWaterAmountLitter(Out.WaterAmountHalf);
		//eeprom 저장 확인
		SaveEepromId(EEP_ID_CONF_USER_AMOUNT_ONE);
	}
	else // if( WaterAmountindex == WATER_AMOUNT_TWO )
	{
		Out.WaterAmountTwo = mu16type;
		Out.WiFiConfUserTwo = TRUE;
		//Out.ConvWaterAmountHalf = CalcWaterAmountLitter(Out.WaterAmountHalf);
		//eeprom 저장 확인
		SaveEepromId(EEP_ID_CONF_USER_AMOUNT_TWO);
	}
}

#endif
