#include "hw.h"
#include "hal_input.h"



static U8   HAL_GetFilter(void);
static U8   HAL_GetTankOpen(void);
static U8   HAL_GetTankOpen_1(void);
static U8   HAL_GetTankOpen_2(void);
//static U8   HAL_GetHeaterPBA(void);
static U8   HAL_GetLevelDrainHigh(void);
static U8   HAL_GetLevelDrainLow(void);


typedef U8 (*Action_T)(void);
typedef struct _input_config_
{
    U8  prevVal; 
    U8  curVal;

    U16 delay;
    U16 setupDelay;

    Action_T    pfGetStatus;    
} InputConfig_T;

//static InputConfig_T inputList[ IN_MAX_NUM ] = 
InputConfig_T inputList[ IN_MAX_NUM ] = 
{ 
    { 0,      0,      INPUT_DELAY_1SEC,    INPUT_DELAY_1SEC,     HAL_GetFilter    },

    { 0,      0,      INPUT_DELAY_1SEC,    INPUT_DELAY_1SEC,     HAL_GetTankOpen  },
    { 0,      0,      INPUT_DELAY_100MS,   INPUT_DELAY_100MS,    HAL_GetTankOpen  },

    { 0,      0,      INPUT_DELAY_100MS,   INPUT_DELAY_1SEC,     HAL_GetTankOpen_1  },
    { 0,      0,      INPUT_DELAY_100MS,   INPUT_DELAY_100MS,    HAL_GetTankOpen_2  },

    { 0,      0,      INPUT_DELAY_1SEC,    INPUT_DELAY_1SEC,     HAL_GetHeaterPBA   },

    { 0,      0,      INPUT_DELAY_1SEC,    INPUT_DELAY_1SEC,     HAL_GetLevelDrainHigh },
    { 0,      0,      INPUT_DELAY_1SEC,    INPUT_DELAY_1SEC,     HAL_GetLevelDrainLow  }
}; 

void InitInputConfDetectTime(void)
{
    SetInputConfDetectTime( IN_FILTER ,         INPUT_DELAY_1SEC );   
    SetInputConfDetectTime( IN_TANK_OPEN,       INPUT_DELAY_1SEC );  
    SetInputConfDetectTime( IN_TANK_OPEN_UV,    INPUT_DELAY_100MS );  
    SetInputConfDetectTime( IN_HEATER,          INPUT_DELAY_1SEC ); 
}

void SetInputConfDetectTime(U8 mu8Id, U8 mu16DetectTime)
{
    inputList[ mu8Id ].setupDelay = mu16DetectTime; 
}


static U8   HAL_GetFilter(void)
{
    return (U8)GET_STATUS_FILTER_DETECT();
}


static U8   HAL_GetTankOpen(void)
{
    // ?ƒ±?¬ ë¦¬ë“œ ?Š¤?œ„ì¹˜ëŠ” ?‘˜?‹¤ ?—´? ¤ ?žˆ?„ ?•Œ, ?—´ë¦? ê²ƒìœ¼ë¡? ì²˜ë¦¬?•œ?‹¤.
#if !CONFIG_TANK_REED
    if( GET_REED_SW_TANK_OPEN() == HIGH ||
            GET_REED_SW_TANK_OPEN_2() == HIGH )
    {
        return HIGH;
    }
    return LOW;
#else
    if( GET_REED_SW_TANK_OPEN() == HIGH )
    {
        return HIGH;
    }
    return LOW;
#endif
}

static U8   HAL_GetTankOpen_1(void)
{
    return (U8)GET_REED_SW_TANK_OPEN();
}

static U8   HAL_GetTankOpen_2(void)
{
    return (U8)GET_REED_SW_TANK_OPEN_2();
}

U8   HAL_GetHeaterPBA(void)
{
    return (U8)GET_STATUS_MODEL_INPUT();
}

static U8   HAL_GetLevelDrainHigh(void)
{
    return (U8)GET_DRAIN_TANK_HIGH();
}

static U8   HAL_GetLevelDrainLow(void)
{
    return (U8)GET_DRAIN_TANK_LOW();
}

#if 0
U32 HAL_GetInput(void)
{
    U32 mu32Val;

    mu32Val = 0UL;

    if( HAL_GetInputValue( IN_FILTER ) != 0 )
    {
        mu32Val |= MK_INPUT_FILTER;
    }

    if( HAL_GetInputValue( IN_TANK_OPEN ) != 0 )
    {
        mu32Val |= MK_INPUT_TANK_OPEN;
    }

    if( HAL_GetInputValue( IN_TANK_OPEN_UV ) != 0 )
    {
        mu32Val |= MK_INPUT_TANK_OPEN_UV;
    }

    if( HAL_GetInputValue( IN_HEATER ) != 0 )
    {
        mu32Val |= MK_INPUT_HEATER_PBA;
    }

    return mu32Val;
}
#endif

U8 HAL_GetInputValue(U8 mu8Id)
{
    if( mu8Id >= IN_MAX_NUM )
    {
        return 0;
    }

    return inputList[ mu8Id ].curVal;
}


void HAL_ProcessInput(void)
{
    U8 i;
    Action_T    pGet = NULL;
    U8  mu8Val;


    for( i = 0; i < IN_MAX_NUM ; i++ )
    {
        pGet = inputList[ i ].pfGetStatus;
        if( pGet != NULL )
        {
            // Read port value
            mu8Val = pGet();
            if( inputList[ i ].prevVal != mu8Val )
            {
                // Update delay time...
                inputList[ i ].delay = inputList[ i ].setupDelay;
                inputList[ i ].prevVal = mu8Val;
            }
            else
            {
                inputList[ i ].delay--;
                if( inputList[ i ].delay == 0 )
                {
                    // update new value..
                    inputList[ i ].curVal = mu8Val;
                }
            }
        }
    }
}
