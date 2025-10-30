#include "hw.h"
#include "key.h"

#include <string.h>


/* Status */
typedef enum
{
    KEY_STATUS_NONE,
    KEY_STATUS_PUSHED, 
    KEY_STATUS_POP,    
    KEY_STATUS_TIME_OUT
}KeyStatus_T;


/* Event Timeout condition @10ms */
#define  EVENT_TIME_OUT_5S          500     // 5sec
#define  EVENT_TIME_OUT_3S          300     // 3sec
#define  EVENT_TIME_OUT_2S          200     // 2sec
#define  EVENT_TIME_OUT_1S          100     // 1sec
#define  EVENT_TIME_OUT_SHORT        10     // 100ms
#define  MAX_EVENT_TIME_OUT         EVENT_TIME_OUT_5S

typedef struct _key_
{
    KeyStatus_T   Status;  

    U32  Val;           // PUSH KEY VAL
    U32  ValPop;        // POP KEY VAL
    U16  PushedTime;    // PUSH TIME...
    U8   EventFlag;     // Event Flag..
} Key_T;

Key_T   Key;



void InitKey(void)
{
    MEMSET((void __FAR *)&Key, 0, sizeof( Key ) );
}


U32 GetKeyVal( void )
{
    return Key.Val;
}

U32 GetKeyPopVal(void)
{
    return Key.ValPop;
}

void SetKeyEventFlag(U8 mu8Event )
{
    Key.EventFlag |= mu8Event;
}

void ClearKeyEventFlag(U8 mu8Event )
{
    Key.EventFlag &= ~mu8Event;
}

U8 IsSetKeyEventFlag( U8 mu8Event )
{
    if( (Key.EventFlag & mu8Event) ==  mu8Event )
    {
        return TRUE;
    }

    return FALSE;
}


/*
 * KEY SCAN 함수
 *
 * Description :
 *  -. KEY 버튼 값을 읽어 온다.
 *  -. 각 KEY 버튼 입력에 대한 EVENT를 설정한다.
 *  -. EVENT는 KEY_EVENT 아래와 같이 5개의 이벤트가 있다.
 *       KEY_EVENT_PUSHED_SHORT  
 *       KEY_EVENT_PUSHED_LONG_5S
 *       KEY_EVENT_PUSHED_LONG_3S
 *       KEY_EVENT_PUSHED_LONG_2S
 *       KEY_EVENT_POP           
 */
typedef struct _KeyEvent_
{
    U16 u16EventTime;
    U8 u8Event;
} KeyEvent_T;

static KeyEvent_T KeyEventList[] = 
{
    { EVENT_TIME_OUT_5S,    KEY_EVENT_PUSHED_LONG_5S },
    { EVENT_TIME_OUT_3S,    KEY_EVENT_PUSHED_LONG_3S },
    { EVENT_TIME_OUT_2S,    KEY_EVENT_PUSHED_LONG_2S },
    { EVENT_TIME_OUT_1S,    KEY_EVENT_PUSHED_LONG_1S },
    { EVENT_TIME_OUT_SHORT, KEY_EVENT_PUSHED_SHORT },

};
#define SZ_KEY_EVENT_LIST   (sizeof(KeyEventList)/sizeof(KeyEvent_T))

static void FindSetKeyEvent(U16 mu16PushedTime)
{
    U8 mu8Index;

    for( mu8Index = 0; mu8Index < SZ_KEY_EVENT_LIST ; mu8Index++ )
    {
        if( KeyEventList[ mu8Index ].u16EventTime == mu16PushedTime )
        {
            SetKeyEventFlag( KeyEventList[ mu8Index ].u8Event );
            return ;
        }
    }
}

void ProcessScanKey(void)
{
    static U32 mu32PrevKeyVal;
    U32  mu32KeyVal;


    // 1. Get Key value 
    mu32KeyVal = HAL_GetKeyVal();
    if( mu32PrevKeyVal != mu32KeyVal )
    {
        if( mu32KeyVal == HAL_KEY_NONE )
        {
            Key.ValPop      = mu32PrevKeyVal;
        }

        mu32PrevKeyVal  = mu32KeyVal;
        Key.Val         = mu32KeyVal;  

        if( mu32KeyVal != HAL_KEY_NONE )
        {
            Key.Status        = KEY_STATUS_PUSHED;
            Key.PushedTime    = 0;  
            ClearKeyEventFlag( KEY_EVENT_ALL );

        }
        else 
        {
            if( Key.PushedTime > EVENT_TIME_OUT_SHORT )
            {
                Key.Status = KEY_STATUS_POP;
                SetKeyEventFlag( KEY_EVENT_POP );
            }
            else
            {
                Key.Status = KEY_STATUS_NONE;
            }
        }
    }
    
    if( Key.Status == KEY_STATUS_PUSHED )
    {
        Key.PushedTime++;
        FindSetKeyEvent( Key.PushedTime );

        if( Key.PushedTime > MAX_EVENT_TIME_OUT )
        {
            Key.Status = KEY_STATUS_TIME_OUT;
        }
    }
}
