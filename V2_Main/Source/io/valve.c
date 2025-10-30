#include "hal_valve.h"
#include "valve.h"

#define TEST_VALVE_ONOFF   0

typedef struct _io_valve_
{
    U16 Cmd;      // On/Off command ( 1: ON, 0: OFF )
    U16 Cur;      // current status ( 1: ON, 0: OFF )
    U16 BootTime;     // 제어 지연 시간..

    U8 TEST_Count;   // TEST counter variable
} IO_Valve_T;

IO_Valve_T  valve;


void InitValve(void)
{
    // NOS VALVE is Normal Open
    valve.Cmd        = NORMAL_OPEN_VALVES;
    valve.Cur        = NORMAL_OPEN_VALVES;
    valve.BootTime   = 100;   // Boot Wait @10ms

    valve.TEST_Count = 0;
}

void OpenValve( U16	mu16Valve)
{
    valve.Cmd |= mu16Valve;
}

void CloseValve( U16 mu16Valve)
{
    valve.Cmd &= ~mu16Valve;
}

U16 GetValveStatus(void)
{
    return valve.Cur;
}

#if TEST_VALVE_ONOFF
U16 the_valve_cmd = 0x0000;
#endif
U16 GetValveCmd(void)
{

#if TEST_VALVE_ONOFF
    return the_valve_cmd;
#else
    return valve.Cmd;
#endif
}

U8 IsOpenValve(U16 mu16Valve )
{
    if( (valve.Cur & mu16Valve ) == mu16Valve )
    {
        return TRUE;
    }

    return FALSE;
}

void ControlValve(void)
{
    U8 i;
    U16 mu16Val;
    U16 mu16Diff;
    U16 mu16Cmd;

    /* Check Boot Time... all off valves.. */
    if( valve.BootTime != 0 )
    {
        valve.BootTime--;
        return ;
    }

    mu16Cmd = GetValveCmd();

    /* Check different between command and current status */
    mu16Diff = valve.Cur ^ mu16Cmd;
    if( mu16Diff == 0 )
    {
        return ;
    }

    /* First - turn off */
    for( i = 0; i < 16 ; i++ )
    {
        mu16Val = 1 << i;
        if( mu16Diff & mu16Val )
        {
            if( ( mu16Cmd & mu16Val ) == 0 )
            {
                HAL_CloseValve( i );
                valve.Cur &= ~mu16Val;
            }
        }
    }

    /* And then... turn on valve */
    mu16Diff = valve.Cur ^ mu16Cmd;
    if( mu16Diff == 0 )
    {
        return ;
    }
    for( i = 0 ; i < 16 ; i++ )
    {
        mu16Val = 1 << i;
        if( mu16Diff & mu16Val )
        {
            if( ( mu16Cmd & mu16Val) != 0 )
            {
                HAL_OpenValve( i );
                valve.Cur |= mu16Val;
            }
        }
    }
}

