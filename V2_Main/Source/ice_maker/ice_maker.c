#include "ice_maker.h"

#include "ice.h"
#include "ice_full.h"
#include "ice_tray.h"
#include "icing_table.h"
#include "deicing_table.h"
#include "dummy_deicing_table.h"

#include "water_out.h"
#include "error.h"
#include "drain_water.h"
#include "ice_door.h"
#include "hot_water.h"

#include "temp.h"
#include "relay.h"
#include "valve.h"
#include "comp.h"
#include "gas_switch_valve.h"
#include "swing_bar.h"
#include "uv.h"
#include "flow_meter.h"
#include "fan_motor.h"
#include "water_out_common.h"
#include "process_make.h"

#define  CONFIG_MAKE_ONE_MORE       0       /* 얼음 한판 더 만들기 */

//#define  ICE_TRAY_FEED_TIME         60      /* 아이스 트레 입수 시간 : 10sec */

#define  ROOM_OUT_TIME_OUT_TIME     150      /* 추출 대기 타임 아웃 : 2min 30sec @1sec */
#define  GAS_SWITCH_TIME_OUT        300     /* 냉매 전환 밸브 대기 식나 : @1sec */


/* 얼음 크기 */
#define ICE_SMALL_RATIO       0.8f        // 80% 

/* COMP RPS */
#define COMP_ICING_RPS                     65U  // temp ambi under 30'c
#define COMP_ICING_RPS_OVER_TEMP_AMBI      60U  // temp ambi over 30'c

/* REDUCE COMP. DE-ICING NOISE */
#define TIME_REDUCE_DEICING_NOISE       300U    // 30sec @100ms


DoInit_T Init;
DoPreheat_T Preheat;
DoReady_T Ready;
DoIcing_T Icing;
DoDeIcing_T DeIcing;
DoDummyDeIcing_T DummyDeIcing;
DoDummyDeIcing_T TrayDeIcing;

IcingTemp_T IcingTemp;

//extern Ice_Turbo_T IceTurbo;


static U8 GetIcingRegion( TEMP_T _tAmbi );
static U8 GetIcingCompRps(U8 mu8Region);
static void StartIcing( U8 mu8Region );


void GetIceMakerInitData( DoInit_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Init, sizeof( DoInit_T ));
}

void GetIceMakerPreheatData( DoPreheat_T *pData )
{
     MEMCPY( (void __FAR *)pData, (const void __FAR *)&Preheat, sizeof( DoPreheat_T ));
}

void GetIceMakerReadyData( DoReady_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Ready, sizeof( DoReady_T ));
}

ReadyStep_T GetIceMakerReadyStep(void)
{
    return Ready.Step;
}


void GetIceMakerIcingData( DoIcing_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Icing, sizeof( DoIcing_T ));
}

void GetIceMakerDeIcingData( DoDeIcing_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&DeIcing, sizeof( DoDeIcing_T ));
}

//void GetIceMakerDummyDeIcingData( DoDummyDeIcing_T *pData )
//{
//    MEMCPY( (void __FAR *)pData, (const void __FAR *)&DummyDeIcing, sizeof( DoDummyDeIcing_T ));
//}

void SetIcingTime(U16 mu16Val )
{
    Icing.IcingTime = mu16Val;
}

void SetDeIcingTime(U16 mu16Val )
{
    DeIcing.OnOffTime = mu16Val;
}

// 더미 탈빙 중지
void StopDummyDeIcing(void)
{
    // 더미 탈빙 중이면, 마지막 스텝으로 이동한다.
    if( IsSetIceMakeMode( ICE_MODE_DUMMY_DEICING ) == TRUE )
    {
        DummyDeIcing.Step = STEP_DUMMY_DEICING_DONE_CONTROL;
    }

    // 트레이 더미 탈빙 중이면, 마지막 스텝으로 이동한다.
    if( IsSetIceMakeMode( ICE_MODE_TRAY_DEICING ) == TRUE )
    {
        TrayDeIcing.Step = STEP_TRAY_DEICING_DONE_CONTROL;
    }
}

void TimeShortIceMakerFeedTime(void)
{
    Ready.TrayFeedAmount = 1UL;
}

static void InitDoInit(void)
{
    Init.Step = 0; 
}

static void InitDoPreheat(void)
{
    Preheat.Step = 0; 
}

static void InitDoReady(void)
{
    Ready.Step = 0;
}

static void InitDoIcing(void)
{
    Icing.Step = 0;
}

static void InitDoDeIcing(void)
{
    DeIcing.Step = 0;
}

static void InitDoDummyDeIcing(void)
{
    DummyDeIcing.Step = 0;
}
static void InitDoTrayDeIcing(void)
{
    TrayDeIcing.Step = 0;
}

static U8 DoInit(void)
{
    U8 mu8Done = FALSE;


    switch( Init.Step )
    {
        case STEP_INIT_TRAY_MOVE:
            // 아이스 트레이 탈빙 위치 이동
            MoveIceTrayDeIcing();

            Init.Step++;
            break;

            /* 아이스 트레이 이동 완료 */
        case STEP_INIT_TRAY_DEICING:
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                Init.Step++;
            }
            break;

        case STEP_INIT_CHECK_CONDITION:
            // 1. 정수 만수위 
            // 2. 압축기 구속 시간 만료
            if( IsExpiredCompProtectTime() == TRUE )
            {
                Init.Step++;
            }
            break;

        case STEP_INIT_TRAY_DONE:
            mu8Done = TRUE;
            break;

        default:
            Init.Step = 0;
            break;
    }

    return mu8Done;
}

//#define PREHEAT_TIME        1800U    // 3min ( 3min x 60sec x 10 ) = 1800sec @100msec )
#define PREHEAT_TIME_14     6000U    // 10min ( 10min x 60sec x 10 ) = 6000sec @100msec )
#define PREHEAT_TIME_20     3600U    // 6min ( 6min x 60sec x 10 ) = 3600sec @100msec )
#define PREHEAT_TIME_25     3600U    // 6min ( 6min x 60sec x 10 ) = 3600sec @100msec )
#define PREHEAT_COMP_RPS    60U     // 60RPS
#define PREHEAT_BASE_TEMP   25.0f   // 예열이 필요한 구간의 기준 온도 ( 25도 미만 )
static U8 DoPreheat(void)
{
    U8 mu8Done = FALSE;

    TEMP_T  tAmbi;

    switch( Preheat.Step )
    {
        case STEP_PREHEAT_CHECK:
            // 외기온도 측정 후 10도 이상 시 Preheat 진행 안함
            if( IsValidDoIcingAmbiTempRange() == FALSE 
                || IsError( ERR_TEMP_AMBIENT ) == TRUE  )
            {
                ClearIceMakeMode( ICE_MODE_ALL );
                SetIceMakeMode( ICE_MODE_INIT );

                TurnOffComp( COMP_TYPE_ICE );

                Preheat.Step = STEP_PREHEAT_DONE;
            }
            else
            {
                tAmbi = GetTemp( TEMP_ID_AMBIENT );

                if( tAmbi < PREHEAT_BASE_TEMP )
                {
                    Preheat.Step++;
                }
                else
                {
                    Preheat.Step = STEP_PREHEAT_DONE;
                }
            }
            break;
    
        case STEP_PREHEAT_CHECK_COMP:
            // Comp 보호시간 체크
            if( IsExpiredCompProtectTime() == TRUE 
                    && IsExpiredHeaterOnTime() == TRUE )
            {
                Preheat.Step++;
            }
            break;

        case STEP_PREHEAT_INIT:
            // PREHEAT RPS SET
            // 핫가스 냉매 전환 FULL CLOSE
            Preheat.CompRps = PREHEAT_COMP_RPS;

            GasSwitchHotGas();

            Preheat.Step++;
            break;

        case STEP_PREHEAT_START:
            // COMP ON / PREHEAT TIME SET
            if( GetGasSwitchStatus() == GAS_SWITCH_HOTGAS )
            {
                TurnOnCompRps( COMP_TYPE_ICE, Preheat.CompRps );

                tAmbi = GetTemp( TEMP_ID_AMBIENT );
                if( tAmbi < 14.0f )
                {
                    Preheat.PreHeatTime = PREHEAT_TIME_14;
                }
                else if( tAmbi < 20.0f )
                {
                    Preheat.PreHeatTime = PREHEAT_TIME_20;
                }
                else
                {
                    Preheat.PreHeatTime = PREHEAT_TIME_25;
                }
                Preheat.Step++;
            }
            break;

        case STEP_PREHEAT_WAIT:
            // PREHEAT TIME WAIT
            if( Preheat.PreHeatTime == 0)
            {
                Preheat.Step++;
            }
            break;

        case STEP_PREHEAT_DONE:
            // 냉매전환밸브 / 핫가스 냉매 상태
            GasSwitchIce();

            mu8Done = TRUE;
            break;

        default:
            Preheat.Step = 0;
            break;
    }

    return mu8Done;
}

/* 유량 센서 카운터 */
static void UpdateIceTrayCurrentAmount(void)
{
    if( IsOpenValve( VALVE_ICE_TRAY_IN ) == TRUE 
            && Ready.TrayFeedAmount != 0UL )
    {
        Ready.TrayFeedAmount--;
    }
    else if( Ready.TrayFeedAmount == 0UL )
    {
        CloseValve( VALVE_ICE_TRAY_IN );
    }
}

static IsUsingWater(void)
{
    if( GetWaterOut() == TRUE )
    {
        return TRUE;
    }

    if( IsStartDrainWater() == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

static void StartFeedIceTray(void)
{
    OpenValve( VALVE_ICE_TRAY_IN );

    RegisterFlowMeterId( FLOW_ID_ICE_TRAY, UpdateIceTrayCurrentAmount );     // Register...
//    StartCheckOutage( OUTAGE_DELAY_TIME, OUTAGE_CHECK_TIME_10SEC, TYPE_ICE_TRAY );
}

static void StopFeedIceTray(void)
{
    CloseValve( VALVE_ICE_TRAY_IN );

    RegisterFlowMeterId( FLOW_ID_ICE_TRAY, NULL );     // Unregister...
//    StopCheckOutage();
}

/* 제빙 준비 */
static U8 DoReady(void)
{
    U8 mu8Done = FALSE;


    switch( Ready.Step )
    {
        case STEP_READY_CHECK_CONDITION:
			SetWifiIceMakeMode(STEP_ICE_MAKE_TRAY_IN_START);
		
            // 제빙 최소 운전 시간 초기화
            ResetDeIcingCompOnTime();

            if( IsExpiredCompProtectTime() == TRUE )
            {
                Ready.Step++;
            }
            break;

            /* 아이스 트레이 제빙 위치로 이동 시작 */
        case STEP_READY_TRAY_MOVE:
            MoveIceTrayIcing();

            Ready.Step++;
            break;

            /* 아이스 트레이 제빙 위치로 이동 완료 */
        case STEP_READY_TRAY_DONE:
            if( GetIceTrayStatus() == ICE_TRAY_ICING )
            {
                // 압축기 동작 중이면, 연속 운전 조건으로...
                // 그렇지 않으면 트레이 입수 동작으러 즉시 이동...
                if( IsTurnOnCompBldc() == TRUE )
                {
                    // GO TO ----  STEP_READY_TRAY_FEED_START
                    Ready.Step++; 
                }
                else
                {
                    // GO TO ----- SKIP STEP_READY_CHECK_CONTY_ICING
                    Ready.Step = STEP_READY_TRAY_FEED_START;
                }
            }
            break;


        case STEP_READY_CHECK_CONTY_ICING:
            // 냉매 전환 밸브 최대 동작 
            GasSwitchIce();
            Ready.TimeOut = GAS_SWITCH_TIME_OUT;    
            Ready.Step++;
            break;

        case STEP_READY_CHECK_CONTY_ICING_DONE:
            if( GetGasSwitchStatus() == GAS_SWITCH_ICE )
            {
                U8 mu8Region;

                mu8Region = GetIcingRegion( GetTemp( TEMP_ID_AMBIENT ));
                StartIcing( mu8Region );

                TurnOnCompRps( COMP_TYPE_ICE, Icing.CompRps );
                TurnOnFan( FAN_TYPE_ICE );

                Ready.Step++;
            }
            else if( Ready.TimeOut == 0 )
            {
                // 냉매 전환 밸브 대기 최대 동작 시가 초과...
                Ready.Step++;
            }
            break;

            /* 트레이 물 공급 시작 */
        case STEP_READY_TRAY_FEED_START:
			SetWifiIceMakeMode(STEP_ICE_MAKE_TRAY_IN_ING);
            if( IsUsingWater() == TRUE )
            {
                break;
            }

            Ready.TrayFeedAmount = GetFindOutTargetTime( OUT_AMOUNT_ICE_TRAY );
            TurnOnTempSensor( TEMP_ID_ROOM_WATER, ROOM_OUT_TIME_OUT_TIME );
            Ready.RoomOutTimeOut = ROOM_OUT_TIME_OUT_TIME;

            StartFeedIceTray();

            Ready.Step++;
            break;

            /* 트레이 물 공급 종료 조건 검사 */
        case STEP_READY_TRAY_FEED_DONE :
            
            // 단수이면 중지
#if 0
            if( GetOutageStatus( TYPE_ICE_TRAY ) == FLOW_OUTAGE )
            {
                // 제빙 취소...
                StopIceMake();
                StopFeedIceTray();
                break;
            }
#endif

            if( Ready.RoomOutTimeOut != 0 )
            {
                Ready.RoomOutTimeOut--;
            }
            else
            {
                if( GetFlowMeterLPM() < 0.1f )
                {
                    StartIceOutageTimer();
                }

                // 제빙 취소...
                StopIceMake();
                StopFeedIceTray();
                break;
            }

            // 추출, 냉수 입수, 온수 입수(식히는 동작) 중이면 대기...
            // 대기 시간 초과시 중지
            if( IsUsingWater() == TRUE 
                    || IsOpenValve( VALVE_HOT_IN ) == TRUE 
                    || IsOpenValve( VALVE_COLD_DRAIN ) == TRUE 
                    || IsOpenValve( VALVE_COLD_IN ) == TRUE )
            {
                CloseValve( VALVE_ICE_TRAY_IN );
                break;
            }

            // 입수 완료 
            OpenValve( VALVE_ICE_TRAY_IN );
            if( Ready.TrayFeedAmount == 0UL )
            {
                StopFeedIceTray();
                Ready.Step++;
            }
            break;

        case STEP_READY_GAS_SWITCH:
            GasSwitchIce();
            Ready.TimeOut = GAS_SWITCH_TIME_OUT;

            Ready.Step++;
            break;

        case STEP_READY_COMP_ON:
            if( GetGasSwitchStatus() == GAS_SWITCH_ICE )
            {
                U8 mu8Region;

                mu8Region = GetIcingRegion( GetTemp( TEMP_ID_AMBIENT ));
                StartIcing( mu8Region );

                TurnOnCompRps( COMP_TYPE_ICE, Icing.CompRps );
                TurnOnFan( FAN_TYPE_ICE );

                Ready.Step++;
            }
            else if( Ready.TimeOut == 0 )
            {
                Ready.Step++;
            }

            break;

        case STEP_READY_DONE:
            mu8Done = TRUE;
            break;

        default:
            Ready.Step = 0;
            break;
    }

    return mu8Done;
}


static U8 CheckIcingTemp(void)
{

    if( IsError( ERR_TEMP_AMBIENT ) == TRUE 
            || IsError( ERR_TEMP_ROOM_WATER ) == TRUE )
    {
        return FALSE;
    }

    if( IsValidDoIcingRoomTempRange() == FALSE )
    {
        return FALSE;
    }

    if( IsValidDoIcingAmbiTempRange() == FALSE )
    {
        return FALSE;
    }

     return TRUE;
}

static U16 GetIcingTimeSize(TEMP_T tRoom, TEMP_T tAmbi, U8 mu8Size )
{
    U16 mu16IcingTime;
    F32 ratio = 0.0f;



    mu16IcingTime = GetIcingTime( tRoom, tAmbi );
    if( mu8Size == ICE_SMALL )
    {
        mu16IcingTime = (U16)((F32)mu16IcingTime * ICE_SMALL_RATIO );
    }

    return mu16IcingTime;
}

static U8 GetIcingRegion( TEMP_T _tAmbi )
{
    return  GetDeIcingRegion( _tAmbi );
}

static U8 GetIcingCompRps(U8 mu8Region)
{
    if( mu8Region == DEICING_REGION_6 )
    {
        return COMP_ICING_RPS_OVER_TEMP_AMBI;
    }

    return COMP_ICING_RPS;
}

static void StartIcing( U8 mu8Region )
{
    Icing.Region  = mu8Region;

    Icing.CompRps = GetIcingCompRps( mu8Region );
}


/* 제빙 */
static U8 DoIcing(void)
{
    U8 mu8Done = FALSE;
    static U8 mu8Region;


    switch( Icing.Step )
    {
        case STEP_ICING_CHECK_COMP:
			SetWifiIceMakeMode(STEP_ICE_MAKE_MAKING_START);
            if( IsExpiredCompProtectTime() == TRUE
                    && IsExpiredHeaterOnTime() == TRUE  )
            {
                Icing.Step++;
            }
            break;

        case STEP_ICING_FIGUARE_OUT_TIME:

            if( IsValidDoIcingRoomTempRange() == FALSE 
                    || IsValidDoIcingAmbiTempRange() == FALSE 
                    || IsError( ERR_TEMP_AMBIENT ) == TRUE 
                    || IsError( ERR_TEMP_ROOM_WATER ) == TRUE )

            {
                ClearIceMakeMode( ICE_MODE_ALL );
                SetIceMakeMode( ICE_MODE_INIT );

                TurnOffComp( COMP_TYPE_ICE );
                TurnOffFan( FAN_TYPE_ICE );
                TurnOffSwingBar();

                Icing.Step = STEP_ICING_DONE;
            }
            else
            {
                U8 mu8Size;
                TEMP_T  tAmbi;
                TEMP_T  tRoom;

                tAmbi = GetTemp( TEMP_ID_AMBIENT );
                tRoom = GetTemp( TEMP_ID_ROOM_WATER );
                mu8Size = GetIceSize();

                Icing.IcingTime = GetIcingTimeSize( tRoom, tAmbi, mu8Size );
                SetSwingBarConfigOffTime( DEFAULT_SWING_BAR_TIME_OFF_1 );
                Icing.IcingTimeSecondPoint = Icing.IcingTime / 3;
                IcingTemp.tRoom = tRoom;
                IcingTemp.tAmbi = tAmbi;
                mu8Region = GetDeIcingRegion( tAmbi );

                GasSwitchIce();
                Icing.Step++;
            }
            break;

        case STEP_ICING:
			SetWifiIceMakeMode(STEP_ICE_MAKE_MAKING_ING);

            // 제빙 시간이 1/3을 지나는 시점 부터는 스윙바 시간을 변경
            if( Icing.IcingTimeSecondPoint == 0 )
            {
                SetSwingBarConfigOffTime( DEFAULT_SWING_BAR_TIME_OFF_2 );
            }

            if( Icing.IcingTime != 0 )
            {
                RPS_T   mRps = Icing.CompRps;

                // 탈빙 직전 COMP RPS를 탈빙 RPS로 전환 시킨다.
                // Reduce compressor de-icing noise
                if( Icing.IcingTime <= TIME_REDUCE_DEICING_NOISE )
                {
                    mRps = GetDeIcingCompRps( mu8Region );
                }

                TurnOnCompRps( COMP_TYPE_ICE, mRps );
                TurnOnFan( FAN_TYPE_ICE );
                TurnOnSwingBar();

                // 강제 탈빙 명령 처리를 위한 최소 제빙 시간 계산
                UpdateDeIcingCompOnTime();  
            }
            else
            {
                Icing.Step++;
                TurnOffSwingBar();
            }
            break;

        case STEP_ICING_DONE:
            mu8Done = TRUE;
            break;

        default:
            Icing.Step = 0;
            break;
    }

    return mu8Done;
}


static void StartDeIcing( U8 mu8Region )
{
    DeIcing.Region = mu8Region;

    DeIcing.Count      = 0;
    DeIcing.RepeatNum  = GetDeIcingRepeatNum( mu8Region );
    DeIcing.CompRps    = GetDeIcingCompRps( mu8Region );
    DeIcing.FanOnOff   = GetDeIcingFanOnOff( mu8Region );
}

static U8 MakeDeIcingOnOff(void)
{
    if( DeIcing.Count >= DeIcing.RepeatNum )
    {
        return FALSE;
    }

    DeIcing.OnOffCmd  = GetDeIcingCmd( DeIcing.Count );
    DeIcing.OnOffTime = GetDeIcingTime( DeIcing.Region, DeIcing.Count );
    DeIcing.CompRps   = GetDeIcingCompRps( DeIcing.Region );
    DeIcing.FanOnOff  = GetDeIcingFanOnOff( DeIcing.Region );

    DeIcing.Count++;

    return TRUE;
}

/* 탈빙 */
static U8 DoDeIcing(void)
{
    static U8 mu8CheckTrayError = FALSE;
    U8 mu8Region;
    U8 mu8Done = FALSE;
    TEMP_T  tAmbi;

    switch( DeIcing.Step )
    {
        case STEP_DEICING_TRAY_MOVE:
            mu8CheckTrayError = FALSE;

            /* 트레이 탈빙 위치 이동 */
            TurnOffSwingBar();
            MoveIceTrayDeIcing();
            DeIcing.Step++;
            break;

        case STEP_DEICING_TRAY_DONE:
            /* 트레이 이동에러 검사 */
            if( IsErrorIceTrayCoilFrezzing() == TRUE )
            {
                // 트레이 이동 에러 발생되면, 우선 탈빙은 진행함.
                
                mu8CheckTrayError = TRUE;
                DeIcing.Step++;
                break;
            }

            /* 트레이 제빙 위치 이동 완료 대기 */
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                // 에러 없이 트레이 이동 했으면, 적산 에러 카운트 초기화
                DeIcing.Step++;
            }

            break;

        case STEP_DEICING_FIGUARE_OUT_REGION:
            /* 탈빙 시간 계산 */
            if( IsError( ERR_TEMP_AMBIENT ) == TRUE 
                    || IsValidDoIcingAmbiTempRange() == FALSE )
            {
                tAmbi = IcingTemp.tAmbi;
            }
            else
            {
                tAmbi = GetTemp( TEMP_ID_AMBIENT );
            }
            mu8Region = GetDeIcingRegion( tAmbi );
            StartDeIcing( mu8Region );
            GasSwitchHotGas();

            TurnOnOffFan( FAN_TYPE_ICE, DeIcing.FanOnOff );
            //TurnOffFan( FAN_TYPE);
            TurnOnCompRps( COMP_TYPE_ICE, DeIcing.CompRps );

            // UV ON
            {
                U16 mu16OnTime;

                mu16OnTime = GetDeIcingTime( mu8Region, 0 );
                //TurnOnUvId( UV_ICE_TRAY_SHOT, 0, mu16OnTime );
            }

            DeIcing.Step++;
            break;

        case STEP_DEICING_COMMAND:
            /* 탈빙 시간 및 탈빙 히터 제어 명령 계산 */
            if( GetGasSwitchStatus() == GAS_SWITCH_HOTGAS )
            {
                if( MakeDeIcingOnOff() == TRUE )
                {

                    DeIcing.Step++;
                }
                else
                {
                    /* 탈빙 ON/OFF 동작이 완료되면, 만빙 검사 단계로 이동 */
                    DeIcing.Step = STEP_DEICING_DONE_CONTROL;
                }
            }
            break;

            
        case STEP_DEICING_CONTROL:
            if( DeIcing.OnOffTime == 0 )
            {
                /* 탈빙 시간 및 탈빙 히터 제어 명령 계산 */
                if( MakeDeIcingOnOff() == FALSE )
                {
                    /* 탈빙 ON/OFF 동작이 완료되면, 만빙 검사 단계로 이동 */
                    DeIcing.Step = STEP_DEICING_DONE_CONTROL;
                    break;
                }
            }

            if( DeIcing.OnOffTime != 0 )
            {
                if( DeIcing.OnOffCmd == ON )
                {
                    TurnOnCompRps( COMP_TYPE_ICE, DeIcing.CompRps );
                    TurnOnOffFan( FAN_TYPE_ICE, DeIcing.FanOnOff );
                    //TurnOffFan( FAN_TYPE_ICE );
                }
                else
                {
                    // Do Nothing...
                }
            }
            break;

        case STEP_DEICING_DONE_CONTROL:

            // ICE TRAY 이동 에러 발생.
            // 아이스 트레이 이동이 성공하면 카운트 초기화
            if( mu8CheckTrayError == TRUE )
            {
                if( GetIceTrayStatus() == ICE_TRAY_DEICING )
                {
                    ResetErrorIceTrayAccumCount();
                //    DeIcing.Step++;
                }
                TurnOffComp( COMP_TYPE_ICE );
                TurnOffFan( FAN_TYPE_ICE );

                DeIcing.Step++;
            }
            else
            {
                DeIcing.Step++;
            }

            break;
        case STEP_DEICING_FORWARD_FEEDER_START:
            // ??쉶???쇰뜑 
            if( IsOpenIceDoor() != TRUE )
            {
                TurnOnRelay( RELAY_FEEDER_CW );
                TurnOffRelay( RELAY_FEEDER_CCW );
                DeIcing.ForwardFeederTime = 2;     // 0.2s @100ms

                DeIcing.Step++;
            }
            else
            {
                DeIcing.ForwardFeederTime = 0;
                DeIcing.Step = STEP_DEICING_CHECK_ICE_FULL;
            }
            break;

        case STEP_DEICING_FORWARD_FEEDER_DONE:
            if( DeIcing.ForwardFeederTime != 0 )
            {
                // Wait
            }
            else
            {
                TurnOffRelay( RELAY_FEEDER_CW );
                TurnOffRelay( RELAY_FEEDER_CCW );
                DeIcing.Step++;
            }
            break;

#if CONFIG_BACK_FEEDER
        case STEP_DEICING_BACK_FEEDER_START:
            // 역회전 피더 
            if( IsOpenIceDoor() != TRUE )
            {
                TurnOffRelay( RELAY_FEEDER_CW );
                TurnOnRelay( RELAY_FEEDER_CCW );
                DeIcing.BackFeederTime = 3;     // 0.3s @100ms

                DeIcing.Step++;
            }
            else
            {
                DeIcing.BackFeederTime = 0;
                DeIcing.Step = STEP_DEICING_CHECK_ICE_FULL;
            }
            break;

        case STEP_DEICING_BACK_FEEDER_DONE:
            if( DeIcing.BackFeederTime != 0 )
            {
                // Wait
            }
            else
            {
                TurnOffRelay( RELAY_FEEDER_CW );
                TurnOffRelay( RELAY_FEEDER_CCW );
                DeIcing.Step++;
            }
            break;
#endif

        case STEP_DEICING_CHECK_ICE_FULL:

            // UV OFF
            //TurnOffUvId( UV_ICE_TRAY_SHOT );

            /* 만빙 검사 */
            StartCheckIceFull();

            DeIcing.Step++;
            break;

        case STEP_DEICING_WAIT_CHECK_ICE_FULL:
            // 2020.01.15, 만빙 후 한판 더 만들기
#if !CONFIG_MAKE_ONE_MORE
            if( IsDoneCheckIceFull() == TRUE )
#endif
            {
                DeIcing.Step++;
            }
            break;

        case STEP_DEICING_DONE:
		   UpdateMakingIceCount();
           mu8Done = TRUE; 
           break;

        default:
           DeIcing.Step = 0;
            break;
    }

    return mu8Done;
}

/* 더미 탈빙 */
static void StartDummyDeIcing( DoDummyDeIcing_T *pVal, U8 mu8Region )
{
    pVal->Region    = mu8Region;
    pVal->Count     = 0;
    pVal->RepeatNum = GetDeIcingRepeatNum( mu8Region );
    pVal->FanOnOff  = GetDeIcingFanOnOff( mu8Region );
}

static U8 MakeDummyDeIcingOnOff(DoDummyDeIcing_T *pVal)
{
    if( pVal->Count >= pVal->RepeatNum )
    {
        return FALSE;
    }

    pVal->OnOffCmd  = GetDeIcingCmd( pVal->Count );
    pVal->OnOffTime = GetDeIcingTime( pVal->Region, pVal->Count );
    pVal->CompRps   = GetDeIcingCompRps( pVal->Region );
    pVal->FanOnOff  = GetDeIcingFanOnOff( pVal->Region );
    pVal->Count++;

    return TRUE;
}


static U8 DoOffLoad(void)
{
    // 부하 OFF

    TurnOffSwingBar();
    TurnOffComp( COMP_TYPE_ICE );
    TurnOffFan( FAN_TYPE_ICE );
    StopFeedIceTray();
	
	SetWifiIceMakeMode(STEP_ICE_MAKE_STANDBY);

    return TRUE;
}

/* 제빙 부하 모두 OFF */
static U8 DoOff(void)
{
    // 더미 탈빙 중지
    // 제빙 중지
    DoOffLoad();

    ClearIceMakeMode( ICE_MODE_DUMMY_DEICING );
    ClearIceMakeMode( ICE_MODE_TRAY_DEICING );
    ClearIceMakeMode( ICE_MODE_ALL );

    return TRUE;
}


/* 더미 탈빙 */
static U8 DoDummyDeIcing(void)
{
    static U8 mu8CheckTrayError = FALSE;
    U8 mu8Region = 0;
    U8 mu8Done = FALSE;
    TEMP_T  tAmbi;


    switch( DummyDeIcing.Step )
    {
        case STEP_DUMMY_DEICING_GAS_SWITCH:
            /* 제빙 위치로 냉매 전환 밸브 이동 */
         //   GasSwitchIce();

            mu8CheckTrayError = FALSE;

            /* 제빙 부하 전부 OFF */
            TurnOffSwingBar();
            TurnOffComp( COMP_TYPE_ICE );
            TurnOffFan( FAN_TYPE_ICE );

            //TurnOffRelay( RELAY_ICE_HEATER );
            CloseValve( VALVE_ICE_TRAY_IN );

            DummyDeIcing.Step++;
            break;

        case STEP_DUMMY_DEICING_TRAY_MOVE: 
            /* 트레이 탈빙 위치로 이동 */
            MoveIceTrayDeIcing();
            DummyDeIcing.Step++;
            break;

        case STEP_DUMMY_DEICING_TRAY_DONE:
            /* 트레이 이동에러 검사 */
            if( IsErrorIceTrayCoilFrezzing() == TRUE )
            {
                mu8CheckTrayError = TRUE;
            }

            /* 트레이 이동 완료 대기 */
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                // 에러 없이 트레이 이동 했으면, 적산 에러 카운트 초기화
                if( mu8CheckTrayError == FALSE )
                {
                    ResetErrorIceTrayAccumCount();
                }

                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_COMP_PROTECT_TIME:
            /* 압축기 구속 시간 대기 */
            if( IsExpiredCompProtectTime() == TRUE 
                    && IsExpiredHeaterOnTime() == TRUE )
            {
                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_FIGUARE_OUT_REGION:

            if( IsError( ERR_TEMP_AMBIENT ) == TRUE )
            {
                /* 압축기 운전 시간 계산 */
                DummyDeIcing.Step = STEP_DUMMY_DEICING_DONE_CONTROL;
            }
            else
            {
                /* 압축기 운전 시간 계산 */
                tAmbi = GetTemp( TEMP_ID_AMBIENT );
                mu8Region = GetDeIcingRegion( tAmbi );
                StartDummyDeIcing( &DummyDeIcing, mu8Region );

                DummyDeIcing.CompOnTime = GetDummyDeIcingCompOnTime( mu8Region );

                GasSwitchIce();

                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_COMP_ON:
            /* 압축기 운전 */
            if( DummyDeIcing.CompOnTime == 0 )
            {
                DummyDeIcing.Step++;
            }
            else
            {
                U8 mu8Region;

                mu8Region = GetIcingRegion( GetTemp( TEMP_ID_AMBIENT ));
                StartIcing( mu8Region );
                TurnOnCompRps( COMP_TYPE_ICE, Icing.CompRps );
                TurnOnOffFan( FAN_TYPE_ICE, DeIcing.FanOnOff );
                //TurnOffFan();
                TurnOffFan( FAN_TYPE_ICE );
            }
            break;

        case STEP_DUMMY_DEICING_COMMAND:
            /* 탈빙 시간 및 탈빙 히터 제어 명령 계산 */
            if( MakeDummyDeIcingOnOff( &DummyDeIcing ) == TRUE )
            {
                GasSwitchHotGas();
                TurnOnCompRps( COMP_TYPE_ICE, DummyDeIcing.CompRps );
                TurnOnOffFan( FAN_TYPE_ICE, DummyDeIcing.FanOnOff );
                TurnOffFan( FAN_TYPE_ICE );

                DummyDeIcing.Step++;
            }
            else
            {
                /* 탈빙 ON/OFF 동작이 완료되면, 만빙 검사 단계로 이동 */
                DummyDeIcing.Step = STEP_DUMMY_DEICING_DONE_CONTROL;
            }
            break;

            
        case STEP_DUMMY_DEICING_CONTROL:
            /* 탈빙 히터 제어 */
            if( DummyDeIcing.OnOffTime == 0 )
            {
                if( MakeDummyDeIcingOnOff( &DummyDeIcing ) == FALSE )
                {
                    DummyDeIcing.Step = STEP_DUMMY_DEICING_DONE_CONTROL;
                    break;
                }
            }

            if( DummyDeIcing.OnOffTime != 0 )
            {
                if( DummyDeIcing.OnOffCmd == ON )
                {
                    TurnOnCompRps( COMP_TYPE_ICE, DummyDeIcing.CompRps );
                    TurnOnOffFan( FAN_TYPE_ICE, DummyDeIcing.FanOnOff );
                    TurnOffFan( FAN_TYPE_ICE );
                }
                else
                {
                    // Do Nothing...
                }
            }
            break;

        case STEP_DUMMY_DEICING_DONE_CONTROL:
            /* 다시 한 번 부하 OFF */
            DummyDeIcing.Step++;
            break;

        case STEP_DUMMY_DEICING_DONE:
           mu8Done = TRUE; 
            break;

        default:
            DummyDeIcing.Step = 0;
            break;
    }

    return mu8Done;
}

static U8 DoTrayDeIcing(void)
{
    U8 mu8Region = 0;
    U8 mu8Done = FALSE;


    switch( TrayDeIcing.Step )
    {
        case STEP_TRAY_DEICING_INIT:
            /* 제빙 부하 전부 OFF */
            TurnOffSwingBar();
            TurnOffComp( COMP_TYPE_ICE );
            TurnOffFan( FAN_TYPE_ICE );
            CloseValve( VALVE_ICE_TRAY_IN );

            /* 냉매 전환 밸브 핫가스로 변경 */
            GasSwitchHotGas();

            TrayDeIcing.Step++;
            break;

#if 0
        case STEP_TRAY_DEICING_TRAY_MOVE: 
            MoveIceTrayDeIcing();
            TrayDeIcing.Step++;
            break;

        case STEP_TRAY_DEICING_TRAY_DONE: 
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                TrayDeIcing.Step++;
            }
            break;
#endif

        case STEP_TRAY_DEICING_COMP_PROTECT_TIME:
            /* 압축기 구속 시간 대기 */
            if( IsExpiredCompProtectTime() == TRUE 
                    && IsExpiredHeaterOnTime() == TRUE )
            {
                TrayDeIcing.Step++;
            }
            break;

        case STEP_TRAY_DEICING_FIGUARE_OUT_REGION:
            if( IsError( ERR_TEMP_AMBIENT ) == TRUE )
            {
                /* 압축기 운전 시간 계산 */
                TrayDeIcing.Step = STEP_TRAY_DEICING_DONE_CONTROL;
            }
            else
            {
                TEMP_T  tAmbi;

                /* 압축기 운전 시간 계산 */
                tAmbi = GetTemp( TEMP_ID_AMBIENT );
                mu8Region = GetDeIcingRegion( tAmbi );
                StartDummyDeIcing( &TrayDeIcing, mu8Region );

                TrayDeIcing.Step++;
            }
            break;

        case STEP_TRAY_DEICING_CHECK_HOTGAS:
            GasSwitchHotGas();
            if( GetGasSwitchStatus() == GAS_SWITCH_HOTGAS )
            {
                TrayDeIcing.Step++;
            }
            break;

        case STEP_TRAY_DEICING_COMMAND:
            /* 탈빙 시간 및 탈빙 히터 제어 명령 계산 */
            if( MakeDummyDeIcingOnOff( &TrayDeIcing ) == TRUE )
            {
                GasSwitchHotGas();
                TurnOnCompRps( COMP_TYPE_ICE, TrayDeIcing.CompRps );
                TurnOnOffFan( FAN_TYPE_ICE, TrayDeIcing.FanOnOff );
                TurnOffFan( FAN_TYPE_ICE );

                TrayDeIcing.Step++;
            }
            else
            {
                /* 탈빙 ON/OFF 동작이 완료되면, 만빙 검사 단계로 이동 */
                TrayDeIcing.Step = STEP_TRAY_DEICING_DONE_CONTROL;
            }
            break;

            
        case STEP_TRAY_DEICING_CONTROL:
            /* 탈빙 히터 제어 */
            if( TrayDeIcing.OnOffTime == 0 )
            {
                if( MakeDummyDeIcingOnOff( &TrayDeIcing ) == FALSE )
                {
                    TrayDeIcing.Step = STEP_TRAY_DEICING_DONE_CONTROL;
                    break;
                }
            }

            if( TrayDeIcing.OnOffTime != 0 )
            {
                if( TrayDeIcing.OnOffCmd == ON )
                {
                    TurnOnCompRps( COMP_TYPE_ICE, TrayDeIcing.CompRps );
                    TurnOnOffFan( FAN_TYPE_ICE, TrayDeIcing.FanOnOff );
                    TurnOffFan( FAN_TYPE_ICE );
                }
                else
                {
                    // Do Nothing...
                }
            }
            break;

        case STEP_TRAY_DEICING_DONE_CONTROL:
            /* 다시 한 번 부하 OFF */
            TurnOffComp( COMP_TYPE_ICE );
            TurnOffFan( FAN_TYPE_ICE );

            TrayDeIcing.Step++;
            break;

        case STEP_TRAY_DEICING_DONE:
           mu8Done = TRUE; 
            break;

        default:
            TrayDeIcing.Step = 0;
            break;
    }

    return mu8Done;

}


typedef void(*InitAction_T)(void);
typedef U8(*DoAction_T)(void);
typedef struct _ice_maker_event_
{
    U16 Mode;
    
    InitAction_T  DoInit;
    DoAction_T  DoAction;
} IceMakerEvent_T;

static IceMakerEvent_T  IceMakerEventList[] = 
{
    { ICE_MODE_OFF_LOAD,            NULL,                DoOffLoad },      // 부하 OFF
    { ICE_MODE_OFF,                 NULL,                DoOff },          // 에러 종료.
    { ICE_MODE_DUMMY_DEICING,       InitDoDummyDeIcing,  DoDummyDeIcing }, // 더미 탈빙
    { ICE_MODE_TRAY_DEICING,        InitDoTrayDeIcing,   DoTrayDeIcing  }, // 트레이 해제 탈빙

    { ICE_MODE_INIT,                InitDoInit,          DoInit },         // 트레이 위치 초기화 
    { ICE_MODE_PREHEAT,             InitDoPreheat,       DoPreheat },      // 탈빙 성능 향상을 위해 COMP 예열
    { ICE_MODE_READY,               InitDoReady,         DoReady },        // 제빙 위치 이동, 트레이 입수
    { ICE_MODE_ICING,               InitDoIcing,         DoIcing },        // 제빙 
    { ICE_MODE_DEICING,             InitDoDeIcing,       DoDeIcing },      // 탈빙
    { ICE_MODE_DEICING_OFF,         NULL,                DoOffLoad }           // 부하 종료
};
#define SZ_ICE_MAKER_EVENT_LIST  (sizeof(IceMakerEventList)/sizeof(IceMakerEvent_T))

void ProcessIceMaker(void)
{
    U8 i;
    U8 mu8Mode;
    U8 mu8Ret;
    IceMakerEvent_T  *pList = NULL;
    U8 mu8Init = FALSE;

    /* 모드에 따른 DoAction은 오직 한 모드만 동작하고, 
     * 그외에 모드들에 대해서는 Init 함수를 호출해서 각 모드들에 대한
     * switch 문의 step을 0으로 초기화 시킨다
     *
     * 이렇게 하는 이유는 각 모드를 제어를 함에 있어 step으 0으로 초기화 되지 않는 경우
     * 오동작을 방지하기 위해서이다.
     * 더미 탈빙, 또는 ice tray recovery 동작 과정에서 ClearIceMakeMode() 함수에 의해
     * 모드가 정상적인 시퀀스에 의하지 않고 clear되는 경우 문제가 발생될 수 있기 때문이다.
     */

    for( i = 0; i < SZ_ICE_MAKER_EVENT_LIST ; i++ )
    {
        pList = &IceMakerEventList[ i ];

        if( mu8Init == TRUE  
                || IsSetIceMakeMode( pList->Mode ) == FALSE )
        {
            if( pList->DoInit != NULL )
            {
                pList->DoInit();
            }
        }
        else
        {
            mu8Init = TRUE;
            mu8Ret = FALSE;
            if( pList->DoAction != NULL )
            {
                mu8Ret = pList->DoAction();
            }

            if( mu8Ret == TRUE )
            {
                ClearIceMakeMode( pList->Mode );
                if( pList->DoInit != NULL )
                {
                    pList->DoInit();
                }
            }
        }
    }


    // Protect Ice Maker
    if( IsOverCompProtectSecondOnTime() == TRUE )
    {
        SetIceMakeMode( ICE_MODE_OFF );
    }
}

void UpdateIceMakerTimer(void)
{
    if( Preheat.PreHeatTime != 0 )
    {
        Preheat.PreHeatTime--;
    }
    // Ready Step Timer
    if( Ready.TimeOut != 0 )
    {
        Ready.TimeOut--;
    }

    // 제빙 타이머
    if( Icing.IcingTime != 0 )
    {
        Icing.IcingTime--;
    }

    // 제빙 타이머
    if( Icing.IcingTimeSecondPoint != 0 )
    {
        Icing.IcingTimeSecondPoint--;
    }

    // 탈빙 타이머
    if( DeIcing.OnOffTime != 0 )
    {
        DeIcing.OnOffTime--;
    }
	
    if( DeIcing.ForwardFeederTime != 0 )
    {
        DeIcing.ForwardFeederTime--;
		if(DeIcing.ForwardFeederTime == 0)
		{
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );			
		}
    }

#if CONFIG_BACK_FEEDER
    if( DeIcing.BackFeederTime != 0 )
    {
        DeIcing.BackFeederTime--;
		if(DeIcing.BackFeederTime == 0)
		{
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );			
		}
    }
#endif
    
    // 더미 탈빙 타이머
    if( DummyDeIcing.OnOffTime != 0 )
    {
        DummyDeIcing.OnOffTime--;
    }

    if( DummyDeIcing.CompOnTime != 0 )
    {
        DummyDeIcing.CompOnTime--;
    }
    if( TrayDeIcing.OnOffTime != 0 )
    {
        TrayDeIcing.OnOffTime--;
    }
}
