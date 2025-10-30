#include "error.h"
#include "eeprom.h"
#include "rtc.h"
#include "sound.h"
#include "flow_meter.h"
#include "ice.h"
#include "eol.h"
#include "process_display.h"


Error_T  Err;

void InitError(void)
{
    MEMSET( (void __FAR *)&Err, 0, sizeof( Error_T ));
}

void GetErrorData(Error_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Err, sizeof( Error_T));
}

void SetErrorStatus(ErrorId_T mId, U8 mu8Status)
{
    Err.Status[ mId ] = mu8Status;
}

void SetErrorStatusDate(ErrorId_T mId, U8 mu8Status, ErrorDate_T *pDate)
{
    if( mId < ERR_NUM ) 
    {
        Err.Status[ mId ] = mu8Status;
        MEMCPY( (void __FAR *)&Err.StatusDate[ mId ], (const void __FAR *)pDate, sizeof( ErrorDate_T ));
    }
}

U8 GetErrorStatus(ErrorId_T mId)
{
    return Err.Status[ mId ];
}

void GetErrorStatusDate(ErrorId_T mId, ErrorDate_T *pDate)
{
    if( mId < ERR_NUM ) 
    {
        MEMCPY(  (void __FAR *)pDate, (const void __FAR *)&Err.StatusDate[ mId ], sizeof( ErrorDate_T ));
    }
}

U8	IsError(ErrorId_T mId )
{
    return Err.Status[ mId ];
}

ErrorId_T	GetErrorId(void)
{
    return Err.ErrorId;
}

void SetErrorId(ErrorId_T mErrId)
{
    Err.ErrorId = mErrId;
}


void GetErrorDate(ErrorDate_T *pDate)
{
    TimeData_T mTime;

    if( pDate != NULL )
    {
        GetRtcTime( &mTime );

        pDate->u16Year = mTime.Year + 2000;
        pDate->u8Month = mTime.Month;
        pDate->u8Date = mTime.Date;
    }
}

U8 IsEmptyMementoError(void)
{
    U8 mu8Num;
    Memento_T mMemento;

    mu8Num = MEMENTO_LIST_NUM - 1;
    GetMementoError( mu8Num, &mMemento );
    if( mMemento.Error == ERR_NONE )
    {
        return TRUE;
    }

    return FALSE;
}

U8	GetMementoError(U8 mu8Index, Memento_T *pMemento)
{
    if( mu8Index < MEMENTO_LIST_NUM )
    {
        MEMCPY( (void __FAR *)pMemento, (const void __FAR *)&Err.MementoList[ mu8Index ], sizeof( Memento_T ));
        return TRUE;
    }

    return FALSE;
}

void	SetMementoError(U8 mu8Index, ErrorId_T mError, ErrorDate_T *pDate )
{
    if( mu8Index >= MEMENTO_LIST_NUM )
    {
        return ;
    }

    if( mError >= ERR_NUM )
    {
        return ;
    }

    Err.MementoList[ mu8Index ].Error = mError;
    MEMCPY( (void __FAR *)&Err.MementoList[ mu8Index ].Date, (const void __FAR *)pDate, sizeof( ErrorDate_T ));
}

void ClearMementoError(void)
{
    U8 i;
    ErrorDate_T mDate;

    MEMSET( (void __FAR *)&mDate, 0, sizeof( ErrorDate_T ));
    for( i = 0; i < MEMENTO_LIST_NUM ; i++ )
    {
        SetMementoError( i, ERR_NONE, &mDate );
        SaveEepromId( EEP_ID_MEMENTO_1 );
        SaveEepromId( EEP_ID_MEMENTO_2 );
        SaveEepromId( EEP_ID_MEMENTO_3 );
        SaveEepromId( EEP_ID_MEMENTO_4 );
        SaveEepromId( EEP_ID_MEMENTO_5 );
    }
}

static void UpdateMementoError( Memento_T *pMemento )
{
    if( pMemento->Error != ERR_NONE )
    {
        // 바로 직전 에러와 현재 에러가 같으면 메멘토 리스트에
        // 추가하지 않는다. 
        if( Err.MementoList[ MEMENTO_LIST_NUM - 1 ].Error != pMemento->Error )
        {
            // 0번이 가장 마지막 에러
            // 4번이 가장 최근 에러
            MEMCPY( (void __FAR *)&Err.MementoList[ 0 ], (const void __FAR *)&Err.MementoList[1], sizeof( Memento_T ) * (MEMENTO_LIST_NUM - 1) ); 
            MEMCPY( (void __FAR *)&Err.MementoList[ MEMENTO_LIST_NUM - 1 ], (const void __FAR *)pMemento, sizeof( Memento_T ) );

            SaveEepromId( EEP_ID_MEMENTO_1 );
            SaveEepromId( EEP_ID_MEMENTO_2 );
            SaveEepromId( EEP_ID_MEMENTO_3 );
            SaveEepromId( EEP_ID_MEMENTO_4 );
            SaveEepromId( EEP_ID_MEMENTO_5 );
        }
    }
}

static U8 IsErrRoomOut(void);
static U8 IsErrColdOut(void);
static U8 IsErrHotOut(void);

static U8 IsErrColdMake(void);
static U8 IsErrHotMake(void);
static U8 IsErrIceMake(void);
static U8 IsErrDrain(void);
static U8 IsErrEmptyColdTank(void);
static U8 IsErrSter(void);

U8 IsErrorType( ErrType_T mType )
{
    U8 IsError = FALSE;

    switch( mType )
    {
        case ERR_TYPE_ROOM_WATER_OUT:
            if( GetEolStatus() == FALSE )
            {
                IsError = IsErrRoomOut();
            }
            break;

        case ERR_TYPE_COLD_WATER_OUT:
            if( GetEolStatus() == FALSE )
            {
                IsError = IsErrColdOut();
            }
            break;

        case ERR_TYPE_HOT_WATER_OUT:
            if( GetEolStatus() == FALSE )
            {
                IsError = IsErrHotOut();
            }
            break;

        case ERR_TYPE_COLD_MAKE:
            IsError = IsErrColdMake();
            break;

        case ERR_TYPE_HOT_MAKE:
            IsError = IsErrHotMake();
            break;

        case ERR_TYPE_ICE_MAKE:
            IsError = IsErrIceMake();
            break;

        case ERR_TYPE_DRAIN:
            IsError = IsErrDrain();
            break;

        case ERR_TYPE_EMPTY_COLD:
            IsError = IsErrEmptyColdTank();
            break;

        case ERR_TYPE_STER:
            IsError = IsErrSter();
            break;

        default:
            break;
    }

    return IsError;
}

static U8 IsErrLeak(void)
{
    if(  IsError( ERR_LEAK ) == TRUE 
            || IsError( ERR_LEAK_FLOW ) == TRUE 
      )
    {
        return TRUE;
    }

    return FALSE;
}

// 온수 추출 불가 조건
static U8 IsErrRoomOut(void)
{
    if( IsErrLeak() == TRUE )
    {
        return TRUE;
    }

    if( IsError( ERR_OUTAGE_WATER ) == TRUE)
    {
        return TRUE;
    }

    return FALSE;
}

static U8 IsErrColdOut(void)
{
    if( IsErrLeak() == TRUE )
    {
        return TRUE;
    }

    if(  IsError( ERR_OUTAGE_WATER ) == TRUE
          //  || IsError( ERR_TEMP_COLD_WATER ) == TRUE 
          //  || IsError( ERR_TEMP_AMBIENT ) == TRUE
          //  || IsError( ERR_COMP ) == TRUE
          //  || IsError( ERR_COMP_COMM ) == TRUE
      )
    {
        return TRUE;
    }
    return FALSE;
}

static U8 IsErrHotOut(void)
{
    if( IsErrLeak() == TRUE )
    {
        return TRUE;
    }

    if( IsError( ERR_TEMP_HOT_WATER ) == TRUE
            || IsError( ERR_TEMP_HOT_WATER_IN ) == TRUE
            || IsError( ERR_TEMP_HEATER ) == TRUE
            //|| IsError( ERR_HEATER_OVERHEAT ) == TRUE
            || IsError( ERR_OUTAGE_HOT_WATER ) == TRUE
            || IsError( ERR_OUTAGE_WATER ) == TRUE
      )
    {
        return TRUE;
    }

    return FALSE;
}

static U8 IsErrHotMake(void)
{
    if( IsErrLeak() == TRUE )
    {
        return TRUE;
    }

    if( IsError( ERR_OUTAGE_HOT_WATER ) == TRUE
        //    || IsError( ERR_HEATER_OVERHEAT ) == TRUE
      )
    {
        return TRUE;
    }
    return FALSE;
}

static U8 IsErrColdMake(void)
{
    if(  IsError( ERR_TEMP_COLD_WATER ) == TRUE 
            || IsError( ERR_TEMP_AMBIENT ) == TRUE
            || IsError( ERR_COMP ) == TRUE
            || IsError( ERR_COMP_COMM ) == TRUE
            || IsError( ERR_PROTECT_COMP ) == TRUE
            || IsError( ERR_PROTECT_COMP_COMM ) == TRUE
            || IsError( ERR_LEAK ) == TRUE
            || IsError( ERR_COMP_CURRENT_SENSING ) == TRUE
            || IsError( ERR_COMP_STARTING_FAIL ) == TRUE
            || IsError( ERR_COMP_OVER_CURRENT ) == TRUE
            || IsError( ERR_COMP_OVERHEAT ) == TRUE
            || IsError( ERR_COMP_DISCONNECTION_OF_COMM ) == TRUE
            || IsError( ERR_COMP_ABNORMAL_VOLTAGE ) == TRUE
      )
    {
        return TRUE;
    }

    return FALSE;
}

static U8 IsErrIceMake(void)
{
    if( IsErrLeak() == TRUE )
    {
        return TRUE;
    }

    if( IsError( ERR_TEMP_ROOM_WATER ) == TRUE
            || IsError( ERR_TEMP_AMBIENT ) == TRUE
            || IsError( ERR_COMP ) == TRUE
            || IsError( ERR_COMP_COMM ) == TRUE
            || IsError( ERR_PROTECT_COMP ) == TRUE
            || IsError( ERR_PROTECT_COMP_COMM ) == TRUE
            || IsError( ERR_OUTAGE_WATER ) == TRUE
            || IsError( ERR_DRAIN_PUMP ) == TRUE 
            || IsError( ERR_MICRO_SW_DETECT ) == TRUE 
            || IsError( ERR_MICRO_SW_MOVE ) == TRUE 
            || IsError( ERR_COMP_CURRENT_SENSING ) == TRUE
            || IsError( ERR_COMP_STARTING_FAIL ) == TRUE
            || IsError( ERR_COMP_OVER_CURRENT ) == TRUE
            || IsError( ERR_COMP_OVERHEAT ) == TRUE
            || IsError( ERR_COMP_DISCONNECTION_OF_COMM ) == TRUE
            || IsError( ERR_COMP_ABNORMAL_VOLTAGE ) == TRUE
      )
    {
        return TRUE;
    }

    return FALSE;
}

static U8 IsErrDrain(void)
{
    if(  IsError( ERR_LEAK ) == TRUE 
            || IsError( ERR_LEAK_FLOW ) == TRUE 
            || IsError( ERR_OUTAGE_WATER ) == TRUE 
            || IsError( ERR_DRAIN_PUMP ) == TRUE 
      )
    {
        return TRUE;
    }

    return FALSE;
}

static U8 IsErrEmptyColdTank(void)
{
    if(  IsError( ERR_LEAK ) == TRUE 
            || IsError( ERR_LEAK_FLOW ) == TRUE 
            || IsError( ERR_DRAIN_PUMP ) == TRUE 
      )
    {
        return TRUE;
    }

    return FALSE;
}

static U8 IsErrSter(void)
{
    if(  IsError( ERR_LEAK ) == TRUE 
            || IsError( ERR_LEAK_FLOW ) == TRUE 
            || IsError( ERR_OUTAGE_WATER ) == TRUE 
            || IsError( ERR_DRAIN_PUMP ) == TRUE 
      )
    {
        return TRUE;
    }

    return FALSE;
}


static void SoundError(U8 mu8Error)
{
    switch( mu8Error )
    {
        case ERR_LEAK:
        case ERR_LEAK_FLOW:
            Sound( SOUND_ERROR_LEAK );
            break;

        case ERR_TEMP_ROOM_WATER:
        case ERR_DRAIN_PUMP:
        case ERR_MICRO_SW_MOVE:
        case ERR_MICRO_SW_DETECT:
            Sound( SOUND_ERROR_ICE );
            break;

        case ERR_COMP:    
        case ERR_COMP_COMM:
        case ERR_TEMP_AMBIENT:
            // 냉각, 얼음 공통 부하는 얼음부터 우선으로 알람표시.
            // 단, 얼음 off되어 있으면 냉각 에어로 출력
            if( GetIceConfigMake() == TRUE )
            {
                Sound( SOUND_ERROR_ICE );
            }
            else 
            {
                Sound( SOUND_ERROR_COLD );
            }
            break;

        case ERR_TEMP_COLD_WATER:
            Sound( SOUND_ERROR_COLD );
            break;

        case ERR_TEMP_HOT_WATER:
        case ERR_TEMP_HOT_WATER_IN:
        case ERR_TEMP_HEATER:
            Sound( SOUND_ERROR_HOT );
            break;

        case ERR_OUTAGE_HOT_WATER:      
        case ERR_OUTAGE_WATER:
            Sound( SOUND_ERROR_OUTAGE );
            break;

        default:
            break;
    }
}

void UpdateNewError(U8 mu8Error)
{
    Memento_T mMemento;

    // EVA 에러는 Skip
    if( mu8Error == ERR_TEMP_EVA_1 )
    {
        return ;
    }

    if( Err.ErrorId != mu8Error ) 
    {
        Err.PrevErrorId = Err.ErrorId;

        mMemento.Error = mu8Error;
        GetErrorStatusDate( mMemento.Error, &mMemento.Date );

        UpdateMementoError( &mMemento );
        
        SoundError( mu8Error );
        StartDispTimerId( DISP_TIMER_ERROR );
    }

    Err.ErrorId = mu8Error;
}


void ClearOutageHotError(void)
{
    // 단수 해제...
    if( GetErrorStatus( ERR_OUTAGE_HOT_WATER ) == TRUE )
    {
        SetErrorStatus( ERR_OUTAGE_HOT_WATER, FALSE );
        SetOutageStatus( TYPE_HOT_WATER, FLOW_SUPPLY );
    }
}

// 단수 에러 해제
void ClearOutageError(void)
{
    // 단수 해제...
    if( GetErrorStatus( ERR_OUTAGE_WATER ) == TRUE )
    {
        SetErrorStatus( ERR_OUTAGE_WATER, FALSE );
        SetOutageStatus( TYPE_WATER, FLOW_SUPPLY );
    }
}
