#include "key_eol_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "display.h"
#include "sound.h"
#include "water_out.h"
#include "process_eol.h"
#include "front.h"
#include "process_display.h"
#include "gas_switch_valve.h"

static void SetKeyBit(U32 mu32MaskBit );

/* EOL */
// KEY EVENT LIST - EOL FRONT TEST 

static U8 EOL_Ice(void);
static U8 EOL_Room(void);
static U8 EOL_Cold(void);
static U8 EOL_Hot(void);
static U8 EOL_Amount(void);
static U8 EOL_Setting(void);
static U8 EOL_IceMake(void);
static U8 EOL_IceLock(void);
static U8 EOL_LockALl(void);
static U8 EOL_HotLock(void);
static U8 EOL_Save(void);
static U8 EOL_ColdMake(void);
static U8 EOL_TestSeal(void);

const static KeyEventList_T EOL_CHP_KeyEventList[] =
{
    /* KEY,      Short Pushed,  Long Pushed 2sec,  3sec, 5sec, Pop, TS */

    /* SINGLE KEY */
    { K_ICE,            EOL_Ice,        NULL,   NULL,   NULL,    NULL,  NULL },
    { K_ROOM,           EOL_Room,       NULL,   NULL,   NULL,    NULL,  NULL },
    { K_COLD,           EOL_Cold,       NULL,   NULL,   NULL,    NULL,  NULL },
    { K_HOT,            EOL_Hot,        NULL,   NULL,   NULL,    NULL,  NULL },
    { K_AMOUNT,         EOL_Amount,     NULL,   NULL,   NULL,    NULL,  NULL },


    { K_SETTING,        EOL_Setting,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_ICE_MAKE,       EOL_IceMake,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_ICE_LOCK,       EOL_IceLock,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_LOCK_ALL,       EOL_LockALl,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_HOT_LOCK,       EOL_HotLock,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_SAVE,           EOL_Save,       NULL,   NULL,   NULL,    NULL,  NULL },
    { K_COLD_MAKE,      EOL_ColdMake,   NULL,   NULL,   NULL,    NULL,  NULL },

};

const static KeyEventList_T EOL_CP_KeyEventList[] =
{
    /* KEY,      Short Pushed,  Long Pushed 2sec,  3sec, 5sec, Pop, TS */

    /* SINGLE KEY */
    { K_ROOM,           EOL_Room,       NULL,   NULL,   NULL,    NULL,  NULL },
    { K_COLD,           EOL_Cold,       NULL,   NULL,   NULL,    NULL,  NULL },
    { K_AMOUNT,         EOL_Amount,     NULL,   NULL,   NULL,    NULL,  NULL },

    { K_SETTING,        EOL_Setting,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_ICE_LOCK,       EOL_IceLock,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_LOCK_ALL,       EOL_LockALl,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_SAVE,           EOL_Save,       NULL,   NULL,   NULL,    NULL,  NULL },
    { K_COLD_MAKE,      EOL_ColdMake,   NULL,   NULL,   NULL,    NULL,  NULL },

    { K_CP_ICE_TURBO,   EOL_Hot,        NULL,   NULL,   NULL,    NULL,  NULL },
    { K_CP_ICE_SIZE,    EOL_Ice,        NULL,   NULL,   NULL,    NULL,  NULL },
    { K_CP_ICE_MAKE,    EOL_IceMake,    NULL,   NULL,   NULL,    NULL,  NULL }

};


// KEY EVENT LIST - EOL LOAD TEST 
static U8 EOL_SelHot(void);
static U8 EOL_SelRoom(void);
static U8 EOL_SelCold(void);
static U8 EOL_Drain(void);

const static KeyEventList_T EOL_CHP_KeyLoadEventList[] =
{
    /* KEY,      Short Pushed,  Long Pushed 2sec,  3sec, 5sec, Pop, TS */
    { K_TEST_EOL,    NULL,     NULL,   NULL,   NULL,    NULL,  EOL_TestSeal },

    /* SINGLE KEY */
    { K_HOT,         EOL_SelHot,       NULL,     NULL, NULL,    NULL, NULL },   // 온수 선택
    { K_ROOM,        EOL_SelRoom,      NULL,     NULL, NULL,    NULL, NULL },   // 정수 선택
    { K_COLD,        EOL_SelCold,      NULL,     NULL, NULL,    NULL, NULL },   // 냉수 선택
    { K_AMOUNT,      EOL_Drain,        NULL,     NULL, NULL,    NULL, NULL }    // 물양 선택( 배수 )
};

const static KeyEventList_T EOL_CP_KeyLoadEventList[] =
{
    /* KEY,      Short Pushed,  Long Pushed 2sec,  3sec, 5sec, Pop, TS */
    { K_TEST_EOL_CP,    NULL,    NULL,   NULL,   NULL,    NULL,  EOL_TestSeal },

    /* SINGLE KEY */
    { K_ROOM,        EOL_SelRoom,      NULL,     NULL, NULL,    NULL, NULL },   // 정수 선택
    { K_COLD,        EOL_SelCold,      NULL,     NULL, NULL,    NULL, NULL },   // 냉수 선택
    { K_AMOUNT,      EOL_Drain,        NULL,     NULL, NULL,    NULL, NULL }    // 물양 선택( 배수 )
};

// 진공 질소 검사 ( CHP, CP 공통)
static U8 EOL_CompOnOff(void);
static U8 EOL_MakeIcing(void);
static U8 EOL_MakeDeIcing(void);
static U8 EOL_MakeCold(void);
const static KeyEventList_T EOL_KeySealdLoadEventList[] =
{
    { K_SETTING,     EOL_CompOnOff,      NULL,     NULL, NULL,    NULL, NULL },
    { K_ICE,         EOL_MakeIcing,      NULL,     NULL, NULL,    NULL, NULL },   
    { K_HOT,         EOL_MakeDeIcing,    NULL,     NULL, NULL,    NULL, NULL },   
    { K_COLD,        EOL_MakeCold,       NULL,     NULL, NULL,    NULL, NULL },   

    { K_CP_ICE_SIZE,    EOL_MakeIcing,   NULL,     NULL, NULL,    NULL, NULL },   
    { K_CP_ICE_TURBO,   EOL_MakeDeIcing, NULL,     NULL, NULL,    NULL, NULL },   
};

static U8 IsValidkeyEol(U32 mu32Key)
{
    if( GetEolMode() != EOL_MODE_READY 
            && GetEolMode() != EOL_MODE_SEALED 
            && GetEolMode() != EOL_MODE_DONE )
    {
        return FALSE;   
    }

    // 추출 중이면, 입력을 받지 않는다.
    if( GetWaterOut() == TRUE )
    {
        return FALSE;
    }

    return TRUE;
}

static U8 IsValidKeyFront(U32 mu32Key)
{
    return IsValidkeyEol( mu32Key );
}

U8 IsValidEolKeyCondition(U32 mu32Key)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        return IsValidKeyFront( mu32Key );
    }

    return IsValidkeyEol( mu32Key );
}

void* GetEolKeyEventList(void)
{
    // KEY FRONT
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        if( GetSystem() == SYSTEM_CHP )
        {
            return (void *)EOL_CHP_KeyEventList;
        }

        return (void *)EOL_CP_KeyEventList;
    }

    // 진공질소 검사
    if( GetEolMode() == EOL_MODE_SEALED )
    {
        return (void *)EOL_KeySealdLoadEventList;
    }

    // KEY LOAD 
    if( GetSystem() == SYSTEM_CHP )
    {
        return (void *)EOL_CHP_KeyLoadEventList;
    }

    return (void *)EOL_CP_KeyLoadEventList;
}

U8 GetEolKeyEventListSize(void)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        if( GetSystem() == SYSTEM_CHP )
        {
            return ( sizeof(EOL_CHP_KeyEventList) / sizeof(KeyEventList_T) );
        }
        return ( sizeof(EOL_CP_KeyEventList) / sizeof(KeyEventList_T) );
    }
    
    // 진공질소 검사
    if( GetEolMode() == EOL_MODE_SEALED )
    {
        return ( sizeof(EOL_KeySealdLoadEventList) / sizeof(KeyEventList_T) );
    }

    if( GetSystem() == SYSTEM_CHP )
    {
        return ( sizeof(EOL_CHP_KeyLoadEventList) / sizeof(KeyEventList_T) );
    }
    return ( sizeof(EOL_CP_KeyLoadEventList) / sizeof(KeyEventList_T) );
}

// It is toggled evey time the button is selected.
static void SetKeyBit(U32 mu32MaskBit )
{
    U32 mu32InputVal;


    mu32InputVal = GetEolTestInputVal();
    if( (mu32InputVal & mu32MaskBit ) != 0 )
    {
        ClearEolTestInputBit( mu32MaskBit );
    }
    else
    {
        SetEolTestInputBit( mu32MaskBit );
    }
}

static U8 EOL_Room(void)
{
    SetKeyBit( MK_EOL_KEY_ROOM );

    return SOUND_SELECT;
}

static U8 EOL_Cold(void)
{
    SetKeyBit( MK_EOL_KEY_COLD );

    return SOUND_SELECT;
}

static U8 EOL_Hot(void)
{
    SetKeyBit( MK_EOL_KEY_HOT );

    return SOUND_SELECT;
}

static U8 EOL_Amount(void)
{
    SetKeyBit( MK_EOL_KEY_AMOUNT );

    return SOUND_SELECT;
}

static U8 EOL_Setting(void)
{
    SetKeyBit( MK_EOL_KEY_SETTING );

    return SOUND_SELECT;
}

static U8 EOL_IceMake(void)
{
    SetKeyBit( MK_EOL_KEY_ICE_MAKE );
    return SOUND_SELECT;
}

static U8 EOL_IceLock(void)
{
    SetKeyBit( MK_EOL_KEY_ICE_LOCK );
    return SOUND_SELECT;
}

static U8 EOL_LockALl(void)
{
    SetKeyBit( MK_EOL_KEY_LOCK_ALL );
    return SOUND_SELECT;
}

static U8 EOL_HotLock(void)
{
    SetKeyBit( MK_EOL_KEY_HOT_LOCK );
    return SOUND_SELECT;
}

static U8 EOL_Save(void)
{
    SetKeyBit( MK_EOL_KEY_SAVE );
    return SOUND_SELECT;
}

static U8 EOL_Ice(void)
{
    SetKeyBit( MK_EOL_KEY_ICE );
    return SOUND_SELECT;
}

static U8 EOL_ColdMake(void)
{
    SetKeyBit( MK_EOL_KEY_COLD_MAKE );
    return SOUND_SELECT;
}

static U8 EOL_TestSeal(void)
{
    StartDisplayInit();
    SetEolMode( EOL_MODE_SEALED_INIT );

    return SOUND_SELECT;
}


// KEY EVENT LIST - EOL LOAD TEST 
static U8 EOL_SelHot(void)
{
    // Select water type
    SetWaterOutSelect( SEL_WATER_HOT );
    SetEolTestInputBit( MK_EOL_KEY_HOT );
    ClearEolTestInputBit( MK_EOL_KEY_ROOM );
    ClearEolTestInputBit( MK_EOL_KEY_COLD );
    return SOUND_SELECT;
}

static U8 EOL_SelRoom(void)
{
    // Select water type
    SetWaterOutSelect( SEL_WATER_ROOM );
    ClearEolTestInputBit( MK_EOL_KEY_HOT );
    SetEolTestInputBit( MK_EOL_KEY_ROOM );
    ClearEolTestInputBit( MK_EOL_KEY_COLD );
    return SOUND_SELECT;
}

static U8 EOL_SelCold(void)
{
    // Select water type
    SetWaterOutSelect( SEL_WATER_COLD );
    ClearEolTestInputBit( MK_EOL_KEY_HOT );
    ClearEolTestInputBit( MK_EOL_KEY_ROOM );
    SetEolTestInputBit( MK_EOL_KEY_COLD );
    return SOUND_SELECT;
}


static U8 EOL_Drain(void)
{
    if( GetEolDrainWater() == FALSE )
    {
        SetEolTestInputBit( MK_EOL_KEY_AMOUNT );
        StartEolDrainWater();
        return SOUND_SETUP;
    }

    ClearEolTestInputBit( MK_EOL_KEY_AMOUNT );
    StopEolDrainWater();
    return SOUND_CANCEL;
}

////////////////////////////////////////////////////////////

// KEY EVENT LIST - EOL SEALD TEST 
extern U8 EOL_PrevComp;
extern U8 EOL_Comp;
extern U8 EOL_GasSwitch;
extern U16 EOL_CompOffDelay;
static U8 EOL_CompOnOff(void)
{
    if( EOL_CompOffDelay != 0 )
    {
        return SOUND_ERROR;
    }

    if( EOL_Comp == OFF )
    {
        EOL_Comp = ON;
    }
    else
    {
        EOL_Comp = OFF;
    }
    return SOUND_SELECT;
}

static U8 EOL_MakeIcing(void)
{
    EOL_GasSwitch = GAS_SWITCH_ICE;
    return SOUND_SELECT;
}

static U8 EOL_MakeDeIcing(void)
{
    EOL_GasSwitch = GAS_SWITCH_HOTGAS;
    return SOUND_SELECT;
}

static U8 EOL_MakeCold(void)
{
    EOL_GasSwitch = GAS_SWITCH_COLD;
    return SOUND_SELECT;
}
////////////////////////////////////////////////////////////
