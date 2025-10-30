
#include "r_cg_timer.h"

#include "prj_type.h"
#include "timer.h"
#include "process_sys_event.h"

#include "hal_serial.h"
#include "hal_input.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_adc.h"
#include "hal_uv.h"
#include "voice.h"
#include "voice_nxd1005.h"
#include "mode.h"


#include <stdio.h>
#include <string.h>


typedef void(*Action_T)(void);
typedef struct _sys_event_
{
    U8   timerId;
    Action_T pFun;
    Action_T pFun_FCT;
    Action_T pFun_EOL;
} SysEvent_T;

/* NORMAL */
static void Evt_1ms_Handler( void );
static void Evt_10ms_Handler( void );
static void Evt_100ms_Handler( void );

const static SysEvent_T	SysEventList[] =
{
    /* TIMER ID,                 NORMAL,                 FCT,    EOL */
    { TIMER_ID_1MS,              Evt_1ms_Handler,        NULL,   NULL },
    { TIMER_ID_10MS,             Evt_10ms_Handler,       NULL,   NULL },
    { TIMER_ID_100MS,            Evt_100ms_Handler,      NULL,   NULL },
};
#define	SZ_LIST		( sizeof( SysEventList ) / sizeof( SysEvent_T ) )

void ProcessEventHandler( void )
{
    U8 i;
    Action_T fun;

    for( i = 0; i < SZ_LIST ; i++ )
    {
        if( IsExpiredTimer( SysEventList[ i ].timerId ) == TIMER_EXPIRE )
        {
            // Disable Expired Timer
            DisableTimer( SysEventList[ i ].timerId );

            // Get Normal Mode Event Handler
            fun = SysEventList[ i ].pFun;

            // Call a event handl
            if( fun != NULL )
            {
                fun();
            }
        }
    }
}

static void Evt_1ms_Handler( void )
{
    StartTimer( TIMER_ID_1MS, 1);

    HAL_ScanKey();
    HAL_ProcessAdc();
}

static void Evt_10ms_Handler( void )
{
    StartTimer( TIMER_ID_10MS, 10);

    HAL_ProcessInput();
}


#if 0
U16 uv_ice = 0;
U16 uv_water = 0;
U16 photo = 0;
static void dbg_adc(void)
{
    uv_ice      = HAL_GetAdcValue( ANI_UV_ICE_FB );
    uv_water    = HAL_GetAdcValue( ANI_UV_WATER_FB );
    photo       = HAL_GetAdcValue( ANI_SENSOR_PHOTO );
}
#endif


#if 0   // by inchoi.. debug..
U8 the_voice = 0;
int the_voice_id = 0;
#endif
static void Evt_100ms_Handler(void)
{
    StartTimer( TIMER_ID_100MS, 100);

#if 0   // by inchoi.. debug..
    if( the_voice != 0 )
    {
        the_voice = 0;
        NXD_Play( the_voice_id );
    }
#endif

    ProcessMode();
    
    NXD_ProcessVoice();

    HAL_ControlUv();

#if 0
    dbg_adc();
#endif
}


void InitSystem(void)
{
    /* IO */
    HAL_InitComm();
    HAL_InitUv();

    InitVoice();
    

    R_TAU0_Channel1_Start();   // 100us dimming timer interrupt
}

