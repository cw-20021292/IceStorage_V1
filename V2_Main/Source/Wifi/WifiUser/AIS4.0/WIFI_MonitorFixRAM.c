
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_SetCommon.h"
#include "WIFI_UserInterface.h"
#include "WIFI_MonitorFixRAM.h"

/* USER */
#include "sound.h"
#include "front.h"
#include "eol.h"
#include "hw.h"
#include "fct.h"
#include "rtc.h"
#include "error.h"
#include "filter.h"
#include "flush_water_out.h"
#include "cold_water.h"
#include "ice.h"
#include "process_display.h"
#include "hot_water.h"
#include "hot_water_out.h"
#include "water_out.h"
#include "eeprom.h"
#include "temp.h"
#include "power_saving.h"
#include "energy.h"
#include "user.h"
#include "ice_door.h"
#include "uv.h"
#include "voice.h"
#include "service.h"
#include "smart_check.h"
#include "diagnosis.h"
#include "WIFI_SetFunctionData.h"
#include "comp.h"
#include "ice_full.h"

extern U8 A_NULL=0;

U8 gu8_Water_DrainLevel_Low = 0;
U8 gu8_Water_DrainLevel_high = 0;
U8 gu8_FAN_CMD = 0;
U8 gu8_COMP_CMD = 0;
// U8 gu8_Heater_CMD = 0;
U16 gu16_Cold_Temperature_One_Degree = 0;
U16 gu16_Room_Temperature_One_Degree = 0;
U16 gu16_Amb_Temperature_One_Degree = 0;
U16 gu16_EVA1_Temperature_One_Degree = 0;
U16 gu16_HOT_OUT_Temperature_One_Degree = 0;
U16 gu16_HOT_IN_Temperature_One_Degree = 0;
U16 gu16_HEATER_Temperature_One_Degree = 0;
// U8 gu8_Error_Code = 0;
// U8 gu8AltitudeStep =0;
U8 gu8IceFullStatus = 0;
U16 gu16IceMakeStatus = 0;
U16 gu16CompOffTime = 0;

static const WifiFixRamList_T WifiRamList[] =
{

    {A_U16_A701_COLD_TEMP,                  (U8*)(&gu16_Cold_Temperature_One_Degree),        TwoByte,        Read_Only     }, 
    {A_U16_A705_HOT_TEMP,                   (U8*)(&gu16_HOT_OUT_Temperature_One_Degree),     TwoByte,        Read_Only      }, 
    {A_U16_A708_OUT_TEMP,                   (U8*)(&gu16_Amb_Temperature_One_Degree),         TwoByte,        Read_Only     }, 
    {A_U16_A709_EVA_TEMP,                   (U8*)(&gu16_EVA1_Temperature_One_Degree),        TwoByte,        Read_Only     }, 
    {A_U16_FF_TRAY_IN_TEMP,                 (U8*)(&gu16_Room_Temperature_One_Degree),        TwoByte,        Read_Only     }, 
    {A_U16_A703_HOT_IN_TEMP,                (U8*)(&gu16_HOT_IN_Temperature_One_Degree),      TwoByte,        Read_Only     }, 
    {A_U16_A704_HEATER_TEMP,                (U8*)(&gu16_HEATER_Temperature_One_Degree),      TwoByte,        Read_Only     }, 
    {A_U8_AF04_COMP_OPERATION_STATUS,       (U8*)(&gu8_COMP_CMD),                            OneByte,        Read_Only     }, 
    {A_U8_AA01_FAN_STATUS,                  (U8*)(&gu8_FAN_CMD),                             OneByte,        Read_Only     }, 
    {A_U16_A401_DRAIN_TANK_LEV_LOW,         (U8*)(&gu8_Water_DrainLevel_Low),                OneByte,        Read_Only     }, 
    {A_U16_A402_DRAIN_TANK_LEV_HIGH,        (U8*)(&gu8_Water_DrainLevel_high),               OneByte,        Read_Only     }, 
    {A_U16_AC02_ICE_FULL_SENSOR,            (U8*)(&gu8IceFullStatus),                        OneByte,        Read_Only     }, 
    {A_U16_B001_ICEMAKE_STATUS,             (U8*)(&gu16IceMakeStatus),                       TwoByte,        Read_Only     }, 
    {A_U16_FF_COMP_ONOFF_TIME,              (U8*)(&gu16CompOffTime),                         TwoByte,        Read_Only     }, 

};

#define SZ_RAM_ADR_LIST ( sizeof(WifiRamList) / sizeof(WifiFixRamList_T) )
/*--------------------------------------------------*/
/*	Parameter : write Ram Address (server -> wifi)  */
/*	return : U8* write value(start address) (wifi -> mcu)           */
/*--------------------------------------------------*/
U8 *SetRamData (U16 mu16address)
{
    static U16 mu16Data = 0U;
    U8 *buf;
    switch ( mu16address )
    {    
        buf = (U8*)mu16address;     // fix ram value

    }
    return (U8*)(&mu16Data);
}

void GetRamData (void)
{
    __near U8 *pFixRamAddress;
    // U8 *TempPtrFix ;
    U8 *pTempPtr ;
    U8 *pTempPtrReturn ;
    U16 RamCheckAddress =0;
    U8 i=0;
    WifiWriteFun_T pWriteFunc;

    gu8_Water_DrainLevel_Low = (U8)(P_DRAIN_TANK_LOW);
    gu8_Water_DrainLevel_high = (U8)(P_DRAIN_TANK_HIGH);
    gu8_FAN_CMD = (U8)(P_FAN_MOTOR_1);
    gu8_COMP_CMD = (U8)(GetCompOnOff());
    gu8IceFullStatus = (U8)(GetIceFullStatus());
    gu16CompOffTime = (U16)(GetCompProtectOffTime());

    /*
    // MODE
    #define  ICE_MODE_NONE              0x0000
    #define  ICE_MODE_INIT              0x0001
    #define  ICE_MODE_PREHEAT           0x0002
    #define  ICE_MODE_READY             0x0004
    #define  ICE_MODE_ICING             0x0008
    #define  ICE_MODE_DEICING           0x0010
    #define  ICE_MODE_DUMMY_DEICING     0x0020
    #define  ICE_MODE_OFF               0x0040
    #define  ICE_MODE_OFF_LOAD          0x0080
    #define  ICE_MODE_DEICING_OFF       0x0100
    #define  ICE_MODE_TRAY_DEICING      0x0200
    */
    gu16IceMakeStatus = (U16)GetIceMakeMode();

    gu16_Amb_Temperature_One_Degree = (U16)((I16)GetTemp(TEMP_ID_AMBIENT));
    gu16_Room_Temperature_One_Degree = (U16)((I16)GetTemp(TEMP_ID_ROOM_WATER));
    gu16_Cold_Temperature_One_Degree = (U16)((I16)GetTemp(TEMP_ID_COLD_WATER));
    gu16_EVA1_Temperature_One_Degree = (U16)((I16)GetTemp(TEMP_ID_EVA_1));
    gu16_HOT_OUT_Temperature_One_Degree = (U16)((I16)GetTemp(TEMP_ID_HOT_OUT));
    gu16_HOT_IN_Temperature_One_Degree = (U16)((I16)GetTemp(TEMP_ID_HOT_IN));
    gu16_HEATER_Temperature_One_Degree = (U16)((I16)GetTemp(TEMP_ID_HEATER));



    for ( i = 0 ; i < SZ_RAM_ADR_LIST; i++ )
    {
        pFixRamAddress = ((__near U8*)WifiRamList[i].AddressList);
        pTempPtr = WifiRamList[i].TargetVariable;
        RamCheckAddress = (U16)(GetUserSystemRamWriteStatus((U16)(WifiRamList[i].AddressList)));
        if (pTempPtr == &A_NULL) 
        {
            RamCheckAddress = CLEAR_ADR;
            SetUserSystemRamWriteStatus((U16)RamCheckAddress); // clear
        }
        else {
            if (RamCheckAddress != CLEAR_ADR) // write check
            {
                pWriteFunc =  WifiRamList[i].WriteFunc;
                if ( pWriteFunc != Read_Only ) // write check
                {
                    pTempPtrReturn = (U8*)pWriteFunc((U16)(RamCheckAddress));
                    *(pTempPtr) = *(pTempPtrReturn); // 1byte write

                    if (WifiRamList[i].VarSize == TwoByte ) // 2byte write
                    {
                        pTempPtr++;
                        RamCheckAddress++;
                        pTempPtrReturn = (U8*)RamCheckAddress;
                        *(pTempPtr) = *(pTempPtrReturn);
                    }
                    RamCheckAddress = CLEAR_ADR;
                    SetUserSystemRamWriteStatus((U16)RamCheckAddress); // clear
                    break;
                }
                else 
                {
                    RamCheckAddress = CLEAR_ADR;
                    SetUserSystemRamWriteStatus((U16)RamCheckAddress); // clear
                }
            }
            else  // read
            { //1byte read
                *pFixRamAddress = *(WifiRamList[i].TargetVariable);

                if (WifiRamList[i].VarSize == TwoByte ) 
                { //2byte variable
                    pTempPtr = WifiRamList[i].TargetVariable;
                    pFixRamAddress++;
                    pTempPtr++;
                    *pFixRamAddress = *pTempPtr;
                }
            }
        }

    }
}

