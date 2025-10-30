#include "hw.h"
#include "valve.h"
#include "user.h"
#include "temp.h"
#include "flow_meter.h"
#include "flow_valve.h"
#include "water_out_common.h"
#include "eol.h"
#include "sound.h"
#include "error.h"
#include "util.h"
#include "hot_water_out.h"
#include "hot_water.h"
#include "hot_water_heater.h"
#include "hot_water_flow.h"
#include "hot_water_temp_table.h"
#include "hot_water_out_preheat.h"
#include "heater_out.h"
#include "heater_table.h"



dbg_hot_t   dbg_hot;

static void InitCupTemp(TEMP_T mTargetTemp);
static void CalcCupTemp(void);

#define MAX_HOT_OUT_LASTED_TIME 60000U  //  60min..  @10ms

// DELAY TIME OPEN VALVE INTERVAL 
#define VALVE_DELAY_TIME    100  // 1sec @10ms
//#define LASTED_TIME_OUT     3000U //30sec @10ms
// call interval @10ms

HotOut_T   HotOut;
CupTemp_T Cup;

static void InitOutEventList(void);
static void UpdateHotCurrentAmount(void);
static void UpdateHotInitCurrentAmount(void);

void InitHotOut(void)
{
    HotOut.Out      = FALSE;
    HotOut.PrevOut  = FALSE;
    HotOut.Type     = 0;

    //HotOut.LPM  = MAX_HOT_OUT_LPM;      // 온수 추출 마지막 LPM
    HotOut.LPM  = MAX_LPM;

    HotOut.TargetAmount  = 0UL;
    HotOut.CurrentAmount = 0UL;

    HotOut.Mode = 0U;
    HotOut.RegionTempIn = REGION_TEMP_MID;

    HotOut.AverageOutTemp  = 0.0f;

    // 초기화 추출량은 약 250CC
    HotOut.InitTargetAmount     = GetFindOutTargetTime( OUT_AMOUNT_250 );
    HotOut.InitCurrentAmount    = 0UL;
    HotOut.InitCheckWattAmount  = 0UL;

    HotOut.CoolingTime  = 0U;
    HotOut.MaxTimeOut   = 0U;
    HotOut.LastedOutTime    = 0U;

    FlowValveFullOpen();
}

void  GetHotOutData(HotOut_T *pData)
{
    MEMCPY((void __FAR *) pData, (const void __FAR *)&HotOut, sizeof( HotOut_T ));
}

TEMP_T  GetHotOutAverageOutTemp(void)
{
    return HotOut.AverageOutTemp;
}

TEMP_T GetHotOutPreHeatTemp(void)
{
    return HotOut.TargetPreHeatTemp;
}

TEMP_T GetHotOutTargetOutTemp(void)
{
    return HotOut.TargetOutTemp;
}

U8 GetHotOutRegionTempIn(void)
{
    return HotOut.RegionTempIn;
}

void UpdateRegionTempIn(void)
{
    TEMP_T tIn;


    tIn = GetTemp( TEMP_ID_HOT_IN );
    if( HotOut.RegionTempIn == REGION_TEMP_LOW )
    {
        if( tIn >= 25.0f )
        {
            HotOut.RegionTempIn = REGION_TEMP_HIGH;
        }
        else if( tIn >= 16.0f )
        {
            HotOut.RegionTempIn = REGION_TEMP_MID;
        }
    }
    else if( HotOut.RegionTempIn == REGION_TEMP_MID )
    {
        if( tIn >= 25.0f )
        {
            HotOut.RegionTempIn = REGION_TEMP_HIGH;
        }
        else if( tIn <= 15.0f )
        {
            HotOut.RegionTempIn = REGION_TEMP_LOW;
        }
    }
    else // if( HotOut.RegionTempIn == REGION_TEMP_HIGH )
    {
        if( tIn <= 24.0f )
        {
            HotOut.RegionTempIn = REGION_TEMP_MID;
        }
        else if( tIn <= 15.0f )
        {
            HotOut.RegionTempIn = REGION_TEMP_LOW;
        }
    }
}


// LPM
F32 GetHotOutLPM(void)
{
    return HotOut.LPM;
}

void SetHotOutLPM(F32 mf32LPM)
{
    HotOut.LPM = mf32LPM;
}

void UpdateHotOutLPM(F32 mf32LPM)
{
    if( mf32LPM > MAX_LPM )
    {
        HotOut.LPM  = MAX_LPM;
    }
    else if( mf32LPM < MIN_LPM )
    {
        HotOut.LPM  = MIN_LPM;
    }
    else
    {
        HotOut.LPM  = mf32LPM;
    }
}


// 100ms interval
#define SUM_DELAY_TIME      10  // 100ms @10ms
static void ResetAverageOutTemp(void)
{
    HotOut.SumCount         = 0;
    HotOut.SumOutTemp       = 0.0f;
    HotOut.SumDelayTime     = SUM_DELAY_TIME;
}

static void CalcAverageOutTemp(void)
{
    if( HotOut.SumDelayTime != 0 )
    {
        HotOut.SumDelayTime--;
    }
    else
    {
        HotOut.SumDelayTime = SUM_DELAY_TIME;

        HotOut.SumCount++;
        HotOut.SumOutTemp += GetTemp( TEMP_ID_HOT_OUT );
        HotOut.AverageOutTemp   = HotOut.SumOutTemp / (TEMP_T)HotOut.SumCount;
    }
}


// 상세 제어 모드 
void SetHotOutMode( U16 mu16Mode )
{
    HotOut.Mode |= mu16Mode;
}

U16 GetHotOutMode(void )
{
    return HotOut.Mode;
}

void ClearHotOutMode( U16 mu16Mode )
{
    HotOut.Mode &= ~mu16Mode;
}

U8 IsSetHotOutMode(U16 mu8Mode )
{
    if( (HotOut.Mode & mu8Mode ) == mu8Mode )
    {
        return TRUE;
    }

    return FALSE;
}

void SetHotOutChangedSel(void)
{
    HotOut.Init = TRUE;
}

void ClearHotOutChangedSel(void)
{
    HotOut.Init = FALSE;
}

U8 GetHotOutChangedSel(void)
{
    return HotOut.Init;
}

// 모드가 변경된 경우...
void ClearHotOutLastedTimeOut(void)
{
    HotOut.LastedOutTime = RECHECK_OUTAGE_TIME;
}

void ResetHotOutLastedTimeOut(void)
{
    HotOut.LastedOutTime = 0;
}

U16 GetHotOutLastedTimeOut(void)
{
    return HotOut.LastedOutTime;
}

static void UpdateHotOutTime(void)
{
    if( HotOut.LastedOutTime < MAX_HOT_OUT_LASTED_TIME )
    {
        HotOut.LastedOutTime++;
    }
}

void TurnOnHotOut(WaterOutType_T mType)
{
    U8 mu8Sel;
	U8 mu8TempSel;
    U8 mu8Class;


    mu8Class = GetHeaterClass();
    mu8Sel   = GetHotSelect();
	mu8TempSel = GetTempConfigList(mu8Sel);
 
    HotOut.Out                  = TRUE;
    HotOut.Type                 = mType;
    HotOut.TargetAmount         = GetFindOutTargetTime( HotOut.Type );
    HotOut.TargetOutTemp        = GetHotTargetOutTemp( mu8Sel, mu8Class, HotOut.RegionTempIn );
    HotOut.TargetPreHeatTemp    = GetHotTargetPreHeatTemp( mu8Sel, mu8Class, HotOut.RegionTempIn );
}


U8 IsHotOut(void)
{
    return HotOut.Out;
}


void TurnOffHotOut(void)
{
    HotOut.Out = FALSE;
}


static U8 IsChangedOutStatus(void)
{
    if( HotOut.Out != HotOut.PrevOut )
    {
        HotOut.PrevOut = HotOut.Out;

        return TRUE;
    }
    
    return FALSE;
}


// 온수 추출 시작
static void StartHotOut(void)
{
    if( GetHotWaterInitFull() == FALSE )
    {
        SetHotOutMode( HOT_OUT_FIRST );
    }
    else if( GetHotWaterCooling() == TRUE )
    {
        SetHotOutMode( HOT_OUT_WITH_COOLING );
    }
    else 
    {
        SetHotOutMode( HOT_OUT_ALL );
    }
    ClearHotOutMode( HOT_OUT_RESET );

    HotOut.CurrentAmount        = 0;
    HotOut.InitCurrentAmount    = 0;
}

// 온수 추출 시작 (EOL 모드용)
static void StartHotOutEol(void)
{
    SetHotOutMode( HOT_OUT_ALL_EOL );

    HotOut.CurrentAmount        = 0;
    HotOut.InitCurrentAmount    = 0;
}

static void StopHotOut(void)
{
    U8 mu8IsOut = FALSE;
    U8 mu8IsPreHeat = FALSE;
    U8 mu8IsInit = FALSE;
    U8 mu8IsEolOut = FALSE;


    mu8IsOut     = IsSetHotOutMode( HOT_OUT_OUT );
    mu8IsPreHeat = IsSetHotOutMode( HOT_OUT_PRE_HEATING );
    mu8IsInit    = IsSetHotOutMode( HOT_OUT_INIT_DRAIN );
    mu8IsEolOut  = IsSetHotOutMode( HOT_OUT_OUT_EOL );
    if ( mu8IsOut == TRUE 
            || mu8IsPreHeat == TRUE
            || mu8IsInit == TRUE
            || mu8IsEolOut == TRUE)
    {
        ClearHotOutMode( HOT_OUT_CANCEL );
        SetHotOutMode( HOT_OUT_DONE );
    }
}



typedef U8 (*DoAction_T)(U8 *pStep);
typedef struct _hot_out_event_
{
    U16 Mode;               // 출수 동작 상세 제어 모드
    U8  Step;               // 이벤트 헨들러 스텝 변수
    DoAction_T DoAction;    // 제어 모드에 대한 이벤트 핸들러
} HotOutEvent_T;

static U8 DoInitDrain(U8 *pStep);
static U8 DoCooling(U8 *pStep);
static U8 DoOut(U8 *pStep);
static U8 DoOutEol(U8 *pStep);
static U8 DoDone(U8 *pStep);
static U8 DoReset(U8 *pStep);
static HotOutEvent_T OutEventList[] = 
{
    // 전력 측정 빠짐..
    { HOT_OUT_INIT_DRAIN,   0U, DoInitDrain     },
    { HOT_OUT_COOLING,      0U, DoCooling       },
    { HOT_OUT_PRE_HEATING,  0U, DoPreHeating    },
    { HOT_OUT_OUT_EOL,      0U, DoOutEol        },
    { HOT_OUT_OUT,          0U, DoOut           },
    { HOT_OUT_DONE,         0U, DoDone          },
    { HOT_OUT_RESET,        0U, DoReset         }
};
#define SZ_HOT_OUT_EVENT_LIST  (sizeof(OutEventList)/sizeof(HotOutEvent_T))

static void InitOutEventList(void)
{
    U8 i;

    for( i = 0; i < SZ_HOT_OUT_EVENT_LIST ; i++ )
    {
        OutEventList[i].Step = 0U;
    }
}

static void Outflow(void)
{
    U8 i;
    U8 mu8Ret = FALSE;
    U8 mu8Step;
    U16 mu16Mode;
    HotOutEvent_T *pList = NULL;

    for( i = 0; i < SZ_HOT_OUT_EVENT_LIST ; i++ )
    {
        pList = &OutEventList[ i ];

        if( IsSetHotOutMode( pList->Mode ) == TRUE )
        {
            if( pList->DoAction != NULL )
            {
                mu8Ret = pList->DoAction( &pList->Step );
                if( mu8Ret == TRUE )
                {
                    ClearHotOutMode( pList->Mode );
                }
                break;
            }
        }
    }
}

/*
 * *** 온수 히터 채우는 동작
 * 
 *  -. 탱크에 물이 없는 상태에서 히팅하는 경우 히터 소손 방지
 *  -. 배수 밸브로 추출
 *  -. 배수 유량은 MAX
 *  -. 단수 발생시 동작 중지
 *  -. 추출 종지 조건
 *     > 최대 동작 시간 초과 ( 3분 )
 */
enum
{
    DRAIN_OPEN_OUT,
    DRAIN_OPEN_IN,
    DRAIN_DOING,
    DRAIN_TURN_OFF_HEATER,
    DRAIN_CLOSE_IN,
    DRAIN_DONE,
};

#define MAX_TIME_OUT_INIT           18000U  // 3min( 3min*60sec*100(@10ms) = 180sec
#define CHECK_HEATER_WATT_TIME      300U    // 3sec
static U8 DoInitDrain(U8 *pStep)
{
    static U16 mu16Time = 0U; // @10ms

    switch( *pStep )
    {
        case DRAIN_OPEN_OUT:
            // 밸브 초기화 ( 드레인 OPEN, 유량 최대 OPEN )
            OpenValve( VALVE_HOT_DRAIN );
            CloseValve( VALVE_HOT_OUT );
            FlowValveFullOpen();

            // START CHECK OUTAGE

            // 히터 전력 검사 시점을 목표 배수량의 50% 지점으로 설정한다.
            HotOut.InitCheckWattAmount  = HotOut.InitTargetAmount / 2UL;
            HotOut.InitCurrentAmount    = 0UL;
            HotOut.MaxTimeOut           = MAX_TIME_OUT_INIT;

            InitCheckHeaterWatt();

            mu16Time = VALVE_DELAY_TIME;  
            (*pStep)++;
            break;

        case DRAIN_OPEN_IN:
            // 대기 시간 후, 입수 밸브 OPEN
            if( mu16Time != 0 )
            {
                mu16Time--;
            }
            else
            {
                // OPEN IN VALVE
                RegisterFlowMeterId( FLOW_ID_WATER, UpdateHotInitCurrentAmount );
                StartCheckOutage( OUTAGE_DELAY_TIME, OUTAGE_CHECK_TIME_3SEC, TYPE_HOT_WATER );

                OpenValve( VALVE_HOT_IN );

                mu16Time = CHECK_HEATER_WATT_TIME;
                (*pStep)++;
            }
            break;

        case DRAIN_DOING:

            // 초기 배수 완료
            if( HotOut.InitTargetAmount <= HotOut.InitCurrentAmount )
            {
                SetHotOutMode( HOT_OUT_DONE );
                SetHotWaterInitFull( TRUE );

                (*pStep)++;
                break;
            }
            else
            {
                // 최대 동작 시간 초과
                if( HotOut.MaxTimeOut != 0 )
                {
                    HotOut.MaxTimeOut--;
                }
                else
                {
                    (*pStep)++;
                    break;
                }
            }

            // CHECK HEATER WATT 
            //  -. TURN ON FULL POWER HEATER
            //  -. AND CHECK HEATER WATT
            if( HotOut.InitCheckWattAmount <= HotOut.InitCurrentAmount )
            {
                if( mu16Time != 0 )
                {
                    mu16Time--;
                    SetHeaterOutput( 100 );
                }
                else
                {
                    SetHeaterOutput( 0 );
                }
            }

            CheckHeaterWatt( GetHeaterOutput() );
            break;

        case DRAIN_TURN_OFF_HEATER:
            SetHeaterOutput( 0 );

            mu16Time = VALVE_DELAY_TIME;  
            (*pStep)++;
            break;

        case DRAIN_CLOSE_IN:
            if( mu16Time != 0 )
            {
                mu16Time--;
            }
            else
            {
                UpdateRegionTempIn();

                StopCheckOutage();
                CloseValve( VALVE_HOT_IN );
                (*pStep)++;
            }
            break;

        case DRAIN_DONE:
            return TRUE;
            break;

        default:
            (*pStep) = 0;

            break;
    }

    return FALSE;
}

/*
 * *** 온수 히터 과열식히는 동작 
 * 
 *  -. 배수 밸브로 추출
 *  -. 배수 유량은 MAX
 *  -. 단수 발생시 동작 중지
 *  -. 추출 종지 조건
 *     > 최대 동작 시간 초과 ( 10초 )
 */
enum
{
    COOLING_OPEN_DRAIN,
    COOLING_OPEN_OUT,
    COOLING_DOING,
    COOLING_CLOSE_IN,
    COOLING_DONE,
};

#define COOLING_TIME            1200 // 12sec @10ms
static U8 DoCooling(U8 *pStep)
{
   static U16 mu16Time = 0U; // @10ms

    switch( *pStep )
    {
        case COOLING_OPEN_DRAIN:
            OpenValve( VALVE_HOT_IN );
            OpenValve( VALVE_HOT_DRAIN );

            SetHeaterOutput( 0 );
            FlowValveFullOpen();

            // START CHECK OUTAGE
            RegisterFlowMeterId( FLOW_ID_WATER, UpdateHotInitCurrentAmount );
            StartCheckOutage( OUTAGE_DELAY_TIME, OUTAGE_CHECK_TIME_3SEC, TYPE_HOT_WATER );

            HotOut.CoolingTime = COOLING_TIME;
            mu16Time = VALVE_DELAY_TIME;

            (*pStep)++;
            break;

        case COOLING_OPEN_OUT:
            if( mu16Time != 0 )
            {
                mu16Time--;
            }
            else
            {
                CloseValve( VALVE_HOT_OUT );
                (*pStep)++;
            }
            break;

        case COOLING_DOING:
            // 단수 상태면 종료
            // COOLING 시간이 초과되면 종료
            if( GetOutageStatus(TYPE_HOT_WATER) == FLOW_OUTAGE )
            {
                (*pStep)++;
            }
            else if( GetTemp( TEMP_ID_HOT_IN ) + 10.0f >= GetTemp( TEMP_ID_HEATER ) )
            {
                // 온도 만족 해제..
                SetHotWaterCooling( FALSE );    
                (*pStep)++;
            }
            else if( HotOut.CoolingTime != 0 )
            {
                HotOut.CoolingTime--;
            }
            else
            {
                // 시간 초과 해제...
                SetHotWaterCooling( FALSE );    
                (*pStep)++;
            }

            mu16Time = VALVE_DELAY_TIME;  
            break;

        case COOLING_CLOSE_IN:
            if( mu16Time != 0 )
            {
                mu16Time--;
            }
            else
            {
                RegisterFlowMeterId( FLOW_ID_WATER, NULL );
                StopCheckOutage();
                CloseValve( VALVE_HOT_IN );
                (*pStep)++;
            }
            break;

        case COOLING_DONE:

            return TRUE;
            break;

        default:
            (*pStep) = 0;

            break;
    }

    return FALSE;
}




/*
 * *** 온수 추출 
 * 
 *  -. 배수 밸브로 추출
 *  -. 배수 유량은 MAX
 *  -. 단수 발생시 동작 중지
 *  -. 추출 종지 조건
 */
enum
{
#if 1
    OUT_INIT_VALVE,
    OUT_INIT_VALVE_2,
#endif
    OUT_INIT,
    OUT_DOING,
    OUT_DONE,
};


// 히터 출력 제어
static U16 ChekHotSel( U16 mu16Output, U8 mTime );
static void ControlWatt(void)
{
    U16 mu16Output;
    F32 mLPM;


#if 0
    CalcHeaterTargetWatt( GetHotWaterTargetLPM() );
#else
    mLPM = GetFlowMeterLPM();
    if( mLPM < MIN_LPM )
    {
        mLPM = MIN_LPM;
    }
#endif
    CalcHeaterTargetWatt( mLPM );
    mu16Output =  CalcHeaterOutput();

    mu16Output = ChekHotSel( mu16Output, FALSE );
    mu16Output = CheckOverHeating( mu16Output );
    SetHeaterOutput( mu16Output );
}



// 릴레이 체터링 방지...
#define RELAY_TIME  30
static U16 CheckLimitOutputRelay(U16 mu16Output, U8 mTime, U8 UpTime, U8 DownTime )
{
    static U8 mu8Up     = RELAY_TIME;
    static U8 mu8Down   = RELAY_TIME;
    static U8 mu8Toggle = OFF;


    // 초기 unstable 상태에서는 해당 타이머 조건을 skip...
    if( mTime == FALSE )
    {
        mu8Up   = UpTime;
        mu8Down = DownTime;
        mu8Toggle = OFF;
        return mu16Output;
    }

    if( mu16Output > 45 
            && mu8Toggle == OFF)
    {
        mu8Down   = DownTime;
        if( mu8Up != 0 )
        {
            mu8Up--;

            return 45;
        }

        mu8Toggle = TRUE;
    }
    if( mu16Output < 55 
            && mu8Toggle == ON)
    {
        mu8Up   = UpTime;
        mu8Toggle = ON;
        if( mu8Down != 0 )
        {
            mu8Down--;

            mu8Toggle = OFF;
            return 55;
        }
    }

    if( mu8Toggle == ON )
    {
        if( mu16Output < 55 )
        {
            return 45;
        }
    }
    else 
    {
        if( mu16Output > 45 )
        {
            return 55;
        }
    }


    return mu16Output;
}

#if 0
#define CHECK_HOT_SEL_TIME  20
static U16 CheckLimitOutput(U16 mu16Output, U8 mTime)
{
    static U8 mu8Count = CHECK_HOT_SEL_TIME;

    // 초기 unstable 상태에서는 해당 타이머 조건을 skip...
    if( mTime == FALSE )
    {
        mu8Count = CHECK_HOT_SEL_TIME;
    }

    if( mu16Output >= 65 )
    {
        if( mu8Count != 0 )
        {
            mu8Count--;

            if( mu16Output > 45 ) 
            {
                mu16Output = 45;
            }
        }
    }
    else
    {
        mu8Count = CHECK_HOT_SEL_TIME;

        if( mu16Output > 45 ) 
        {
            mu16Output = 45;
        }
    }

    return mu16Output;
}
#endif
#define CHECK_HOT_SEL_TIME  30
static U16 CheckLimitOutput(U16 mu16Output, U8 mTime)
{
    static U8 mu8UpCount    = CHECK_HOT_SEL_TIME;
    static U8 mu8DownCount  = CHECK_HOT_SEL_TIME;

    // 초기 unstable 상태에서는 해당 타이머 조건을 skip...
    if( mTime == FALSE )
    {
        mu8UpCount      = CHECK_HOT_SEL_TIME;
        mu8DownCount    = CHECK_HOT_SEL_TIME;
    }

    // ON RELAY..
    if( GetHeaterOutRegion() == REGION_DOUBLE )
    {
        if( mu16Output <= 45 )
        {
            if( mu8DownCount != 0 )
            {
                mu8DownCount--;

                mu16Output = 46;
            }
        }
        else
        {
            mu8DownCount = CHECK_HOT_SEL_TIME;
        }

        mu8UpCount = CHECK_HOT_SEL_TIME;
    }
    // OFF RELAY...
    else
    {
        if( mu16Output >= 50 )
        {
            if( mu8UpCount != 0 )
            {
                mu8DownCount = CHECK_HOT_SEL_TIME;
                mu8UpCount--;

                mu16Output = 50;
            }
        }
        else if( mu16Output > 45 )
        {
            mu8DownCount = CHECK_HOT_SEL_TIME;
            mu16Output = 45;
        }
        else
        {
            mu8UpCount = CHECK_HOT_SEL_TIME;
        }

        mu8DownCount = CHECK_HOT_SEL_TIME;
    }

    return mu16Output;
}

static U16 ChekHotSel( U16 mu16Output, U8 mTime )
{
    // 분유이면.... 출력이 65 이상 2초 이상 유지되면 않으면
    // 45%로 출력을 제한한다.... 트라이악만 사용하기 위해서

    if(  GetHotSelect() == SEL_HOT_TEA 
            && GetTemp( TEMP_ID_HOT_IN ) > 18.0f 
            && GetTemp( TEMP_ID_HOT_IN ) < 28.0f 
            && GetHeaterClass() == HEATER_CLASS_LOW )
    {
        if( GetTemp( TEMP_ID_HEATER ) < 80.0f )
        {
            if( mu16Output <= 55 )
            {
                mu16Output = 55;
            }
        }
    }
    else if(  GetHotSelect() == SEL_HOT_COFFEE  
            && GetTemp( TEMP_ID_HOT_IN ) > 18.0f 
            && GetTemp( TEMP_ID_HOT_IN ) < 28.0f 
            && GetHeaterClass() == HEATER_CLASS_LOW )
    {
        // 릴레이 off되는 소음을 막기 위해서 55% 이하여도...
        // 출력 유지...
        // 단 과열 방지를 위해서 히터 내부 온도 90도 이하에서만..
        if( GetTemp( TEMP_ID_HEATER ) < 90.0f )
        {
            if( mu16Output <= 55 )
            {
                mu16Output = 55;
            }
        }
    }

    if(  GetHotSelect() == SEL_HOT_USER 
            && GetTemp( TEMP_ID_HOT_IN ) > 27.0f 
            && GetHeaterClass() == HEATER_CLASS_HIGH )
    {
        if( GetTemp( TEMP_ID_HEATER ) >= 95.0f )
        {
            if( mTime == FALSE )
            {
                if( mu16Output > 55 )
                {
                    mu16Output = 55;
                }
            }
        }
    }
    else if( ( GetHotSelect() == SEL_HOT_COFFEE ) || ( GetHotSelect() == SEL_HOT_DEGREE_80 ) || ( GetHotSelect() == SEL_HOT_DEGREE_75 ) )
    {
        // 커피 추출시 내부 온도 고온이면 이상이면 출력 제한...
        if( GetTemp( TEMP_ID_HEATER ) >= 98.0f )
        {
            if( mu16Output >= 55 )
            {
                mu16Output = 55;
            }
        }
        else if( GetTemp( TEMP_ID_HEATER ) >= 95.0f )
        {
            if( mu16Output >= 70 )
            {
                mu16Output = 70;
            }
        }
		else {}
    }
    // 티 추출시 온도 제한...
    else if( ( GetHotSelect() == SEL_HOT_TEA ) || ( GetHotSelect() == SEL_HOT_DEGREE_65 ) || ( GetHotSelect() == SEL_HOT_DEGREE_60 ) || ( GetHotSelect() == SEL_HOT_DEGREE_55 ) || ( GetHotSelect() == SEL_HOT_DEGREE_50 ) )
    {
        if( GetTemp( TEMP_ID_HOT_OUT ) >= 85.0f
                || GetTemp( TEMP_ID_HEATER ) >= 90.0f )
        {
            if( mu16Output >= 55 )
            {
                mu16Output = 55;
            }
            else if( mu16Output >= 45 )
            {
                mu16Output = 20;
            }
        }
    }


    //else if(  GetHotSelect() == SEL_HOT_COFFEE  
    //        && GetTemp( TEMP_ID_HOT_IN ) > 23.0f 
    //        && GetHeaterClass() == HEATER_CLASS_HIGH )
    //{
    //    // 히터 과열 방지... 히터 내부 온도가 90도가 넘어가면 히터 제한...
    //    if( GetTemp( TEMP_ID_HEATER ) >= 95.0f )
    //    {
    //        if( mu16Output >= 80 )
    //        {
    //            mu16Output = 80;
    //        }
    //    }
    //}
    //else if( GetHotSelect() == SEL_HOT_MILK 
    //        || 
    //        ( GetHotSelect() == SEL_HOT_TEA 
    //          && GetTemp( TEMP_ID_HOT_IN ) > 28.0f )
    //        //||
    //        //( GetHotSelect() == SEL_HOT_TEA 
    //        //  && GetTemp( TEMP_ID_HOT_IN ) > 18.0f 
    //        //  && GetHeaterClass() != HEATER_CLASS_LOW )
    //       )

    if( GetHotSelect() == SEL_HOT_MILK 
            || GetHotSelect() == SEL_HOT_DEGREE_50
            || GetHotSelect() == SEL_HOT_DEGREE_55
            || GetHotSelect() == SEL_HOT_DEGREE_60
            || GetHotSelect() == SEL_HOT_DEGREE_65
            || GetHotSelect() == SEL_HOT_TEA
            || GetHotSelect() == SEL_HOT_DEGREE_75
            || GetHotSelect() == SEL_HOT_DEGREE_80
            || GetHotSelect() == SEL_HOT_COFFEE
            || GetHotSelect() == SEL_HOT_DEGREE_90
            || GetHotSelect() == SEL_HOT_DEGREE_95
            || GetHotSelect() == SEL_HOT_USER
      )
    {
        mu16Output = CheckLimitOutput( mu16Output, mTime );
    }

    return mu16Output;
}

// 저유량 조건에서는 히터 출력으로 10%으로 낮춤
#define RECAL_FLOW_TIME     10      // 1sec @100ms
U8 gu8LowFlowCount = RECAL_FLOW_TIME;
static U16 CheckLowFlow(F32 mLPM, U16 mu16Output)
{
    if( mLPM <= 0.01f )
    {
        if( gu8LowFlowCount != 0 )
        {
            gu8LowFlowCount--;
        }
        else
        {
            if( mu16Output > 10 )
            {
                mu16Output = 10;
            }
        }
    }
    else
    {
        gu8LowFlowCount = RECAL_FLOW_TIME;
    }

    return mu16Output;
}

#define RECAL_WATT_TIME    10   // 100ms @10ms
U16 ReCalTime = 0;
static void ControlWattByOffset(F32 CurrentLPM)
{
    static U16 mu16Output = 0;

    if( ReCalTime != 0 )
    {
        ReCalTime--;
    }
    else
    {
        ReCalTime = RECAL_WATT_TIME;

        // 목표 WATT 계산
        CalcHeaterTargetWatt( CurrentLPM ); 
        mu16Output =  CalcHeaterOutput();

        CalcOutputOffset( CurrentLPM, HotOut.TargetOutTemp, GetTemp(TEMP_ID_HOT_OUT) );
        mu16Output = ChekHotSel( mu16Output, TRUE );
        mu16Output = CheckOverHeating( mu16Output );
        mu16Output = CheckLowFlow( CurrentLPM, mu16Output );
        SetHeaterOutput( mu16Output );
    }
}


#define CHECK_UNDER_TEMP_TIME      200  // 2sec @10ms
static U16 GetUnderTempTime(void)
{
    // 고온수는 빨리... 1초 주기로...
    if( ( GetHotSelect() == SEL_HOT_USER ) || ( GetHotSelect() == SEL_HOT_DEGREE_95 ) || ( GetHotSelect() == SEL_HOT_DEGREE_90 )  )
    {
        return 50;  // 500ms...
    }

    // 나머지는 2초 주기로... 
    return CHECK_UNDER_TEMP_TIME;
}
static F32 GetUnderTempFlowVal(void)
{
    if( ( GetHotSelect() == SEL_HOT_USER ) || ( GetHotSelect() == SEL_HOT_DEGREE_95 ) || ( GetHotSelect() == SEL_HOT_DEGREE_90 )  )
    {
        if( GetHeaterClass() == HEATER_CLASS_LOW )
        {
            return 0.04f;
        }
    }
    return 0.02f;
}

U8 dbg_the_flow = 0;
static void CheckUnderTemp(void)
{
    static U16 mu16Time = CHECK_UNDER_TEMP_TIME;
    static U8 mu8Flag = FALSE;
    F32 mLPM;


    //if( GetHotSelect() != SEL_HOT_USER )
    {
        if( GetHeaterOutput() < 101 )
        {
            mu16Time = GetUnderTempTime();
            return ;
        }

        if( mu16Time != 0 )
        {
            mu16Time--;
        }
        else
        {
            mu16Time = GetUnderTempTime();

            mLPM = GetFlowOffsetLPM();
            mLPM -= GetUnderTempFlowVal(); //0.04f;
            SetFlowOffsetLPM( mLPM );
        }
    }
    //else
    {
#if 0   
        // 고온수 추출시 과열 방지를 위해 유량을 살짝 살짝  조절하는 동작...
        // 히터 출력 제한으로 커버가 가능해서 해당 기능 삭제
        if( dbg_the_flow == 1 )
        {
            if( GetTemp( TEMP_ID_HEATER ) >= 98.0f )
            {
                // 고열 조건이면 줄어든 유량을 정상화 시킨다.
                if( mu16Time != 0 )
                {
                    mu16Time--;
                }
                else
                {
                    mu16Time = GetUnderTempTime();
                    if( mu8Flag == 0 )
                    {
                        mu8Flag = 1;
                        SetFlowOffsetLPM( 0.02f );
                    }
                    else
                    {
                        mu8Flag = 0;
                        SetFlowOffsetLPM( 0.00f );
                    }
                }
            }
            else
            {
                mu8Flag = 0;
                SetFlowOffsetLPM( 0.0f );
                mu16Time = GetUnderTempTime();
            }
        }
#endif
    }
}


static void ControlFlow(void)
{
    F32 mTargetLPM = 0.0f;
    F32 mCurrentLPM = 0.0f;



    // 계산된 목푱 유량에 맞추는 로직
    CalcHeaterTargetLPM();                          
    mTargetLPM  = GetHotWaterTargetLPM();

	/* sean [24-03-22] 과히팅 되어 높은 온도 추출시, 유량 증가 */
    if( GetHeaterClass() == HEATER_CLASS_HIGH )
    {
        if( ( GetHotSelect() == SEL_HOT_COFFEE ) || (GetHotSelect() == SEL_HOT_DEGREE_55)  || ( GetHotSelect() == SEL_HOT_DEGREE_60 ) 
			   || ( GetHotSelect() == SEL_HOT_DEGREE_65 )  )
        {
            if( GetHeaterBaseWatt() >= HIGH_HEATER_WATT )
            {
                mTargetLPM += 0.05f;        // 과열 방지
            }
        }
        //else if( ( GetHotSelect() == SEL_HOT_USER ) || ( GetHotSelect() == SEL_HOT_DEGREE_95 ) || ( GetHotSelect() == SEL_HOT_DEGREE_90 )  )
        //{
        //    mTargetLPM -= 0.07f;        // 유량 부족...
        //}
		//else {}
    }
    else if( GetHeaterClass() == HEATER_CLASS_LOW )
    {
        //if( ( GetHotSelect() == SEL_HOT_USER ) || ( GetHotSelect() == SEL_HOT_DEGREE_95 ) || ( GetHotSelect() == SEL_HOT_DEGREE_90 )  )
        //{
        //   mTargetLPM -= 0.07f;        // 유량 부족...
        //}
		//else if ( GetHotSelect() == SEL_HOT_DEGREE_60 )
		if ( GetHotSelect() == SEL_HOT_DEGREE_60 )
		{
            mTargetLPM -= 0.02f;        // 유량 부족...
        }
		else {}
    }
    else if( GetHeaterClass() == HEATER_CLASS_MID )
    {
        if( GetHotSelect() == SEL_HOT_TEA )
        {
            mTargetLPM -= 0.02f;        // 유량 부족...
        }
        //else if( ( GetHotSelect() == SEL_HOT_USER ) || ( GetHotSelect() == SEL_HOT_DEGREE_95 ) || ( GetHotSelect() == SEL_HOT_DEGREE_90 )  )
        //{
        //   mTargetLPM -= 0.07f;        // 유량 부족...
        //}
		//else {}
    }

	if( ( GetHotSelect() == SEL_HOT_USER ) || ( GetHotSelect() == SEL_HOT_DEGREE_95 ) || ( GetHotSelect() == SEL_HOT_DEGREE_90 )  )
    {
        if( GetHotOutPreHeatType() != LOW_PREHEAT )
        {
            if( GetTemp( TEMP_ID_HOT_OUT ) <= 77.0f )
            {
                mTargetLPM -= 0.15f;        // 80도 이하면 0.15 lpm 
            }
            else if( GetTemp( TEMP_ID_HOT_OUT ) <= 80.0f )
            {
                mTargetLPM -= 0.10f;        // 80도 이하면 0.10 lpm 
            }
            else if( GetTemp( TEMP_ID_HOT_OUT ) <= 90.0f )
            {
                mTargetLPM -= 0.07f;        // 90도 이하면 0.07 lpm 
            }
            else    // if 90도 이상
            {
                if( GetTemp( TEMP_ID_HOT_IN ) <= 13.0f )
                {
                    mTargetLPM -= 0.07f;        // 90도 이하면 0.05 lpm down..
                }
                else
                {
                    mTargetLPM -= 0.05f;        // 90도 이하면 0.05 lpm down..
                }
            }
        }
        else
        {
            // 입수 온도가 너무 낮을때, 추출 완료 온도에서 -1에서 -2도 정도
            // 부족한 현상 개선
            if( GetTemp( TEMP_ID_HOT_IN ) <= 13.0f )
            {
                mTargetLPM -= 0.07f;        // 90도 이하면 0.05 lpm down..
            }
            else
            {
                mTargetLPM -= 0.05f;        // 90도 이하면 0.05 lpm down..
            }
        }
    }

    mCurrentLPM = GetFlowMeterLPM();

    CheckUnderTemp();
    AdjustFlowValveByFlow( mTargetLPM + GetFlowOffsetLPM(), mCurrentLPM );
}
    

#define UNSTABLE_AMOUNT   AMOUNT_20
static void Outing(void)
{
    // 초기 unstable 상태이므로 대기...
    if( HotOut.CurrentAmount < UNSTABLE_AMOUNT )
    {
        // 목표 WATT 계산
        ControlWatt();
        gu8LowFlowCount = RECAL_FLOW_TIME;
    }
    else
    {
        // 목표 WATT 제어 - 현재 유량
        ControlWattByOffset( GetFlowMeterLPM() );
        ControlFlow();
    }

    // Re-Check heater watt
    CheckHeaterWatt( GetHeaterOutput() );

    CalcAverageOutTemp();   // IOT, 평균 온수 추출 온도 계산
}

static void StopOut(void)
{
    if( Cup.SumCount < CUP_HALF_NUM )
    {
        // 유량 조절 밸브 초기화
        InitFlowValveSampleLPM();
    }
    else
    {
        // 유량 대비 모터 위치 값 2차 측정
        SetFlowValveSampleLPM( GetFlowMeterLPM(), GetFlowValveTarget() );
    }

    // TURN OFF HEATER
    SetHeaterOutput( 0 );

    // CLOSE VALVES..
    CloseValve( VALVE_HOT_OUT );
    OpenValve( VALVE_HOT_DRAIN );
    RegisterFlowMeterId( FLOW_ID_WATER, NULL );

}

static void SetupStartingFlowLPM(void)
{
    InitOffsetLPM();

    if( GetHotSelect() == SEL_HOT_USER 
            && GetHeaterClass() == HEATER_CLASS_LOW 
            && GetTemp( TEMP_ID_HOT_IN ) < 20.0f )
    {
        SetFlowOffsetLPM( -0.04f );
    }
	if( GetHotSelect() == SEL_HOT_DEGREE_95 
            && GetHeaterClass() == HEATER_CLASS_LOW 
            && GetTemp( TEMP_ID_HOT_IN ) < 20.0f )
    {
        SetFlowOffsetLPM( -0.04f );
    }
	if( GetHotSelect() == SEL_HOT_DEGREE_90 
            && GetHeaterClass() == HEATER_CLASS_LOW 
            && GetTemp( TEMP_ID_HOT_IN ) < 20.0f )
    {
        SetFlowOffsetLPM( -0.04f );
    }
			
}

#if 0
static U8 DoOut(U8 *pStep)
{
    static U16 mu16Time = 0U; // @10ms


    switch( *pStep )
    {
        case OUT_INIT:
            // VALVE CONTROL
            OpenValve( VALVE_HOT_OUT );
            OpenValve( VALVE_HOT_IN );
            CloseValve( VALVE_HOT_DRAIN );

            InitCupTemp( HotOut.TargetOutTemp );

            // SET TARGET TEMPERTURE
            SetHeaterTempTargetOut( HotOut.TargetOutTemp );
            ResetAverageOutTemp();

            // CLEAR CURRENT AMOUNT
            HotOut.CurrentAmount    = 0U;
            RegisterFlowMeterId( FLOW_ID_WATER, UpdateHotCurrentAmount );
            StartCheckOutage( OUTAGE_HOT_OUT_DELAY_TIME, OUTAGE_CHECK_TIME_3SEC, TYPE_HOT_WATER );

            InitAdjustFlowValve();
            SetupStartingFlowLPM();

            ResetHeaterOffset();

            InitCheckHeaterWatt();

            dbg_hot.adjust_up   = 0;
            dbg_hot.adjust_down = 0;

            (*pStep)++;
            break;

        case OUT_DOING:
            if( HotOut.TargetAmount > HotOut.CurrentAmount )
            {
                /* 추출 중 */
                Outing();
            }
            else
            {
                /* 추출 종료 */
                StopOut();

                // SET CLOSER IN VALVE DELAY TIME...
                //mu16Time = VALVE_DELAY_TIME;      // 1sec
                mu16Time = 20;  // 200ms
                (*pStep)++;
            }
            break;

        case OUT_DONE:
            if( mu16Time != 0 )
            {
                mu16Time--;
            }
            else
            {
                CloseValve( VALVE_HOT_IN );

                SetHotOutMode( HOT_OUT_DONE );
                (*pStep) = 0;
                return TRUE;
            }

            break;


        default:
            (*pStep) = 0;
            break;
    }

    return FALSE;
}
#else
static U8 DoOut(U8 *pStep)
{
    static U16 mu16Time = 0U; // @10ms


    switch( *pStep )
    {
        case OUT_INIT_VALVE:
            // VALVE CONTROL
            CloseValve( VALVE_HOT_OUT );
            OpenValve( VALVE_HOT_IN );
            OpenValve( VALVE_HOT_DRAIN );

            if( ( GetHotSelect() == SEL_HOT_USER ) || ( GetHotSelect() == SEL_HOT_DEGREE_95 ) || ( GetHotSelect() == SEL_HOT_DEGREE_90 )  )
            {
                // 재추출시 에어 쏘는 효과 개선을 위해서 드레인 1초...
                mu16Time = 120U; // 1200ms @10ms
            }
            else
            {
                mu16Time = 20U; // 200ms @10ms
            }
            (*pStep)++;
            break;

        case OUT_INIT_VALVE_2:
            if( mu16Time != 0 )
            {
                mu16Time--;
                break;
            }

            OpenValve( VALVE_HOT_OUT );
            OpenValve( VALVE_HOT_IN );
            OpenValve( VALVE_HOT_DRAIN );

            mu16Time = 20U; // 200ms @10ms
            (*pStep)++;
            break;

        case OUT_INIT:
            // VALVE CONTROL
            //OpenValve( VALVE_HOT_OUT );
            //OpenValve( VALVE_HOT_IN );
            //CloseValve( VALVE_HOT_DRAIN );

            if( mu16Time != 0 )
            {
                mu16Time--;
                break;
            }
            OpenValve( VALVE_HOT_OUT );
            OpenValve( VALVE_HOT_IN );
            CloseValve( VALVE_HOT_DRAIN );

            InitCupTemp( HotOut.TargetOutTemp );

            // SET TARGET TEMPERTURE
            SetHeaterTempTargetOut( HotOut.TargetOutTemp );
            ResetAverageOutTemp();

            // CLEAR CURRENT AMOUNT
            HotOut.CurrentAmount    = 0U;
            RegisterFlowMeterId( FLOW_ID_WATER, UpdateHotCurrentAmount );
            StartCheckOutage( OUTAGE_HOT_OUT_DELAY_TIME, OUTAGE_CHECK_TIME_3SEC, TYPE_HOT_WATER );

            InitAdjustFlowValve();
            SetupStartingFlowLPM();

            ResetHeaterOffset();

            InitCheckHeaterWatt();

            dbg_hot.adjust_up   = 0;
            dbg_hot.adjust_down = 0;

            (*pStep)++;
            break;

        case OUT_DOING:
            if( HotOut.TargetAmount > HotOut.CurrentAmount )
            {
                /* 추출 중 */
                Outing();
            }
            else
            {
                /* 추출 종료 */
                StopOut();

                // SET CLOSER IN VALVE DELAY TIME...
                //mu16Time = VALVE_DELAY_TIME;      // 1sec
                mu16Time = 20;  // 200ms
                (*pStep)++;
            }
            break;

        case OUT_DONE:
            if( mu16Time != 0 )
            {
                mu16Time--;
            }
            else
            {
                CloseValve( VALVE_HOT_IN );

                SetHotOutMode( HOT_OUT_DONE );
                (*pStep) = 0;
                return TRUE;
            }

            break;


        default:
            (*pStep) = 0;
            break;
    }

    return FALSE;
}
#endif


// DO OUT - EOL MODE
// 테스트 모드 추출 동작이므로.. 진공 질소임.
// 히터 제어 없음
enum
{
    EOL_OUT_OPEN_OUT,
    EOL_OUT_DOING,
    EOL_OUT_DONE,
};
static U8 DoOutEol(U8 *pStep)
{
    static U16 mu16Time = 0U; // @10ms


    switch( *pStep )
    {
        case EOL_OUT_OPEN_OUT:
            OpenValve( VALVE_HOT_OUT );
            OpenValve( VALVE_HOT_IN );
            CloseValve( VALVE_HOT_DRAIN );
            FlowValveFullOpen();

            mu16Time = VALVE_DELAY_TIME;

            HotOut.CurrentAmount = 0U;

            RegisterFlowMeterId( FLOW_ID_WATER, UpdateHotCurrentAmount );
            StartCheckOutage( OUTAGE_DELAY_TIME, OUTAGE_CHECK_TIME_3SEC, TYPE_HOT_WATER );

            (*pStep)++;
            break;

        case EOL_OUT_DOING:
            SetHeaterOutput( 0 );

            if( HotOut.TargetAmount > HotOut.CurrentAmount )
            {
                // doing...
                OpenValve( VALVE_HOT_IN );
            }
            else
            {
                RegisterFlowMeterId( FLOW_ID_WATER, NULL );
                StopCheckOutage();
                CloseValve( VALVE_HOT_OUT );
                OpenValve( VALVE_HOT_DRAIN );

                mu16Time = VALVE_DELAY_TIME;
                (*pStep)++;
            }
            break;

        case EOL_OUT_DONE:
            if( mu16Time != 0 )
            {
                mu16Time--;
            }
            else
            {
                CloseValve( VALVE_HOT_IN );

                SetHotOutMode( HOT_OUT_DONE );
                (*pStep) = 0;
            }
            break;

        default:
            (*pStep) = 0;
            break;
    }

    return FALSE;
}

// 밸브 초기화
// 히터 OFF
// 유량 센서 OFF
U8 dbg_do_done = 0;
static U8 DoDone(U8 *pStep)
{
    // 온수 단수 에러 발생된 경우, 밸브 위치 초기화
    if( IsErrorType( ERR_TYPE_HOT_WATER_OUT ) == TRUE 
            || IsError( ERR_OUTAGE_HOT_WATER ) == TRUE 
            || IsError( ERR_HEATER_OVERHEAT ) == TRUE )
    {
        InitFlowValveSampleLPM();
    }
    else
    {
        // 추출 전에 취소한 경우
        if( Cup.SumCount < CUP_HALF_NUM )
        {
            //InitFlowValveSampleLPM();
        }
        else if( IsOpenValve( VALVE_HOT_OUT ) == TRUE )
        {
            // DEFAULT SETTING VALVES...
            SetFlowValveSampleLPM( GetFlowMeterLPM(), GetFlowValveTarget() );
        }
    }

#if CONFIG_TEST_HOT_DRAIN
    CloseValve( VALVE_HOT_DRAIN );
#else
    OpenValve( VALVE_HOT_DRAIN );
#endif
    OpenValve( VALVE_HOT_IN );
    CloseValve( VALVE_HOT_OUT );

    UpdateRegionTempIn();

    // TURN OFF HEATER...
    SetHeaterOutput( 0 );
    SetHotWaterHeaterOffsetWatt(0);
    TURN_OFF_HEATER_TRIAC();
    TURN_OFF_HEATER_RELAY();

    // DEFAULT CURRENT AMOUNT & CHECK OUTAGE 
    //RegisterFlowMeterId( FLOW_ID_WATER, NULL );
    //StopCheckOutage();

    SetHotOutMode( HOT_OUT_RESET );
    dbg_do_done++;

    return TRUE;
}

enum
{
    RESET_INIT,
    RESET_DOING,
    RESET_DONE
};

#if 0
U8 dbg_do_rest_time = 0;
#define RESET_TIME_1S  dbg_do_rest_time     // 1sec @ 10ms
#define RESET_TIME_2S  dbg_do_rest_time     // 1sec @ 10ms
#else
#define RESET_TIME_1S  100     // 1sec @ 10ms
#define RESET_TIME_2S  200     // 1sec @ 10ms
#endif

static U8 DoReset(U8 *pStep)
{
#if 1
    static U16 mu16Time = 0U; // @10ms


    switch( *pStep )
    {
        case RESET_INIT:
#if 0
            if( GetTemp( TEMP_ID_HOT_OUT ) >= 98.0f 
                    || GetTemp( TEMP_ID_HEATER ) >= 98.0f )
            {
                OpenValve( VALVE_HOT_DRAIN );
                OpenValve( VALVE_HOT_IN );

                if( GetTemp( TEMP_ID_HEATER ) >= 100.0f )
                {
                    mu16Time = RESET_TIME_2S;  // 2esc
                }
                else
                {
                    mu16Time = RESET_TIME_1S;
                }
                (*pStep)++;
            }
            else
#endif
            {
                (*pStep) = RESET_DONE;
            }
            break;

        case RESET_DOING:
            if( --mu16Time == 0 
                    || GetWaterOut() == TRUE )
            {
                OpenValve( VALVE_HOT_DRAIN );
                CloseValve( VALVE_HOT_IN );

                StopCheckOutage();

                (*pStep) = 0;
                return TRUE;
            }
            break;

        case RESET_DONE:
            OpenValve( VALVE_HOT_DRAIN );
            CloseValve( VALVE_HOT_IN );

            StopCheckOutage();

            (*pStep) = 0;
            return TRUE;

        default:
            (*pStep) = 0;
            break;
    }

    return FALSE;
#else
    OpenValve( VALVE_HOT_DRAIN );
    CloseValve( VALVE_HOT_IN );
    StopCheckOutage();

    return TRUE;
#endif
}



U8 GetHotOutPercent(void)
{
    return CalcPercent( HotOut.TargetAmount, HotOut.CurrentAmount );
}

// 추출 진행량( 0~ 999 mL )
U16 GetHotOutAmount(void)
{
    return CalcLiter( HotOut.CurrentAmount );
}

static void UpdateUserUsage(U32 mu32Amount)
{
    U32 mu32TargetAmount;

    mu32TargetAmount = GetFindOutTargetTime(  OUT_AMOUNT_10 );
    if( mu32TargetAmount != 0 )
    {
        mu32Amount = mu32Amount / mu32TargetAmount;
        mu32Amount *= 10;
    }
    else
    {
        mu32Amount = 0;
    }

    AddUserUsageWater( mu32Amount );
}


static void InitCupTemp(TEMP_T mTargetTemp)
{
    U8 i;

    Cup.tSum             = 0.0f;
    Cup.tAverage         = 0.0f;
    Cup.SumCount         = 0;
    Cup.AmountHz         = AMOUNT_10;
    Cup.FirstTargetTemp  = mTargetTemp;
    Cup.SecondTargetTemp = 0.0f;

    for( i = 0; i < CUP_LIST_NUM ; i++ )
    {
        Cup.tSumList[ i ] = 0.0f;
        Cup.lpmSumList[ i ] = 0.0f;
    }
}

static void CalcCupTemp(void)
{
    TEMP_T tOut = 0.0f;
    TEMP_T tTemp;

    if( --Cup.AmountHz == 0 )
    {
        Cup.AmountHz = AMOUNT_10;

        if( Cup.SumCount < CUP_LIST_NUM )
        {
            tOut = GetTemp( TEMP_ID_HOT_OUT );
            Cup.lpmSumList[ Cup.SumCount ]  = GetFlowMeterLPM();
            Cup.tSumList[ Cup.SumCount ]    = tOut;
            Cup.tSum += tOut;

            Cup.SumCount++;
            Cup.tAverage = Cup.tSum / Cup.SumCount;

            if( Cup.SumCount == CUP_HALF_NUM )
            {
                Cup.SecondTargetTemp = (Cup.FirstTargetTemp * 2) - Cup.tAverage;
                // 앞 부분이 낮으면 +좀 해주고...
                if( Cup.FirstTargetTemp > Cup.tAverage )
                {
                    Cup.SecondTargetTemp += 1.0f;
                }
                // 앞 부분이 높으면 -좀 해주고
                else if( Cup.FirstTargetTemp < Cup.tAverage )
                {
                    Cup.SecondTargetTemp -= 1.0f;
                }

                // cut off max
                if( Cup.SecondTargetTemp > 98.0f )
                {
                    Cup.SecondTargetTemp = 98.0f;
                }

                SetHeaterTempTargetOut( Cup.SecondTargetTemp );

            }
            else if( Cup.SumCount == CUP_ONE_NUM )
            {
                SetHeaterTempTargetOut( Cup.FirstTargetTemp );
            }
        }
    }
}

// 추출량 카운트
static void UpdateHotCurrentAmount(void)
{
    CalcCupTemp();
    HotOut.CurrentAmount++;     
}

// 초기 배수 및 사용자 추출량
void UpdateHotInitCurrentAmount(void)
{
    HotOut.InitCurrentAmount++; 
}

static U8 IsDoneHotOut(void)
{
    // 단수 조건이면 종료한다.
    if( IsErrorType( ERR_TYPE_HOT_WATER_OUT ) == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

#define HOT_OUT_ALARAM_TIME         800U
U16 gu16SoundAlaram = HOT_OUT_ALARAM_TIME;  // 3sec @10ms..
static void InitHotOutSound(void)
{
    gu16SoundAlaram = HOT_OUT_ALARAM_TIME;  // 3sec @10ms..
}

static void HotOutSound(void)
{
    if( GetHotSelect() == SEL_HOT_USER )
    {
        if( gu16SoundAlaram != 0 )
        {
            gu16SoundAlaram--;
            if( gu16SoundAlaram == 1 )
            {
                Sound( SOUND_HOT_OUT_USER_OUT );
            }
        }
    }
}

// 물선택이 변경되었는지 확인해서 온수 재추출 조건을 클리어
static void CheckHotSelect(void)
{
    static U8 mu8Prev = SEL_HOT_USER;
    U8 mu8Select;

    mu8Select = GetHotSelect();
    if( mu8Prev != mu8Select )
    {
        mu8Prev = mu8Select;
        SetHotOutChangedSel();
    }
}

// 추출 제어
U8 ControlWaterHotOut(void)
{
    // 추출 상태 변경
    if( IsChangedOutStatus() == TRUE )
    {
        // 정지 상태이면, 추출 시작
        if( HotOut.Out == TRUE )
        {
            CheckHotSelect();

            // 추출 시작
            if( GetEolStatus() == TRUE )
            {
                StartHotOutEol();
            }
            else
            {
                StartHotOut();
            }
            InitOutEventList();
        }
        else
        {
            // 추출 중이였으면, 추출 중지
            // Update User Usage
            UpdateUserUsage( HotOut.CurrentAmount );

            StopHotOut();
            InitHotOutSound();
        }
    }


    // 목표 추출량 만큼만 추출하고 종료한다.
    if( HotOut.Out == TRUE )
    {
        if( IsDoneHotOut() == TRUE )
        {
            TurnOffHotOut();
            Sound(SOUND_EFFLUENT_END);
            return WATER_OUT_DONE;
        }
        else if( IsSetHotOutMode( HOT_OUT_DONE ) == TRUE )
        {
            TurnOffHotOut();
            Sound(SOUND_EFFLUENT_END);
            ResetHotOutLastedTimeOut();
            return WATER_OUT_DONE;
        }
        else if( IsError( ERR_HEATER_OVERHEAT ) == TRUE 
                && IsSetHotOutMode( HOT_OUT_COOLING ) == FALSE )
        {
            TurnOffHotOut();
            Sound(SOUND_EFFLUENT_END);
            ResetHotOutLastedTimeOut();
            return WATER_OUT_DONE;
        }
        else if( IsSetHotOutMode( HOT_OUT_RESET ) == TRUE )
        {
            return WATER_OUT_DONE;
        }

        HotOutSound();
    }


    UpdateHotOutTime();
    Outflow();
    return WATER_OUT_GOING;
}
