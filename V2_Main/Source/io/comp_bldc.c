#include "comp_bldc.h"
#include "relay.h"
#include "timer.h"
#include "hot_water.h"

#include <string.h>



#define COMP_STABLE_TIME    3U   // 3sec...
#define OFF_TIME_5SEC       5U   // 5sec..
#define OFF_TIME_30SEC      30U  // 30sec..

CompBldc_T CompBldc;


void InitCompBldc(void)
{
    CompBldc.Target     = 0U;
    CompBldc.Current    = 0U;
    CompBldc.Max        = MAX_RPS;
    CompBldc.Min        = MIN_RPS;
    CompBldc.ErrorCode          = ERR_BLDC_NONE;
    CompBldc.LastedErrorCode    = ERR_BLDC_NONE;
    CompBldc.ErrorComm          = FALSE;
    CompBldc.TestComm           = FALSE;


    CompBldc.Protect        = FALSE;
    CompBldc.OnStableTime   = 0U;
    CompBldc.OffMaxTime     = 0U;
}

void GetCompBldcData( CompBldc_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&CompBldc, sizeof( CompBldc_T ));
}

void SetCompBldcCommTest(U8 mu8Test)
{
    CompBldc.TestComm = mu8Test;
}

U8 GetCompBldcCommTest(void)
{
    return CompBldc.TestComm;
}

// SET/GET TARGET RPS
void SetCompBldcTargetRps(RPS_T mRps)
{
    CompBldc.Target = mRps;
}

RPS_T GetCompBldcTargetRps(void)
{
    return CompBldc.Target;
}


// SET/GET CURRENT RPS
void SetCompBldcCurrentRps(RPS_T mRps)
{
    CompBldc.Current = mRps;
}

RPS_T GetCompBldcCurrentRps(void)
{
    return CompBldc.Current;
}

// ERROR CODE
void SetCompBldcErrorCode(BldcErrorId_T mErrorId)
{
    if( mErrorId >= ERR_BLDC_OUT_OF_RANGE )
    {
        mErrorId = ERR_BLDC_OUT_OF_RANGE;
    }
    if( mErrorId != ERR_BLDC_NONE )
    {
        if( CompBldc.LastedErrorCode != mErrorId )
        {
            CompBldc.LastedErrorCode = mErrorId;
        }
    }
    CompBldc.ErrorCode = mErrorId;
}

BldcErrorId_T GetCompBldcErrorCode(void)
{
    return CompBldc.ErrorCode;
}

void SetCompBldcLastedErrorCode(BldcErrorId_T mErrorId)
{
    CompBldc.LastedErrorCode = mErrorId;
}

BldcErrorId_T GetCompBldcLastedErrorCode(void)
{
    return CompBldc.LastedErrorCode;
}


// PROTECT
void SetCompBldcProtect(U8 mu8Protect)
{
    CompBldc.Protect = mu8Protect;
}

U8 GetCompBldcProtect(void)
{
    return CompBldc.Protect;
}


// ERROR CODE
void SetCompBldcErrorComm(U8 mu8Error)
{
    CompBldc.ErrorComm = mu8Error;
}

U8 GetCompBldcErrorComm(void)
{
    return CompBldc.ErrorComm;
}

U8 IsCompBldcProtectMode(void)
{
    if( CompBldc.ErrorCode != ERR_BLDC_NONE )
    {
        return TRUE;    // Protect
    }

    if( CompBldc.ErrorComm == TRUE )
    {
        return TRUE;    // Protect
    }

    return FALSE;
}

// IS TURN ON COMP???
U8 IsTurnOnCompBldc(void)
{
    if( CompBldc.Target > 0 )
            //&& CompBldc.ErrorCode == ERR_BLDC_NONE )
    {
        return TRUE;
    }

    return FALSE;
}


enum
{
    ON_CHECK_HOT_HEATER,
    ON_INIT,
    ON_WAIT,
    ON_CONTROL_COMP_BLDC,
    ON_RUNNING,
};

static U8 TurnOn(U8 mu8Step)
{
    switch( mu8Step )
    {
        case ON_CHECK_HOT_HEATER:
            if( IsExpiredHeaterOnTime() == TRUE )
            {
                mu8Step++;
            }
            break;

        case ON_INIT:
            TurnOnRelay( RELAY_COMP );
            CompBldc.OnStableTime = COMP_STABLE_TIME;

            mu8Step++;
            break;

        case ON_WAIT:
            if( CompBldc.OnStableTime != 0 )
            {
                CompBldc.OnStableTime--;
            }
            else
            {
                mu8Step++;
            }
            break;


        case ON_CONTROL_COMP_BLDC:
            StartTimer( TIMER_ID_COMP, 0 );
            StartTimer( TIMER_ID_COMM_COMP_RX_ERR, SEC( ERROR_COMP_COMM_TIMEOUT ) );

            mu8Step++;
            break;

        case ON_RUNNING:
            // Doing... Running..
            break;

        default:
            break;
    }

    return mu8Step;
}

// 보호동작에 의해서 압축기 OFF시에는
// 대기 시간을 5초로 단축한다.
// 압축기 자체 보호동작에 의해서 이미 압축기는 OFF상태이다.
static U8 GetCompOffMaxTime(void)
{
    if( CompBldc.Protect == TRUE )
    {
        return OFF_TIME_5SEC;
    }

    return OFF_TIME_30SEC;
}

enum
{
    OFF_INIT,
    OFF_WAIT_CURRENT_RPS,
    OFF_WAIT,
    OFF_WAIT_BEFORE_OFF_RELAY,
};

static U8 TurnOff(U8 mu8Step)
{
    switch( mu8Step )
    {
        case OFF_INIT:
            CompBldc.OffMaxTime = GetCompOffMaxTime();
            mu8Step++;
            break;

        case OFF_WAIT_CURRENT_RPS:
            if( CompBldc.OffMaxTime != 0 )
            {
                CompBldc.OffMaxTime--;
            }
            else
            {
                CompBldc.OnStableTime = COMP_STABLE_TIME;
                mu8Step++;
                break;
            }

            if( GetCompBldcCurrentRps() == 0 )
            {
                CompBldc.OnStableTime = COMP_STABLE_TIME;
                mu8Step++;
            }
            break;

        case ON_WAIT:
            if( CompBldc.OnStableTime != 0 )
            {
                CompBldc.OnStableTime--;
            }
            else
            {
                mu8Step++;
            }
            break;

        case OFF_WAIT_BEFORE_OFF_RELAY:
            TurnOffRelay( RELAY_COMP );

            SetCompBldcCurrentRps( 0 );     // feedback rps 초기화
            SetCompBldcProtect( FALSE );    // 보혿 동작 해제

            StopTimer( TIMER_ID_COMP );
            StopTimer( TIMER_ID_COMM_COMP_RX_ERR );
            break;

        default:
            break;
    }

    return mu8Step;
}

// CONTROL RPS...   @ 1sec..
void ControlCompBldc(void)
{
    static U8 mu8Step = 0U;
    static RPS_T prevRps = 0U;
    RPS_T   targetRps = 0U;


    targetRps = GetCompBldcTargetRps();
    if( targetRps != prevRps )
    {
        if( prevRps == 0 && targetRps > 0 )
        {
            // off -> on
            mu8Step = 0;
        }
        else if( prevRps > 0 && targetRps == 0 )
        {
            // on -> off
            mu8Step = 0;
        }
        prevRps = targetRps;
    }

    if( targetRps > 0 )
    {
        mu8Step = TurnOn( mu8Step );
    }
    else
    {
        mu8Step = TurnOff( mu8Step );
    }
}
