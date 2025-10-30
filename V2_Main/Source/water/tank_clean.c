#include "tank_clean.h"
#include "water_out.h"
#include "ice.h"
#include "ice_maker.h"
#include "service.h"
#include "cold_water.h"


// 기능 삭제. 2020.03.24
// 사용자들이 기능을 많이 헷갈려함.
// 냉수 연속 추출은 추출 시간은 늘리는 것으로...
// 탱크 청소 기능 사용시에 부하 정지 기능 활성화
#define CONFIG_TANK_CLEAN_STOP_LOAD     0       // 0: 미사용 1:사용

// 냉수 저수위 미감지 후, 잔수 제거 시간

typedef struct _tank_clean_
{
    /* Clean Tank Mode */
    U8   Out;     // TRUE or FALSE
    U8   OffLoad;    // TRUE or FALSE (부하 OFF)

    U8   CodyService;       // 코디 서비스 모드 ( TRUE, FALSE )
} TankClean_T;


TankClean_T Clean;

void InitTankClean(void)
{
    Clean.Out       = FALSE;
    Clean.OffLoad   = FALSE;

    Clean.CodyService   = FALSE;
}



/* 탱크 비우기 모드 설정/해제 */
void SetTankClean(U8 mu8Mode)
{
    Clean.Out = mu8Mode;

#if CONFIG_TANK_CLEAN_STOP_LOAD
    if( mu8Mode == TRUE )
    {
        Clean.OffLoad = TRUE;
        StopIceMake();
    }
#endif
}

U8 GetTankClean(void )
{
    return Clean.Out;
}


void SetCodyService(U8 mu8Mode)
{
    Clean.CodyService = mu8Mode;

    if( mu8Mode == TRUE )
    {
        StopDummyDeIcing();
        StopIceMake();
    }
}

U8 GetCodyService(void)
{
    return Clean.CodyService;
}

U8 IsTankCleanMode(void)
{
    /* 탱크 탑 OPEN,
     * 냉수 선택,
     * 연속 추출 선택 
     */
#if CONFIG_TANK_CLEAN_STOP_LOAD
    if( GetServiceCheckTankOpen() == TRUE 
            && GetWaterOutSelect() == SEL_WATER_COLD
            && GetWaterOutAmoutType() == FIXED_AMOUNT_INFINITY)
    {
        return TRUE; // 탱크 비우기 모드 조건 만족
    }
#endif

    return FALSE; 
}

U8 GetTankCleanOffLoad(void)
{
    return Clean.OffLoad;
}


void ClearTankCleanOffLoad(void)
{
    Clean.OffLoad = FALSE;
}

void ProcessTankClean(void)
{
#if CONFIG_TANK_CLEAN_STOP_LOAD
    // 2020.03.24, 기능 삭제 ( 사용자들에 많이 헷갈려함 )
    if( GetServiceCheckTankOpen() == FALSE 
            && GetTankClean() == FALSE )
    {
        ClearTankCleanOffLoad();
    }
#endif
}

