#ifndef __FLUSH_WATER_OUT_H__
#define __FLUSH_WATER_OUT_H__

#include "prj_type.h"
#include "water_out_type.h"


// 상세 제어 모드
#define FLUSH_OUT_PARING_WIFI    0x0001     // WIFI 페어링
#define FLUSH_OUT_FILTER         0x0002     // 필터 플러싱( CHP )
#define FLUSH_OUT_FILTER_CP      0x0004     // 필터 플러싱(CP)
#define FLUSH_OUT_HEATER         0x0008     // 온수 히터 채우기( CHP )
#define FLUSH_OUT_COLD           0x0010     // 냉수 채우기
#define FLUSH_OUT_FLUSH_COLD     0x0020     // 냉수 채우기 및 탱크 플러싱
#define FLUSH_OUT_CLOSE_VALVE    0x0040     // 밸브 CLOSE 
#define FLUSH_OUT_DONE           0x0080


// OUT SETUP - CHPI
#define FLUSH_OUT_CHP  (\
        FLUSH_OUT_PARING_WIFI   | \
        FLUSH_OUT_FILTER        | \
        FLUSH_OUT_HEATER        | \
        FLUSH_OUT_FLUSH_COLD )

// OUT SETUP - CPI
#define FLUSH_OUT_CP  (\
        FLUSH_OUT_PARING_WIFI   | \
        FLUSH_OUT_FILTER_CP     | \
        FLUSH_OUT_FLUSH_COLD )

// OUT POWER ON - CHPI
#define POWER_ON_CHP  (\
        FLUSH_OUT_HEATER     | \
        FLUSH_OUT_COLD )

// OUT POWER ON - CPI
#define POWER_ON_CP  (\
        FLUSH_OUT_COLD )


// OUT FILTER CHANGED - EXCEPT WIFI
// 최초 플러싱이 아닌 경우에는 wifi paring
// 동작은 제외시킨다.
#define FLUSH_OUT_EXCEPT_SETUP_INIT (\
        FLUSH_OUT_FLUSH_COLD | \
        FLUSH_OUT_PARING_WIFI )


// CANCEL 
#define FLUSH_OUT_CANCEL  (\
        FLUSH_OUT_FILTER    | \
        FLUSH_OUT_FILTER_CP | \
        FLUSH_OUT_HEATER    | \
        FLUSH_OUT_COLD | \
        FLUSH_OUT_FLUSH_COLD )


typedef enum
{
    FLUSH_STATUS_INIT,    // 초기화 미완료
    FLUSH_STATUS_DONE,    // 완료
} FlushStatus_T;


typedef struct _flush_water_out_
{
    FlushStatus_T  SetupInit;   // 최초 설치 1회
    FlushStatus_T  Status;      // 필터 교체 플러싱
    FlushStatus_T  PowerOn;      // 전원 리셋 자동 채우기

    U8  Out;                // 현재 추출 명령 상태(TRUE, FALSE)
    U8  PrevOut;            // 이전 추출 명령 상태

    U16 Mode;               // 상세 제어 모드

    U32 TargetAmount;       // 추출량 타입에 따른 추출 목표량
    U32 FilterTargetAmount;
    U32 FlushColdTargetAmount;
    U32 CurrentAmount;

    U16 FilterTimeOut;      // 최대 동작 시간
    U16 ColdTimeOut;        // 최대 동작 시간( 냉수 탱크 채우기 )
    U16 FlushColdTimeOut;   // 최대 동작 시간( 냉수 탱크 비우기 )
    U16 DoneWaitTime;       // 완료 후 대기 시간.. ( 완료 표시 잠깐 보여줘야함 )
} FlushOut_T;

void InitFlushOut(void);

// Get/Set Flush Setupt Init
void SetFlushSetupInit(FlushStatus_T mu8Status);
FlushStatus_T GetFlushSetupInit(void);

// Get/Set Flush Status
FlushStatus_T GetFlushStatus(void);
void SetFlushStatus(FlushStatus_T mu8Status);

void SetFlushPowerOn(FlushStatus_T mu8Status);
FlushStatus_T GetFlushPowerOn(void);

// Get/Set Flush Current Amount
U32 GetFlushCurrentAmount(void);
void SetFlushCurrentAmount(U32 mu32Amount);

// Flush 완료 여부
U8 IsDoneFlushStatus(void);

// Flush 및 초기 배수 완료 여부
U8 IsDoneFlushInitSetup(void);

// Reset FlushWaterOut
// 플러싱 초기화
void ResetFlushWaterOut(void);

// 추출 시작 명령
// mType : 추출 타입 - unused...
void TurnOnFlushOut(WaterOutType_T mType);

// 추출 상태
// TRUE : 추출, FALSE, 정지
U8 IsFlushOut(void);

// 추출 중지 명령
void TurnOffFlushOut(void);

U8 GetFlushOutPercent(void);

// 추출량 ml 단위 반환
U16 GetFlushOutAmount(void);

// 추출 제어
U8 ControlWaterFlushOut(void);

// 제어 모드 
void SetFlushOutMode( U16 mu16Mode );
U16 GetFlushOutMode(void );
void ClearFlushOutMode( U16 mu16Mode );
U8 IsSetFlushOutMode(U16 mu8Mode );

#endif /* __FLUSH_WATER_OUT_H__ */
