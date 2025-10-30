#include "hw.h"
#include "hot_water_out_preheat.h"

#include "hot_water_out.h"
#include "hot_water.h"
#include "valve.h"
#include "user.h"
#include "temp.h"
#include "flow_meter.h"
#include "flow_valve.h"
#include "hot_water_heater.h"
#include "hot_water_flow.h"
#include "hot_water_temp_table.h"
#include "heater_table.h"
#include "water_out_common.h"
#include "heater_out.h"
#include "util.h"
#include "hot_water.h"


#define MAX_TIME_OUT_PREHEAT_VERY_LONG   2000U       // 20sec @10ms
#define MAX_TIME_OUT_PREHEAT_LONG   1500U       // 15sec @10ms
#define MAX_TIME_OUT_PREHEAT        1000U       // 10sec @10ms
#define MIN_PREHEAT_ON_TIME_500MS   50U     // 0.5sec @10ms
#define MIN_PREHEAT_ON_TIME_1S     100U     // 1sec @10ms
#define MIN_PREHEAT_ON_TIME_2S     200U     // 2sec @10ms
#define MIN_PREHEAT_ON_TIME_3S     300U     // 3sec @10ms



PreHeat_T PreHeat;



static void DBG_CheckPreHeatTemp(void)
{
    // 히팅 종료 시점 온도 측정( for recoding(debug) )
    PreHeat.tPreheatOut     = GetTemp( TEMP_ID_HOT_OUT );
    PreHeat.tPreheatHeater  = GetTemp( TEMP_ID_HEATER );
}

PreHeatType_T   GetHotOutPreHeatType(void)
{
    return PreHeat.Type;
}

// DISPLAY PRE-HEATING LEVEL 
U8 GetHotOutPreHeatLevel(void)
{
    return PreHeat.DispStep;
}




U16 GetMinHeatingTime(void)
{
    TEMP_T tDelta;
    U16 mu16OnTime = MIN_PREHEAT_ON_TIME_500MS;

    tDelta = GetHotOutTargetOutTemp() - GetTemp( TEMP_ID_HOT_OUT );
    if( tDelta < -3.0f )
    {
        mu16OnTime =  MIN_PREHEAT_ON_TIME_500MS;
    }
    else if( tDelta < 0.0f )
    {
        mu16OnTime =  MIN_PREHEAT_ON_TIME_1S;
    }
    else if( tDelta <= 3.0f )
    {
        mu16OnTime =  MIN_PREHEAT_ON_TIME_1S;
    }
    else if( tDelta <= 5.0f )
    {
        if( GetHeaterClass() == HEATER_CLASS_HIGH )
        {
            mu16OnTime =  MIN_PREHEAT_ON_TIME_1S;
        }
        else
        {
            mu16OnTime =  MIN_PREHEAT_ON_TIME_2S;
        }
    }
    else if( tDelta <= 10.0f )
    {
        mu16OnTime =  MIN_PREHEAT_ON_TIME_2S;
    }
    else
    {
        mu16OnTime = MIN_PREHEAT_ON_TIME_3S;
    }

    if( GetHotOutLastedTimeOut() >= SHORT_HEATING_TIME )
    {
        mu16OnTime = MIN_PREHEAT_ON_TIME_3S;
    }

    // 100도 출수시 80도 이하면... 최소 5초 예열
    if( ( (GetHotSelect() == SEL_HOT_USER ) || (GetHotSelect() == SEL_HOT_DEGREE_95 ) || (GetHotSelect() == SEL_HOT_DEGREE_90 ) )
            && GetTemp( TEMP_ID_HEATER ) < 78.0f )
    {
        mu16OnTime = MIN_PREHEAT_ON_TIME_3S + MIN_PREHEAT_ON_TIME_3S;
    }


    return mu16OnTime;
}

static void InitPreHeatDispLevel(TEMP_T tPreHeat, TEMP_T tHeater)
{
    // 가열 시간(s)
    // = (목표-입수) * 히터체적 * 4.2 / ( 히터 출력 * 효율 )

    F32 val;

    if( tPreHeat > tHeater  )
    {
        val = ((tPreHeat - tHeater ) * 95.0f * 4.2f ) / ( GetHeaterWatt() * 0.97f );
        PreHeat.CalculatedTimeOut = (U16)val;
        PreHeat.CalculatedTimeOut *= 100U;
    }
    else
    {
        PreHeat.CalculatedTimeOut = GetMinHeatingTime();
    }

    PreHeat.DispStep = PREHEAT_PROGRESS_LEVEL_1;
}

static void CheckPreHeatDispLevel(TEMP_T tPreHeat, TEMP_T tHeater)
{
    U16 Mid;
    U16 Max;

    Mid = (U16)(PreHeat.CalculatedTimeOut * 0.4f);
    Max = (U16)(PreHeat.CalculatedTimeOut * 0.8f);
    
    if( PreHeat.HeatingTime >= Max )
    {
        PreHeat.DispStep = PREHEAT_PROGRESS_LEVEL_3;
    }
    else if( PreHeat.HeatingTime >= Mid )
    {
        PreHeat.DispStep = PREHEAT_PROGRESS_LEVEL_2;
    }
    else
    {
        PreHeat.DispStep = PREHEAT_PROGRESS_LEVEL_1;
    }

}


static void DonePreHeatDispLevel(void)
{
    PreHeat.DispStep = PREHEAT_PROGRESS_LEVEL_3;
}

static U16 GetPreHeatPower(void)
{
    U8 mu8HotSel;
    TEMP_T tHeater;


	mu8HotSel = GetHotSelect();
    tHeater   = GetTemp( TEMP_ID_HEATER );

    if( ( mu8HotSel == SEL_HOT_USER ) || ( mu8HotSel == SEL_HOT_DEGREE_95 ) || ( mu8HotSel == SEL_HOT_DEGREE_90 )  )
    {
        // 과열 조건에서는 히터 출력을 약하게
        if( GetHeaterClass() == HEATER_CLASS_HIGH )
        {
            if( GetHeaterBaseWatt() >= HIGH_HEATER_WATT )
            {
                if( tHeater >= 95.0f )      { return 0; }
                else if( tHeater >= 90.0f ) { return 55; }

                if( GetTemp( TEMP_ID_HOT_IN ) < 18.0f )
                {
                    if( tHeater >= 80.0f ) { return 60; }
                    else if( tHeater >= 50.0f ) { return 55; }
                }
                else
                {
                    if( tHeater >= 80.0f )      { return 55; }
                    else if( tHeater >= 50.0f ) { return 60; }
                }
            }
            else
            {
                if( tHeater >= 95.0f )      { return 0; }
                else if( tHeater >= 90.0f ) { return 55; }

                if( GetTemp( TEMP_ID_HOT_IN ) < 18.0f )
                {
                    if( tHeater >= 80.0f ) { return 60; }
                    else if( tHeater >= 50.0f ) { return 60; }
                }
                else
                {
                    if( tHeater >= 80.0f )      { return 60; }
                    else if( tHeater >= 60.0f ) { return 60; }
                }
            }
        }
        else if( GetHeaterClass() == HEATER_CLASS_MID )
        {
            if( tHeater >= 98.0f )      { return 0; }
            else if( tHeater >= 95.0f ) { return 55; }
            else if( tHeater >= 90.0f ) { return 55; }

            if( GetTemp( TEMP_ID_HOT_IN ) < 18.0f )
            {
                if( tHeater >= 80.0f ) { return 60; }
                else if( tHeater >= 50.0f ) { return 55; }
            }
            else
            {
                if( tHeater >= 80.0f ) { return 60; }
                else if( tHeater >= 50.0f ) { return 60; }
            }
        }
        else
        {
            if( tHeater >= 98.0f )      { return 0; }
            else if( tHeater >= 95.0f ) { return 55; }
            else if( tHeater >= 90.0f ) { return 55; }
            else if( tHeater >= 50.0f ) { return 60; }
        }
    }
    else if( mu8HotSel == SEL_HOT_COFFEE )
    {
        if( GetHeaterClass() == HEATER_CLASS_LOW )
        {    
            if( tHeater >= 80.0f )
            {
                return 55;
            }

            return 100;
        }
        else if( GetHeaterClass() == HEATER_CLASS_HIGH )
        {
            if( tHeater >= 80.0f )
            {
                return 55;
            }
            else if( tHeater >= 50.0f )
            {
                return 60;
            }

            return 100;
        }
        else // if( GetHeaterClass() == HEATER_CLASS_MID )
        {
            if( tHeater >= 80.0f )
            {
                return 55;
            }
            else if( tHeater >= 50.0f )
            {
                if( GetTemp( TEMP_ID_HOT_IN ) > 18.0f )
                {
                    // 초기 예열시 과열 방지
                    return 70;
                }
                else
                {
                    return 80;
                }
            }
			else
			{
				return 100;
			}
        }
    }
	/*sean [24-02-20] 75,80도구간 과히팅으로 인한 구역 통합 */
    else if( ( mu8HotSel == SEL_HOT_TEA ) || ( mu8HotSel == SEL_HOT_DEGREE_75 ) || ( mu8HotSel == SEL_HOT_DEGREE_80 ) )
    {
        if( GetHeaterClass() == HEATER_CLASS_HIGH )
        {
            if( tHeater >= 80.0f )
            {
                return 40;
            }
            else if( tHeater >= 65.0f )
            {
                return 55;
            }
            else if( tHeater >= 40.0f )
            {
                // 초기 예열시 과열 방지
                return 80;
            }
            else
            {
                if( GetTemp( TEMP_ID_HOT_IN ) < 20.0f )
                {
                    return 100;
                }
                else if( GetTemp( TEMP_ID_HOT_IN ) < 30.0f )
                {
                    return 100;
                }
                else
                {
                    return 80;
                }
            }
        }
        else if( GetHeaterClass() == HEATER_CLASS_MID )
        {

            if( tHeater >= 80.0f )
            {
                return 40;
            }
            else if( tHeater >= 65.0f )
            {
                if( GetTemp( TEMP_ID_HOT_IN ) > 18.0f )
                {
                    return 55;
                }
                else
                {
                    return 60;
                }
            }
            else if( tHeater >= 50.0f )
            {
                return 80;
            }
            else if( tHeater >= 40.0f )
            {
                // 초기 예열시 과열 방지
                return 80;
            }
			else
			{
				return 100;
			}
        }
        else // if( GetHeaterClass() == HEATER_CLASS_LOW )
        {
            if( tHeater >= 80.0f )
            {
                return 40;
            }
            else if( tHeater >= 65.0f )
            {
                return 55;
            }
            else if( tHeater >= 40.0f )
            {
                // 초기 예열시 과열 방지
                return 80;
            }
            else
            {
                // CLASS LOW
                if( GetTemp( TEMP_ID_HOT_IN ) < 20.0f )
                {
                    return 100;
                }
                else if( GetTemp( TEMP_ID_HOT_IN ) < 30.0f )
                {
                    return 80;
                }
				else
				{
					return 100;
				}
            }
        }
    }
    else if( mu8HotSel == SEL_HOT_DEGREE_65 )
    {
        if( tHeater >= 75.0f )
        {
            return 0;
        }
        else if( tHeater >= 70.0f )
        {
            return 20;
        }
        else
        {
            // 우유 커피 중간 상태로 Heater 출력 Up
            return 90;
        }
    }
    else if( mu8HotSel == SEL_HOT_DEGREE_60 )
    {
        if( tHeater >= 70.0f )
        {
            return 0;
        }
        else if( tHeater >= 65.0f )
        {
            return 20;
        }
        else
        {
            // 우유 커피 중간 상태로 Heater 출력 Up
            return 90;
        }
    }
    else if( mu8HotSel == SEL_HOT_DEGREE_55 )
    {
        if( tHeater >= 65.0f )
        {
            return 0;
        }
        else if( tHeater >= 60.0f )
        {
            return 20;
        }
        else
        {
#if 0
			/*sean [24-02-20] 50,55도 구간 해당 내용 삽입시, 2~3도 오름 */
        	if( GetHeaterClass() == HEATER_CLASS_HIGH )
        	{
	            // 우유 커피 중간 상태로 Heater 출력 Up
	            return 80;
        	}
			else
			{
	            return 40;
			}
#else
			return 80;
#endif
        }
    }
    else if( mu8HotSel == SEL_HOT_DEGREE_50 )
    {
        if( tHeater >= 60.0f )
        {
            return 0;
        }
        else if( tHeater >= 55.0f )
        {
            return 20;
        }
        else
        {
#if 0
			/*sean [24-02-20] 50,55도 구간 해당 내용 삽입시, 2~3도 오름 */
        	if( GetHeaterClass() == HEATER_CLASS_HIGH )
        	{
	            // 우유 커피 중간 상태로 Heater 출력 Up
	            return 80;
        	}
			else
			{
	            return 40;
			}
#else
			return 80;
#endif
        }
    }
    else if( mu8HotSel == SEL_HOT_MILK )
    {
        if( tHeater >= 55.0f )
        {
            return 0;
        }
        else if( tHeater >= 50.0f )
        {
            return 20;
        }
        else // if( tHeater < 50.0f )
        {
            // 우유 선택시... 히터 반만 on
            return 40;
        }
    }
	/*.. sean [24-04-05] 고온 추출후 낮은온도 뽑을 때에 온도 Tracking 되지 못하여 overheat일때 예열일 경우, 출력값 높임 ..*/
    if( PreHeat.Type == OVER_PREHEAT )
    {
    	if( ( mu8HotSel == SEL_HOT_DEGREE_80 ) || ( mu8HotSel == SEL_HOT_DEGREE_75 ) )
        	return 90;
    	else if( ( mu8HotSel == SEL_HOT_DEGREE_65 ) || ( mu8HotSel == SEL_HOT_DEGREE_60 ) )
        	return 80;
    }
	else {}
	
    	return 100;
}

static void Heating(void)
{
    U16 mu16Power;

    // HEATING...
    mu16Power = GetPreHeatPower();  // 히터 출력 반환
    SetHeaterOutput( mu16Power );   // 히터 출력 ON

    CheckHeaterWatt( GetHeaterOutput() );   // 히터 전력 측정
}


static U8 IsTimeOutPreHeating(void)
{
    if( PreHeat.HeatingTime < PreHeat.MaxTimeOut )
    {
        PreHeat.HeatingTime++;
        return FALSE;
    }

    return TRUE;
}


#define HEATER_TEMP_NUM 3
TEMP_T tHeaterTemp[ HEATER_TEMP_NUM ];
U8 tHeaterCount = 0;

static void InitHeaterTemp(void)
{
    tHeaterCount = HEATER_TEMP_NUM;

    tHeaterTemp[ 0 ] = 0.0f;
    tHeaterTemp[ 1 ] = 0.0f; 
    tHeaterTemp[ 2 ] = 0.0f; 
}

static void SetHeaterTemp(TEMP_T mTemp)
{
    if( tHeaterTemp[2] != mTemp )
    {
        if( tHeaterCount != 0 )
        {
            tHeaterCount--;
        }

        tHeaterTemp[ 0 ] = tHeaterTemp[ 1 ];
        tHeaterTemp[ 1 ] = tHeaterTemp[ 2 ];
        tHeaterTemp[ 2 ] = mTemp;
    }
}

#define TEMP_STATUS_NONE        0
#define TEMP_STATUS_RISING      1
#define TEMP_STATUS_FALLING     2
static U8 GetStatusHeaterTemp(void)
{
    if( tHeaterCount != 0 )
    {
        return TEMP_STATUS_NONE;
    }

    if( tHeaterTemp[0] < tHeaterTemp[1] 
            && tHeaterTemp[1] < tHeaterTemp[2] )
    {
        return TEMP_STATUS_RISING;
    }

    if( tHeaterTemp[0] > tHeaterTemp[1] 
            && tHeaterTemp[1] > tHeaterTemp[2] )
    {
        return TEMP_STATUS_FALLING;
    }

    return TEMP_STATUS_NONE;
}

static void UpdateHeatingTime(void)
{
    if( GetHeaterOutput() > 0 )
    {
        PreHeat.HeaterOnTime++;
    }
}

static U8 IsReachedTargetTemp(void)
{
    if( GetHotOutPreHeatTemp() <= GetTemp( TEMP_ID_HEATER ) )
    {
        // 목표 온도에 도달했지만, 최소 히팅 시간은 지킨다.
        if( PreHeat.HeaterOnTime >= PreHeat.HeaterConfOnTime )
        {
            return TRUE;
        }
    }

    return FALSE;
}

static U8 CheckHeating(void)
{
    // 히터가 off 조건이면, 즉시 반환
    if( GetHeaterOutput() == 0 )
    {
        return TRUE;
    }

	//add
	if( GetTemp( TEMP_ID_HEATER ) >= 100.0f )
	{
		return TRUE;
	}

    // 100도, 현재 내부 온도 98도 이상이면 즉시 출수전환
#if 0
    if( GetHeaterClass() != HEATER_CLASS_HIGH )
    {
        if( GetHotSelect() == SEL_HOT_USER 
                && GetTemp( TEMP_ID_HEATER ) >= 98.0f )
        {
            return TRUE;
        }
    }
    else
    {
        if( GetHotSelect() == SEL_HOT_USER 
                && GetTemp( TEMP_ID_HEATER ) >= 95.0f )
        {
            return TRUE;
        }
    }
#else
    if( GetHotSelect() == SEL_HOT_USER 
            && GetTemp( TEMP_ID_HEATER ) >= 92.0f )
    {
        return TRUE;
    }
#endif
    if( GetHotSelect() == SEL_HOT_DEGREE_95 
            && GetTemp( TEMP_ID_HEATER ) >= 92.0f )
    {
        return TRUE;
    }
    if( GetHotSelect() == SEL_HOT_DEGREE_90 
            && GetTemp( TEMP_ID_HEATER ) >= 92.0f )
    {
        return TRUE;
    }

	/*sean [24-03-18] 온도가 과하게 높아지는것을 막기 위해서 +10'C도 제한 */ 
	if( GetHotSelect() == SEL_HOT_DEGREE_50 
			   && GetTemp( TEMP_ID_HEATER ) >= 60.0f )
	{
	   return TRUE;
	}
    else if( GetHotSelect() == SEL_HOT_DEGREE_55 
            && GetTemp( TEMP_ID_HEATER ) >= 62.0f )
    {
        return TRUE;
    }
    else if( GetHotSelect() == SEL_HOT_DEGREE_60 
            && GetTemp( TEMP_ID_HEATER ) >= 70.0f )
    {
        return TRUE;
    }
    else if( GetHotSelect() == SEL_HOT_DEGREE_65 
            && GetTemp( TEMP_ID_HEATER ) >= 75.0f )
    {
        return TRUE;
    }
    else if( GetHotSelect() == SEL_HOT_TEA 
            && GetTemp( TEMP_ID_HEATER ) >= 80.0f )
    {
        return TRUE;
    }
    else if( GetHotSelect() == SEL_HOT_DEGREE_75 
            && GetTemp( TEMP_ID_HEATER ) >= 85.0f )
    {
        return TRUE;
    }
    else if( GetHotSelect() == SEL_HOT_DEGREE_80 
            && GetTemp( TEMP_ID_HEATER ) >= 90.0f )
    {
        return TRUE;
    }
    else if( GetHotSelect() == SEL_HOT_MILK )
    {
        if( GetTemp( TEMP_ID_HEATER ) >= 55.0f )
        {
            return TRUE;
        }
    }
	else {}
#if 0
	/*sean [24-03-11] 상한치 히터 오버히팅됨*/ 
    /*sean [24-03-11] TEA & 75℃ 일때 오버히팅됨*/ 
    if( GetHotSelect() == SEL_HOT_TEA 
            && GetTemp( TEMP_ID_HEATER ) >= 78.0f )
    {
        return TRUE;
    }
	else {}


    if( GetHotSelect() == SEL_HOT_DEGREE_75 
            && GetTemp( TEMP_ID_HEATER ) >= 82.0f )
    {
        return TRUE;
    }
	else {}
#endif

    SetHeaterTemp( GetTemp( TEMP_ID_HEATER ));
    if( IsReachedTargetTemp() == TRUE )
    {
        if(GetStatusHeaterTemp() == TEMP_STATUS_RISING )
        {
            return TRUE;
        }
    }

    return FALSE;
}

static F32 SetupLPM(void)
{
    F32 mLPM;
    I16 mStep;

    // 1. 목표 LPM 계산
    CalcHeaterTargetLPM();                          
    mLPM = GetHotWaterTargetLPM();

    // 2. 목표 유량 조절( 상대 위치 추정 )
    mLPM = SetMinMaxF32( mLPM, 0.3f, 1.0f );
    mStep = GetFlowValveStep( mLPM );
    if( IsCalSampleLPM() == TRUE )
    {
        // 2-1.  상대 위치 추정 
        FlowValveOffset( mStep ); 

        // sync...
        SetFlowValveSampleLPM( mLPM, GetFlowValveTarget() );
    }
    else
    {
        // 2-2.  절대 위치 추정
        FlowValveTarget( mStep ); 
    }

    return mLPM;

}

static void SetupHeaterOutput(F32 mLPM)
{
    U16 mu16Power;

    // 3. 목표 WATT 설정
    CalcHeaterTargetWatt( mLPM );                   
    mu16Power = CalcHeaterOutput();

    SetHeaterOutput( mu16Power );   // 히터 출력 ON
}


// 예열 타입 반환
PreHeatType_T CheckPreHeatType( TEMP_T tHeater, TEMP_T tPreheat, TEMP_T tTarget)
{
    if( tHeater < tPreheat )
    {
        return LOW_PREHEAT;
    }
    else if( tHeater > tTarget )
    {
        return OVER_PREHEAT;
    }

    return MIDDLE_PREHEAT;
}



// 0.5초 동안 0.2LPM 이상 측정
#define PRE_HEAT_OUTAGE_CHECK_TIME  50   // 500ms
#define MIN_PREHEAT_LPM             0.2f 
U16 gu16PreHeatOutageCheckTime = 0;
static void InitPreHeatOutage(void)
{
    gu16PreHeatOutageCheckTime = 0;
}

static U8 IsGoodPreHeatOutage(void)
{
    if( GetFlowMeterLPM() >= MIN_PREHEAT_LPM )
    {
        if( ++gu16PreHeatOutageCheckTime > PRE_HEAT_OUTAGE_CHECK_TIME )
        {
            return TRUE;
        }
    }
    else
    {
        gu16PreHeatOutageCheckTime = 0;
    }

    return FALSE;
}

static TEMP_T GetCoolingTemp(void)
{
#if 1
    U8 mu8Sel = SEL_HOT_USER;
	/*.. sean [24-04-05] 높은 온도에서 낮은온도 전환시에, cooling 기준 추가 ..*/
    mu8Sel = GetHotSelect();
    if( ( GetHotSelect() == SEL_HOT_USER ) || ( GetHotSelect() == SEL_HOT_DEGREE_95 ) || ( GetHotSelect() == SEL_HOT_DEGREE_90 )  )
    {
        return 95.0f;
    }
    else if( ( mu8Sel == SEL_HOT_COFFEE ) || ( GetHotSelect() == SEL_HOT_DEGREE_80 ) || ( GetHotSelect() == SEL_HOT_DEGREE_75 ) )
    {
        return 75.0f;
    }
    else if( ( mu8Sel == SEL_HOT_TEA ) || ( GetHotSelect() == SEL_HOT_DEGREE_65 ) || ( GetHotSelect() == SEL_HOT_DEGREE_60 ) )
    {
        return 70.0f;
    }
    else if( ( GetHotSelect() == SEL_HOT_DEGREE_55 ) || ( GetHotSelect() == SEL_HOT_DEGREE_50 ) )
    {
        return 65.0f;
    }
    else 
    {
        //if( mu8Sel == SEL_HOT_MILK )
        //return 55.0f;
        return 60.0f;
    }
#else
    TEMP_T tIn;

    tIn = GetTemp( TEMP_ID_HOT_IN );
    if( tIn >= 40.0f )
    {
        return tIn;
    }

    return 40.0f;
#endif
}

static U8 IsGoodCoolingHeater(TEMP_T tTarget, TEMP_T tHeater)
{
    if( tTarget >= tHeater )
    {
        return TRUE;
    }

    return FALSE;
}


static void ValveCheckOutage(void)
{
    CloseValve( VALVE_HOT_OUT );
    OpenValve( VALVE_HOT_DRAIN );
    OpenValve( VALVE_HOT_IN );
}

static void ValveReady(void)
{
    CloseValve( VALVE_HOT_OUT );
    OpenValve( VALVE_HOT_DRAIN );
    CloseValve( VALVE_HOT_IN );
}

static void ValvePreHeating(void)
{
    CloseValve( VALVE_HOT_OUT );
    OpenValve( VALVE_HOT_DRAIN );
    CloseValve( VALVE_HOT_IN );
}



//1. 히터 내부 온도가 예열 온도 보다 낮으면 예열을 시작한다. 
//2. 단수 검사를 한다. ( 50ms ) 짧게 밸브를 열고 닫는다.
//   이때, 단수이면 예열을 중지한다.
//3. 목표 유량을 계산해서 유량 조절 밸브를 조정한다.
//4. 히터 온도가 예열 온도에 도달하면 출수로 전환한다.

enum
{
    LOW_INIT,
    LOW_CHECK_OUTAGE,
    LOW_HEATING,
    LOW_DONE,
};


typedef struct _preheat_low_
{
    U8  Step;
    F32 LPM;        // Target LPM
} PreHeatLow_T;

PreHeatLow_T PreHeatLow;

static U8 PreHeatingLow(void)
{
    switch( PreHeatLow.Step )
    {
        case LOW_INIT:
            // 30초 이내 재추출인 경우 단수 검사 Skip
            if( GetHotOutLastedTimeOut() < RECHECK_OUTAGE_TIME 
                    && GetHotOutChangedSel() == FALSE )
            {
                InitPreHeatOutage();

                ValveReady();
                PreHeatLow.LPM = SetupLPM();
                PreHeatLow.Step = LOW_HEATING;
            }
            else
            {
                ClearHotOutChangedSel();

                ValveCheckOutage();
                StartCheckOutage( OUTAGE_HOT_OUT_DELAY_TIME, OUTAGE_CHECK_TIME_3SEC, TYPE_HOT_WATER );

                InitPreHeatOutage();

                PreHeatLow.LPM = SetupLPM();
                PreHeatLow.Step++;
            }
            break;

        case LOW_CHECK_OUTAGE:
            if( IsGoodPreHeatOutage() == TRUE )
            {
                ValvePreHeating();
                StopCheckOutage();
                PreHeatLow.Step++;
            }
            break;

        case LOW_HEATING:
            CheckPreHeatDispLevel( GetHotOutPreHeatTemp(), GetTemp( TEMP_ID_HEATER ));
            Heating();  // HEATING...

            // CHECKING REACH TARGET TEMP...
            if( CheckHeating() == TRUE )
            {
                if( ( GetHotSelect() == SEL_HOT_USER ) || ( GetHotSelect() == SEL_HOT_DEGREE_95 ) || ( GetHotSelect() == SEL_HOT_DEGREE_90 )  )
                {
                    if( GetTemp( TEMP_ID_HEATER ) >= GetHotOutPreHeatTemp()
                            && PreHeat.HeaterOnTime > 400 )
                    {
                        PreHeatLow.Step++;
                    }
                    else if( GetTemp( TEMP_ID_HEATER ) >= 75.0f )
                    {
                        PreHeatLow.Step++;
                    }
                }
                else 
                {
                    PreHeatLow.Step++;
                }
            }
            break;

        case LOW_DONE:
            DBG_CheckPreHeatTemp();

            SetupHeaterOutput( PreHeatLow.LPM );

            PreHeatLow.Step = 0;
            return TRUE;

        default:
            break;
    }

    if( IsTimeOutPreHeating() == TRUE )
    {
        PreHeatLow.Step = LOW_DONE;
    }

    return FALSE;
}

static U8 IsReCheckOutage(void)
{      
    // 고온수 출수시 상시 단수 검사
    if( ( GetHotSelect() == SEL_HOT_USER ) || ( GetHotSelect() == SEL_HOT_DEGREE_95 ) || ( GetHotSelect() == SEL_HOT_DEGREE_90 )  )
    {
        if( GetHotOutLastedTimeOut() > RECHECK_OUTAGE_TIME  )
        {
            return FALSE;   // 즉시 재추출
        }

        if( GetTemp( TEMP_ID_HEATER ) >= 103.0f )
        {
            return TRUE;    // 단수 검사
        }
    }
#if 0
    else if( GetHotSelect() == SEL_HOT_MILK )
    {
        // 분유 재추출시 히터 내부 온도가 너무 높으면, 
        // 예열 동작
        if( GetTemp( TEMP_ID_HEATER ) >= 60.0f )
        {
            return TRUE;    // 단수 검사...
        }
    }
#endif

    //if( GetTemp( TEMP_ID_HEATER ) + 15.0f  <= GetHotOutTargetOutTemp() 
    //        || GetTemp( TEMP_ID_HEATER ) >= 100.0f )
    //{
    //    return FALSE;
    //}

    // 재추출은 단수 검사 없이 재추출
    if( GetHotOutLastedTimeOut() < RECHECK_OUTAGE_TIME 
            && GetTemp( TEMP_ID_HEATER ) + 7.0f  <= GetHotOutTargetOutTemp() )
    {
        return TRUE;    //단수 검사
    }

    return FALSE;   // 즉시 재추출
}

enum
{
    MID_INIT,
    MID_CHECK_OUTAGE,
    MID_HEATING,
    MID_DONE,
};


typedef struct _preheat_middle_
{
    U8  Step;
    F32 LPM;
} PreHeatMid_T;

PreHeatMid_T PreHeatMid;

static U8 PreHeatingMiddle(void)
{
    switch( PreHeatMid.Step )
    {
        case MID_INIT:
            // 30초 이내 재추출인 경우 단수 검사 Skip
            if( IsReCheckOutage() == FALSE 
                    && GetHotOutChangedSel() == FALSE )
            {
                InitPreHeatOutage();

                ValveReady();
                PreHeatMid.LPM = SetupLPM();
                PreHeatMid.Step = MID_HEATING;
            }
            else
            {
                ClearHotOutChangedSel();
                ValveCheckOutage();
                StartCheckOutage( OUTAGE_HOT_OUT_DELAY_TIME, OUTAGE_CHECK_TIME_3SEC, TYPE_HOT_WATER );

                InitPreHeatOutage();

                PreHeatMid.LPM = SetupLPM();
                PreHeatMid.Step++;
            }
            break;

        case MID_CHECK_OUTAGE:
            if( IsGoodPreHeatOutage() == TRUE )
            {
                ValvePreHeating();
                StopCheckOutage();
                PreHeatMid.Step++;
            }
            break;

        case MID_HEATING:
            CheckPreHeatDispLevel( GetHotOutPreHeatTemp(), GetTemp( TEMP_ID_HEATER ));
            Heating();  // HEATING...

            // CHECKING REACH TARGET TEMP...
#if 0
            if( CheckHeating() == TRUE )
            {
                PreHeatMid.Step++;
            }
#else
            if( CheckHeating() == TRUE )
            {
                if( ( GetHotSelect() == SEL_HOT_USER ) || ( GetHotSelect() == SEL_HOT_DEGREE_95 ) || ( GetHotSelect() == SEL_HOT_DEGREE_90 )  )
                {
                    if( GetTemp( TEMP_ID_HEATER ) >= 80.0f 
                            || PreHeat.HeaterOnTime >= 400 )
                    {
                        PreHeatMid.Step++;
                    }
                    else if( GetTemp( TEMP_ID_HEATER ) > 85.0f )
                    {
                        PreHeatMid.Step++;
                    }
                }
                else 
                {
                        PreHeatMid.Step++;
                }
            }
#endif
            break;

        case MID_DONE:
            DBG_CheckPreHeatTemp();

            SetupHeaterOutput( PreHeatMid.LPM );

            PreHeatMid.Step = 0;
            return TRUE;

        default:
            break;
    }

    if( IsTimeOutPreHeating() == TRUE )
    {
        PreHeatMid.Step = MID_DONE;
    }

    return FALSE;
}


enum
{
    OVER_INIT,
    OVER_CHECK_OUTAGE,
    OVER_COOLING_HEATER,
    OVER_COOLING_WAIT,
    OVER_HEATING,
    OVER_DONE,
};


typedef struct _preheat_over_
{
    U8  Step;
    F32 LPM;
} PreHeatOver_T;

PreHeatOver_T PreHeatOver;
static U8 PreHeatingOver(void)
{
    static U16 mu16Delay = 0;

    switch( PreHeatOver.Step )
    {
        case OVER_INIT:

            mu16Delay = 0; // 500ms...
            // 1. 시간 이내에 재출하면 단수 검사 없이 추출.
            if( IsReCheckOutage() == FALSE 
                    && GetHotOutChangedSel() == FALSE )
            {
                InitPreHeatOutage();

                ValveReady();
                PreHeatOver.LPM = SetupLPM();
                PreHeatOver.Step = OVER_HEATING;
            }
            else
            {
                ClearHotOutChangedSel();
                ValveCheckOutage();
                StartCheckOutage( OUTAGE_HOT_OUT_DELAY_TIME, OUTAGE_CHECK_TIME_3SEC, TYPE_HOT_WATER );

                InitPreHeatOutage();

                PreHeatOver.LPM = SetupLPM();
                PreHeatOver.Step++;
            }
            break;

        case OVER_CHECK_OUTAGE:
            if( IsGoodPreHeatOutage() == TRUE )
            {
                PreHeatOver.Step++;
            }
            break;

        case OVER_COOLING_HEATER:
            //if( IsGoodCoolingHeater( GetHotOutTargetOutTemp(), GetTemp( TEMP_ID_HEATER )) == TRUE )
            if( IsGoodCoolingHeater( GetCoolingTemp(), GetTemp( TEMP_ID_HEATER )) == TRUE )
            {
                ValvePreHeating();
                StopCheckOutage();
                mu16Delay = 50; // 500ms...

                PreHeatOver.Step++;
            }
            break;

        case OVER_COOLING_WAIT:
            if( mu16Delay != 0 )
            {
                mu16Delay--;
                return;
            }

            PreHeat.HeaterConfOnTime = 250; // 2.5sec
            if( GetHeaterClass() == HEATER_CLASS_LOW )
            {
                if( GetTemp( TEMP_ID_HOT_IN ) < 20.0f )
                {
                    PreHeat.HeaterConfOnTime = 350; // 3.5sec
                }
            }
            PreHeatOver.Step++;
            break;
            
        case OVER_HEATING:
            CheckPreHeatDispLevel( GetHotOutPreHeatTemp(), GetTemp( TEMP_ID_HEATER ));
            Heating();  // HEATING...

            // CHECKING REACH TARGET TEMP...
            if( CheckHeating() == TRUE )
            {
                PreHeatOver.Step++;
            }
            break;

        case OVER_DONE:
            DBG_CheckPreHeatTemp();

            SetupHeaterOutput( PreHeatOver.LPM );

            PreHeatOver.Step = 0;
            return TRUE;

        default:
            break;
    }

    if( IsTimeOutPreHeating() == TRUE )
    {
        PreHeatOver.Step = OVER_DONE;
    }

    return FALSE;
}


static U16 GetPreHeatMaxTimeOut(void)
{
	/* sean [24-02-22] Heater_ClASS_MID 및 LOW에 대한 조건문 추가 */
	if( PreHeat.Type == OVER_PREHEAT )
	{
        return MAX_TIME_OUT_PREHEAT_VERY_LONG;
	}
    else {}
		/* sean [24-02-22] Heater_ClASS_MID 및 LOW에 대한 조건문 추가 */
    if( GetHotSelect() == SEL_HOT_USER 
			|| GetHotSelect() == SEL_HOT_DEGREE_95
			|| GetHotSelect() == SEL_HOT_DEGREE_90
            || GetHotSelect() == SEL_HOT_COFFEE
			|| GetHotSelect() == SEL_HOT_DEGREE_80 )
    {
    	if( GetTemp( TEMP_ID_HOT_IN ) <= 18.0f 
                || GetHeaterClass() != HEATER_CLASS_HIGH )
        {
            return MAX_TIME_OUT_PREHEAT_LONG;
        }
		/* sean [24-02-22] Heater_ClASS_MID 및 LOW에 대한 조건문 추가 */
		else
		{
            return MAX_TIME_OUT_PREHEAT_LONG;
		}
    }

    return MAX_TIME_OUT_PREHEAT;
}

enum
{
    PRE_HEAT_INIT,
    PRE_HEAT_DOING,
    PRE_HEAT_DONE
};

U8 DoPreHeating(U8 *pStep)
{
    switch( *pStep )
    {
        case PRE_HEAT_INIT:

            // 예열 타입 판단.
            PreHeat.Type = CheckPreHeatType( 
                    GetTemp( TEMP_ID_HEATER ), 
                    GetHotOutPreHeatTemp(),
                    GetHotOutTargetOutTemp()
                    );

            InitHeaterTemp();
            PreHeatLow.Step = 0;
            PreHeatMid.Step = 0;
            PreHeatOver.Step = 0;
            PreHeat.HeatingTime     = 0;
            PreHeat.MaxTimeOut      = GetPreHeatMaxTimeOut();

            PreHeat.HeaterOnTime     = 0;
            PreHeat.HeaterConfOnTime = GetMinHeatingTime();

            SetHeaterTempTargetOut( GetHotOutTargetOutTemp() );
            InitPreHeatDispLevel( GetHotOutPreHeatTemp(), GetTemp( TEMP_ID_HEATER ));
            (*pStep)++;
            break;

        case PRE_HEAT_DOING:

            if( PreHeat.Type == LOW_PREHEAT )
            {
                if( PreHeatingLow() == TRUE )
                {
                    (*pStep)++;
                }
            }
            else if( PreHeat.Type == MIDDLE_PREHEAT )
            {
                if( PreHeatingMiddle() == TRUE )
                {
                    (*pStep)++;
                }
            }
            else 
            {
                if( PreHeatingOver() == TRUE )
                {
                    (*pStep)++;
                }
            }
            break;

        case PRE_HEAT_DONE:
            DonePreHeatDispLevel();

            (*pStep) = 0;
            return TRUE;

        default:
            (*pStep) = 0;
            break;
    }

    UpdateHeatingTime();

    return FALSE;
}

