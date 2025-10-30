#include "key_handler.h"
#include "key_common_handler.h"


// NORMAL  
static KeyValidFun_T    pFunValid       = NULL;      // 유효성 검사 함수
static KeyEventList_T   *pKeyEventList  = NULL;    // 이벤트 리스트 
//static KeyEventSize_T   pFunListSize    = NULL;  // 이벤트 리스트 크기 반환 함수
static U8 gu8ListSize = 0;

void RegisterKeyEvent( KeyValidFun_T pValid, KeyEventList_T *pList, U8 mu8Size )
{
    pFunValid   = pValid;
    pKeyEventList = pList;
    gu8ListSize = mu8Size;
}

U8 IsValidKeyCondition(U32 mu32Key)
{
    U8 mu8Ret = TRUE;


    if( pFunValid != NULL )
    {
        mu8Ret = pFunValid( mu32Key );
    }

    return mu8Ret;
}

void* GetKeyEventList(void)
{
    return pKeyEventList;
}

U8 GetKeyEventListSize(void)
{
    return gu8ListSize;
}

