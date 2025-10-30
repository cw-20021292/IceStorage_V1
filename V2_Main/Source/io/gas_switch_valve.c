#include "hw.h"
#include "hal_step_motor.h"

#include "gas_switch_valve.h"
#include "comp.h"


enum
{
    INIT_POWER_ON,
    INIT_WAIT_POWER_ON,

    INIT_MOVE_ZERO,
    INIT_WAIT_ZERO,

    INIT_MOVE_COLD,
    INIT_WAIT_COLD,

    INIT_DONE
};

enum
{
    COLD_POWER_ON,
    COLD_WAIT_POWER_ON,

    COLD_CHECK_POSITION,

    COLD_MOVE,
    COLD_WAIT,

    COLD_DONE
};


enum
{
    ICE_POWER_ON,
    ICE_WAIT_POWER_ON,

    ICE_CHECK_POSITION,

    ICE_MOVE,
    ICE_WAIT,

    ICE_DONE
};


enum
{
    HOTGAS_POWER_ON,
    HOTGAS_WAIT_POWER_ON,

    HOTGAS_CHECK_POSITION,

    HOTGAS_MOVE,
    HOTGAS_WAIT,

    HOTGAS_DONE
};

#define  GAS_SWITCH_ID       SM_ID_0
#define  GAS_SWITCH_SPEED    SM_SPEED_1

// DPP 
#define  GAS_SWITCH_COLD_STEP_VAL    (24)
#define  GAS_SWITCH_ICE_STEP_VAL     (64)
#define  GAS_SWITCH_HOTGAS_STEP_VAL  (104)
#define  GAS_SWITCH_END_STEP_VAL     (200)
#define  GAS_SWITCH_INIT_STEP_VAL    (-15)

#define POWER_ON_TIME       10U     // 1sec @100ms
#define COMP_OFF_INIT_TIME  60U
GasSwitchValve_T  GasSwitch;

void InitGasSwitch(void)
{
    GasSwitch.Status    = GAS_SWITCH_ICE;

    GasSwitch.Mode      = 0; 
    GasSwitch.InitStep  = 0;
    GasSwitch.ColdStep  = 0;
    GasSwitch.IceStep   = 0;
    GasSwitch.PowerOnTime = POWER_ON_TIME; // @100ms, 1sec


    HAL_InitStepVal( GAS_SWITCH_ID );
    HAL_SetCurrentStep( GAS_SWITCH_ID, GAS_SWITCH_END_STEP_VAL );
    HAL_SetTargetStep( GAS_SWITCH_ID, GAS_SWITCH_END_STEP_VAL );
    HAL_SetStepSpeed( GAS_SWITCH_ID, GAS_SWITCH_SPEED );
}

void GetGasSwitchData(GasSwitchValve_T *pData)
{
    MEMCPY( (void __far *)pData, (const void __far *)&GasSwitch, sizeof( GasSwitch ));
}


static void SetMode(U8 mu8Mode );
static void ClearMode(U8 mu8Mode );
U8 GetGasSwitchStatus(void)
{
    return GasSwitch.Status;
}

static void SetGasSwitchStatus(U8 mu8Status)
{
    GasSwitch.Status = mu8Status;
}

U8 GetGasSwitchMode(void)
{
    return GasSwitch.Mode;
}

void GasSwitchInit(void)
{
    SetMode( GAS_SWITCH_MODE_INIT );
}
void GasSwitchCold(void)
{
    SetMode( GAS_SWITCH_MODE_COLD );
}

void GasSwitchIce(void)
{
    SetMode( GAS_SWITCH_MODE_ICE );
}

void GasSwitchHotGas(void)
{
    SetMode( GAS_SWITCH_MODE_HOTGAS );
}


static void SetMode(U8 mu8Mode )
{
    // Set Mode
    GasSwitch.Mode |= mu8Mode;

    if( mu8Mode == GAS_SWITCH_MODE_COLD )
    {
        ClearMode( GAS_SWITCH_MODE_ICE );
        ClearMode( GAS_SWITCH_MODE_HOTGAS );
    }
    else if( mu8Mode == GAS_SWITCH_MODE_ICE )
    {
        ClearMode( GAS_SWITCH_MODE_COLD );
        ClearMode( GAS_SWITCH_MODE_HOTGAS );
    }
    else if( mu8Mode == GAS_SWITCH_MODE_HOTGAS )
    {
        ClearMode( GAS_SWITCH_MODE_ICE );
        ClearMode( GAS_SWITCH_MODE_COLD );
    }

    // Set Init Mode by case
}

static void ClearMode(U8 mu8Mode )
{
    GasSwitch.Mode &= ~mu8Mode;

    //if( mu8Mode == GAS_SWITCH_MODE_POWER_INIT )
    //{
    //    GasSwitch.InitStep = 0;
    //}
    //else 
    if( mu8Mode == GAS_SWITCH_MODE_INIT )
    {
        GasSwitch.InitStep = 0;
    }
    else if( mu8Mode == GAS_SWITCH_MODE_COLD )
    {
        GasSwitch.ColdStep = 0;
    }
    else if( mu8Mode == GAS_SWITCH_MODE_ICE )
    {
        GasSwitch.IceStep = 0;
    }
    else if( mu8Mode == GAS_SWITCH_MODE_HOTGAS )
    {
        GasSwitch.HotGasStep = 0;
    }
}

static void ClearMode(U8 mu8Mode );
static U8 InitSwitch(void);
static U8 ColdSwitch(void);
static U8 IceSwitch(void);
static U8 HotGasSwitch(void);
static U8 ColdSwitchWithInit(void);
void ControlGasSwitch(void)
{
    U8 mu8Ret = FALSE;

    /* CONTROL SMPS 12V */

#if 0
    /* Power Init */
    // 원점을 잡기 위해 초기화 한 후, 초기 위치 설정을 냉수측으로 정의한다.
    if( (GasSwitch.Mode & GAS_SWITCH_MODE_POWER_INIT ) == GAS_SWITCH_MODE_POWER_INIT )
    {
        mu8Ret = InitSwitch();
        if( mu8Ret == TRUE )
        {
            ClearMode( GAS_SWITCH_MODE_POWER_INIT );
            SetGasSwitchStatus( GAS_SWITCH_COLD );
        }
        return ;
    }
#endif


    /* Init */
    // 초기화를 하고 실제 스텝 위치는 냉수측으로 설정한다.
    if( (GasSwitch.Mode & GAS_SWITCH_MODE_INIT ) == GAS_SWITCH_MODE_INIT )
    {
        mu8Ret = InitSwitch();
        if( mu8Ret == TRUE )
        {
            ClearMode( GAS_SWITCH_MODE_INIT );
            SetGasSwitchStatus( GAS_SWITCH_COLD );
        }
        return ;
    }


    /* Cold */
    if( (GasSwitch.Mode & GAS_SWITCH_MODE_COLD ) == GAS_SWITCH_MODE_COLD )
    {
        mu8Ret = ColdSwitch();
        if( mu8Ret == TRUE )
        {
            ClearMode( GAS_SWITCH_MODE_COLD );
            SetGasSwitchStatus( GAS_SWITCH_COLD );
        }
        return ;
    }

    /* Ice */
    if( (GasSwitch.Mode & GAS_SWITCH_MODE_ICE ) == GAS_SWITCH_MODE_ICE )
    {
        mu8Ret = IceSwitch();
        if( mu8Ret == TRUE )
        {
            ClearMode( GAS_SWITCH_MODE_ICE );
            SetGasSwitchStatus( GAS_SWITCH_ICE );
        }
        return ;
    }

    /* HOT-GAS */
    if( (GasSwitch.Mode & GAS_SWITCH_MODE_HOTGAS ) == GAS_SWITCH_MODE_HOTGAS )
    {
        mu8Ret = HotGasSwitch();
        if( mu8Ret == TRUE )
        {
            ClearMode( GAS_SWITCH_MODE_HOTGAS );
            SetGasSwitchStatus( GAS_SWITCH_HOTGAS );
        }
        return ;
    }
}


static U8 InitSwitch(void)
{
    switch( GasSwitch.InitStep )
    {
        case INIT_POWER_ON:
            GasSwitch.PowerOnTime = POWER_ON_TIME;
            GasSwitch.InitStep++;
            break;

        case INIT_WAIT_POWER_ON:
            if( GasSwitch.PowerOnTime != 0 )
            {
                GasSwitch.PowerOnTime--;
            }
            else
            {
                GasSwitch.InitStep++;
            }
            break;

        case INIT_MOVE_ZERO:
            HAL_SetHoldStep( GAS_SWITCH_ID, 0 );
            HAL_SetTargetStep( GAS_SWITCH_ID, GAS_SWITCH_INIT_STEP_VAL );
            GasSwitch.InitStep++;
            break;

        case INIT_WAIT_ZERO:
            if( HAL_IsDoneMoveStep( GAS_SWITCH_ID ) == TRUE )
            {
                GasSwitch.InitStep++;

                HAL_InitStepVal( GAS_SWITCH_ID );
            }
            break;

        case INIT_MOVE_COLD:
            HAL_SetHoldStep( GAS_SWITCH_ID, 0 );
            HAL_SetTargetStep( GAS_SWITCH_ID, GAS_SWITCH_COLD_STEP_VAL );
            GasSwitch.InitStep++;
            break;

        case INIT_WAIT_COLD:
            if( HAL_IsDoneMoveStep( GAS_SWITCH_ID ) == TRUE )
            {
                GasSwitch.InitStep++;
            }
            break;

        case INIT_DONE :
            return TRUE;

        default :
            GasSwitch.InitStep = INIT_POWER_ON;
            break;
    }

    return FALSE;
}

static U8 ColdSwitch(void)
{
    switch( GasSwitch.ColdStep )
    {
        // POWER ON 
        case COLD_POWER_ON:
            GasSwitch.PowerOnTime = POWER_ON_TIME;
            GasSwitch.ColdStep++;
            break;

        case COLD_WAIT_POWER_ON:
            if( GasSwitch.PowerOnTime != 0 )
            {
                GasSwitch.PowerOnTime--;
            }
            else
            {
                GasSwitch.ColdStep++;
            }
            break;


            // MOVE
        case COLD_CHECK_POSITION:
            if( HAL_GetTargetStep( GAS_SWITCH_ID ) == GAS_SWITCH_COLD_STEP_VAL )
            {
                GasSwitch.ColdStep = COLD_WAIT;
            }
            else
            {
                GasSwitch.ColdStep++;
            }
            break;

        case COLD_MOVE:
            HAL_SetHoldStep( GAS_SWITCH_ID, 0 );
            HAL_SetTargetStep( GAS_SWITCH_ID, GAS_SWITCH_COLD_STEP_VAL );
            GasSwitch.ColdStep++;
            break;

        case COLD_WAIT:
            if( HAL_IsDoneMoveStep( GAS_SWITCH_ID ) == TRUE )
            {
                GasSwitch.ColdStep++;
            }
            break;

        case COLD_DONE:
            return TRUE;

        default :
            GasSwitch.ColdStep = COLD_POWER_ON;
            break;
    }
    return FALSE;
}


static U8 IceSwitch(void)
{
    switch( GasSwitch.IceStep )
    {
        case ICE_POWER_ON:
            GasSwitch.PowerOnTime = POWER_ON_TIME;
            GasSwitch.IceStep++;
            break;

        case ICE_WAIT_POWER_ON:
            if( GasSwitch.PowerOnTime != 0 )
            {
                GasSwitch.PowerOnTime--;
            }
            else
            {
                GasSwitch.IceStep++;
            }
            break;

        // 현재 위치이면 이동 없이 정상 종료
        case ICE_CHECK_POSITION:
            if( HAL_GetTargetStep( GAS_SWITCH_ID ) == GAS_SWITCH_ICE_STEP_VAL )
            {
                GasSwitch.IceStep = ICE_WAIT;
            }
            else
            {
                GasSwitch.IceStep++;
            }
            break;

        case ICE_MOVE:
            HAL_SetHoldStep( GAS_SWITCH_ID, 0 );
            HAL_SetTargetStep( GAS_SWITCH_ID, GAS_SWITCH_ICE_STEP_VAL );
            GasSwitch.IceStep++;
            break;

        case ICE_WAIT:
            if( HAL_IsDoneMoveStep( GAS_SWITCH_ID ) == TRUE )
            {
                GasSwitch.IceStep++;
            }
            break;

        case ICE_DONE:
            return TRUE;

        default :
            GasSwitch.IceStep = ICE_POWER_ON;
            break;

    }

    return FALSE;
}

static U8 HotGasSwitch(void)
{
    switch( GasSwitch.HotGasStep )
    {
        case HOTGAS_POWER_ON:
            GasSwitch.PowerOnTime = POWER_ON_TIME;
            GasSwitch.HotGasStep++;
            break;

        case HOTGAS_WAIT_POWER_ON:
            if( GasSwitch.PowerOnTime != 0 )
            {
                GasSwitch.PowerOnTime--;
            }
            else
            {
                GasSwitch.HotGasStep++;
            }
            break;

        // 현재 위치이면 이동 없이 정상 종료
        case HOTGAS_CHECK_POSITION:
            if( HAL_GetTargetStep( GAS_SWITCH_ID ) == GAS_SWITCH_HOTGAS_STEP_VAL )
            {
                GasSwitch.HotGasStep = ICE_WAIT;
            }
            else
            {
                GasSwitch.HotGasStep++;
            }
            break;

        case HOTGAS_MOVE:
            HAL_SetHoldStep( GAS_SWITCH_ID, 0 );
            HAL_SetTargetStep( GAS_SWITCH_ID, GAS_SWITCH_HOTGAS_STEP_VAL );
            GasSwitch.HotGasStep++;
            break;

        case HOTGAS_WAIT:
            if( HAL_IsDoneMoveStep( GAS_SWITCH_ID ) == TRUE )
            {
                GasSwitch.HotGasStep++;
            }
            break;

        case HOTGAS_DONE:
            return TRUE;

        default :
            GasSwitch.HotGasStep = HOTGAS_POWER_ON;
            break;

    }

    return FALSE;
}

static U8 ColdSwitchWithInit(void)
{
    switch( GasSwitch.ColdStep )
    {
        // 현재 위치이면 이동 없이 정상 종료
        case 0:
            if( HAL_GetTargetStep( GAS_SWITCH_ID ) == GAS_SWITCH_COLD_STEP_VAL )
            {
                GasSwitch.ColdStep = 4;
            }
            else
            {
                GasSwitch.ColdStep++;
            }
            break;
        // 원점으로 이동..
        case 1:
            HAL_SetHoldStep( GAS_SWITCH_ID, 0 );
            HAL_SetTargetStep( GAS_SWITCH_ID, -10 );
            GasSwitch.ColdStep++;
            break;

            // 원점 이동하고 나서는
            // 변수도 원점 기준으로 초기화 한다.
        case 2:
            if( HAL_IsDoneMoveStep( GAS_SWITCH_ID ) == TRUE )
            {
                HAL_InitStepVal( GAS_SWITCH_ID );
                GasSwitch.ColdStep++;
            }
            break;

            // 이제 냉수 위치로 이동...
        case 3:
            HAL_SetHoldStep( GAS_SWITCH_ID, 0 );
            HAL_SetTargetStep( GAS_SWITCH_ID, GAS_SWITCH_COLD_STEP_VAL );
            GasSwitch.ColdStep++;
            break;

        case 4:
            if( HAL_IsDoneMoveStep( GAS_SWITCH_ID ) == TRUE )
            {
                GasSwitch.ColdStep++;
            }
            break;

        case 5:
            return TRUE;

    }

    return FALSE;
}



void ProcessGasSwitch(void)
{
    static U8 mu8PrevCmd = OFF;
    static U8 mu8RunInit = TRUE;
    U8 mu8CurCmd = OFF;


    mu8CurCmd = GetCompOnOff();
    if( mu8PrevCmd != mu8CurCmd )
    {
        mu8PrevCmd = mu8CurCmd;
        if( mu8CurCmd == OFF )
        {
            mu8RunInit = TRUE;  // 원점 초기화 설정 
        }
    }


    // 압축기 OFF 후, 4분 후에 원점 초기화 동작
    if( mu8RunInit == TRUE )
    {
        if( IsExpiredCompProtectTime() == FALSE )
        {
            // 4분 후..
            if( GetCompProtectOffTime() < COMP_OFF_INIT_TIME )
            {
                mu8RunInit = FALSE;
                GasSwitchInit();
            }
        }
    }


#if CONFIG_TEST_GAS_SWITCH
    TEST_RepeatGasValve();
#endif
}


/*************************************************************************************************************************
 * TEST
 * 냉매 전환 밸브 위치 반복 테스트 프로그램
 */
#define TEST_DELAY  0
GasSwitchValveTest_T  GasTest;

void TEST_GetGasValveData(GasSwitchValveTest_T *pData)
{
    MEMCPY( (void __far *)pData, (const void __far *)&GasTest, sizeof( GasTest ));
}

void TEST_StartGasValve(void)
{
    GasTest.Run         = TRUE;
    GasTest.Delay       = 0;
    GasTest.RepeatCount = 0;
}

void TEST_StopGasValve(void)
{
    GasTest.Run         = FALSE;
}

void TEST_RepeatGasValve(void)
{
    static U8 mu8Toggle = 0;


    if( GasTest.Run == FALSE )
    {
        return ;
    }

    if( mu8Toggle != 0 )
    {
        if( GasSwitch.Mode == 0 )
        {
            mu8Toggle = 0;

            GasTest.RepeatCount++;
            
            if( GasTest.Delay != 0 )
            {
                GasTest.Delay--;
                return ;
            }

            GasSwitchCold();
        }
        else
        {
            GasTest.Delay = TEST_DELAY;
        }
    }
    else
    {
        if( GasSwitch.Mode == 0 )
        {
            mu8Toggle = 1;

            if( GasTest.Delay != 0 )
            {
                GasTest.Delay--;
                return ;
            }
            GasSwitchIce();
        }
        else
        {
            GasTest.Delay = TEST_DELAY;
        }
    }
}

