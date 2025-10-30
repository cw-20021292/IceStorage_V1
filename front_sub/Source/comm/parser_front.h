#ifndef __PARSER_FRONT_H__
#define __PARSER_FRONT_H__

#include "prj_type.h"
#include "comm.h"

I16 IsValidPktFront(U8 *buf, I16 len);
I16 ParserPktFront(U8 *buf, I16 len);
I16 Crc16Main(U8 *buf, I16 len);
I16 MakePktFront(CommHeader_T *p_comm, U8 *buf);

I16 IsValidPktFront(U8 *buf, I16 len);

#endif /* __PARSER_MAIN_H__ */
