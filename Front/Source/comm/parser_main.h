#ifndef __PARSER_MAIN_H__
#define __PARSER_MAIN_H__

#include "prj_type.h"
#include "comm.h"

#define PKT_REQ_MAIN        (0x01)
#define PKT_ACK_MAIN        (0x81)

I16 IsValidPktMain(U8 *buf, I16 len);
I16 ParserPktMain(U8 *buf, I16 len);
I16 Crc16Main(U8 *buf, I16 len);
I16 MakePktMain(CommHeader_T *p_comm, U8 *buf);

I16 IsValidPktSubFront(U8 *buf, I16 len);

#endif /* __PARSER_MAIN_H__ */
