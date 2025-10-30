/**
 * 배수( 자동 / 수동 ) 기능에 대한 모듈
 *
 */
#include "drain_water.h"
#include "health_maker.h"
#include "water_out.h"
#include "cold_water.h"
#include "error.h"
#include "flow_meter.h"


// 추출량
#define MIN_CONSUMPTION_COLD_WATER     (AMOUNT_500)


DrainWater_T     DrainWater;

// 배수 목표 시간 @100ms
static U32 gu32DrainTimeTable[ 2 ] = 
{
    AMOUNT_250,   // HALF DRAIN TIME, 250ml
    AMOUNT_12000,  // FULL DRAIN TIME, 12L
};

static U8 IsValidStartDrain(void);
static U8 CheckWaterConsumption(void);
static void UpdateDrainTime(void);

void  InitDrainWater(void)
{
    DrainWater.Start  = FALSE;
    DrainWater.Mode   = HALF_DRAIN;

    DrainWater.DrainAmount   = 0UL;
    DrainWater.ConsumColdWater = MIN_CONSUMPTION_COLD_WATER;
}

void  GetDrainWaterData(DrainWater_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&DrainWater, sizeof( DrainWater_T ));
}

// 자동 배수 시작/중지
U8 StartDrainWater(U8 mu8Mode)
{
    // 시작 조건이 맞는 경우에만, 시작한다.
    if( IsValidStartDrain() == TRUE )
    {
        // HALF 모드에서는 물 사용량을 조건을 확인.
        if( mu8Mode == HALF_DRAIN )
        {
            if( CheckWaterConsumption() == FALSE )
            {
                DrainWater.ConsumColdWater = MIN_CONSUMPTION_COLD_WATER;
                return FALSE;
            }
        }

        DrainWater.Start = TRUE;

        DrainWater.Mode = mu8Mode;
        DrainWater.DrainAmount  = gu32DrainTimeTable[ DrainWater.Mode ];
        InitHealthDrain();

        RegisterFlowMeterId( FLOW_ID_DRAIN, UpdateDrainTime );

        return TRUE;
    }

    return FALSE;
}

static void StopDrainWater(void)
{
    DrainWater.Start = FALSE;

    DrainWater.ConsumColdWater = MIN_CONSUMPTION_COLD_WATER;

    InitHealthDrain();

    RegisterFlowMeterId( FLOW_ID_DRAIN, NULL );
}

U8 IsStartDrainWater(void)
{
    return DrainWater.Start;
}

U8 IsDrainWaterHalf(void)
{
    if( DrainWater.Mode == HALF_DRAIN )
    {
        return TRUE;
    }

    return FALSE;
}

U8 GetDrainTimePercent(void)
{
    U32 mu32Val = 0;
    U32 mu32Current;
    U32 mu32Base;

    mu32Current = DrainWater.DrainAmount * 100;
    mu32Base    = gu32DrainTimeTable[ DrainWater.Mode ];

   mu32Val = (mu32Current / mu32Base );
   mu32Val = 100UL - mu32Val;

    return (U8)mu32Val;
}

// 순환 강제 배수 중이면, TRUE
// 그렇지 않으면 FALSE
U8 IsGoingOnDrainWaterFull(void)
{
    if( IsStartDrainWater() == TRUE
            && IsDrainWaterHalf() == FALSE )
    {
        return TRUE;   
    }

    return FALSE;
}

// 배수 기능 취소
U8 ResetDrainWater(void)
{
    if( IsStartDrainWater() == TRUE )
    {
        StopHealthDrain();
        return TRUE;
    }

    return FALSE;
}


/* 배수 동작 가능 확인
 * Return
 *  - TRUE : 가능
 *  - FALSE : 불가능
 */
static U8 IsValidStartDrain(void)
{
    if( IsErrorType( ERR_TYPE_DRAIN ) == TRUE )
    {
        return FALSE;
    }

    return TRUE;
}

void UpdateDrainColdWaterConsume(U32 mu32Amount)
{
    if( DrainWater.ConsumColdWater <= mu32Amount )
    {
        DrainWater.ConsumColdWater = 0UL;
    }
    else
    {
        DrainWater.ConsumColdWater -= mu32Amount;
    }
}

/* 물 사용량 계산
 *  - 배수를 할 만큼 물 사용량이 많은지 확인한다.
 *
 * Return
 *  - TRUE : 배수 조건 만족
 *  - FALSE : 배수 조건 불만족
 */
static U8 CheckWaterConsumption(void)
{
    if( DrainWater.ConsumColdWater != 0 )
    {
        // 충분히 물을 사용하지 않았으면, 배수 조건이다.
        return TRUE;
    }

    return FALSE;
}


static void UpdateDrainTime(void)
{
    if( DrainWater.DrainAmount != 0 )
    {
        DrainWater.DrainAmount--;
    }
}

void ControlDrainWater(void)
{
    U8 mu8Ret = FALSE;
    

    if( IsStartDrainWater() == FALSE )
    {
        return ;
    }

    // 배수 타임 아웃 종료
    // 에러 발생시 종료
    if( DrainWater.DrainAmount == 0
            || IsErrorType( ERR_TYPE_DRAIN ) == TRUE )
    {
        StopHealthDrain();
    }

    // 배수...
    if( DoHealthDrain() == STATUS_DONE )
    {
        StopDrainWater();
    }
}

