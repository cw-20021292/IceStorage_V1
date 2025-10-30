#include "filter.h"
#include "hal_input.h"
#include "sound.h"
#include "process_display.h"
#include "valve.h"
#include "water_out.h"
#include "flush_water_out.h"
#include "eeprom.h"
#include "cody_kit.h"

#include <stdio.h>
#include <string.h>


#define	ONE_DAY_MIN ( 1440U )	/* 60min x 24hour = 1440 min  */

Filter_T Filter;

void InitFilter(void)
{
    Filter.Status[ ID_FILTER_COVER ]   = FILTER_CLOSE;
    Filter.Status[ ID_FILTER ]         = FILTER_CLOSE;
    Filter.ChangedStatus        = FILTER_MOUNTED;
    Filter.ChangedFlushStatus   = FILTER_CHANGED_NONE;

    Filter.UsageStatus   = FILTER_USAGE_NORMAL;
    Filter.UsageTime     = FILTER_USAGE_TIME;
    Filter.SaveTime      = ONE_DAY_MIN;     // EEPROM에 업데이트 주기 
}


U8 GetFilterChangedFlushStatus(void)
{
    return Filter.ChangedFlushStatus;
}

void SetFilterChangedFlushStatus(U8 mu8Flush)
{
     Filter.ChangedFlushStatus = mu8Flush;
}

U8 GetFilterChangedStatus(void)
{
    return Filter.ChangedStatus;
}

void SetFilterChangedStatus(U8 mu8Changed)
{
    Filter.ChangedStatus = mu8Changed;
}

U8 IsOpenFilter(U8 mu8Id)
{
    if( Filter.Status[ mu8Id ] == FILTER_OPEN )
    {
        return TRUE;    // OPEN
    }

    return FALSE;   // CLOSE
}

void SetFilterStatus(U8 mu8Id, U8 mu8Status)
{
    Filter.Status[ mu8Id ] = mu8Status;
}

U8 GetFilterStatus(U8 mu8Id)
{
    return Filter.Status[ mu8Id ];
}


// 필터 사용 상태
void SetFilterUsageStatus(U8 mu8Status)
{
    Filter.UsageStatus = mu8Status;
}

U8 GetFilterUsageStatus(void)
{
    return Filter.UsageStatus;
}

// 필터 사용시간
void SetFilterUsageTime(U32 mu32Time)
{
    Filter.UsageTime = mu32Time;

    SaveEepromId( EEP_ID_FILTER_USAGE_TIME );
}

U32 GetFilterUsageTime(void)
{
    return Filter.UsageTime;
}

void ResetFilterUsage(void)
{
    SetFilterUsageStatus( FILTER_USAGE_NORMAL );
    SetFilterUsageTime( FILTER_USAGE_TIME );
}


 void CheckFilterStatus(void)
{
    if( HAL_GetInputValue( IN_FILTER ) == HIGH )
    {
        Filter.Status[ ID_FILTER ] = FILTER_OPEN;
    }
    else
    {
        Filter.Status[ ID_FILTER ] = FILTER_CLOSE;
    }
}

#define INIT_DELAY_TIME 50      // 5sec..
#define DELAY_TIME      20      // 2sec..
#define OPEN_TIME       70      // 7sec..

U8 the_filter_open = FALSE;
static void CheckOpenFilterCover(void)
{
    static U8 mu8CheckFilter = FALSE;
    static U8 mu8Check      = FALSE;
    static U8 mu8Delay      = OPEN_TIME;      
    static U8 mu8InitDelay  = INIT_DELAY_TIME;    
    static U8 mu8DelayWaterOut  = 0;      
    //static U8 mu8Open           = 0;      


    if( GetDisplayInit() == FALSE )
    {
        return ;
    }

    // 코디 키트 유로 살균 중에는 SKIP
    if( GetCodyKitStatus() == TRUE )
    {
        return ;
    }

    if( mu8InitDelay != 0 )
    {
        mu8InitDelay--;
        return ;
    }

    if( IsOpenFilter( ID_FILTER_COVER ) == FILTER_OPEN 
            || IsOpenFilter( ID_FILTER ) == FILTER_OPEN )
    {
        // 추출 중이면, 우선 추출 중지
        if( GetWaterOut() == TRUE )
        {
            StopWaterOut();
            Sound( SOUND_EFFLUENT_END );

            mu8DelayWaterOut = DELAY_TIME;
            return ;
        }

        // 추출 동작이 완전히 마무리 될 때까지 대기
        if( mu8DelayWaterOut != 0 )
        {
            mu8DelayWaterOut--;
            return ;
        }

        if( mu8Check == FALSE )
        {
            mu8Check = TRUE;

            SetFilterDisp( TRUE );

            Sound( SOUND_COVER_OPEN_FRONT );
            
            if( IsOpenValve( VALVE_ICE_TRAY_IN ) == FALSE )
            {
                OpenValve( VALVE_ICE_TRAY_IN );
                the_filter_open = TRUE;
            }
        }

        // 필터 레버는 열려 있고, 필터 커버는 닫혀 있는 상황
        // 필터 커버가 열려 있다고 소리 출력
        if( IsOpenFilter( ID_FILTER_COVER ) == FILTER_CLOSE 
                && IsOpenFilter( ID_FILTER ) == FILTER_OPEN )
        {
            if( mu8CheckFilter == FALSE )
            {
                mu8CheckFilter = TRUE;
                Sound( SOUND_FILTER_CHECK );
            }
        }
        else
        {
            mu8CheckFilter = FALSE;
        }
    }
    else
    {
        if( mu8Check == TRUE )
        {
            if( Filter.ChangedStatus == FILTER_CHANGED )
            {
                // 펄터 OPEN -> CLOSE
                Sound( SOUND_FILTER_FLUSHING_READY );
            }
            else
            {
            //    if( IsOpenFilter( ID_FILTER ) == FILTER_OPEN )
            //    {
            //        Sound( SOUND_FILTER_FLUSHING_READY );
            //    }
            }
        }
        mu8Check = FALSE;

        SetFilterDisp( FALSE );

        // tray가 open 상태이면... 7초간 open 상태 유지...
        if( the_filter_open == FALSE )
        {
            mu8Delay = OPEN_TIME;
        }
        else
        {
            mu8Delay = 0;   
        }
        mu8CheckFilter = FALSE;
    }

    
    if( mu8Delay != 0 )
    {
        mu8Delay--;
    }
    else
    {
        if( the_filter_open == TRUE )
        {
            the_filter_open = FALSE;
            CloseValve( VALVE_ICE_TRAY_IN );
        }
    }
}

// 필터가 교체되는지 검사한다.
// 1. 필터 커버가 OPEN 상태
// 2. 필터 레버가 OPEN -> CLOSE 되면 교체로 인식한다.
// 3. 필터 커버가 CLOSE 상태

static void CheckChangedFilterResetFlushOut(void)
{
    static U8 mu8PrevFilter = FILTER_CLOSE;
    U8 mu8CurFilter;


    if( GetFilterStatus( ID_FILTER_COVER ) == FILTER_OPEN )
    {
        mu8CurFilter = GetFilterStatus( ID_FILTER );
        if( mu8PrevFilter != mu8CurFilter )
        {
            mu8PrevFilter = mu8CurFilter;
            if( mu8CurFilter == FILTER_CLOSE 
                    && Filter.ChangedStatus != FILTER_CHANGED)
            {
                Filter.ChangedStatus = FILTER_CHANGED;
                SaveEepromId( EEP_ID_FILTER_CHANGED );
            }
        }
    }
    else
    {
        // 필터 CLOSED, 필터 레버가 OPEN->CLOSED된 상태
        if( GetFilterStatus( ID_FILTER ) == FILTER_CLOSE 
                && Filter.ChangedStatus == FILTER_CHANGED )
        {
            Filter.ChangedStatus = FILTER_MOUNTED;

            // RESET FILTER!!!!
            ResetFlushWaterOut();   // 필터 플러싱 재설정
        }
    }
}

// 필터 잔여 사용 시간을 카운트 한다.
void CountFilterUsage(void)
{
    if( IsOpenFilter( ID_FILTER ) == FILTER_OPEN )
    {
        return ;
    }

    if( Filter.UsageTime != 0 )
    {
        Filter.UsageTime--;
    }


    if( Filter.UsageStatus == FILTER_USAGE_NORMAL )
    {
        if( Filter.UsageTime == 0 )
        {
            // TIME OUT!!!
            SetFilterUsageStatus( FILTER_USAGE_TIME_OUT );
            SaveEepromId( EEP_ID_FILTER_USAGE_TIME );
        }
        else
        {
            // NORMAL!!! 
            if( Filter.SaveTime !=  0 )
            {
                Filter.SaveTime--;
            }
            else
            {
                // SAVE TIME EVERY DAY.
                Filter.SaveTime = ONE_DAY_MIN;
                SaveEepromId( EEP_ID_FILTER_USAGE_TIME );
            }
        }
    }
}

#if CONFIG_TEST_FILTER
void TEST_CheckFilter(void)
{
    static U8 mu8Prev = FILTER_CLOSE;
    static U8 mu8Cur = FILTER_CLOSE;

    mu8Cur = GetFilterStatus( ID_FILTER );
    if( mu8Cur != mu8Prev  )
    {
        if( mu8Cur == FILTER_OPEN )
        {
            Sound( SOUND_SELECT );
        }
        else
        {
            Sound( SOUND_ERROR );
        }

        mu8Prev = mu8Cur;
    }
}
#endif

void ProcessFilter(void)
{
    // 필터 상태 업데이트
    CheckFilterStatus();

    // 필터 커버 상태 업데이트
    CheckOpenFilterCover();

    CheckChangedFilterResetFlushOut();

#if CONFIG_TEST_FILTER
    SetInputConfDetectTime( IN_FILTER , INPUT_DELAY_100MS );   
    TEST_CheckFilter();
#endif
}

