#ifndef __FLOW_METER_H__
#define __FLOW_METER_H__

#include "prj_type.h"


// Default Flow sensor Amount @ COUNTER TICK
#define AMOUNT_5        17UL           
#define AMOUNT_10       35UL           
#define AMOUNT_20       70UL           
#define AMOUNT_15       52UL           
#define AMOUNT_30       105UL           
#define AMOUNT_50       175UL
#define AMOUNT_60       210UL
#define AMOUNT_120      420UL
#define AMOUNT_130      455UL
#define AMOUNT_140      490UL
#define AMOUNT_150      525UL
#define AMOUNT_160      560UL
#define AMOUNT_170      595UL
#define AMOUNT_180      630UL
#define AMOUNT_190      665UL
#define AMOUNT_200      700UL
#define AMOUNT_210      735UL
#define AMOUNT_220      770UL
#define AMOUNT_230      805UL
#define AMOUNT_235      822UL
#define AMOUNT_240      840UL
#define AMOUNT_250      875UL
#define AMOUNT_260      910UL
#define AMOUNT_280      980UL
#define AMOUNT_300      1050UL
#define AMOUNT_320      1120UL
#define AMOUNT_340      1190UL
#define AMOUNT_360      1260UL
#define AMOUNT_380      1330UL
#define AMOUNT_400      1400UL
#define AMOUNT_420      1470UL
#define AMOUNT_440      1540UL
#define AMOUNT_460      1610UL
#define AMOUNT_480      1680UL
#define AMOUNT_500      1750UL
#define AMOUNT_750      2625UL
#define AMOUNT_1000     3500UL
#define AMOUNT_1500     5250UL
#define AMOUNT_2000     7000UL
#define AMOUNT_3000    10500UL
#define AMOUNT_4000    14000UL     
#define AMOUNT_5000    17500UL     
#define AMOUNT_6000    21000UL     
#define AMOUNT_7000    24500UL     
#define AMOUNT_8000    28000UL     
#define AMOUNT_9000    31500UL     
#define AMOUNT_10000   35000UL     // 10L
#define AMOUNT_11000   38500UL     // 11L
#define AMOUNT_12000   42000UL     // 12L
#define AMOUNT_20000   70000UL     // 20L, 20min...@1LPM
#define AMOUNT_30000  105000UL     // 30L, 30min
#define AMOUNT_60000  210000UL     // 60L, 60min...@1LPM

// Outage Status
typedef enum
{
    FLOW_SUPPLY, 
    FLOW_OUTAGE
} OutageStatus_T;
#define OUTAGE_HOT_OUT_DELAY_TIME     1U // 10U     // 1sec @100ms
#define OUTAGE_DELAY_TIME             1U // 100U    // 10sec @100ms..
#define OUTAGE_FLUSHING_DELAY_TIME    1U // 300U    // 30sec @100ms..

// Outage Type
typedef enum
{
    TYPE_WATER, 
    TYPE_HOT_WATER,
    TYPE_ICE_TRAY,
    TYPE_NUM
} OutageType_T;

// Flow meter id 
typedef enum
{
    FLOW_ID_WATER,
    FLOW_ID_ICE_TRAY,
    FLOW_ID_DRAIN,
    NUM_FLOW_ID
}FlowId_T;


#define TICK_HZ_NUM        10
typedef struct _flow_meter_
{
   // Hz 
    U8 Sync;
    U8 IndexList;
    U16 TickHzList[ TICK_HZ_NUM ];
    U16 TickHz;  // 100ms...

    U16 Hz;         
    F32 LPM;   
    F32 LastedLPM;   

    // Outage
    U8 CheckOutage;             // TRUE or FALSE ( START or STOP )
    OutageStatus_T Outage[ TYPE_NUM ]; // SUPPLY or OUTAGE
    OutageType_T Type;      
    U16 WaitTime;               // Stable Timer..

    U16 ConfOutageTime;         // ?®Ïàò Í≤Ä???§Ï†ï ?úÍ∞Ñ ( Î¨?Ï¢ÖÎ•ò???∞Îùº ?§Î¶Ñ )
    U16 CheckOutageTime;        // Check Timer..

    // ISR - FLOW METER PIN INTERRUPT 
    void (*FlowMeterFun[NUM_FLOW_ID])(void);         // WATER OUT
} FlowMeter_T;


void InitFlowMeter(void);

void GetFlowMeterData( FlowMeter_T *pData );


// Outage
// Outage Í≤Ä???úÍ∞Ñ ?§Ï†ï @ 100ms...
#define OUTAGE_CHECK_TIME_1SEC      10UL        // 1sec
#define OUTAGE_CHECK_TIME_2SEC      20UL        // 2sec
#define OUTAGE_CHECK_TIME_3SEC      30UL        // 3sec
#define OUTAGE_CHECK_TIME_10SEC     100UL       // 10sec
#define OUTAGE_CHECK_TIME_60SEC     600UL       // 60sec
#define OUTAGE_CHECK_TIME_FLUSHING  600UL       // 60sec
void StartCheckOutage(U16 mu16WaitTime, U16 mu16CheckTime, OutageStatus_T mType );
#if 0
void StartCheckOutage(U16 mu16WaitTime, OutageType_T mType );

void SetConfigOutageTime(U16 mu16CheckTime);
#endif

void StopCheckOutage(void);


OutageStatus_T GetOutageStatus(OutageType_T mType);

void SetOutageStatus(OutageType_T mType, OutageStatus_T mu8Status );


OutageType_T GetOutageType(void);

void SetOutageType(OutageType_T mu8Type );


// Return Hz
U16 GetFlowMeterHz(void);

// Return LPM
F32 GetFlowMeterLPM(void);

U16 GetFlowMeterCc(void);

// Return LPM
F32 GetFlowMeterLastedLPM(void);

// Register 
void RegisterFlowMeterId(FlowId_T id,  void (*pCall)(void) );

F32 ConvHz2LPM( U16 mu16Hz );

U16 ConvLPM2Hz( F32 mf32LPM );

void FilterFlowMeterNoise(void);

void TestAutoCountingFlowMeter(void);

void ProcessFlowMeter(void);

#endif /* __FLOW_METER_H__ */
