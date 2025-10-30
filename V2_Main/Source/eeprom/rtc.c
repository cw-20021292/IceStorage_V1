#include "rtc.h"
#include "hal_eeprom.h"
#include "eeprom.h"
#include "util.h"
#include "power_saving.h"

#include <string.h>


RealTimeClock_T  Rtc;

static void EnableRtcCCR(void);
static void DisableRtcCCR(void);
static void ReadRtcTime(TimeData_T *pTime);
static void WriteRtcTime(TimeData_T *pTime);
static void InitTimeData( TimeData_T *pTime );

void InitRtc(void)
{
    U16 mu16Delay = 30;

    // 만약 배터리 모드로 동작하는 중이면, 재시도...
    do
    {
        Delay_MS( 100 );
        mu16Delay--;
        if( mu16Delay == 0 )
        {
            break;
        }
    } while( IsRtcBatBackUpMode() == TRUE );


    // Write Default setting
    EnableRtcCCR();
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_PWR, 0x00 );      // Trip Voltage 2.2V
    DisableRtcCCR();
    HAL_RTC_ByteRead( DEV_ADDR_RTC, ADDR_CTR_PWR, &Rtc.Reg_Power );


    EnableRtcCCR();
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_DTR, 0x03 );  // Digital Trimming ( +30ppm)
    DisableRtcCCR();

    EnableRtcCCR();
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_ATR, (0x20|0x10) );
    DisableRtcCCR();

    EnableRtcCCR();
    //HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_INT, 0x18 );  // Output freq 32.768khz
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_INT, 0x00 );  // Output freq disabled
    DisableRtcCCR();

    EnableRtcCCR();
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_BL,  0x00 );
    DisableRtcCCR();

    // Init Varibable..
    Rtc.Config = FALSE;
    Rtc.Write = FALSE;
    Rtc.Reg_Status = 0U;

    InitTimeData( &Rtc.ReadTime );

    // Read Time..
    ProcessRtc();
}

void SetRtcConfigInit(U8 mu8Config)
{
    if( Rtc.Config != mu8Config )
    {
        SaveEepromId( EEP_ID_TIME );
    }

    Rtc.Config = mu8Config;
}

U8 GetRtcConfigInit(void)
{
    return Rtc.Config;
}

void GetRtcData( RealTimeClock_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Rtc, sizeof( RealTimeClock_T ));
}

U8 GetRtcRegStatus(void)
{
    return Rtc.Reg_Status;
}

void GetRtcTime(TimeData_T *pTime)
{
    MEMCPY( (void __FAR *)pTime, (const void __FAR *)&Rtc.ReadTime, sizeof( TimeData_T ));
}

void SetRtcTime(TimeData_T *pTime)
{
    MEMCPY( (void __FAR *)&Rtc.WriteTime, (const void __FAR *)pTime, sizeof( TimeData_T ));
    MEMCPY( (void __FAR *)&Rtc.ReadTime, (const void __FAR *)pTime, sizeof( TimeData_T ));

    // Rtc.Write = TRUE;
    WriteRtcTime( pTime );
}

static void EnableRtcCCR(void)
{
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x02 );
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x06 );
    Delay_MS(200);
}

static void DisableRtcCCR(void)
{
    HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x00 );
}


U8 IsRtcBatBackUpMode(void)
{
    U8 mu8Ret = 0;
    U8 mu8Reg = 0;

    mu8Ret = HAL_RTC_ByteRead( DEV_ADDR_RTC, ADDR_RTC_STATUS, &mu8Reg );
    if( mu8Ret != TRUE )
    {
        return TRUE;    // Error
    }

    if( (mu8Reg & MASK_BAT) != 0 )
    {
        return TRUE;   // Battery BackUp Mode
    }

    return FALSE;  // Vcc Mode
}

U8 IsRtc_ErrorOscf(void)
{
    HAL_RTC_ByteRead( DEV_ADDR_RTC, ADDR_RTC_STATUS, &Rtc.Reg_Status);

    if( (Rtc.Reg_Status & MASK_OSCF ) != 0 )
    {
        return TRUE;   // ERROR ( 크리스탈 오류 )
    }

    return FALSE;    // 정상
}

/* RTC IC 오류 확인
 * 
 * Return
 *  TRUE : RTC 오류!!
 *  FALSE : RTC 정상!!
 *
 */
U8 IsRTC_Error(void)
{
    if( (Rtc.Reg_Status & (MASK_OSCF|MASK_RTCF)) == 0 )
    {
        return FALSE; // 정상
    }

    return TRUE; //ERROR
}


// 윤년 계산
static U8 IsLeafYear(U16 mu16Year)
{
    if( (mu16Year % 4) == 0 )
    {
        return TRUE;
    }

    return FALSE;
}


// 달의 마지막 날 계산
static const I8 mu8LastDateList[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static const I8 mu8LastDateLeafYearList[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
I8 GetLastDateByMonth(U16 mYear, U8 mMonth)
{
    if( mMonth > 12 || mMonth == 0 )
    {
        return -1;
    }
    
    if( IsLeafYear( mYear ) == TRUE )
    {
        return mu8LastDateLeafYearList[ mMonth - 1 ];
    }

    return mu8LastDateList[ mMonth - 1 ];

}


static void ReadRtcTime(TimeData_T *pTime)
{
    U8 mu8Ret_1;
    U8 mu8Ret_2;

    // Read time buffer
    mu8Ret_1 = HAL_RTC_SeqRead( DEV_ADDR_RTC, ADDR_RTC_SECONDS, &Rtc.Readbuf_1[0], 8 );
    if( mu8Ret_1 == FALSE )
    {
        return ;
    }

    // Read aging time buffer
    mu8Ret_2 = HAL_RTC_SeqRead( DEV_ADDR_RTC, ADDR_RTC_SECONDS, &Rtc.Readbuf_2[0], 8 );
    if( mu8Ret_2 == FALSE )
    {
        return ;
    }

    // [0] 번지의 sec 값을 제외하고 나머지 시간이 동일하면 시간을 저장함.
    // read 과정에서 시간 값이 noise에 의해서 손상되는 것을 막기 위한 동작 사양임
    if( Rtc.Readbuf_1[1] == Rtc.Readbuf_2[1]
            && Rtc.Readbuf_1[2] == Rtc.Readbuf_2[2]
            && Rtc.Readbuf_1[3] == Rtc.Readbuf_2[3]
            && Rtc.Readbuf_1[4] == Rtc.Readbuf_2[4]
            && Rtc.Readbuf_1[5] == Rtc.Readbuf_2[5]
            && Rtc.Readbuf_1[6] == Rtc.Readbuf_2[6]
            && Rtc.Readbuf_1[7] == Rtc.Readbuf_2[7]
      )
    {

        pTime->Sec      =  Hex2Dec( Rtc.Readbuf_2[0] );
        pTime->Min      =  Hex2Dec( Rtc.Readbuf_2[1] );
        pTime->Hour     =  Hex2Dec( Rtc.Readbuf_2[2] & 0x3F );
        pTime->Date     =  Hex2Dec( Rtc.Readbuf_2[3] );
        pTime->Month    =  Hex2Dec( Rtc.Readbuf_2[4] );
        pTime->Year     =  Hex2Dec( Rtc.Readbuf_2[5] );
        pTime->DayWeek  =  Hex2Dec( Rtc.Readbuf_2[6] );
        pTime->Y2K      =  Hex2Dec( Rtc.Readbuf_2[7] );
    }
}

static void WriteRtcTime(TimeData_T *pTime)
{
    U8 mu8Ret;

    Rtc.WriteBuf[0] =  Dec2Hex( pTime->Sec     );
    Rtc.WriteBuf[1] =  Dec2Hex( pTime->Min     );
    Rtc.WriteBuf[2] =  Dec2Hex( pTime->Hour    ) | MASK_MIL; 
    Rtc.WriteBuf[3] =  Dec2Hex( pTime->Date    );
    Rtc.WriteBuf[4] =  Dec2Hex( pTime->Month   );
    Rtc.WriteBuf[5] =  Dec2Hex( pTime->Year    );
    Rtc.WriteBuf[6] =  Dec2Hex( pTime->DayWeek );
    Rtc.WriteBuf[7] =  Dec2Hex( pTime->Y2K     );

    EnableRtcCCR();
    HAL_RTC_PageWrite( DEV_ADDR_RTC, ADDR_RTC_SECONDS, &Rtc.WriteBuf[0], 8 );
    DisableRtcCCR();
}


/*
 * 현재 RTC 시간과 비교
 *
 * Param 
 *  - mu8DecHour: 시간(Decimal)
 *  - mu8DecMin : 분(Decimal)
 *  - mu8DecSec : 초(Decimal)
 *
 * Return 
 *  STATUS_ERR_RTC_1 : RTC 백업 모드 동작 
 *  STATUS_ERR_RTC_2 : RTC 초기화 
 *  STATUS_ALARAM    : 알람
 *  STATUS_NOT_YET   : 알람 아님
 */
AlaramStatus_T IsAlaramTime(U8 mu8DecHour, U8 mu8DecMin, U8 mu8DecSec)
{
    TimeData_T mTime;
    U32 mu32SrcTime;
    U32 mu32DestTime;


    // Check Batter Bakcup Mode
    if( IsRtcBatBackUpMode() == TRUE )
    {
        return STATUS_ERR_RTC_1; // ERROR
    }

    // Check RTC Error
    if( IsRTC_Error() == TRUE )
    {
        return STATUS_ERR_RTC_2; //ERROR
    }


    GetRtcTime( &mTime );
    mu32SrcTime = (U32)mTime.Hour * 60UL * 60UL;     /* hour x 60min x 60sec */
    mu32SrcTime += (U32)mTime.Min * 60UL;            /* min x 60sec */
    mu32SrcTime += (U32)mTime.Sec;

    mu32DestTime = (U32)mu8DecHour * 60UL * 60UL;
    mu32DestTime += (U32)mu8DecMin * 60UL;
    mu32DestTime += (U32)mu8DecSec;

    if( mu32DestTime <= mu32SrcTime )
    {
        return STATUS_ALARAM;
    }

    return STATUS_NOT_YET;
}

// TEST Code
void UpdateRtcTestTime(void)
{
    TimeData_T mTime;

    GetRtcTime( &mTime );
    mTime.Hour = 23;
    mTime.Min = 59;
    mTime.Sec = 55;

    SetRtcTime( &mTime );
}

#define DW_MON  0
#define DW_TUE  1
#define DW_WED  2
#define DW_THU  3
#define DW_FRI  4
#define DW_SAT  5
#define DW_SUN  6

static void InitTimeData( TimeData_T *pTime )
{
    if( pTime == NULL )
    {
        return ;
    }

    // 2020-1-1(WED), 00h 00m 00s
    pTime->Y2K       = 1;
    pTime->DayWeek   = DW_WED;
    pTime->Year      = 20;
    pTime->Month     = 1;
    pTime->Date      = 1;
    pTime->Hour      = 0;
    pTime->Min       = 0;
    pTime->Sec       = 0;
}

void InitRtcTime(void)
{
    TimeData_T mTime;

    InitTimeData( &mTime );

    SetRtcTime( &mTime );
}

void UpdateRtcTime(void)
{
    // Writting Time to RTC  
    if( Rtc.Write == TRUE )
    {
        Rtc.Write = FALSE;
        WriteRtcTime( &Rtc.WriteTime );
        return ;
    }
}

void ProcessRtc(void)
{
    // Read RTC Power 
    if( Rtc.Reg_Power != 0x00 )
    {
        EnableRtcCCR();
        HAL_RTC_ByteWrite( DEV_ADDR_RTC, ADDR_CTR_PWR, 0x00 );
        DisableRtcCCR();
        HAL_RTC_ByteRead( DEV_ADDR_RTC, ADDR_CTR_PWR, &Rtc.Reg_Power );
    }


    // Read RTC Status
    HAL_RTC_ByteRead( DEV_ADDR_RTC, ADDR_RTC_STATUS, &Rtc.Reg_Status);

    // RTC 크리스탈 에러인 겨우..
    if( IsRtc_ErrorOscf() == TRUE )
    {
        return ;
    }

    // Init RTC Default Time
    if( IsRTC_Error() == TRUE )
    {
        InitRtcTime();

        // 시간 초기화되면 
        // 사용자 설정도 초기화 및 슬립 모드 해제
        SetRtcConfigInit( FALSE );  
        SetSavingConfig( FALSE );

        return ;
    }

    // Writting Time to RTC  
    if( Rtc.Write == TRUE )
    {
        Rtc.Write = FALSE;
        WriteRtcTime( &Rtc.WriteTime );
        return ;
    }

    // Read Time from RTC
    ReadRtcTime( &Rtc.ReadTime );
}

U8 TestRtc(void)
{
    if( IsRtc_ErrorOscf() == FALSE )
    {
        return TRUE;    // 정상
    }

    return FALSE;   // 에러
}
