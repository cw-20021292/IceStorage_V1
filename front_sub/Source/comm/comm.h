/**
 * File : comm.h
 * 
 * Uart Commnuication
*/

#ifndef __COMM_H__
#define __COMM_H__

#include "prj_type.h"

#include "hal_serial.h"

typedef U8 CommHeader_T;

void InitComm(void);
void InitCommId(ECommId id);

U8 IsFullRecvBuffer(ECommId id);

void InitRecvLength(ECommId id);

void SetRecvBuffer(ECommId id, U8 data);

void SendByte(ECommId id);
U8 IsCompleteTx(ECommId id);

void SetCommHeader(ECommId id, CommHeader_T cmd);
CommHeader_T GetCommHeader(ECommId id);

I16 SendCommPacket(ECommId id, U8 *sendBuf, I16 len);
I8 RecvCommPacket(ECommId id, U8 *recvBuf);

void SetFrontRxErr(U8 err);
U8 GetMainRxErr(void);
void UpdateFrontRxErrTimer(void);
void DispFrontRxErr(void);

void SetSubFrontRxErr(U8 err);
U8 GetSubFrontRxErr(void);

#endif /* __COMM_H__ */
