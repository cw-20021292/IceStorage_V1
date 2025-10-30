#include "hw.h"
#include "hal_adc.h"



/* TEMP ON/OFF DELAY TIME */
#define TURN_ON_DELAY       1000UL       // 1000ms @1ms

/* MACRO ADC */
#define ADC_SET_CHANNEL(x)      do{ ADS = (x); }while(0)
#define ADC_START_CONVERSION()  do{ ADCS = 1; }while(0)
#define ADC_STOP_CONVERSION()   do{ ADCS = 0; }while(0)
#define ADC_SAMPLING()             while(!ADIF)
#define ADC_GET_VALUE()         (ADCR >> 6)

typedef void (*Action_T)(void);

typedef struct _adc_config_
{
    U8   id;        // ADC ID
    U8   channel;   // ADC CHANNEL
    U8   count;     // SUM COUNTER
    U16  min;       // FILTERING MIN VALUE
    U16  max;       // FILTERING MAX VALUE

    U8          OnOff;          // Channel on/off
    U16         TurnOnDelay;    // Adc Channel On delay
    Action_T    pfEnable;       // Adc Channel On Function
    Action_T    pfDisable;      // Adc Channel Off Function
} TAdcConfig;

static TAdcConfig adcConfList[ ANI_MAX_NUM ] = 
{ 
    /* ADC CHANNEL,         CH COUNTER,    MAX ,    MIN      On/Off  On Delay   pfEnable  pfDislabe */
    { ANI_UV_WATER_FB,      0,  52U,       1024U,      0U,     ON,     0U,        NULL,     NULL},
    { ANI_UV_ICE_FB,        1,  52U,       1024U,      0U,     ON,     0U,        NULL,     NULL},
}; 


typedef struct _sensor_
{
    U16  sum;        
    U8   sum_count; 

    U16  average;  

    U16  min;        
    U16  max;       
} sensor_t;
sensor_t        adcValue[ ANI_MAX_NUM ];


static U16 SampleAdc( U8 channel );
static void Sum( void );
static void Average( void );


void HAL_InitAdc(void)
{
    U8 i;
    U8 ch;

    // 초기화 값?? ??정????
    for( i = 0; i < ANI_MAX_NUM ; i++ )
    {
        ch       = adcConfList[ i ].channel;
        adcValue[ i ].average = SampleAdc( ch );
    }

}

U16 HAL_GetAdcValue( U8 mu8Id )
{
    if( mu8Id >= ANI_MAX_NUM )
    {
        return 0;
    }

    return adcValue[ mu8Id ].average;
}


void HAL_ProcessAdc(void)
{
    Sum();
    Average();
}

static U16 SampleAdc( U8 channel )
{
    U16     raw = 0;


    /* Stop */
    ADIF = 0;
    ADC_STOP_CONVERSION();

    /* Set channel */
    ADC_SET_CHANNEL( channel );

    /* Start adc */
    R_ADC_Set_OperationOn();
    ADC_START_CONVERSION();

    /* Drop garbage value */
    ADC_SAMPLING();
    raw = ADC_GET_VALUE();

    /* Get real value */
    ADC_SAMPLING();
    raw = ADC_GET_VALUE();

    /* Stop */
    ADC_STOP_CONVERSION();
    R_ADC_Set_OperationOff();


    return raw;
}

static void InitAdcValue(U8 mu8Id)
{
    adcValue[ mu8Id ].sum         = 0;
    adcValue[ mu8Id ].sum_count   = 0;
    adcValue[ mu8Id ].min         = adcConfList[ mu8Id ].min;
    adcValue[ mu8Id ].max         = adcConfList[ mu8Id ].max;
}

static void Sum( void )
{
    U8  i;
    U8  mu8Ch;
    U8   mu8Count;
    U16  mu16Val;

    for( i = 0; i < ANI_MAX_NUM ; i++ )
    {
#if !CONFIG_TEST_TEMP_READ
        if( adcConfList[ i ].OnOff == OFF )
        {
            // 초기화
            if( adcConfList[ i ].pfDisable != NULL )
            {
                // PORT를 DISABLE하고 나서는 지연 시간을 초기화 한다.
                adcConfList[ i ].pfDisable();
                adcConfList[ i ].TurnOnDelay = TURN_ON_DELAY;
            }

            InitAdcValue( i );
        }
        else
#endif
        {
            // 센싱 
            if( adcConfList[ i ].pfEnable != NULL )
            {
                adcConfList[ i ].pfEnable();

#if !CONFIG_TEST_TEMP_READ
                // PORT를 ENABLE하고, 센서 값이 안정화 될 때가지 지연시간을 둔다.
                if( adcConfList[ i ].TurnOnDelay != 0 )
                {
                    adcConfList[ i ].TurnOnDelay--;
                    continue ;
                }
#endif
            }

            mu8Count = adcValue[ i ].sum_count;
            if( mu8Count < adcConfList [ i ].count )
            {
                mu8Ch       = adcConfList[ i ].channel;
                mu16Val     = SampleAdc( mu8Ch ); 

                adcValue[ i ].sum += mu16Val;
                adcValue[ i ].sum_count++;

                // Sampling min/max value
                if( mu16Val < adcValue[ i ].min )
                {
                    adcValue[ i ].min = mu16Val;
                }

                if( mu16Val > adcValue[ i ].max )
                {
                    adcValue[ i ].max = mu16Val;
                }
            }
        }
    }
}


static void Average( void )
{
    U8   i;
    U8   count;
    U16 average;


    for( i = 0; i < ANI_MAX_NUM ; i++ )
    {
        count = adcValue[ i ].sum_count;
        if(  adcConfList[ i ].count == count )
        {

            adcValue[ i ].sum = adcValue[ i ].sum - ( adcValue[ i ].min + adcValue[ i ].max );
            average = adcValue[ i ].sum / (count - 2);

            adcValue[ i ].average     = average;

            InitAdcValue( i );
        }
    }
}
