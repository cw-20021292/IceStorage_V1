#pragma interrupt INTTM02 HeaterOutInterrupt

/*************************************************************************************************
 * HEATER OUT
 *
 *  TRIAC 및 RELAY를 이용해서 히터 출력 제어
 *  유량 증대형 (NOVEL SERO MODEL)
 *  - LPM : 0.65 ~ 0.7
 *  -. 2860 kW ( 1600 / 1300 )
 *  -. 첫잔 대기 시간 : 8.5s ~ 11s
 *  -. 내부 체적 : 95ml
 *
 *  - HEATER OUTPUT에 값에 따라 HEATER 제어
 *  - HEATER OUTPUT : 0% ~ 100%
 *  - HEATER OUTPUT은 TRIAC HETAER와 RELAY HEATER 비율로 나눠진다.
 *
 *
 */
#include "hw.h"
#include "heater_out.h"
#include "hot_water_heater.h"
#include "util.h"
#include <string.h>

/* TRIAC TYPE */
#define TRIAC_LARGE         0
#define TRIAC_SMALL         1
#define CONFIG_HEATER_TYPE   TRIAC_SMALL


/* TIMER-2 
 * HEATER TRIAC OUT 
 * At 60hz 
 * 120hz = 8333us
 */
#define INTS_BYTE()     unsigned char _ints_byte
#define MASK_TIMER                       TMMK02
#define DISABLE_INT_MASK_TIMER()          do { MASK_TIMER = 1; }while(0)
#define ENABLE_INT_MASK_TIMER()           do { MASK_TIMER = 0; }while(0)

#define ENTER_CRITICAL_SECTION()  \
    do{ \
        _ints_byte = MASK_TIMER; \
        DISABLE_INT_MASK_TIMER(); \
    }while(0)

#define EXIT_CRITICAL_SECTION()   \
    do{ \
        MASK_TIMER = _ints_byte; \
    } while(0)



// HEATER RELAY & TRIAC POWER RATIO
#if (CONFIG_HEATER_TYPE == TRIAC_LARGE)
#define RELAY_HEATER_OUT     45U      // ### WATT  45%  
#define TRIAC_HEATER_OUT     55U      // ### WATT  55%
#define MAX_TRIAC_TICK       2UL      // TRAIC CONTROL RESOLUTION ( @Double... )
#else
//#if (CONFIG_HEATER_TYPE == TRIAC_SMALL)

#define RELAY_HEATER_OUT     55U      // ### WATT  55%  
#define TRIAC_HEATER_OUT     45U      // ### WATT  45%
//#define TRIAC_RESOLUTION     20U       // TRAIC CONTROL RESOLUTION ( @Double... )
#define MAX_TRIAC_TICK       20UL

#endif



// 히터 제어 영역
// SINGLE이면 triac only
// double이면 triac & relay
//#define REGION_SINGLE    0
//#define REGION_DOUBLE    1

HeaterOut_T HeaterOut;


void  InitHeaterOut(void)
{
    HeaterOut.Output        = MIN_HEATER_OUT;

    HeaterOut.Region        = REGION_SINGLE;
    HeaterOut.MaxTick       = 0;
    HeaterOut.CurrentTick   = 0;           
    HeaterOut.TargetTick    = 0;           
    HeaterOut.NextTargetTick    = 0;           

    R_TAU0_Channel2_Start();
}


void  GetHeaterOutData(HeaterOut_T *pData)
{
    MEMCPY( (void __far *)pData, (const void __far *)&HeaterOut, sizeof( HeaterOut_T ));
}

U8 GetHeaterOutRegion(void)
{
    return HeaterOut.Region;
}


U16 CalcHeaterAdc2Watt(U16 mu16Adc)
{
    U32 a = 0UL;
    U32 b = 0UL;
    U32 c = 0UL;
    U16 watt = 0;

    if( mu16Adc >= 194UL /* MIN ADC */ )
    {
        a = (U32)mu16Adc;
        b = (U32)((780UL * a) / 100UL);
        c = (U32)( b - 1341UL);
    }

    watt = (U16)c;
    return watt;
}

typedef struct _triac_watt_
{
    U32  Heater;     // WATT, 히터 전체 용량
    U32  Triac;      // WATT, 히터 전체 용량 중 트라이악 용량
    U32  Target;     // WATT, 트라이악 출력 용량
    U32  Tick;       // WATT, Tick당 용량  ( 소숫점 두자리 )
    U16  TickNum;    // 필요한 Tick
} Traic_T;

Traic_T TriacWatt;
static void SetTriacOutput(U16 mu16Output)
{
    INTS_BYTE();

    // 히터의 총 watt
    TriacWatt.Heater = GetHeaterBaseWatt();

    // 히터의 총 WATT 중 triac의 watt
    TriacWatt.Triac  = (TriacWatt.Heater * TRIAC_HEATER_OUT) / 100UL;
    TriacWatt.Target = (TriacWatt.Heater * mu16Output ) / 100UL;

    // Tick당 Watt
    TriacWatt.Tick   = (TriacWatt.Triac * 100UL) / MAX_TRIAC_TICK;
    
    // 소숫점 첫자리까지 구해서 반올림 처리
    TriacWatt.TickNum = (U16)(( ((TriacWatt.Target * 1000UL) / TriacWatt.Tick ) + 5UL ) / 10UL);



    // TRIAC 제어가 INTERRUPT TIMER에서 제어되기 때문에
    // 데이터 공유 문제를 처리하기 위핸 CRITICAL SECTION 처리 한다.
    ENTER_CRITICAL_SECTION();
    HeaterOut.MaxTick    = MAX_TRIAC_TICK;
    HeaterOut.NextTargetTick = TriacWatt.TickNum;
    if( HeaterOut.NextTargetTick > MAX_TRIAC_TICK )
    {
        HeaterOut.NextTargetTick = MAX_TRIAC_TICK;
    }
    EXIT_CRITICAL_SECTION();
}

// HETAER 출력이 0 이면 OFF
static void ClearTriacOutput(void)
{
    INTS_BYTE();

    ENTER_CRITICAL_SECTION();

    HeaterOut.MaxTick       = 0U;
    HeaterOut.CurrentTick   = 0U;
    HeaterOut.TargetTick    = 0U;

    EXIT_CRITICAL_SECTION();
}

static void ResetRegion(void)
{
    INTS_BYTE();

    ENTER_CRITICAL_SECTION();
    HeaterOut.Region = REGION_SINGLE;
    EXIT_CRITICAL_SECTION();
}


#define SINGLE_POWER    ( TRIAC_HEATER_OUT )
#define DOUBLE_POWER    ( RELAY_HEATER_OUT )
static void CheckRegion(U16 mu16Output)
{
    INTS_BYTE();
    static U16 mu16PrevOutput = 0;


    // 최초 히터가 on이 되는 경우에는 SINGLE 영역으로 동작
    if( mu16PrevOutput != mu16Output )
    {
        if( mu16PrevOutput == 0 )
        {
            if( mu16Output <= SINGLE_POWER )
            {
                ENTER_CRITICAL_SECTION();
                HeaterOut.Region = REGION_SINGLE;
                EXIT_CRITICAL_SECTION();
            }
            else
            {
                ENTER_CRITICAL_SECTION();
                HeaterOut.Region = REGION_DOUBLE;
                EXIT_CRITICAL_SECTION();
            }

            mu16PrevOutput = mu16Output;
            return ;
        }

        mu16PrevOutput = mu16Output;
    }


    if( HeaterOut.Region == REGION_SINGLE )
    {
        if( mu16Output > SINGLE_POWER )
        {
            ENTER_CRITICAL_SECTION();
            HeaterOut.Region = REGION_DOUBLE;
            EXIT_CRITICAL_SECTION();
        }
    }
    else
    {
        if( mu16Output <= SINGLE_POWER )
        {
            ENTER_CRITICAL_SECTION();
            HeaterOut.Region = REGION_SINGLE;
            EXIT_CRITICAL_SECTION();
        }
    }
}


// 히터 출력율 설정
U8 dbg_flag_heater = 0;
U16 dbg_heater_ouput = 0;
void SetHeaterOutput(U16 mu16Output)
{

    HeaterOut.Output = mu16Output; 

	if(HeaterOut.Output > 100)
    	HeaterOut.Output = 100; 
	else {}		

    ///////////////////////////////////////////////////
    // DEBUG CODE...
    if( dbg_flag_heater != 0 )
    {
        HeaterOut.Output = dbg_heater_ouput;
    }
    ///////////////////////////////////////////////////

    CheckRegion( HeaterOut.Output );

    if( HeaterOut.Output == MIN_HEATER_OUT )
    {
        // OFF 
        ClearTriacOutput();
        ResetRegion();
    }
    else if( HeaterOut.Region == REGION_DOUBLE )
    {
        // REALY ON, TRIAC ON
        if( HeaterOut.Output <= RELAY_HEATER_OUT )
        {
            SetTriacOutput( 0 );
        }
        else
        {
            U16 mu16Output = 0;

            mu16Output = HeaterOut.Output - RELAY_HEATER_OUT;
            if( mu16Output >= TRIAC_HEATER_OUT )
            {
                SetTriacOutput( TRIAC_HEATER_OUT );
            }
            else
            {
                SetTriacOutput( mu16Output );
            }
        }
    }
    else
    {
        // SINGLE 
        // TRAIC ON, RELAY OFF
        if( HeaterOut.Output >= TRIAC_HEATER_OUT )
        {
            SetTriacOutput( TRIAC_HEATER_OUT );
        }
        else
        {
            SetTriacOutput( HeaterOut.Output  );
        }
    }
}


// 히터 출력율 반환 ( 0 ~ 100% )
U16 GetHeaterOutput(void)
{
    return HeaterOut.Output;
}


// TRIAC DUTY CONTROL
static U8 GetOnOffTriac(void)
{
    U8 mu8OnOff = OFF;


    HeaterOut.CurrentTick++;
    if(HeaterOut.CurrentTick <= HeaterOut.TargetTick)
    {
        mu8OnOff = ON;
    }
    else
    {
        mu8OnOff = OFF;
    }

    if(HeaterOut.CurrentTick >= HeaterOut.MaxTick)
    {
        HeaterOut.CurrentTick = 0;
        HeaterOut.TargetTick = HeaterOut.NextTargetTick;
    }

    return mu8OnOff;
}


// TRIAC
static void TurnOnOffTriac(U8 mu8OnOff )
{
    if( mu8OnOff == ON )
    {
        if( GetOnOffTriac() == ON )
        {
            TURN_ON_HEATER_TRIAC();
        }
        else
        {
            TURN_OFF_HEATER_TRIAC();
        }
    }
    else
    {
        TURN_OFF_HEATER_TRIAC();
    }
}

U16 dbg_relay_flag = 0;
U16 dbg_relay_count = 0;

// RELAY 
static void TurnOnOffRelay(U8 mu8OnOff )
{
    /////////// DBG ///////////////////
    if( dbg_relay_flag != mu8OnOff )
    {
        if( mu8OnOff == ON )
        {
            dbg_relay_count++;
        }
    }
    /////////// DBG ///////////////////

    if( mu8OnOff == ON )
    {
        TURN_ON_HEATER_RELAY();
        dbg_relay_flag = 1; // for.. dbg
    }
    else
    {
        TURN_OFF_HEATER_RELAY();
        dbg_relay_flag = 0; //for ... dbg
    }
}

#if CONFIG_TEST_HEATER
static void ProcessHeaterOut(void)
{
    TurnOnOffTriac( OFF );
    TurnOnOffRelay( OFF );
}
#else

#define HEATER_DELAY_TIME   121U    // 1sec @ 8.3ms
 U16 mu16OnDelay  = 0;     // Relay chattering timer
 U16 mu16OffDelay = 0;     // Relay chattering timer
static void ProcessHeaterOut(void)
{
    static U8 mu8On = FALSE;
//    static U16 mu16OnDelay  = 0;     // Relay chattering timer
//    static U16 mu16OffDelay = 0;     // Relay chattering timer
    U16 mu16Output;


    mu16Output = GetHeaterOutput();

    // OFF 조건
    if( mu16Output == MIN_HEATER_OUT )
    {
        mu16OnDelay  = 0;     // Relay chattering timer
        mu16OffDelay = 0;     // Relay chattering timer

        TurnOnOffTriac( OFF );
        TurnOnOffRelay( OFF );
    }
    // 일반 출력
    else if( HeaterOut.Region == REGION_DOUBLE )
    {
        mu16OffDelay = HEATER_DELAY_TIME;

        TurnOnOffTriac( ON );

        if( mu16OnDelay != 0 )
        {
            mu16OnDelay--;
            TurnOnOffRelay( OFF );
        }
        else
        {
            TurnOnOffRelay( ON );
        }
    }
    // 낮은 출력
    else
    {
        mu16OnDelay = HEATER_DELAY_TIME;

        if( mu16OffDelay != 0 )
        {
            mu16OffDelay--;
            TurnOnOffRelay( ON );
            TurnOnOffTriac( OFF );
        }
        else
        {
            TurnOnOffRelay( OFF );
            TurnOnOffTriac( ON );
        }
    }
}
#endif



/* INTERRUPT SERVICE ROUTIN */
void HeaterOutInterrupt(void)
{
    EI();
    ProcessHeaterOut();
}

