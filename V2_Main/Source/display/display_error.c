#include "hw.h"
#include "display_error.h"

#include "display.h"
#include "error.h"
#include "comp_bldc.h"




typedef struct _error_display_list_
{
    ErrorId_T  Id;
} ErrDisplay_T;

static ErrDisplay_T ErrDisplayList[] = 
{

    { ERR_MICRO_SW_MOVE     },        
    { ERR_MICRO_SW_DETECT   },      
    { ERR_DRAIN_PUMP        },           
    { ERR_TEMP_HEATER       },
    { ERR_HEATER_OVERHEAT   },      
    { ERR_TEMP_HOT_WATER_IN },    
    { ERR_TEMP_HOT_WATER    },       
    { ERR_TEMP_COLD_WATER   },      
    { ERR_TEMP_AMBIENT      },         
    { ERR_TEMP_ROOM_WATER   },      
    { ERR_COMP              },                 
    { ERR_COMP_COMM         },            
    { ERR_OUTAGE_WATER      },         
    { ERR_OUTAGE_HOT_WATER  },     
    { ERR_LEAK              },                 
    { ERR_LEAK_FLOW         },                  
    { ERR_COMP_CURRENT_SENSING			},                   
    { ERR_COMP_STARTING_FAIL			},                   
    { ERR_COMP_OVER_CURRENT				},                   
    { ERR_COMP_OVERHEAT					},                   
    { ERR_COMP_DISCONNECTION_OF_COMM	},    
    { ERR_COMP_ABNORMAL_VOLTAGE			},            
};
#define  SZ_LIST  ( sizeof(ErrDisplayList) / sizeof(ErrDisplay_T ))


static U8  GetErrorDisplayId(ErrorId_T mErrId);
static void CommonErrCode(ErrorId_T id, U8 mu8Cmd);
void DisplayError(U8 mu8ErrCode, U8 mu8Cmd )
{
    U8 i;


    for( i = 0; i < SZ_LIST ; i++ )
    {
        if( ErrDisplayList[ i ].Id == mu8ErrCode )
        {
            CommonErrCode( mu8ErrCode, mu8Cmd );
            break;
        }
    }
}

static void CommonErrCode(ErrorId_T id, U8 mu8Cmd)
{
    U8 mu8ErrCode;

    mu8ErrCode = GetErrorDisplayId( id );

    if( mu8Cmd == ON )
    {
        DispSegmentError( mu8ErrCode );
    }
    else
    {
        DispSegmentError( 0xFF );
    }
}


// ERR_PROTECT_COMP 는 ERR_COMP의 보호동작이다.
// PROTECT은 에러 표시를 하지 않고, 시스템 제어는 ERR와 동일하다
#if CONFIG_COMP_ERROR_CODE
#include "comp_bldc.h"
#endif
U8  GetErrorDisplayId(ErrorId_T mErrId)
{
    U8 mu8ErrorCode = 0;

    switch( mErrId )
    {
        case ERR_LEAK:                  mu8ErrorCode = 1; break;
        case ERR_LEAK_FLOW:             mu8ErrorCode = 2; break;

        case ERR_OUTAGE_HOT_WATER:      mu8ErrorCode = 8; break;
        case ERR_OUTAGE_WATER:          mu8ErrorCode = 9; break;

#if CONFIG_COMP_ERROR_CODE
        case ERR_COMP:                  mu8ErrorCode = 50 + (U8)GetCompBldcErrorCode(); break;
        case ERR_PROTECT_COMP:          mu8ErrorCode = 50 + (U8)GetCompBldcErrorCode(); break;       
#else
        //case ERR_COMP:                  mu8ErrorCode = 27; break;
        //case ERR_PROTECT_COMP:          mu8ErrorCode = 27; break;       
#endif

        case ERR_COMP_COMM:             mu8ErrorCode = 27; break;
        //case ERR_PROTECT_COMP_COMM:     mu8ErrorCode = 29; break;

        case ERR_HEATER_OVERHEAT:       mu8ErrorCode = 40; break;
        case ERR_TEMP_ROOM_WATER:       mu8ErrorCode = 42; break;
        case ERR_TEMP_AMBIENT:          mu8ErrorCode = 43; break;
        case ERR_TEMP_COLD_WATER:       mu8ErrorCode = 44; break;
        case ERR_TEMP_HOT_WATER_IN:     mu8ErrorCode = 46; break;
        case ERR_TEMP_HOT_WATER:        mu8ErrorCode = 47; break;
        case ERR_TEMP_HEATER:           mu8ErrorCode = 48; break;

        case ERR_DRAIN_PUMP:            mu8ErrorCode = 60; break;
        case ERR_MICRO_SW_DETECT:       mu8ErrorCode = 61; break;
        case ERR_MICRO_SW_MOVE:         mu8ErrorCode = 62; break;
        case ERR_TEMP_EVA_1:            mu8ErrorCode = 63; break;
		
		/*.. sean [24-10-14] Comp Error 동작 세분화..*/
        case ERR_COMP_CURRENT_SENSING:  	    mu8ErrorCode = 81; break;
        case ERR_COMP_STARTING_FAIL:       		mu8ErrorCode = 82; break;
        case ERR_COMP_OVER_CURRENT:          	mu8ErrorCode = 83; break;
        case ERR_COMP_OVERHEAT:       			mu8ErrorCode = 84; break;
        case ERR_COMP_DISCONNECTION_OF_COMM:    mu8ErrorCode = 85; break;
        case ERR_COMP_ABNORMAL_VOLTAGE:			mu8ErrorCode = 86; break;

        //case ERR_OUTAGE_ICE_TRAY:       mu8ErrorCode = 7; break;
        //case ERR_HOT_WATER_DRAIN:       mu8ErrorCode = 63; break;
        //case ERR_TEMP_EVA_2:            mu8ErrorCode = 64; break;
        //case ERR_COLD_OVF:              mu8ErrorCode = 20; break;
        //case ERR_COLD_HIGH_LEVEL:       mu8ErrorCode = 23; break;
        //case ERR_SODA_PUMP:             mu8ErrorCode = 94; break;
        //case ERR_ROOM_LOW_LEVEL:        mu8ErrorCode = 11; break;
        //case ERR_ROOM_HIGH_LEVEL:       mu8ErrorCode = 13; break;
        //case ERR_ROOM_COMPLEX:          mu8ErrorCode = 14; break;
        //case ERR_ROOM_OVF:              mu8ErrorCode = 10; break;
        //case ERR_COMM_FRONT_DISPLAY:    mu8ErrorCode = 11; break;
        //case ERR_COMM_LCD:              mu8ErrorCode = 11; break;
        default:                        mu8ErrorCode = 0; break;
                                        
    }

    return mu8ErrorCode;
}
