#include "hw.h"
#include "monitoring.h"
#include "hal_adc.h"
#include "hal_pump.h"
#include "hal_uv.h"
#include "heater_out.h"



static U8 GasSwitchVavleOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STEP_MOTOR_GAS_SW_1() == HIGH
            || GET_STEP_MOTOR_GAS_SW_2() == HIGH
            || GET_STEP_MOTOR_GAS_SW_3() == HIGH
            || GET_STEP_MOTOR_GAS_SW_4() == HIGH )
    {
        OnOffStatus = ON; 
    }

    if( GET_STATUS_SMPS_12V() == HIGH )
    {
        OnOffStatus = OFF; 
    }

    return OnOffStatus;
}


static U8 NosValveOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_VALVE_NOS() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}


static U8 HotOutputValveOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_VALVE_HOT_OUT() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 RoomOutputValveOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_VALVE_ROOM_COLD_OUT() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 ColdOutputValveOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_VALVE_COLD_OUT() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 HotInputValveOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_VALVE_HOT_IN() == HIGH )
    {
       OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 HotDrainValveOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_VALVE_HOT_DRAIN() == HIGH )
    {
       OnOffStatus = ON; 
    }

    return OnOffStatus;
}



static U8 ColdAirValveOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_VALVE_COLD_AIR() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 ColdInputValveOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_VALVE_COLD_IN() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 ColdDrainValveOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_VALVE_COLD_DRAIN() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}


static U8 IceTrayInputValveOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_VALVE_ICE_TRAY_IN() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 FlushValveOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_VALVE_COLD_FLUSH() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 FlowMotorOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STEP_MOTOR_FLOW_VALVE_1() == HIGH
            || GET_STEP_MOTOR_FLOW_VALVE_2() == HIGH
            || GET_STEP_MOTOR_FLOW_VALVE_3() == HIGH
            || GET_STEP_MOTOR_FLOW_VALVE_4() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 IceDoorOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STEP_MOTOR_ICE_DOOR_1() == HIGH
            || GET_STEP_MOTOR_ICE_DOOR_2() == HIGH
            || GET_STEP_MOTOR_ICE_DOOR_3() == HIGH
            || GET_STEP_MOTOR_ICE_DOOR_4() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 DrainPumpOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( HAL_IsTurnOnDrainPump() == TRUE )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}




static U8 SwingBarOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_SWING_BAR() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 FanOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_FAN_MOTOR_1_ONOFF() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 HotHeater_1_OnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_HEATER_RELAY() == HIGH 
            && GET_STATUS_HEATER_TRIAC() == LOW )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}

static U8 HotHeater_2_OnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_HEATER_RELAY() == LOW 
            && GET_STATUS_HEATER_TRIAC() == HIGH )
    {
        OnOffStatus = ON; 
    }

    return OnOffStatus;
}



static U8 UvWaterOutOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( HAL_GetUvOnOffStatus( HAL_UV_WATER_OUT  ) == HIGH )
    {
        OnOffStatus = ON;
    }

    if( GET_STATUS_SMPS_12V() == HIGH )
    {
        OnOffStatus = OFF; 
    }

    return OnOffStatus;
}

static U8 UvIceDoorOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( HAL_GetUvOnOffStatus( HAL_UV_ICE_DOOR ) == HIGH )
    {
        OnOffStatus = ON;
    }

    if( GET_STATUS_SMPS_12V() == HIGH )
    {
        OnOffStatus = OFF; 
    }

    return OnOffStatus;
}

static U8 UvIceTankOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( HAL_GetUvOnOffStatus( HAL_UV_ICE_TANK ) == HIGH )
    {
        OnOffStatus = ON;
    }

    if( GET_STATUS_SMPS_12V() == HIGH )
    {
        OnOffStatus = OFF; 
    }

    return OnOffStatus;
}

static U8 UvIceTrayOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( HAL_GetUvOnOffStatus( HAL_UV_ICE_TRAY ) == HIGH )
    {
        OnOffStatus = ON;
    }

    if( GET_STATUS_SMPS_12V() == HIGH )
    {
        OnOffStatus = OFF; 
    }

    return OnOffStatus;
}

static U8 IceFullOnOff(void)
{
    U8 OnOffStatus = OFF;

    if( GET_STATUS_ICE_INFRARED() == HIGH )
    {
        OnOffStatus = ON;
    }

    return OnOffStatus;
}


typedef U8 (*OnOff_T)(void);
typedef struct _monitoring_
{
    E_API_A1080_T Id;
    U8 AdcId;
    InputSrc Src;
    U8 OnOffStatus;
    U16 Adc;
    U16 Current;
    OnOff_T pFOnOff;
} Monitoring_T;

Monitoring_T MonitoringList[] =
{
    // 12V Load
    {WIFI_PART_0808_ICE_COURSE_CHANGE_VV, ANI_12V_FB, INPUT_SRC_12V,  0,  INIT_DATA,  INIT_DATA,  GasSwitchVavleOnOff},

    // 24V Load
    {WIFI_PART_0001_VALVE_NOS,          ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  NosValveOnOff},
    {WIFI_PART_0002_VALVE_HOT_OUT,      ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  HotOutputValveOnOff},
    {WIFI_PART_0003_VALVE_PURE_OUT,     ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  RoomOutputValveOnOff},
    {WIFI_PART_0004_VALVE_COLD_OUT,     ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  ColdOutputValveOnOff},

    {WIFI_PART_0009_VALVE_HOT_IN,       ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  HotInputValveOnOff},
    {WIFI_PART_000A_VALVE_HOT_DRAIN,    ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  HotDrainValveOnOff},
    {WIFI_PART_000B_VALVE_COLD_AIR,     ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  ColdAirValveOnOff},
    {WIFI_PART_000C_VALVE_COLD_IN,      ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  ColdInputValveOnOff},

    {WIFI_PART_000D_VALVE_COLD_DRAIN,   ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  ColdDrainValveOnOff},
    {WIFI_PART_0015_VALVE_ICE_TRAY_IN,  ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  IceTrayInputValveOnOff},
    {WIFI_PART_0018_VALVE_FLUSHING,     ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  FlushValveOnOff},

    {WIFI_PART_0304_HOT_FLOW_MOTOR,     ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  FlowMotorOnOff},

    {WIFI_PART_0802_ICE_SWING_BAR,      ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  SwingBarOnOff},
    {WIFI_PART_0807_ICE_DOOR_STEPMOTOR, ANI_24V_FB, INPUT_SRC_24V,  0,  INIT_DATA,  INIT_DATA,  IceDoorOnOff},

    // Feedback Load
    {WIFI_PART_0204_COLD_FAN,              ANI_DC_FAN,          INPUT_SRC_FEEDBACK,  0,  INIT_DATA,  INIT_DATA,  FanOnOff},
    {WIFI_PART_0302_HOT_INSTANT_HEATER_1,  ANI_HEATER_FB,       INPUT_SRC_FEEDBACK,  0,  INIT_DATA,  INIT_DATA,  HotHeater_1_OnOff},
    {WIFI_PART_0303_HOT_INSTANT_HEATER_2,  ANI_HEATER_FB,       INPUT_SRC_FEEDBACK,  0,  INIT_DATA,  INIT_DATA,  HotHeater_2_OnOff},

    {WIFI_PART_0501_STER_UV_FAUCET,        ANI_UV_WATER_OUT,    INPUT_SRC_FEEDBACK,   0,  INIT_DATA,  INIT_DATA,  UvWaterOutOnOff},
    {WIFI_PART_0502_STER_UV_FAUCET_ICE,    ANI_UV_ICE_DOOR,     INPUT_SRC_FEEDBACK,   0,  INIT_DATA,  INIT_DATA,  UvIceDoorOnOff},
    {WIFI_PART_0503_STER_UV_ICE_TANK,      ANI_UV_ICE_TANK,     INPUT_SRC_FEEDBACK,   0,  INIT_DATA,  INIT_DATA,  UvIceTankOnOff},
    {WIFI_PART_0504_STER_UV_ICE_TRAY,      ANI_UV_ICE_TRAY,     INPUT_SRC_FEEDBACK,   0,  INIT_DATA,  INIT_DATA,  UvIceTrayOnOff},

    {WIFI_PART_080A_ICE_DRAIN_PUMP,        ANI_PUMP_DRAIN_FB,   INPUT_SRC_FEEDBACK,   0,  INIT_DATA,  INIT_DATA,  DrainPumpOnOff},

    // Sensor
    {WIFI_PART_0801_ICE_FULL_SENSOR,       ANI_SENSOR_INFRARED, INPUT_SRC_SENSOR,   0,  INIT_DATA,  INIT_DATA,  IceFullOnOff}
};

#define SZ_MONITORING_LIST  (sizeof(MonitoringList)/sizeof(Monitoring_T))
static U8 FindIndexMonitoringList( E_API_A1080_T id, U8 *pIndex )
{
    U8 i = 0; 

    for( i = 0; i < SZ_MONITORING_LIST; i++ )
    {
        if( MonitoringList[ i ].Id == id )
        {
            if( pIndex != NULL )
            {
                *pIndex = i;
            }

            return TRUE;
        }   
    }

    return FALSE;
}

void ClearLoadAdc(U8 Src)
{
    U8 i=0;

    for(i=0; i<SZ_MONITORING_LIST; i++)
    {
        if(MonitoringList[i].Src == Src)
        {
            MonitoringList[i].Adc = INIT_DATA;
        }
    }    
}

void ClearLoadCurrent(U8 Src)
{
    U8 i=0;

    for(i=0; i<SZ_MONITORING_LIST; i++)
    {
        if(MonitoringList[i].Src == Src)
        {
            MonitoringList[i].Current = INIT_DATA;
        }
    }    
}


U16 GetLoadAdc(E_API_A1080_T Id)
{
    U16 mu16Adc = 0;
    U8 Index = 0;

    if( FindIndexMonitoringList( Id, &Index ) == TRUE )
    {
        mu16Adc = MonitoringList[Index].Adc;
    }

    return mu16Adc;
}

void SetLoadAdc(E_API_A1080_T Id, U16 mu16Adc)
{
    U8 Index = 0;

    if( FindIndexMonitoringList( Id, &Index ) == TRUE )
    {
        MonitoringList[Index].Adc = mu16Adc;
    }
}


U16 GetLoadCurrent(E_API_A1080_T Id)
{
    U16 mu16Current = 0;
    U8 Index = 0;

    if( FindIndexMonitoringList( Id, &Index ) == TRUE )
    {
        mu16Current = MonitoringList[Index].Current;
    }

    return mu16Current;
}

void SetLoadCurrent(E_API_A1080_T Id, U16 mu16Current)
{
    U8 Index = 0;

    if( FindIndexMonitoringList( Id, &Index ) == TRUE )
    {
        MonitoringList[Index].Current = mu16Current;
    }
}


U8 GetLoadPowerOnOffStatus(U8 Src)
{
    U8 i = 0;
    U8 OnOffCnt = 0;
    Monitoring_T *pList = NULL;

    for(i=0; i<SZ_MONITORING_LIST; i++)
    {
        pList = &MonitoringList[i];

        if( pList->Src == Src )
        {
            OnOffCnt += pList->OnOffStatus;
        }
    }

    return OnOffCnt;
}


static U16 CalcCurrent( U8 Src, E_API_A1080_T Id, U16 mu16Adc )
{
    F32 mf32Coefficient = 0.0f;
    F32 mf32Current = 0.0f;

    if( Src == INPUT_SRC_12V )
    {
        mf32Coefficient = 2.37f;
    }
    else if( Src == INPUT_SRC_24V )
    {
        mf32Coefficient  = 2.37f;
    }
    else if( Src == INPUT_SRC_AC )
    {
        mf32Coefficient = 2.37f;    // ë¯¸ì • (?…Œ?Š¤?Š¸ì¤?)
    }
    else if( Src == INPUT_SRC_FEEDBACK )
    {
        if( Id == WIFI_PART_0209_COLD_ROTATE_PUMP)
        {
            mf32Coefficient = 0.444f;
        }
        else if( Id == WIFI_PART_080A_ICE_DRAIN_PUMP)
        {
            mf32Coefficient = 0.444f;
        }
        else if( Id == WIFI_PART_0204_COLD_FAN)
        {
            mf32Coefficient = 0.488f;
        }
        else if( Id == WIFI_PART_0500_STER_CSI_MODULE)
        {
            mf32Coefficient = 1.110f;
        }
        else if( Id == WIFI_PART_0501_STER_UV_FAUCET)
        {
            mf32Coefficient = 0.488f;      
        }
        else if( Id == WIFI_PART_0502_STER_UV_FAUCET_ICE )
        {
            mf32Coefficient = 0.488f;       
        }
        else if( Id ==  WIFI_PART_0503_STER_UV_ICE_TANK)
        {
            mf32Coefficient = 0.488f;
        }
        else if( Id == WIFI_PART_0504_STER_UV_ICE_TRAY)
        {
            mf32Coefficient = 0.488f;
        }
        else if( Id == WIFI_PART_0302_HOT_INSTANT_HEATER_1
                ||  Id == WIFI_PART_0303_HOT_INSTANT_HEATER_2)
        {
            return mu16Adc;
            //return CalcHeaterAdc2Watt( mu16Adc );
        }

    }
    else
    {
        return mu16Adc;
    }

    mf32Current = (F32)mu16Adc * mf32Coefficient;
    mf32Current += 0.5f;
    return (U16)(mf32Current);
}

U8 Debug12vCnt = 0;
U8 Debug24vCnt = 0;
U8 DebugAcCnt = 0;
U8 DebugFeedbackCnt = 0;
U8 DebugSensorCnt = 0;

U16 DebugAdc = 0;
U16 DebugCurrent = 0;
void ControlMonitoring(void)
{
    U8 i = 0;
    Monitoring_T *pList = NULL;


    for(i=0; i<SZ_MONITORING_LIST; i++)
    {
        pList = &MonitoringList[i];

        pList->OnOffStatus =  pList->pFOnOff();
    }


    for(i=0; i<SZ_MONITORING_LIST; i++)
    {
        pList = &MonitoringList[i];

        if( pList->OnOffStatus == HIGH )
        {
            if( pList->Src <= INPUT_SRC_AC)
            {
                if(GetLoadPowerOnOffStatus(pList->Src) != 1)
                {
                    continue;
                }
            }

            pList->Adc = HAL_GetAdcValue(pList->AdcId);
            pList->Current = CalcCurrent(pList->Src, pList->Id, pList->Adc);
        }
    }

    // for dbg..
    Debug12vCnt         = GetLoadPowerOnOffStatus(INPUT_SRC_12V);
    Debug24vCnt         = GetLoadPowerOnOffStatus(INPUT_SRC_24V);
    DebugAcCnt          = GetLoadPowerOnOffStatus(INPUT_SRC_AC);
    DebugFeedbackCnt    = GetLoadPowerOnOffStatus(INPUT_SRC_FEEDBACK);
    DebugSensorCnt      = GetLoadPowerOnOffStatus(INPUT_SRC_SENSOR);

    DebugAdc            = GetLoadAdc(WIFI_PART_0809_ICE_HOT_GAS_VV);
    DebugCurrent        = GetLoadCurrent(WIFI_PART_0809_ICE_HOT_GAS_VV);
}

