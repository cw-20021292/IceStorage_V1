#include "hw.h"
#include "hal_input.h"


#define DELAY_100MS  10U       /* 100ms @10ms */
#define DELAY_1SEC   100U      /* 1sec @10ms */

static U8   HAL_GetModel(void);
static U8   HAL_GetFilter(void);


typedef U8 (*Action_T)(void);
typedef struct _input_config_
{
    U8  prevVal; 
    U8  curVal;

    U16 delay;
    U16 setupDelay;

    Action_T    pfGetStatus;    
} InputConfig_T;

InputConfig_T inputList[ IN_MAX_NUM ] = 
{ 
    { 0,      0,      DELAY_1SEC,     DELAY_1SEC,      HAL_GetModel     },
    { 0,      0,      DELAY_1SEC,    DELAY_1SEC,     HAL_GetFilter  }
}; 

static U8   HAL_GetModel(void)
{
    return (U8)GET_STATUS_MODEL_CHPI();
}

static U8   HAL_GetFilter(void)
{
    return (U8)GET_STATUS_FILTER();
}


U8 HAL_GetInput(void)
{
    U8 mu8Val;

    mu8Val = 0U;
    if( HAL_GetInputValue( IN_MODEL ) != 0 )
    {
        mu8Val |= MK_INPUT_MODEL;
    }

    if( HAL_GetInputValue( IN_FILTER ) != 0 )
    {
        mu8Val |= MK_INPUT_FILTER;
    }

    return mu8Val;
}

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
