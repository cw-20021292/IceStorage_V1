#ifndef __PARSER_MAIN_H__
#define __PARSER_MAIN_H__

#include "prj_type.h"
#include "comm.h"


I16 IsValidPkt_Main( U8 *buf, I16 len );
I16 ParserPkt_Main( U8 *buf, I16 len);
I16 Crc16_Main( U8 *buf, I16 len );
I16 MakePkt_Main( CommHeader_T *p_comm, U8 *buf );


#endif /* __PARSER_MAIN_H__ */
