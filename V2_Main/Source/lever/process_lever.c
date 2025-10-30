#include "hw.h"
#include "prj_type.h"

#include "lever.h"
#include "lever_common_handler.h"
#include "lever_handler.h"
#include "process_lever.h"
#include "process_display.h"

#include "key.h"
#include "front.h"
#include "sound.h"
#include "power_saving.h"

#include "ice.h"
#include "water_out.h"
#include "filter.h"
#include "front.h"
#include "flush_water_out.h"
#include "error.h"
#include "cody_kit.h"
#include "empty_cold_tank.h"

#include "eol.h"
#include "eol_lever_handler.h"

#include "fct.h"
#include "fct_lever_handler.h"



static U8 IsValidIceOut(void)
{
    return TRUE;
}

static U8 IsErrorWaterOut(void)
{
    U8 mu8SelWater;

    mu8SelWater = GetWaterOutSelect();
    if( mu8SelWater == SEL_WATER_ROOM
            && IsErrorType( ERR_TYPE_ROOM_WATER_OUT) == TRUE )
    {
        return TRUE;
    }

    if( mu8SelWater == SEL_WATER_COLD
            && IsErrorType( ERR_TYPE_COLD_WATER_OUT) == TRUE )
    {
        return TRUE;
    }

    if( mu8SelWater == SEL_WATER_HOT
            && IsErrorType( ERR_TYPE_HOT_WATER_OUT) == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

static U8 IsValidWaterOut(void)
{
    // 플러싱에서는 단수 에러는 즉시 해제
    if( GetWaterOutSelect() == SEL_WATER_FLUSH )
    {
        ClearOutageError();
        ClearOutageHotError();
    }
    // 단수 에러 해제
    else if( GetWaterOutSelect() == SEL_WATER_ROOM 
         || GetWaterOutSelect() == SEL_WATER_COLD )
    {
        ClearOutageError();
    }

    if( IsErrorWaterOut() == TRUE )
    {
        StartDispTimerId( DISP_TIMER_ERROR );
        Sound( SOUND_ERROR );

        return FALSE;
    }

    if( IsOpenFilter( ID_FILTER_COVER ) == FILTER_OPEN 
            || IsOpenFilter( ID_FILTER ) == FILTER_OPEN )
    {
        // EOL - Skip...
        // CodyKit - SKip..
        if( GetEolStatus() == TRUE 
                || GetCodyKitStatus() == TRUE )
        {
            return TRUE;
        } 

        if( IsOpenFilter( ID_FILTER_COVER ) == FILTER_OPEN )
        {
            Sound( SOUND_ALARM_COVER_OPEN_FRONT );
        }

        if( IsOpenFilter( ID_FILTER ) == FILTER_OPEN )
        {
            Sound( SOUND_FILTER_CHECK );
        }

        return FALSE;
    }

    return TRUE;
}

static U8 IsValidLeverCondition(U8 mu8Id )
{
    // EMPTY COLD TANK MODE
    if( IsStartEmptyColdTank() == TRUE )
    {
        StopEmptyColdTank();
        return FALSE;
    }

    // 설정 메뉴
    if( GetSoundDisp() == TRUE 
            || GetClockDisp() == TRUE
            || GetSettingDisp() == TRUE 
            || GetCustomerDisp() == TRUE 
            )
    {
        return FALSE;
    }


    // COMMON
    // 전체 잠금 중에는 팝업 표시하고 종료.
    if( IsDoneFlushInitSetup() == FALSE )
    {
        if( GetLockAll() == LOCK )
        {
            StartDispTimerId( DISP_TIMER_ALARM_LOCK_ALL );
            Sound( SOUND_ALARM_LOCK_ALL ); 
            return FALSE;
        }
    }

    // WATER or ICE
    if( mu8Id == LEVER_ID_WATER )
    {
        return IsValidWaterOut();   // 물 추출 조건 검사
    }

    return IsValidIceOut(); // 얼음 추출 조건 검사
}


#define LEVER_WAIT_TIME_OUT     60U
#define LEVER_NO_WAIT_TIME_OUT  0U
static U8 EventHandler(U8 mu8Id, LeverEventList_T *pList)
{
    U8 mu8Sound;
    static U8 mu8ErrSkip[ MAX_LEVER_ID ]  = {FALSE};


    if( IsSetLeverEventFlag( mu8Id, LEVER_EVENT_PUSHED_SHORT ) == TRUE )
    {
        ClearLeverEventFlag( mu8Id,  LEVER_EVENT_PUSHED_SHORT );

        if( IsValidLeverCondition( mu8Id ) == FALSE )
        {
            mu8ErrSkip[ mu8Id ] = TRUE;
        }
        else if( (pList + mu8Id )->actionEvent_1 != NULL )
        {
            mu8Sound = (pList + mu8Id)->actionEvent_1();
            Sound( mu8Sound );
        }

        WakeUpSavingMode();
        StartDispTimerId( DISP_TIMER_SLEEP );
        StartDispTimerId( DISP_TIMER_CLOCK );
        ResetUnusedTime();

        return LEVER_WAIT_TIME_OUT;
    }
    // 레버 입력 LONG
    else if( IsSetLeverEventFlag( mu8Id, LEVER_EVENT_PUSHED_LONG ) == TRUE )
    {
        ClearLeverEventFlag( mu8Id, LEVER_EVENT_PUSHED_LONG );

        if( (pList + mu8Id )->actionEvent_2 != NULL )
        {
            mu8Sound = (pList + mu8Id )->actionEvent_2();
            Sound( mu8Sound );
            return LEVER_WAIT_TIME_OUT;
        }
    }
    // 레버 입력 LONG-2
    else if( IsSetLeverEventFlag( mu8Id, LEVER_EVENT_PUSHED_LONG_2 ) == TRUE )
    {
        ClearLeverEventFlag( mu8Id, LEVER_EVENT_PUSHED_LONG_2 );

        if( (pList + mu8Id )->actionEvent_3 != NULL )
        {
            mu8Sound = (pList + mu8Id )->actionEvent_3();
            Sound( mu8Sound );
            return LEVER_WAIT_TIME_OUT;
        }
    }
    // 레버 입력 POP
    else if( IsSetLeverEventFlag( mu8Id, LEVER_EVENT_POP ) == TRUE )
    {
        ClearLeverEventFlag( mu8Id, LEVER_EVENT_POP );

        // 입력시 에러 발생되면, POP 이벤틀르 Skip한다.
        if( mu8ErrSkip[ mu8Id ] == TRUE )
        {
            mu8ErrSkip[ mu8Id ] = FALSE;
        }

        if( (pList + mu8Id )->actionEvent_4 != NULL )
        {
            mu8Sound = (pList + mu8Id )->actionEvent_4();
            Sound( mu8Sound );
            return LEVER_WAIT_TIME_OUT;
        }

    }

    return LEVER_NO_WAIT_TIME_OUT;
}

void ProcessLeverEventHandler(void)
{
    LeverEventList_T  *pEventList = NULL;
    U8 mu8KeyEventSize = 0;
    U8 mu8Ret = FALSE;
    U8 mu8TimeOut = 0;
    static U16 mu16DelayWater = 0;     /* 키 입력 간 지연 시간 */
    static U16 mu16DelayIce = 0;     /* 키 입력 간 지연 시간 */


    if( GetDisplayInit() == FALSE )
    {
        return ;
    }


    if( GetEolStatus() == TRUE )
    {
        // EOL
        pEventList = (LeverEventList_T *)GetEolLeverEventList();
    }
    else if( GetFctStatus() == TRUE )
    {
        // FCT
        pEventList = (LeverEventList_T *)GetFctLeverEventList();
    }
    else
    {
        // NORMAL
        pEventList = (LeverEventList_T *)GetLeverEventList();
    }

    if( mu16DelayWater != 0 )
    {
        mu16DelayWater--;
    }
    else
    {
        mu16DelayWater = EventHandler( LEVER_ID_WATER, pEventList );
    }

    if( mu16DelayIce != 0 )
    {
        mu16DelayIce--;
    }
    else
    {
        mu16DelayIce = EventHandler( LEVER_ID_ICE, pEventList );
    }
}
