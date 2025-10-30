#ifndef __WATER_OUT_TYPE_H__
#define __WATER_OUT_TYPE_H__

#define TIME_SEC(x) ( x * 100UL )
#define TIME_100MS(x) ( x * 10UL )

// 추출량 타입
typedef enum
{
    OUT_AMOUNT_0,      // 추출량 선택이 없는 경우, 레버 추출 같은 경우...
    OUT_AMOUNT_10,     // 10ml..
    OUT_AMOUNT_120,    // 120ml..
    OUT_AMOUNT_250,    // 250ml..
    OUT_AMOUNT_500,
    OUT_AMOUNT_1000,
//    OUT_AMOUNT_USER,
	OUT_AMOUNT_HALF_WIFI,
	OUT_AMOUNT_ONE_WIFI,
	OUT_AMOUNT_TWO_WIFI,
    OUT_AMOUNT_INFINITY,   // 3분 추출
    OUT_AMOUNT_CLEAN,      // 청소 모드
    OUT_AMOUNT_ICE_TRAY,   // ICE TRAY 입수량

    OUT_AMOUNT_FLUSH_FILTER,
    OUT_AMOUNT_FLUSH_HEATER,
    OUT_AMOUNT_FLUSH_COOLER,
    OUT_AMOUNT_FLUSH_COOLER_DRAIN,
    OUT_AMOUNT_FLUSH_CHECK_HEATER,      // 플러싱 + 온수 히터 검사 시점 유량
    OUT_AMOUNT_CHECK_HEATER,            // 온수만 채울때 히터 검사 시점 유량

    OUT_AMOUNT_NUM,
}WaterOutType_T;

typedef struct _water_out_common_
{
    U8  Out;        // 현재 추출 명령 상태(TRUE, FALSE)
    U8  PrevOut;    // 이전 추출 명령 상태

    WaterOutType_T  Type;   // 추출량 타입
    U32 TargetAmount;   // 추출량 타입에 따른 추출 목표량
    U32 PaddingAmount;  // 오차 보정 추출량
    U32 CurrentAmount;  // 현재 추출량

    U8 OpenStep;
    U8 CloseStep;
    U16 WaitTime;

} WaterOut_T;


// 추출 테이블 
typedef struct _out_table_
{
    WaterOutType_T  Type;
    U32 Amount;
} WaterOutTable_T;


// xx_WATER_OUT RETURN VALUE
enum
{
    WATER_OUT_GOING,
    WATER_OUT_DONE,
};

#endif /* __WATER_OUT_TYPE_H__ */
