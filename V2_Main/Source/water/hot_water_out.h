#ifndef __HOT_WATER_OUT_H__
#define __HOT_WATER_OUT_H__

#include "prj_type.h"
#include "water_out_type.h"


// 출수 동작 상세 제어 모드
#define HOT_OUT_INIT_DRAIN         0x0001
#define HOT_OUT_COOLING            0x0002
#define HOT_OUT_PRE_HEATING        0x0010
#define HOT_OUT_OUT_EOL            0x0020
#define HOT_OUT_OUT                0x0040
#define HOT_OUT_DONE               0x0080
#define HOT_OUT_OVER_HEAT          0x0100
#define HOT_OUT_RESET              0x0200


// INIT DRAIN..
#define HOT_OUT_FIRST  (\
        HOT_OUT_INIT_DRAIN )

// HOT HOT with COOLING 
#define HOT_OUT_WITH_COOLING  (\
        HOT_OUT_COOLING | \
        HOT_OUT_PRE_HEATING | \
        HOT_OUT_OUT )

// HOT OUT 
#define HOT_OUT_ALL    (\
        HOT_OUT_PRE_HEATING | \
        HOT_OUT_OUT )


// HOT OUT - EOL MODE
#define HOT_OUT_ALL_EOL    (\
        HOT_OUT_OUT_EOL \
        )

// CANCEL
#define HOT_OUT_CANCEL  (\
        HOT_OUT_INIT_DRAIN |  \
        HOT_OUT_COOLING |     \
        HOT_OUT_PRE_HEATING | \
        HOT_OUT_OUT |         \
        HOT_OUT_OUT_EOL | \
        HOT_OUT_DONE )

typedef struct _dbg_hot_
{
    U8 protect;         // 보호동작 발생(1)

    U16 adjust_up;      // 유량 조절 밸브 up 횟수
    U16 adjust_down;    // 유량 조절 밸브 down 횟수
} dbg_hot_t;
extern dbg_hot_t   dbg_hot;



// Temp In Region
#define REGION_TEMP_LOW      0
#define REGION_TEMP_MID      1
#define REGION_TEMP_HIGH     2

typedef struct _hot_water_out_
{
    U8  Init;               // 온수 타입 변경 (TRUE:변경, FALSE:유지)
    U8  Out;                // 현재 추출 명령 상태(TRUE, FALSE)
    U8  PrevOut;            // 이전 추출 명령 상태
    U16 LastedOutTime;      // 마지막 추출시간 (30sec)

    WaterOutType_T  Type;   // 추출량 타입
    U32 TargetAmount;       // 추출량 타입에 따른 추출 목표량
    U32 CurrentAmount;

    U16 Mode;               // 상세 제어 모드

    F32 LPM;                // 온수 추출 LPM

    U8 RegionTempIn;        // 입수 온도 조건

    // TARGET TEMP
    TEMP_T  TargetOutTemp;
    TEMP_T  TargetPreHeatTemp;

    // MODE - INIT
    U32 InitTargetAmount;     // 초기화 목표 추출량
    U32 InitCurrentAmount;    // 초기화 현재 추출량
    U32 InitCheckWattAmount;  // 초기배수시 WATT 측정 포인트

    // MODE - COOLING 
    U16 CoolingTime;        // 쿨링 시간
    
    U16 MaxTimeOut;         // 최대 동작 시간( 시간 초과시 강제 종료 )

    // Calc Averaget Out Temp
    U8      SumDelayTime;
    U32     SumCount;
    TEMP_T  SumOutTemp;
    TEMP_T  AverageOutTemp;
} HotOut_T;


#define CUP_LIST_NUM    12  // 500ml
#define CUP_ONE_NUM     12  // 120ml
#define CUP_HALF_NUM    3  // 120ml
typedef struct _cup_temp_
{
    TEMP_T tSum;
    TEMP_T tAverage;
    U8 SumCount;
    U16 AmountHz;   // 10mL Hz

    TEMP_T FirstTargetTemp;
    TEMP_T SecondTargetTemp;

    TEMP_T tSumList[ CUP_LIST_NUM ];
    F32    lpmSumList[ CUP_LIST_NUM ];
} CupTemp_T;


void InitHotOut(void);

void  GetHotOutData(HotOut_T *pData);

void UpdateRegionTempIn(void);

TEMP_T  GetHotOutAverageOutTemp(void);

TEMP_T GetHotOutPreHeatTemp(void);

TEMP_T GetHotOutTargetOutTemp(void);

U8 GetHotOutRegionTempIn(void);

// LPM
F32 GetHotOutLPM(void);
void SetFlowMeterLPM(F32 mf32LPM);
void UpdateHotOutLPM(F32 mf32LPM);

// 제어 모드 
void SetHotOutMode( U16 mu16Mode );
U16 GetHotOutMode(void );
void ClearHotOutMode( U16 mu16Mode );
U8 IsSetHotOutMode(U16 mu8Mode );


void SetHotOutChangedSel(void);
void ClearHotOutChangedSel(void);
U8 GetHotOutChangedSel(void);


// 마지막 추출 시간
// 단수 검사 재검사 시간
#define RECHECK_OUTAGE_TIME         3000U       // 30sec @10ms
#define SHORT_HEATING_TIME          3000U       // 30sec @10ms
void ClearHotOutLastedTimeOut(void);
void ResetHotOutLastedTimeOut(void);
U16 GetHotOutLastedTimeOut(void);

// 추출 시작 명령
// mType : 추출 타입
void TurnOnHotOut(WaterOutType_T mType);

// 추출 상태
// TRUE : 추출, FALSE, 정지
U8 IsHotOut(void);

// 추출 중지 명령
void TurnOffHotOut(void);

U8 GetHotOutPercent(void);

// 추출 진행량( 0~ 999 mL )
U16 GetHotOutAmount(void);

void UpdateHotInitCurrentAmount(void);

// 추출 제어
U8 ControlWaterHotOut(void);


#endif /* __HOT_WATER_OUT_H__ */
