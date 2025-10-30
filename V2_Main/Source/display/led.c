/**
 * File : led.h
 * 
 * Led
**/
#include "led.h"

#include "front.h"

/* Front Control Function */
typedef struct _led_list_
{
    ELedId_T Led;
    U8 OnOff;
    U8 Type;
    U8 CurrentDuty;
    U8 OnDuty;
    U8 OffDuty;
} SLedList_T;

/**
 * 10ms Scheduler Time Delay : led 가 많아지면서 find index for 문과 led 선택 for문이 중첩이 되어 지연이 악화됨
 * LedList 안의 led id는 인덱스와 같이 매칭시켜주어 frind index를 할 필요없이 진행함 => 지연해소됨
 */
SLedList_T LedList[] = 
{
    /* led_id                               On_Off  TYPE                Current_Duty  On_Duty            Off_Duty  */
    { LED_ID_SEG_1A,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_90,   LED_DUTY_OFF    },
    { LED_ID_SEG_1B,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_1C,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_1D,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_1E,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_1F,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_1G,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_2A,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_2B,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_2C,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_2D,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_2E,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_2F,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_2G,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_3A,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_3B,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_3C,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_3D,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_3E,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_3F,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_3G,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_4A,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_4B,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_4C,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_4D,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_4E,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_4F,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_4G,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_5A,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_5B,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_5C,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_5D,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_5E,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_5F,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_5G,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_6A,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_6B,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_6C,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_6D,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_6E,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_6F,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SEG_6G,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    
    /* 살균부 */
    { LED_ID_UV_HOT_STER_TXT,           OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_UV_STER_TXT,               OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_UV_WATER_FAUCET,           OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_UV_ICE_FAUCET,             OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_UV_ICE_TRAY,               OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_UV_ICE_STORAGE_BOX,        OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },

    /* 좌측 온도설정 단계 */
    { LED_ID_TEMP_STEP_1,               OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_TEMP_STEP_2,               OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_TEMP_STEP_3,               OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_TEMP_STEP_4,               OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_TEMP_STEP_5,               OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },

    /* 얼음가득/부족 */
    { LED_ID_ICE_FULL_TXT,              OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_LACK_TXT,              OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },

    /* 빠른제빙/취침모드/:/'C */
    { LED_ID_FAST_ICE_ICON,             OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SLEEP_ICON,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_COLON,                     OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_CELCIUS,                   OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },

    { LED_ID_RAMEN,                     OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_DRIP_COFFEE,               OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_TEA,                       OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_MY1_ICON,                  OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_MY2_ICON,                  OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_MY3_ICON,                  OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    
    { LED_ID_SEG_DOT,                   OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_FILTER_CLEAN_TXT,          OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    
    { LED_ID_WIFI_ICON,                 OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SUNNY_ICON,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_CLOUD_ICON,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_UMBRELLA_ICON,             OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    
    { LED_ID_AMOUNT_INFINITE,           OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_AMOUNT_STEP_4,             OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_AMOUNT_STEP_3,             OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_AMOUNT_STEP_2,             OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_AMOUNT_STEP_1,             OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },

    { LED_ID_PERCENT,                   OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_LITER,                     OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ML,                        OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },

    { LED_ID_ICE_WATER_SEL,             OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_SEL,                   OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_HOT_WATER_SEL,             OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ROOM_WATER_SEL,            OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_COLD_WATER_SEL,            OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_AMOUNT_SEL,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_MY_SEL,                    OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    
    { LED_ID_ICE_TYPE_NORMAL,           OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_TYPE_PIECE,            OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_LOCKER,                OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_HOT_WATER_LOCKER,          OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },

    { LED_ID_COLD_WATER_COMP_WHITE,           OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_COLD_WATER_COMP_BLUE,            OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    
    { LED_ID_ICE_OFF_SEL,               OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_SIZE_SEL,              OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_BIG,                   OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_LINE,                  OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_SMALL,                 OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_HOT_WATER_LOCK_SEL,        OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_FAST_ICE,              OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_COLD_WATER_SEL_TEMP,                 OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_COLD_WATER_SEL_TEMP_WEAK,            OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_COLD_WATER_SEL_TEMP_LINE,            OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_COLD_WATER_SEL_TEMP_STRONG,          OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_COLD_WATER_SEL_OFF_SEL,              OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_SLEEP_SEL,                 OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    
    { LED_ID_ICE_OFF_SEL_SEL,              OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ALL_LOCK_SEL,              OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },

    { LED_ID_ICE_EX_IN_WHITE_SEL,       OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_EX_IN_RED_SEL,         OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_EX_IN_BLUE_SEL,        OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_EX_IN_GREEN_SEL,       OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },

    { LED_ID_ICE_EX_OUTER_WHITE_SEL,    OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_EX_OUTER_RED_SEL,      OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_EX_OUTER_BLUE_SEL,     OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_ICE_EX_OUTER_GREEN_SEL,    OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },

    { LED_ID_WATER_EX_WHITE_SEL,        OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_WATER_EX_RED_SEL,          OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_WATER_EX_BLUE_SEL,         OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_WATER_EX_GREEN_SEL,        OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },

    /* WELCOME LED */
    { LED_ID_WELCOME_LED_LEFT,          OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
    { LED_ID_WELCOME_LED_RIGHT,         OFF,   LED_TYPE_DUTY_DIM,   0,            LED_DUTY_ON_100,   LED_DUTY_OFF    },
};

#define SZ_LED_LIST       ( sizeof(LedList) / sizeof(SLedList_T) )

/**
 * @ brief                Function For Find Index Of Led Id in List
 * @ detail               none
 * @ param     mEled      led id 
 * @ param     xUindex    Index
 * @ return               Result
**/
static U8 FindLedId(ELedId_T led, U8 *pIndex)
{
    U8 i = 0U;
    U8 result = FALSE;
    SLedList_T *pLedList = NULL;

    for( i=0; i<SZ_LED_LIST; i++ )
    {
        pLedList = &LedList[i];
    
        if( pLedList->Led == led )
        {
            *pIndex = i;
            result = TRUE;
            break;
        }
    }

    return result;
}

/**
 * @ brief                Function For OnOff Led
 * @ detail               none
 * @ param     mEled      led id 
 * @ param     xUonOff    On Off Status
 * @ return               none
**/
void OnOffLed(ELedId_T led, U8 onOff)
{
    //U8 findResult = 0;
    //U8 index = 0;
    SLedList_T *pLedList = NULL;
    
    //findResult = FindLedId(led, &index);

    //if( findResult == FALSE )
    //{
    //    return;
    //}

    //pLedList = &LedList[index];
    pLedList = &LedList[led];

    pLedList->OnOff = onOff;

    if( pLedList->OnOff == ON )
    {
        // ON
        pLedList->CurrentDuty = pLedList->OnDuty;
    }
    else
    {
        // OFF
        pLedList->CurrentDuty = pLedList->OffDuty;;
    }
}

void SetLedType(ELedId_T led, U8 type)
{
    //U8 findResult = 0;
    //U8 index = 0;
    SLedList_T *pLedList = NULL;

    //findResult = FindLedId(led, &index);

    //if( findResult == FALSE )
    //{
    //  return;
    //} 

    //pLedList = &LedList[index];
    pLedList = &LedList[led];

    pLedList->Type = type;
}

U8 GetLedType(ELedId_T led)
{
    //U8 findResult = 0;
    //U8 index = 0;
    SLedList_T *pLedList = NULL;

    //findResult = FindLedId(led, &index);

    //if( findResult == FALSE )
    //{
    //    return;
    //}

    //pLedList = &LedList[index];
    pLedList = &LedList[led];

    return (U8)pLedList->Type;
}

void SetLedOnDuty(ELedId_T led, U8 duty)
{
    //U8 findResult = 0;
    //U8 index = 0;
    SLedList_T *pLedList = NULL;

    //findResult = FindLedId(led, &index);

    //if( findResult == FALSE )
    //{
    //    return;
    //}

    //pLedList = &LedList[index];
    pLedList = &LedList[led];

    pLedList->OnDuty = duty;
}

void SetLedOffDuty(ELedId_T led, U8 duty)
{
    //U8 findResult = 0;
    //U8 index = 0;
    SLedList_T *pLedList = NULL;

    //findResult = FindLedId(led, &index);

    //if( findResult == FALSE )
    //{
    //    return;
    //}

    //pLedList = &LedList[index];
    pLedList = &LedList[led];

    pLedList->OffDuty = duty;
}

/**
 * @ brief                Function For Get Led Current Duty ( For Protocol )
 * @ detail               none
 * @ param     mEled      led id 
 * @ return               led Duty 
**/
U8 GetLedDuty(ELedId_T led)
{
    //U8 findResult = 0;
    //U8 index = 0;
    SLedList_T *pLedList = NULL;

    //findResult = FindLedId(led, &index);

    //if( findResult == FALSE )
    //{
    //    return;
    //}

    //pLedList = &LedList[index];
    pLedList = &LedList[led];

    return (U8)pLedList->CurrentDuty;
}

void OnAllLed(void)
{
    U8 i = 0;
    SLedList_T *pLedList = NULL;

    for( i=0; i<SZ_LED_LIST; i++ )
    {
        pLedList = &LedList[i];
        pLedList->OnOff = ON;
        pLedList->CurrentDuty = pLedList->OnDuty;
    }
}

void OffAllLed(void)
{
    U8 i = 0;
    SLedList_T *pLedList = NULL;

    for( i=0; i<SZ_LED_LIST; i++ )
    {
        pLedList = &LedList[i];
        pLedList->OnOff = OFF;
        pLedList->CurrentDuty = 0;
    }
}

U8 ConvertDuty2Protocol(ELedId_T led)
{
    U8 curType = 0U;
    U8 curDuty = 0U;
    U8 result = 0U;

    curType = GetLedType(led);
    curDuty = GetLedDuty(led);

    result = curType;

    if( curDuty >= 100 )
    {
        result |= PROTOCOL_LED_DUTY_100;
    }
    else if( curDuty >= 90 )
    {
        result |= PROTOCOL_LED_DUTY_90;
    }
    else if( curDuty >= 80 )
    {
        result |= PROTOCOL_LED_DUTY_80;
    }
    else if( curDuty >= 70 )
    {
        result |= PROTOCOL_LED_DUTY_70;
    }
    else if( curDuty >= 60 )
    {
        result |= PROTOCOL_LED_DUTY_60;
    }
    else if( curDuty >= 50 )
    {
        result |= PROTOCOL_LED_DUTY_50;
    }
    else if( curDuty >= 40 )
    {
        result |= PROTOCOL_LED_DUTY_40;
    }
    else if( curDuty >= 30 )
    {
        result |= PROTOCOL_LED_DUTY_30;
    }
    else if( curDuty >= 20 )
    {
        result |= PROTOCOL_LED_DUTY_20;
    }
    else if( curDuty >= 10 )
    {
        result |= PROTOCOL_LED_DUTY_10;
    }
    else
    {
        result |= PROTOCOL_LED_DUTY_0;
    }

    return result;
}
