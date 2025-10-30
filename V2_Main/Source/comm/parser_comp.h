#ifndef __PARSER_COMP_H__
#define __PARSER_COMP_H__

#include "prj_type.h"
#include "comm.h"


#define PKT_COMP_REQ   0x01

I16 IsValidPkt_Comp( U8 *buf, I16 len );
I16 ParserPkt_Comp( U8 *buf, I16 len);
I16 Crc16_Comp( U8 *buf, I16 len );
I16 MakePkt_Comp( CommHeader_T *p_comm, U8 *buf );


#endif /* __PARSER_COMP_H__ */
