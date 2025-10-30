
/*******************************************************************************
&Description :
    A10A0 ~ A10Ax
    Accumulate Data Table
  
&Histiry : 
    ShinHM 2021.04.13 update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_SetCommon.h"
#include "WIFI_TxForm.h"
#include "WIFI_SetAccumulateData.h"


/******************************************************************************/
/***** Static Function ********************************************************/
/******************************************************************************/
static void EvtWifiAccumul ( U16 mId, I8 *pBuf );


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
static const WifiTxFuncList_T WifiAccumulateList[] = 
{   /*  ID                                  Function                    */
    {   WIFI_ACCM_0000_MONTH_WATER_USER,    NULL,               NULL    },
    {   WIFI_ACCM_0001_MONTH_WATER_A,       NULL,               NULL    },
    {   WIFI_ACCM_0002_MONTH_WATER_B,       NULL,               NULL    },
    {   WIFI_ACCM_0003_MONTH_WATER_C,       NULL,               NULL    },
    {   WIFI_ACCM_0004_DAY_WATER_USER,      NULL,               NULL    },
    {   WIFI_ACCM_0005_DAY_WATER_A,         NULL,               NULL    },
    {   WIFI_ACCM_0006_DAY_WATER_B,         NULL,               NULL    },
    {   WIFI_ACCM_0007_DAY_WATER_C,         NULL,               NULL    },
        
    {   WIFI_ACCM_0010_LAST_MONTH_WATT_KRW, NULL,               NULL    }
};

#define SZ_AC_LIST ( sizeof(WifiAccumulateList) / sizeof(WifiTxFuncList_T) )


/******************************************************************************/
/***** Global Function ********************************************************/
/******************************************************************************/
void* GetWifiAccumulateEventList ( void )
{
    return WifiAccumulateList;
}

U16 GetWifiAccumulateListSize ( void )
{
    return SZ_AC_LIST;
}


/******************************************************************************/
/***** Local Function *********************************************************/
/******************************************************************************/
static void EvtWifiAccumul ( U16 mId, I8 *pBuf )
{
    static I8 mai8Buf[LENGTH_WIFI_DATA_ONE];
    
    _MEMSET_( (void __FAR*) mai8Buf, '\0', sizeof(mai8Buf) );
    
    ConvertDataHexToAscii( TYPE_ACCUMULATE_CMD, mId, mai8Buf );
    
    far_sprintf_s( pBuf, 0, mai8Buf );
}


