
/**************************************************************
 *
 *  순간 온수 히터 제어
 *
 *   -. 순간 온수 히터 제어에 필요한 유량 계산
 *
 */
#include "hot_water_flow.h"
#include "hot_water_heater.h"
#include "hal_adc.h"
#include "temp.h"
#include "util.h"
#include "flow_meter.h"
#include "flow_valve.h"
#include "heater_out.h"
#include "hot_water_out.h"  // for dbg
#include "hot_water.h"  // for dbg


// 유량 센서 위치 값 계산
typedef struct _calc_flow_valve_
{
    // 샘플 완료 여부
    U8 IsSampled;       // FALSE : 계산이 된적 없음. TRUE : 계산된 값이 있음.

    // 샘플 값
    I16 Step;
    F32 LPM;
} CalcFlowValve_T;
CalcFlowValve_T CalcFlow;


HotWaterFlow_T HeaterFlow;

typedef struct _flow_valve_step_
{
    F32 LPM;
    I16 Step;
}FlowValveStep_T;

// v2 : 2022.03.16
// 20PSI 기준으로 작성
const static FlowValveStep_T  FlowValveStepList[] = 
{
    /* LPM,     STEP */
    { 0.90f,     1700 },
    { 0.75f,     1600 },
    { 0.70f,     1500 },
    { 0.6f,      1400 },
    { 0.50f,     1300 },
    { 0.45f,     1200 },
    { 0.40f,     1000 },
};
#define SZ_FLOW_VALVE_POSITION  (sizeof(FlowValveStepList) / sizeof(FlowValveStep_T) )
static I16 FindTargetValveStep(F32 mLPM)
{
    U8 i;
    U8 mu8ListSize;
    I16 mi16Step;


    mu8ListSize = SZ_FLOW_VALVE_POSITION;
    mi16Step    = FlowValveStepList[ mu8ListSize - 1 ].Step;
    for( i = 0; i < mu8ListSize; i++ )
    {
        if( FlowValveStepList[ i ].LPM <= mLPM )
        {
            mi16Step = FlowValveStepList[ i ].Step;
            break;
        }
    }

    return mi16Step;
}

void  InitHotWaterFlow(void)
{
    HeaterFlow.TargetLPM    = 0.0f;
    HeaterFlow.TargetHz     = 0U;
    HeaterFlow.MinHz = ConvLPM2Hz( MIN_LPM );
    HeaterFlow.MaxHz = ConvLPM2Hz( MAX_LPM );

    // 유량/스텝 계산
    CalcFlow.IsSampled = FALSE; 
}

void  GetHotWaterFlowData(HotWaterFlow_T *pData)
{
    MEMCPY( (void __far *)pData, (const void __far *)&HeaterFlow, sizeof( HotWaterFlow_T ));
}


F32 GetHotWaterTargetLPM(void)
{
    return HeaterFlow.TargetLPM;
}

U16 GetHotWaterTargetHz(void)
{
    return HeaterFlow.TargetHz;
}

// 히터 전력과 입수/출수 온도 차이에 따른
// 목표 유량을 계산한다.
static F32 CalcTargetHz(TEMP_T mTempTarget, TEMP_T mTempIn, WATT_T mHeaterWatt)
{
    F32 a = 0.0f;
    F32 b = 0.0f;
    F32 c = 0.0f;
    F32 d = 0.0f;
    F32 mTargetLPM = MAX_LPM;


    // CALC FLOW
    if( mTempTarget > mTempIn )       
    {
        /* HEATER EFFIENCY * 860kcal * HEATER WATT ) 
         * -> ( HEATER EFFIECY * 860 * HEATER WATT ) / 10000 ) 
         */
        a = (HEATER_EFFICIENCY * 860.0f * (F32)mHeaterWatt) / 1000.0f /* 860kCal */;
        b = mTempTarget - mTempIn;
        c = a/b;
        d = (c/60.0f);
        mTargetLPM = d;
    }

    // TAGET LPM and HZ
    HeaterFlow.RawTargetLPM = mTargetLPM;

    return HeaterFlow.RawTargetLPM;
}



typedef struct _flow_offset_
{
    F32 OffsetLPM;       // 목표 Hz 보정 

    U16 DownCount;      // 보정 주기 카운터 @100ms
    U16 UpCount;
    U16 SetCount;
} FlowOffset_T;

FlowOffset_T FlowOffset;


void InitOffsetLPM(void)
{
    FlowOffset.OffsetLPM    = 0.0f;

    FlowOffset.DownCount    = 0;
    FlowOffset.UpCount      = 0;
}

F32 GetFlowOffsetLPM(void)
{
    return FlowOffset.OffsetLPM;
}

void SetFlowOffsetLPM(F32 mLPM)
{
    FlowOffset.OffsetLPM = mLPM;
}

#define TEMP_HEATER_OVER_HEAT          102.0f   
#define OFFSET_TEMP     1.0f

#define MAX_OFFSET_LPM  0.10f
#define MIN_OFFSET_LPM  -0.10f
#define ADJUST_LPM      0.01f
F32 CalcOffsetLPM(F32 mTargetLPM, TEMP_T tTargetOut, TEMP_T tCurrentOut )
{
    I16 mi16Hz;
    TEMP_T mTempHeater;


    // UPPER REGION - A
    if( tTargetOut > tCurrentOut - OFFSET_TEMP )
    {
        FlowOffset.UpCount++;
        FlowOffset.DownCount = 0;

        // Set FlowCount 
        if( tTargetOut >= tCurrentOut - 5.0f )
        {
            if( FlowOffset.OffsetLPM < 0 )
            {
                FlowOffset.SetCount = 350;     // 2.5sec
            }
            else
            {
                FlowOffset.SetCount = 470;     // 3.7sec
            }
        }
        else
        {
            FlowOffset.SetCount = 400;         // 2.0sec
        }

        if( FlowOffset.UpCount >= FlowOffset.SetCount )
        {
            FlowOffset.UpCount = 0;

            FlowOffset.OffsetLPM = FlowOffset.OffsetLPM - ADJUST_LPM;

            // MIN
            if( FlowOffset.OffsetLPM <= MIN_OFFSET_LPM )
            {
                FlowOffset.OffsetLPM = MIN_OFFSET_LPM;
            }
        }
    }
    // LOWER REGION - B
    else if( tTargetOut < tCurrentOut + OFFSET_TEMP )
    {
        FlowOffset.DownCount++;
        FlowOffset.UpCount = 0;

        if( FlowOffset.DownCount >= 400 ) //400 )
        {
            FlowOffset.DownCount = 0;

            FlowOffset.OffsetLPM = FlowOffset.OffsetLPM + ADJUST_LPM;

            // MIN
            if( FlowOffset.OffsetLPM >= MAX_OFFSET_LPM )
            {
                FlowOffset.OffsetLPM = MAX_OFFSET_LPM;
            }
        }
    }
    else
    {
        // Reset Count
        FlowOffset.DownCount = 0;
        FlowOffset.UpCount = 0;
    }

    if( tCurrentOut >= 102.0f 
            || GetTemp( TEMP_ID_HEATER ) >= 102.0f )
    {
        FlowOffset.OffsetLPM = MAX_OFFSET_LPM;// + 0.10f;
    }

    return FlowOffset.OffsetLPM;
}



#define ADJUST_FLOW_TIME    10    // 100 ms @10ms
//static U8 mu8AdjustTime = ADJUST_FLOW_TIME;   
 U16 mu16AdjustTime = ADJUST_FLOW_TIME;   
void InitAdjustFlowValve(void)
{
    mu16AdjustTime = ADJUST_FLOW_TIME;
}

// 유량 조절 밸브가 최대로 닫혀 있으면 TRUE
F32 MinLPM;
static U8 IsMinFlowValve(void)
{

    MinLPM = GetHotWaterTargetLPM() - 0.2f;
    if( MinLPM <= MIN_LPM )
    {
        MinLPM = MIN_LPM;
    }

    if( GetFlowMeterLPM() <= MinLPM )
    {
        return TRUE;
    }

    return FALSE;
}

// 유량 조절 밸브가 최대로 열려 있으면 TRUE
F32 MaxLPM;
static U8 IsMaxFlowValve(void)
{

    MaxLPM = GetHotWaterTargetLPM() + 0.2f;
    if( MaxLPM >= MAX_LPM )
    {
        MaxLPM = MAX_LPM;
    }
    //if( MaxLPM >= GetHotOutLPM() )
    //{
    //    MaxLPM = GetHotOutLPM();
    //}

    if( GetFlowMeterLPM() < MaxLPM )
    {
        return FALSE;
    }

    return TRUE;
}

// 목표 유량
// 0.01 lpm == 15 step
#define ADJUST_INTERVAL     10  //  @10ms
void AdjustFlowValveByTemp(TEMP_T tTarget, TEMP_T tCurrent)
{
    I16 offset = 0;
    TEMP_T tDelta = 0.0f;

    // 100ms 주기로 제어할수 있는 최대 step은 40step이다.
    if( mu16AdjustTime != 0 )
    {
        mu16AdjustTime--;
    }
    else
    {
        mu16AdjustTime = ADJUST_INTERVAL;

        tDelta = tTarget - tCurrent;
        if( tDelta > 2.0f ) { offset = 50; }
        else if( tDelta > 1.5f ) { offset = 40; }
        else if( tDelta > 1.0f ) { offset = 30; }
        else if( tDelta > 0.5f ) { offset = 20; }
        else if( tDelta < -0.5f ) { offset = -20; }
        else if( tDelta < -1.0f ) { offset = -30; }
        else if( tDelta < -1.5f ) { offset = -40; }
        else if( tDelta < -2.0f ) { offset = -50; }

    }
}

U16 the_adjust_flow = 15;
//#define ADJUST_INTERVAL_FLOW     200  //  2sec @10ms
#define ADJUST_INTERVAL_FLOW     the_adjust_flow

U8 the_flow_auto  = 1;
F32 dbg_t_delta = 0;
U8 dbg_flow_cnt = 0;
void AdjustFlowValveByFlow(F32 mTargetLPM, F32 mCurrentLPM)
{
    I16 offset = 0;
    F32 tDelta = 0.0f;

    // 100ms 주기로 제어할수 있는 최대 step은 40step이다.
    if( mu16AdjustTime != 0 )
    {
        if( the_flow_auto)
            mu16AdjustTime--;
    }
    else
    {
        mu16AdjustTime = ADJUST_INTERVAL_FLOW;

        tDelta = mTargetLPM - mCurrentLPM;
        dbg_t_delta = tDelta;

        // 0.01 = 10step
        //tDelta = tDelta / 0.01f;
        //offset = (I16)tDelta;       // float -> int
        //offset *= 10;               // 0.1 lpm per 10step

        if( tDelta > 0.05f ) { offset = 50; }
        else if( tDelta > 0.04f ) { offset = 40; }
        else if( tDelta > 0.03f ) { offset = 30; }
        else if( tDelta > 0.02f ) { offset = 20; }
        //else if( tDelta < -0.02f ) { offset = -20; }
        else if( tDelta < -0.03f ) { offset = -30; }
        else if( tDelta < -0.04f ) { offset = -40; }
        else if( tDelta < -0.05f ) { offset = -50; }
        else 
        {
            if( mTargetLPM > mCurrentLPM )
            {
                offset = 20;
            }
        }

        dbg_flow_cnt++;
        if( offset > 0 )
        {
            if( IsMaxFlowValve() != TRUE )
            {
                dbg_hot.adjust_up++;
                FlowValveOffset( offset );
            }
        }
        else if( offset < 0 )
        {
            if( IsMinFlowValve() != TRUE )
            {
                dbg_hot.adjust_down++;
                FlowValveOffset( offset );
            }
        }
    }
}


// 예열 목표 유량 계산
// 예열 중에는 목표 유량을 작게해서 목표 온도에 도달하는 시간을 단축시킨다.
#define PREHEAT_HIGH_LPM    0.45f
#define PREHEAT_LOW_LPM     0.35f

#define OFFSET_HIGH_LPM     0.1f
#define OFFSET_LOW_LPM      0.05f
static F32 CalcPreHeatTargetLPM(F32 mLPM)
{
    F32 mOffsetLPM = 0.0f;


    if( mLPM >= PREHEAT_HIGH_LPM )
    {
        mOffsetLPM = OFFSET_HIGH_LPM;
    }
    else if( mLPM >= PREHEAT_HIGH_LPM )
    {
        mOffsetLPM = OFFSET_LOW_LPM;
    }

    return (mLPM - mOffsetLPM );
}


void InitFlowValveSampleLPM(void)
{
    CalcFlow.IsSampled  = FALSE;
    CalcFlow.LPM        = 0;
    CalcFlow.Step       = 0;

    FlowValveInit();
}

U8 IsCalSampleLPM(void)
{
    return CalcFlow.IsSampled;
}


void SetFlowValveSampleLPM( F32 mLPM, I16 mStep)
{
    CalcFlow.IsSampled  = TRUE;
    CalcFlow.LPM        = mLPM;
    CalcFlow.Step       = mStep;
}

static I16 FindOffsetValveStep(F32 mLPM)
{
    I16 mStep = 0;
    F32 mDeltaLPM;

    if( CalcFlow.LPM > mLPM )
    {
        mDeltaLPM = CalcFlow.LPM - mLPM;
        mDeltaLPM += 0.005f;     // 반올림
        mDeltaLPM *= 100.0f;     // 0.01 LPM 단위까지 처리
        mStep  = (I16)mDeltaLPM; // 나머지 소숫점 버림
        mStep  *= 10;// 0.01 LPM 당 10step
        mStep = -mStep;
    }
    else if( CalcFlow.LPM < mLPM )
    {
        mDeltaLPM =  mLPM - CalcFlow.LPM;
        mDeltaLPM += 0.005f;     // 반올림
        mDeltaLPM *= 100.0f;     // 0.01 LPM 단위까지 처리
        mStep  = (I16)mDeltaLPM; // 나머지 소숫점 버림
        mStep  *= 10;// 0.01 LPM 당 10step
    }

    return mStep;
}


I16 GetFlowValveStep(F32 mLPM)
{
    if( IsCalSampleLPM() == TRUE )
    {
        return FindOffsetValveStep( mLPM );
    }

    return FindTargetValveStep( mLPM );
}

/* traic 만 사용하기 때문에 유량을 제한한다. */
static F32 GetHotMilkLPM(TEMP_T tIn)
{
    if( tIn <= 10.0f ) { return 0.4f; }
    else if( tIn <= 15.0f ) { return 0.5f; }
    else if( tIn <= 20.0f ) { return 0.6f; }
    else if( tIn <= 25.0f ) { return 0.7f; }
    else
    {
        return 0.8f;
    }
}

F32 dbg_calc_offset_lpm = 0.02f;
void CalcHeaterTargetLPM(void)
{
    TEMP_T tIn          = 0.0f;
    TEMP_T tTargetOut   = 0.0f;
    WATT_T mHeaterWatt  = 0;
    F32 mTargetLPM;
    U16 mTargetHz;


    mHeaterWatt = GetHeaterWatt();
    tTargetOut  = GetHeaterTempTargetOut();
    tIn         = GetTemp( TEMP_ID_HOT_IN );

	/*sean [24-02-20] 히터 오버스펙이 생겨서 다음과 같이 수정*/
	//55도, 50도는 아래와 같이 히터 계산함
    if( (GetHotSelect() == SEL_HOT_MILK) ||  (GetHotSelect() == SEL_HOT_DEGREE_50) || (GetHotSelect() == SEL_HOT_DEGREE_55) || (GetHotSelect() == SEL_HOT_DEGREE_60) || (GetHotSelect() == SEL_HOT_DEGREE_65))
    {
        if( GetHeaterClass() == HEATER_CLASS_LOW )
        {
            mHeaterWatt = (WATT_T)((F32)mHeaterWatt * 0.40f);  // 40%만 사용
        }
        else
        {
            mHeaterWatt = (WATT_T)((F32)mHeaterWatt * 0.42f);  // 42%만 사용
        }
    }
#if 0
    else if( GetHotSelect() == SEL_HOT_TEA )
    {
        if( GetTemp( TEMP_ID_HOT_IN ) > 28.0f )
        {
            mHeaterWatt = (WATT_T)((F32)mHeaterWatt * 0.40f);  // 40%만 사용
        }
    }
#endif

#if 0
    // 분유인 경우, 트라이악 사이즈로 유량 제한
    if( GetHeaterClass() == HEATER_CLASS_HIGH 
        || GetHeaterClass() == HEATER_CLASS_MID )
    {
        if( GetHotSelect() == SEL_HOT_MILK )
        {
            mHeaterWatt = (WATT_T)((F32)mHeaterWatt * 0.40f);  // 40%만 사용
        }
        else if( GetHotSelect() == SEL_HOT_TEA 
                && GetHotOutRegionTempIn() == REGION_TEMP_HIGH )
        {
            mHeaterWatt = (WATT_T)((F32)mHeaterWatt * 0.40f);  // 45%만 사용
        }
    }
    else
    {
        // CLASS LOW
        if( GetHotSelect() == SEL_HOT_MILK 
                || GetHotSelect() == SEL_HOT_TEA )
        {
            mHeaterWatt = (WATT_T)((F32)mHeaterWatt * 0.42f);  // 42%만 사용
        }
    }
#endif

    // Calc target LPM
    mTargetLPM = CalcTargetHz( tTargetOut, tIn, mHeaterWatt );

    // 계산치 보다 약간 낮게 시작함.
    //if( GetHotSelect() == SEL_HOT_COFFEE )
    //{
    //    if( GetTemp( TEMP_ID_HOT_IN ) < 20.0f )
    //    {
    //        mTargetLPM -= 0.04;
    //    }
    //}
    //else
    {
        mTargetLPM -= 0.02;
    }

    HeaterFlow.TargetLPM = SetMinMaxF32( mTargetLPM, MIN_LPM, MAX_LPM );
    HeaterFlow.TargetHz  = ConvLPM2Hz( HeaterFlow.TargetLPM );

    // Calc Preheat target LPM
    //HeaterFlow.PreHeatTargetLPM = CalcPreHeatTargetLPM( HeaterFlow.TargetLPM );
}

