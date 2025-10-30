#include "hw.h"
#include "hal_level.h"

#if 0
U8 HAL_GetLevelRoomTank(void)
{
#if 0
    U8 mu8Val = 0;


    if( GET_ROOM_TANK_LOW() == HIGH )
    {
        mu8Val |= LEVEL_DETECT_LOW;
    }
    
    if( GET_ROOM_TANK_HIGH()  == HIGH )
    {
        mu8Val |= LEVEL_DETECT_HIGH;
    }

    if( GET_ROOM_TANK_OVERFLOW() == HIGH )
    {
        mu8Val |= LEVEL_DETECT_OVF;
    }

    return mu8Val;
#else
    return (LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH);
#endif
}

U8 HAL_GetLevelHotTank(void)
{
#if 0
    U8 mu8Val = 0;


    /* 정수 탱크의 저수위 센서가
     * 온수 탱크의 만수위 센서 역할을 한다.
     *
     * 수위 센서는 저수위 센서와 만수위 센서 2개가 있다고 가정한다.
     */

    if( GET_ROOM_TANK_LOW() == HIGH )
    {
        mu8Val |= LEVEL_DETECT_LOW;
        mu8Val |= LEVEL_DETECT_HIGH;
    }

    return mu8Val;

#else

    return (LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH);
#endif
}

U8 HAL_GetLevelColdTank(void)
{
    return (LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH);
}
#endif

U8 HAL_GetLevelDrainTank(void)
{
    U8 mu8Val = 0;


    if( GET_DRAIN_TANK_LOW() == HIGH )
    {
        mu8Val |= LEVEL_DETECT_LOW;
    }
    
    if( GET_DRAIN_TANK_HIGH()  == HIGH )
    {
        mu8Val |= LEVEL_DETECT_HIGH;
    }

    return mu8Val;
}

#if 0
U8 HAL_GetLevelIceTray(void)
{
#if 0
    U8 mu8Val = 0;


    /* 드레인 탱크의 저수위 센서로
     * 아이스 트레이의 수위를 확인 할 수있다 
     */
    if( GET_DRAIN_TANK_LOW() == HIGH )
    {
        mu8Val |= LEVEL_DETECT_HIGH;
    }
    
    return mu8Val;
#else
    return (LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH);
#endif
}
#endif

