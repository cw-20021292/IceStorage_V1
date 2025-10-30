#include "hal_pump.h"
#include "drain_pump.h"
#include "hal_adc.h"

#include <string.h>

#define RETRY_COUNT         25//24
#define DEFAULT_LOAD_TIME   5
#define DEFAULT_DEAD_TIME   5

// drain pump 무부하 조건 port 입력 전압 : 0.44 ~ 0.66[v] == 90 ~ 135[adc]
// 계산치는 : 135 [adc]
// 측정치는 : 140 [adc]
// 적용수치는 :  175[adc]  계산치에 + 30% 가산
//
// [펌프 ADC 데이터 측정]
// 구분	                    ADC
// 무부하	                110
// 부하 - 짧은 유로	        265 ~ 271
// 부하 - 긴 유로	        300 ~330
// 보호동작 걸리는 시점	    390 ~ 400
// 보호 동작 발생 후	    20

#define PROTECT_DEAD_ADC    40
#define NO_LOAD_ADC         175

DrainPump_T  DrainPump;

void InitDrainPump(void)
{
    DrainPump.Cmd          = OFF;
    DrainPump.RetryCmd     = OFF;
    DrainPump.RetryCount   = 0;
    DrainPump.OnOffTime    = 0;
    DrainPump.Error        = FALSE;

    // 펌프 부하 검사
    DrainPump.LoadTime     = DEFAULT_LOAD_TIME;
    DrainPump.Load         = FALSE;
}

void GetDrainData(DrainPump_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&DrainPump, sizeof( DrainPump_T ) );
}

U8 GetDrainPumpStatus(void)
{
    return DrainPump.Cmd;
}

U8 GetDrainPumpError(void)
{
    return DrainPump.Error;
}

void TurnOnDrainPump(void)
{
    DrainPump.Cmd  = ON;
}

U8 IsTurnOnDrainPump(void)
{
    if( DrainPump.RetryCmd != 0 )
    {
        return TRUE;
    }

    return FALSE;
}


void TurnOffDrainPump(void)
{
    DrainPump.Cmd = OFF;

    DrainPump.RetryCmd     = 0;
    DrainPump.RetryCount   = 0;
    DrainPump.OnOffTime    = 0;
}

void TurnOffDrainPumpEx(void)
{
    TurnOffDrainPump();
    HAL_TurnOffPump( HAL_PUMP_DRAIN );
}

// 0이면, OFF
// 0이 아니면 제어 전압
static const U8 OnOffCmdList[] = 
//U8 OnOffCmdList[] = 
{
    24,    
    5,     0,        // 0 1
    24,    0,        // 2 3
    24,    0,        // 4 5
    24,    0,        // 6 7
    24,    0,        // 8 9
    24,    0,        // 10 11
    24,    0,        // 12 13 
    24,    0,        // 14 15
    24,    0,        // 16 17
    24,    0,        // 18 19
    24,    0,        // 20 21
    24,    0,        // 22 23
    24                  
};

// 배수 시간, 휴지 시간
static const U16 OnOffTimeList[] = 
{ 
    3,              // 0-1  ( 3초간 24V 제어 )
    597,    60,     // 0-2
    120,    60,     // 1
    120,    60,     // 2 
    120,    60,     // 3
    120,    60,     // 4
    120,    60,     // 5
    120,    60,     // 6
    120,    60,     // 7
    120,    60,     // 8
    120,    60,     // 9
    120,    60,     // 10
    120,    60,     // 11
    120   
};


// 배수 펌프 부하 확인 ( 물이 없는 경우 )
// RETURN
//  - TRUE - 부하가 있음.
//  - FALSE - 부하가 없음
U8 GetDrainPumpLoadStatus(void)
{
    return DrainPump.Load;
}

static void InitDrainPumpLoad(void)
{
    DrainPump.Load      = TRUE;
    DrainPump.LoadTime  = DEFAULT_LOAD_TIME;

    DrainPump.Dead      = TRUE;
    DrainPump.DeadTime  = DEFAULT_DEAD_TIME;
}

static void CheckDrainPumpLoad(void)
{
    U16 mu16Adc;

    mu16Adc = HAL_GetAdcValue( ANI_PUMP_DRAIN_FB );

    // Count load time
    if( mu16Adc < PROTECT_DEAD_ADC )
    {
        if( DrainPump.DeadTime != 0 )
        {
            DrainPump.DeadTime--;
        }
        else
        {
            DrainPump.Dead = TRUE;
        }
    }
    else
    {
        DrainPump.Dead = FALSE;
        DrainPump.DeadTime = DEFAULT_DEAD_TIME;
    }

    if( mu16Adc < NO_LOAD_ADC )
    {
        if( DrainPump.LoadTime != 0 )
        {
            DrainPump.LoadTime--;
        }

        if( DrainPump.LoadTime == 0 )
        {
            DrainPump.Load = FALSE;
        }

        return ;
    }

    // Reload load time
    DrainPump.Load = TRUE;
    DrainPump.LoadTime = DEFAULT_LOAD_TIME;
}

// 최초 기동시 24V 제어
// 3초후 19V로 제어
// 2번째 부터 다시 24V로 제어
void ControlDrainPump(void)
{
    U8 mu8OnOff;

    if( DrainPump.Cmd == OFF 
            || DrainPump.Error == TRUE )
    {
        HAL_TurnOffPump( HAL_PUMP_DRAIN );
        InitDrainPumpLoad();
        return ;
    }


    if( DrainPump.OnOffTime == 0 )
    {
        DrainPump.RetryCmd  = OnOffCmdList[ DrainPump.RetryCount ];
        DrainPump.OnOffTime = OnOffTimeList[ DrainPump.RetryCount ];

        DrainPump.RetryCount++;
        if( DrainPump.RetryCount > RETRY_COUNT )
        {
            DrainPump.Error = TRUE;
        }

        return ;
    }


    if( DrainPump.OnOffTime != 0 )
    {
        DrainPump.OnOffTime--;
    }

    if( DrainPump.RetryCmd != 0 )
    {
        if( DrainPump.Dead == TRUE )
        {
            // 펌프가 protection으로 off되면, 펌프 전원을 off해서 reset 시킨다.
            DrainPump.Dead      = FALSE;
            DrainPump.DeadTime  = DEFAULT_DEAD_TIME;
            HAL_TurnOffPump( HAL_PUMP_DRAIN );
        }
        else
        {
            HAL_TurnOnPump( HAL_PUMP_DRAIN, (F32)DrainPump.RetryCmd );
        }

        CheckDrainPumpLoad();
    }
    else
    {
        HAL_TurnOffPump( HAL_PUMP_DRAIN );
    }
}

