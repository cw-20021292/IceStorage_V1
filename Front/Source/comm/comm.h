#ifndef __COMM_H__
#define __COMM_H__

#include "prj_type.h"
#include "hal_serial.h"

typedef U8 CommHeader_T;

CommHeader_T GetCommHeader( U8 id );
void    SetCommHeader( U8 id, CommHeader_T val );

I16     CommSendPacket( U8 id,  U8 *send_pkt, I16  len );
I8  CommRecvPacket( U8 id , U8 *recv_pkt );

#endif /* __COMM_H__ */
