#include "err_temp_hot_overheat.h"
#include "err_temp.h"
#include "hot_water.h"
#include "hot_water_out.h"
#include "temp.h"

// icon 2.0 - 106도 (히터 내부/추출 )
// noble 세로/가로 - 102도 ( only 추출 온도 )

#define TEMP_OVERHEAT_HEATER        106.0f
#define TEMP_OVERHEAT_OUT           106.0f  // noble 102.0f 

#define DETECT_TIME             5U      // 0.5sec @100ms
#define AUTO_RELEASE_TIME       100U    // 10sec @100ms

// 온수 추출 중... 온수 출수 온도가 기준치 이상 조건이
// 0.5초 이상 지속되면 에러 발생

typedef struct _temp_heater_over_heat_
{
    TEMP_T  tHeater;
    TEMP_T  tHotOut;

    U16 DetectTime;
    U16 ReleaseTime;
} OverHeat_T;

OverHeat_T  OverHeat;

static void ClearOverHeat(void)
{
    OverHeat.DetectTime     = DETECT_TIME;
    OverHeat.ReleaseTime    = AUTO_RELEASE_TIME;
}

U8 dbg_the_hot_err = 0;
U8 CheckErrHeaterOverheat(U8 mu8Error)
{
    if( IsHotOut() == FALSE )
    {
        ClearOverHeat();
        return FALSE;
    }

    // 고온수 추출 중에는 검사 하지 않음
    if(  ( GetHotSelect() == SEL_HOT_USER ) || (GetHotSelect() == SEL_HOT_DEGREE_95 ) || (GetHotSelect() == SEL_HOT_DEGREE_90 ) )
    {
        ClearOverHeat();
        return FALSE;
    }

    OverHeat.tHeater = GetTemp( TEMP_ID_HEATER );
    OverHeat.tHotOut = GetTemp( TEMP_ID_HOT_OUT );

    if( dbg_the_hot_err )
    {
        OverHeat.tHotOut = 110.0f;
    }

    if( OverHeat.tHeater >= TEMP_OVERHEAT_HEATER 
            || OverHeat.tHotOut >= TEMP_OVERHEAT_OUT )
    {
        if( OverHeat.DetectTime != 0 )
        {
            OverHeat.DetectTime--;
        }
        else
        {
            SetHotWaterCooling( TRUE );
            return TRUE;    // 에러 발생
        }
    }
    else
    {
        OverHeat.DetectTime = DETECT_TIME;
    }


    OverHeat.ReleaseTime = AUTO_RELEASE_TIME;
    return FALSE;   // 정상
}


U8 ReleaseErrHeaterOverheat(U8 mu8Error)
{
    // 에러 자동 해제 타이머 동작
    if( OverHeat.ReleaseTime != 0 )
    {
        OverHeat.ReleaseTime--;
    }
    else
    {
        ClearOverHeat();
        return FALSE;    // 에러 해제
    }

    return TRUE;   // 에러 유지
}
