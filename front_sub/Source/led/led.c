/**
 * File : led.h
 * 
 * Led
**/
#include "hw.h"
#include "led.h"

#include "api_led.h"

#define MAX_DUTY           (100U)     // Max Duty 100%  (Tick = 10)
#define MIN_DUTY           (1U)       // Min Duty 1%    (Tick = 0)
#define MAX_DUTY_TICK      (10U)      // 10ms / 100Hz @1ms intterupt
#define MAX_SYNC_DELAY     (25U)      // 25ms / 40Hz @1ms interrupt

typedef struct _led_cnt_
{
    U16 LedCntCycle;
    U16 LedCntSyncDelay;
} SLedCnt_T;

SLedCnt_T LedCnt;

void InitLed(void)
{
    LedCnt.LedCntCycle = MAX_DUTY_TICK;
    LedCnt.LedCntSyncDelay = MAX_SYNC_DELAY;

    LedInit();
}

static U16 ConvDuty2Tick(U8 duty)
{
    U16 tick = 0U;

    if( duty > MAX_DUTY )
    {
        duty = MAX_DUTY;
    }
    else if( duty < MIN_DUTY )
    {
        duty = MIN_DUTY;
    }

    tick = (U16)((MAX_DUTY_TICK * (U16)duty) / MAX_DUTY);

    return tick;
}

typedef void (*FPLedOnOff_T)(U8 onOff);
typedef struct _led_list_
{
    ELedId_T        Led;
    U16             TargetTick;
    U16             CurrentTick;
    FPLedOnOff_T    LedOnOff;
} SLedList_T;

SLedList_T LedList[] = 
{
    /* led_id           Target_Tick  CurrentTick  Fun_LedOnOff */
    { LED_ID_SEG_1A,    0,    0,    LedOnOffSeg1A    },
    { LED_ID_SEG_1B,    0,    0,    LedOnOffSeg1B    },
    { LED_ID_SEG_1C,    0,    0,    LedOnOffSeg1C    },
    { LED_ID_SEG_1D,    0,    0,    LedOnOffSeg1D    },
    { LED_ID_SEG_1E,    0,    0,    LedOnOffSeg1E    },
    { LED_ID_SEG_1F,    0,    0,    LedOnOffSeg1F    },
    { LED_ID_SEG_1G,    0,    0,    LedOnOffSeg1G    },
    { LED_ID_SEG_2A,    0,    0,    LedOnOffSeg2A    },
    { LED_ID_SEG_2B,    0,    0,    LedOnOffSeg2B    },
    { LED_ID_SEG_2C,    0,    0,    LedOnOffSeg2C    },
    { LED_ID_SEG_2D,    0,    0,    LedOnOffSeg2D    },
    { LED_ID_SEG_2E,    0,    0,    LedOnOffSeg2E    },
    { LED_ID_SEG_2F,    0,    0,    LedOnOffSeg2F    },
    { LED_ID_SEG_2G,    0,    0,    LedOnOffSeg2G    },
    { LED_ID_SEG_3A,    0,    0,    LedOnOffSeg3A    },
    { LED_ID_SEG_3B,    0,    0,    LedOnOffSeg3B    },
    { LED_ID_SEG_3C,    0,    0,    LedOnOffSeg3C    },
    { LED_ID_SEG_3D,    0,    0,    LedOnOffSeg3D    },
    { LED_ID_SEG_3E,    0,    0,    LedOnOffSeg3E    },
    { LED_ID_SEG_3F,    0,    0,    LedOnOffSeg3F    },
    { LED_ID_SEG_3G,    0,    0,    LedOnOffSeg3G    },
    { LED_ID_SEG_4A,    0,    0,    LedOnOffSeg4A    },
    { LED_ID_SEG_4B,    0,    0,    LedOnOffSeg4B    },
    { LED_ID_SEG_4C,    0,    0,    LedOnOffSeg4C    },
    { LED_ID_SEG_4D,    0,    0,    LedOnOffSeg4D    },
    { LED_ID_SEG_4E,    0,    0,    LedOnOffSeg4E    },
    { LED_ID_SEG_4F,    0,    0,    LedOnOffSeg4F    },
    { LED_ID_SEG_4G,    0,    0,    LedOnOffSeg4G    },
    { LED_ID_SEG_5A,    0,    0,    LedOnOffSeg5A    },
    { LED_ID_SEG_5B,    0,    0,    LedOnOffSeg5B    },
    { LED_ID_SEG_5C,    0,    0,    LedOnOffSeg5C    },
    { LED_ID_SEG_5D,    0,    0,    LedOnOffSeg5D    },
    { LED_ID_SEG_5E,    0,    0,    LedOnOffSeg5E    },
    { LED_ID_SEG_5F,    0,    0,    LedOnOffSeg5F    },
    { LED_ID_SEG_5G,    0,    0,    LedOnOffSeg5G    },
    { LED_ID_SEG_6A,    0,    0,    LedOnOffSeg6A    },
    { LED_ID_SEG_6B,    0,    0,    LedOnOffSeg6B    },
    { LED_ID_SEG_6C,    0,    0,    LedOnOffSeg6C    },
    { LED_ID_SEG_6D,    0,    0,    LedOnOffSeg6D    },
    { LED_ID_SEG_6E,    0,    0,    LedOnOffSeg6E    },
    { LED_ID_SEG_6F,    0,    0,    LedOnOffSeg6F    },
    { LED_ID_SEG_6G,    0,    0,    LedOnOffSeg6G    },
};

#define SZ_LED_LIST       ( sizeof(LedList) / sizeof(SLedList_T) )

static void OnOffTick(SLedList_T *ledList, U16 ledCntCycle)
{
    if( ledCntCycle < ledList->CurrentTick )
    {
        ledList->LedOnOff(ON);
    }
    else
    {
        ledList->LedOnOff(OFF);
    }
}

static void Sync(SLedList_T *ledList)
{
    if( ledList->TargetTick > ledList->CurrentTick )
    {
        ledList->CurrentTick++;
    }
    else if( ledList->TargetTick < ledList->CurrentTick )
    {
        ledList->CurrentTick--;
    }
}

void OnOffLed(ELedId_T id, U8 duty)
{
    SLedList_T *pLedList = NULL;
    U8         ledType = 0U;
    U8         ledDuty = 0U;

    ledType = ( duty & (0xF0) );
    ledDuty = ConvertProtocol2Duty( duty & (0x0F) );

    pLedList = &LedList[id];

    pLedList->TargetTick = ConvDuty2Tick(ledDuty);

    if( ledType == LED_TYPE_DUTY )
    {
        pLedList->CurrentTick = ConvDuty2Tick(ledDuty);    
    }
}

void AllOffLed(void)
{
    U8 i = 0U;

    for( i=0; i<LED_ID_MAX; i++ )
    {
        OnOffLed((ELedId_T)i, 0U);
    }
}

U8 ConvertDuty2Protocol(ELedId_T led)
{
    SLedList_T *pLedList = NULL;

    U16 TarDuty = 0U;
    U8 result = 0U;

    pLedList = &LedList[led];
    TarDuty = pLedList->TargetTick;

    if( TarDuty >= 100 )
    {
        result = PROTOCOL_LED_DUTY_100;
    }
    else if( TarDuty >= 90 )
    {
        result = PROTOCOL_LED_DUTY_90;
    }
    else if( TarDuty >= 80 )
    {
        result = PROTOCOL_LED_DUTY_80;
    }
    else if( TarDuty >= 70 )
    {
        result = PROTOCOL_LED_DUTY_70;
    }
    else if( TarDuty >= 60 )
    {
        result = PROTOCOL_LED_DUTY_60;
    }
    else if( TarDuty >= 50 )
    {
        result = PROTOCOL_LED_DUTY_50;
    }
    else if( TarDuty >= 40 )
    {
        result = PROTOCOL_LED_DUTY_40;
    }
    else if( TarDuty >= 30 )
    {
        result = PROTOCOL_LED_DUTY_30;
    }
    else if( TarDuty >= 20 )
    {
        result = PROTOCOL_LED_DUTY_20;
    }
    else if( TarDuty >= 10 )
    {
        result = PROTOCOL_LED_DUTY_10;
    }
    else
    {
        result = PROTOCOL_LED_DUTY_0;
    }

    return result;
}

U8 ConvertProtocol2Duty(U8 data)
{
    U8 result = 0U;

    switch( data )
    {
        case PROTOCOL_LED_DUTY_100 :
            result = 100;
            break;

        case PROTOCOL_LED_DUTY_90 :
            result = 90;
            break;

        case PROTOCOL_LED_DUTY_80 :
            result = 80;
            break;

        case PROTOCOL_LED_DUTY_70 :
            result = 70;
            break;

        case PROTOCOL_LED_DUTY_60 :
            result = 60;
            break;

        case PROTOCOL_LED_DUTY_50 :
            result = 50;
            break;

        case PROTOCOL_LED_DUTY_40 :
            result = 40;
            break;

        case PROTOCOL_LED_DUTY_30 :
            result = 30;
            break;

        case PROTOCOL_LED_DUTY_20 :
            result = 20;
            break;

        case PROTOCOL_LED_DUTY_10 :
            result = 10;
            break;

        default :
            result = 0;
            break;
    }

    return result;
}

void ControlLed(void)
{
    U8 i = 0U;
    
    LedCnt.LedCntCycle++;
    if( LedCnt.LedCntCycle > MAX_DUTY_TICK )
    {
        LedCnt.LedCntCycle = 0U;
    }

    LedCnt.LedCntSyncDelay--;

    for(i=0; i<SZ_LED_LIST; i++ )
    {
        OnOffTick(&LedList[i], LedCnt.LedCntCycle);

        if( LedCnt.LedCntSyncDelay == 0 )
        {
            Sync(&LedList[i]);
        }
    }

    if( LedCnt.LedCntSyncDelay == 0 )
    {
        LedCnt.LedCntSyncDelay = MAX_SYNC_DELAY;
    }
}
