#include "hw.h"
#include "prj_type.h"
#include "process_key.h"
#include "key.h"
#include "sound.h"
#include "power_saving.h"

#include "process_display.h"
#include "key_common_handler.h"
#include "key_handler.h"
#include "key_normal_handler.h"
#include "key_eol_handler.h"
#include "key_fct_handler.h"


/*
 *
 * Param :
 *  -. pValidFunc, : 유효성 검사 함수 포인터 
 *  -. pList, : 키 버튼 이벤트 리스트
 *  -. mu8Size : 키 버튼 이벤트 리스트 크기
 * 
 * Return :
 *  -. FALSE : 키 입력 처리가 된게 없음.
 *  -. TRUE : 키 입력 처리가 된게 있음.
 */
extern U8 gu8ColdOff;
extern U8 gu8IceOff;
static U8 KeyEventHandler(KeyValidFun_T pValidFunc, KeyEventList_T *pList, U8 mu8Size )
{
    U8   i;
    U32  mu32KeyVal;
    U32  mu32KeyPopVal;
    U8   mu8Sound;

    static U32  mu32PrevKeyVal = K_NONE;
    static U8 mu8ErrKeySkip = FALSE;



    /* 키 입력 데이터를 가져온다. */
    mu32KeyVal    = GetKeyVal();
    mu32KeyPopVal = GetKeyPopVal();

    /* 키 입력 시, 
     * -. 절전 기능 해제
     * -. 표시부 대기 시간 재설정
     */
    if( mu32KeyVal != K_NONE )
    {
        WakeUpSavingMode();
    }

    if( mu32KeyVal != mu32PrevKeyVal )
    {
        mu32PrevKeyVal = mu32KeyVal;
        mu8ErrKeySkip = FALSE;

        gu8ColdOff = FALSE;
        gu8IceOff = FALSE;
    }

    if( mu8ErrKeySkip == TRUE )
    {
        return TRUE;
    }

    if( pList == NULL || mu8Size == 0 )
    {
        return TRUE;
    }

    /* Check Pop Key */
    if( mu32KeyVal == K_NONE 
            && IsSetKeyEventFlag( KEY_EVENT_POP ) == TRUE )
    {
        ClearKeyEventFlag( KEY_EVENT_POP );
        for( i = 0; i < mu8Size ; i++ )
        {
            if( (pList+i)->Key == mu32KeyPopVal )
            {
                if( (pList+i)->actionEvent[KEY_ACTION_POP] != NULL )
                {
                    mu8Sound = (pList+i)->actionEvent[KEY_ACTION_POP]();
                    Sound( mu8Sound );
                    return TRUE;
                }
            }
        }
    }
    
    /* Check Pusehd Key */
    for( i = 0; i < mu8Size ; i++ )
    {
        if( (pList+i)->Key == mu32KeyVal )
        {
            if( IsSetKeyEventFlag( KEY_EVENT_PUSHED_SHORT ) == TRUE )
            {
                ClearKeyEventFlag( KEY_EVENT_PUSHED_SHORT );
		
                /* 키 버튼 입력 유효성 검사 */
                if( mu32KeyVal != K_NONE 
                        && pValidFunc != NULL )
                {

                    StartDispTimerId( DISP_TIMER_CLOCK );
                    StartDispTimerId( DISP_TIMER_SLEEP );

                    if( pValidFunc( mu32KeyVal ) == FALSE )
                    {
                        //Sound( SOUND_ERROR );
                        mu8ErrKeySkip = TRUE;
                        return TRUE;
                    }
                }

                if( (pList+i)->actionEvent[KEY_ACTION_SHORT] != NULL )
                {
                    mu8Sound = (pList+i)->actionEvent[KEY_ACTION_SHORT]();
                    Sound( mu8Sound );
                    return TRUE;
                }
                break;
            }
            else if( IsSetKeyEventFlag( KEY_EVENT_PUSHED_LONG_5S ) == TRUE )
            {
                ClearKeyEventFlag( KEY_EVENT_PUSHED_LONG_5S );

                if( (pList+i)->actionEvent[KEY_ACTION_5_SEC] != NULL )
                {
                    mu8Sound = (pList+i)->actionEvent[KEY_ACTION_5_SEC]();
                    Sound( mu8Sound );
                    return TRUE;
                }
                break;
            }
            else if( IsSetKeyEventFlag( KEY_EVENT_PUSHED_LONG_3S ) == TRUE )
            {
                ClearKeyEventFlag( KEY_EVENT_PUSHED_LONG_3S );

                if( (pList+i)->actionEvent[ KEY_ACTION_3_SEC ] != NULL )
                {
                    mu8Sound = (pList+i)->actionEvent[ KEY_ACTION_3_SEC ]();
                    Sound( mu8Sound );
                    return TRUE;
                }
                break;
            }
            else if( IsSetKeyEventFlag( KEY_EVENT_PUSHED_LONG_2S ) == TRUE )
            {
                ClearKeyEventFlag( KEY_EVENT_PUSHED_LONG_2S );

                if( (pList+i)->actionEvent[ KEY_ACTION_2_SEC ] != NULL )
                {
                    mu8Sound = (pList+i)->actionEvent[ KEY_ACTION_2_SEC ]();
                    Sound( mu8Sound );
                    return TRUE;
                }
                break;
            }
            else if( IsSetKeyEventFlag( KEY_EVENT_PUSHED_LONG_1S ) == TRUE )
            {
                ClearKeyEventFlag( KEY_EVENT_PUSHED_LONG_1S );

                if( (pList+i)->actionEvent[ KEY_ACTION_1_SEC ] != NULL )
                {
                    mu8Sound = (pList+i)->actionEvent[ KEY_ACTION_1_SEC ]();
                    Sound( mu8Sound );
                    return TRUE;
                }
                break;
            }
        }
    }

    return FALSE;
}

void ProcessKeyEventHandler(void)
{
    U8 mu8Ret = FALSE;
    static U16 mu16Delay = 0;     /* 키 입력 간 지연 시간 */
    KeyValidFun_T  pKeyValidFun = NULL;
    KeyEventList_T  *pKeyEventList = NULL;
    U8 mu8KeyEventSize = 0;



    /* 디스플레이 표시 초기화  */
    if( GetDisplayInit() == FALSE )
    {
        return ;
    }

    /* 정상 키 입력 후, 
     * 다시 키 입력이 즉시 적용되지 않도록 지연 시간을 적용
     *
     *  -. 부저 음이 완료 되기 전에, 부저음이 연속해서 발생되는
     *  문제점을 개선하기 위해 적용하였음.
     */
    if( mu16Delay != 0 )
    {
        mu16Delay--;
        return ;
    }

    // Get Event Handler by case..
    if( GetEolStatus() == TRUE )
    {
        // EOL
        pKeyValidFun = IsValidEolKeyCondition;
        pKeyEventList = (KeyEventList_T *)GetEolKeyEventList();
        mu8KeyEventSize = GetEolKeyEventListSize();
    }
    else if( GetFctStatus() == TRUE )
    {
        // FCT
        pKeyValidFun = IsValidFctKeyCondition;
        pKeyEventList = (KeyEventList_T *)GetFctKeyEventList();
        mu8KeyEventSize = GetFctKeyEventListSize();
    }
    else
    {
        // NORMAL
        pKeyValidFun = IsValidNormalKeyCondition; 
        pKeyEventList = (KeyEventList_T *)GetNormalKeyEventList();
        mu8KeyEventSize = GetNormalKeyEventListSize();
    }

    // Call Key Event Handler
    mu8Ret = KeyEventHandler( pKeyValidFun, pKeyEventList, mu8KeyEventSize );
    if( mu8Ret == TRUE )
    {
        mu16Delay = 30; // wait.. 300ms...
    }
}
