#include "hw.h"
#include "hal_led.h"
#include "hal_input.h"

#include "display_eol.h"
#include "display_lib.h"
#include "display.h"
#include "eol.h"
#include "water_out.h"
#include "service.h"
#include "filter.h"
#include "WIFI_Control.h"
#include "comp_bldc.h"
#include "gas_switch_valve.h"
#include "hal_step_motor.h"


static void DispWifi(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( ICON_WIFI_BLE, mu8OnOff );
    HAL_TurnOnOffLED( ICON_WIFI, mu8OnOff );
}

static void DispKeyHot(U8 mu8OnOff)
{
    // 온수 표시
    HAL_TurnOnOffLED( HOT_COFFE,        mu8OnOff );
    HAL_TurnOnOffLED( HOT_TEA,          mu8OnOff );
    HAL_TurnOnOffLED( HOT_MILK,         mu8OnOff );
    HAL_TurnOnOffLED( HOT_WATER,        mu8OnOff );
    HAL_TurnOnOffLED( ICON_HOT_LOCK,    mu8OnOff );

    HAL_TurnOnOffLED( CP_ICE_TURBO_2,   mu8OnOff );
    HAL_TurnOnOffLED( CP_ICE_TURBO,     mu8OnOff );
}


// EOL - FRONT check button..
#define FRONT_UV_ADC        30
static void EolFrontCheckInput(void)
{
    U32 mu32Val = 0UL;


    mu32Val = GetEolTestInputVal();

    if( GetEolCheckStatus( EOL_CHK_ID_UV_ICE_DOOR ) < FRONT_UV_ADC )
    {
        DispSegmentError( 1 );
    }
#if !CONFIG_JIG_FRONT_UNUSED_UV
    else if( GetEolCheckStatus( EOL_CHK_ID_UV_WATER_OUT ) < FRONT_UV_ADC )
    {
        DispSegmentError( 2 );
    }
#endif

    else
    {
        TurnOnAllLED();

        // FRONT JIG 프로그램에서는 탱크 리드 커버 제외..
        // MK_EOL_FILTER_COVER에 포함시킴..
#if CONFIG_JIG_RBK_FRONT
        if( (mu32Val & MK_EOL_FILTER_COVER) != 0 )
#else
        if( ( mu32Val & MK_EOL_TANK_COVER) != 0 )
#endif
        {
            HAL_TurnOnOffLED( ICON_UV_SMART_CARE_TM, OFF );
            HAL_TurnOnOffLED( ICON_UV_SMART_CARE,    OFF );
            HAL_TurnOnOffLED( ICON_UV_WATER,    OFF );
            HAL_TurnOnOffLED( ICON_UV_ICE,      OFF );
            HAL_TurnOnOffLED( ICON_UV_ICE_TANK, OFF );
            HAL_TurnOnOffLED( ICON_UV_ICE_TRAY, OFF );
        }

        if( (mu32Val & MK_EOL_FILTER_COVER) != 0 )
        {
            // 상단 표시부 (필터교체,필터세척,유로배수)
            HAL_TurnOnOffLED( ICON_FILTER, OFF );
            HAL_TurnOnOffLED( ICON_FLUSHING,  OFF );
            HAL_TurnOnOffLED( ICON_DRAIN,  OFF );
        }


        if( (mu32Val & MK_EOL_KEY_SETTING) != 0 )
        {
            // 7세그먼트 위에 상태 표시 아이콘
            HAL_TurnOnOffLED( SETTING, OFF );
        }

        if( (mu32Val & MK_EOL_KEY_HOT ) != 0 )
        {
            DispKeyHot( OFF );
            //// 온수 표시
            //HAL_TurnOnOffLED( HOT_COFFE,        OFF );
            //HAL_TurnOnOffLED( HOT_TEA,          OFF );
            //HAL_TurnOnOffLED( HOT_MILK,         OFF );
            //HAL_TurnOnOffLED( HOT_WATER,        OFF );
            //HAL_TurnOnOffLED( ICON_HOT_LOCK,    OFF );

            //HAL_TurnOnOffLED( CP_ICE_TURBO_2,   OFF );
            //HAL_TurnOnOffLED( CP_ICE_TURBO,     OFF );
        }
        else
        {
            static U8 mu8OnOff = OFF;
            mu8OnOff = BlinkLED( mu8OnOff, DispKeyHot, DISP_TIMER_ID_500MS );
        }

        if( (mu32Val & MK_EOL_KEY_ROOM ) != 0 )
        {
            // AM/PM
            HAL_TurnOnOffLED( ICON_AM,    OFF );
            HAL_TurnOnOffLED( ICON_PM,    OFF );

            // 7SEGMENT 및 단위 표시 
            DispSegmentChar( SEGMENT_CHAR_ID_SPACE );
            HAL_TurnOnOffLED( COLON, OFF );

            // 정수 버튼
            HAL_TurnOnOffLED( ROOM_WATER,    OFF );
        }

        if( (mu32Val & MK_EOL_KEY_COLD ) != 0 )
        {
            // 날씨 
            HAL_TurnOnOffLED( ICON_SAVING,   OFF );
            DispWeather( OFF );

            HAL_TurnOnOffLED( ICON_ML, OFF );
            HAL_TurnOnOffLED( ICON_PERCENT, OFF );
            HAL_TurnOnOffLED( ICON_CELSIUS, OFF );

            HAL_TurnOnOffLED( COLD_TEMP_WHITE,  OFF );
            HAL_TurnOnOffLED( COLD_TEMP_BLUE,   OFF );
            HAL_TurnOnOffLED( COLD_WATER,       OFF );
//#if CONFIG_JIG_RBK_FRONT
            HAL_TurnOnOffLED( ICON_WIFI_BLE, OFF );
            HAL_TurnOnOffLED( ICON_WIFI, OFF );
//#endif
        }

#if 0
#if !CONFIG_JIG_RBK_FRONT
        if( (mu32Val & MK_EOL_WIFI ) != 0 )
        {
            static U8 mu8OnOff = OFF;
            mu8OnOff = BlinkLED( mu8OnOff, DispWifi, DISP_TIMER_ID_500MS );
        }
        else
        {
            DispWifi( ON );
        }
#endif
#endif

        if( (mu32Val & MK_EOL_KEY_AMOUNT ) != 0 )
        {
            HAL_TurnOnOffLED( AMOUNT_INFINITY,  OFF );
            HAL_TurnOnOffLED( AMOUNT_500,   OFF );
            HAL_TurnOnOffLED( AMOUNT_250,   OFF );
            HAL_TurnOnOffLED( AMOUNT_120,   OFF );
            HAL_TurnOnOffLED( AMOUNT,       OFF );
        }

        if( (mu32Val & MK_EOL_KEY_SAVE ) != 0 )
        {
            HAL_TurnOnOffLED( SAVING,       OFF );
        }

        if( (mu32Val & MK_EOL_KEY_HOT_LOCK ) != 0 )
        {
            HAL_TurnOnOffLED( HOT_LOCK,       OFF );
        }

        if( (mu32Val & MK_EOL_KEY_LOCK_ALL ) != 0 )
        {
            HAL_TurnOnOffLED( LOCK_ALL,       OFF );
        }

        if( (mu32Val & MK_EOL_KEY_COLD_MAKE ) != 0 )
        {
            HAL_TurnOnOffLED( COLD_MAKE,       OFF );
        }

        if( (mu32Val & MK_EOL_KEY_ICE_LOCK ) != 0 )
        {
            HAL_TurnOnOffLED( ICE_LOCK,       OFF );
        }

        if( (mu32Val & MK_EOL_KEY_ICE_MAKE ) != 0 )
        {
            HAL_TurnOnOffLED( ICE_MAKE,       OFF );
            HAL_TurnOnOffLED( CP_ICE_MAKE,       OFF );
        }

        if( (mu32Val & MK_EOL_KEY_ICE ) != 0 )
        {
            HAL_TurnOnOffLED( ICON_ICE_LOCK,    OFF );
            HAL_TurnOnOffLED( ICE_TURBO,        OFF );
            HAL_TurnOnOffLED( ICE_SIZE_LARGE,   OFF );
            HAL_TurnOnOffLED( ICE_SIZE_BAR,     OFF );
            HAL_TurnOnOffLED( ICE_SIZE_SMALL,   OFF );

            HAL_TurnOnOffLED( ICE_MODE,   OFF );
        }
        
        if( (mu32Val & MK_EOL_LEVER_WATER ) != 0 )
        {
            HAL_TurnOnOffLED( WATER_OUT_1, OFF );
            HAL_TurnOnOffLED( WATER_OUT_2, OFF );
            HAL_TurnOnOffLED( WELCOM_WATER, OFF );
        }

        if( (mu32Val & MK_EOL_LEVER_ICE ) != 0 )
        {
            HAL_TurnOnOffLED( ICON_ICE_FULL, OFF );

            HAL_TurnOnOffLED( ICE_OUT_1, OFF );
            HAL_TurnOnOffLED( ICE_OUT_2, OFF );
            HAL_TurnOnOffLED( WELCOM_ICE, OFF );
        }
    }
}


static void DispFilterOnOff(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( AMOUNT,   mu8OnOff );
}

// LOAD TEST, LED 표시
static void EolCheckInput(void)
{
    U32 mu32Val = 0UL;


    mu32Val = GetEolTestInputVal();

    TurnOffAllLED();

    // DIMMING ON
    //HAL_DimmingLed( HOT_WATER,  ON );
    //HAL_DimmingLed( ROOM_WATER, ON );
    //HAL_DimmingLed( COLD_WATER, ON );
    //HAL_DimmingLed( AMOUNT,     ON );



    if( (mu32Val & MK_EOL_KEY_HOT ) != 0 )
    {
        HAL_TurnOnOffLED( HOT_WATER,    ON );
    }

    if( (mu32Val & MK_EOL_KEY_ROOM ) != 0 )
    {
        // 정수 버튼
        HAL_TurnOnOffLED( ROOM_WATER,    ON );
    }

    if( (mu32Val & MK_EOL_KEY_COLD ) != 0 )
    {
        HAL_TurnOnOffLED( COLD_WATER,       ON );
    }

    if( (mu32Val & MK_EOL_KEY_AMOUNT ) != 0 )
    {
        HAL_TurnOnOffLED( AMOUNT,       ON );
    }


    HAL_TurnOnOffLED( WELCOM_WATER, OFF );
    if( (mu32Val & MK_EOL_LEVER_WATER ) != 0 )
    {
        HAL_TurnOnOffLED( WATER_OUT_1, ON );
        HAL_TurnOnOffLED( WATER_OUT_2, ON );
        HAL_TurnOnOffLED( WELCOM_WATER, ON );
    }

    HAL_TurnOnOffLED( WELCOM_ICE, OFF );
    if( (mu32Val & MK_EOL_LEVER_ICE ) != 0 )
    {
        HAL_TurnOnOffLED( ICE_OUT_1, ON );
        HAL_TurnOnOffLED( ICE_OUT_2, ON );
        HAL_TurnOnOffLED( WELCOM_ICE, ON );
    }

    // FILTER ON/OFF 감지되면 물양 버튼 점멸
    if( (mu32Val & MK_EOL_FILTER_ONOFF ) != 0 )
    {
        static U8 mu8OnOff = OFF;
        mu8OnOff = BlinkLED( mu8OnOff, DispFilterOnOff, DISP_TIMER_ID_500MS );
    }
}

static U8 ShowLcdKey(U32 mu32Val, U32 mu32Mk, U8 mId)
{
    if( (mu32Val & mu32Mk ) == mu32Mk )
    {
        ClearEolTestInputBit( mu32Mk );
        return TRUE;
    }

    return FALSE;
}


// FRONT 검사 DISPLAY
// 1. 키 버튼 입력,
// 2. 조도 센서 감지
// 3. 서비스 점검 
static void FrontTest(void)
{
    // Check eol input
    if( HAL_GetInputValue( IN_TANK_OPEN ) == LOW )
    {
        SetEolTestInputBit( MK_EOL_TANK_COVER );
    }

    if( GetFilterStatus( ID_FILTER_COVER ) == FILTER_CLOSE )
    {
        SetEolTestInputBit( MK_EOL_FILTER_COVER );
    }

    //if( GetFilterStatus( ID_FILTER ) == FILTER_CLOSE )
    //{
    //    SetEolTestInputBit( MK_EOL_FILTER );
    //}

    // button 
    EolFrontCheckInput();
}

static U8 CheckFilterOnOff(void)
{
    static U8 mu8Prev = FILTER_CLOSE;
    U8 mu8Current;

    mu8Current = GetFilterStatus( ID_FILTER );
    if( mu8Prev != mu8Current )
    {
        mu8Prev = mu8Current;

        if( mu8Current == FILTER_CLOSE )
        {
            return TRUE;
        }
    }

    return FALSE;
}


static void DispEolSealdComp(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( SETTING,  mu8OnOff );
}

static void DispEolSealdGasSwitch(U8 mu8OnOff)
{
    HAL_TurnOnOffLED( COLD_WATER,  mu8OnOff );
    HAL_TurnOnOffLED( ICE_MODE,    mu8OnOff );
    HAL_TurnOnOffLED( HOT_WATER,   mu8OnOff );

    HAL_TurnOnOffLED( CP_ICE_SIZE,   mu8OnOff ); // CP - ICE_MODE
    HAL_TurnOnOffLED( CP_ICE_TURBO,    mu8OnOff ); // CP - HOT_WATER
    HAL_TurnOnOffLED( CP_ICE_TURBO_2,   mu8OnOff ); // CP - HOT_WATER
}

// EOL - LOAD TEST 모드
static void LoadTest(void)
{
    U8 mu8Mode;
    U8 mu8SelWater = SEL_WATER_ROOM;

    // 만약 버튼 EOL TEST FLAG가 SET되어 있으면,
    // EOL 통신 상태만 표시 하기위해 모든 LED를 다시 OFF한다.
    mu8Mode = GetEolMode();
    if( mu8Mode == EOL_MODE_SEALED )
    {
        // 압축기 점멸
        if( GetCompBldcTargetRps() != 0 )
        {
            static U8 mu8OnOff = OFF;
            mu8OnOff = BlinkLED( mu8OnOff, DispEolSealdComp, DISP_TIMER_ID_500MS );
        }
        else
        {
            DispEolSealdComp( OFF );
        }

        // 냉매 전환 밸브 위치
        if( HAL_GetTargetStep( SM_ID_0 ) != HAL_GetCurrentStep( SM_ID_0 ) )
        {
            // 냉매 전환 밸브 동작 중 점멸...
            static U8 mu8OnOff = OFF;
            mu8OnOff = BlinkLED( mu8OnOff, DispEolSealdGasSwitch, DISP_TIMER_ID_500MS );
        }
        else
        {
            // 냉매 전환 밸브 위치 표시
            if( GetGasSwitchStatus() == GAS_SWITCH_COLD )
            {
                HAL_TurnOnOffLED( COLD_WATER,  ON );
                HAL_TurnOnOffLED( ICE_MODE,    OFF );
                HAL_TurnOnOffLED( HOT_WATER,   OFF );

                HAL_TurnOnOffLED( CP_ICE_SIZE,   OFF ); // CP - ICE_MODE
                HAL_TurnOnOffLED( CP_ICE_TURBO,    OFF ); // CP - HOT_WATER
                HAL_TurnOnOffLED( CP_ICE_TURBO_2,   OFF ); // CP - HOT_WATER
            }
            else if( GetGasSwitchStatus() == GAS_SWITCH_ICE )
            {
                HAL_TurnOnOffLED( COLD_WATER,  OFF );
                HAL_TurnOnOffLED( ICE_MODE,    ON );
                HAL_TurnOnOffLED( HOT_WATER,   OFF );

                HAL_TurnOnOffLED( CP_ICE_SIZE,   ON ); // CP - ICE_MODE
                HAL_TurnOnOffLED( CP_ICE_TURBO,    OFF ); // CP - HOT_WATER
                HAL_TurnOnOffLED( CP_ICE_TURBO_2,   OFF ); // CP - HOT_WATER
            }
            else if( GetGasSwitchStatus() == GAS_SWITCH_HOTGAS )
            {
                HAL_TurnOnOffLED( COLD_WATER,  OFF );
                HAL_TurnOnOffLED( ICE_MODE,    OFF );
                HAL_TurnOnOffLED( HOT_WATER,   ON );

                HAL_TurnOnOffLED( CP_ICE_SIZE,   OFF ); // CP - ICE_MODE
                HAL_TurnOnOffLED( CP_ICE_TURBO,    ON ); // CP - HOT_WATER
                HAL_TurnOnOffLED( CP_ICE_TURBO_2,   ON );   // CP - HOT_WATER
            }
        }
    }
    else if( mu8Mode != EOL_MODE_READY 
            && mu8Mode != EOL_MODE_DONE )
    {
        TurnOffAllLED();

        DispWelcomIce( OFF );
        DispWelcomWater( OFF );
    }
    else
    {
        // 선택된 물 종류 점멸(냉온정 추출 선택)
        mu8SelWater = GetWaterOutSelect();
        if( mu8SelWater == SEL_WATER_ROOM )
        {
            SetEolTestInputBit( MK_EOL_KEY_ROOM );
            ClearEolTestInputBit( MK_EOL_KEY_COLD );
            ClearEolTestInputBit( MK_EOL_KEY_HOT );
        }
        else if( mu8SelWater == SEL_WATER_COLD )
        {
            ClearEolTestInputBit( MK_EOL_KEY_ROOM );
            ClearEolTestInputBit( MK_EOL_KEY_HOT );
            SetEolTestInputBit( MK_EOL_KEY_COLD );
        }
        else 
        {
            ClearEolTestInputBit( MK_EOL_KEY_ROOM );
            ClearEolTestInputBit( MK_EOL_KEY_COLD );
            SetEolTestInputBit( MK_EOL_KEY_HOT );
        }


        // 배수 동작시, LED 점멸
        if( GetEolDrainWater() == TRUE )
        {
            SetEolTestInputBit( MK_EOL_KEY_AMOUNT );
        }
        else
        {
            ClearEolTestInputBit( MK_EOL_KEY_AMOUNT );
        }

        SetEolTestInputBit( MK_EOL_LEVER_WATER );
        SetEolTestInputBit( MK_EOL_LEVER_ICE );

        if( CheckFilterOnOff() == TRUE )
        {
            SetEolTestInputBit( MK_EOL_FILTER_ONOFF );
            SetEolCheckStatus( EOL_CHK_ID_FILTER_ONOFF, 1 ); /* 감지 */
        }

        EolCheckInput();
    }

    // WIFI Connection 정보
    if( GetWifiStatusValue(WIFI_STATUS_TEST) == 0 )
    {
        // Disconncted
        DispWifi( OFF );
    }
    else
    {
        // Connected
        DispWifi( ON );
    }
}

void ProcessDisplayEol(void)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        // FRONT 검사 모드
        FrontTest();
    }
    else
    {
        // LOAD 검사 모드
        LoadTest();
    }
}
