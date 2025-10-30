#ifndef __RELAY_H__
#define __RELAY_H__

#include "prj_type.h"

void InitRelay(void);

#define RELAY_COMP            0x0001
#define RELAY_FEEDER_CW       0x0002
#define RELAY_FEEDER_CCW      0x0004
#define RELAY_ICE_TRAY_CW     0x0008
#define RELAY_ICE_TRAY_CCW    0x0010
#define MAX_RELAY_NUM         5

//#define RELAY_ICE_HEATER      0x0004

void TurnOnRelay(U16 mu16Relay);
void TurnOffRelay(U16 mu16Relay);
void TurnOffRelayAll(void);

U8 IsTurnOnRelay(U16 mu16Relay);

U16 GetRelayStatus(void);

void ControlRelayExclusive(void);
void ControlRelayConcurrent(void);

#if 0   // unsued
void TEST_ControlRelay(void);
#endif

#endif /* __RELAY_H__ */
