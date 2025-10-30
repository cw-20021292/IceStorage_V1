#include "front.h"
#include "eeprom.h"
#include "timer.h"
#include "ice.h"
#include "hal_input.h"
#include "util.h"


#define LIMIT_TIMER         30  // 30sec @ 1sec

typedef struct _purifier_
{
    U8 ConfigDeco[ MAX_DECO_NUM ];      // Deco LED ON/OFF

    /* KEY - LOCK */
    U8 LockAll;         // LOCK or UNLOCK
    U8 LockHot;         // LOCK or UNLOCK
    U8 LockIce;         // LOCK or UNLOCK

    U8 System;          // BODY - CP or CHP
    U8 FrontSystem;     // FRONT - CP or CHP
    U8 Customer;        // 고객 �?�? 
    U8 FactorySetup;    // FALSE or TRUE

    U32 UnusedTime;     // @min
    U16 LimitTimer;     //

    U8 Fota;
} Front_T;

Front_T Front;


void InitFront(void)
{
    Front.ConfigDeco[ DECO_LED_WELCOME ] = TRUE;
    Front.ConfigDeco[ DECO_LED_BUTTON ]  = TRUE;

    Front.LockAll    = UNLOCK;
    Front.LockHot    = UNLOCK;
    Front.LockIce    = UNLOCK;
    Front.FrontSystem   = SYSTEM_CHP;
    Front.System        = SYSTEM_CHP;
    Front.Customer      = CUSTOMER_VISIT;

    Front.FactorySetup = FALSE;

    Front.UnusedTime    = 0UL;
    Front.LimitTimer    = LIMIT_TIMER;

    Front.Fota          = FALSE;
}


void SetFactroySetup(U8 mu8Setup)
{
    Front.FactorySetup = mu8Setup;
}

U8 GetFactorySetup(void)
{
    return Front.FactorySetup;
}

// 공장 초기?�� RESET?�� 경우, 
// �? 모듈?�� �??�� 초기?�� ?���?. 
// 그다?�� EEPROM ?��?�� 보상 ?��?��?�� ?��출되?��?�� ?��?��.
// ?��?�� ?��?��
// 1. ?��?�� ?���? ?���? 취소
// 2. ?��축기 구속 ?���? 초기?��?
void CheckFactroySetup(void)
{
    if( Front.FactorySetup == FALSE )
    {
        //ClearIceMakeMode( ICE_MODE_DUMMY_DEICING );

        Front.FactorySetup = TRUE;
        SaveEepromId( EEP_ID_FACTORY_SETUP );
    }
}


void SetConfigDeco(U8 mu8Id, U8 mu8OnOff )
{
    if( mu8OnOff != TRUE && mu8OnOff != FALSE )
    {
        return ;
    }
    Front.ConfigDeco[mu8Id] = mu8OnOff;
    SaveEepromId( EEP_ID_CONF_DECO_LED );
}

U8 GetConfigDeco(U8 mu8Id)
{
    return Front.ConfigDeco[mu8Id];
}

void SetSystem(U8 mu8Type )
{
    Front.System = mu8Type;
}

U8 GetSystem(void)
{
#if (CONFIG_SYSTEM_TYPE == CONFIG_SYSTEM_CHP )
    return SYSTEM_CHP;
#elif (CONFIG_SYSTEM_TYPE == CONFIG_SYSTEM_CP )
    return SYSTEM_CP;
#elif (CONFIG_JIG_RBK_FRONT != 0)
    return Front.FrontSystem;
#else
    return Front.System;
#endif
}


#define	DEFAULT_CHECK_COUNT	100 // 1sec @10ms
void CheckSystem( void )
{
    static U16 CountChp = DEFAULT_CHECK_COUNT;
    static U16 CountCp  = DEFAULT_CHECK_COUNT;
    static U8 IsDoneCheck = FALSE;

    do
    {
        Delay_MS(30); // 30 -> actually 10ms

        if( HAL_GetHeaterPBA() != 0 )
        {
            CountCp = DEFAULT_CHECK_COUNT;
            CountChp--;
            if( CountChp == 0 )
            {
                SetSystem( SYSTEM_CHP);
                IsDoneCheck = TRUE;
            }
        }
        else
        {
            CountChp = DEFAULT_CHECK_COUNT;
            CountCp--;
            if( CountCp == 0 )
            {
                SetSystem( SYSTEM_CP);
                IsDoneCheck = TRUE;
            }
        }
    }
    while (IsDoneCheck == FALSE);
}


void SetFrontSystem(U8 mu8Type )
{
    Front.FrontSystem = mu8Type;
}

U8 GetFrontSystem(void)
{
#if (CONFIG_SYSTEM_TYPE == CONFIG_SYSTEM_CHP )
    return SYSTEM_CHP;
#elif (CONFIG_SYSTEM_TYPE == CONFIG_SYSTEM_CP )
    return SYSTEM_CP;
#else
    return Front.FrontSystem;
#endif
}

void SetCustomerType(U8 mu8Type)
{
    Front.Customer = mu8Type;
    SaveEepromId( EEP_ID_CUSTOMER );
}

U8 GetCustomerType(void)
{
    return Front.Customer;
}

void SetLockAll(U8 mu8Lock )
{
    if( mu8Lock != LOCK && mu8Lock != UNLOCK )
    {
        return ;
    }

    Front.LockAll = mu8Lock;
    SaveEepromId( EEP_ID_LOCK_ALL );
}

U8 GetLockAll(void)
{
    return Front.LockAll;
}


void SetLockHot(U8 mu8Lock )
{
    if( mu8Lock != LOCK && mu8Lock != UNLOCK )
    {
        return ;
    }

    Front.LockHot = mu8Lock;
    SaveEepromId( EEP_ID_LOCK_HOT );
}

U8 GetLockHot(void)
{
    return Front.LockHot;
}


void SetLockIce(U8 mu8Lock )
{
    if( mu8Lock != LOCK && mu8Lock != UNLOCK )
    {
        return ;
    }

    Front.LockIce = mu8Lock;
    SaveEepromId( EEP_ID_LOCK_ICE );
}

U8 GetLockIce(void)
{
    return Front.LockIce;
}

// �? 추출 미사?�� ?���? ( ?���? �??�� )
U32 GetSystemUnusedTime(void)
{
    return Front.UnusedTime;
}

void ResetUnusedTime(void)
{
    Front.UnusedTime = 0UL;
}

void CountingUnusedTime(void)
{
    if( Front.UnusedTime <= 0xFFFFFFFFUL )
    {
        Front.UnusedTime++;
    }
}

U8 GetFotaReboot(void)
{
    return Front.Fota;
}

void SetFotaReboot(U8 mu8Fota)
{
    Front.Fota = mu8Fota;
}


U8 IsExpiredTestKeyLimitTimer(void)
{
    if( Front.LimitTimer != 0 )
    {
        return FALSE;   // no expired
    }
    return TRUE;  // expired!!
}


void UpdateTestKeyLimitTimer(void)
{
    if( Front.LimitTimer != 0 )
    {
        Front.LimitTimer--;
    }
}
