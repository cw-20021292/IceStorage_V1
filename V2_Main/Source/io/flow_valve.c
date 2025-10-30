#include "flow_valve.h"

// 유량 밸브 초기화 방법
// CLOSE TYPE은 완전히 닫은 다음에 기본 위치로 이동
// OPEN TYPE은 완전히 연다음에 기본 위치로 이동
#define CLOSE_TYPE      0
#define OPEN_TYPE       1
#define FLOW_INIT_TYPE  OPEN_TYPE       // 0: CLOSE TYPE 1: OPEN TYPE


#if (FLOW_INIT_TYPE == CLOSE_TYPE)
// INIT - CLOSE TYPE
enum
{
    INIT_MOVE_ZERO,
    INIT_WAIT_ZERO,

    INIT_MOVE_FULL_OPEN,
    INIT_WAIT_FULL_OPEN,

    INIT_DONE
};
#else
// INIT - OPEN TYPE
enum
{
    INIT_MOVE_FULL_OPEN,
    INIT_WAIT_FULL_OPEN,

    INIT_MOVE_TARGET,
    INIT_WAIT_TARGET,

    INIT_DONE
};
#endif

// OPEN 
enum
{
    OPEN_CHECK_POSITION,

    OPEN_MOVE,
    OPEN_WAIT,

    OPEN_DONE
};

// TARGET
enum
{
    TARGET_MOVE,
    TARGET_WAIT,

    TARGET_DONE
};

// OFFSET
enum
{
    OFFSET_MOVE_TARGET,
    OFFSET_WAIT_TARGET,

    OFFSET_DONE
};

// 온수 유량 조절 밸브
// DC 24V
// 11.25 / STEPPER
// 400PPS 
// 여자 방식 : 2
// FULL OPEN : 1400 setp

// 3000step 인경우, full open 시간 4초
// full close 하고 원점 초기화 이후 full open 하면 약 10초 

#define  FLOW_VALVE_ID       SM_ID_2
#define  FLOW_VALVE_SPEED    SM_SPEED_0       // 400pps 


#define  FLOW_VALVE_OPEN_STEP_VAL    (MAX_FLOW_VALVE_TARGET)   
#define  FLOW_VALVE_CLOSE_STEP_VAL   (-1800)


/* MODE */
#define  FLOW_VALVE_MODE_NONE       0x00
#define  FLOW_VALVE_MODE_INIT       0x01        // 초기화
#define  FLOW_VALVE_MODE_OPEN       0x02        // FULL OPEN
#define  FLOW_VALVE_MODE_TARGET     0x04        // 절대 위치 이동
#define  FLOW_VALVE_MODE_OFFSET     0x08        // 현재 위치에서 +- offset
#define  FLOW_VALVE_MODE_TEST       0x10


FlowValve_T  FlowValve;


static void SetMode(U8 mu8Mode );
static void ClearMode(U8 mu8Mode );

void InitFlowValve(void)
{
    FlowValve.Status        = FLOW_VALVE_STOP;

    FlowValve.Mode          = FLOW_VALVE_MODE_INIT;
    FlowValve.InitStep      = 0;
    FlowValve.OpenStep      = 0;
    FlowValve.TargetStep    = 0;
    FlowValve.OffsetStep    = 0;
    FlowValve.TestStep      = 0;

    FlowValve.Offset        = 0;      // offset 값...
    FlowValve.Target        = 0;      // 현재 밸브 위치

    HAL_InitStepVal( FLOW_VALVE_ID );

#if (FLOW_INIT_TYPE == CLOSE_TYPE)
    HAL_SetTargetStep( FLOW_VALVE_ID, 1800 );    // for power on init
    HAL_SetCurrentStep( FLOW_VALVE_ID, 1800 );    // for power on init
#else
    HAL_SetTargetStep( FLOW_VALVE_ID, 0 );    // for power on init
    HAL_SetCurrentStep( FLOW_VALVE_ID, 0 );    // for power on init
#endif
    HAL_SetStepSpeed( FLOW_VALVE_ID, FLOW_VALVE_SPEED );
}

void  GetFlowValveData(FlowValve_T *pData)
{
    MEMCPY( (void __far *)pData, (const void __far *)&FlowValve, sizeof( FlowValve_T ));
}

U8 GetFlowValveStatus(void)
{
    return FlowValve.Status;
}


void FlowValveInit(void)
{
    SetMode( FLOW_VALVE_MODE_INIT );
}


void FlowValveFullOpen(void)
{
    SetMode( FLOW_VALVE_MODE_OPEN );
}


void FlowValveTarget(I16 target)
{
    FlowValve.Target = (sm_step_t)target;
    SetMode( FLOW_VALVE_MODE_TARGET );
}

I16 GetFlowValveTarget(void)
{
    return FlowValve.Target;
}


void FlowValveOffset(I16 offset)
{
    FlowValve.Offset = (sm_step_t)offset;
    SetMode( FLOW_VALVE_MODE_OFFSET );
}


static void SetMode(U8 mu8Mode )
{
    FlowValve.Mode |= mu8Mode;
    if( mu8Mode == FLOW_VALVE_MODE_INIT )
    {
        ClearMode( FLOW_VALVE_MODE_OFFSET );
        ClearMode( FLOW_VALVE_MODE_OPEN );
        ClearMode( FLOW_VALVE_MODE_TARGET );
    }
    else if( mu8Mode == FLOW_VALVE_MODE_OPEN )
    {
        ClearMode( FLOW_VALVE_MODE_TARGET );
        ClearMode( FLOW_VALVE_MODE_OFFSET );
    }
    else if( mu8Mode == FLOW_VALVE_MODE_TEST )
    {
        ClearMode( FLOW_VALVE_MODE_INIT );
        ClearMode( FLOW_VALVE_MODE_OPEN );
        ClearMode( FLOW_VALVE_MODE_TARGET );
        ClearMode( FLOW_VALVE_MODE_OFFSET );
    }
}

static void ClearMode(U8 mu8Mode )
{
    FlowValve.Mode &= ~mu8Mode;

    if( mu8Mode == FLOW_VALVE_MODE_INIT )
    {
        FlowValve.InitStep = 0;
    }
    else if( mu8Mode == FLOW_VALVE_MODE_OPEN )
    {
        FlowValve.OpenStep = 0;
    }
    else if( mu8Mode == FLOW_VALVE_MODE_OFFSET )
    {
        FlowValve.OffsetStep = 0;
    }
    else if( mu8Mode == FLOW_VALVE_MODE_TARGET )
    {
        FlowValve.TargetStep = 0;
    }
    else if( mu8Mode == FLOW_VALVE_MODE_TEST )
    {
        FlowValve.TestStep = 0;
    }
}

static void SetFlowValveStatus(U8 mu8Status)
{
    FlowValve.Status = mu8Status;
}


static U8 DoInit(void);
static U8 DoOpen(void);
static U8 DoTarget(void);
static U8 DoOffset(void);

//static U8 TestValve(void);

typedef U8 (*pValve_T)(void);
typedef struct _flow_valve_list_
{
    U8 mId;
    pValve_T    pValve;
} FlowValveEvent_T;

const static FlowValveEvent_T FlowValveEventList[] = 
{
    { FLOW_VALVE_MODE_INIT,       DoInit    },
    { FLOW_VALVE_MODE_OPEN,       DoOpen    },
    { FLOW_VALVE_MODE_TARGET,     DoTarget  },
    { FLOW_VALVE_MODE_OFFSET,     DoOffset  }
};
#define  SZ_FLOW_VALVE_LIST      (sizeof(FlowValveEventList)/sizeof(FlowValveEvent_T))

void ControlFlowValve(void)
{
    U8 i;
    U8 mu8Ret;
    U8 mId;
    pValve_T    pFun;


    for( i = 0; i < SZ_FLOW_VALVE_LIST ; i++ )
    {
        mId     = FlowValveEventList[ i ].mId;
        pFun    = FlowValveEventList[ i ].pValve;
        if( (FlowValve.Mode & mId ) == mId )
        {
            if( pFun != NULL )
            {
                mu8Ret = pFun();
                if( mu8Ret == TRUE )
                {
                    ClearMode( mId );
                    SetFlowValveStatus( FLOW_VALVE_STOP );
                }
                else
                {
                    SetFlowValveStatus( FLOW_VALVE_MOVING );
                }

                return ;
            }
        }
    }

}


static void CheckMinMax(void)
{
    // 완전히 닫히면, 원점 초기화
    if( FlowValve.Target <= MIN_FLOW_VALVE_TARGET )
    {
        FlowValve.Target = MIN_FLOW_VALVE_TARGET;
    }
    else if( FlowValve.Target >= MAX_FLOW_VALVE_TARGET )
    {
        FlowValve.Target = MAX_FLOW_VALVE_TARGET;
    }
}

// 초기화...
// Power on wait time.. about 1sec
// FULL CLOSE and then FULL OPEN 
#if (FLOW_INIT_TYPE == CLOSE_TYPE)
static U8 DoInit(void)
{
    switch( FlowValve.InitStep )
    {
        case INIT_MOVE_ZERO:
            HAL_SetHoldStep( FLOW_VALVE_ID, 0 );
            HAL_SetTargetStep( FLOW_VALVE_ID, -20 );
            FlowValve.InitStep++;
            break;

        case INIT_WAIT_ZERO:
            if( HAL_IsDoneMoveStep( FLOW_VALVE_ID ) == TRUE )
            {
                FlowValve.InitStep++;

                // 이동 완료 후 원점 초기화
                HAL_InitStepVal( FLOW_VALVE_ID );
                FlowValve.Target    = 0;
            }
            break;

        case INIT_MOVE_FULL_OPEN:
            HAL_SetHoldStep( FLOW_VALVE_ID, 0 );
            HAL_SetTargetStep( FLOW_VALVE_ID, FLOW_VALVE_OPEN_STEP_VAL );
            FlowValve.InitStep++;
            break;

        case INIT_WAIT_FULL_OPEN:
            if( HAL_IsDoneMoveStep( FLOW_VALVE_ID ) == TRUE )
            {
                FlowValve.Target    = FLOW_VALVE_OPEN_STEP_VAL;
                FlowValve.InitStep++;
            }
            break;

        case INIT_DONE :
            return TRUE;

        default :
            FlowValve.InitStep = INIT_MOVE_ZERO;
            break;
    }

    return FALSE;
}
#else
static U8 DoInit(void)
{
    switch( FlowValve.InitStep )
    {
        case INIT_MOVE_FULL_OPEN:
            HAL_InitStepVal( FLOW_VALVE_ID );
            HAL_SetHoldStep( FLOW_VALVE_ID, 0 );
            HAL_SetTargetStep( FLOW_VALVE_ID, 2000 );
            FlowValve.InitStep++;
            break;

        case INIT_WAIT_FULL_OPEN:
            if( HAL_IsDoneMoveStep( FLOW_VALVE_ID ) == TRUE )
            {
                FlowValve.InitStep++;
                FlowValve.Target    = 2000;
            }
            break;

        case INIT_MOVE_TARGET:
            HAL_SetHoldStep( FLOW_VALVE_ID, 0 );
            HAL_SetTargetStep( FLOW_VALVE_ID, FLOW_VALVE_OPEN_STEP_VAL );
            FlowValve.InitStep++;
            break;

        case INIT_WAIT_TARGET:
            if( HAL_IsDoneMoveStep( FLOW_VALVE_ID ) == TRUE )
            {
                FlowValve.Target    = FLOW_VALVE_OPEN_STEP_VAL;
                FlowValve.InitStep++;
            }
            break;

        case INIT_DONE :
            return TRUE;

        default :
            FlowValve.InitStep = INIT_MOVE_FULL_OPEN;
            break;
    }

    return FALSE;
}
#endif


// move to fulll open 
static U8 DoOpen(void)
{
    switch( FlowValve.OpenStep )
    {
        case OPEN_CHECK_POSITION:
            if( HAL_GetTargetStep( FLOW_VALVE_ID ) == FLOW_VALVE_OPEN_STEP_VAL )
            {
                FlowValve.OpenStep = OPEN_WAIT;
            }
            else
            {
                FlowValve.OpenStep++;
            }
            break;

        case OPEN_MOVE:
            HAL_SetHoldStep( FLOW_VALVE_ID, 0 );
            HAL_SetTargetStep( FLOW_VALVE_ID, FLOW_VALVE_OPEN_STEP_VAL );
            FlowValve.OpenStep++;
            break;

        case OPEN_WAIT:
            if( HAL_IsDoneMoveStep( FLOW_VALVE_ID ) == TRUE )
            {
                FlowValve.Target = FLOW_VALVE_OPEN_STEP_VAL;
                FlowValve.OpenStep++;
            }
            break;

        case OPEN_DONE:
            return TRUE;

        default :
            FlowValve.OpenStep = OPEN_CHECK_POSITION;
            break;
    }

    return FALSE;

}


static U8 DoOffset(void)
{
    sm_step_t          mTarget = 0;

    switch( FlowValve.OffsetStep )
    {
        case OFFSET_MOVE_TARGET:
            mTarget             = HAL_GetTargetStep( FLOW_VALVE_ID );
            FlowValve.Target    = mTarget + FlowValve.Offset;
            FlowValve.Offset    = 0;

            CheckMinMax();
            HAL_SetHoldStep( FLOW_VALVE_ID, 0 );
            HAL_SetTargetStep( FLOW_VALVE_ID, FlowValve.Target );

            FlowValve.OffsetStep++;
            break;

        case OFFSET_WAIT_TARGET:
            if( HAL_IsDoneMoveStep( FLOW_VALVE_ID ) == TRUE )
            {
                FlowValve.OffsetStep++;
            }
            break;

        case OFFSET_DONE:
            return TRUE;

        default :
            FlowValve.OffsetStep = OFFSET_MOVE_TARGET;
            break;
    }

    return FALSE;
}

static U8 DoTarget(void)
{
    switch( FlowValve.TargetStep )
    {
        case TARGET_MOVE:

            CheckMinMax();
            HAL_SetHoldStep( FLOW_VALVE_ID, 0 );
            HAL_SetTargetStep( FLOW_VALVE_ID, FlowValve.Target );

            FlowValve.TargetStep++;
            break;

        case TARGET_WAIT:
            if( HAL_IsDoneMoveStep( FLOW_VALVE_ID ) == TRUE )
            {
                FlowValve.TargetStep++;
            }
            break;

        case TARGET_DONE:
            return TRUE;

        default :
            FlowValve.TargetStep = TARGET_MOVE;
            break;
    }

    return FALSE;
}


/*************************************************************************************************************************
 * TEST
 */
typedef struct _ice_door_repeat_test_
{
    U8 Run;
    U8 Delay;
    U32 RepeatCount;
} FlowValveTest_T;

FlowValveTest_T FlowTest;
void TEST_StartFlowValve(void)
{
    FlowTest.Run         = TRUE;
    FlowTest.Delay       = 0;
    FlowTest.RepeatCount = 0;
}

void TEST_StopFlowValve(void)
{
    FlowTest.Run         = FALSE;
}

#define TEST_DELAY  0
#if 0
void TEST_RepeatFlowValve(void)
{
    static U8 mu8Toggle = 0;


    if( FlowTest.Run == FALSE )
    {
        return ;
    }

    if( mu8Toggle != 0 )
    {
        if( HAL_IsDoneMoveStep( FLOW_VALVE_ID ) == TRUE )
        {
            mu8Toggle = 0;

            FlowTest.RepeatCount++;
            
            if( FlowTest.Delay != 0 )
            {
                FlowTest.Delay--;
                return ;
            }

            FlowValveInit();
        }
        else
        {
            FlowTest.Delay = TEST_DELAY;
            FlowValveFullOpen();
        }
    }
    else
    {
        if( HAL_IsDoneMoveStep( FLOW_VALVE_ID ) == TRUE )
        {
            mu8Toggle = 1;

            if( FlowTest.Delay != 0 )
            {
                FlowTest.Delay--;
                return ;
            }
            FlowValveFullOpen();
        }
        else
        {
            FlowTest.Delay = TEST_DELAY;
            FlowValveInit();
        }
    }
}
#endif

void TEST_RepeatFlowValve(void)
{
    static U8 mu8Toggle = 0;


    if( FlowTest.Run == FALSE )
    {
        return ;
    }


    if( mu8Toggle != 0 )
    {
        if( FlowValve.Mode == 0 )
        {
            mu8Toggle = 0;

            FlowTest.RepeatCount++;
            
            if( FlowTest.Delay != 0 )
            {
                FlowTest.Delay--;
                return ;
            }

            FlowValveTarget( 1000 );
        }
        else
        {
            FlowTest.Delay = TEST_DELAY;
            //FlowValveTarget( 1400 );
        }
    }
    else
    {
        if( FlowValve.Mode == 0 )
        {
            mu8Toggle = 1;

            if( FlowTest.Delay != 0 )
            {
                FlowTest.Delay--;
                return ;
            }
            FlowValveTarget( 1700 );
        }
        else
        {
            FlowTest.Delay = TEST_DELAY;
            //FlowValveTarget( 400 );
        }
    }
}
