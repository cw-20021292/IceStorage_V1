#ifndef __PARSER_FRONT_H__
#define __PARSER_FRONT_H__

#include "prj_type.h"
#include "comm.h"


#define PKT_FRONT_REQ_LED       0x01
#define PKT_FRONT_REQ_VOICE     0x02
#define PKT_FRONT_REQ_LED_2     0x03
#define PKT_FRONT_REQ_LED_3     0x04

#define PKT_FRONT_ACK_LED       0x81
#define PKT_FRONT_ACK_VOICE     0x82
#define PKT_FRONT_ACK_LED_2     0x83
#define PKT_FRONT_ACK_LED_3     0x84

I16 IsValidPkt_Front( U8 *buf, I16 len );
I16 ParserPkt_Front( U8 *buf, I16 len);
I16 Crc16_Front( U8 *buf, I16 len );
I16 MakePkt_Front( CommHeader_T *p_comm, U8 *buf );


#endif /* __PARSER_FRONT_H__ */
