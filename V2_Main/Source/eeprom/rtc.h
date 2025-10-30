#ifndef __RTC_H__
#define __RTC_H__

#include "prj_type.h"

/* MASK BIT OF STATUS REGISTER */
#define  MASK_BAT    0x80
#define  MASK_AL1    0x40
#define  MASK_AL0    0x20
#define  MASK_OSCF   0x10
#define  MASK_RWEL   0x04
#define  MASK_WEL    0x02
#define  MASK_RTCF   0x01

#define  MASK_MIL    0x80

/* RTC Memory Map  ----------------------------------------------------------*/
// RTC ADDRESS                  
#define ADDR_RTC_STATUS   0x003F
#define ADDR_RTC_Y2K      0x0037
#define ADDR_RTC_DAY      0x0036
#define ADDR_RTC_YEAR     0x0035
#define ADDR_RTC_MONTH    0x0034
#define ADDR_RTC_DATE     0x0033
#define ADDR_RTC_HOURS    0x0032
#define ADDR_RTC_MINUTES  0x0031
#define ADDR_RTC_SECONDS  0x0030

// CONTROL ADDRESS
#define ADDR_CTR_PWR      0x0014  // Power
#define ADDR_CTR_DTR      0x0013  // Digital Trimming
#define ADDR_CTR_ATR      0x0012  // Analog Trimming
#define ADDR_CTR_INT      0x0011  // Interrupt Control & Freq Output 
#define ADDR_CTR_BL       0x0010  // Block Protect Bits

typedef struct _time_
{
    U8 Y2K;       // 19 or 20
    U8 DayWeek;   // 0 ~ 6
    U8 Year;      // 0 ~ 99
    U8 Month;     // 1 ~ 12
    U8 Date;      // 1 ~ 31
    U8 Hour;      // 0 ~ 23
    U8 Min;       // 0 ~ 59
    U8 Sec;       // 0 ~ 59
} TimeData_T;

typedef struct _real_time_clock_
{
    U8 Config;    // 초기화 설정 유무

    //U8 Read;      
    U8 Write;      

    /* Buffer */
    U8 Readbuf_1[8];
    U8 Readbuf_2[8];
    U8 WriteBuf[8];

    /* Time */
    TimeData_T ReadTime;
    TimeData_T WriteTime;

    /* STATUS REGISTER */
    U8 Reg_Status;
    U8 Reg_Power;
} RealTimeClock_T;

void InitRtc(void);

// 시간 설정 여부
void SetRtcConfigInit(U8 mu8Config);
U8 GetRtcConfigInit(void);

void GetRtcData(RealTimeClock_T *pData );

void InitRtcTime(void);

U8 GetRtcRegStatus(void);

void GetRtcTime(TimeData_T *pTime);

void SetRtcTime(TimeData_T *pTime);

void SetRtcUpdate(void);

U8 IsRtcBatBackUpMode(void);


// RETURN FALSE : 정상
// TRUE : 에러 발생
U8 IsRTC_Error(void);

// RETURN FALSE : 정상
// TRUE : 에러 발생
U8 IsRtc_ErrorOscf(void);

I8 GetLastDateByMonth(U16 mYear, U8 mMonth);


// ALARAM TIME 반환 타입 정의
typedef enum
{
    STATUS_ERR_RTC_1,
    STATUS_ERR_RTC_2,
    STATUS_ALARAM,
    STATUS_NOT_YET,
} AlaramStatus_T;
AlaramStatus_T IsAlaramTime(U8 mu8DecHour, U8 mu8DecMin, U8 mu8DecSec);

void UpdateRtcTestTime(void);

void UpdateRtcTime(void);

void ProcessRtc(void);

U8 TestRtc(void);
#endif /* __RTC_H__ */
