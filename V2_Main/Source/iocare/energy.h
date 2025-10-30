#ifndef __ENERGY_H__
#define __ENERGY_H__

#include "prj_type.h"


void InitEnergy(void);


#if 0
U8 GetEnergyMonth(void);
void SetEnergyMonth(U8 mu8Month);
#endif

// 요금 반환
U32 GetEnergyRateMonth(U8 mu8Month);

// 소비 전력 반환
U32 GetEnergyWattAll(void);
#if 0
U32 GetEnergyWattMonth(U8 mu8Month);
void SetEnergyWattMonth(U8 mu8Month, U32 mu32Watt);
#endif

// 소비 전력 초기화
void ClearAllEnergyWatt(void);

void ProcessEnergy(void);

#endif /* __ENERGY_H__ */
