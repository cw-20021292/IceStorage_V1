#include "hal_relay.h"
#include "relay.h"


#define  MK_EX_TYPE  ( RELAY_COMP )
//#define  MK_EX_TYPE  ( RELAY_COMP | RELAY_HEATER )
#define  MK_CON_TYPE ( ~MK_EX_TYPE )

/* Relay 제어는 배차적인 제어 방식과 동시 제어 방식으로 구분된다.
 *
 * 배타 제어 방식은 1초 간격으로 제어된다.
 * 동시에 turn on 될 수 없다.
 *
 * 동시 제어 방식은 제어 주기 타이머에 영향을 받지 않으며,
 * 동시에 turn on 될 수 있다.
 *
 */

typedef struct _io_relay_
{
    U16 Cmd;      // On/Off command ( 1: ON, 0: OFF )
    U16 Cur;      // current status ( 1: ON, 0: OFF )

    // exclusive type
    U16 CmdEx;      // On/Off command ( 1: ON, 0: OFF )
    U16 CurEx;      // current status ( 1: ON, 0: OFF )

    // concurrent type
    U16 CmdCon;   
    U16 CurCon;   

    U8 TEST_Count;   // TEST counter varaible
} IO_Relay_T;

IO_Relay_T  relay;


void InitRelay(void)
{
    relay.Cmd = 0;
    relay.Cur = 0;

    relay.CmdEx = 0;
    relay.CurEx = 0;

    relay.CmdCon = 0;
    relay.CurCon = 0;
    relay.TEST_Count = 0;
}


void TurnOnRelay(U16 mu16Relay)
{
    if( (mu16Relay & MK_EX_TYPE ) != 0 )
    {
        relay.CmdEx |= (mu16Relay & MK_EX_TYPE);
    }
    else
    {
        relay.CmdCon |= (mu16Relay & MK_CON_TYPE);
    }
}

void TurnOffRelay(U16 mu16Relay)
{
    relay.Cmd &= ~mu16Relay;

    if( (mu16Relay & MK_EX_TYPE ) != 0 )
    {
        relay.CmdEx &= ~(mu16Relay & MK_EX_TYPE);
    }
    else
    {
        relay.CmdCon &= ~(mu16Relay & MK_CON_TYPE);
    }
}

void TurnOffRelayAll(void)
{
    relay.Cmd = 0;
    relay.CmdEx = 0;
    relay.CmdCon = 0;
}

U8 IsTurnOnRelay(U16 mu16Relay)
{
    U16 mu16Cmd;

    mu16Cmd = relay.CmdEx | relay.CmdCon;
    if( (mu16Cmd & mu16Relay) == mu16Relay )
    {
        return TRUE;
    }

    return FALSE;
}

U16 GetRelayStatus(void)
{
    U16 mu16Cmd;

    mu16Cmd = relay.CurEx | relay.CurCon;
    return mu16Cmd;
}

static U16 OnOffRelay(U8 mu8Ex, U16 mu16Cur, U16 mu16Cmd)
{
    U8 i;
    U16 mu16Val;
    U16 mu16Diff;


    /* Check different between command and current status */
    mu16Diff = mu16Cur ^ mu16Cmd;
    if( mu16Diff == 0 )
    {
        // 제어 명령과 현재 상태에 차이가 없으면 종료
        return mu16Cur;
    }

    /* OFF 명령 부터 처리 */
    for( i = 0; i < 16 ; i++ )
    {
        // bit shift하면서 differ한 부분을 찾는다.
        mu16Val = 1 << i;
        if( mu16Diff & mu16Val )
        {
            // bit가 clear이면 off 제어
            if( ( mu16Cmd & mu16Val ) == 0 )
            {
                HAL_TurnOffRelay( i );
                mu16Cur &= ~mu16Val;
            }
        }
    }

    /* ON 명령 처리 */
    mu16Diff = mu16Cur ^ mu16Cmd;   
    if( mu16Diff == 0 )
    {
        // 제어 명령과 현재 상태에 차이가 없으면 종료
        return mu16Cur;
    }

    for( i = 0 ; i < 16 ; i++ )
    {
        // bit shift하면서 differ한 부분을 찾는다.
        mu16Val = 1 << i;
        if( mu16Diff & mu16Val )
        {
            // bit가 set이면 on 제어
            if( ( mu16Cmd & mu16Val) != 0 )
            {
                HAL_TurnOnRelay( i );
                mu16Cur |= mu16Val;

                // 배타적으로 한 번에 하나의 relay만 on할 수 있음
                if( mu8Ex == TRUE )
                {
                    return mu16Cur;
                }
            }
        }
    }

    return mu16Cur;
}

void ControlRelayExclusive(void)
{
    relay.CurEx = OnOffRelay( TRUE, relay.CurEx, relay.CmdEx ) ;
}

void ControlRelayConcurrent(void)
{
    relay.CurCon = OnOffRelay( FALSE, relay.CurCon, relay.CmdCon ) ;
}

#if 0
void TEST_ControlRelay(void)
{
    U16 cmd;

    cmd = 1 << relay.TEST_Count;
    TurnOffRelay( 0xFFFF );   // turn off all relay
    TurnOnRelay( cmd );   // turn on relay
    if( relay.TEST_Count++ >= MAX_RELAY_NUM )
    {
        relay.TEST_Count = 0;
    }

    ControlRelayExclusive();
    ControlRelayConcurrent();
}
#endif
