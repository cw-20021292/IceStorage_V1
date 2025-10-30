#include "hw.h"
#include "hal_adc.h"
#include "leak.h"


#define  LEAK_ERROR_COUNT           30 /* 3sec, @100ms */

#define  DEFAULT_WAIT_TIME          50 /* 5sec, @100ms */
#define  DEFAULT_CHECK_TIME         90 /* 9sec, @100ms */
#define  DEFAULT_CHECK_WAIT_TIME   ( DEFAULT_CHECK_TIME - 10 )  

Leakage_T  Leak;


void InitLeak(void)
{
    Leak.Status = FALSE;
    
    Leak.WaitTime    = 0;
    Leak.CheckTime   = DEFAULT_CHECK_TIME;  
    Leak.CheckCount  = 0; 

    TURN_ON_LEAK_ONOFF();
}

void GetLeakData( Leakage_T *pData )
{
    MEMCPY( (void __far *)pData, (const void __FAR *)&Leak, sizeof( Leakage_T ));
}

U8 GetLeakStatus(void)
{
    return Leak.Status;
}

U8 IsLeakSemi(void)
{
    U16 mu16Adc = 0;

    TURN_ON_LEAK_ONOFF();

    HAL_InitAdc();
    mu16Adc = HAL_GetAdcValue( ANI_SENSOR_LEAK );
    if( mu16Adc > 50 )
    {
        return TRUE;
    }

    return FALSE;
}

void ProcessLeak(void)
{
    // 검사 휴지 시간 대기
    if( Leak.WaitTime != 0 )
    {
        Leak.WaitTime--;

        Leak.CheckTime = DEFAULT_CHECK_TIME;
        Leak.CheckCount = 0;
        TURN_OFF_LEAK_ONOFF();
        return ;
    }


    // 검사 시간 만료인 경우, 중지
    if( Leak.CheckTime == 0 )
    {
        Leak.WaitTime   = DEFAULT_WAIT_TIME;

        // 에러 판정
        if( Leak.CheckCount >= LEAK_ERROR_COUNT )
        {
            Leak.Status = TRUE;     // 누수 감지
        }
        else
        {
            Leak.Status = FALSE;    // 누수 감지 없음
        }

        return ;
    }
    else
    {
        Leak.CheckTime--;
    }


    // 검사 동작~!
    TURN_ON_LEAK_ONOFF();
    if( Leak.CheckTime < DEFAULT_CHECK_WAIT_TIME )
    {
        U16 mu16Adc;

        mu16Adc = HAL_GetAdcValue( ANI_SENSOR_LEAK );
        if( mu16Adc >= LEAK_ADC_LEVEL )
        {
            Leak.CheckCount++;
        }
        else
        {
            Leak.CheckCount = 0;
        }
    }
}

