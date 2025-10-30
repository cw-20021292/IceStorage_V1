
/*******************************************************************************
&Description :
    Z2010 ~ Z201x
    RAM Monitor Table
  
&Histiry : 
    SSG 2022.07. update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_DefUser.h"
#include "WIFI_SetCommon.h"
#include "WIFI_TxForm.h"
#include "WIFI_SetMonitorData.h"


/******************************************************************************/
/***** Static Function ********************************************************/
/******************************************************************************/
static void EvtWifiTxMonitor ( U16 mAddress, I8 *pBuf, U16 mLen );
static void EvtWifiRxMonitor ( U16 mAddress, I8 *pBuf );


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
static const WifiTxFuncList_Monitor_T WifiMonitorList[] = 
{   /*  ID                                  Tx Function     Rx Function     */
    {   WIFI_MNT_0000_RamAddress,    EvtWifiTxMonitor,  EvtWifiRxMonitor   },
};


#define SZ_Monitor_LIST ( sizeof(WifiMonitorList) / sizeof(WifiTxFuncList_Monitor_T) )


/******************************************************************************/
/***** Global Function ********************************************************/
/******************************************************************************/
void* GetWifiMonitorEventList ( void )
{
    return WifiMonitorList;
}

U16 GetWifiMonitorListSize ( void )
{
    return SZ_Monitor_LIST;
}



/******************************************************************************/
/***** Local Function *********************************************************/
/******************************************************************************/
static void EvtWifiTxMonitor ( U16 mAddress, I8 *pBuf, U16 mLen )
{
    static I8 mai8Buf[LENGTH_WIFI_DATA_ONE_MAX];
    
    _MEMSET_( (void __FAR*) mai8Buf, '\0', sizeof(mai8Buf) );
    // _MEMSET_( mai8Buf, '\0', sizeof(mai8Buf) );
    
    ConvertDataHexToAscii_RAM_Monitor(TYPE_MONITOR_DATA_CMD, mAddress, mai8Buf, mLen );
    
    far_sprintf_s( pBuf, 0, mai8Buf );
}

static void EvtWifiRxMonitor ( U16 mAddress, I8 *pBuf )
{
    ApplyMonitorData( mAddress, pBuf );
}


