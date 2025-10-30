#include "fct_lever_handler.h"
#include "lever_common_handler.h"

#include "fct.h"
#include "sound.h"

/* WATER OUT */
static U8 LeverOpenTapWater(void);
static U8 LeverCloseTapWater(void);

/* ICE DOOR */
static U8 LeverOpenIceDoor(void);
static U8 LeverCloseIceDoor(void);

const static LeverEventList_T FCT_LeverEventList[] =
{
    /* Short,               Long,   Long2     Pop  */
    {  LeverOpenTapWater,   NULL,   NULL,     NULL  },
    {  LeverOpenIceDoor,    NULL,   NULL,     NULL   }
};

/* WATER OUT - START */
static U8 LeverOpenTapWater(void)
{
    if( IsSetFctTestInputVal( MK_FCT_LEVER_WATER ) == TRUE )
    {
        ClearFctTestInputBit( MK_FCT_LEVER_WATER );
    }
    else
    {
        SetFctTestInputBit( MK_FCT_LEVER_WATER );
    }

    return SOUND_SELECT;
}

static U8 LeverCloseTapWater(void)
{
    ClearFctTestInputBit( MK_FCT_LEVER_WATER );

    return SOUND_SELECT;
}


/* ICE DOOR - PUSHED( OPEN )*/
static U8 LeverOpenIceDoor(void)
{
    if( IsSetFctTestInputVal( MK_FCT_LEVER_ICE ) == TRUE )
    {
        ClearFctTestInputBit( MK_FCT_LEVER_ICE );
    }
    else
    {
        SetFctTestInputBit( MK_FCT_LEVER_ICE );
    }

    return SOUND_SELECT;
}

static U8 LeverCloseIceDoor(void)
{
    ClearFctTestInputBit( MK_FCT_LEVER_ICE );

    return SOUND_SELECT;
}

void* GetFctLeverEventList(void)
{
    // return address of lever event table 
    return (void *)FCT_LeverEventList;
}

