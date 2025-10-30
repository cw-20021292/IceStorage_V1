#ifndef __EMPTY_COLD_TANK_H__
#define __EMPTY_COLD_TANK_H__

#include "prj_type.h"

// TYPE COLD EMPTY
typedef enum
{
    TYPE_FAUCET,    // 추출 파우셋으로 동작
    TYPE_DRAIN      // 배수 드레인으로 동작
} ColdEmptyType_T;

typedef struct _empty_cold_tank_
{
    U8 Start;     // 배수 동작 ( 0: 중지, 1: 시작 )

    // Do EMPTY 
    ColdEmptyType_T Type;
    U8 Step;
    U16 ExitTime;

} EmptyColdTank_T;



void  InitEmptyColdTank(void);
void  GetEmptyColdTankData(EmptyColdTank_T *pData);

void StartEmptyColdTank(ColdEmptyType_T mu8Type);
//void StartEmptyColdTank(void);
void StopEmptyColdTank(void);
U8 IsStartEmptyColdTank(void);
ColdEmptyType_T GetEmptyColdTankType(void);

void ControlEmptyColdTank(void);

#endif /* __EMPTY_COLD_TANK_H__ */
