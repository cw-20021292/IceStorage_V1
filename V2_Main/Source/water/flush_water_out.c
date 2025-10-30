#include "hw.h"
#include "flush_water_out.h"
#include "valve.h"
#include "flow_meter.h"
#include "user.h"
#include "level.h"
#include "eeprom.h"
#include "front.h"
#include "water_out_common.h"
#include "hot_water.h"
#include "hot_water_out.h"
#include "hot_water_heater.h"
#include "heater_out.h"
#include "flow_valve.h"
#include "WIFI_Control.h"
#include "sound.h"
#include "drain_pump.h"
#include "cold_water.h"
#include "error.h"
#include "filter.h"


// ????��?�� 조건 ?���? ?��?��.
// ?��?��?��??? 3�?
//


FlushOut_T   FlushOut;


static void InitOutEventList(void);
static void UpdateFlushCurrentAmount(void);
static U32 GetFindOutTargetTime( WaterOutType_T mType);


#if 0   
// ?��?�� ?��?��?�� 목표 ?���?... @10ms..
#define FLUSH_AMOUNT_FILTER       18000UL // ( 3min * 60ses * 100 )  // ?��?�� ?��?��?�� ?���? + ?��?�� 채�?? ?���?
#define FLUSH_AMOUNT_HEATER        3000UL // ( 30sec * 100 )         // ?��?�� ?��?�� 채�?? ?���? 
#define FLUSH_AMOUNT_COOLER        6000UL // ( 1min * 60sec * 100 )  // ?��?�� ?��?�� 채�?? ?���?
#endif

#define VALVE_DELAY_TIME            10U

// ?��?�� ?��?��?�� 최�?? ?��?�� ?���?
#define MAX_FILTER_TIME_OUT         ( 5U * 60U * 100U ) // 5min * 60sec * 100ms 

// ?��?�� ?��?�� 채우�? 최�?? ?��?�� ?���?
#define MAX_COLD_TIME_OUT           ( 3U * 60U * 100U ) // 3min * 60sec * 100ms 

#define MAX_FLUSH_COLD_TIME_OUT     ( 5U * 60U * 100U ) // 5min * 60sec * 100ms 

// ?���? ?���? ???�? 
#define MAX_DONE_WAIT_TIME          ( 1U * 100U )      // 1sec * 100ms 


const static WaterOutTable_T FlushTableList[] = 
{
    { OUT_AMOUNT_FLUSH_FILTER,          AMOUNT_3000  },
    //{ OUT_AMOUNT_FLUSH_HEATER,          AMOUNT_250   },
    { OUT_AMOUNT_FLUSH_HEATER,          AMOUNT_500   },
    { OUT_AMOUNT_FLUSH_COOLER,          AMOUNT_1000  },
    { OUT_AMOUNT_FLUSH_COOLER_DRAIN,    AMOUNT_5000 },
    { OUT_AMOUNT_FLUSH_CHECK_HEATER,    AMOUNT_500   },
    { OUT_AMOUNT_CHECK_HEATER,          AMOUNT_120   }
};

#define SZ_FLUSH_TABLE   (sizeof(FlushTableList)/sizeof(WaterOutTable_T))
#define DEFAULT_AMOUNT  0UL

// ?��?���? 추출 ?��간을 반환?��?��.
static U32 GetFindOutTargetTime( WaterOutType_T mType)
{
    U8 i;

    for( i = 0; i < SZ_FLUSH_TABLE ; i++ )
    {
        if( FlushTableList[ i ].Type == mType )
        {
            return FlushTableList[ i ].Amount;
        }
    }

    return DEFAULT_AMOUNT;
}


void InitFlushOut(void)
{
    FlushOut.SetupInit = FLUSH_STATUS_INIT;     // 초기 ?���?
    FlushOut.Status    = FLUSH_STATUS_INIT;     // ?��?�� 교체 
    FlushOut.PowerOn   = FLUSH_STATUS_INIT;     // ?��?��?��?��?�� ?��?�� ?��?�� 발생?�� ?��?��?��?��  리셋
    FlushOut.Out       = FALSE;
    FlushOut.PrevOut   = FALSE;

    FlushOut.TargetAmount       = 0UL;
    FlushOut.FilterTargetAmount = 0UL;
    FlushOut.FlushColdTargetAmount = 0UL;
    FlushOut.CurrentAmount      = 0UL;

    FlushOut.Mode = 0U;
}


// FILTER SETUP FLUSHING STATUS
void SetFlushSetupInit(FlushStatus_T mu8Status)
{
    FlushOut.SetupInit = mu8Status;
}

FlushStatus_T GetFlushSetupInit(void)
{
    return FlushOut.SetupInit;
}


// FILTER FLUSHING STATUS
void SetFlushStatus(FlushStatus_T mu8Status)
{
    FlushOut.Status = mu8Status;
}

FlushStatus_T GetFlushStatus(void)
{
    return FlushOut.Status;
}


// POWER ON FLUSHING STATUS
void SetFlushPowerOn(FlushStatus_T mu8Status)
{
    FlushOut.PowerOn = mu8Status;
}

FlushStatus_T GetFlushPowerOn(void)
{
    return FlushOut.PowerOn;
}


U32 GetFlushCurrentAmount(void)
{
    return FlushOut.CurrentAmount;
}

void SetFlushCurrentAmount(U32 mu32Amount)
{
    FlushOut.CurrentAmount = mu32Amount;
}

// 초기 ?���? �? ?��?�� ?��?��?��
U8 IsDoneFlushStatus(void)
{
    if( GetFlushStatus() == FLUSH_STATUS_DONE
            && GetFlushSetupInit() == FLUSH_STATUS_DONE 
      )
    {
        return TRUE;
    }

    return FALSE;
}


// 초기 ?���? & ?��?�� ?��?��?�� & 초기?�� 배수
U8 IsDoneFlushInitSetup(void)
{
    if( IsDoneFlushStatus() == TRUE 
            && GetFlushPowerOn() == FLUSH_STATUS_DONE )
    {
        return TRUE;
    }

    return FALSE;
}

void ResetFlushWaterOut(void)
{
    FlushOut.Status = FLUSH_STATUS_INIT;
    FlushOut.CurrentAmount = 0UL;

    SaveEepromId( EEP_ID_FLUSH_STATUS );
}

// ?��?�� ?��?�� 모드 
void SetFlushOutMode( U16 mu16Mode )
{
    FlushOut.Mode |= mu16Mode;
}

void ClearFlushOutMode( U16 mu16Mode )
{
    FlushOut.Mode &= ~mu16Mode;
}

U8 IsSetFlushOutMode(U16 mu8Mode )
{
    if( (FlushOut.Mode & mu8Mode ) == mu8Mode )
    {
        return TRUE;
    }

    return FALSE;
}


U16 GetFlushOutMode(void )
{
    return FlushOut.Mode;
}


#if 0
void TurnOnFlushOut(WaterOutType_T mType)
{
    (void)mType;

    FlushOut.Out          = TRUE;

    // ?��?�� ?��?��?�� 목표?��(?��?�� ?��?��) 계산
    // ?��?�� ?��?��?�� + ?��?�� ?��?�� 채우�? + ?��?�� ?��?�� 채우�?
    FlushOut.TargetAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_FILTER );
    FlushOut.TargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );

    // 최초 ?��치시?���? ?��?�� 채우�? ?��?��
    if( FlushOut.SetupInit == FLUSH_STATUS_INIT )
    {
        FlushOut.TargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_COOLER );
    }

    // ?��?�� ?��?��?�� + ?��?�� ?��?�� 채우�? ?���? 계산
    FlushOut.FilterTargetAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_FILTER );
    FlushOut.FilterTargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );
}
#endif
void TurnOnFlushOut(WaterOutType_T mType)
{
    (void)mType;

    FlushOut.Out          = TRUE;

    if( FlushOut.SetupInit == FLUSH_STATUS_INIT )
    {
        // ?��?�� ?��?��?�� + ?��?�� ?��?�� 채우�? + ?��?�� ?��?�� 채우�?
        FlushOut.TargetAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_FILTER );
        FlushOut.TargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );
        FlushOut.TargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_COOLER );
        FlushOut.TargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_COOLER_DRAIN );

        // ?��?�� ?��?��?�� + ?��?�� ?��?�� 채우�? ?���? 계산
        FlushOut.FilterTargetAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_FILTER );
        FlushOut.FilterTargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );

        // ?��?�� ?��?��?�� + ?��?�� ?��?�� + ?��?�� 채우�? ?���? 계산
        FlushOut.FlushColdTargetAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_FILTER );
        FlushOut.FlushColdTargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );
        FlushOut.FlushColdTargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_COOLER );
    }
    else if( FlushOut.Status == FLUSH_STATUS_INIT )
    {
        // ?��?�� ?��?��?�� + ?��?�� ?��?�� 채우�? + ?��?�� ?��?�� 채우�?
        FlushOut.TargetAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_FILTER );
        FlushOut.TargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );
        FlushOut.TargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_COOLER );

        // ?��?�� ?��?��?�� + ?��?�� ?��?�� 채우�? ?���? 계산
        FlushOut.FilterTargetAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_FILTER );
        FlushOut.FilterTargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );
    }
    else 
    {
        if( GetSystem() == SYSTEM_CHP )
        {
            FlushOut.TargetAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );
            FlushOut.TargetAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_COOLER );

            FlushOut.FilterTargetAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );
        }
        else
        {
            FlushOut.TargetAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_COOLER );
        }
    }
}


U8 IsFlushOut(void)
{
    return FlushOut.Out;
}


void TurnOffFlushOut(void)
{
    FlushOut.Out = FALSE;
}


static U8 IsChangedOutStatus(void)
{
    if( FlushOut.Out != FlushOut.PrevOut )
    {
        FlushOut.PrevOut = FlushOut.Out;

        return TRUE;
    }

    return FALSE;
}

static void StartFlushOut(void)
{
    if( FlushOut.SetupInit == FLUSH_STATUS_DONE 
            && FlushOut.Status == FLUSH_STATUS_DONE )
    {
        // POWER ON
        if( GetSystem() == SYSTEM_CHP )
        {
            SetFlushOutMode( POWER_ON_CHP );
        }
        else
        {
            SetFlushOutMode( POWER_ON_CP );
        }
    }
    else
    {
        // FLUSH SETUP or FILTER
        if( GetSystem() == SYSTEM_CHP )
        {
            SetFlushOutMode( FLUSH_OUT_CHP );
        }
        else
        {
            SetFlushOutMode( FLUSH_OUT_CP );
        }

        // ?��?�� 교체?�� 경우 wifi paring, ?��?�� ?��?�� ?��?��?�� ?��?��
        if( FlushOut.SetupInit == FLUSH_STATUS_DONE )
        {
            ClearFlushOutMode( FLUSH_OUT_EXCEPT_SETUP_INIT );
            SetFlushOutMode( FLUSH_OUT_COLD );
        }
    }
}


static void StopFlushOut(void)
{
    ClearFlushOutMode( FLUSH_OUT_CANCEL );
    SetFlushOutMode( FLUSH_OUT_CLOSE_VALVE );
}



typedef U8 (*DoAction_T)(U8 *pStep);
typedef struct _flush_out_event_
{
    U16 Mode;               // 출수 ?��?�� ?��?�� ?��?�� 모드
    U8  Step;               // ?��벤트 ?��?��?�� ?��?�� �??��
    DoAction_T DoAction;    // ?��?�� 모드?�� ????�� ?��벤트 ?��?��?��
} FlushOutEvent_T;

static U8 DoParingWifi(U8 *pStep);
static U8 DoFilterFlushing_CHP(U8 *pStep);
static U8 DoFilterFlushing_CP(U8 *pStep);
static U8 DoFillHotWater(U8 *pStep);
static U8 DoFillColdWater(U8 *pStep);
static U8 DoFillColdFlushWater(U8 *pStep);
static U8 DoCloseValves(U8 *pStep);
static U8 DoDone(U8 *pStep);
static FlushOutEvent_T OutEventList[] = 
{
    { FLUSH_OUT_PARING_WIFI,    0U, DoParingWifi         },
    { FLUSH_OUT_FILTER,         0U, DoFilterFlushing_CHP },     // FILTER + HEATER ( HOT DRAIN )
    { FLUSH_OUT_FILTER_CP,      0U, DoFilterFlushing_CP  },     // FILTER + HEATER ( ROOM OUT )
    { FLUSH_OUT_HEATER,         0U, DoFillHotWater       },
    { FLUSH_OUT_COLD,           0U, DoFillColdWater      },     // FILLING COLD
    { FLUSH_OUT_FLUSH_COLD,     0U, DoFillColdFlushWater },     // FILLING COLD & FLUSH COLD
    { FLUSH_OUT_CLOSE_VALVE,    0U, DoCloseValves        },
    { FLUSH_OUT_DONE,           0U, DoDone               }
};
#define SZ_FLUSH_OUT_EVENT_LIST  (sizeof(OutEventList)/sizeof(FlushOutEvent_T))

static void InitOutEventList(void)
{
    U8 i;

    for( i = 0; i < SZ_FLUSH_OUT_EVENT_LIST ; i++ )
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
    FlushOutEvent_T *pList = NULL;

    for( i = 0; i < SZ_FLUSH_OUT_EVENT_LIST ; i++ )
    {
        pList = &OutEventList[ i ];

        if( IsSetFlushOutMode( pList->Mode ) == TRUE )
        {
            if( pList->DoAction != NULL )
            {
                mu8Ret = pList->DoAction( &pList->Step );
                if( mu8Ret == TRUE )
                {
                    ClearFlushOutMode( pList->Mode );
                }
                break;
            }
        }
    }
}

static void SoundFlushingDone(void)
{
    if( GetFlushSetupInit() == FALSE )
    {
        // ?���? ?��?��?�� 종료?��
        Sound( SOUND_SETUP_FLUSHED );
    }
    else if( GetFlushStatus() == FALSE )
    {
        // ?��?�� ?��?��?�� 종료?��
        Sound( SOUND_FILTER_FLUSHED );
    }
    else
    {
        // ?��?�� 채우�? 종료?��
        if( GetFotaReboot() == TRUE )
        {
            SetFotaReboot( FALSE );
        }
        else
        {
            Sound( SOUND_EFFLUENT_END );
        }
    }
}

// ?��?���? ?��?��
static U8 DoParingWifi(U8 *pStep)
{
    // WIFI ?��?��?�� 모드 ?��?��
    WifiKey(WIFI_KEY_BLE);

    // 초기 ?��?��그인 출력
    Sound( SOUND_SETUP_FLUSHING );

    return TRUE;
}

enum
{
    FLUSH_OPEN_VALVE,
    FLUSH_CHECK_OUTAGE,
    FLUSH_FILLING,
    FLUSH_DONE
};

// ?��?�� ?��?��?�� + ?��?�� ?��?�� 채우�? 3�?...
#define CHECK_HEATER_WATT_TIME   300U    // 3sec
static U8 DoFilterFlushing_CHP(U8 *pStep)
{
    static U16 mu16Time = 0U; // @10ms
    U32  mAmount;


    switch( *pStep )
    {
        case FLUSH_OPEN_VALVE:
            // open valve... hot water drain 
            OpenValve( VALVE_HOT_IN );
            OpenValve( VALVE_HOT_DRAIN );
            CloseValve( VALVE_HOT_OUT );

            mu16Time = VALVE_DELAY_TIME;
            (*pStep)++;
            break;

        case FLUSH_CHECK_OUTAGE:
            // Check outage..
            // 0.1�? ???�?...
            if( mu16Time > 0 )
            {
                mu16Time--;
            }
            else
            {
                FlushOut.FilterTimeOut  = MAX_FILTER_TIME_OUT;

                RegisterFlowMeterId( FLOW_ID_WATER, UpdateFlushCurrentAmount );
                StartCheckOutage( OUTAGE_FLUSHING_DELAY_TIME, OUTAGE_CHECK_TIME_FLUSHING, TYPE_HOT_WATER  );

                FlowValveInit();

                mu16Time = CHECK_HEATER_WATT_TIME;
                (*pStep)++;
            }
            break;


        case FLUSH_FILLING:
            if( FlushOut.FilterTimeOut != 0 )
            {
                FlushOut.FilterTimeOut--;
            }

            // ?��?�� ?��?��?�� ?��르면, ?��?�� ?��?�� �? ?��?�� ?��?�� 계산
            mAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_CHECK_HEATER );
            if( FlushOut.CurrentAmount >= mAmount )
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

                SetHotWaterInitFull( TRUE );
            }
            CheckHeaterWatt( GetHeaterOutput() );

            // done filter flush and fill heater 
            if( FlushOut.FilterTimeOut == 0 )
            {
                // 1. 최�?? ?��?�� ?���? ????�� ?��?��
                FlushOut.CurrentAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_FILTER );
                FlushOut.CurrentAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );

                SetHeaterOutput( 0 );
                (*pStep)++;
            }
            else if( FlushOut.CurrentAmount >= FlushOut.FilterTargetAmount )
            {
                // 2. 2. 목표 ?��?�� ?��?�� 
                SetHeaterOutput( 0 );
                (*pStep)++;
            }

            break;

        case FLUSH_DONE:
            SetHeaterOutput( 0 );

            UpdateRegionTempIn();
            StopCheckOutage();

            ResetFilterUsage();     // ?��?�� ?��?�� ?���? 초기?��
            FlowValveTarget( DEFAULT_FLOW_TARGET );
            CloseValve( VALVE_HOT_IN );
            (*pStep) = 0;
            return TRUE;
            break;

        default:
            (*pStep) = 0;

            break;
    }

    return FALSE;
}

static U8 DoFilterFlushing_CP(U8 *pStep)
{
    static U16 mu16Time = 0U; // @10ms


    switch( *pStep )
    {
        case FLUSH_OPEN_VALVE:
            OpenValve( VALVE_COLD_FLUSH );

            mu16Time = VALVE_DELAY_TIME;
            (*pStep)++;
            break;

        case FLUSH_CHECK_OUTAGE:
            // Check outage..
            // 0.1�? ???�?...
            if( mu16Time > 0 )
            {
                mu16Time--;
            }
            else
            {
                FlushOut.FilterTimeOut  = MAX_FILTER_TIME_OUT;

                RegisterFlowMeterId( FLOW_ID_WATER, UpdateFlushCurrentAmount );
                StartCheckOutage( OUTAGE_FLUSHING_DELAY_TIME, OUTAGE_CHECK_TIME_FLUSHING, TYPE_WATER );

                (*pStep)++;
            }
            break;


        case FLUSH_FILLING:
            if( FlushOut.FilterTimeOut != 0 )
            {
                FlushOut.FilterTimeOut--;
            }

            // done filter flush and fill heater 
            if( FlushOut.FilterTimeOut == 0 )
            {
                // 1. 최�?? ?��?�� ?���? ????�� ?��?��
                FlushOut.CurrentAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_FILTER );
                FlushOut.CurrentAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );

                (*pStep)++;
            }
            else if( FlushOut.CurrentAmount >= FlushOut.FilterTargetAmount )
            {
                // 2. 2. 목표 ?��?�� ?��?�� 
                (*pStep)++;
            }

            break;

        case FLUSH_DONE:
            CloseValve( VALVE_COLD_FLUSH );
            StopCheckOutage();
            ResetFilterUsage();     // ?��?�� ?��?�� ?���? 초기?��
            (*pStep) = 0;
            return TRUE;
            break;

        default:
            (*pStep) = 0;

            break;
    }

    return FALSE;
}

enum
{
    HOT_OPEN_VALVE,
    HOT_CHECK_OUTAGE,
    HOT_FILLING,
    HOT_DONE
};

static U8 DoFillHotWater(U8 *pStep)
{
    static U16 mu16Time = 0U; // @10ms
    U32  mAmount;


    switch( *pStep )
    {
        case HOT_OPEN_VALVE:
            // open valve... hot water drain 
            OpenValve( VALVE_HOT_IN );
            OpenValve( VALVE_HOT_DRAIN );
            CloseValve( VALVE_HOT_OUT );

            InitCheckHeaterWatt();

            mu16Time = VALVE_DELAY_TIME;
            (*pStep)++;
            break;

        case HOT_CHECK_OUTAGE:
            // Check outage..
            // 0.1�? ???�?...
            if( mu16Time > 0 )
            {
                mu16Time--;
            }
            else
            {
                FlushOut.FilterTimeOut  = MAX_FILTER_TIME_OUT;

                RegisterFlowMeterId( FLOW_ID_WATER, UpdateFlushCurrentAmount );
                StartCheckOutage( OUTAGE_FLUSHING_DELAY_TIME, OUTAGE_CHECK_TIME_10SEC, TYPE_HOT_WATER  );

                FlowValveInit();
                mu16Time = CHECK_HEATER_WATT_TIME;

                (*pStep)++;
            }
            break;


        case HOT_FILLING:
            if( FlushOut.FilterTimeOut != 0 )
            {
                FlushOut.FilterTimeOut--;
            }

            // ?��?�� ?��?��?�� ?��르면, ?��?�� ?��?�� �? ?��?�� ?��?�� 계산
            mAmount = GetFindOutTargetTime( OUT_AMOUNT_CHECK_HEATER );
            if( FlushOut.CurrentAmount >= mAmount )
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

                SetHotWaterInitFull( TRUE );
            }
            CheckHeaterWatt( GetHeaterOutput() );

            // done filter flush and fill heater 
            if( FlushOut.FilterTimeOut == 0 )
            {
                // 1. 최�?? ?��?�� ?���? ????�� ?��?��
                FlushOut.CurrentAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );

                SetHeaterOutput( 0 );
                (*pStep)++;
            }
            else if( FlushOut.CurrentAmount >= FlushOut.FilterTargetAmount )
            {
                // 2. 2. 목표 ?��?�� ?��?�� 
                SetHeaterOutput( 0 );
                (*pStep)++;
            }

            break;

        case HOT_DONE:
            StopCheckOutage();

            SetHeaterOutput( 0 );
            UpdateRegionTempIn();

            FlowValveTarget( DEFAULT_FLOW_TARGET );
            CloseValve( VALVE_HOT_IN );
            (*pStep) = 0;
            return TRUE;
            break;

        default:
            (*pStep) = 0;

            break;
    }

    return FALSE;
}



enum
{
    N_COLD_CHECK_WAIT,
    N_COLD_CHECK_LEVEL,
    N_COLD_WAIT_DRAIN_LEVEL,
    N_COLD_OPEN_VALVE,
    N_COLD_CHECK_OUTAGE,
    N_COLD_FILLING,
    N_COLD_LAZY_DONE,
    N_COLD_DONE
};


static void CloseFillColdWater(void)
{
    // INIT VALVES...
    StopCheckOutage();

    SetFlushOutMode( FLUSH_OUT_CLOSE_VALVE );

    UpdateHotOutLPM( GetFlowMeterLPM() );
    SetColdWaterInitFull( TRUE );

    CloseValve( VALVE_COLD_IN );
}

static U8 DoFillColdWater(U8 *pStep)
{
    static U16 mu16Time = 0U; // @10ms


    switch( *pStep )
    {
        case N_COLD_CHECK_WAIT:
            // ?��?�� ?��?�� 감�?? �??�� ???�? ?���?
            mu16Time = 400U;    // 4sec..

            (*pStep)++;
            break;

        case N_COLD_CHECK_LEVEL:
            if( mu16Time != 0 )
            {
                mu16Time--;
                break;
            }

            // 배수 ?��?�� �??�� ( 배수 ?��?�� ) 
            if( GetTankLevel( LEVEL_ID_DRAIN ) != LEVEL_LOW )
            {
                TurnOnDrainPump();
            }
            else 
            {
                CloseValve( VALVE_ROOM_COLD_OUT );
                OpenValve( VALVE_COLD_AIR );
            }

            (*pStep)++;
            break;

        case N_COLD_WAIT_DRAIN_LEVEL:
            // 배수 ?��?�� ?��?���? low�? ?��?��까�?? ???�?
            if( GetDrainPumpStatus() == OFF 
            || GetTankLevel( LEVEL_ID_DRAIN ) == LEVEL_LOW )
            {
                (*pStep)++;
            }
            break;

        case N_COLD_OPEN_VALVE:
            // fill cold water..
            OpenValve( VALVE_COLD_IN );
            CloseValve( VALVE_COLD_DRAIN );

            mu16Time = 10U;    // 100ms
            (*pStep)++;
            break;

        case N_COLD_CHECK_OUTAGE:
            if( mu16Time > 0 )
            {
                mu16Time--;
            }
            else
            {
                FlushOut.ColdTimeOut    = MAX_COLD_TIME_OUT;

                RegisterFlowMeterId( FLOW_ID_WATER, UpdateFlushCurrentAmount );
                StartCheckOutage( OUTAGE_FLUSHING_DELAY_TIME, OUTAGE_CHECK_TIME_10SEC, TYPE_WATER );

                (*pStep)++;
            }
            break;

        case N_COLD_FILLING:
            if( FlushOut.ColdTimeOut != 0 )
            {
                FlushOut.ColdTimeOut--;
            }

            // ?��?�� ?��?�� 만수?��?���? ?��?��?�� 종료
            if( GetTankLevel(LEVEL_ID_DRAIN) != LEVEL_LOW 
            || FlushOut.ColdTimeOut == 0 )
            {
                CloseFillColdWater();
                (*pStep)++;
            }
            else if( FlushOut.TargetAmount <= FlushOut.CurrentAmount )
            {
                (*pStep) = N_COLD_DONE;
            }

            break;


        case N_COLD_LAZY_DONE:
             // 즉시 종료?���? ?���?,CurrentAmount�? 100%�? 채우�? 종료?��.

            FlushOut.CurrentAmount += 10;
            if( FlushOut.TargetAmount <= FlushOut.CurrentAmount )
            {
                (*pStep)++;
            }
            break;

        case N_COLD_DONE:
            CloseFillColdWater();

            (*pStep) = 0;
            return TRUE;

            break;

        default:
            (*pStep) = 0;

            break;
    }

    return FALSE;
}

enum
{
    // ?��?�� ?��?�� 채우�?
    N_FLUSH_COLD_CHECK_WAIT,      
    N_FLUSH_COLD_CHECK_LEVEL,     
    N_FLUSH_COLD_WAIT_DRAIN_LEVEL,
    N_FLUSH_COLD_OPEN_VALVE,
    N_FLUSH_COLD_CHECK_OUTAGE,
    N_FLUSH_COLD_FILLING,

    // ?��?�� ?��?�� ?��?��?��
    N_FLUSH_COLD_READY,      // 배수 �?�?
    N_FLUSH_COLD,
    N_FLUSH_COLD_COLD,
    N_FLUSH_COLD_CLOSE_DRAIN,
    N_FLUSH_COLD_LAZY_DONE,
    N_FLUSH_COLD_DONE,
};

// ?��?�� 채우�? & ?��?�� ?��?�� ?��?��?��
static U8 DoFillColdFlushWater(U8 *pStep)
{
    /*?���? 배수?��?�� Step*/
    static U16 mu16Time = 0;
    U32 mAmount = 0;


    switch( *pStep )
    {
        case N_FLUSH_COLD_CHECK_WAIT:
            // ?��?�� ?��?�� 감�?? �??�� ???�? ?���?
            mu16Time = 400U;    // 4sec..

            (*pStep)++;
            break;

        case N_FLUSH_COLD_CHECK_LEVEL:
            if( mu16Time != 0 )
            {
                mu16Time--;
                break;
            }

            // 배수 ?��?�� �??�� ( 배수 ?��?�� ) 
            if( GetTankLevel( LEVEL_ID_DRAIN ) != LEVEL_LOW )
            {
                TurnOnDrainPump();
            }
            else 
            {
                CloseValve( VALVE_ROOM_COLD_OUT );
                OpenValve( VALVE_COLD_AIR );
            }

            (*pStep)++;
            break;

        case N_FLUSH_COLD_WAIT_DRAIN_LEVEL:
            // 배수 ?��?�� ?��?���? low�? ?��?��까�?? ???�?
            if( GetDrainPumpStatus() == OFF 
                    || GetTankLevel( LEVEL_ID_DRAIN ) == LEVEL_LOW )
            {
                (*pStep)++;
            }
            break;

        case N_FLUSH_COLD_OPEN_VALVE:
            // fill cold water..
            OpenValve( VALVE_COLD_IN );
            CloseValve( VALVE_COLD_DRAIN );

            mu16Time = 10U;    // 100ms
            (*pStep)++;
            break;

        case N_FLUSH_COLD_CHECK_OUTAGE:
            if( mu16Time > 0 )
            {
                mu16Time--;
            }
            else
            {
                FlushOut.ColdTimeOut    = MAX_COLD_TIME_OUT;

                RegisterFlowMeterId( FLOW_ID_WATER, UpdateFlushCurrentAmount );
                StartCheckOutage( OUTAGE_FLUSHING_DELAY_TIME, OUTAGE_CHECK_TIME_10SEC, TYPE_WATER );

                (*pStep)++;
            }
            break;

        case N_FLUSH_COLD_FILLING:
            if( FlushOut.ColdTimeOut != 0 )
            {
                FlushOut.ColdTimeOut--;
            }

            mAmount = GetFindOutTargetTime( OUT_AMOUNT_FLUSH_FILTER );
            mAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_HEATER );
            mAmount += GetFindOutTargetTime( OUT_AMOUNT_FLUSH_COOLER );

            // ?��?�� ?��?�� 만수?��?���? ?��?��?�� 종료
            if( GetTankLevel(LEVEL_ID_DRAIN) != LEVEL_LOW 
                    || FlushOut.ColdTimeOut == 0  )
            {
                // ?��?�� ?��?�� ????�� ?��?��..
                FlushOut.CurrentAmount = mAmount;       
                (*pStep)++;
            }
            else if(  FlushOut.CurrentAmount >= mAmount )
            {
                (*pStep)++;
            }
            break;

            ///////////// FLUSH COLD WATER 
        case N_FLUSH_COLD_READY:
            // open drain valve
            OpenValve( VALVE_COLD_DRAIN );

            // ?��?�� �??�� ?��?�� ( ?���? �?�? )
            FlushOut.ColdTimeOut = MAX_FLUSH_COLD_TIME_OUT;

            mu16Time = WAIT_200MS;
            (*pStep)++;
            break;

        case N_FLUSH_COLD:
            // close air valve
            if( mu16Time != 0 )
            {
                mu16Time--;
            }
            else
            {
                StartCheckOutage( OUTAGE_FLUSHING_DELAY_TIME, OUTAGE_CHECK_TIME_60SEC, TYPE_WATER );

                CloseValve( VALVE_COLD_AIR );
                OpenValve( VALVE_COLD_IN );
                (*pStep)++;
            }
            break;

        case N_FLUSH_COLD_COLD:
            if( FlushOut.ColdTimeOut != 0 )
            {
                FlushOut.ColdTimeOut--;
            }

            // ?��?�� ?��?�� 비우�?
            CloseValve( VALVE_COLD_AIR );
            CloseValve( VALVE_ROOM_COLD_OUT );

            OpenValve( VALVE_COLD_DRAIN );
            OpenValve( VALVE_COLD_IN );

            // ?��?�� ?��?�� 만수?��?���? ?��?��?�� 종료
            if( FlushOut.ColdTimeOut == 0 )
            {
                // ?��?�� 밸브 먼�?? OPEN?���? �??�� ?���? ?��?�� ?��, COLD DRAIN 밸브�? CLOSE
                OpenValve( VALVE_COLD_AIR );
                mu16Time = 200; // 200ms...

                (*pStep)++;
            }
            else if( FlushOut.TargetAmount <= FlushOut.CurrentAmount )
            {
                OpenValve( VALVE_COLD_AIR );
                mu16Time = 200; // 200ms...

                (*pStep)++;
            }

            break;
            
        case  N_FLUSH_COLD_CLOSE_DRAIN:
            if( mu16Time != 0 )
            {
                mu16Time--;
                break;
            }

            if( FlushOut.ColdTimeOut == 0 )
            {
                CloseValve( VALVE_COLD_DRAIN );
                CloseFillColdWater();
                (*pStep)++;
            }
            else
            {
                CloseValve( VALVE_COLD_DRAIN );
                (*pStep) = N_FLUSH_COLD_DONE;
            }

            break;

        case N_FLUSH_COLD_LAZY_DONE:
             // 즉시 종료?���? ?���?,CurrentAmount�? 100%�? 채우�? 종료?��.

            FlushOut.CurrentAmount += 50;
            if( FlushOut.TargetAmount <= FlushOut.CurrentAmount )
            {
                (*pStep)++;
            }
            break;

        case N_FLUSH_COLD_DONE:
            CloseFillColdWater();

            (*pStep) = 0;
            return TRUE;

            break;

        default:
            (*pStep) = 0;
            break;
    }


    return FALSE;
}

enum
{
    CLOSE_INIT,
    CLOSE_VALVE,
    CLOSE_WAIT
};

static U8 DoCloseValves(U8 *pStep)
{
    switch( *pStep )
    {
        case CLOSE_INIT:
            FlushOut.DoneWaitTime = MAX_DONE_WAIT_TIME;

            (*pStep)++;
            break;

        case CLOSE_VALVE:
            // Close All Valvaes..
            CloseValve( VALVE_HOT_IN );
            OpenValve( VALVE_HOT_DRAIN );

            OpenValve( VALVE_COLD_AIR );
            CloseValve( VALVE_COLD_IN );

            CloseValve( VALVE_COLD_FLUSH );   // for CP model...

            CloseValve( VALVE_COLD_DRAIN );

            SetHeaterOutput( 0 );
            // Unreigster flow meter..
            RegisterFlowMeterId( FLOW_ID_WATER, NULL );


            (*pStep)++;
            break;

        case CLOSE_WAIT:
            if( FlushOut.DoneWaitTime != 0 )
            {
                FlushOut.DoneWaitTime--;
            }
            else
            {
                (*pStep) = 0;

                // FLUSING DONE... ( �??�� 초기?�� )
                if( FlushOut.TargetAmount <= FlushOut.CurrentAmount )
                {
                    SetFlushOutMode( FLUSH_OUT_DONE );

                }
                return TRUE;
            }


            break;

        default:
            (*pStep) = 0;
            break;
    }

    return FALSE;
}



static U8 DoDone(U8 *pStep)
{
    SoundFlushingDone();

    // ?��?�� ?��?��?�� ?���? 체크
    if( GetFlushSetupInit() == FLUSH_STATUS_DONE 
            && GetFlushStatus() == FLUSH_STATUS_INIT )
    {
        SetFilterChangedFlushStatus( FILTER_CHANGED_FLUSH );
    }

    // 최초 ?��?�� ?���?
    SetFlushSetupInit( FLUSH_STATUS_DONE );
    SaveEepromId( EEP_ID_FLUSH_SETUP_INIT );

    // ?��?��?�� ?���?
    SetFlushStatus( FLUSH_STATUS_DONE );
    SaveEepromId( EEP_ID_FLUSH_STATUS );

    // 채우�? ?���?
    SetFlushPowerOn( FLUSH_STATUS_DONE );

    // Select water room
    SetWaterOutSelect( SEL_WATER_ROOM );
    return TRUE;
}


// 추출?�� 반환 ( percent )
U8 GetFlushOutPercent(void)
{
    return CalcPercent( FlushOut.TargetAmount, FlushOut.CurrentAmount );
}

// 추출?�� ml ?��?�� 반환
U16 GetFlushOutAmount(void)
{
    return CalcLiter( FlushOut.CurrentAmount );
}


static void UpdateFlushCurrentAmount(void)
{
    FlushOut.CurrentAmount++;     
}

static U8 IsDoneFlushOut(void)
{
    // ?��?�� 조건?���? 종료?��?��.
    if( IsErrorType( ERR_TYPE_ROOM_WATER_OUT ) == TRUE )
    {
        return TRUE;
    }

    if( GetSystem() == SYSTEM_CHP )
    {
        if( IsError( ERR_OUTAGE_HOT_WATER ) == TRUE )
        {
            return TRUE;
        }
    }

    return FALSE;
}

// 추출 ?��?��
// RETURN 0x01 : 추출 ?���?
// 0x00 추출 �?...
U8 ControlWaterFlushOut(void)
{
    // 추출 ?��?�� �?�?
    if( IsChangedOutStatus() == TRUE )
    {
        // ?���? ?��?��?���?, 추출 ?��?��
        if( FlushOut.Out == TRUE )
        {
            // 추출 ?��?��
            StartFlushOut();
            InitOutEventList();
        }
        else
        {
            // 추출 중이????���?, 추출 중�??
            StopFlushOut();

            StopCheckOutage();
        }
    }


    // 목표 추출?�� 만큼�? 추출?���? 종료?��?��.
    if( FlushOut.Out == TRUE )
    {
        if( IsDoneFlushOut() == TRUE )
        {
            SetFlushCurrentAmount( 0UL );
            TurnOffFlushOut();
            return WATER_OUT_DONE;
        }
        else if( IsSetFlushOutMode( FLUSH_OUT_DONE ) == TRUE )
        {
            TurnOffFlushOut();
            return WATER_OUT_DONE;
        }
    }

    Outflow();
    return WATER_OUT_GOING;
}
