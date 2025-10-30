#ifndef __ICE_TRAY_TEST_H__
#define __ICE_TRAY_TEST_H__

#include "prj_type.h"


void InitIceTrayTest(void);

/* 아이스 트레이 장착 테스트 */
void StartIceTrayTest(void);

void StopIceTrayTest(void);


U8 GetIceTrayTestStatus(void);
U8 GetIceTrayTestCheckIcing(void);
U8 GetIceTrayTestCheckDeIcing(void);

U8 IsIceTrayTest(void);


void ProcessIceTrayTest(void);

#endif /* __ICE_TRAY_TEST_H__ */
