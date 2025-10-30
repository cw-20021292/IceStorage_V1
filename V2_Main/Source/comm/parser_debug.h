#ifndef __PARSER_DEBUG_H__
#define __PARSER_DEBUG_H__

#include "prj_type.h"
#include "comm.h"

void ParserDebugControl( U8 *buf, I16 len);
I16 MakePkt_Debug_1( U8 *buf, U16 mu16PktType );
I16 MakePkt_Debug_2( U8 *buf, U16 mu16PktType );
I16 MakePkt_Debug_3( U8 *buf, U16 mu16PktType );
I16 MakePkt_Debug_4( U8 *buf, U16 mu16PktType );
I16 MakePkt_Debug_5( U8 *buf, U16 mu16PktType );
I16 MakePkt_Debug_6( U8 *buf, U16 mu16PktType );
//I16 MakePkt_Debug_7( U8 *buf, U16 mu16PktType );
#endif /* __PARSER_DEBUG_H__ */
