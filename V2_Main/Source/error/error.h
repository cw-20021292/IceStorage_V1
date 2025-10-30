#ifndef __ERROR_H__
#define __ERROR_H__

#include "prj_type.h"


/* Error Id List
 *
 * Top - Low priority for error display 
 * Bottom - Top Priority for error dispaly
 */
typedef enum _error_id_
{
    ERR_NONE,

    ERR_TEMP_EVA_1,
    ERR_MICRO_SW_MOVE,
    ERR_MICRO_SW_DETECT,
    ERR_DRAIN_PUMP,

    ERR_TEMP_HEATER,    
    ERR_TEMP_HOT_WATER,
    ERR_TEMP_HOT_WATER_IN,    
    ERR_TEMP_COLD_WATER,
    ERR_TEMP_AMBIENT,
    ERR_TEMP_ROOM_WATER,

    ERR_COMP,               /* BLDC COMPRESSOR */
    ERR_COMP_COMM,          /* BLDC COMM. ERR */

    ERR_PROTECT_COMP,       /* PROTECTION BLDC COMPRESSOR */
    ERR_PROTECT_COMP_COMM,  /* PROTECTION BLDC COMM. */

	/*.. sean [24-10-14] Comp Error 동작 세분화..*/
	ERR_COMP_CURRENT_SENSING,
	ERR_COMP_STARTING_FAIL,
	ERR_COMP_OVER_CURRENT,
	ERR_COMP_OVERHEAT,
	ERR_COMP_DISCONNECTION_OF_COMM,
	ERR_COMP_ABNORMAL_VOLTAGE,

    ERR_OUTAGE_WATER,
    ERR_OUTAGE_HOT_WATER,
    ERR_HEATER_OVERHEAT,    /* 온수 과열 */

    ERR_LEAK,               /* 누수 감지 - 누수 센서 */
    ERR_LEAK_FLOW,          /* 누수 감지 - 유량 센서 */


    //ERR_OUTAGE_ICE_TRAY,
    //ERR_TEMP_EVA_2,
    //ERR_COLD_HIGH_LEVEL,   /* high */
    //ERR_COLD_OVF,          /* feed valve */
    //ERR_SODA_PUMP,
    //ERR_ROOM_COMPLEX,      /* high, low */
    //ERR_ROOM_HIGH_LEVEL,   /* high */
    //ERR_ROOM_LOW_LEVEL,    /* low */
    //ERR_ROOM_OVF,          /* feed valve */

    //ERR_TEMP_TDS_IN,        /* TDS-IN Temperture */
    //ERR_TEMP_TDS_OUT,       /* TDS-OUT Temperture */
    //ERR_TDS_IN,             /* TDS-IN Resistor Sensor */
    //ERR_TDS_OUT,            /* TDS-OUT Resistor Sensor */

    //ERR_COMM_FRONT_DISPLAY,
    //ERR_COMM_LCD,
    //ERR_COMM_WIFI,

    //ERR_NFC_COMM,
    //ERR_STR_MOUDLE,
    //ERR_CIRCULATE_PUMP,
    ERR_NUM              
} ErrorId_T;

/* ERROR ISSUED DATE */
typedef struct _error_date_
{
    U16 u16Year;
    U8 u8Month;
    U8 u8Date;
} ErrorDate_T;

typedef struct _memento_
{
    ErrorId_T   Error;
    ErrorDate_T Date;
} Memento_T;

/* ERROR TYPE */
#define  MEMENTO_LIST_NUM   5
typedef struct _error_
{
    ErrorId_T PrevErrorId;    /* Previous ERROR STATUS */
    ErrorId_T ErrorId;        /* Current ERROR STATUS */
    ErrorDate_T ErrorDate;    /* Current ERROR ISSUED DATE */

    U8 Status[ ERR_NUM ];     /* TRUE : ERROR, FALSE : NORMAL */
    ErrorDate_T StatusDate[ ERR_NUM ];

    /* Memento */
    //ErrorId_T MementoList[ MEMENTO_LIST_NUM ];  
    Memento_T MementoList[ MEMENTO_LIST_NUM ];  
}Error_T;


void InitError(void);

void GetErrorData(Error_T *pData);

void SetErrorStatus(ErrorId_T mId, U8 mu8Status);
void SetErrorStatusDate(ErrorId_T mId, U8 mu8Status, ErrorDate_T *pDate);

U8 GetErrorStatus(ErrorId_T mId);
void GetErrorStatusDate(ErrorId_T mId, ErrorDate_T *pDate);

U8 IsError(ErrorId_T mId );
ErrorId_T GetErrorId(void);
void SetErrorId(ErrorId_T mErrId);
void GetErrorDate(ErrorDate_T *pDate);


// 메멘토 에러 리스트 갑싱 있으면 TRUE 반환
U8 IsEmptyMementoError(void);
U8 GetMementoError(U8 mu8Index, Memento_T *pMemento);
void SetMementoError(U8 mu8Index, ErrorId_T mError, ErrorDate_T *pDate );
void ClearMementoError(void);


// ERROR TYPE
typedef enum
{
    ERR_TYPE_ROOM_WATER_OUT,
    ERR_TYPE_COLD_WATER_OUT,
    ERR_TYPE_HOT_WATER_OUT,
    ERR_TYPE_SODA_WATER_OUT,

    ERR_TYPE_COLD_MAKE,
    ERR_TYPE_HOT_MAKE,
    ERR_TYPE_ICE_MAKE,

    ERR_TYPE_DRAIN,
    ERR_TYPE_EMPTY_COLD,
    ERR_TYPE_STER,
} ErrType_T;
U8 IsErrorType(ErrType_T mId );

void UpdateNewError(U8 mu8Error);

void ClearOutageHotError(void);

void ClearOutageError(void);

#endif /* _ERROR_H__ */
