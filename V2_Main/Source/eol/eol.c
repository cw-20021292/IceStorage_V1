#include "eol.h"

#include "process_display.h"
#include "WIFI_Control.h"
#include "hot_water.h"


#define  DEFAULT_LIMIT_TIME_VALUE      1800U    /* 3min = 3min x 60sec = 180, @100msec */
#define  DEFAULT_COMM_ERR_TIMER        30U      /* 3sec, @100msec */

typedef struct _end_of_line_
{
    U8 Status;         /* START or STOP */
    U8 Type;           /* TYPE, EOL_TYPE_FRONT, EOL_TYPE_LOAD */
    U8 Mode;           /* READY, LOAD */
    
    U8 TestMode;        /* TEST_1, TEST_2 */

    /* CHECK INPUT */
    U32 InputTest;      /* Input Test Value */

    /* LOAD TEST */
    U8 mu8LoadComm;    /* START or STOP - 부하 검사 통신 시작 */
    
    
    U8 DrainWater;     /* START or STOP - 냉수 탱크 비움 */  

    /* Error */
    U8 ErrComm;

    U16 TestTimer;
    U16 LimitTimer;
    U16 CommErrTimer;

    /* CHECK STATUS */
    U16 CheckList[ EOL_CHK_ID_NUM ];
} Eol_T;

Eol_T eol;


void InitEol(void)
{
    eol.Status       = FALSE; 
    eol.LimitTimer   = DEFAULT_LIMIT_TIME_VALUE;
    eol.TestTimer    = DEFAULT_TEST_TIME_VALUE;
    eol.CommErrTimer = DEFAULT_COMM_ERR_TIMER;

    eol.Mode         = EOL_MODE_READY;
    eol.DrainWater   = FALSE;

    eol.TestMode     = EOL_TEST_MODE_0;

    // 초기 값 설정을 0x01(에러)로 설정한다.
    SetEolCheckStatus( EOL_CHK_ID_ICE_FULL, 1 );
    //SetEolCheckStatus( EOL_CHK_ID_SODA_PUMP, 0 );
    SetEolCheckStatus( EOL_CHK_ID_DRAIN_PUMP, 0 );      
    SetEolCheckStatus( EOL_CHK_ID_UV_ICE_TANK, 0 );
    SetEolCheckStatus( EOL_CHK_ID_UV_ICE_TRAY, 0 );
    SetEolCheckStatus( EOL_CHK_ID_FILTER_ONOFF, 0 );
    SetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_DEICING, 0 );
    SetEolCheckStatus( EOL_CHK_ID_ICE_TRAY_ICING, 0 );
    SetEolCheckStatus( EOL_CHK_ID_SYSTEM_COLD, 0 );
    SetEolCheckStatus( EOL_CHK_ID_SYSTEM_ICE,  0 );
    SetEolCheckStatus( EOL_CHK_ID_LEAK,  0 );
    SetEolCheckStatus( EOL_CHK_ID_EEPROM,  0xA5 );
}


void StartEol(U8 mu8Type)
{
    if( eol.LimitTimer == 0 )
    {
        return ;
    }

    eol.Status = TRUE;
    eol.Type = mu8Type;
}

U8 IsExpiredEolLimitTimer(void)
{
    if( eol.LimitTimer != 0 )
    {
        return FALSE;   // no expired
    }
    return TRUE;  // expired!!
}

U8 GetEolStatus(void)
{
    return eol.Status;
}

U8 GetEolType(void)
{
    return eol.Type;
}


U8 StartEolLoad(void)
{
    // EOL 테스트 진입 시간이 만료되면,
    // 부저음 없이 Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return FALSE;
    }

    // 현재 EOL 테스트 중이면, Pass
    if( GetEolStatus() == TRUE )
    {
        return FALSE;
    }

    // EOL 시작
    StartEol( EOL_TYPE_LOAD );
    StartDisplayInit();
    SetVersionDisp( 0 );
    ClearHotHeaterOnTime();

    // WIFI 
    WifiKey(WIFI_KEY_TEST);

    return TRUE;
}

void SetEolMode(U8 mu8Mode)
{
    eol.Mode = mu8Mode;
}

U8 GetEolMode(void)
{
    return eol.Mode;
}

void SetEolTestMode(U8 mu8Mode)
{
    eol.TestMode = mu8Mode;
}

U8 GetEolTestMode(void)
{
    return eol.TestMode;
}

void SetEolTestTimer(U16 mu16Time)
{
    eol.TestTimer = mu16Time;
}

U16 GetEolTestTimer(void)
{
    return eol.TestTimer;
}



// Drain Water
void StartEolDrainWater(void)
{
    eol.DrainWater = TRUE;
}

void StopEolDrainWater(void)
{
    eol.DrainWater = FALSE;
}

U8 GetEolDrainWater(void)
{
    return eol.DrainWater;
}


// Test Input
void SetEolTestInputBit(U32 mu32MaskBit)
{
    eol.InputTest |= mu32MaskBit;
}

void ClearEolTestInputBit(U32 mu32MaskBit)
{
    eol.InputTest &= ~mu32MaskBit;
}

void SetEolTestInputVal(U32 mu32MaskBit)
{
    eol.InputTest = mu32MaskBit;
}

U32 GetEolTestInputVal(void)
{
    return eol.InputTest;
}


// Error
void SetEolErrorComm(U8 mu8Error)
{
    eol.ErrComm = mu8Error;
}

U8 GetEolErrorComm(void)
{
    return eol.ErrComm;
}



U16 GetEolCheckStatus(U8 mu8Id )
{
    if( mu8Id >= EOL_CHK_ID_NUM )
    {
        return 0xFF; // index error
    }

    return eol.CheckList[ mu8Id ];
}

void SetEolCheckStatus(U8 mu8Id, U16 mu16Val)
{
    if( mu8Id >= EOL_CHK_ID_NUM )
    {
        return ;
    }

    eol.CheckList[ mu8Id ] = mu16Val;
}

void UpdateEolTimer(void)
{
    if( eol.LimitTimer != 0 )
    {
        eol.LimitTimer--;
    }

    if( eol.TestTimer != 0 )
    {
        eol.TestTimer--;
    }
}

