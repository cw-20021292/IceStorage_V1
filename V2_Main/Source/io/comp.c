#include "comp.h"

#if (CONFIG_BLDC_COMP_TYPE == COMP_TYPE_RELAY)
#include "relay.h"
#else
#include "comp_bldc.h"
#endif

#include <string.h>


// 압축기 연속 운전 시간 제한 
#define CONFIG_PROTECT_ON_TIME      1       // 1: 연속 운전 시간 제한 사용, 0: 미사용


/* PROTECT TIME */
#define  PROTECT_OFF_TIME  300UL            // 5min( 5min x 60sec  = 300sec )
#define  PROTECT_ON_TIME   9000UL           // 150min ( 150min x 60sec =  9000sec )
#define  PROTECT_SECOND_ON_TIME  11400UL    // 190min ( 190min x 60sec = 114000sec )

/* ON/OFF TIME LIMITED */
#define  MAX_TIME          60000UL  // sec


Compressor_T  Comp;

U16 gUwifiCompTime = 0U;              // 와이파이 comp on time

static void CountingWifiCompTime(void);

void InitComp(void)
{
    Comp.Mode        = COMP_MODE_NONE;
    Comp.Cold_OnOff  = OFF;
    Comp.Ice_OnOff   = OFF;
    Comp.ForcedOnOff = OFF;

    Comp.ProtectOffTime = PROTECT_OFF_TIME;
    Comp.OnTime         = 0;
    Comp.OffTime        = 5;

    InitCompBldc();

	gUwifiCompTime = 0U;
}

void GetCompData( Compressor_T *pData )
{
    MEMCPY((void __FAR *) pData, (const void __FAR *)&Comp, sizeof( Compressor_T ));
}

void SetCompMode(U8 mu8Mode)
{
    Comp.Mode = mu8Mode;
}

U8 GetCompMode()
{
    return Comp.Mode;
}

void SetCompProtectOffTime(U16 mu16Val)
{
    Comp.ProtectOffTime = mu16Val;
}

U16 GetCompProtectOffTime(void)
{
    return Comp.ProtectOffTime;
}

U8 IsOverCompProtectOnTime(void)
{
#if CONFIG_PROTECT_ON_TIME
    if( Comp.OnTime > PROTECT_ON_TIME )
    {
        return TRUE; // 보호 시간 초과
    }
#endif

    return FALSE; // 보호 시간 이내
}

U8 IsOverCompProtectSecondOnTime(void)
{
#if CONFIG_PROTECT_ON_TIME
    if( Comp.OnTime > PROTECT_SECOND_ON_TIME )
    {
        return TRUE; // 보호 시간 초과
    }
#endif

    return FALSE; // 보호 시간 이내
}

// Turn On, with RPS
void TurnOnCompRps(U8 mu8Type, RPS_T mRps )
{
    TurnOnComp( mu8Type );
#if (CONFIG_BLDC_COMP_TYPE == COMP_TYPE_BLDC)
    SetCompBldcTargetRps( mRps );
#endif
}

void TurnOnComp(U8 mu8Type)
{
    if( mu8Type == COMP_TYPE_COLD )
    {
        Comp.Cold_OnOff = ON;
    }
    else
    {
        Comp.Ice_OnOff = ON;
    }
}

void TurnOffComp(U8 mu8Type)
{
    if( mu8Type == COMP_TYPE_COLD )
    {
        Comp.Cold_OnOff = OFF;
    }
    else
    {
        Comp.Ice_OnOff = OFF;
    }
}

U8 GetCompOnOffType(U8 mu8Type)
{
    if( mu8Type == COMP_TYPE_COLD )
    {
        return Comp.Cold_OnOff;
    }

    return Comp.Ice_OnOff;
}

void TurnOnForcedComp(void)
{
    Comp.ForcedOnOff = ON;
}

void TurnOffForcedComp(void)
{
    Comp.ForcedOnOff = OFF;
}

U32 GetCompOnTime(void)
{
    return Comp.OnTime;
}

U32 GetCompOffTime(void)
{
    return Comp.OffTime;
}
static U8 IsTurnOnComp(void)
{
#if (CONFIG_BLDC_COMP_TYPE == COMP_TYPE_BLDC)
    if( IsTurnOnCompBldc() == TRUE )
    {
        return TRUE;
    }
#else
    if( IsTurnOnRelay( RELAY_COMP ) == TRUE )
    {
        return TRUE;
    }
#endif

    return FALSE;
}

static void TurnOnCompType(void)
{
#if (CONFIG_BLDC_COMP_TYPE == COMP_TYPE_BLDC)
    if( GetCompBldcTargetRps() < MIN_RPS )
    {
        SetCompBldcTargetRps( MIN_RPS );
    }
#else
    TurnOnRelay( RELAY_COMP );
#endif
}

static void TurnOffCompType(void)
{
#if (CONFIG_BLDC_COMP_TYPE == COMP_TYPE_BLDC)
    SetCompBldcTargetRps( 0 );
#else
    TurnOffRelay( RELAY_COMP );
#endif
}


U8 GetCompOnOff(void)
{
    if( IsTurnOnComp() == TRUE )
    {
        return ON;
    }

    return OFF;
}


U8 IsExpiredCompProtectTime(void)
{
    if( IsTurnOnComp() == FALSE 
            && Comp.ProtectOffTime != 0 )
    {
        return FALSE;   // COMP OFF 상태
    }

    return TRUE;    // COMP ON 상태
}

static void UpdateCompTimer(void)
{
    // 압축기가 OFF이면, Update off timer value
    // 그렇지 않으면 초기화
    if( IsTurnOnComp() == FALSE )
    {
        if( Comp.ProtectOffTime != 0 )
        {
            Comp.ProtectOffTime--;
        }
    }
    else
    {
        Comp.ProtectOffTime = PROTECT_OFF_TIME;
    }
}

static void ControlComp(void)
{
    U8 mu8OnOff = OFF;


    // 0. Get OnOff Command
    if( Comp.Cold_OnOff == ON )
    {
        mu8OnOff = ON;
    }

    if( Comp.Ice_OnOff == ON )
    {
        mu8OnOff = ON;
    }


    // 1. Protect Comp Off Time, Priority-2
    if( IsExpiredCompProtectTime() == FALSE )
    {
        mu8OnOff = OFF;
    }

    // 2. Forced OnOff, Prioirty-1
    if( Comp.Mode == COMP_MODE_FORCED )
    {
        mu8OnOff = Comp.ForcedOnOff;
    }
    
    
    // 3. Control Compressor 
    if( mu8OnOff == ON )
    {
        // Turn on Comp
        TurnOnCompType();

        // Count On Timer
        Comp.OffTime = 0;
        if( Comp.OnTime < MAX_TIME )
        {
            Comp.OnTime++;
        }

        // Reload Protect Comp Off Time
        Comp.ProtectOffTime = PROTECT_OFF_TIME;

    }
    else 
    {
        // Reload Protect Comp Off Time 
        if( IsTurnOnComp() == TRUE )
        {
            Comp.ProtectOffTime = PROTECT_OFF_TIME;
        }
        
        // Turn Off Comp
       TurnOffCompType();

        // Count Off timer
        Comp.OnTime = 0;
        if( Comp.OffTime < MAX_TIME )
        {
            Comp.OffTime++;
        }
    }
}

void ProcessComp(void)
{
    ControlComp();
#if (CONFIG_BLDC_COMP_TYPE == COMP_TYPE_BLDC)
    ControlCompBldc();
#endif
    UpdateCompTimer();


	if( IsTurnOnComp() == TRUE )
	{
      CountingWifiCompTime();
	}

}

static U8 gUsec = 10U;
static void CountingWifiCompTime(void)
{
    if( gUsec != 0U )
    {
        gUsec--;
    }
    else
    {
        gUsec = 10U;
        if( gUwifiCompTime < 65535 )
        {
            gUwifiCompTime++;
        }
    }
}

void ResetWifiCompTime(void)
{
    gUsec = 10U;
    gUwifiCompTime = 0U;
}

U16 GetWifiCompTime(void)
{
    return gUwifiCompTime;
}

