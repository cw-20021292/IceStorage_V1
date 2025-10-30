#ifndef __LEVER_H__
#define __LEVER_H__

#include "hal_key.h"

/* Lever ID */
#define  LEVER_ID_WATER    0
#define  LEVER_ID_ICE      1
#define  MAX_LEVER_ID      2

void InitLever(void);

U16 GetLeverVal(U8 mu8Id);

U16 GetLeverPopVal(U8 mu8Id);

/* EvnetFlag Index */
#define  LEVER_EVENT_PUSHED_SHORT     0x01
#define  LEVER_EVENT_PUSHED_LONG      0x02
#define  LEVER_EVENT_PUSHED_LONG_2    0x04
#define  LEVER_EVENT_POP              0x08
#define  LEVER_EVENT_PUSEHD_ALL       0x07
#define  LEVER_EVENT_ALL              0x0F
void SetLeverEventFlag(U8 mu8Id, U8 mu8Event );
void ClearLeverEventFlag(U8 mu8Id, U8 mu8Event );
U8 IsSetLeverEventFlag(U8 mu8Id,  U8 mu8Event );

void ProcessScanLever(void);

#endif /* __LEVER_H__ */
