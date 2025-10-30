
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_UserInterface.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_SetSensorData.h"
#include "WIFI_SetErrorData.h"
#include "WIFI_SetPartData.h"
#include "WIFI_SetAccumulateData.h"
#include "WIFI_SetInformationData.h"
#include "WIFI_Status.h"


#ifdef MODEL_NO_1
#include "WIFI_UserInterface_IconAIS.h"
#endif

#ifdef MODEL_NO_2
#include "WIFI_UserInterface_BuiltIn.h"
#endif

#ifdef MODEL_NO_3
#include "WIFI_UserInterface_Icon2_0.h"
#endif

#ifdef MODEL_NO_4
#include "WIFI_UserInterface_Hidden.h"
#endif

typedef struct _interface_
{
    /* EEp Read / Write */
    void (*UserWriteEep)( U16 mAddr, U8 mVal );
    U8 (*UserReadEep)( U16 mAddr );
    
    /* Buzzer / User Interface Data */
    void (*SetUserSound)( U8 mBuzz );
    U8 (*GetUserStatus) ( U8 mVal );
    void (*SetUserDisplay)( U8 mVal );
    void (*ClearUserData) ( void );

    /* User Data Matching */
    U32 (*GetUserA9010Func)( U16 mVal, U16 mLen);
    U16 (*GetUserA1010Func)( U16 mVal );
    I16 (*GetUserA1020Sens)( U16 mVal );
    U16 (*GetUserA1030Err)( U16 mVal );
    U16 (*GetUserA1080Part)( U16 mVal );
    U32 (*GetUserA2010Acc)( U16 mVal );

    /* App Control -> User System Function Data */
    void (*SetUserA1010Func)( U16 mId, U16 mVal );

    /* RAM Monitor */
    void (*SetUserA9010Func)( U16 mId, I8 *mVal );
    
    /* Time Setting */
    void (*SetUserTime)( U16 mYear, U16 mMonth, U16 mDate, U16 mHour, U16 mMin, U16 mSec, U16 mDay );
}InterFaceWIFI_T;

InterFaceWIFI_T interface;

static void InitInterfaceWIFI ( InterFaceWIFI_T *pInterface );

/* User Function Matching */ 
void InitializeWifiUserInterface ( void )
{
    InterFaceWIFI_T  mInterface;

    mInterface.UserWriteEep = UserWriteEep;
    mInterface.UserReadEep = UserReadEep;
    
    mInterface.SetUserSound = UserBuzzSound;
    mInterface.GetUserStatus = GetUserSystemStatus;
    mInterface.SetUserDisplay = SetUserSystemDisplay;
    mInterface.ClearUserData = ClearUserSystemData; 
    
    mInterface.GetUserA9010Func = GetUserSystemRamMonitor;
    mInterface.GetUserA1010Func = GetUserSystemFunction;
    mInterface.GetUserA1020Sens = GetUserSystemSensor;
    mInterface.GetUserA1030Err = GetUserSystemError;
    mInterface.GetUserA1080Part = GetUserSystemPart;
    mInterface.GetUserA2010Acc = GetUserSystemAccumulate;
    
    mInterface.SetUserA1010Func = SetUserSystemFunction;

    mInterface.SetUserA9010Func = SetUserSystemRamMonitor;

    mInterface.SetUserTime = SetUserSystemTime;
    
    InitInterfaceWIFI( &mInterface );
}

static void InitInterfaceWIFI ( InterFaceWIFI_T *pInterface )
{
    interface.UserWriteEep = pInterface->UserWriteEep;
    interface.UserReadEep = pInterface->UserReadEep;
    
    interface.SetUserSound = pInterface->SetUserSound;
    interface.GetUserStatus = pInterface->GetUserStatus;
    interface.SetUserDisplay = pInterface->SetUserDisplay;
    interface.ClearUserData = pInterface->ClearUserData;

    interface.GetUserA1010Func = pInterface->GetUserA1010Func;
    interface.GetUserA1020Sens = pInterface->GetUserA1020Sens;
    interface.GetUserA1030Err = pInterface->GetUserA1030Err;
    interface.GetUserA1080Part = pInterface->GetUserA1080Part;
    interface.GetUserA2010Acc = pInterface->GetUserA2010Acc;
    interface.GetUserA9010Func = pInterface->GetUserA9010Func;

    interface.SetUserA1010Func = pInterface->SetUserA1010Func;
    interface.SetUserA9010Func = pInterface->SetUserA9010Func;
    
    interface.SetUserTime = pInterface->SetUserTime;
}

void WifiWriteEep ( U16 mAddr, U8 mVal )
{
    interface.UserWriteEep(mAddr, mVal);
}

U8 WifiReadEep ( U16 mAddr )
{
    U8 mu8Data = 0U;
    mu8Data = interface.UserReadEep(mAddr);
    return mu8Data;
}

void SetWifiBuzzSound ( U8 mBuzz )
{
    if ( GetWifiFotaStatus( STATUS_FOTA_MCU ) == CLEAR )
    {
        interface.SetUserSound(mBuzz);
    }
}

/* User System Status */
U8 GetUserInterface ( U8 mVal )
{
    /*
    mVal = USER_MODEL_TYPE;
        0 : CHP
        1 : CP
        4 : HP
    mVal = USER_FACTORY_STATUS;
        0 CLEAR : Normal
        1 SET : Factory/PCB Test mode
    mVal = USER_RX_STATUS;
        0 FALSE : App Control X
        1 TRUE : App Control O
    mVal = USER_SMART_POSSIBLE_STATUS;
        0 FALSE : Smart Check X
        1 TRUE : Smart Check O
    mVal = USER_ERROR_STATUS;
        0 FALSE : Normal
        1 TRUE : Error
    mVal = USER_FOTA_STATUS;
        0 FALSE : Fota X
        1 TRUE : Fota O
    mVal = USER_SMART_SENSING_STATUS;
        0 FALSE : Normal
        1 TRUE : Smart Checking
    */
    U8 mu8Return = 0U;
    mu8Return = interface.GetUserStatus(mVal); 
    return mu8Return; 
}

/* User Display Setting */
void SetUserDisplay ( U8 mDisplay )
{
    if ( mDisplay == USER_DISPLAY_FOTA )
    {
        WifiWriteEep(EEPROM_ADDR_FOTA_MCU,SET);
    }
    else if ( mDisplay == USER_DISPLAY_SELFCHECK )
    {

    }
    else if ( mDisplay == USER_DISPLAY_PARTCHECK )
    {

    }
    else
    {

    }
    interface.SetUserDisplay(mDisplay);
}

/* User Data Clear */
void ClearUserData ( void ) 
{
    interface.ClearUserData();
}

U32 GetUserMonitorData ( U16 mu16Func, U16 mu16Len )
{
    U32 mu32Data = 0U;
    mu32Data = interface.GetUserA9010Func(mu16Func, mu16Len);
    return mu32Data;
}

U16 GetUserFunctionData ( U16 mu16Func )
{
    U16 mu16Data = 0U;
    mu16Data = interface.GetUserA1010Func(mu16Func);
    return mu16Data;
}

I16 GetUserSensorData ( U16 mu16Sen )
{ // 센서 데이터
    I16 mi16Data = 0;
    mi16Data = interface.GetUserA1020Sens(mu16Sen);
    return mi16Data;
}

U16 GetUserErrorData ( U16 mu16Err )
{
    U16 mu16Data = 0U;
    mu16Data = interface.GetUserA1030Err(mu16Err);
    return mu16Data;
}

U16 GetUserPartData ( U16 mu16Part )
{
    U16 mu16Data = 0U;
    mu16Data = interface.GetUserA1080Part(mu16Part);
    return mu16Data;
}

U32 GetUserAccumulateData ( U16 mu16Acc )
{
    U32 mu32Data = 0U;
    mu32Data = interface.GetUserA2010Acc(mu16Acc);
    return mu32Data;
}

void SetUserFunctionData ( U16 mu16Func, U16 mData )
{
    interface.SetUserA1010Func(mu16Func,mData);
}

void SetUserMonitorData ( U16 mu16address, I8 *mData )
{
    interface.SetUserA9010Func(mu16address, mData);
}

void SetUserTimeSetting ( U16* pData )
{
    interface.SetUserTime(pData[WIFI_INFO_0000_YEAR],pData[WIFI_INFO_0001_MONTH],pData[WIFI_INFO_0002_DATE],
        pData[WIFI_INFO_0003_HOUR],pData[WIFI_INFO_0004_MINUTE],pData[WIFI_INFO_0005_SECOND],pData[WIFI_INFO_0006_DAY]);
}

void StartFotaOperation ( void )
{
    BOOT_setOTARequest();
}


