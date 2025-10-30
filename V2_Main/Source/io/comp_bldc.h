#ifndef __COMP_BLDC_H__
#define __COMP_BLDC_H__

#include "prj_type.h"

#define CONFIG_BLDC_COMP_LG         0
#define CONFIG_BLDC_COMP_SAMSUNG    1

// BLDC COMP ERROR CODE 
#if CONFIG_BLDC_COMP_LG
typedef enum
{
    ERR_BLDC_NONE,    

    ERR_BLDC_SENSING,    
    ERR_BLDC_LOW_VOLATE,  
    ERR_BLDC_SENSORLESS,  

    ERR_BLDC_COMP_CONNECT,
    ERR_BLDC_OVER_CURRENT,
    ERR_BLDC_IPM_FAULT,

    ERR_BLDC_OUT_OF_RANGE,              // 이후 추가되는 에러코드 

    ERR_BLDC_NUM,
} BldcErrorId_T;
#endif  // CONFIG_BLDC_COMP_LG

#if CONFIG_BLDC_COMP_SAMSUNG
typedef enum
{
    ERR_BLDC_NONE,    

    ERR_BLDC_STARTING_FAIL,             // After three attempts to start
    ERR_BLDC_CURRENT_SENSING_ERROR,     // disconnection of comp wire, Or poor current control
    ERR_BLDC_OVER_CURRENT,              // Current exceeds 2.5A peak

    ERR_BLDC_ABNORMAL_VOLTAGE,          // DC link voltge is out of the min 80 V, max 450 V
    ERR_BLDC_OVERHEAT_CONTROLLER,       // IPM temperture exceeds 110'c
    ERR_BLDC_DISCONNECTION_COMM,        // After 2 min of receiving the last data

    ERR_BLDC_OUT_OF_RANGE,              // 이후 추가되는 에러 코드

    ERR_BLDC_NUM
} BldcErrorId_T;
#endif  // CONFIG_BLDC_COMP_SAMSUNG

// BLDC MIN/MAX RPS
#define MIN_RPS     37
#define MAX_RPS     50
typedef unsigned char RPS_T;


typedef struct _comp_bldc_
{
    RPS_T Target;
    RPS_T Current;
    RPS_T Min;
    RPS_T Max;

    BldcErrorId_T   ErrorCode;  // COMP ERR - 에러 코드
    BldcErrorId_T   LastedErrorCode;  // 에러 코드
    U8  ErrorComm;  // COMP ERR - 통신 에러
    U8  Protect;    // COMP PROTECT 
    U8  TestComm;           // 통신 테스트 (FCT)

    U8 OnStableTime;       // ON 동작시 대기 시간
    U8 OffMaxTime;         // COMP 동작시 대기 시간
} CompBldc_T;


void InitCompBldc(void);
void GetCompBldcData( CompBldc_T *pData );

void SetCompBldcCommTest(U8 mu8Test);
U8 GetCompBldcCommTest(void);

void SetCompBldcTargetRps(RPS_T mRps);
RPS_T GetCompBldcTargetRps(void);

void SetCompBldcCurrentRps(RPS_T mRps);
RPS_T GetCompBldcCurrentRps(void);

#define ERROR_COMP_COMM_TIMEOUT    30  // @sec
void SetCompBldcErrorCode(BldcErrorId_T mErrorId);
BldcErrorId_T GetCompBldcErrorCode(void);

void SetCompBldcLastedErrorCode(BldcErrorId_T mErrorId);
BldcErrorId_T GetCompBldcLastedErrorCode(void);

void SetCompBldcErrorComm(U8 mu8Error);
U8 GetCompBldcErrorComm(void);


void SetCompBldcProtect(U8 mu8Protect);
U8 GetCompBldcProtect(void);

U8 IsCompBldcProtectMode(void);

U8 IsTurnOnCompBldc(void);

void ControlCompBldc(void);

#endif /* __COMP_BLDC_H__ */
