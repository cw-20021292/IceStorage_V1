#include "eol_lever_handler.h"
#include "lever_common_handler.h"

#include "eol.h"
#include "ice_door.h"
#include "water_out.h"
#include "sound.h"
#include "front.h"

/* WATER OUT */
static U8 LeverOpenTapWater(void);
static U8 LeverCloseTapWater(void);

/* ICE DOOR */
static U8 LeverOpenIceDoor(void);
static U8 LeverCloseIceDoor(void);

const static LeverEventList_T EOL_LeverEventList[] =
{
    /* Short,               Long, Long2     Pop  */
    {  LeverOpenTapWater,   NULL, NULL,     LeverCloseTapWater  },
    {  LeverOpenIceDoor,    NULL, NULL,     LeverCloseIceDoor   }
};

/* WATER OUT - START */
static U8 LeverOpenTapWater(void)
{
    U32 mu32Mask = MK_JIG_FRONT_LAST_CHP;

    if( GetSystem() == SYSTEM_CP )
    {
        mu32Mask = MK_JIG_FRONT_LAST_CP;
    }

    if( GetEolType() == EOL_TYPE_FRONT )
    {
        if( (GetEolTestInputVal() & MK_EOL_LEVER_WATER ) != 0UL )
        {
            ClearEolTestInputBit( MK_EOL_LEVER_WATER );
        }
        else
        {
            SetEolTestInputBit( MK_EOL_LEVER_WATER );

            // 모든 버튼을 다 누른다음에 마지막 물 추출 버튼을 누른 경우 LAST STEP 처리
            if( (GetEolTestInputVal() & mu32Mask ) == mu32Mask )
            {
                SetEolTestInputBit( MK_EOL_LAST_STEP );
            }
        }

        return SOUND_SELECT;
    }

    if( GetEolMode() != EOL_MODE_READY 
            && GetEolMode() != EOL_MODE_DONE )
    {
        return SOUND_NONE;
    }

    SetWaterOutFixedAmountType( FIXED_AMOUNT_INFINITY );
    StartWaterOut();
    return SOUND_SELECT;
}

/* WATER OUT - STOP */
static U8 LeverCloseTapWater(void)
{
    if( GetEolType() == EOL_TYPE_LOAD )
    {
        StopWaterOut();
    }

    return SOUND_NONE;
}


/* ICE DOOR - PUSHED( OPEN )*/
static U8 LeverOpenIceDoor(void)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        if( (GetEolTestInputVal() & MK_EOL_LEVER_ICE ) != 0 )
        {
            ClearEolTestInputBit( MK_EOL_LEVER_ICE );
        }
        else
        {
            SetEolTestInputBit( MK_EOL_LEVER_ICE );
        }
        return SOUND_SELECT;
    }


    if( GetEolMode() != EOL_MODE_READY 
            && GetEolMode() != EOL_MODE_DONE )
    {
        return SOUND_NONE;
    }

    OpenIceDoor();
    return SOUND_SELECT;
}

/*ICE DOOR - POP ( CLOSE ) */
static U8 LeverCloseIceDoor(void)
{
    CloseIceDoor();
    return SOUND_NONE;
}

void* GetEolLeverEventList(void)
{
    // return address of lever event table 
    return (void *)EOL_LeverEventList;
}

