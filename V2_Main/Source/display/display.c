#include "display.h"
#include "hal_led.h"
#include "display_lib.h"
#include "display_clock.h"
#include "drain.h"
#include "water_out.h"
#include "power_saving.h"
#include "ice.h"
#include "ice_full.h"
#include "ice_maker.h"
#include "cold_water.h"
#include "hot_water.h"
#include "comp.h"
#include "gas_switch_valve.h"
#include "front.h"
#include "voice.h"
#include "power_saving.h"


/* 7segment:  0 ~ 9 */
static U8 gu8Digit[10] = 
{
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x07F, 0x06F 
};

/* 7segment Char */
#define CHAR_O              0x3F
#define CHAR_n              0x54
#define CHAR_F              0x71
#define CHAR_S              0x6D
#define CHAR_T              0x78
#define CHAR_E              0x79
#define CHAR_o              0x5C
#define CHAR_C              0x39
#define CHAR_H              0x76
#define CHAR_A              0x77
#define CHAR_P              0x73
#define CHAR_SPACE          0x00
#define BAR_LEFT            0x30
#define BAR_RIGHT           0x06
#define BAR_TOP             0x01
#define BAR_CENTER          0x40
#define BAR_BOTTOM          0x08
#define BAR_BRAKET_LEFT     0x39    // '['
#define BAR_BRAKET_RIGHT    0x0F    // ']'
#define BAR_TOP_BOTTOM      0x09    // '='
#define RING_TOP            0x63    // 
#define RING_BOTTOM         0x5C
#define STACK_BAR_CENTER   (BAR_CENTER|BAR_BOTTOM)
#define STACK_BAR_TOP      (BAR_CENTER|BAR_BOTTOM|BAR_TOP)

#define SEG_OFF             0x00    //
#define SEG_A               0x01    // SEG_0
#define SEG_B               0x02    // SEG_1
#define SEG_C               0x04    // SEG_2
#define SEG_D               0x08    // SEG_4
#define SEG_E               0x10    // SEG_5
#define SEG_F               0x20    // SEG_6
#define SEG_G               0x40    // SEG_7

#define SEG_EF              (SEG_E|SEG_F)
#define SEG_EFA             (SEG_E|SEG_F|SEG_A)
#define SEG_EFAD            (SEG_E|SEG_F|SEG_A|SEG_D)
#define SEG_FAD             (SEG_F|SEG_A|SEG_D)
#define SEG_AD              (SEG_A|SEG_D)
#define SEG_AB              (SEG_A|SEG_B)
#define SEG_ABC             (SEG_A|SEG_B|SEG_C)
#define SEG_ABCD            (SEG_A|SEG_B|SEG_C|SEG_D)
#define SEG_BCD             (SEG_B|SEG_C|SEG_D)
#define SEG_CD              (SEG_C|SEG_D)


/* 7segemnt id */
#define SEGMENT_1       SEG_3_G
#define SEGMENT_10      SEG_2_G
#define SEGMENT_100     SEG_1_G
#define SEGMENT_1000    SEG_0_C
static void DispSegment(U8 mu8Seg, U8 mu8Val)
{
    LedId_T mLedId;

    if( mu8Seg == SEGMENT_1000 )
    {
        mLedId = mu8Seg;
        HAL_TurnOnOffLED( mLedId,      ((mu8Val & 0x04) >> 2) );
        HAL_TurnOnOffLED( mLedId + 1,  ((mu8Val & 0x02) >> 1) );
    }
    else
    {
        mLedId = mu8Seg;
        HAL_TurnOnOffLED( mLedId,     ((mu8Val & 0x40) >> 6) );
        HAL_TurnOnOffLED( mLedId + 1, ((mu8Val & 0x20) >> 5) );
        HAL_TurnOnOffLED( mLedId + 2, ((mu8Val & 0x10) >> 4) );
        HAL_TurnOnOffLED( mLedId + 3, ((mu8Val & 0x08) >> 3) );
        HAL_TurnOnOffLED( mLedId + 4, ((mu8Val & 0x04) >> 2) );
        HAL_TurnOnOffLED( mLedId + 5, ((mu8Val & 0x02) >> 1) );
        HAL_TurnOnOffLED( mLedId + 6, (mu8Val & 0x01)  );
    }
}

#if 0
static void DispSegementEffect(U8 mu8Seg, U8 mu8Val)
{
    LedId_T mLedId;

    if( mu8Seg == SEGMENT_1000 )
    {
        mLedId = mu8Seg;

        HAL_DimmingLed( mLedId,     ((mu8Val & 0x04) >> 2) );
        HAL_DimmingLed( mLedId + 1, ((mu8Val & 0x02) >> 1) );
    }
    else
    {
        mLedId = mu8Seg;
        HAL_DimmingLed( mLedId,     ((mu8Val & 0x40) >> 6) );
        HAL_DimmingLed( mLedId + 1, ((mu8Val & 0x20) >> 5) );
        HAL_DimmingLed( mLedId + 2, ((mu8Val & 0x10) >> 4) );
        HAL_DimmingLed( mLedId + 3, ((mu8Val & 0x08) >> 3) );
        HAL_DimmingLed( mLedId + 4, ((mu8Val & 0x04) >> 2) );
        HAL_DimmingLed( mLedId + 5, ((mu8Val & 0x02) >> 1) );
        HAL_DimmingLed( mLedId + 6,  (mu8Val & 0x01) );
    }
}



void DispSegmentDigitEffect(U16 mu16Val)
{
    static U16 mu16PrevVal = 0;

    if( mu16PrevVal != mu16Val )
    {
        mu16PrevVal = mu16Val;
        DispSegementEffect( SEGMENT_100, gu8Digit[ 8 ] );
        DispSegementEffect( SEGMENT_10, gu8Digit[ 8 ] );
        DispSegementEffect( SEGMENT_1, gu8Digit[ 8 ] );
    }
    else
    {
        DispSegementEffect( SEGMENT_100, CHAR_SPACE );
        DispSegementEffect( SEGMENT_10,  CHAR_SPACE );
        DispSegementEffect( SEGMENT_1,   CHAR_SPACE );
    }

    DispSegmentDigit( mu16Val );
}
#endif

void DispSegmentDigit(U16 mu16Val)
{
    U8 mu8Val_1000;
    U8 mu8Val_100;
    U8 mu8Val_10;
    U8 mu8Val_1;

    mu8Val_1000 = (U8)(mu16Val / 1000);
    mu8Val_100  = (U8)((mu16Val % 1000) / 100);
    mu8Val_10   = (U8)((mu16Val % 100) / 10);
    mu8Val_1    = (U8)((mu16Val % 10));

    if( mu8Val_1000 > 0 )
    {
        DispSegment( SEGMENT_1000, gu8Digit[ mu8Val_1000 ] );
    }
    else
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE );
    }

    if( mu8Val_100 > 0 || mu8Val_1000 > 0)
    {
        DispSegment( SEGMENT_100, gu8Digit[ mu8Val_100 ] );
    }
    else
    {
        DispSegment( SEGMENT_100, CHAR_SPACE );
    }

    if( mu8Val_10 > 0 || mu8Val_100 > 0 || mu8Val_1000 > 0 )
    {
        DispSegment( SEGMENT_10, gu8Digit[ mu8Val_10 ] );
    }
    else
    {
        DispSegment( SEGMENT_10, CHAR_SPACE );
    }

    DispSegment( SEGMENT_1, gu8Digit[ mu8Val_1 ] );
}


/* mu8Val : 0 ~ 9
 * display -1-.. -2- ... -3-...
 */

static void DispCenterVal(U8 mu8Val)
{
    if( mu8Val > 9 )
    {
        mu8Val = 9;
    }
    DispSegment( SEGMENT_1000,  CHAR_SPACE  );
    DispSegment( SEGMENT_100,   BAR_CENTER );
    DispSegment( SEGMENT_10,    gu8Digit[mu8Val] );
    DispSegment( SEGMENT_1,     BAR_CENTER );
}

// 7세그먼트에 캐릭터 표시
void DispSegmentChar(U8 mu8CharId)
{
    if( mu8CharId == SEGMENT_CHAR_ID_ON )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,  CHAR_O );
        DispSegment( SEGMENT_10,   CHAR_n );
        DispSegment( SEGMENT_1,    CHAR_SPACE);
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_OFF )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,  CHAR_O );
        DispSegment( SEGMENT_10,   CHAR_F );
        DispSegment( SEGMENT_1,    CHAR_F );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CON )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,  CHAR_C );
        DispSegment( SEGMENT_10,   CHAR_o );
        DispSegment( SEGMENT_1,    CHAR_n );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_STACK_BAR_CENTER )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,   STACK_BAR_CENTER  );
        DispSegment( SEGMENT_10,    STACK_BAR_CENTER  );
        DispSegment( SEGMENT_1,     STACK_BAR_CENTER  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_STACK_BAR_TOP )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,   STACK_BAR_TOP  );
        DispSegment( SEGMENT_10,    STACK_BAR_TOP  );
        DispSegment( SEGMENT_1,     STACK_BAR_TOP  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_BAR_BOTTOM )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   BAR_BOTTOM  );
        DispSegment( SEGMENT_10,    BAR_BOTTOM  );
        DispSegment( SEGMENT_1,     BAR_BOTTOM  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_BAR_CENTER )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,   BAR_CENTER  );
        DispSegment( SEGMENT_10,    BAR_CENTER  );
        DispSegment( SEGMENT_1,     BAR_CENTER  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_BAR_TOP )
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE  );
        DispSegment( SEGMENT_100,   BAR_TOP  );
        DispSegment( SEGMENT_10,    BAR_TOP  );
        DispSegment( SEGMENT_1,     BAR_TOP  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CHP )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   CHAR_C  );
        DispSegment( SEGMENT_10,    CHAR_H  );
        DispSegment( SEGMENT_1,     CHAR_P  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CP )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   CHAR_C  );
        DispSegment( SEGMENT_10,    CHAR_P  );
        DispSegment( SEGMENT_1,     CHAR_SPACE  );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CIRCLE )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   BAR_BRAKET_LEFT );
        DispSegment( SEGMENT_10,    BAR_TOP_BOTTOM );
        DispSegment( SEGMENT_1,     BAR_BRAKET_RIGHT );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_ICE_1 )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   RING_TOP );
        DispSegment( SEGMENT_10,    CHAR_SPACE );
        DispSegment( SEGMENT_1,     CHAR_SPACE );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_ICE_2 )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   RING_BOTTOM );
        DispSegment( SEGMENT_10,    RING_TOP );
        DispSegment( SEGMENT_1,     CHAR_SPACE );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_ICE_3 )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   RING_TOP );
        DispSegment( SEGMENT_10,    RING_BOTTOM );
        DispSegment( SEGMENT_1,     RING_TOP );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_ICE_4 )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   RING_BOTTOM );
        DispSegment( SEGMENT_10,    RING_TOP );
        DispSegment( SEGMENT_1,     RING_BOTTOM );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CENTER_1 )
    {
        DispCenterVal( 1 );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CENTER_2 )
    {
        DispCenterVal( 2 );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CENTER_3 )
    {
        DispCenterVal( 3 );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_SE )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   CHAR_SPACE );
        DispSegment( SEGMENT_10,    CHAR_S );
        DispSegment( SEGMENT_1,     CHAR_E );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_CO )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE  );
        DispSegment( SEGMENT_100,   CHAR_SPACE );
        DispSegment( SEGMENT_10,    CHAR_C );
        DispSegment( SEGMENT_1,     CHAR_O );
    }
    else if( mu8CharId == SEGMENT_CHAR_ID_STE )
    {
        DispSegment( SEGMENT_1000,  CHAR_SPACE );
        DispSegment( SEGMENT_100,   CHAR_S );
        DispSegment( SEGMENT_10,    CHAR_T );
        DispSegment( SEGMENT_1,     CHAR_E );
    }
    else
    {
        // mu8CharId == SPACE
        DispSegment( SEGMENT_1000, CHAR_SPACE );
        DispSegment( SEGMENT_100,  CHAR_SPACE );
        DispSegment( SEGMENT_10,   CHAR_SPACE );
        DispSegment( SEGMENT_1,    CHAR_SPACE );
    }
}

void DispSegmentOff(void)
{
    // DISPALY OFF
    HAL_TurnOnOffLED( ICON_AM, OFF );
    HAL_TurnOnOffLED( ICON_PM, OFF );
    HAL_TurnOnOffLED( COLON, OFF );

    HAL_TurnOnOffLED( ICON_PERCENT, OFF );
    HAL_TurnOnOffLED( ICON_ML,      OFF );
    HAL_TurnOnOffLED( ICON_CELSIUS, OFF );

    DispSegment( SEGMENT_1000, CHAR_SPACE );
    DispSegment( SEGMENT_100, CHAR_SPACE );
    DispSegment( SEGMENT_10, CHAR_SPACE );
    DispSegment( SEGMENT_1, CHAR_SPACE );
}

void DispSegmentError(U8 mu8ErrCode )
{
    U8 mu8Val_10;
    U8 mu8Val_1;


    if( mu8ErrCode == 0xFF )
    {
        DispSegmentOff();
    }
    else
    {
        // DISPALY ERROR CODE
        HAL_TurnOnOffLED( ICON_AM, OFF );
        HAL_TurnOnOffLED( ICON_PM, OFF );
        HAL_TurnOnOffLED( COLON, OFF );

        HAL_TurnOnOffLED( ICON_PERCENT, OFF );
        HAL_TurnOnOffLED( ICON_ML,      OFF );
        HAL_TurnOnOffLED( ICON_CELSIUS, OFF );

        DispSegment( SEGMENT_1000, CHAR_SPACE );
        DispSegment( SEGMENT_100,  CHAR_E );

        mu8Val_10   = (U8)((mu8ErrCode % 100) / 10);
        mu8Val_1    = (U8)((mu8ErrCode % 10));
        if( mu8Val_10 > 0 )
        {
            DispSegment( SEGMENT_10, gu8Digit[ mu8Val_10 ] );
        }
        else
        {
            DispSegment( SEGMENT_10, gu8Digit[ 0 ] );
        }

        DispSegment( SEGMENT_1, gu8Digit[ mu8Val_1 ] );
    }
}

void DispSegmentWifiError(U8 mu8ErrCode )
{
    U8 mu8Val_1;


    if( mu8ErrCode == 0xFF )
    {
        DispSegmentOff();
    }
    else
    {
        // DISPALY ERROR CODE
        HAL_TurnOnOffLED( ICON_AM, OFF );
        HAL_TurnOnOffLED( ICON_PM, OFF );
        HAL_TurnOnOffLED( COLON, OFF );

        HAL_TurnOnOffLED( ICON_PERCENT, OFF );
        HAL_TurnOnOffLED( ICON_ML,      OFF );
        HAL_TurnOnOffLED( ICON_CELSIUS, OFF );

        DispSegment( SEGMENT_1000, CHAR_SPACE );
        DispSegment( SEGMENT_100,  CHAR_A );
        DispSegment( SEGMENT_10,   CHAR_P );

        mu8Val_1    = (U8)((mu8ErrCode % 10));
        DispSegment( SEGMENT_1, gu8Digit[ mu8Val_1 ] );
    }
}


#if 0
#define BIG_CIRCLE_1    SEG_A_5
#define BIG_CIRCLE_2    SEG_A_6
#define BIG_CIRCLE_3    SEG_A_1
#define BIG_CIRCLE_4    SEG_B_1
#define BIG_CIRCLE_5    SEG_C_1
#define BIG_CIRCLE_6    SEG_C_2
#define BIG_CIRCLE_7    SEG_C_3
#define BIG_CIRCLE_8    SEG_C_4
#define BIG_CIRCLE_9    SEG_B_4
#define BIG_CIRCLE_10   SEG_A_4
static LedId_T gBigCircle[ 10 ] = 
{
    BIG_CIRCLE_1, 
    BIG_CIRCLE_2, 
    BIG_CIRCLE_3, 
    BIG_CIRCLE_4, 
    BIG_CIRCLE_5, 
    BIG_CIRCLE_6, 
    BIG_CIRCLE_7, 
    BIG_CIRCLE_8, 
    BIG_CIRCLE_9, 
    BIG_CIRCLE_10
};

void DispSegmentCon(U8 mu8Step)
{
    LedId_T mId;


    if( mu8Step == 0 )
    {
        DispSegment( SEGMENT_100,  CHAR_SPACE );
        DispSegment( SEGMENT_10,   CHAR_SPACE );
        DispSegment( SEGMENT_1,    CHAR_SPACE );
    }
    else if( mu8Step >= 1  && mu8Step <= 10)
    {
        mId = gBigCircle[ mu8Step ];
        HAL_TurnOnOffLED( mId, ON );
    }
    else if( mu8Step >= 11 && mu8Step <= 20 )
    {
        mId = gBigCircle[ mu8Step - 10 ];
        HAL_TurnOnOffLED( mId, OFF );
    }
}
#endif

void TurnOffAllLED(void)
{
    HAL_TurnOnOffLEDAll( OFF );
    HAL_DimmingLedAll( OFF );
}


// CHP or CP에 따라 LED 점등 제외...
static void TurnOffChpiLed(void)
{
    if( GetSystem() == SYSTEM_CP )
    {
        HAL_TurnOnOffLEDEx( ICON_HOT_LOCK,   OFF );
        HAL_TurnOnOffLEDEx( HOT_COFFE,  OFF );
        HAL_TurnOnOffLEDEx( HOT_TEA,    OFF );
        HAL_TurnOnOffLEDEx( HOT_MILK,   OFF );
        HAL_TurnOnOffLEDEx( ICE_MAKE,   OFF );
    }
}

void TurnOnAllLED(void)
{
    HAL_TurnOnOffLEDAll( ON );
    HAL_DimmingLedAll( OFF );

    TurnOffChpiLed();
}

#if CONFIG_TEST_LED 
U8 dbg_duty_on           = 90;
U8 dbg_duty_dimming      = 40;

U8 dbg_duty_dark         = 50;
U8 dbg_duty_dimming_dark = 20;
#endif

void DispLedDuty(void)
{
#if CONFIG_TEST_LED
    if( GetSavingConfig() == TRUE )
    {
        HAL_SetLedDuty( dbg_duty_dark );
        HAL_SetLedDimmingDuty( dbg_duty_dimming_dark );
    }
    else
    {
        HAL_SetLedDuty( dbg_duty_on );
        HAL_SetLedDimmingDuty( dbg_duty_dimming );
    }
#else
    if( GetSavingStatus() == TRUE )
    {
        HAL_SetLedDuty( DEFAULT_ONOFF_DUTY_DARK );
        HAL_SetLedDimmingDuty( DEFAULT_DIMMING_DUTY_DARK );
    }
    else
    {
        HAL_SetLedDuty( DEFAULT_ONOFF_DUTY );
        HAL_SetLedDimmingDuty( DEFAULT_DIMMING_DUTY );
    }
#endif
}

#if 0
void OffDispIceOut(void)
{
    HAL_DimmingLed( ICE_OUT_1,  OFF );
    HAL_DimmingLed( ICE_OUT_2,  OFF );

    HAL_TurnOnOffLED( ICE_OUT_1, OFF );
    HAL_TurnOnOffLED( ICE_OUT_2, OFF );
}
#endif

// 얼음 추출
void DispIceOut( U8 mu8OnOff )
{
    // DIMMING
#if 0
    HAL_DimmingLed( WELCOM_ICE, OFF );
    HAL_DimmingLed( ICE_OUT_1,  ON );
    HAL_DimmingLed( ICE_OUT_2,  ON );
#else
    HAL_DimmingLed( WELCOM_ICE, OFF );
    HAL_DimmingLed( ICE_OUT_1,  OFF );
    HAL_DimmingLed( ICE_OUT_2,  OFF );
#endif

    HAL_TurnOnOffLED( ICE_OUT_1, ON );
    HAL_TurnOnOffLED( ICE_OUT_2, ON );
    if( mu8OnOff == OFF )
    {
        // WELCOM 
        HAL_TurnOnOffLED( WELCOM_ICE, OFF );
    }
    else
    {
        // WELCOM 
        HAL_TurnOnOffLED( WELCOM_ICE, ON );
    }


    // DECO CONFIGURATION...
    if( GetConfigDeco( DECO_LED_BUTTON ) == FALSE )
    {
        HAL_TurnOnOffLED( ICE_OUT_1, OFF );
        HAL_TurnOnOffLED( ICE_OUT_2, OFF );
    }

    if( GetConfigDeco( DECO_LED_WELCOME ) == FALSE )
    {
        HAL_TurnOnOffLED( WELCOM_ICE, OFF );
    }
}


void DispWaterOutCircle(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( WATER_OUT_1, mu8OnOff );
    HAL_TurnOnOffLED( WATER_OUT_2, mu8OnOff );
}

#if 0
void OffDispWaterOut(void)
{
    HAL_DimmingLed( WATER_OUT_1, OFF );
    HAL_DimmingLed( WATER_OUT_2, OFF );

    HAL_TurnOnOffLED( WATER_OUT_1, OFF );
    HAL_TurnOnOffLED( WATER_OUT_2, OFF );
}
#endif
// 물 추출
// OFF시 50% 디밍
void DispWaterOut( U8 mu8OnOff )
{
    // Dimming OFF
    HAL_DimmingLed( WATER_OUT_1, OFF );
    HAL_DimmingLed( WATER_OUT_2, OFF );
    HAL_DimmingLed( WELCOM_WATER, OFF );


    HAL_TurnOnOffLED( WATER_OUT_1, ON );
    HAL_TurnOnOffLED( WATER_OUT_2, ON );
    if( mu8OnOff == OFF )
    {
        // WELCOM 
        HAL_TurnOnOffLED( WELCOM_WATER, OFF );
    }
    else
    {
        // WELCOM 
        HAL_TurnOnOffLED( WELCOM_WATER, ON );

    }

    // DECO CONFIGURATION...
    if( GetConfigDeco( DECO_LED_BUTTON ) == FALSE )
    {
        HAL_TurnOnOffLED( WATER_OUT_1, OFF );
        HAL_TurnOnOffLED( WATER_OUT_2, OFF );
    }

    if( GetConfigDeco( DECO_LED_WELCOME ) == FALSE )
    {
        HAL_TurnOnOffLED( WELCOM_WATER, OFF );
    }
}

// 냉각 온도 상태 표시
// 냉수 온도가 높으면 BLUE LED
// 냉수 온도가 낮으면 WHITE LED
void DispColdTemp(U8 mu8OnOff, U8 mu8ColdGood)
{
    // 냉수 기능 OFF
    if( mu8OnOff == OFF )
    {
        HAL_TurnOnOffLED( COLD_TEMP_WHITE, OFF );
        HAL_TurnOnOffLED( COLD_TEMP_BLUE,  OFF );
    }
    else
    {
        if( mu8ColdGood == TRUE )
        {
            HAL_TurnOnOffLED( COLD_TEMP_WHITE, OFF );
            HAL_TurnOnOffLED( COLD_TEMP_BLUE,  ON );
        }
        else
        {
            HAL_TurnOnOffLED( COLD_TEMP_WHITE, ON );
            HAL_TurnOnOffLED( COLD_TEMP_BLUE,  OFF );
        }

    }
}



static void DispSettingUpDownKey(void)
{
    if( IsExpiredDispTimer( DISP_TIMER_UP ) == FALSE )
    {
        HAL_TurnOnOffLED( COLD_WATER, ON );
    }
    else
    {
        HAL_TurnOnOffLED( COLD_WATER, OFF );
    }


    if( GetSystem() == SYSTEM_CHP )
    {
        // 버튼 조작시 
        if( IsExpiredDispTimer( DISP_TIMER_DOWN ) == FALSE )
        {
            HAL_TurnOnOffLED( HOT_WATER, ON );
        }
        else
        {
            HAL_TurnOnOffLED( HOT_WATER, OFF );
        }
    }
    else
    {
        if( IsExpiredDispTimer( DISP_TIMER_DOWN ) == FALSE )
        {
            HAL_TurnOnOffLED( CP_ICE_TURBO, ON );
            HAL_TurnOnOffLED( CP_ICE_TURBO_2, ON );
        }
        else
        {
            HAL_TurnOnOffLED( CP_ICE_TURBO, OFF );
            HAL_TurnOnOffLED( CP_ICE_TURBO_2, OFF );
        }
    }

    // ROOM
    if( IsExpiredDispTimer( DISP_TIMER_NEXT ) == FALSE )
    {
        HAL_TurnOnOffLED( SETTING, ON );
    }
    else
    {
        HAL_TurnOnOffLED( SETTING, OFF );
    }
}

// 사운드 모드에서의 주요 버튼 
void DispSettingClockKey(void)
{
    HAL_DimmingLed( SETTING, ON );
    //HAL_DimmingLed( ICE_MODE, ON );
    HAL_DimmingLed( COLD_WATER, ON );
    //HAL_DimmingLed( ROOM_WATER, ON );
    //HAL_DimmingLed( AMOUNT, ON );
    if( GetSystem() == SYSTEM_CHP )
    {
        HAL_DimmingLed( HOT_WATER, ON );
    }
    else
    {
        HAL_DimmingLed( CP_ICE_TURBO, ON );
        HAL_DimmingLed( CP_ICE_TURBO_2, ON );
    }

    DispSettingUpDownKey();
}

void DispSettingSoundKey(void)
{
    //HAL_DimmingLed( SETTING, ON );
    //HAL_DimmingLed( ICE_MODE, ON );
    HAL_DimmingLed( COLD_WATER, ON );
    HAL_DimmingLed( ROOM_WATER, ON );
    HAL_DimmingLed( AMOUNT, ON );
    if( GetSystem() == SYSTEM_CHP )
    {
        HAL_DimmingLed( HOT_WATER, ON );
    }
    else
    {
        HAL_DimmingLed( CP_ICE_TURBO, ON );
        HAL_DimmingLed( CP_ICE_TURBO_2, ON );
    }

    DispSettingUpDownKey();
}

// 사운드 모드에서의 주요 버튼 
void DispSettingKey(void)
{
    HAL_DimmingLed( SETTING, ON );
    HAL_DimmingLed( ICE_MODE, ON );
    HAL_DimmingLed( COLD_WATER, ON );
    HAL_DimmingLed( ROOM_WATER, ON );
    HAL_DimmingLed( AMOUNT, ON );
    if( GetSystem() == SYSTEM_CHP )
    {
        HAL_DimmingLed( HOT_WATER, ON );
    }
    else
    {
        HAL_DimmingLed( CP_ICE_TURBO, ON );
        HAL_DimmingLed( CP_ICE_TURBO_2, ON );
    }

    DispSettingUpDownKey();
}

void DispIceSizeSel(U8 mu8Size, U8 mu8Lock, U8 mu8Dimming)
{
    HAL_DimmingLed( ICE_TURBO,      OFF );
    HAL_TurnOnOffLED( ICE_TURBO,  OFF );
    HAL_DimmingLed( ICE_SIZE_SMALL, mu8Dimming );
    HAL_DimmingLed( ICE_SIZE_BAR,   mu8Dimming );
    HAL_DimmingLed( ICE_SIZE_LARGE, mu8Dimming );
    HAL_DimmingLed( ICE_MODE,       mu8Dimming );

    HAL_TurnOnOffLED( ICE_SIZE_BAR, OFF );

    // LOCK 
    HAL_DimmingLed( ICON_ICE_LOCK,  OFF );
    HAL_TurnOnOffLED( ICON_ICE_LOCK,  mu8Lock );

    // TURBO 

    // SIZE
    if( mu8Size == ICE_LARGE )
    {
        HAL_TurnOnOffLED( ICE_SIZE_LARGE,  ON );
        HAL_TurnOnOffLED( ICE_SIZE_SMALL,  OFF );
    }
    else
    {
        HAL_TurnOnOffLED( ICE_SIZE_LARGE,  OFF );
        HAL_TurnOnOffLED( ICE_SIZE_SMALL,  ON );
    }

    // ICE MODE
    HAL_TurnOnOffLED( ICE_MODE,  ON );
}

// 얼음 
void DispIceSel(U8 mu8Turbo, U8 mu8Size, U8 mu8Lock, U8 mu8Dimming)
{
    HAL_DimmingLed( ICE_TURBO,      mu8Dimming );
    HAL_DimmingLed( ICE_SIZE_SMALL, mu8Dimming );
    HAL_DimmingLed( ICE_SIZE_BAR,   mu8Dimming );
    HAL_DimmingLed( ICE_SIZE_LARGE, mu8Dimming );
    HAL_DimmingLed( ICE_MODE,       mu8Dimming );

    HAL_TurnOnOffLED( ICE_SIZE_BAR, OFF );

    // LOCK 
    HAL_DimmingLed( ICON_ICE_LOCK,  OFF );
    HAL_TurnOnOffLED( ICON_ICE_LOCK,  mu8Lock );

    // TURBO 
    HAL_TurnOnOffLED( ICE_TURBO,  mu8Turbo );

    // SIZE
    if( mu8Size == ICE_LARGE )
    {
        HAL_TurnOnOffLED( ICE_SIZE_LARGE,  ON );
        HAL_TurnOnOffLED( ICE_SIZE_SMALL,  OFF );
    }
    else
    {
        HAL_TurnOnOffLED( ICE_SIZE_LARGE,  OFF );
        HAL_TurnOnOffLED( ICE_SIZE_SMALL,  ON );
    }

    // ICE MODE
    HAL_TurnOnOffLED( ICE_MODE,  ON );
}

// 얼음 선택 표시 OFF
void DispIceSelOff(U8 mu8OnOff, U8 mu8Turbo, U8 mu8Lock, U8 mu8SizeOnOff, U8 mu8Size)
{
    HAL_DimmingLed( ICE_TURBO,      OFF );
    HAL_DimmingLed( ICE_SIZE_SMALL, OFF );
    HAL_DimmingLed( ICE_SIZE_BAR,   OFF );
    HAL_DimmingLed( ICE_SIZE_LARGE, OFF );
    HAL_DimmingLed( ICE_MODE,       OFF );

    // LOCK 
    HAL_TurnOnOffLED( ICON_ICE_LOCK,  mu8Lock );

    if( mu8OnOff == OFF )
    {
        HAL_TurnOnOffLED( ICE_TURBO,  OFF );
        HAL_TurnOnOffLED( ICE_SIZE_LARGE,  OFF );
        HAL_TurnOnOffLED( ICE_SIZE_SMALL,  OFF );
        HAL_TurnOnOffLED( ICE_MODE,  OFF );
    }
    else
    {
        // TURBO 
        HAL_TurnOnOffLED( ICE_TURBO,  mu8Turbo );

        // SIZE
        if( mu8SizeOnOff == OFF )
        {
            HAL_TurnOnOffLED( ICE_SIZE_LARGE,  OFF );
            HAL_TurnOnOffLED( ICE_SIZE_SMALL,  OFF );
        }
        else
        {
            if( mu8Size == ICE_LARGE )
            {
                HAL_TurnOnOffLED( ICE_SIZE_LARGE,  ON );
                HAL_TurnOnOffLED( ICE_SIZE_SMALL,  OFF );
            }
            else
            {
                HAL_TurnOnOffLED( ICE_SIZE_LARGE,  OFF );
                HAL_TurnOnOffLED( ICE_SIZE_SMALL,  ON );
            }
        }

        // ICE MODE
        HAL_TurnOnOffLED( ICE_MODE,  GetIceConfigMake() );
    }
}

// 얼음 빠른제빙 - Only CP
void DispIceTurbo(U8 mu8OnOff)
{
    HAL_DimmingLed( CP_ICE_TURBO,   ON );
    HAL_DimmingLed( CP_ICE_TURBO_2, ON );

    HAL_TurnOnOffLED( CP_ICE_TURBO, mu8OnOff );
    HAL_TurnOnOffLED( CP_ICE_TURBO_2, mu8OnOff );
}

void DispIceTurboOff(void)
{
    HAL_DimmingLed( CP_ICE_TURBO,   OFF );
    HAL_DimmingLed( CP_ICE_TURBO_2, OFF );

    HAL_TurnOnOffLED( CP_ICE_TURBO, OFF );
    HAL_TurnOnOffLED( CP_ICE_TURBO_2, OFF );
}


void DispWater(U8 mu8Sel)
{
    if( mu8Sel == SEL_WATER_COLD )
    {
        HAL_TurnOnOffLED( COLD_WATER, ON );
        HAL_TurnOnOffLED( ROOM_WATER, OFF );
        HAL_TurnOnOffLED( HOT_WATER, OFF );
    }
    else if( mu8Sel == SEL_WATER_HOT )
    {
        HAL_TurnOnOffLED( COLD_WATER, OFF );
        HAL_TurnOnOffLED( ROOM_WATER, OFF );
        HAL_TurnOnOffLED( HOT_WATER, ON );
    }
    else
    {
        // SEL_WATER_ROOM
        HAL_TurnOnOffLED( COLD_WATER, OFF );
        HAL_TurnOnOffLED( ROOM_WATER, ON );
        HAL_TurnOnOffLED( HOT_WATER, OFF );
    }
}

// 냉온정 선택
void DispWaterSel(U8 mu8Sel, U8 mu8ConfCold)
{
    HAL_DimmingLed( COLD_WATER, mu8ConfCold );
    HAL_DimmingLed( ROOM_WATER, ON );
    HAL_DimmingLed( HOT_WATER, ON );

    DispWater( mu8Sel );
}

void DispWaterSelEmptyColdFaucet(void)
{
    HAL_DimmingLed( COLD_WATER,     ON );
    if( GetSystem() == SYSTEM_CP )
    {
        HAL_DimmingLed( CP_ICE_TURBO,   ON );
    }
    else
    {
        HAL_DimmingLed( HOT_WATER,      ON );
    }
}

void DispWaterSelEmptyColdDrain(void)
{
    HAL_DimmingLed( COLD_WATER,     ON );
    HAL_DimmingLed( ROOM_WATER,      ON );
    if( GetSystem() == SYSTEM_CP )
    {
        HAL_DimmingLed( CP_ICE_TURBO,   ON );
    }
    else
    {
        HAL_DimmingLed( HOT_WATER,      ON );
    }
}


/* 물 추출 LED 제어 
 *
 * Param
 *  - mu8OnOff : ON, OFF 
 */
void DispWelcomWater( U8 mu8OnOff )
{
    HAL_TurnOnOffLED( WELCOM_WATER, mu8OnOff );
}

void DispWelcomIce( U8 mu8OnOff )
{
    HAL_TurnOnOffLED( WELCOM_ICE, mu8OnOff );
}


// 물양 종류 - 변경 중
void DispAmountSel(U8 mu8Amount, U8 mu8Dimming )
{
    HAL_DimmingLed( AMOUNT_INFINITY,    mu8Dimming );
    HAL_DimmingLed( AMOUNT_500,         mu8Dimming );
    HAL_DimmingLed( AMOUNT_250,         mu8Dimming );
    HAL_DimmingLed( AMOUNT_120,         mu8Dimming );

    // AMOUNT 
    HAL_TurnOnOffLED( AMOUNT, ON );

    // AMOUNT TYPE
    if( mu8Amount == FIXED_AMOUNT_INFINITY )
    {
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
        HAL_TurnOnOffLED( AMOUNT_500,       OFF );
        HAL_TurnOnOffLED( AMOUNT_250,       OFF );
        HAL_TurnOnOffLED( AMOUNT_120,       OFF );
    }
    else if( mu8Amount == FIXED_AMOUNT_FOUR )
    {
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  ON );
        HAL_TurnOnOffLED( AMOUNT_500,       OFF );
        HAL_TurnOnOffLED( AMOUNT_250,       OFF );
        HAL_TurnOnOffLED( AMOUNT_120,       OFF );
    }
    else if( (mu8Amount == FIXED_AMOUNT_TWO) || (mu8Amount == UNFIXED_AMOUNT_TWO) )
    {
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
        HAL_TurnOnOffLED( AMOUNT_500,       ON );
        HAL_TurnOnOffLED( AMOUNT_250,       OFF );
        HAL_TurnOnOffLED( AMOUNT_120,       OFF );
    }
    else if( (mu8Amount == FIXED_AMOUNT_ONE) || (mu8Amount == UNFIXED_AMOUNT_ONE) )
    {
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
        HAL_TurnOnOffLED( AMOUNT_500,       OFF );
        HAL_TurnOnOffLED( AMOUNT_250,       ON );
        HAL_TurnOnOffLED( AMOUNT_120,       OFF );
    }
    else if( (mu8Amount == FIXED_AMOUNT_HALF) || (mu8Amount == UNFIXED_AMOUNT_HALF) )
    {
        // FIXED AMOUNT_HALF
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
        HAL_TurnOnOffLED( AMOUNT_500,       OFF );
        HAL_TurnOnOffLED( AMOUNT_250,       OFF );
        HAL_TurnOnOffLED( AMOUNT_120,       ON );
    }
    else
    {
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
        HAL_TurnOnOffLED( AMOUNT_500,       OFF );
        HAL_TurnOnOffLED( AMOUNT_250,       OFF );
        HAL_TurnOnOffLED( AMOUNT_120,       OFF );
    }

#if 0
    // AMOUNT USER ( MY AMOUNT )
    // 설정 해제 상태이면 기능 OFF
    if( GetWaterOutConfigUser() == FALSE )
    {
        HAL_DimmingLed( AMOUNT_USER,      OFF );
        HAL_TurnOnOffLED( AMOUNT_USER,      OFF );
    }
#endif

}



// 물양 종류  표시 - 최종 상태
void DispAmountSelOff(U8 mu8Amount)
{
    // AMOUNT SIZE TYPE - DIMMING OFF
    HAL_DimmingLed( AMOUNT_INFINITY,    OFF );
    HAL_DimmingLed( AMOUNT_500,         OFF );
    HAL_DimmingLed( AMOUNT_250,         OFF );
    HAL_DimmingLed( AMOUNT_120,         OFF );
    HAL_DimmingLed( AMOUNT,             ON );

    // AMOUNT 
    HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
    HAL_TurnOnOffLED( AMOUNT_500,       OFF );
    HAL_TurnOnOffLED( AMOUNT_250,       OFF );
    HAL_TurnOnOffLED( AMOUNT_120,       OFF );
    HAL_TurnOnOffLED( AMOUNT,   ON );

    // AMOUNT TYPE
    if( mu8Amount == FIXED_AMOUNT_FOUR )
    {
        HAL_TurnOnOffLED( AMOUNT_INFINITY,  ON );
    }
    else if( (mu8Amount == FIXED_AMOUNT_TWO) || (mu8Amount == UNFIXED_AMOUNT_TWO) )
    {
        HAL_TurnOnOffLED( AMOUNT_500,       ON );
    }
    else if( (mu8Amount == FIXED_AMOUNT_ONE) || (mu8Amount == UNFIXED_AMOUNT_ONE) )
    {
        HAL_TurnOnOffLED( AMOUNT_250,       ON );
    }
    else if( (mu8Amount == FIXED_AMOUNT_HALF) || (mu8Amount == UNFIXED_AMOUNT_HALF) )
    {
        HAL_TurnOnOffLED( AMOUNT_120,       ON );
    }
}

void DispAmount(U8 mu8OnOff)
{
    // AMOUNT SIZE TYPE - DIMMING OFF
    HAL_DimmingLed( AMOUNT_INFINITY,    OFF );
    HAL_DimmingLed( AMOUNT_500,         OFF );
    HAL_DimmingLed( AMOUNT_250,         OFF );
    HAL_DimmingLed( AMOUNT_120,         OFF );
    HAL_DimmingLed( AMOUNT,             ON );

    // AMOUNT SIZE
    HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
    HAL_TurnOnOffLED( AMOUNT_500,       OFF );
    HAL_TurnOnOffLED( AMOUNT_250,       OFF );
    HAL_TurnOnOffLED( AMOUNT_120,       OFF );

    if( mu8OnOff == ON )
    {
        HAL_TurnOnOffLED( AMOUNT,   ON );
    }
    else
    {
        HAL_TurnOnOffLED( AMOUNT,   OFF );
    }
}

// 온수 종류 - 변경 중
#if 0
void DispHotLock(U8 mu8OnOff )
{
    HAL_TurnOnOffLED( HOT_LOCK, mu8OnOff );
}
#endif


// 온수 선택
#if 0
void DispHotSel(U8 mu8Sel, U8 mu8Lock, U8 mu8Dimming)
{
    // HOT SEL 
    //HAL_DimmingLed( HOT_COFFE,  mu8Dimming );
    //HAL_DimmingLed( HOT_TEA,    mu8Dimming );
    //HAL_DimmingLed( HOT_MILK,   mu8Dimming );

    //if( mu8Sel == SEL_HOT_MILK )
    //{
    //    HAL_TurnOnOffLED( HOT_COFFE,  OFF );
    //    HAL_TurnOnOffLED( HOT_TEA,    OFF );
    //    HAL_TurnOnOffLED( HOT_MILK,   ON );
    //}
    //else if( mu8Sel == SEL_HOT_TEA )
    //{
    //    HAL_TurnOnOffLED( HOT_COFFE,  OFF );
    //    HAL_TurnOnOffLED( HOT_TEA,    ON );
    //    HAL_TurnOnOffLED( HOT_MILK,   OFF );
    //}
    //else if( mu8Sel == SEL_HOT_COFFEE )
    //{
    //    // SEL_HOT_COFFEE
    //    HAL_TurnOnOffLED( HOT_COFFE,  ON );
    //    HAL_TurnOnOffLED( HOT_TEA,    OFF );
    //    HAL_TurnOnOffLED( HOT_MILK,   OFF );
    //}
    //else    
    {
        // SEL_HOT_USER
        HAL_TurnOnOffLED( HOT_COFFE,  OFF );
        HAL_TurnOnOffLED( HOT_TEA,    OFF );
        HAL_TurnOnOffLED( HOT_MILK,   OFF );
    }

    // HOT LOCK 
    HAL_TurnOnOffLED( ICON_HOT_LOCK, mu8Lock );
}
#endif

// 온수 종류  표시 - 최종 상태
//  - 온수 OFF
//  - 그리고 선택된 온수 종류만 표시
void DispHotSelOff(U8 mu8Lock)
{
    // HOT SEL 
    HAL_DimmingLed( HOT_COFFE,  OFF );
    HAL_DimmingLed( HOT_TEA,    OFF );
    HAL_DimmingLed( HOT_MILK,   OFF );

    HAL_TurnOnOffLED( HOT_COFFE,  OFF );
    HAL_TurnOnOffLED( HOT_TEA,    OFF );
    HAL_TurnOnOffLED( HOT_MILK,   OFF );

    HAL_TurnOnOffLED( ICON_HOT_LOCK, mu8Lock );
}


void DispUvCare(U8 mu8OnOff, U8 mu8WaterOut, U8 mu8IceOut, U8 mu8IceTray, U8 mu8IceTank)
{
    if( mu8OnOff == ON )
    {
        // SMART CARE TM
        HAL_TurnOnOffLED( ICON_UV_SMART_CARE_TM, ON );
        HAL_TurnOnOffLED( ICON_UV_SMART_CARE,    ON );

        // DIMMING 
        HAL_DimmingLed( ICON_UV_WATER,    ON );
        HAL_DimmingLed( ICON_UV_ICE,      ON );
        HAL_DimmingLed( ICON_UV_ICE_TRAY, ON );
        HAL_DimmingLed( ICON_UV_ICE_TANK, ON );

        // LED 
        HAL_TurnOnOffLED( ICON_UV_WATER,    mu8WaterOut );
        HAL_TurnOnOffLED( ICON_UV_ICE,      mu8IceOut );
        HAL_TurnOnOffLED( ICON_UV_ICE_TRAY, mu8IceTray );
        HAL_TurnOnOffLED( ICON_UV_ICE_TANK, mu8IceTank );
    }
    else
    {
        HAL_DimmingLed( ICON_UV_WATER,    OFF );
        HAL_DimmingLed( ICON_UV_ICE,      OFF );
        HAL_DimmingLed( ICON_UV_ICE_TRAY, OFF );
        HAL_DimmingLed( ICON_UV_ICE_TANK, OFF );

        HAL_TurnOnOffLED( ICON_UV_WATER,    OFF );
        HAL_TurnOnOffLED( ICON_UV_ICE,      OFF );
        HAL_TurnOnOffLED( ICON_UV_ICE_TRAY, OFF );
        HAL_TurnOnOffLED( ICON_UV_ICE_TANK, OFF );
    }
}


static void TurnOffAllIcon(void)
{
    HAL_TurnOnOffLED( ICON_ML, OFF );
    HAL_TurnOnOffLED( ICON_PERCENT, OFF );
    HAL_TurnOnOffLED( ICON_CELSIUS, OFF );
    HAL_TurnOnOffLED( COLON, OFF );

    HAL_TurnOnOffLED( ICON_AM, OFF );
    HAL_TurnOnOffLED( ICON_PM, OFF );
}

// 버전 표시
void DispVersion( U8 mu8Version)
{
    DispSegmentDigit( mu8Version );

    TurnOffAllIcon();
}

// 버전 표시
//U8 DispVersionOnTime(void)
//{
//    static U16 mu16Delay = 200;    // 2sec.. @10ms..
//
//
//    if( mu16Delay != 0)
//    {
//        mu16Delay--;
//
//        DispVersion( VERSION_MAJOR );
//
//        return FALSE;
//    }
//
//    return TRUE;
//}

// 물 추출량 표시
void DispWaterOutAmount(U16 mu16Val)
{
    if( mu16Val == 9999 )
    {
        // 연속 선택시 mL 미표시
        DispSegmentChar( SEGMENT_CHAR_ID_CIRCLE );
        //DispWaterOutAmountInfinity();   // 연속 추출 표시
        TurnOffAllIcon();
    }
	else if( mu16Val >= 2000)
	{
        DispWaterOutAmountInfinity();   // 연속 추출 표시		
	}
    else
    {
        // 정량 선택시 mL 표시
        DispSegmentDigit( mu16Val );
        TurnOffAllIcon();
        HAL_TurnOnOffLED( ICON_ML, ON );
    }
}

// 퍼센트 표시 0% ~ 100%
void DispPercent(U16 mu16Val)
{
    DispSegmentDigit( mu16Val );

    HAL_TurnOnOffLED( ICON_PERCENT, ON );
}

// 사운드 볼륨 설정 
void DispSettingSoundVolume(U8 mu8Vol)
{
    U8 mu8Percent = 0;

    // Show bar...
    if( mu8Vol == VOICE_VOLUME_1)
    {
        mu8Percent = 20;
    }
    else if( mu8Vol == VOICE_VOLUME_2)
    {
        mu8Percent = 40;
    }
    else if( mu8Vol == VOICE_VOLUME_3)
    {
        mu8Percent = 60;
    }
    else if( mu8Vol == VOICE_VOLUME_4)
    {
        mu8Percent = 80;
    }
    else if( mu8Vol == VOICE_VOLUME_5)
    {
        mu8Percent = 100;
    }
    else
    {
        mu8Percent = 0xFF;  // off
    }
}

// 사운드 모드 설정
void DispSettingSoundMode(U8 mu8Mode)
{
    if( mu8Mode == VOICE_LEVEL_OFF )
    {
        // 무음
        DispSegmentChar( SEGMENT_CHAR_ID_CENTER_1 );
    }
    else if( mu8Mode == VOICE_LEVEL_BEEP )
    {
        // 효과음
        DispSegmentChar( SEGMENT_CHAR_ID_CENTER_2 );
    }
    else 
    {
        // 음성 
        DispSegmentChar( SEGMENT_CHAR_ID_CENTER_3 );
    }
}

void DispSettingCustomerMode(U8 mu8Mode)
{
    if( mu8Mode == CUSTOMER_SELF )
    {
        // 무음
        DispSegmentChar( SEGMENT_CHAR_ID_SE );
    }
    else 
    {
        // 음성 
        DispSegmentChar( SEGMENT_CHAR_ID_CO );
    }
}

static void DispClockItemAm(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ICON_AM, mu8OnOff );
}

static void DispClockItemPm(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ICON_PM, mu8OnOff );
}


TimeData_T mSettingTime;
static void DispClockItemHour(U8 mu8OnOff)
{
    U8 mu8Val_10;
    U8 mu8Val_1;

    if( mu8OnOff == ON )
    {
        mu8Val_10 = (U8)(mSettingTime.Hour / 10);
        mu8Val_1  = (U8)(mSettingTime.Hour % 10);

        if( mu8Val_10 > 0 )
        {
            DispSegment( SEGMENT_1000, gu8Digit[ mu8Val_10 ] );
        }
        else
        {
            DispSegment( SEGMENT_1000, CHAR_SPACE );
        }

        DispSegment( SEGMENT_100, gu8Digit[ mu8Val_1 ] );
    }
    else
    {
        DispSegment( SEGMENT_1000, CHAR_SPACE );
        DispSegment( SEGMENT_100, CHAR_SPACE );
    }
}

static void DispClockItemMin(U8 mu8OnOff)
{
    U8 mu8Val_10;
    U8 mu8Val_1;

    if( mu8OnOff == ON )
    {
        mu8Val_10 = (U8)(mSettingTime.Min / 10);
        mu8Val_1  = (U8)(mSettingTime.Min % 10);

        if( mu8Val_10 > 0 )
        {
            DispSegment( SEGMENT_10, gu8Digit[ mu8Val_10 ] );
        }
        else
        {
            DispSegment( SEGMENT_10, gu8Digit[ 0 ] );
        }

        DispSegment( SEGMENT_1, gu8Digit[ mu8Val_1 ] );
    }
    else
    {
        DispSegment( SEGMENT_10, CHAR_SPACE );
        DispSegment( SEGMENT_1, CHAR_SPACE );
    }
}


void DispSettingClockDone(U8 mu8OnOff)
{
    MenuTime_T mTime;


    GetClockTime( &mTime );
    TurnOffAllIcon();

    HAL_TurnOnOffLED( COLON, mu8OnOff );

    if( mTime.u8AmPm == TIME_AM )
    {
        DispClockItemAm( mu8OnOff );
        DispClockItemPm( OFF );
    }
    else
    {
        DispClockItemAm( OFF );
        DispClockItemPm( mu8OnOff );
    }
    DispClockItemHour( mu8OnOff );
    DispClockItemMin( mu8OnOff );
}

// 시간 설정 화면
void DispSettingClock(U8 mItem, U8 mAmPm, U8 mHour, U8 mMin)
{
    static U8 mu8OnOff = ON;


    mSettingTime.Hour = mHour;
    mSettingTime.Min  = mMin;

    TurnOffAllIcon();
    HAL_TurnOnOffLED( COLON, ON );
    DispClockItemHour( ON );
    DispClockItemMin( ON );
    if( mAmPm == TIME_AM )
    {
        DispClockItemAm( ON );
        DispClockItemPm( OFF );
    }
    else
    {
        DispClockItemAm( OFF );
        DispClockItemPm( ON );
    }

    // UP/DOWN 버튼 조작이 없을 때 1sec 점멸...
    if( IsExpiredDispTimer( DISP_TIMER_UP ) == TRUE 
        && IsExpiredDispTimer( DISP_TIMER_DOWN ) == TRUE )
    {
        // AM/PM
        if( mItem == 0 )
        {
            if( mAmPm == TIME_AM )
            {
                mu8OnOff = BlinkLED( mu8OnOff, DispClockItemAm, DISP_TIMER_ID_500MS ); 
                DispClockItemPm( OFF );
            }
            else
            {
                mu8OnOff = BlinkLED( mu8OnOff, DispClockItemPm, DISP_TIMER_ID_500MS ); 
                DispClockItemAm( OFF );
            }
        }
        else if( mItem == 1 )
        {
            mu8OnOff = BlinkLED( mu8OnOff, DispClockItemHour, DISP_TIMER_ID_500MS ); 
        }
        else if( mItem == 2 )
        {
            mu8OnOff = BlinkLED( mu8OnOff, DispClockItemMin, DISP_TIMER_ID_500MS ); 
        }
    }
}

typedef struct _seg_
{
    U8 Seg_100;
    U8 Seg_10;
    U8 Seg_1;
} InfinityList_T;

const static InfinityList_T EffectInifinity[] = 
{
    {   SEG_OFF,    SEG_OFF,    SEG_OFF },
    {   SEG_E,      SEG_OFF,    SEG_OFF },
    {   SEG_EF,     SEG_OFF,    SEG_OFF },
    {   SEG_EFA,    SEG_OFF,    SEG_OFF },
    {   SEG_EFA,    SEG_A,      SEG_OFF },
    {   SEG_EFA,    SEG_A,      SEG_A   },

    {   SEG_EFA,    SEG_A,      SEG_AB   },
    {   SEG_EFA,    SEG_A,      SEG_ABC  },
    {   SEG_EFA,    SEG_A,      SEG_ABCD },
    {   SEG_EFA,    SEG_AD,     SEG_ABCD },
    {   SEG_EFAD,   SEG_AD,     SEG_ABCD },

    {   SEG_FAD,    SEG_AD,     SEG_ABCD  },
    {   SEG_AD,     SEG_AD,     SEG_ABCD  },
    {   SEG_D,      SEG_AD,     SEG_ABCD  },
    {   SEG_D,      SEG_D,      SEG_ABCD  },
    {   SEG_D,      SEG_D,      SEG_BCD   },
    {   SEG_D,      SEG_D,      SEG_CD    },
    {   SEG_D,      SEG_D,      SEG_D     },

    {   SEG_D,      SEG_D,      SEG_OFF   },
    {   SEG_D,      SEG_OFF,    SEG_OFF   }
};
#define SZ_INFINITY_LIST    (sizeof(EffectInifinity)/sizeof(InfinityList_T))

#define DELAY_INFINITY   2  // 200ms @100ms
U8 gu8InfinityCount = 0U;
U8 gu8InfinityDelay = DELAY_INFINITY;
void ResetInfinityCount(void)
{
    gu8InfinityCount = 0;
    gu8InfinityDelay = DELAY_INFINITY;
}

// 연속 추출 화면 표시
// > 화면 돌아가는 표시
// > mL 표시
void DispWaterOutAmountInfinity(void)
{
    InfinityList_T *pVal;

    pVal = &EffectInifinity[ gu8InfinityCount ];
    DispSegment( SEGMENT_1000, SEG_OFF );
    DispSegment( SEGMENT_100,  pVal->Seg_100 );
    DispSegment( SEGMENT_10,   pVal->Seg_10 );
    DispSegment( SEGMENT_1,    pVal->Seg_1 );


    TurnOffAllIcon();
    HAL_TurnOnOffLED( ICON_ML, OFF );

    if( gu8InfinityDelay != 0 )
    {
        gu8InfinityDelay--;
        return ;
    }

    gu8InfinityDelay = DELAY_INFINITY;
    gu8InfinityCount++;

    if( gu8InfinityCount >= SZ_INFINITY_LIST )
    {
        gu8InfinityCount = 0;
    }
}


void DispHotWaterPreHeating(U8 mu8Step, U8 mu8Temp)
{
    DispSegmentDigit( mu8Temp );
    if( mu8Step == 0 )
    {
        DispSegment( SEGMENT_100,   BAR_BOTTOM  );
        DispSegment( SEGMENT_10,    BAR_BOTTOM  );
        DispSegment( SEGMENT_1,     BAR_BOTTOM  );
    }
    else if( mu8Step == 1 )
    {
        DispSegment( SEGMENT_100,   STACK_BAR_CENTER  );
        DispSegment( SEGMENT_10,    STACK_BAR_CENTER  );
        DispSegment( SEGMENT_1,     STACK_BAR_CENTER  );
    }
    else
    {
        DispSegment( SEGMENT_100,   STACK_BAR_TOP  );
        DispSegment( SEGMENT_10,    STACK_BAR_TOP  );
        DispSegment( SEGMENT_1,     STACK_BAR_TOP  );
    }

    HAL_TurnOnOffLED( ICON_CELSIUS, OFF );
}

// 온도 표시
void DispTemp(U16 mu16Val)
{
    DispSegmentDigit( mu16Val );

    TurnOffAllIcon();
    HAL_TurnOnOffLED( ICON_CELSIUS, ON );
}


// 현재 시간 표시
void DispClock(TimeData_T *pTime)
{
    U8 mu8Am = TRUE;    // AM
    U16 mu16Hour;
    U16 mu16Time;
    

    mu16Hour = pTime->Hour;
    if( mu16Hour >= 12 )
    {
        mu8Am = FALSE;  // PM
        mu16Hour = mu16Hour - 12;
    }

    if( mu16Hour == 0 )
    {
        mu16Hour = 12;
    }

    mu16Time = ( mu16Hour * 100 ) + pTime->Min;
    
    // HOUR:MIN
    DispSegmentDigit( mu16Time );

    // AM/PM
    TurnOffAllIcon();

    HAL_TurnOnOffLED( COLON, ON );
    if( mu8Am == TRUE )
    {
        // AM
        HAL_TurnOnOffLED( ICON_AM, ON );
        HAL_TurnOnOffLED( ICON_PM, OFF );
    }
    else
    {
        // PM
        HAL_TurnOnOffLED( ICON_AM, OFF );
        HAL_TurnOnOffLED( ICON_PM, ON );
    }
}

void DispWeather(U8 mu8Status )
{
    if( mu8Status == WEATHER_SUN )
    {
        HAL_TurnOnOffLED( ICON_SUN,     ON );
        HAL_TurnOnOffLED( ICON_CLOUD,   OFF );
        HAL_TurnOnOffLED( ICON_RAIN,    OFF );
    }
    else if( mu8Status == WEATHER_CLOUD )
    {
        HAL_TurnOnOffLED( ICON_SUN,     OFF );
        HAL_TurnOnOffLED( ICON_CLOUD,   ON );
        HAL_TurnOnOffLED( ICON_RAIN,    OFF );
    }
    else if( mu8Status == WEATHER_RAIN )
    {
        HAL_TurnOnOffLED( ICON_SUN,     OFF );
        HAL_TurnOnOffLED( ICON_CLOUD,   OFF );
        HAL_TurnOnOffLED( ICON_RAIN,    ON );
    }
    else
    {
        // WEATHER 
        HAL_TurnOnOffLED( ICON_SUN,     OFF );
        HAL_TurnOnOffLED( ICON_CLOUD,   OFF );
        HAL_TurnOnOffLED( ICON_RAIN,    OFF );
    }

}

void DispDrainWater(U8 mu8OnOff, U8 mu8Percent)
{
    HAL_TurnOnOffLED( ICON_DRAIN, mu8OnOff );
    HAL_TurnOnOffLED( ICON_PERCENT, mu8OnOff );
    if( mu8OnOff == ON )
    {
        DispSegmentDigit( mu8Percent );
    }
}


void DispRoom(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ROOM_WATER, mu8OnOff );
}

void DispColdDimming(U8 mu8OnOff)
{
    HAL_DimmingLed( COLD_WATER, mu8OnOff );
}

void DispCold(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( COLD_WATER, mu8OnOff );
}


void DispWifi(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ICON_WIFI, mu8OnOff );
}

void DispWifiBle(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ICON_WIFI_BLE, mu8OnOff );
}

void DispFlushing(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ICON_FLUSHING, mu8OnOff );
}

void DispFilterStatus(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ICON_FILTER, mu8OnOff );
}

void DispIceFull(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ICON_ICE_FULL, mu8OnOff );
}

void DispSaving(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ICON_SAVING, mu8OnOff );
}

#if 0
void DispIceLock(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ICE_LOCK, mu8OnOff );
}
#endif

void DispAllLock(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( LOCK_ALL, mu8OnOff );
}

void DispAllLockSel(U8 mu8OnOff)
{
    HAL_DimmingLed( LOCK_ALL,    ON );

    if( mu8OnOff == ON )
    {
        HAL_TurnOnOffLED( LOCK_ALL, ON );
    }
    else
    {
        HAL_TurnOnOffLED( LOCK_ALL, OFF );
    }
}

#if 0
void DispIceMake(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ICE_MAKE, mu8OnOff );
    HAL_TurnOnOffLED( CP_ICE_MAKE, mu8OnOff );
}
#endif


#if 0
void DispColdMake(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( COLD_MAKE, mu8OnOff );
}
#endif



#if 0
U8 GetColdStatus(void)
{
    if( GetColdWaterConfigMake() == FALSE )
    {
        return STATUS_OFF;
    }

    if( GetColdWaterMakeMode() == COLD_MODE_COOLING
            && GetGasSwitchStatus() == GAS_SWITCH_COLD 
            && GetCompOnOff() == ON )
    {
        return STATUS_MAKE;
    }

    return STATUS_STOP;
}

// 제빙 상태 반환
U8 GetIceStatus(void)
{
    if( GetIceConfigMake() == FALSE )
    {
        return STATUS_OFF;
    }

    // 제빙 초기화, 준비 단계는 제빙 중 미표시
    if( IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == FALSE )
    {
        // 트레이 입수 후 부터 표시..
        if( IsSetIceMakeMode( ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
        {
            if( GetIceMakerReadyStep() >= STEP_READY_TRAY_FEED_START )
            {
                // 제빙 중 표시
                return STATUS_MAKE;
            }
        }
        else if( IsSetIceMakeMode( ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
        {
            // 제빙 중 표시
            return STATUS_MAKE;
        }
        else if(  IsSetIceMakeMode( ICE_MODE_DEICING ) == TRUE )
        {
            // 탈빙 중 표시
            return STATUS_DEICING;
        }
    }

    return STATUS_STOP;
}
// 추가 제빙 기능으로 인해
// 만빙 중에도 제빙 동작이 가능함.
// 만빙이라도 제빙 중이라면 LCD에 만빙 표시 하지 않음
U8 GetDispIceFullStatus(void)
{
    if( GetIceFullStatus() == TRUE )
    {
        if( GetIceStatus() == STATUS_MAKE 
            || GetIceStatus() == STATUS_DEICING )
        {
            return FALSE;
        }

        return TRUE;
    }

    return FALSE;
}
#endif


// 메멘토 버튼 표시
void DispMementoKey( mu8Sel )
{
    HAL_DimmingLed( SETTING, ON );
    HAL_DimmingLed( ROOM_WATER, ON );

    HAL_TurnOnOffLED( SETTING, ON );
    HAL_TurnOnOffLED( ROOM_WATER, ON );
    //if( mu8Sel == MMT_SEL_SETTING )
    //{
    //    HAL_TurnOnOffLED( SETTING, ON );
    //}
    //else if( mu8Sel == MMT_SEL_ROOM )
    //{
    //    HAL_TurnOnOffLED( ROOM_WATER, ON );
    //}
}


void DispSettingButton(U8 mu8OnOff)
{
    HAL_DimmingLed( SETTING,   OFF );
    HAL_TurnOnOffLED( SETTING, mu8OnOff );
}

void DispAlarmLockAll(U8 mu8OnOff)
{
    HAL_DimmingLed( LOCK_ALL ,  OFF );
    HAL_TurnOnOffLED( LOCK_ALL, mu8OnOff );
}

void DispAlarmLockHot(U8 mu8OnOff)
{
    DispSettingButton( mu8OnOff );

    HAL_DimmingLed( HOT_LOCK,  OFF );
    HAL_TurnOnOffLED( HOT_LOCK, mu8OnOff );
}

void DispAlarmLockIce(U8 mu8OnOff)
{
    DispSettingButton( mu8OnOff );

    HAL_DimmingLed( ICE_LOCK,  OFF );
    HAL_TurnOnOffLED( ICE_LOCK, mu8OnOff );
}

void DispAlarmMakeIce(U8 mu8OnOff)
{
    DispSettingButton( mu8OnOff );

    HAL_DimmingLed( ICE_MAKE,  OFF );
    HAL_TurnOnOffLED( ICE_MAKE, mu8OnOff );
}

void DispAlarmMakeIceCp(U8 mu8OnOff)
{
    DispSettingButton( mu8OnOff );

    HAL_DimmingLed( CP_ICE_MAKE,  OFF );
    HAL_TurnOnOffLED( CP_ICE_MAKE, mu8OnOff );
}

void DispAlarmMakeCold(U8 mu8OnOff)
{
    DispSettingButton( mu8OnOff );

    HAL_DimmingLed( COLD_MAKE,  OFF );
    HAL_TurnOnOffLED( COLD_MAKE, mu8OnOff );
}

void DispSetting(U8 mu8OnOff )
{
    if( mu8OnOff == OFF )
    {
        // Setting Button
        HAL_DimmingLed( SETTING,    ON );
        HAL_TurnOnOffLED( SETTING, OFF );

        HAL_DimmingLed( ICE_MAKE,  OFF );
        HAL_DimmingLed( CP_ICE_MAKE,  OFF );
        HAL_DimmingLed( ICE_LOCK,  OFF );
        HAL_DimmingLed( HOT_LOCK,  OFF );
        HAL_DimmingLed( LOCK_ALL,  OFF );
        HAL_DimmingLed( COLD_MAKE, OFF );
        HAL_DimmingLed( SAVING,    OFF );
        HAL_DimmingLed( CP_ICE_TURBO,   OFF );
        HAL_DimmingLed( CP_ICE_TURBO_2, OFF );

        HAL_TurnOnOffLED( ICE_MAKE,  OFF );
        HAL_TurnOnOffLED( CP_ICE_MAKE,  OFF );
        HAL_TurnOnOffLED( ICE_LOCK,  OFF );
        HAL_TurnOnOffLED( HOT_LOCK,  OFF );
        HAL_TurnOnOffLED( LOCK_ALL,  OFF );
        HAL_TurnOnOffLED( COLD_MAKE, OFF );
        HAL_TurnOnOffLED( SAVING,    OFF );
        HAL_TurnOnOffLED( CP_ICE_TURBO, OFF );
        HAL_TurnOnOffLED( CP_ICE_TURBO_2, OFF );
    }
    else
    {
        // Setting Button
        HAL_DimmingLed( SETTING,      ON );
        HAL_DimmingLed( ICE_MODE,     GetIceConfigMake() );
        HAL_DimmingLed( HOT_WATER,    ON );
        HAL_DimmingLed( ROOM_WATER,   ON );
        HAL_DimmingLed( COLD_WATER,   GetColdWaterConfigMake() );
        HAL_DimmingLed( AMOUNT,       ON );
        HAL_DimmingLed( ICON_SAVING,  OFF );
        HAL_DimmingLed( CP_ICE_TURBO,   GetIceConfigMake() );
        if( GetSystem() == SYSTEM_CP )
        {
            HAL_DimmingLed( CP_ICE_TURBO_2, GetIceConfigMake() );
        }

        // 설정
        HAL_TurnOnOffLED( SETTING,       ON );
        HAL_TurnOnOffLED( ICE_MODE,      OFF);
        HAL_TurnOnOffLED( ICON_ICE_LOCK, GetLockIce() );
        HAL_TurnOnOffLED( ICON_HOT_LOCK, GetLockHot() );
        HAL_TurnOnOffLED( COLD_WATER,    OFF );
        HAL_TurnOnOffLED( ICON_SAVING,   GetSavingConfig() );
        HAL_TurnOnOffLED( CP_ICE_TURBO,  OFF );
        HAL_TurnOnOffLED( CP_ICE_TURBO_2, OFF );

        // On/off button 
        HAL_TurnOnOffLED( ICE_MAKE,  ON );
        HAL_TurnOnOffLED( CP_ICE_MAKE,  ON );
        HAL_TurnOnOffLED( ICE_LOCK,  ON );
        HAL_TurnOnOffLED( HOT_LOCK,  ON );
        HAL_TurnOnOffLED( LOCK_ALL,  ON );
        HAL_TurnOnOffLED( COLD_MAKE, ON );
        HAL_TurnOnOffLED( SAVING,    ON );
    }
}

