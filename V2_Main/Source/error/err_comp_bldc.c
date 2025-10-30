#include "err_comp_bldc.h"
#include "comp_bldc.h"
#include "comp.h"

#if CONFIG_COMP_ERROR_ONESHOT
#define ERR_COUNT_NUM   0 // ���� �ݺ� �߻� ī��Ʈ ( 3ȸ �߻� ����)
#else
/*.. sean [24-10-10] 3ȸ �ݺ��� ���� ǥ�ÿ��� 10ȸ ǥ�÷� ���� ..*/
#define ERR_COUNT_NUM   10	// ���� �ݺ� �߻� ī��Ʈ ( 3ȸ �߻� ����)
#define ERR_PRT_COUNT_NUM   10	// ���� ���� �ݺ� �߻� ī��Ʈ ( 3ȸ �߻� ����)
#endif
#define RELEASE_TIME    (12UL*60UL*60UL*10UL)  // 12hour = 12h*60m*60s*10 @100ms
#define NORMAL_COMP_OPERATION_TIME    (10UL*60UL*10UL)  // 10min = 10m*60s*10 @100ms

typedef struct _err_comp_bldc_code_
{
    U8 Error;

    U8 ErrCountList[ ERR_BLDC_NUM ];
    U8 PrtCountList[ ERR_BLDC_NUM ];

    U32 ReleaseTime[ ERR_BLDC_NUM ];
} ErrCompBldc_T;


ErrCompBldc_T ErrCompBldc;

void InitErrCompBldc(void)
{
    ErrCompBldc.Error = FALSE;

    ResetCompBldcErrorCount();
}

U8 GetErrCompBldcCount(U8 mType, BldcErrorId_T mId)
{
    if( mType == 0 )
    {
        return ErrCompBldc.ErrCountList[ mId ];
    }

    return ErrCompBldc.ErrCountList[ mId ];
}

// ���������� �߻��� �̷��� ������, TRUE ��ȯ
static U8 IsErrorCount(void)
{
    U8 i;

    for( i = 1; i < ERR_BLDC_NUM ; i++ )
    {
        if( ErrCompBldc.ErrCountList[ i ] < ERR_COUNT_NUM )
        {
            return TRUE;
        }
    }

    return FALSE;
}

// ��ȣ������ �߻��� �̷��� ������, TRUE ��ȯ
static U8 IsPrtCount(void)
{
    U8 i;

    for( i = 1; i < ERR_BLDC_NUM ; i++ )
    {
        if( ErrCompBldc.PrtCountList[ i ] < ERR_PRT_COUNT_NUM )
        {
            return TRUE;
        }
    }

    return FALSE;
}

static void ResetCompBldcErrorCount(void)
{
    U8 i;

    ErrCompBldc.ErrCountList[ 0 ] = 0;
    ErrCompBldc.PrtCountList[ 0 ] = 0;
    for( i = 1; i < ERR_BLDC_NUM ; i++ )
    {
        ErrCompBldc.ErrCountList[ i ] = ERR_COUNT_NUM;
        ErrCompBldc.PrtCountList[ i ] = ERR_PRT_COUNT_NUM;
    	ErrCompBldc.ReleaseTime[ i ] = NORMAL_COMP_OPERATION_TIME;
    }
	//�迭�� �����Ͽ� ���� �ڵ� �ּ�
    //ErrCompBldc.ReleaseTime = NORMAL_COMP_RELEASE_TIME;
}


static void CountDownError( BldcErrorId_T mError )
{
    if( ErrCompBldc.ErrCountList[ mError ] != 0 )
    {
        ErrCompBldc.ErrCountList[ mError ]--;
    }
}

static void ClearError(void)
{
    SetCompBldcErrorCode( ERR_BLDC_NONE );
}

// ���� �߻� �˻�
// ���� �߻� �� 24�ð� �������� ���� ī��Ʈ �ʱ�ȭ
U8 CheckErrCompBldc(U8 mu8Error)
{
	return FALSE;
#if 0    
    if( ErrCompBldc.Error != ERR_BLDC_NONE )
    {
        if( ErrCompBldc.ErrCountList[ ErrCompBldc.Error ] == 0 )
        {
            return TRUE;    // ERROR
        }
        else
        {
            ErrCompBldc.Error = ERR_BLDC_NONE;
        }
    }
    else 
    {
        ResetCompBldcErrorCount();
        if( ErrCompBldc.ReleaseTime != 0 )
        {
            ErrCompBldc.ReleaseTime--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // ������ �߻��� �̷��� ������ 24�ð� �ʱ�ȭ
        if( IsErrorCount() == FALSE 
                && IsPrtCount() == FALSE )
        {
            ErrCompBldc.ReleaseTime = RELEASE_TIME;
        }	
	}

    return FALSE;   // NORMAL
#endif
}

/******************************************************************************************************************
 *PROTECT
 */


U8 CheckProtectCompBldc(U8 mu8Error)
{
    BldcErrorId_T mError;


    mError = GetCompBldcErrorCode();
    if( mError != ERR_BLDC_NONE )
    {
        ErrCompBldc.Error = mError;

        // error
        if( ErrCompBldc.PrtCountList[ mError ] != 0 )
        {
            ErrCompBldc.PrtCountList[ mError ]--;
            ClearError();
        }
        else
        {
            ErrCompBldc.PrtCountList[ mError ]  = ERR_PRT_COUNT_NUM;

            CountDownError( mError );
            SetCompBldcProtect( TRUE );

            return TRUE;    // error
        }
    }

    return FALSE;   // normal
}

U8 ReleaseProtectCompBldc(U8 mu8Error)
{
    if( GetCompBldcProtect() == FALSE )
    {
        return FALSE;   // normal
    }

    return TRUE;    // error
}
U8 CheckErrCompCurrentSensing(U8 mu8Error)
{
    BldcErrorId_T mError;
    mError = GetCompBldcErrorCode();
	
    if( mError != ERR_BLDC_NONE )
    {
        if( ErrCompBldc.ErrCountList[ ERR_BLDC_CURRENT_SENSING_ERROR ] == 0 )
        {
        	SetCompBldcErrorCode(ERR_BLDC_CURRENT_SENSING_ERROR);
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == ERR_BLDC_CURRENT_SENSING_ERROR  )
            {
                ErrCompBldc.Error = ERR_BLDC_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[ERR_BLDC_CURRENT_SENSING_ERROR] != 0 )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_CURRENT_SENSING_ERROR]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // ������ OFF �� ���� �̰ų� 
        // ������ ON �� ���¿��� ������ �߻��� �̷��� ������ 10�� �ʱ�ȭ
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount() == FALSE 
                && IsPrtCount() == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_CURRENT_SENSING_ERROR] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

U8 CheckErrCompStartingFail(U8 mu8Error)
{
    BldcErrorId_T mError;
    mError = GetCompBldcErrorCode();
	
    if( mError != ERR_BLDC_NONE )
    {
        if( ErrCompBldc.ErrCountList[ ERR_BLDC_STARTING_FAIL ] == 0 )
        {
        	SetCompBldcErrorCode(ERR_BLDC_STARTING_FAIL);
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == ERR_BLDC_STARTING_FAIL  )
            {
                ErrCompBldc.Error = ERR_BLDC_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[ERR_BLDC_STARTING_FAIL] != 0 )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_STARTING_FAIL]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // ������ OFF �� ���� �̰ų� 
        // ������ ON �� ���¿��� ������ �߻��� �̷��� ������ 10�� �ʱ�ȭ
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount() == FALSE 
                && IsPrtCount() == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_STARTING_FAIL] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

U8 CheckErrCompOverCurrent(U8 mu8Error)
{
    BldcErrorId_T mError;
    mError = GetCompBldcErrorCode();
	
    if( mError != ERR_BLDC_NONE )
    {
        if( ErrCompBldc.ErrCountList[ ERR_BLDC_OVER_CURRENT ] == 0 )
        {
        	SetCompBldcErrorCode(ERR_BLDC_OVER_CURRENT);
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == ERR_BLDC_OVER_CURRENT  )
            {
                ErrCompBldc.Error = ERR_BLDC_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[ERR_BLDC_OVER_CURRENT] != 0 )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_OVER_CURRENT]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // ������ OFF �� ���� �̰ų� 
        // ������ ON �� ���¿��� ������ �߻��� �̷��� ������ 10�� �ʱ�ȭ
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount() == FALSE 
                && IsPrtCount() == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_OVER_CURRENT] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

U8 CheckErrCompOverheat(U8 mu8Error)
{
    BldcErrorId_T mError;
    mError = GetCompBldcErrorCode();
	
    if( mError != ERR_BLDC_NONE )
    {
        if( ErrCompBldc.ErrCountList[ ERR_BLDC_OVERHEAT_CONTROLLER ] == 0 )
        {
        	SetCompBldcErrorCode(ERR_BLDC_OVERHEAT_CONTROLLER);
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == ERR_BLDC_OVERHEAT_CONTROLLER  )
            {
                ErrCompBldc.Error = ERR_BLDC_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[ERR_BLDC_OVERHEAT_CONTROLLER] != 0 )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_OVERHEAT_CONTROLLER]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // ������ OFF �� ���� �̰ų� 
        // ������ ON �� ���¿��� ������ �߻��� �̷��� ������ 10�� �ʱ�ȭ
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount() == FALSE 
                && IsPrtCount() == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_OVERHEAT_CONTROLLER] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

U8 CheckErrCompDisconnection(U8 mu8Error)
{
    BldcErrorId_T mError;
    mError = GetCompBldcErrorCode();
	
    if( mError != ERR_BLDC_NONE )
    {
        if( ErrCompBldc.ErrCountList[ ERR_BLDC_DISCONNECTION_COMM ] == 0 )
        {
        	SetCompBldcErrorCode(ERR_BLDC_DISCONNECTION_COMM);
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == ERR_BLDC_DISCONNECTION_COMM  )
            {
                ErrCompBldc.Error = ERR_BLDC_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[ERR_BLDC_DISCONNECTION_COMM] != 0 )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_DISCONNECTION_COMM]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // ������ OFF �� ���� �̰ų� 
        // ������ ON �� ���¿��� ������ �߻��� �̷��� ������ 10�� �ʱ�ȭ
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount() == FALSE 
                && IsPrtCount() == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_DISCONNECTION_COMM] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

U8 CheckErrCompAbnormalVoltage(U8 mu8Error)
{
    BldcErrorId_T mError;
    mError = GetCompBldcErrorCode();
	
    if( mError != ERR_BLDC_NONE )
    {
        if( ErrCompBldc.ErrCountList[ ERR_BLDC_ABNORMAL_VOLTAGE ] == 0 )
        {
        	SetCompBldcErrorCode(ERR_BLDC_ABNORMAL_VOLTAGE);
            return TRUE;    // ERROR
        }
        else
        {
            if( ErrCompBldc.Error == ERR_BLDC_ABNORMAL_VOLTAGE  )
            {
                ErrCompBldc.Error = ERR_BLDC_NONE;
            }
        }
    }
    else
    {
        if( ErrCompBldc.ReleaseTime[ERR_BLDC_ABNORMAL_VOLTAGE] != 0 )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_ABNORMAL_VOLTAGE]--;
        }
        else
        {
            ResetCompBldcErrorCount();
        }

        // ������ OFF �� ���� �̰ų� 
        // ������ ON �� ���¿��� ������ �߻��� �̷��� ������ 10�� �ʱ�ȭ
        if( GetCompOnOff() == FALSE 
            || ( IsErrorCount() == FALSE 
                && IsPrtCount() == FALSE ) )
        {
            ErrCompBldc.ReleaseTime[ERR_BLDC_ABNORMAL_VOLTAGE] = RELEASE_TIME;
        }
    }

    return FALSE;   // NORMAL
}

