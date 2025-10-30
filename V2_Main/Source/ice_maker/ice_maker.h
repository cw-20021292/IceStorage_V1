#ifndef __ICE_MAKER_H__
#define __ICE_MAKER_H__

#include "prj_type.h"

typedef struct _ice_maker_temp_t
{
    TEMP_T  tRoom;
    TEMP_T  tAmbi;
} IcingTemp_T;

/* INIT */
typedef enum
{
    STEP_INIT_TRAY_MOVE,
    STEP_INIT_TRAY_DEICING,
    STEP_INIT_CHECK_CONDITION,
    STEP_INIT_TRAY_DONE
} InitStep_T;

typedef struct _do_init_
{
    InitStep_T Step;
} DoInit_T;
typedef enum
{
    STEP_PREHEAT_CHECK,
    STEP_PREHEAT_CHECK_COMP,
    STEP_PREHEAT_INIT,
    STEP_PREHEAT_START,
    STEP_PREHEAT_WAIT,
    STEP_PREHEAT_DONE
} PreheatStep_T;
typedef struct _do_preheat_
{
    PreheatStep_T Step;
    U16 PreHeatTime;        // Preheat Time
    U8  CompRps;            // Rps of Comp On
} DoPreheat_T;

/* READY */
typedef enum
{
    STEP_READY_CHECK_CONDITION,

    STEP_READY_TRAY_MOVE,
    STEP_READY_TRAY_DONE,

    STEP_READY_CHECK_CONTY_ICING,
    STEP_READY_CHECK_CONTY_ICING_DONE,

    STEP_READY_TRAY_FEED_START,
    STEP_READY_TRAY_FEED_DONE,

    STEP_READY_GAS_SWITCH,
    //STEP_READY_GAS_SWITCH_DONE,

    STEP_READY_COMP_ON,

    STEP_READY_DONE

} ReadyStep_T;

typedef struct _do_ready_
{
    ReadyStep_T Step;

    U32 RetryTimeOut;       // Roomout time out, retry time out..
    U32 TrayFeedAmount;      // Time of Ice tray feeding
    U16 RoomOutTimeOut;     // 정수 추출 시간 초과
    U16 TimeOut;            // Step Timerout
} DoReady_T;



/* ICING */
typedef enum
{
    STEP_ICING_CHECK_COMP,
    STEP_ICING_FIGUARE_OUT_TIME,
    STEP_ICING,

    STEP_ICING_DONE
} IcingStep_T;

typedef struct _do_icing_
{
    IcingStep_T Step;

    U8 Region;
    U8 CompRps;

    U16 IcingTime;      // Time of Comp On
    U16 IcingTimeSecondPoint;   // 1/3을 지나는 시점
} DoIcing_T;


/* DE-ICING */
typedef enum
{
    STEP_DEICING_TRAY_MOVE,
    STEP_DEICING_TRAY_DONE,

    STEP_DEICING_FIGUARE_OUT_REGION,
    STEP_DEICING_COMMAND,
    STEP_DEICING_CONTROL,
    STEP_DEICING_DONE_CONTROL,

    STEP_DEICING_FORWARD_FEEDER_START,
    STEP_DEICING_FORWARD_FEEDER_DONE,

#if CONFIG_BACK_FEEDER
    STEP_DEICING_BACK_FEEDER_START,
    STEP_DEICING_BACK_FEEDER_DONE,
#endif

    STEP_DEICING_CHECK_ICE_FULL,
    STEP_DEICING_WAIT_CHECK_ICE_FULL,

    STEP_DEICING_DONE
} DeIcingStep_T;

typedef struct _do_deicing_
{
    DeIcingStep_T Step;

    /* DEICING HEATER REPEAT ON/OFF COMMAND & TIME */
    U8 Region;
    U8 Count;
    U8 RepeatNum;

    /* DEICING HEATER or COMP */
    U8 OnOffCmd;
    U16 OnOffTime;
    U8 CompRps;
    U8 FanOnOff;

#if CONFIG_BACK_FEEDER
    U16 BackFeederTime;
#endif
	U16	ForwardFeederTime;
} DoDeIcing_T;



/* DUMMY DE-ICING */
typedef enum
{
    STEP_DUMMY_DEICING_GAS_SWITCH,
    STEP_DUMMY_DEICING_TRAY_MOVE,
    STEP_DUMMY_DEICING_TRAY_DONE,

    STEP_DUMMY_DEICING_COMP_PROTECT_TIME,
    STEP_DUMMY_DEICING_FIGUARE_OUT_REGION,
    STEP_DUMMY_DEICING_COMP_ON,

    STEP_DUMMY_DEICING_COMMAND,
    STEP_DUMMY_DEICING_CONTROL,
    STEP_DUMMY_DEICING_DONE_CONTROL,

    STEP_DUMMY_DEICING_DONE
} DummyDeIcingStep_T;

typedef enum
{
    STEP_TRAY_DEICING_INIT,
#if 0
    STEP_TRAY_DEICING_TRAY_MOVE,
    STEP_TRAY_DEICING_TRAY_DONE,
#endif
    STEP_TRAY_DEICING_COMP_PROTECT_TIME,
    STEP_TRAY_DEICING_FIGUARE_OUT_REGION,
    STEP_TRAY_DEICING_CHECK_HOTGAS,
    STEP_TRAY_DEICING_COMMAND,
    STEP_TRAY_DEICING_CONTROL,
    STEP_TRAY_DEICING_DONE_CONTROL,
    STEP_TRAY_DEICING_DONE
} TrayDeIcingStep_T;

typedef struct _do_init_deicing_
{
    U16 Step;

    /* DEICING HEATER REPEAT ON/OFF COMMAND & TIME */
    U8 Region;
    U8 Count;
    U8 RepeatNum;

    /* COMP TIME */
    U16 CompOnTime;

    /* DEICING HEATER or COMP */
    U8 OnOffCmd;
    U16 OnOffTime;
    U8 CompRps;
    U8 FanOnOff;
} DoDummyDeIcing_T;


/* WiFi 상태 - 제빙 모드 */
typedef enum
{
    STEP_ICE_MAKE_STANDBY,
    STEP_ICE_MAKE_TRAY_IN_START,
    STEP_ICE_MAKE_TRAY_IN_ING,
    STEP_ICE_MAKE_MAKING_START,
    STEP_ICE_MAKE_MAKING_ING,
    STEP_ICE_MAKE_TAKE_OFF_START,
    STEP_ICE_MAKE_TAKE_OFF_ING,
    
    STEP_ICE_MAKE_FINISH_START,
    STEP_ICE_MAKE_FINISH_ING,
} WifiIceMakeStep_T;

void GetIceMakerInitData( DoInit_T *pData );

void GetIceMakerPreheatData( DoPreheat_T *pData );
void GetIceMakerReadyData( DoReady_T *pData );
ReadyStep_T GetIceMakerReadyStep(void);

void GetIceMakerIcingData( DoIcing_T *pData );

void GetIceMakerDeIcingData( DoDeIcing_T *pData );

void GetIceMakerInitDeIcingData( DoDummyDeIcing_T *pData );

void SetIcingTime(U16 mu16Val );
void SetDeIcingTime(U16 mu16Val );

// 더미 탈빙 중지
void StopDummyDeIcing(void);

// 트레이 입수 시간 단축
void TimeShortIceMakerFeedTime(void);

void ProcessIceMaker(void);

// 타이머 업데이트 ( @100ms )
void UpdateIceMakerTimer(void);

#endif /* __ICE_MAKER_H__ */
