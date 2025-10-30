#include "process_make.h"
#include "comp.h"
#include "fan_motor.h"

#include "cold_water.h"
#include "ice.h"
#include "ice_maker.h"
#include "gas_switch_valve.h"
#include "temp.h"
#include "hot_water.h"

void InitProcessMake(void)
{
    InitComp();
    InitFanMotor();
}


/* 냉각 순환 펌프 제어 */
#if 0
static void ControlMakeColdCirculatePump(void)
{
    static U16 mu16PumpOnTime = 0; // @1sec
    static U8 mu8PumpOn = FALSE;
    static U8 mu8PrevCmd = OFF;
    U8 mu8Cmd = OFF;


    // 냉수 온도 10'C 이상이면 펌프 중지, 그렇지 않으면 운전
    if( GetTemp( TEMP_ID_COLD_WATER ) >= 10.0f )
    {
        mu8Cmd = OFF;
    }
    else
    {
        mu8Cmd = ON;
    }

    // 외기 온도 30'C 이상 이면 펌프 중지
    if( GetTemp( TEMP_ID_AMBIENT ) >= 30.0f )
    {
        mu8Cmd = OFF;
    }

    if( mu8PrevCmd != mu8Cmd )
    {
        if( mu8Cmd == ON && mu8PumpOn == FALSE )
        {
            mu16PumpOnTime = 30;
        }
        mu8PrevCmd = mu8Cmd;
    }

    if( mu16PumpOnTime != 0 )
    {
        mu16PumpOnTime--;
    }

    if( mu16PumpOnTime != 0 || mu8Cmd == ON )
    {
        mu8PumpOn = TRUE;
        TurnOnCirculatePumpMakeCold();
    }
    else
    {
        mu8PumpOn = FALSE;
        TurnOffCirculatePumpMakeCold();
    }
}
#endif

// 냉각 제어 ( 초기화, 냉각, 완료 )
static void ProcessMakeColdWater(void)
{
    /* 냉수 초기화 
     *  -. 냉수 순환 펌프 중지
     *  -. 냉매 밸브 전환...
     *
     */
    if( IsSetColdWaterMakeMode( COLD_MODE_INIT ) == TRUE )
    {
        // 냉각 초기화
        // 냉매 전환 밸브 냉각으로 위치 이동
        if( IsExpiredCompProtectTime() == TRUE 
                && IsExpiredHeaterOnTime() == TRUE )
        {
            GasSwitchCold();
            ClearColdWaterMakeMode( COLD_MODE_INIT );
        }
    }
    else if( IsSetColdWaterMakeMode( COLD_MODE_DONE ) == TRUE )
    {
        TurnOffComp(COMP_TYPE_COLD);
        TurnOffFan(FAN_TYPE_COLD);

        ClearColdWaterMakeMode( COLD_MODE_COOLING );
        ClearColdWaterMakeMode( COLD_MODE_DONE );
    }
    else if( IsSetColdWaterMakeMode( COLD_MODE_COOLING ) == TRUE )
    {
        RPS_T mRps;
        
        mRps = GetColdWaterMakeCompRps();
        TurnOnCompRps(COMP_TYPE_COLD, mRps );
        TurnOnFan(FAN_TYPE_COLD);
    }
    else 
    {
        // COLD_MODE_NONE
        TurnOffComp(COMP_TYPE_COLD);
        TurnOffFan(FAN_TYPE_COLD);
    }
}

static void ProcessMakeIce(void)
{
    /* Process Ice Maker */
    ProcessIceMaker();
}


/* 냉각 운전 상세 모드 결정 */
static void MakeColdWaterMode(void)
{
    static U8 mu8PrevMake = FALSE;
    U8 mu8Make;
    U8 mu8Mode;


    mu8Make = GetColdWaterMake();         // 냉각 운전 상태
    mu8Mode = GetColdWaterMakeMode();     // 냉각 운전 상세 모드

    // 냉각 운전 상태가 변할 때에 운전할지 정지할지 결정 한다.
    // 만약, 운전 조건인데 운전 모드가 없으면 강제 운전 시킨다 
    if( mu8PrevMake != mu8Make )
    {
        // 정지 -> 운전 
        if( mu8Make == TRUE )
        {
            if( mu8Mode == COLD_MODE_NONE )
            {
                SetColdWaterMakeMode( COLD_MODE_INIT | COLD_MODE_COOLING );
            }
        }
        // 운전 -> 정지
        else 
        {
            if( mu8Mode != COLD_MODE_NONE )
            {
                SetColdWaterMakeMode( COLD_MODE_DONE );
            }
        }

        mu8PrevMake = mu8Make;
    }
    else if( mu8Make == TRUE )
    {
        // 냉각 모드에서 냉각중이 아니라면, 냉각을 시작한다.
        if( mu8Mode == COLD_MODE_NONE )
        {
            SetColdWaterMakeMode( COLD_MODE_INIT | COLD_MODE_COOLING );
        }
    }
}

#define PREHEAT_DETECT_TIME   1800UL    // 30min ( 30min * 60sec = 1800sec )
#define PREHEAT_RELEASE_TIME  180UL    // 3min ( 3min * 60sec = 180sec )

U8 DebugPreheat = OFF;
static U8 GetPreheatStatus(void)
{
    static U8 mu8Preheat = OFF;
    if( GetCompOnOff() == TRUE )
    {
        if( GetCompOnTime() >= PREHEAT_RELEASE_TIME )
        {
            mu8Preheat = OFF;
        }
    }
    else
    {
        if( GetCompOffTime() >= PREHEAT_DETECT_TIME )
        {
            mu8Preheat = ON;
        }
    }
    DebugPreheat = mu8Preheat;
    return mu8Preheat;
}
/* 제빙 상세 운전 모드 결정 */
static void MakeIceMakeMode(void)
{
    static U8 mu8PrevMake = FALSE;
    U8 mu8Make;
    U16 mu16Mode;
    U8 mu8Preheat;


    // 제빙 생성
    mu8Make = GetIceMake();        // 제빙 ON or OFF ?
    mu16Mode = GetIceMakeMode();          // 제빙 상세 모드
    mu8Preheat = GetPreheatStatus();      // Comp Preheat ON or OFF?
    if( mu8PrevMake != mu8Make )
    {
        // 정지 -> 운전 
        if(  mu8Make == TRUE )
        {
            if( mu16Mode == ICE_MODE_NONE )
            {
				SetWifiIceMakeMode(STEP_ICE_MAKE_STANDBY);            	
                if( mu8Preheat == ON )
                {
                    SetIceMakeMode( ICE_MODE_PREHEAT );
                }
                SetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING);
                ClearIceMakeMode( ICE_MODE_DEICING_OFF );
            }
        }
        else
        {
            // 운전 -> 정지
            // 제빙을 시작하지 않은 경우, 기본 위치로 이동시킨다.
            if( IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE 
                    || IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE 
                    || IsSetIceMakeMode( ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE 
                    ||  IsSetIceMakeMode( ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
            {
                ClearIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
                SetIceMakeMode( ICE_MODE_INIT );
            }

            // 연속 운전을 중지 시키기 위해서 압축기 OFF
            SetIceMakeMode( ICE_MODE_DEICING_OFF );
        }

        mu8PrevMake = mu8Make;
    }
    else if( mu8Make == TRUE )
    {
        /* 제빙 중 */
        if( mu16Mode == ICE_MODE_NONE )
        {
            if( mu8Preheat == ON )
            {
                SetIceMakeMode( ICE_MODE_PREHEAT );
            }
            SetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING);
            ClearIceMakeMode( ICE_MODE_DEICING_OFF );
        }
    }

    // 제빙 중지 상태에서 더미 탈빙 모드이면, 부하 종료...
    if( mu8Make == FALSE )
    {
        if( mu16Mode == ICE_MODE_DUMMY_DEICING 
            || mu16Mode == ICE_MODE_TRAY_DEICING )
        {
            SetIceMakeMode( ICE_MODE_DEICING_OFF );
        }
    }

}

// 외기 온도 20'C 이하 조건에서 탈빙 중 냉각을 
// 중지하기 위한 조건 검사 함수
// Return 
// TRUE : 20'C 미만  조건
// FALSE : 20'C 이상 조건
#define  DEFAULT_AMBI_TIMER   10  // @1sec
static U8 IsUnderTempAmbi(TEMP_T tAmbi )
{
    static U16 mu16AmbiTimer = DEFAULT_AMBI_TIMER;


    if( (GetTemp(TEMP_ID_AMBIENT) < tAmbi ) )
    {
        if( mu16AmbiTimer != 0 )
        {
            mu16AmbiTimer--;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        mu16AmbiTimer = DEFAULT_AMBI_TIMER; // 5sec..
    }

    return FALSE;
}


/*
 *  우선 모드에 따라,
 *  제빙/냉각 운전 결정.
 */
#if CONFIG_ENABLE_DEICING_COOLING
static void ManagerIceColdMode(void)
{
    U8 mu8ColdMode;
    U16 mu16IceMode;


    mu8ColdMode = GetColdWaterMakeMode();
    mu16IceMode  = GetIceMakeMode();


    /* 강제 탈빙을 최우선으로 처리한다. */
    if( IsSetIceMakeMode( ICE_MODE_DUMMY_DEICING ) == TRUE 
            || IsSetIceMakeMode( ICE_MODE_TRAY_DEICING ) == TRUE )
    {
        ClearColdWaterMakeMode( COLD_MODE_ALL );
        return ;
    }

    /* 냉각과 제빙이 동시 실행 조건인 경우에 대해 검사
     *
     * 제빙 우선 - 냉각 모드를 해제한다.
     *  단, 탈빙 중에는 냉각으로 전환된다.
     *
     * 냉각 우선 - 제빙 모드를 해제한다.
     *  단, 제빙 중에 냉각 모드가 설정된 경우에는 진행 중인 제빙은 완료하고 냉각으로 전환한다.
     *
     */
    if( mu8ColdMode != COLD_MODE_NONE 
            && mu16IceMode != ICE_MODE_NONE )
    {
        if( GetIceTurbo() == TRUE )
        {
            /* 제빙 우선, 제빙 중인 경우를 제외하고는 냉각 조건에서는 냉각 운전을 시킨다. */
            if( mu16IceMode != ICE_MODE_DEICING 
                    && mu16IceMode != ( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) 
                    && mu16IceMode != ( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) 
                    && mu16IceMode != ( ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) 
                    && mu16IceMode != ( ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) )
            {
                ClearColdWaterMakeMode( COLD_MODE_ALL );
            }

            // 외기 온도가 20'C 이상 조건에서만
            // 얼음 탈빙 중 냉각 동작을 할 수 있다.
            // 20'C 이하 조건이 10초 이상 유지 되어야 동작한다.
            if( IsUnderTempAmbi( 20.0f ) == TRUE )
            {
                ClearColdWaterMakeMode( COLD_MODE_ALL );
            }
        }
        else
        {
            if( IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE
                    || IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
            {
                /* 냉각 우선, 제빙 동작을 해제!!! */ 
                ClearIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
            }
            else if( IsSetIceMakeMode( ICE_MODE_PREHEAT ) == TRUE
                    || IsSetIceMakeMode( ICE_MODE_READY ) == TRUE 
                    || IsSetIceMakeMode( ICE_MODE_ICING ) == TRUE )
            {
                /* 단, 하던 제빙은 마무리하고~! */
                ClearColdWaterMakeMode( COLD_MODE_ALL );
            }
            else if( IsSetIceMakeMode( ICE_MODE_DEICING ) == TRUE )
            {
                /* 탈빙 단계에서는 외기 온도 20'c 이상 에서만 탈빙 중 냉각 허용 */
                if( IsUnderTempAmbi( 20.0f ) == TRUE )
                {
                    ClearColdWaterMakeMode( COLD_MODE_ALL );
                }
            }
			else {}
			
        }
    }
}
#else


static void CancelIceMakeMode(void)
{
	if( IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE
            || IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
    {
        /* 냉각 우선, 제빙 동작을 해제!!! */ 
        ClearIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
		SetIceMakeMode(ICE_MODE_DEICING_OFF);
		UpdateIsCoolingContinueTime();
	}
    else if( IsSetIceMakeMode( ICE_MODE_PREHEAT ) == TRUE 
            || IsSetIceMakeMode( ICE_MODE_READY ) == TRUE 
            || IsSetIceMakeMode( ICE_MODE_ICING ) == TRUE )
    {
        /* 단, 하던 제빙은 마무리하고~! */
        ClearColdWaterMakeMode( COLD_MODE_ALL );
    }
    else if( IsSetIceMakeMode( ICE_MODE_DEICING ) == TRUE 
            || IsSetIceMakeMode( ICE_MODE_DEICING_OFF ) == TRUE )
    {
        /* 단, 하던 탈빙은 마무리하고~! */
        ClearColdWaterMakeMode( COLD_MODE_ALL );
    }
}

static void ManagerIceColdMode(void)
{
    U8 mu8ColdMode;
    U16 mu16IceMode;


    mu8ColdMode = GetColdWaterMakeMode();
    mu16IceMode  = GetIceMakeMode();


    /* 강제 탈빙을 최우선으로 처리한다. */
    if( IsSetIceMakeMode( ICE_MODE_DUMMY_DEICING ) == TRUE 
            || IsSetIceMakeMode( ICE_MODE_TRAY_DEICING ) == TRUE )
    {
        ClearColdWaterMakeMode( COLD_MODE_ALL );
        return ;
    }

    /* 냉각과 제빙이 동시 실행 조건인 경우에 대해 검사
     *
     * 제빙 우선 - 냉각 모드를 해제한다.
     *  단, 탈빙 중에는 냉각으로 전환된다.
     *
     * 냉각 우선 - 제빙 모드를 해제한다.
     *  단, 제빙 중에 냉각 모드가 설정된 경우에는 진행 중인 제빙은 완료하고 냉각으로 전환한다.
     *
     */
    if( mu8ColdMode != COLD_MODE_NONE 
            && mu16IceMode != ICE_MODE_NONE )
    {
        if( GetIceTurbo() == TRUE )
        {
        	if( IsMaxMakingIceCount() == TRUE )
        	{
				// 제빙 취소
				// cold_water.c 에서 재기동 온도 및 목표 온도를 조정함
				// 제빙횟수가 3회이상이면 cold make 조건이 15도가 되어야 함
				// 목표 온도 조건은 7.5도가 되어야 함
				CancelIceMakeMode();
        	}
			else
			{
         		ClearColdWaterMakeMode( COLD_MODE_ALL );
			}
        }
        else
        {
            if( IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE
            	|| IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
		    {
		        /* 냉각 우선, 제빙 동작을 해제!!! */ 
		        ClearIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
				SetIceMakeMode(ICE_MODE_DEICING_OFF);

			}
		    else if( IsSetIceMakeMode( ICE_MODE_PREHEAT ) == TRUE 
		            || IsSetIceMakeMode( ICE_MODE_READY ) == TRUE 
		            || IsSetIceMakeMode( ICE_MODE_ICING ) == TRUE )
		    {
		        /* 단, 하던 제빙은 마무리하고~! */
		        ClearColdWaterMakeMode( COLD_MODE_ALL );
		    }
		    else if( IsSetIceMakeMode( ICE_MODE_DEICING ) == TRUE 
		            || IsSetIceMakeMode( ICE_MODE_DEICING_OFF ) == TRUE )
		    {
		        /* 단, 하던 탈빙은 마무리하고~! */
		        ClearColdWaterMakeMode( COLD_MODE_ALL );
		    }
        }
    }
}
#endif // if CONFIG_ENABLE_DEICING_COOLING

#if 0
U8 GetIceTurboStatus(void)
{
	return IceTurbo.Status;
}

void SetIceTurboStatus(U8 mu8status)
{
	IceTurbo.Status = mu8status;

	if(IceTurbo.Status == ICE_TURBO_ONGOING)
	{
		IceTurbo.MakingIceCount = 0;
		IceTurbo.IsCoolingContinueTime = 0;
	}
	else {}
}
#endif

void ProcessMake(void)
{
#if !CONFIG_JIG_GAS_SWITCH
    /* Make Mode COLD & ICE */
    MakeIceMakeMode();        // 제빙 Make Mode 결정
    MakeColdWaterMode();      // 냉각 Make Mode 결정
    ManagerIceColdMode();     // 냉각, 제빙 우선 순위 결정

    /* Process Cold Water */
    ProcessMakeColdWater();   // 냉각 MakeMode Handler

    /* Process Make Ice */
    ProcessMakeIce();         // 제빙 MakeMode Handler
#endif

    /* Process Compressor */
    ProcessComp();            // 압축기 제어
    ControlFanMotor();        // DC FAN MOTOR

}
