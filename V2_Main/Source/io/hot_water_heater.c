/**************************************************************************************************************************
 *  HOT WATER HEATER
 *
 *  -. 목표 온도, 그리고 입수 유량에 따라서 히터 출력 값을 계산한다.
 *
 *
 */
#include "hot_water_heater.h"
#include "hal_adc.h"
#include "temp.h"
#include "util.h"
#include "heater_out.h"
#include "heater_table.h"
#include "flow_meter.h"
#include "hot_water.h"

#include "hot_water_out.h" // for dbg


#define CONFIG_HEATER_OFFSET    1       // OFFSET 기능 활성화 ( 1: enable, 0: disable )



// HEATER CLASS LEVEL (hysteresis)
// CLASS HIGH : 231v ~ 242v
// CLASS MID : 230v ~ 210V
// CLASS LOW : 209v ~ 198v
//#define WATT_HIGH_IN        3042U        // 3.04kw   @ 234v
#if 0
#define WATT_HIGH_IN        3100U        // 3.04kw   @ 234v
#define WATT_HIGH_OUT       3003U        // 3.00kw   @ 231v
#define WATT_MID_IN         2717U        // 2.71kw   @ 209v
#define WATT_MID_OUT        2600U        // 2.67kw   @ 206v
#else
// 220V
// 상 : 2917
// 중 : 2832
// 하 : 2707
#define WATT_HIGH_IN        2905U
#define WATT_HIGH_OUT       2870U
#define WATT_MID_IN         2800U
#define WATT_MID_OUT        2750U
#endif


// HEATER WATT CHECK TIMER @10ms
#define STABLE_HEATER_WATT_TIME  50U //(1U /*SEC*/ * 100U /*@10ms*/ )    
//#define CHECK_HEATER_WATT_TIME   150U // 1.5sec
#define CHECK_HEATER_WATT_TIME   50U // 0.5sec
#define MAX_HEATER_ADC_COUNT     48U

#define TRANS_NORMAL    0
#define TRANS_UP        1
#define TRANS_DOWN      2




AverageAdc_T HeaterAdc;
HotWaterHeater_T Heater;
HeaterOffset_T  HeaterOffset;

void  InitHotWaterHeater(void)
{
    Heater.Class        = HEATER_CLASS_MID;

    Heater.BaseWatt     = MID_HEATER_WATT;
    Heater.OffsetWatt   = 0;
    Heater.Watt         = MID_HEATER_WATT;

    Heater.RawTargetWatt    = 0;
    Heater.TargetWatt       = 0;
}


void SetHotWaterHeaterOffsetWatt(WATT_T mWatt)
{
    Heater.OffsetWatt   = mWatt;
}

void  GetHotWaterHeaterData(HotWaterHeater_T *pData)
{
    MEMCPY( (void __far *)pData, (const void __far *)&Heater, sizeof( HotWaterHeater_T ));
}

void SetHeaterTempTargetOut(TEMP_T mTargetTemp)
{
    Heater.TargetOut = mTargetTemp;
}

TEMP_T GetHeaterTempTargetOut(void)
{
    return Heater.TargetOut;
}


static WATT_T SetMinMaxWatt(WATT_T mWatt, WATT_T mMin, WATT_T mMax )
{
    if( mWatt <= mMin )
    {
        mWatt = mMin;
    }
    else if( mWatt >= mMax )
    {
        mWatt = mMax;
    }

    return mWatt;
}

// apply heater compensation
// Watt = Base + Offset
static void CalcWatt(void)
{
    Heater.Watt = Heater.BaseWatt ;//+ Heater.OffsetWatt;
    Heater.Watt = SetMinMaxWatt( Heater.Watt, MIN_HEATER_WATT, MAX_HEATER_WATT );
}

HeaterClass_T GetHeaterClass(void)
{
    return Heater.Class;

}

WATT_T GetHeaterBaseWatt(void)
{
    return Heater.BaseWatt;
}

WATT_T GetHeaterWatt(void)
{
    return Heater.Watt;
}


static void UpdateHeaterClass(void)
{
    if( Heater.Class == HEATER_CLASS_MID )
    {
        if( Heater.BaseWatt <= WATT_MID_OUT )
        {
            Heater.Class = HEATER_CLASS_LOW;
        }
        else if( Heater.BaseWatt >= WATT_HIGH_IN )
        {
            Heater.Class = HEATER_CLASS_HIGH;
        }

    }
    else if( Heater.Class == HEATER_CLASS_HIGH )
    {
        if( Heater.BaseWatt <= WATT_MID_OUT )
        {
            Heater.Class = HEATER_CLASS_LOW;
        }
        else if( Heater.BaseWatt <= WATT_HIGH_OUT )
        {
            Heater.Class = HEATER_CLASS_MID;
        }

    }
    else    
    {
        if( Heater.BaseWatt >= WATT_HIGH_IN )
        {
            Heater.Class = HEATER_CLASS_HIGH;
        }
        else if( Heater.BaseWatt >= WATT_MID_IN )
        {
            Heater.Class = HEATER_CLASS_MID;
        }
    }

}


void InitCheckHeaterWatt(void)
{
    HeaterAdc.Sum        = 0UL;
    HeaterAdc.Count      = 0U;
    HeaterAdc.Average    = 0U;

    HeaterAdc.StableTime = STABLE_HEATER_WATT_TIME;
    HeaterAdc.CheckTime  = CHECK_HEATER_WATT_TIME;
}


static void SumHeaterWatt(void)
{
    if( HeaterAdc.Count < MAX_HEATER_ADC_COUNT )
    {
        HeaterAdc.Sum += HAL_GetAdcValue( ANI_HEATER_FB );
        HeaterAdc.Count++;
    }
}

static void AverageHeaterWatt(void)
{
    HeaterAdc.Average = (U16)(HeaterAdc.Sum / (U32)HeaterAdc.Count);
}

// Heater : 2860watt
// 198v : 2574watt 
// 242v : 3146watt
static void CalcHeaterWatt(void)
{
    Heater.RawWatt = CalcHeaterAdc2Watt( HeaterAdc.Average );

    Heater.BaseWatt = Heater.RawWatt;
    Heater.Watt     = Heater.RawWatt;
    CalcWatt();

    // IF AD CIRCUIT IS WRONG... USE DEFAULT VALUE
    if( Heater.Watt <= MIN_HEATER_WATT )
    {
        Heater.BaseWatt = MIN_HEATER_WATT;
        Heater.Watt     = MIN_HEATER_WATT;
    }
    else if( Heater.Watt >= MAX_HEATER_WATT )
    {
        Heater.BaseWatt = MAX_HEATER_WATT;
        Heater.Watt     = MAX_HEATER_WATT;
    }

    // Update New Heater Class
    UpdateHeaterClass();
}


// Return heater power (0~100%)
void CheckHeaterWatt(U16 mu16Output)
{
    if( mu16Output < 100 )
    {
        // clear variable
        InitCheckHeaterWatt();
        return ;
    }

    // 1. wait stable..
    if( HeaterAdc.StableTime != 0 )
    {
        HeaterAdc.StableTime--;
        return ;
    }

    // 2. calc watt
    if( HeaterAdc.CheckTime != 0 )
    {
        HeaterAdc.CheckTime--;

        SumHeaterWatt();
        AverageHeaterWatt();
        CalcHeaterWatt();
    }
    else
    {
        // 4. clear variable
        InitCheckHeaterWatt();
    }
}



// DANGER
#define TEMP_OVER_DANGER_DELAY      5
#define TEMP_OVER_DANGER            105.0f   

// OVER
#define HIGH_TEMP_OVER_OUT           97.5f   
#define HIGH_TEMP_OVER_HEATER        96.0f   

//#define TEMP_OVER_OUT                98.5f   
//#define TEMP_OVER_HEATER             96.0f   
#if 0
#define TEMP_OVER_OUT                98.5f   
#define TEMP_OVER_HEATER             98.0f   
#else
#define TEMP_OVER_OUT                97.5
#define TEMP_OVER_HEATER             96.0
#endif

U16 CheckOverHeating(I16 mi16Output)
{
    static U16 mu16Delay = TEMP_OVER_DANGER_DELAY;      // 0.5sec @100ms
    TEMP_T  tOut;
    TEMP_T  tHeater;
    I16 mi16NewOutput = 100;
    TEMP_T tOverOut = TEMP_OVER_OUT;
    TEMP_T tOverHeat = TEMP_OVER_HEATER;


    if( GetHeaterBaseWatt() >= HIGH_HEATER_WATT )
    {
        tOverOut  = HIGH_TEMP_OVER_OUT;
        tOverHeat = HIGH_TEMP_OVER_HEATER;
    }

    if( mi16Output < 0 )
    {
        mi16Output =  0;
    }

    tOut = GetTemp( TEMP_ID_HOT_OUT );
    tHeater = GetTemp( TEMP_ID_HEATER );
    if( tOut >= TEMP_OVER_DANGER || tHeater >= TEMP_OVER_DANGER )
    {
        if( mu16Delay != 0 )
        {
            mu16Delay--;
        }
        else
        {
            mi16NewOutput = 20;    // relay off...
        }
    }
    else if( tOut >= tOverOut || tHeater >= tOverHeat )
    {
        if( GetHeaterBaseWatt() >= HIGH_HEATER_WATT )
        {
            mi16NewOutput = 55;    // off all triac..
        }
        else
        {
#if 0
            if( GetTemp( TEMP_ID_HOT_IN ) <= 13.0f )
            {
                mi16NewOutput = 80;    // off all triac..
            }
            else
            {
                mi16NewOutput = 70;    // off all triac..
            }
#else
            mi16NewOutput = 80;    // off all triac..
#endif
        }
    }
    else
    {
       mu16Delay = TEMP_OVER_DANGER_DELAY;
    }

    if( mi16NewOutput < mi16Output )
    {
        mi16Output = mi16NewOutput;
    }

    return mi16Output;
}



void ResetHeaterOffset(void)
{
    HeaterOffset.OffsetWatt = 0;

    HeaterOffset.PrevTrans  = 0;
    HeaterOffset.Trans      = 0;

    HeaterOffset.UpCount    = 0;
    HeaterOffset.DownCount  = 0;
    HeaterOffset.SetCount   = 10;     // 0.1sec
}

#define OVER_OFFSET_TEMP     1.0f
#define UNDER_OFFSET_TEMP    0.5f
#define MAX_TEMP       50
#define MIN_TEMP       50

static U16 GetUpCount(TEMP_T tTarget, TEMP_T tHotOut)
{
    if( (GetHotSelect() == SEL_HOT_USER ) || (GetHotSelect() == SEL_HOT_DEGREE_95 ) || (GetHotSelect() == SEL_HOT_DEGREE_90 ) )
    {
        if( tHotOut < 95.0f )
        {
            return 1;
        }
        else
        {
            return 3;
        }
    }

    if( tTarget >= tHotOut - 3.0f )
    {
        if( HeaterOffset.OffsetWatt < 0 )
        {
            return 10;
        }
        else
        {
            return 30;
        }
    }

    return 10;
}
static U16 GetDownCount(void)
{
    if( GetTemp( TEMP_ID_HEATER ) > 99.0f )
    {
        return 1;
    }

//    if( GetHotSelect() == SEL_HOT_MILK )
//    {
//        return 30;  // 3sec...
//    }

    return 3;
}

void CalcOutputOffset(F32 mLPM, TEMP_T tTarget, TEMP_T tHotOut)
{
    WATT_T mWatt;
    WATT_T MaxWatt;
    WATT_T MinWatt;


    mWatt       = GetWattByLPM( mLPM );

    HeaterOffset.AdjustWatt = mWatt / 2;
    HeaterOffset.MaxWatt = mWatt * MAX_TEMP;
    HeaterOffset.MinWatt = -(mWatt * MIN_TEMP);

    // 목표 온도 보다 낮음
    if( tTarget > tHotOut - UNDER_OFFSET_TEMP )
    {
        if( HeaterOffset.PrevTrans != HeaterOffset.Trans )
        {
            HeaterOffset.PrevTrans = HeaterOffset.Trans;
        }
        HeaterOffset.Trans = TRANS_UP;
        HeaterOffset.UpCount++;
        HeaterOffset.DownCount = 0;

        // Set FlowCount 
        HeaterOffset.SetCount = GetUpCount( tTarget, tHotOut);

        if( HeaterOffset.UpCount >= HeaterOffset.SetCount )
        {
            HeaterOffset.UpCount = 0;

            HeaterOffset.OffsetWatt +=  HeaterOffset.AdjustWatt;
            
            // MIN
            if( HeaterOffset.OffsetWatt >= HeaterOffset.MaxWatt )
            {
                HeaterOffset.OffsetWatt = HeaterOffset.MaxWatt;
            }
        }
    }
    // 목표 온도 보다 높음
    else if( tTarget < tHotOut + OVER_OFFSET_TEMP )
    {
        if( HeaterOffset.PrevTrans != HeaterOffset.Trans )
        {
            HeaterOffset.PrevTrans = HeaterOffset.Trans;
        }
        HeaterOffset.Trans = TRANS_DOWN;
        HeaterOffset.DownCount++;
        HeaterOffset.UpCount = 0;

        //if( HeaterOffset.DownCount >= 3 ) // 0.3sec
        if( HeaterOffset.DownCount >= GetDownCount() ) // 0.3sec
        {
            HeaterOffset.DownCount = 0;

            HeaterOffset.OffsetWatt -=  HeaterOffset.AdjustWatt;

            // MIN
            if( HeaterOffset.OffsetWatt <= HeaterOffset.MinWatt )
            {
                HeaterOffset.OffsetWatt = HeaterOffset.MinWatt;
            }
        }
    }
    else
    {
        if( HeaterOffset.PrevTrans != HeaterOffset.Trans )
        {
            HeaterOffset.PrevTrans = HeaterOffset.Trans;
        }
        HeaterOffset.Trans = TRANS_NORMAL;

        // Reset Count
        HeaterOffset.DownCount = 0;
        HeaterOffset.UpCount = 0;
    }

    if( HeaterOffset.Trans != TRANS_NORMAL 
            && HeaterOffset.PrevTrans != HeaterOffset.Trans )
    {
        HeaterOffset.OffsetWatt = 0;
    }

    if( tHotOut >= 99.8f 
            || GetTemp( TEMP_ID_HEATER ) >= 101.0f )
    {
        HeaterOffset.OffsetWatt = HeaterOffset.MinWatt;
    }

    SetHotWaterHeaterOffsetWatt( HeaterOffset.OffsetWatt );
}


// 히터 출력율 계산 ( OUTPUT RATE )
// 목표 WATT를 히터 WATT의 백분율로 계산
static U16 CalcOutputRate(WATT_T mTargetWatt, WATT_T mHeaterWatt)
{
    U16 mu16Output;
    F32 Val;

    Val = (((F32)mTargetWatt / (F32)mHeaterWatt ) * 100.0f );
    if( Val <= 0.0f )
    {
        mu16Output = 0;
    }
    else
    {
        mu16Output = (U16)Val;
    }

    return mu16Output;
}

// 히터 출력율 계산
U16 CalcHeaterOutput(void)
{
    return  CalcOutputRate( Heater.TargetWatt,  Heater.Watt );
}


// 히터 목표 WATT 계산
// mf32CurrentLPM : 현재 유량
// tTarget : 목표 온도 
// tInput : 입수 온도
// Return : 히터 출력율 
static WATT_T CalcTargetWatt( F32 mf32CurrentLPM, TEMP_T tTarget, TEMP_T tInput)
{
    F32 a = 0.0f;
    F32 b = 0.0f;
    F32 c = 0.0f;
    F32 d = 0.0f;
    F32 e = 0.0f;


    // 입력 온도가 더 높으면 최저치 반환
    if( tTarget <= tInput )
    {
        return 0;
    }

    a = mf32CurrentLPM * 60.0f;
    b = tTarget - tInput;
    c = a * b;
    d = (c / 860.0f);
    e = (d / HEATER_EFFICIENCY ) * 1000.0f;

    // RAW DATA 
    Heater.RawTargetWatt = (WATT_T)(e);
    //mTargetWatt = SetMinMaxWatt( mTargetWatt, 0, MAX_HEATER_WATT );     // CHECK ONLY MAX

    return Heater.RawTargetWatt;

}


// 히터 목표 WATT 계산
// 목표 온도, 입수 온도, LPM, HEATER 효율
void CalcHeaterTargetWatt(F32 mf32LPM)
{
    TEMP_T tHotIn   = 0.0f;
    TEMP_T tHotOut  = 0.0f;
    TEMP_T tHotTargetOut  = 0.0f;


    // 1. 유량, 목표 온도에 따른 히터 출력 계산
    tHotTargetOut   = GetHeaterTempTargetOut();
    tHotIn          = GetTemp( TEMP_ID_HOT_IN );
    tHotOut         = GetTemp( TEMP_ID_HOT_OUT );
    Heater.TargetWatt  = CalcTargetWatt( mf32LPM, tHotTargetOut, tHotIn );

#if CONFIG_HEATER_OFFSET
    // 2. Target = RawTarget + OffsetTarget
    Heater.TargetWatt += Heater.OffsetWatt;
#endif

    // 3. 목표 WATT에 결정 
    //Heater.TargetWatt = SetMinMaxWatt( Heater.TargetWatt, 500, MAX_HEATER_WATT );
}


