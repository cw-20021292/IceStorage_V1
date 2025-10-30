#include "ice_door.h"
#include "hal_step_motor.h"
#include "relay.h"
#include "ice_full.h"
#include "ice.h"


#define  ICE_DOOR_ID       SM_ID_1
#define  ICE_DOOR_SPEED    SM_SPEED_0       // 400pps

//#define  ICE_DOOR_OPEN_STEP_VAL    (-380)   // Just 350 steps
//#define  ICE_DOOR_CLOSE_STEP_VAL   (400)
#define  ICE_DOOR_OPEN_STEP_VAL    (-400)   // Just 350 steps
#define  ICE_DOOR_CLOSE_STEP_VAL   (400)

/* MODE */
#define  ICE_DOOR_MODE_NONE    0x00
#define  ICE_DOOR_MODE_INIT    0x01
#define  ICE_DOOR_MODE_CLOSE   0x02
#define  ICE_DOOR_MODE_OPEN    0x04
//#define  ICE_DOOR_MODE_TEST    0x08


typedef struct _ice_door_
{
    /* STATUS */
    U8 Status;       // Open or Close
    
    /* MODE */
    U8 Mode;         // Init / Close / Open 
    U8 InitStep;
    U8 OpenStep;
    U8 CloseStep;
    //U8 TestStep;

    /* Forced Close Door Timer */
    U32 CloseTime;

    U16 OpeningTimeIot;        /* @Sec */
    U16 OpeningTime;        /* @Sec */
} IceDoor_T;

IceDoor_T  Door;

void InitIceDoor(void)
{
    Door.Status      = ICE_DOOR_CLOSE;

    Door.Mode        = ICE_DOOR_MODE_INIT;
    Door.InitStep    = 0;
    Door.OpenStep    = 0;
    Door.CloseStep   = 0;
    //Door.TestStep    = 0;

    Door.CloseTime      = ICE_CLOSE_TIME;
    Door.OpeningTimeIot = 0;
    Door.OpeningTime    = 0;

    HAL_InitStepVal( ICE_DOOR_ID );
    HAL_SetStepSpeed( ICE_DOOR_ID, ICE_DOOR_SPEED );
}

U16 GetIceDoorOpeningTime(void)
{
    return Door.OpeningTime;
}

void SetIceDoorOpeningTime(U16 mu16Time)
{
    Door.OpeningTime = mu16Time;
}

U16 GetIceDoorOpeningTimeIot(void)
{
    return Door.OpeningTimeIot;
}

void SetIceDoorOpeningTimeIot(U16 mu16Time)
{
    Door.OpeningTimeIot = mu16Time;
}

void CountingIceDoorOpeningTime(void)
{
    if( IsOpenIceDoor() == TRUE )
    {
        if( Door.OpeningTime< 65535U )
        {
            Door.OpeningTime++;
        }

        if( Door.OpeningTimeIot< 65535U )
        {
            Door.OpeningTimeIot++;
        }

    }
}

void SetIceDoorConfClosedTime(U32 mu16Time)
{
    Door.CloseTime   = mu16Time;
}

U32 GetIceDoorConfClosedTime(void)
{
    return Door.CloseTime;
}


static void SetMode(U8 mu8Mode );
static void ClearMode(U8 mu8Mode );

U8 GetIceDoorStatus(void)
{
    return Door.Status;
}

// 아이스 도어 추출 여부
// RETURN
// TRUE - 추출 중
// FALSE - 추출 중지
U8 IsOpenIceDoor(void)
{
    if( Door.Status == ICE_DOOR_OPEN 
            || Door.Status == ICE_DOOR_OPENING )
    {
        return TRUE;
    }

    return FALSE;
}

static void SetIceDoorStatus(U8 mu8Status)
{
    Door.Status = mu8Status;
    if( mu8Status == ICE_DOOR_CLOSE )
    {
        Door.CloseTime = ICE_CLOSE_TIME;
    }
}

void OpenIceDoor(void)
{
    SetMode( ICE_DOOR_MODE_OPEN );
}

void CloseIceDoor(void)
{
    SetMode( ICE_DOOR_MODE_CLOSE );
}


// 강제 CLOSE
void ForcedCloseIceDoor(void)
{
    // 아이스 도어는 닫혀 있을 때에만, 한 번더 닫아준다.
    // 아이스 도어가 open되어 있다는 것은 현재 사용자가 사용 중인 상태이고,
    // 강제 닫기 동작은 사용자가 사용 중일 때에는 동작해서는 안된다.
    if( IsOpenIceDoor() == FALSE )
    {
        SetMode( ICE_DOOR_MODE_INIT );
    }
}

static void SetMode(U8 mu8Mode )
{
    Door.Mode |= mu8Mode;
    if( mu8Mode == ICE_DOOR_MODE_OPEN )
    {
        ClearMode( ICE_DOOR_MODE_CLOSE );
    }
    else if( mu8Mode == ICE_DOOR_MODE_CLOSE )
    {
        ClearMode( ICE_DOOR_MODE_OPEN );
    }
    //else if( mu8Mode == ICE_DOOR_MODE_TEST )
    //{
    //    ClearMode( ICE_DOOR_MODE_OPEN );
    //    ClearMode( ICE_DOOR_MODE_CLOSE );
    //}
}

static void ClearMode(U8 mu8Mode )
{
    Door.Mode &= ~mu8Mode;

    if( mu8Mode == ICE_DOOR_MODE_INIT )
    {
        Door.InitStep = 0;
    }
    else if( mu8Mode == ICE_DOOR_MODE_OPEN )
    {
        Door.CloseStep = 0;
    }
    else if( mu8Mode == ICE_DOOR_MODE_CLOSE )
    {
        Door.OpenStep = 0;
    }
    //else if( mu8Mode == ICE_DOOR_MODE_TEST )
    //{
    //    Door.TestStep = 0;
    //}
}


// 24시간 기준 강제 닫기 동작
void ControlForcedCloseIceDoor(void)
{
    // 얼음 OFF이면 동작하지 않음.
    if( GetIceConfigMake() == FALSE )
    {
        Door.CloseTime = ICE_CLOSE_TIME;
        return ;
    }


    if( Door.CloseTime > 0 )
    {
        Door.CloseTime--;
        return ;
    }

    ForcedCloseIceDoor();
    Door.CloseTime = ICE_CLOSE_TIME;
}


static void ClearMode(U8 mu8Mode );
static U8 InitDoor(void);
static U8 OpenDoor(void);
static U8 CloseDoor(void);
#if 0
static U8 TestDoor(void);
#endif
#if 0 //test
U8 the_test = 0;
I16 dbg_val = 10;
U8 sm_id = 2;
#endif
void ControlIceDoor(void)
{
    U8 mu8Ret = FALSE;

#if 0 // test 
    {
        if( the_test )
        {
            if( HAL_IsDoneMoveStep( sm_id ) == TRUE )
            {
                HAL_SetTargetStep( sm_id, HAL_GetTargetStep( sm_id ) + dbg_val );
            }
        }



    }
#endif

#if 0
    /* TEST */
    if( (Door.Mode & ICE_DOOR_MODE_TEST ) == ICE_DOOR_MODE_TEST )
    {
        mu8Ret = TestDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_TEST );
            SetIceDoorStatus( ICE_DOOR_CLOSE );
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_CLOSING );
        }
        return ;
    }
#endif

    /* Init */
    if( (Door.Mode & ICE_DOOR_MODE_INIT ) == ICE_DOOR_MODE_INIT )
    {
        mu8Ret = InitDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_INIT );
            SetIceDoorStatus( ICE_DOOR_CLOSE );
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_CLOSING );
        }
        return ;
    }

    /* Open */
    if( (Door.Mode & ICE_DOOR_MODE_OPEN ) == ICE_DOOR_MODE_OPEN )
    {
        mu8Ret = OpenDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_OPEN );
            SetIceDoorStatus( ICE_DOOR_OPEN );
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_OPENING );
        }
        return ;
    }

    /* Close */
    if( (Door.Mode & ICE_DOOR_MODE_CLOSE ) == ICE_DOOR_MODE_CLOSE )
    {
        mu8Ret = CloseDoor();
        if( mu8Ret == TRUE )
        {
            ClearMode( ICE_DOOR_MODE_CLOSE );
            SetIceDoorStatus( ICE_DOOR_CLOSE );

            StartCheckIceFull();
        }
        else
        {
            SetIceDoorStatus( ICE_DOOR_CLOSING );
        }
        return ;
    }

}

/*
 * 아이스 도어 스텝 모터 제어 ( close )
 */
static void CloseStepMotor(void)
{
    // 현재 스텝 모터 위치 초기화를 하고 ICE_DOOR_CLOSE_STEP_VAL 이동한다.
    HAL_SetTargetStep( ICE_DOOR_ID, ICE_DOOR_CLOSE_STEP_VAL );
}

/* 아이스 도어 스텝 모터 제어 ( open )
 */
static void OpenStepMotor(void)
{
    // 현재 스텝 모터 위치 초기화를 하고 ICE_DOOR_OPEN_STEP_VAL 이동한다.
    HAL_SetTargetStep( ICE_DOOR_ID, ICE_DOOR_OPEN_STEP_VAL );
}

// 도어 초기화 제어
// 강제로 닫고 원점 초기화
static U8 InitDoor(void)
{
    switch( Door.InitStep )
    {
        case 0:
            HAL_InitStepVal( ICE_DOOR_ID ); // 위치 초기화
            HAL_SetTargetStep( ICE_DOOR_ID, ICE_DOOR_CLOSE_STEP_VAL );
            Door.InitStep++;
            break;

        case 1:
            if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                HAL_InitStepVal( ICE_DOOR_ID ); // 위치 초기화
                Door.InitStep++;
            }
            break;

        case 2:
            return TRUE;

    }

    return FALSE;
}

// 도어 OPEN 제어
static U8 OpenDoor(void)
{
    static U16 mu16DelayTime = 0;   /* @100ms */


    switch( Door.OpenStep )
    {
        case 0:
            mu16DelayTime = 5;  // 0.5sec..

            HAL_StopMove( ICE_DOOR_ID );
            HAL_SetTargetStep( ICE_DOOR_ID, ICE_DOOR_OPEN_STEP_VAL );
            Door.OpenStep++;
            break;

        case 1:
            if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                Door.OpenStep++;
            }
            break;

        case 2:
            if( mu16DelayTime == 0 )
            {
                Door.OpenStep++;
            }
            else
            {
                mu16DelayTime--;
            }
            break;

        case 3:
            TurnOnRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            Door.OpenStep++;
            break;

        case 4:
            return TRUE;

    }

    return FALSE;

}

// 도어 CLOSE 제어
#if 0
static U8 CloseDoor(void)
{
    static U16 mu16DelayTime = 0;   /* @100ms */


    switch( Door.CloseStep )
    {
        case 0:
            mu16DelayTime = 10; 
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            HAL_StopMove( ICE_DOOR_ID );

            Door.CloseStep++;
            break;

        case 1:
            if( mu16DelayTime == 0 )
            {
                Door.CloseStep++;
            }
            else
            {
                mu16DelayTime--;
            }
            break;

        case 2:
            HAL_StopMove( ICE_DOOR_ID );
            HAL_SetTargetStep( ICE_DOOR_ID, 0 );
            Door.CloseStep++;
            break;

        case 3:
            if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                Door.CloseStep++;
            }
            break;

        case 4:
            return TRUE;

    }

    return FALSE;
}
#else
#define FEEDER_BACK_TIME    20  // 2sec @100ms
static U8 CloseDoor(void)
{
    static U16 mu16DelayTime = 0;   /* @100ms */
    static U8 ClearFeederStatus = FALSE;


    switch( Door.CloseStep )
    {
        case 0:
            mu16DelayTime = 10; 
            ClearFeederStatus = FALSE;
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            HAL_StopMove( ICE_DOOR_ID );

            Door.CloseStep++;
            break;

        case 1:
            if( mu16DelayTime == 0 )
            {
                Door.CloseStep++;
            }
            else
            {
                mu16DelayTime--;
            }
            break;

        case 2:
            // 피더 역회전
            if( GetIceFullOutStatus() == TRUE )
            {
#if 0
                if( Door.OpeningTime >= ICE_DOOR_CLEAR_BACK_FEEDER_TIME )
                {
                    // 추출 시간이 매우 긴 경우.. 얼음 추출이 다 된것으로 판다.
                    ClearIceFullOutStatus();
                    Door.OpeningTime = 0;
                }
                else 
#endif
                if( Door.OpeningTime >= ICE_DOOR_OPEN_BACK_FEEDER_TIME )
                {
                    // 추출 시간이 어중간하면... 입구에 얼음이 쌓였다고 판단
                    ClearFeederStatus = TRUE;
                    TurnOffRelay( RELAY_FEEDER_CW );
                    TurnOnRelay( RELAY_FEEDER_CCW );

                    Door.OpeningTime = 0;
                    mu16DelayTime = FEEDER_BACK_TIME;
                }
            }
            else
            {
                mu16DelayTime = 0;
            }

            Door.OpeningTime = 0;

            // 아이스 도어 CLOSE 
            HAL_StopMove( ICE_DOOR_ID );
            HAL_SetTargetStep( ICE_DOOR_ID, 0 );
            Door.CloseStep++;
            break;

        case 3:
            if( mu16DelayTime != 0 )
            {
                mu16DelayTime--;
            }

            if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE 
                    && mu16DelayTime == 0)
            {
                // 피더 역회전 중지
                if( ClearFeederStatus == TRUE )
                {
                    ClearIceFullOutStatus();
                }
                TurnOffRelay( RELAY_FEEDER_CW );
                TurnOffRelay( RELAY_FEEDER_CCW );

                Door.CloseStep++;
            }
            break;

        case 4:
            return TRUE;

    }

    return FALSE;
}
#endif


#if 0
static U8 TestDoor(void)
{
    switch( Door.TestStep )
    {
        case 0:
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            HAL_StopMove( ICE_DOOR_ID );

            Door.TestStep++;
            break;

        case 1:
            HAL_StopMove( ICE_DOOR_ID );
            HAL_SetTargetStep( ICE_DOOR_ID, 0 );
            Door.TestStep++;
            break;

        case 2:
            if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                HAL_SetTargetStep( ICE_DOOR_ID, ICE_DOOR_OPEN_STEP_VAL );
                Door.TestStep++;
            }
            break;
            
        case 3:
            if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                HAL_SetTargetStep( ICE_DOOR_ID, 0 );
                Door.TestStep++;
            }
            break;

            // 종료 단계.
        case 4:
            HAL_StopMove( ICE_DOOR_ID );
            HAL_SetTargetStep( ICE_DOOR_ID, 0 );
            Door.TestStep++;
            break;

        case 5:
            if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
            {
                Door.TestStep++;
            }
            break;

        case 6:
            return TRUE;

    }

    return FALSE;
}
#endif



/*************************************************************************************************************************
 * TEST
 */
typedef struct _ice_door_repeat_test_
{
    U8 Run;
    U8 Delay;
    U32 RepeatCount;
} IceDoorTest_T;

IceDoorTest_T DoorTest;
void TEST_StartIceDoor(void)
{
    DoorTest.Run         = TRUE;
    DoorTest.Delay       = 0;
    DoorTest.RepeatCount = 0;
}

void TEST_StopIceDoor(void)
{
    DoorTest.Run         = FALSE;
}

#define TEST_DELAY  0
void TEST_RepeatIceDoor(void)
{
    static U8 mu8Toggle = 0;


    if( DoorTest.Run == FALSE )
    {
        return ;
    }

    if( mu8Toggle != 0 )
    {
        if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
        {
            mu8Toggle = 0;

            DoorTest.RepeatCount++;
            
            if( DoorTest.Delay != 0 )
            {
                DoorTest.Delay--;
                return ;
            }

            CloseIceDoor();
        }
        else
        {
            DoorTest.Delay = TEST_DELAY;
            OpenIceDoor();
        }
    }
    else
    {
        if( HAL_IsDoneMoveStep( ICE_DOOR_ID ) == TRUE )
        {
            mu8Toggle = 1;

            if( DoorTest.Delay != 0 )
            {
                DoorTest.Delay--;
                return ;
            }
            OpenIceDoor();
        }
        else
        {
            DoorTest.Delay = TEST_DELAY;
            CloseIceDoor();
        }
    }
}
