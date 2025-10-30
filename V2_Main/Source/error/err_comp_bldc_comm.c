#include "err_comp_bldc_comm.h"
#include "comp_bldc.h"

#if CONFIG_COMP_ERROR_ONESHOT
#define ERR_COUNT   0
#else
#define ERR_COUNT   10
#endif
#define RELEASE_TIME    (24UL*60UL*60UL*10UL)  // 24hour = 24h*60m*60s*10 @100ms


typedef struct _err_comp_bldc_comm_
{
    U8 Error;           // Error ���� (TRUE:����, FALSE:����)
    U8 Count;           // ��õ� Ƚ��
    U32 ReleaseTime;    // ��õ� Ƚ�� �ʱ�ȭ Ÿ�̸�
} ErrCompBldcComm_T;

ErrCompBldcComm_T  ErrCompComm;


void InitErrCompBldcComm(void)
{
    ErrCompComm.Error = FALSE;
    ErrCompComm.Count = ERR_COUNT;
    ErrCompComm.ReleaseTime = RELEASE_TIME;
}

void ResetCompBldcCommErrorCount(void)
{
    ErrCompComm.Count = ERR_COUNT;
    ErrCompComm.ReleaseTime = RELEASE_TIME;
}

U8 CheckErrCompBldcComm(U8 mu8Error)
{
    if( ErrCompComm.Error == TRUE )
    {
        return TRUE;    // ����
    }

    return FALSE;   // ����
}


/******************************************************************************************************************
 *PROTECT
 */
U8 CheckProtectCompBldcComm(U8 mu8Error)
{
    if( GetCompBldcErrorComm() == TRUE )
    {
        if( ErrCompComm.Count != 0 )
        {
            ErrCompComm.Count--;
            SetCompBldcErrorComm( FALSE );
            SetCompBldcProtect( TRUE );

            return TRUE;    // error
        }
        else
        {
            ErrCompComm.Error = TRUE;  // ���� ���� �߻�
        }
    }
    else
    {
#if 0
        if( ErrCompComm.ReleaseTime != 0 )
        {
            ErrCompComm.ReleaseTime--;
        }
        else
        {
            ResetCompBldcCommErrorCount();
        }
#endif
    }

    return FALSE;   // normal
}

U8 ReleaseProtectCompBldcComm(U8 mu8Error)
{
    if( GetCompBldcProtect() == FALSE )
    {
        return FALSE;   // normal
    }

    return TRUE;    // error
}

