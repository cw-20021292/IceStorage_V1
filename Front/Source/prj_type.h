#ifndef __PRJ_TYPE_H__
#define __PRJ_TYPE_H__

#include "config.h"
#include <stdio.h>
#include <string.h>


typedef unsigned char		U8;
typedef signed char			I8;
typedef unsigned short int	U16;
typedef signed short int	I16;
typedef unsigned long int	U32;
typedef signed long int		I32;
typedef double              F32;
typedef unsigned char	   BOOL_T;
typedef unsigned long 	   TIME_T;
//typedef signed short int   TEMP_T;
typedef double   TEMP_T;


#define	__DEBUG__		1

#if __DEBUG__
#define LOCAL 
#else
#define LOCAL static
#endif

#ifdef NULL
#undef	NULL
#endif	/* ifdef NULL */

#define	NULL	((void *)0)

#ifdef TRUE
#undef TRUE
#endif /* ifdef TURE */

#define	TRUE	1

#ifdef FALSE
#undef FALSE 
#endif /* ifdef FALSE */

#define	FALSE 	0


#ifdef ON
#undef ON
#endif

#define	ON		1

#ifdef OFF
#undef	OFF
#endif

#define	OFF		0

#define  HIGH     1
#define  LOW      0

#endif /* __TYPE_H__ */
