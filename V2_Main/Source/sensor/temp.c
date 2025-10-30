#include "hal_adc.h"
#include "temp.h"
#include "temp_table.h"


typedef TEMP_T(*Action_T)(U16 mu16Adc);
typedef struct _temp_list_
{
    U8 Id;                  // Analoge Channel Id 
    Action_T ConvAdc2Temp;  // Convert raw adc to temperture

    U8 OverrideFlag;        // Override Flag
    TEMP_T OverrideTemp;    // Overrided Temperture Value

    U32 TimeOut;            // @
} TempList_T;

TempList_T TempList[] = 
{
    { ANI_TEMP_AMBIENT,         ConvAdc2Temp_Ambient,    FALSE,  24.0f,     0 },
    { ANI_TEMP_ROOM_WATER,      ConvAdc2Temp_IceTrayIn,  FALSE,  24.0f,     0 },
    { ANI_TEMP_COLD_WATER,      ConvAdc2Temp_ColdWater,  FALSE,  24.0f,     0 },
    { ANI_TEMP_EVA_1,           ConvAdc2Temp_Eva,        FALSE,  24.0f,     0 },
    { ANI_TEMP_HOT_OUT,         ConvAdc2Temp_HotOut,     FALSE,  24.0f,     0 },
    { ANI_TEMP_HOT_IN,          ConvAdc2Temp_HotIn,      FALSE,  24.0f,     0 },
    { ANI_TEMP_HEATER,          ConvAdc2Temp_HotOut,     FALSE,  24.0f,     0 }
};
#define	SZ_LIST		( sizeof( TempList ) / sizeof( TempList_T ))

static U8 ConvAdc2Temp( U8 mu8Id, U16 mu16AdcRaw, TEMP_T *pTemp )
{
    if( pTemp == NULL )
    {
        return FALSE;
    }

    if( TempList[ mu8Id ].ConvAdc2Temp != NULL )
    {
        *pTemp = TempList[ mu8Id ].ConvAdc2Temp( mu16AdcRaw );

        return TRUE;
    }

    return FALSE;
}

TEMP_T GetTemp(U8 mu8Id )
{
    U16  mu16Adc = 0U;
    TEMP_T  mTemp = 0.0f;


    /* Out of range */
    if( mu8Id >= TEMP_ID_MAX )
    {
        return 0.0f;
    }

    /* Get Adc value */
    mu16Adc = HAL_GetAdcValue( TempList[ mu8Id ].Id );

    /* Conv Adc to Temperture */
    ConvAdc2Temp( mu8Id, mu16Adc, &mTemp );

#if CONFIG_TEST_COLD_TEMP_UP
    if( mu8Id == TEMP_ID_COLD_WATER )
    {
        mTemp += 0.5f;
    }
#elif CONFIG_TEST_COLD_TEMP_DOWN
    if( mu8Id == TEMP_ID_COLD_WATER )
    {
        mTemp -= 0.5f;
    }
#endif

#if CONFIG_TEST_AMBI_TEMP_UP
    if( mu8Id == TEMP_ID_AMBIENT )
    {
        mTemp += 0.4f;
    }
#elif CONFIG_TEST_AMBI_TEMP_DOWN
    if( mu8Id == TEMP_ID_AMBIENT )
    {
        mTemp -= 0.4f;
    }
#endif

    /* Override Temp */
    if( TempList[ mu8Id ].OverrideFlag == TRUE )
    {
        return TempList[ mu8Id ].OverrideTemp;
    }

    return mTemp;
}

U16 GetTempSensorTime(U8 mu8Id)
{
    return (U16)( TempList[ mu8Id ].TimeOut / 10UL );
}

// 현재 시간 보다 큰 경우에는 업데이트 해준다.
void TurnOnTempSensor(U8 mu8Id, U16 mu16TimeOut)
{
    U32 mu32Time;


    mu32Time = (U32)mu16TimeOut * 10UL;
    if( mu32Time > TempList[ mu8Id ].TimeOut )
    {
        HAL_TurnOnSensingAdc( TempList[ mu8Id ].Id );
        TempList[ mu8Id ].TimeOut = mu32Time;
    }
}

void TurnOffTempSensor(U8 mu8Id)
{
    HAL_TurnOffSensingAdc( TempList[ mu8Id ].Id );
    TempList[ mu8Id ].TimeOut = 0;
}


// Override
void SetOverrideTemp(U8 mu8Id, U8 mOverride, TEMP_T mTemp )
{
    TempList[ mu8Id ].OverrideFlag = mOverride; 
    TempList[ mu8Id ].OverrideTemp = mTemp; 

}

TEMP_T GetOverrideTemp(U8 mu8Id, U8 mOverride )
{
    return TempList[ mu8Id ].OverrideTemp = mOverride; 
}

TEMP_T  gTempRoom = 24.0f;
TEMP_T  gTempCold = 24.0f;
TEMP_T  gTempAmbi = 24.0f;
TEMP_T  gTempEva1 = 24.0f;
TEMP_T  gTempHotIn = 24.0f;
TEMP_T  gTempHotOut = 24.0f;
TEMP_T  gTempHeater = 24.0f;

U16     gAdcLeak        = 0;
U16     gAdcIceFull     = 0;
U16     gAdcUv_1        = 0;
U16     gAdcUv_2        = 0;
U16     gAdcHeater      = 0;
U16     gAdcDrain       = 0;
U16     gAdcFan         = 0;

U16     gAdc24_h       = 0;
U16     gAdc12_h       = 0;


void ProcessTemp(void)
{
    U8 i;

    for( i = 0; i < SZ_LIST; i++ )
    {
        if( TempList[ i ].TimeOut != 0UL )
        {
            TempList[ i ].TimeOut--;
            if( TempList[ i ].TimeOut == 0UL )
            {
                TurnOffTempSensor( i );
            }
        }
    }
    
    // FOR DEBUGGING...
    gTempRoom = GetTemp( TEMP_ID_ROOM_WATER );
    gTempCold = GetTemp( TEMP_ID_COLD_WATER );
    gTempAmbi = GetTemp( TEMP_ID_AMBIENT );
    gTempEva1 = GetTemp( TEMP_ID_EVA_1 );
    gTempHotOut = GetTemp( TEMP_ID_HOT_OUT );
    gTempHotIn = GetTemp( TEMP_ID_HOT_IN );
    gTempHeater = GetTemp( TEMP_ID_HEATER );

    gAdcFan     = HAL_GetAdcValue( ANI_DC_FAN );
    gAdcLeak    = HAL_GetAdcValue( ANI_SENSOR_LEAK );
    gAdcIceFull = HAL_GetAdcValue( ANI_SENSOR_INFRARED );
    gAdcUv_1     = HAL_GetAdcValue( ANI_UV_ICE_TRAY );
    gAdcUv_2     = HAL_GetAdcValue( ANI_UV_ICE_TANK );
    gAdcHeater  = HAL_GetAdcValue( ANI_HEATER_FB );
    gAdcDrain   = HAL_GetAdcValue( ANI_PUMP_DRAIN_FB );

    gAdc24_h = HAL_GetAdcValue( ANI_24V_FB );
    gAdc12_h = HAL_GetAdcValue( ANI_12V_FB );
}

