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
    /* led_id                              Target_Tick  CurrentTick  Fun_LedOnOff */
    /* 살균부 */
    { LED_ID_UV_HOT_STER_TXT,               0,          0,           LedOnOffUvHotSterTxt            },
    { LED_ID_UV_STER_TXT,                   0,          0,           LedOnOffLedUvCareSystem               },
    { LED_ID_UV_WATER_FAUCET,               0,          0,           LedOnOffLedUvWaterFaucet           },
    { LED_ID_UV_ICE_FAUCET,                 0,          0,           LedOnOffLedUvIceFaucet             },
    { LED_ID_UV_ICE_TRAY,                   0,          0,           LedOnOffLedUvIceTray               },
    { LED_ID_UV_ICE_STORAGE_BOX,            0,          0,           LedOnOffLedUvIceTank         },

    /* 좌측 온도설정 단계 */
    { LED_ID_TEMP_STEP_1,                   0,          0,           LedOnOffLedTempStep1               },
    { LED_ID_TEMP_STEP_2,                   0,          0,           LedOnOffLedTempStep2               },
    { LED_ID_TEMP_STEP_3,                   0,          0,           LedOnOffLedTempStep3               },
    { LED_ID_TEMP_STEP_4,                   0,          0,           LedOnOffLedTempStep4               },
    { LED_ID_TEMP_STEP_5,                   0,          0,           LedOnOffLedTempStep5               },

    /* 얼음가득/부족 */
    { LED_ID_ICE_FULL_TXT,                  0,          0,           LedOnOffLedStatusIceFull              },
    { LED_ID_ICE_LACK_TXT,                  0,          0,           LedOnOffLedStatusIceLack              },

    /* 빠른제빙/취침모드/:/'C */
    { LED_ID_FAST_ICE_ICON,                 0,          0,           LedOnOffFastIce             },
    { LED_ID_SLEEP_ICON,                    0,          0,           LedOnOffSleepIcon               },
    { LED_ID_COLON,                         0,          0,           LedOnOffColon                   },
    { LED_ID_CELCIUS,                       0,          0,           LedOnOffLedStatusUnitCelsius                 },

    /* 메뉴 아이콘 */
    { LED_ID_RAMEN,                         0,          0,           LedOnOffRamen                   },
    { LED_ID_DRIP_COFFEE,                   0,          0,           LedOnOffDripCoffee              },
    { LED_ID_TEA,                           0,          0,           LedOnOffTea                     },
    { LED_ID_MY1_ICON,                      0,          0,           LedOnOffMy1                 },
    { LED_ID_MY2_ICON,                      0,          0,           LedOnOffMy2                 },
    { LED_ID_MY3_ICON,                      0,          0,           LedOnOffMy3                 },

    /* 세그먼트 및 필터 */
    { LED_ID_SEG_DOT,                       0,          0,           LedOnOffSegDot                  },
    { LED_ID_FILTER_CLEAN_TXT,              0,          0,           LedOnOffFilterCleanTxt          },

    /* WiFi 및 날씨 아이콘 */
    { LED_ID_WIFI_ICON,                     0,          0,           LedOnOffWifi                },
    { LED_ID_SUNNY_ICON,                    0,          0,           LedOnOffSunny               },
    { LED_ID_CLOUD_ICON,                    0,          0,           LedOnOffCloud               },
    { LED_ID_UMBRELLA_ICON,                 0,          0,           LedOnOffRain            },

    /* 양 설정 단계 */
    { LED_ID_AMOUNT_INFINITE,               0,          0,           LedOnOffLedStatusBarInfinite          },
    { LED_ID_AMOUNT_STEP_4,                 0,          0,           LedOnOffLedStatusBarHigh             },
    { LED_ID_AMOUNT_STEP_3,                 0,          0,           LedOnOffLedStatusBarMiddle             },
    { LED_ID_AMOUNT_STEP_2,                 0,          0,           LedOnOffLedStatusBar2             },
    { LED_ID_AMOUNT_STEP_1,                 0,          0,           LedOnOffLedStatusBarLow             },

    /* 단위 표시 */
    { LED_ID_PERCENT,                       0,          0,           LedOnOffPercent                 },
    { LED_ID_LITER,                         0,          0,           LedOnOffLedStatusUnitL                   },
    { LED_ID_ML,                            0,          0,           LedOnOffLedStatusUnitMl                      },

    /* 메뉴 선택 */
    { LED_ID_ICE_WATER_SEL,                 0,          0,           LedOnOffLedIceWater             },
    { LED_ID_ICE_SEL,                       0,          0,           LedOnOffLedIce                  },
    { LED_ID_HOT_WATER_SEL,                 0,          0,           LedOnOffLedHot             },
    { LED_ID_ROOM_WATER_SEL,                0,          0,           LedOnOffLedRoom            },
    { LED_ID_COLD_WATER_SEL,                0,          0,           LedOnOffLedCold            },
    { LED_ID_AMOUNT_SEL,                    0,          0,           LedOnOffLedAmount               },
    { LED_ID_MY_SEL,                        0,          0,           LedOnOffMyKey                   },

    /* 얼음 타입 및 잠금 */
    { LED_ID_ICE_TYPE_NORMAL,               0,          0,           LedOnOffIceTypeNormal           },
    { LED_ID_ICE_TYPE_PIECE,                0,          0,           LedOnOffIceTypePiece            },
    { LED_ID_ICE_LOCKER,                    0,          0,           LedOnOffIceLocker               },
    { LED_ID_HOT_WATER_LOCKER,              0,          0,           LedOnOffIceHotWaterLocker          },

    /* 냉각 컴프레서 */
    { LED_ID_COLD_COMP_WHITE,               0,          0,           LedOnOffIceCompWhite           },
    { LED_ID_COLD_COMP_BLUE,                0,          0,           LedOnOffIceCompBlue            },

    /* 설정 선택 */
    { LED_ID_ICE_OFF_SEL,                   0,          0,           LedOnOffLedIceOff               },
    { LED_ID_ICE_SIZE_SEL,                  0,          0,           LedOnOffIceSizeKey              },
    { LED_ID_ICE_BIG,                       0,          0,           LedOnOffIceBigIcon                  },
    { LED_ID_ICE_LINE,                      0,          0,           LedOnOffIceLine                 },
    { LED_ID_ICE_SMALL,                     0,          0,           LedOnOffIceSmallIcon                },
    { LED_ID_HOT_WATER_LOCK_SEL,            0,          0,           LedOnOffHotWaterLockKey         },
    { LED_ID_FAST_ICE_SEL,                  0,          0,           LedOnOffFastIceKey              },
    { LED_ID_COLD_TEMP,                     0,          0,           LedOnOffColdTemp                },
    { LED_ID_COLD_TEMP_WEAK,                0,          0,           LedOnOffColdTempWeak            },
    { LED_ID_COLD_TEMP_LINE,                0,          0,           LedOnOffColdTempLine            },
    { LED_ID_COLD_TEMP_STRONG,              0,          0,           LedOnOffColdTempStrong          },
    { LED_ID_COLD_OFF_SEL,                  0,          0,           LedOnOffColdOffKey              },
    { LED_ID_SLEEP_SEL,                     0,          0,           LedOnOffSleepModeKey                },

    /* 잠금 선택 */
    { LED_ID_ICE_LOCK_SEL,                  0,          0,           LedOnOffIceLockKey              },
    { LED_ID_ALL_LOCK_SEL,                  0,          0,           LedOnOffAllLockKey              },

    /* 얼음 내부 LED (RGB) */
    { LED_ID_ICE_EX_IN_WHITE_SEL,           0,          0,           LedOnOffLedEXTRACTLeftWhiteI         },
    { LED_ID_ICE_EX_IN_RED_SEL,             0,          0,           LedOnOffLedEXTRACTLeftRedI           },
    { LED_ID_ICE_EX_IN_BLUE_SEL,            0,          0,           LedOnOffLedEXTRACTLeftBlueI          },
    { LED_ID_ICE_EX_IN_GREEN_SEL,           0,          0,           LedOnOffLedEXTRACTLeftGreenI         },

    /* 얼음 외부 LED (RGB) */
    { LED_ID_ICE_EX_OUTER_WHITE_SEL,        0,          0,           LedOnOffLedEXTRACTLeftWhiteOuter      },
    { LED_ID_ICE_EX_OUTER_RED_SEL,          0,          0,           LedOnOffLedEXTRACTLeftRedOuter        },
    { LED_ID_ICE_EX_OUTER_BLUE_SEL,         0,          0,           LedOnOffLedEXTRACTLeftBlueOuter       },
    { LED_ID_ICE_EX_OUTER_GREEN_SEL,        0,          0,           LedOnOffLedEXTRACTLeftGreenOuter      },

    /* 물 외부 LED (RGB) */
    { LED_ID_WATER_EX_WHITE_SEL,            0,          0,           LedOnOffLedEXTRACTRightWhite         },
    { LED_ID_WATER_EX_RED_SEL,              0,          0,           LedOnOffLedEXTRACTRightRed           },
    { LED_ID_WATER_EX_BLUE_SEL,             0,          0,           LedOnOffLedEXTRACTRightBlue          },
    { LED_ID_WATER_EX_GREEN_SEL,            0,          0,           LedOnOffLedEXTRACTRightGreen         },
    
    /* WELCOME LED */
    { LED_ID_WELCOME_LED_LEFT,            0,          0,            LedOnOffWelcomeLedLeft         },
    { LED_ID_WELCOME_LED_RIGHT,            0,          0,           LedOnOffWelcomeLedRight         },
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
