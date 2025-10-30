#include "key_fct_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "display.h"
#include "sound.h"
#include "process_fct.h"


static void SetKeyBit(U32 mu32MaskBit );

static U8 FCT_IceMode(void);
static U8 FCT_Room(void);
static U8 FCT_Cold(void);
static U8 FCT_Hot(void);
static U8 FCT_Amount(void);
static U8 FCT_Setting(void);
static U8 FCT_IceSize(void);
static U8 FCT_IceMake(void);
static U8 FCT_IceLock(void);
static U8 FCT_LockALl(void);
static U8 FCT_HotLock(void);
static U8 FCT_Save(void);
static U8 FCT_AmountUser(void);
static U8 FCT_IceTurbo(void);
static U8 FCT_ColdMake(void);

/* FCT */
const static KeyEventList_T FCT_KeyEventList[] =
{
    /* KEY,      Short Pushed,  Long Pushed 2sec,  3sec, 5sec, Pop, TS */

    /* SINGLE KEY */
    { K_SETTING,        FCT_Setting,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_ICE,            FCT_IceMode,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_HOT,            FCT_Hot,        NULL,   NULL,   NULL,    NULL,  NULL },
    { K_ROOM,           FCT_Room,       NULL,   NULL,   NULL,    NULL,  NULL },
    { K_COLD,           FCT_Cold,       NULL,   NULL,   NULL,    NULL,  NULL },
    { K_AMOUNT,         FCT_Amount,     NULL,   NULL,   NULL,    NULL,  NULL },

    /* CP MODEL */
    { K_CP_ICE_SIZE,    FCT_IceMode,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_CP_ICE_TURBO,   FCT_Hot,        NULL,   NULL,   NULL,    NULL,  NULL },
    { K_CP_ICE_MAKE,    FCT_HotLock,    NULL,   NULL,   NULL,    NULL,  NULL },

    { K_ICE_MAKE,       FCT_IceMake,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_ICE_LOCK,       FCT_IceLock,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_HOT_LOCK,       FCT_HotLock,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_LOCK_ALL,       FCT_LockALl,    NULL,   NULL,   NULL,    NULL,  NULL },
    { K_COLD_MAKE,      FCT_ColdMake,   NULL,   NULL,   NULL,    NULL,  NULL },
    { K_SAVE,           FCT_Save,       NULL,   NULL,   NULL,    NULL,  NULL },
};


U8 IsValidFctKeyCondition(U32 mu32Key)
{

    return TRUE;
}

void* GetFctKeyEventList(void)
{
    return (void *)FCT_KeyEventList;
}

U8 GetFctKeyEventListSize(void)
{
    return ( sizeof(FCT_KeyEventList) / sizeof(KeyEventList_T) );
}


/*************************************************************************************************************
 * KEY EVENT HANDLER....
 */

static void SetKeyBit(U32 mu32MaskBit )
{
    U32 mu32InputVal;


    mu32InputVal = GetFctTestInputVal();
    if( (mu32InputVal & mu32MaskBit ) != 0 )
    {
        ClearFctTestInputBit( mu32MaskBit );
    }
    else
    {
        SetFctTestInputBit( mu32MaskBit );
    }
}


static U8 FCT_Room(void)
{
    SetKeyBit( MK_FCT_KEY_ROOM );

    return SOUND_SELECT;
}

static U8 FCT_Cold(void)
{
    SetKeyBit( MK_FCT_KEY_COLD );

    return SOUND_SELECT;
}

static U8 FCT_Hot(void)
{
    SetKeyBit( MK_FCT_KEY_HOT );

    return SOUND_SELECT;
}

static U8 FCT_Amount(void)
{
    SetKeyBit( MK_FCT_KEY_AMOUNT );

    return SOUND_SELECT;
}

static U8 FCT_Setting(void)
{
    SetKeyBit( MK_FCT_KEY_SETTING );

    return SOUND_SELECT;
}

static U8 FCT_IceMode(void)
{
    SetKeyBit( MK_FCT_KEY_ICE );

    return SOUND_SELECT;
}

static U8 FCT_IceMake(void)
{
    SetKeyBit( MK_FCT_KEY_ICE_MAKE );
    return SOUND_SELECT;
}

static U8 FCT_IceLock(void)
{
    SetKeyBit( MK_FCT_KEY_ICE_LOCK );
    return SOUND_SELECT;
}

static U8 FCT_LockALl(void)
{
    SetKeyBit( MK_FCT_KEY_LOCK_ALL );
    return SOUND_SELECT;
}

static U8 FCT_HotLock(void)
{
    SetKeyBit( MK_FCT_KEY_HOT_LOCK );
    return SOUND_SELECT;
}

static U8 FCT_Save(void)
{
    SetKeyBit( MK_FCT_KEY_SAVE );
    return SOUND_SELECT;
}

static U8 FCT_ColdMake(void)
{
    SetKeyBit( MK_FCT_KEY_COLD_MAKE );
    return SOUND_SELECT;
}
