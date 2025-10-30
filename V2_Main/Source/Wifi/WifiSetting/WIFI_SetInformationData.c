
/*******************************************************************************
&Description :
    A2010 ~ A201x
    Information Table
  
&Histiry : 
    ShinHM 2021.04.13 update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_SetCommon.h"
#include "WIFI_TxForm.h"
#include "WIFI_SetInformationData.h"


/******************************************************************************/
/***** Static Function ********************************************************/
/******************************************************************************/
static void EvtWifiTxInfo ( U16 mId, I8 *pBuf );
static void EvtWifiRxInfo ( U16 mId, I8 *pBuf );


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
static const WifiTxFuncList_T WifiInformationList[] = 
{   /*  ID                                  Tx Function     Rx Function     */
    {   WIFI_INFO_0000_YEAR,                EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0001_MONTH,               EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0002_DATE,                EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0003_HOUR,                EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0004_MINUTE,              EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0005_SECOND,              EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0006_DAY,                 EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0007_PART_PERIOD,         EvtWifiTxInfo,  EvtWifiRxInfo   },
        
    {   WIFI_INFO_0010_WEATHER,             EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0011_AQI,                 EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0012_PM10,                NULL,           NULL            },
    {   WIFI_INFO_0013_PM25,                NULL,           NULL            },
    {   WIFI_INFO_0014_CURRENT_TEMP,        EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0015_HIGH_TEMP,           EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0016_LOW_TEMP,            EvtWifiTxInfo,  EvtWifiRxInfo   },

    {   WIFI_INFO_0020_LAST_HEART_YEAR,     NULL,           NULL            },
    {   WIFI_INFO_0021_LAST_HEART_MONTH,    NULL,           NULL            },
    {   WIFI_INFO_0022_LAST_HEART_DAY,      NULL,           NULL            },
    {   WIFI_INFO_0023_NEXT_HEART_YEAR,     NULL,           NULL            },
    {   WIFI_INFO_0024_NEXT_HEART_MONTH,    NULL,           NULL            },
    {   WIFI_INFO_0025_NEXT_HEART_DAY,      NULL,           NULL            },
    {   WIFI_INFO_0026_FILTER_CHANGE_DAY1,  EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0027_FILTER_CHANGE_DAY2,  NULL,           NULL            }    
};

static const WifiTxFuncList_T WifiInformationList_USER[] = 
{   /*  ID                                  Tx Function     Rx Function     */
    {   WIFI_INFO_0000_YEAR,                EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0001_MONTH,               EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0002_DATE,                EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0003_HOUR,                EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0004_MINUTE,              EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0005_SECOND,              EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0006_DAY,                 EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0007_PART_PERIOD,         EvtWifiTxInfo,  EvtWifiRxInfo   },
        
    {   WIFI_INFO_0010_WEATHER,             NULL,           NULL            },
    {   WIFI_INFO_0011_AQI,                 NULL,           NULL            },
    {   WIFI_INFO_0012_PM10,                NULL,           NULL            },
    {   WIFI_INFO_0013_PM25,                NULL,           NULL            },
    {   WIFI_INFO_0014_CURRENT_TEMP,        NULL,           NULL            },
    {   WIFI_INFO_0015_HIGH_TEMP,           NULL,           NULL            },
    {   WIFI_INFO_0016_LOW_TEMP,            NULL,           NULL            },

    {   WIFI_INFO_0020_LAST_HEART_YEAR,     NULL,           NULL            },
    {   WIFI_INFO_0021_LAST_HEART_MONTH,    NULL,           NULL            },
    {   WIFI_INFO_0022_LAST_HEART_DAY,      NULL,           NULL            },
    {   WIFI_INFO_0023_NEXT_HEART_YEAR,     NULL,           NULL            },
    {   WIFI_INFO_0024_NEXT_HEART_MONTH,    NULL,           NULL            },
    {   WIFI_INFO_0025_NEXT_HEART_DAY,      NULL,           NULL            },
    {   WIFI_INFO_0026_FILTER_CHANGE_DAY1,  EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0027_FILTER_CHANGE_DAY2,  NULL,           NULL            }
};

static const WifiTxFuncList_T WifiInformationList_DATA[] = 
{   /*  ID                                  Tx Function     Rx Function     */
    {   WIFI_INFO_0000_YEAR,                NULL,           NULL            },
    {   WIFI_INFO_0001_MONTH,               NULL,           NULL            },
    {   WIFI_INFO_0002_DATE,                NULL,           NULL            },
    {   WIFI_INFO_0003_HOUR,                NULL,           NULL            },
    {   WIFI_INFO_0004_MINUTE,              NULL,           NULL            },
    {   WIFI_INFO_0005_SECOND,              NULL,           NULL            },
    {   WIFI_INFO_0006_DAY,                 NULL,           NULL            },
    {   WIFI_INFO_0007_PART_PERIOD,         EvtWifiTxInfo,  EvtWifiRxInfo   },
        
    {   WIFI_INFO_0010_WEATHER,             EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0011_AQI,                 EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0012_PM10,                NULL,           NULL            },
    {   WIFI_INFO_0013_PM25,                NULL,           NULL            },
    {   WIFI_INFO_0014_CURRENT_TEMP,        EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0015_HIGH_TEMP,           EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0016_LOW_TEMP,            EvtWifiTxInfo,  EvtWifiRxInfo   },

    {   WIFI_INFO_0020_LAST_HEART_YEAR,     NULL,           NULL            },
    {   WIFI_INFO_0021_LAST_HEART_MONTH,    NULL,           NULL            },
    {   WIFI_INFO_0022_LAST_HEART_DAY,      NULL,           NULL            },
    {   WIFI_INFO_0023_NEXT_HEART_YEAR,     NULL,           NULL            },
    {   WIFI_INFO_0024_NEXT_HEART_MONTH,    NULL,           NULL            },
    {   WIFI_INFO_0025_NEXT_HEART_DAY,      NULL,           NULL            },
    {   WIFI_INFO_0026_FILTER_CHANGE_DAY1,  EvtWifiTxInfo,  EvtWifiRxInfo   },
    {   WIFI_INFO_0027_FILTER_CHANGE_DAY2,  NULL,           NULL            }
};

#define SZ_INFO_LIST ( sizeof(WifiInformationList) / sizeof(WifiTxFuncList_T) )


/******************************************************************************/
/***** Global Function ********************************************************/
/******************************************************************************/
void* GetWifiInformationEventList ( void )
{
    return WifiInformationList;
}

void* GetWifiInfoUserEventList ( void )
{
    return WifiInformationList_USER;
}

void* GetWifiInfoDataEventList ( void )
{
    return WifiInformationList_DATA;
}

U16 GetWifiInformationListSize ( void )
{
    return SZ_INFO_LIST;
}

// 정수데이터 예외처리
U8 IsPositiveData ( U16 mId )
{
    if ( ( mId == WIFI_INFO_0014_CURRENT_TEMP )
        || ( mId == WIFI_INFO_0015_HIGH_TEMP )
        || ( mId == WIFI_INFO_0016_LOW_TEMP ) )
    {
        return FALSE; // Negative
    }
    return TRUE; // Positive
}


/******************************************************************************/
/***** Local Function *********************************************************/
/******************************************************************************/
static void EvtWifiTxInfo ( U16 mId, I8 *pBuf )
{
    static I8 mai8Buf[LENGTH_WIFI_DATA_ONE];
    
    _MEMSET_( (void __FAR*) mai8Buf, '\0', sizeof(mai8Buf) );
    
    ConvertDataHexToAscii( TYPE_INFO_RQST_CMD, mId, mai8Buf );
    
    far_sprintf_s( pBuf, 0, mai8Buf );
}

static void EvtWifiRxInfo ( U16 mId, I8 *pBuf )
{
    ApplyInfoData( mId, pBuf );
}


