#pragma interrupt INTP11 r_intc11_interrupt

#include "hw.h"
#include "flow_meter.h"
#include "water_out.h"
#include "timer.h"
#include <string.h>


/* MIN OUTAGE VALUE */
// 5hz = 0.085 LPM
// 6hz = 0.102 LPM
#define OUTAGE_WATER_HZ    5    


// Dictionary
// FLOW : ?ú†?üâ
// TICK : Hz?ùò ?ûë??? ?ã®?úÑ ( 100ms )
// HZ Ï∏°Ï†ï ????ö¥?Ñ∞

/* INTERRUPT */
#define INTS_BYTE_FLOW_METER()                     unsigned char _ints_flow_byte
#define MASK_FLOW_METER                 PMK11
#define DISABLE_INT_MASK_INTP()         do { MASK_FLOW_METER = 1; }while(0)
#define ENABLE_INT_MASK_INTP()          do { MASK_FLOW_METER = 0; }while(0)

#define ENTER_CRITICAL_SECTION_INTP()  \
    do{ \
        _ints_flow_byte = MASK_FLOW_METER; \
        DISABLE_INT_MASK_INTP(); \
    }while(0)

#define EXIT_CRITICAL_SECTION_INTP()   \
    do{ \
        MASK_FLOW_METER = _ints_flow_byte; \
    } while(0)


static void StartFlowMeter(void);
static void StopFlowMeter(void);


FlowMeter_T  Flow;

void InitFlowMeter(void)
{
    U8 i;

    MEMSET( (void __FAR *)&Flow, 0, sizeof( FlowMeter_T ));

    for( i = 0; i < NUM_FLOW_ID; i++ )
    {
        Flow.FlowMeterFun[ i ] = NULL;
    }

    Flow.CheckOutage                = FALSE;
    Flow.Outage[ TYPE_WATER ]       = FLOW_SUPPLY;        
    Flow.Outage[ TYPE_HOT_WATER ]   = FLOW_SUPPLY;        
    Flow.ConfOutageTime             = OUTAGE_CHECK_TIME_10SEC;
    Flow.CheckOutageTime            = OUTAGE_CHECK_TIME_10SEC;

    StartFlowMeter();
}

void GetFlowMeterData( FlowMeter_T *pData )
{
    MEMCPY( (void __far *)pData, (const void __far *)&Flow, sizeof( FlowMeter_T ));
}


void StartCheckOutage(U16 mu16WaitTime, U16 mu16CheckTime, OutageStatus_T mType )
{
    Flow.CheckOutage        = TRUE;             // start check!!

    Flow.WaitTime           = mu16WaitTime;     // Stable wait time..
    Flow.ConfOutageTime     = mu16CheckTime;    // check time
    Flow.Type               = mType;

    Flow.CheckOutageTime    = mu16CheckTime;
}


void StopCheckOutage(void)
{
    Flow.CheckOutage = FALSE;
}


// Outage
OutageStatus_T GetOutageStatus(OutageType_T mType)
{
    return Flow.Outage[mType];
}

void SetOutageStatus(OutageType_T mType, OutageStatus_T mu8Status )
{
    Flow.Outage[mType] = mu8Status;
}


// Type
OutageType_T GetOutageType(void)
{
    return Flow.Type;
}

void SetOutageType(OutageType_T mu8Type )
{
    Flow.Type = mu8Type;
}



// Return Hz
U16 GetFlowMeterHz(void)
{
    return Flow.Hz;
}

// Return LPM
F32 GetFlowMeterLPM(void)
{
    return Flow.LPM;
}

// Return CC
U16 GetFlowMeterCc(void)
{
    U16 mu16cc;

    mu16cc  = (U16)(Flow.LPM  * 1000.0f);
    return mu16cc;
}

F32 GetFlowMeterLastedLPM(void)
{
    return Flow.LastedLPM;
}

void RegisterFlowMeterId(FlowId_T id,  void (*pCall)(void) )
{
    INTS_BYTE_BASE_TIMER();
    INTS_BYTE_FLOW_METER();

    ENTER_CRITICAL_SECTION_TIMER();
    {
        ENTER_CRITICAL_SECTION_INTP();
        Flow.FlowMeterFun[ id ] = pCall;
        EXIT_CRITICAL_SECTION_INTP();
    }
    EXIT_CRITICAL_SECTION_TIMER();
}

// Hz to LPM
F32 ConvHz2LPM( U16 mu16Hz )
{
    F32 mf32LPM = 0.0f;

    /* LPM = (Hz * 60sec ) / (HZ_AMOUNT_10ML * 100 ) */
    mf32LPM = ((U32)mu16Hz * 60.0f) / (AMOUNT_10 * 100.0f );

    return mf32LPM;
}

// LPM to Hz
U16 ConvLPM2Hz( F32 mf32LPM )
{
    U16 mu16Hz;

    /* ((LPM * 100ml) / 60sec ) * AMOUNT_10 */
    mu16Hz = (U16)(((mf32LPM * 100.0f ) / 60.0f) * (F32)AMOUNT_10);

    return mu16Hz;
}

// Tick : 100ms..
// ?ú†?üâ Í≥ÑÏÇ∞
#if CONFIG_TEST_FLOW_METER
U16 dbg_flow_hz = 0;
#endif
static void CalcFlowMeterHz(void)
{
    U8 i;
    U16 mu16Sum;
    U16 mu16TickHz;
    INTS_BYTE_FLOW_METER();
    INTS_BYTE_BASE_TIMER();


    ENTER_CRITICAL_SECTION_TIMER();
    {
        ENTER_CRITICAL_SECTION_INTP();
        mu16TickHz = Flow.TickHz;
        Flow.TickHz = 0;
        EXIT_CRITICAL_SECTION_INTP();
    }
    EXIT_CRITICAL_SECTION_TIMER();

    Flow.TickHzList[ Flow.IndexList ] = mu16TickHz;
    Flow.IndexList++;
    if( Flow.IndexList >= TICK_HZ_NUM )
    {
        Flow.IndexList = 0;
    }

    mu16Sum = 0; 
    for( i = 0; i < TICK_HZ_NUM ; i++ )
    {
        mu16Sum += Flow.TickHzList[ i ];
    }

    // Update Hz, Flow Rate(LPM)
    Flow.Hz = mu16Sum;
#if CONFIG_TEST_FLOW_METER
    Flow.Hz += dbg_flow_hz;
#endif
    Flow.LPM = ConvHz2LPM( Flow.Hz );

    if( Flow.LPM != 0 )
    {
        Flow.LastedLPM = Flow.LPM;
    }
}


static void StartFlowMeter(void)
{
    // Start Flow Meter Pin interrupt service routin
    R_INTC11_Start();
}

static void StopFlowMeter(void)
{
    // Stop Flow Meter Pin interrupt service routin
    R_INTC11_Stop();
}


// Prevent interference noise...
// Call by 1ms routin.....
void FilterFlowMeterNoise(void)
{
    if( Flow.Sync == TRUE )
    {
        Flow.Sync = FALSE;

        StartFlowMeter();
    }
}

// PIN INTERRUPT ISR
// DETECTING FLOW SENSOR INPUT...
static void CheckFlowMeterIsr(void)
{
    U8 i;

    Flow.Sync = TRUE;

    Flow.TickHz++;
    
    for( i = 0; i < NUM_FLOW_ID; i++ )
    {
        if( Flow.FlowMeterFun[i] != NULL )
        {
            Flow.FlowMeterFun[i]();
        }
    }

    StopFlowMeter();
}

__interrupt static void r_intc11_interrupt(void)
{
    CheckFlowMeterIsr();
}


// ?ã®?àò Í≤??Ç¨ Î°úÏßÅ
static void CheckOutageStatus(void)
{
    if( Flow.CheckOutage == FALSE )
    {
        // Reset check timer..
        Flow.CheckOutageTime = Flow.ConfOutageTime;
        return ;
    }


    // Wait stable time..
    if( Flow.WaitTime != 0 )
    {
        Flow.WaitTime--;

        return ;
    }

    // Check..
    if( Flow.Hz <= OUTAGE_WATER_HZ )
    {
        if( Flow.CheckOutageTime != 0 )
        {
            Flow.CheckOutageTime--;
        }
        else
        {
            // timeout outage!!!
            Flow.Outage[ Flow.Type ] = FLOW_OUTAGE;
        }
    }
    else
    {
        // Reset check timer
        Flow.CheckOutageTime = Flow.ConfOutageTime;
    }
}


// @100ms..
void ProcessFlowMeter(void)
{
    CalcFlowMeterHz();

    CheckOutageStatus();
}


#if CONFIG_TEST_FLOW_METER
#include "drain_water.h"
#include "valve.h"
U8 dbg_the_flow_test = 0;
#endif
void TestAutoCountingFlowMeter(void)
{
#if CONFIG_TEST_FLOW_METER
    if( dbg_the_flow_test != 0 )
    {
        return ;
    }

    if( (GetWaterOut() == TRUE &&
            (
             IsOpenValve( VALVE_HOT_IN ) == TRUE 
             || IsOpenValve( VALVE_ROOM_COLD_OUT ) == TRUE 
             || IsOpenValve( VALVE_COLD_IN ) == TRUE 
             || IsOpenValve( VALVE_COLD_FLUSH ) == TRUE 
            )
            ||
            (
            IsOpenValve( VALVE_ICE_TRAY_IN ) == TRUE 
            || IsStartDrainWater() == TRUE
            )
        )
      )
    {
        CheckFlowMeterIsr();
        return ;
    }

#endif
}
