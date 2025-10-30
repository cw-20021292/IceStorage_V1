#include "hw.h"
#include "hal_adc.h"
#include "ice_full.h"
#include "process_make.h"


#define  ICE_FULL_ADC_LEVEL   300 /* 1.47V */

#define  CHECK_TIME        100 /* @100ms, 10sec */
#define  CHECK_CONDITION   50 

IceFull_T  IceFull;

//extern Ice_Turbo_T IceTurbo;


void InitIceFull(void)
{
    IceFull.Full          = FALSE;        // 만빙 상태
    IceFull.Out           = FALSE;        // 만빙 이후 추출 여부 

    IceFull.Start         = TRUE;         // 만빙 검사 시작 FLAG
    IceFull.CheckTime      = CHECK_TIME;  // 만빙 검사 시간
    IceFull.CheckCount     = 0;           // ADC 기준치 초과 횟수

    IceFull.PowerOnCheck   = FALSE;       //  전원 RESET 후, 최초 검사 실행 여부

	IceFull.initFull	   = FALSE;
}

void GetIceFullData( IceFull_T *pIceFull )
{
    MEMCPY( (void __far *)pIceFull, (const void __far *)&IceFull, sizeof( IceFull_T ));
}

U8 GetInitIceFullStatus(void)
{
    return IceFull.initFull;
}

void SetInitIceFullStatus(U8 initIceFullStatus)
{
    IceFull.initFull = initIceFullStatus;
}

U8 GetIceFullStatus(void)
{
    return IceFull.Full;
}

U8 GetIceFullOutStatus(void)
{
    return IceFull.Out;
}

void ClearIceFullOutStatus(void)
{
    IceFull.Out = FALSE;
}

U8 GetIceFullPowerOnCheck(void)
{
    return IceFull.PowerOnCheck;
}

void StartCheckIceFull(void)
{
    IceFull.Start = TRUE;
}

/* 만빙 검사 동작 진행 여부 반환
 *
 * Return :
 *  TRUE - 검사 완료
 *  FALSE - 검사 중...
 */


U8 IsDoneCheckIceFull(void)
{
    if( IceFull.Start == TRUE )
    {
        return FALSE; // 검사 진행 중..
    }

    return TRUE; // 검사 종료
}

void CheckIceFull(void)
{
    if( IceFull.Start == FALSE )
    {
        return ;
    }


    /* 1. 만빙 검사 - IR 출력 */
    if( GET_STATUS_ICE_INFRARED() == 0 ) 
    {
        TURN_ON_ICE_INFRARED();

        IceFull.CheckTime = CHECK_TIME;
        IceFull.CheckCount = 0;
        return ;
    }

    /* 2. 만빙 검사 - ADC 값 측정*/
    /* 만빙 수신부 10초간 확인, 얼음이 없는 경우 ADC 값이 높게 나옴 */ 
    if( HAL_GetAdcValue( ANI_SENSOR_INFRARED ) >= ICE_FULL_ADC_LEVEL )
    {
        IceFull.CheckCount++;
    }

    IceFull.CheckTime--;
    if( IceFull.CheckTime != 0 )
    {
        return ;
    }

    /* 3. 검사 종료 후, 결과 판정 */
    IceFull.Start = FALSE;
    TURN_OFF_ICE_INFRARED();

    if( IceFull.CheckCount >= CHECK_CONDITION )
    {
        IceFull.Full = FALSE;    // 얼음 없음
    }
    else
    {
        IceFull.Full = TRUE;     // 만빙
        IceFull.Out  = TRUE;
    }

    /* 4. Power On Check */
    IceFull.PowerOnCheck = TRUE;
}

 
