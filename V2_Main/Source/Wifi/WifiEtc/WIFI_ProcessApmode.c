
/***** Header *****************************************************************/
#include "WIFI_Common.h"

#define USE_AP 0
#if (USE_AP == 0)

#else
/***** Define *****************************************************************/
#define AP_CMD_SITE 3 // 0:STX / 1,2:LENGTH / 3,4:CMD / 5~:DATA / Checksum / ETX
#define AP_DATA_SITE 5
#define AP_CMD_LENGTH 2
#define AP_PRT_LENGTH 256
#define LENGTH_SSID 65
#define LENGTH_PID 18
#define LENGTH_PW  33
#define LENGTH_BSSID 12 // MAC과 동일

/***** Variable ***************************************************************/
U8 u8WifiApModeState = CLEAR; // AP모드내 Setup 상태 Flag
I8 ai8SSID[LENGTH_SSID+1];	// SSID + "Null"
I8 ai8PW[LENGTH_PW+1]; 			// PASSWORD + "Null"
I8 ai8PID[LENGTH_PID+1];		// 제품 바코드(Product ID) + "Null"
U8 u8WifiDHCP = SET;				// DHCP 설정상태 (F_Wifi_DHCP)
I8 ai8AP_StaticIP[9];				// IP + "Null"
I8 ai8AP_StaticGateway[9];	// GATEWAY + "Null"
I8 ai8AP_StaticSubnet[9];		// SUBNET + "Null"
I8 ai8AP_StaticDNS[9];			// DNS + "Null"
I8 ai8BSSID[LENGTH_BSSID+1];// BSSID + "Null"

/***** Constant ***************************************************************/
const char WIFI_AP_SSID[] = "01";
const char WIFI_AP_PASSWORD[] = "02";
const char WIFI_AP_PID[] = "03";
const char WIFI_AP_DHCP[] = "04";
const char WIFI_AP_BSSID[] = "05";
const char WIFI_AP_TIME[] = "10";
const char WIFI_AP_STERILIZE[] = "11";

/***** Static Function ********************************************************/
static U8 IsApDataCorrect(I8 *pBuf, U16 mu16Len);
static void ApSsid(I8 *pBuf, U16 mu16Len);
static void ApPw(I8 *pBuf, U16 mu16Len);
static void ApPid(I8 *pBuf, U16 mu16Len);
static void ApDhcp(I8 *pBuf, U16 mu16Len);
static void ApBssid(I8 *pBuf, U16 mu16Len);
static void ApTime(I8 *pBuf, U16 mu16Len);
static void ApStrlz(I8 *pBuf, U16 mu16Len);

/***** Table ******************************************************************/
typedef void (*WifiApFun_T)(I8 *pBuf, U16 mu16Len);
typedef struct _wifi_apmode_list_
{
    I8 *Id;
    WifiApFun_T Func;
} WifiApModeList_T;

// ApMode Protocol Table
WifiApModeList_T WifiApModeList[] = 
{		// ID										Function
    { WIFI_AP_SSID,					ApSsid	},
    { WIFI_AP_PASSWORD,			ApPw		},
    { WIFI_AP_PID,					ApPid		},
    { WIFI_AP_DHCP,					ApDhcp	},
    { WIFI_AP_BSSID,				ApBssid	},
    { WIFI_AP_TIME,					ApTime	},
    { WIFI_AP_STERILIZE,		ApStrlz	}
};
#define SZ_AP_LIST  ( sizeof(WifiApModeList) / sizeof(WifiApModeList_T) )

/***** Global Function ********************************************************/
/*
void InitializeWifiAp ( void )
{
		u8WifiApModeState = CLEAR;
		memset(ai8SSID, '\0', sizeof(ai8SSID));
		memset(ai8PW, '\0', sizeof(ai8PW));
		memset(ai8PID, '\0', sizeof(ai8PID));
		u8WifiDHCP = SET;
		memset(ai8AP_StaticIP, '\0', sizeof(ai8AP_StaticIP));
		memset(ai8AP_StaticGateway, '\0', sizeof(ai8AP_StaticGateway));
		memset(ai8AP_StaticSubnet, '\0', sizeof(ai8AP_StaticSubnet));
		memset(ai8AP_StaticDNS, '\0', sizeof(ai8AP_StaticDNS));
		memset(ai8BSSID, '\0', sizeof(ai8BSSID));
}

// WIFI AP MODE STATE
U8 GetWifiApModeStep ( void )
{
		return u8WifiApModeState;
}
void SetWifiApModeState ( U8 mu8State )
{
		u8WifiApModeState = mu8State;
}

// WIFI PASSWORD byte get, byte set
I8 GetWifiPassword ( U16 mu16i )
{
		return ai8PW[mu16i];
}
void SetWifiPassword ( U8 mu8i, U8 mu8Data )
{
		ai8PW[mu8i] = mu8Data;
}

// WIFI PID stirng get, byte set
void GetWifiPid ( I8 *pBuf )
{
		sprintf(pBuf, "%s", ai8PID);
}
void SetWifiPid ( U8 mu8i, U8 mu8Data )
{
		ai8PID[mu8i] = mu8Data;
}

// WIFI DHCP byte get, byte set
U8 GetWifiDHCP ( void ) 
{
		return u8WifiDHCP;
}
void SetWifiDHCP ( U8 mu8State )
{
		u8WifiDHCP = mu8State;
}

// WIFI Static IP byte get, byte set
U16 GetWifiStaticIp ( U16 mu16i )
{
		U16 mu16Data = 0;
		U16 mu16Cnt = 0;
		
		mu16Cnt = mu16i*2;
	  mu16Data = ((ASCII2HEX(ai8AP_StaticIP[mu16Cnt]) << 4) & 0x00F0) + (ASCII2HEX(ai8AP_StaticIP[mu16Cnt+1])& 0x000F);
	  
    return mu16Data;
}
void SetWifiStaticIp ( U8 mu8i, U8 mu8Data )
{
		ai8AP_StaticIP[mu8i] = mu8Data;
}

// WIFI Static Gateway byte get, byte set
U16 GetWifiStaticGateway ( U16 mu16i )
{
		U16 mu16Data = 0;
		U16 mu16Cnt = 0;
		
		mu16Cnt = mu16i*2;
	  mu16Data = ((ASCII2HEX(ai8AP_StaticGateway[mu16Cnt]) << 4) & 0x00F0) + (ASCII2HEX(ai8AP_StaticGateway[mu16Cnt+1])& 0x000F);
	  
    return mu16Data;
}
void SetWifiStaticGateway ( U8 mu8i, U8 mu8Data )
{
		ai8AP_StaticGateway[mu8i] = mu8Data;
}

// WIFI Static Subnet byte get, byte set
U16 GetWifiStaticSubnet ( U16 mu16i )
{
		U16 mu16Data = 0;
		U16 mu16Cnt = 0;
		
		mu16Cnt = mu16i*2;
	  mu16Data = ((ASCII2HEX(ai8AP_StaticSubnet[mu16Cnt]) << 4) & 0x00F0) + (ASCII2HEX(ai8AP_StaticSubnet[mu16Cnt+1])& 0x000F);
	  
    return mu16Data;
}
void SetWifiStaticSubnet ( U8 mu8i, U8 mu8Data )
{
		ai8AP_StaticSubnet[mu8i] = mu8Data;
}

// WIFI Static DNS byte get, byte set
U16 GetWifiStaticDns ( U16 mu16i )
{
		U16 mu16Data = 0;
		U16 mu16Cnt = 0;
		
		mu16Cnt = mu16i*2;
	  mu16Data = ((ASCII2HEX(ai8AP_StaticDNS[mu16Cnt]) << 4) & 0x00F0) + (ASCII2HEX(ai8AP_StaticDNS[mu16Cnt+1])& 0x000F);
	  
    return mu16Data;
}
void SetWifiStaticDns ( U8 mu8i, U8 mu8Data )
{
		ai8AP_StaticDNS[mu8i] = mu8Data;
}

// WIFI BSSID byte get, byte set
I8 GetWifiBssid ( U16 mu16i )
{
		return ai8BSSID[mu16i];
}
void SetWifiBssid ( U8 mu8i, U8 mu8Data )
{
		ai8BSSID[mu8i] = mu8Data;
}

// AP MODE PROTOCOL
void WifiRxApProtocol ( void )
{
	  U16 mu16i = 0; // 데이터 입력을 위한 for문에서 사용
	  U8 mu8Check = 0; // 입력된 데이터가 정상인지 판별한 값
	  I8 mai8RxCmd[AP_CMD_LENGTH];	// ID 데이터
	  U8 mu8PacketLength = 0; // 패킷길이
	  I8 mai8RxApProtocol[AP_PRT_LENGTH]; // 받은 데이터
	  U16 mu16Size = 0; // 테이블 사이즈 
	  WifiApModeList_T *pList = NULL; // 테이블
	  
	  memset(mai8RxCmd, '\0', sizeof(mai8RxCmd));
	  memset(mai8RxApProtocol, '\0', sizeof(mai8RxApProtocol));
	  
	  // Rx 데이터 수신
	  for( mu16i = 0 ; mu16i < 255 ; mu16i++ )
	  {
		  	mu8PacketLength++;
		  	GetWifiRxApData( mai8RxApProtocol, mu16i );
		  	if(mai8RxApProtocol[mu16i] == ETX)
		  	{
		  			break;
		  	}
	  }
	  
	  // 유효성 검사
	  mu8Check = IsApDataCorrect( mai8RxApProtocol, mu8PacketLength );
	  if(mu8Check == FALSE)
	  {
		  	memset(mai8RxApProtocol, '\0', sizeof(mai8RxApProtocol));
		  	SetWifiTxApReply(WIFI_AP_NACK);
		    return;
	  }
	  
	  // 커맨드 추출
	  for( mu16i = 0; mu16i < 2 ; mu16i++ )
	  {
	    	mai8RxCmd[mu16i] = mai8RxApProtocol[AP_CMD_SITE + mu16i];
	  }
	  
	  pList = WifiApModeList;
	  mu16Size = SZ_AP_LIST;
	  
	  // 해당 커맨드의 함수 동작
	  for( mu16i = 0 ; mu16i < mu16Size; mu16i++ )
	  {
		  	WifiApFun_T pFun;
			  pFun = (pList+mu16i)->Func;
			  if( strncmp((pList+mu16i)->Id, (I8 *)mai8RxCmd, AP_CMD_LENGTH) == 0 )
				{
						pFun(&mai8RxApProtocol[0], mu8PacketLength);
						break;
				}
		}
	  
	  memset(mai8RxApProtocol, '\0', sizeof(mai8RxApProtocol));
	  ClearWifiRxProtocol();
}
*/
/***** Local Function *********************************************************/
/*
// 수신된 데이터 유효성 검사
static U8 IsApDataCorrect ( I8 *pBuf, U16 mu16Len )
{
		U8 mu8DataLength = 0;
		U16 mu16i = 0;
		U32 mu32CheckSumBuf = 0;
		U8 mu8CheckSum = 0;
		U8 mu8RxCheckSum = 0;
		
		if ( (pBuf[0] != ASCII_STX) || (pBuf[mu16Len - 1] != ASCII_ETX) )
	  {	// STX, ETX 분석
	    	return FALSE;   // STX, ETX가 맞지 않으면 return
	  }
	  
	  mu8DataLength = (ASCII2HEX(pBuf[1]) << 4) & 0xF0;
	  mu8DataLength += ASCII2HEX(pBuf[2]) & 0x0F;
	
	  if ( (mu8DataLength + 3) != mu16Len )
	  {	// Packet Length 비교
	    	return FALSE;   // Pakcet Length가 맞지 않으면 return
	  }
	
	  for ( mu16i = 0 ; mu16i < (mu16Len - 3) ; mu16i++ ) // CHECKSUM과 ETX를 빼고 모두 CHECKSUM
	  {
	    	mu32CheckSumBuf += pBuf[mu16i];
	  }
	  mu8CheckSum = (U8)mu32CheckSumBuf;
	  mu8RxCheckSum = ((ASCII2HEX(pBuf[mu16Len-3]) << 4) & 0xF0);
	  mu8RxCheckSum += (ASCII2HEX(pBuf[mu16Len-2]) & 0x0F);
	
	  if ( mu8RxCheckSum != mu8CheckSum )
	  {	// Rx Checksum과 계산한 Checksum 분석
	    	return FALSE;	// Checksum이 맞지 않으면 return
	  }
	  return TRUE;
}

// SSID 수신
static void ApSsid ( I8 *pBuf, U16 mu16Len )
{
		U16 mu16i = 0;
	
		if ( (U8)(pBuf[AP_DATA_SITE]) != 0xFF )
    {
	      memset(ai8SSID, '\0', sizeof(ai8SSID));
	    	
	      for ( mu16i = 0 ; mu16i < (mu16Len - 8) ; mu16i++ )
	      {
		        if ( mu16i >= LENGTH_SSID )
		        {
		          	break;
		        }
		        else
		        {
		          	ai8SSID[mu16i] = pBuf[AP_DATA_SITE + mu16i];
		        }
	      }
    }
    
    SetWifiTxApReply( WIFI_AP_ACK );
    u8WifiApModeState |= 0x01;
    SetWifiBuzzSound( WIFI_BUZZER_PAIRING );
}

// Password 수신
static void ApPw ( I8 *pBuf, U16 mu16Len )
{
		U16 mu16i = 0;
	
		if ( (U8)(pBuf[AP_DATA_SITE]) != 0xFF )
    {
	      memset(ai8PW, '\0', sizeof(ai8PW));
	      
	      if ( mu16Len > 8 )
	      {
		        for ( mu16i = 0 ; mu16i < (mu16Len - 8) ; mu16i++ )
		        {
			          if ( mu16i >= LENGTH_PW )
			          {
			            	break;
			          }
			          else
			          {
			            	ai8PW[mu16i] = pBuf[AP_DATA_SITE + mu16i];
			          }
		        }
	      }
	      
	      for ( mu16i = 0 ; mu16i < LENGTH_PW ; mu16i++ )
	      {
	      		WifiWriteEep((EEPROM_ADDR_PW + mu16i), ai8PW[mu16i]);
	      }
    }
    
    SetWifiTxApReply(WIFI_AP_ACK);
    u8WifiApModeState |= 0x02;
    if ((u8WifiApModeState & 0x03) == 0x03)
    {
      	SetWifiBuzzSound(WIFI_BUZZER_PAIRING);
    }
}

// PID 수신
static void ApPid ( I8 *pBuf, U16 mu16Len )
{
		U16 mu16i = 0;
		
		if ( (U8)(pBuf[AP_DATA_SITE]) != 0xFF )
	  {
	      memset(ai8PID, '\0', sizeof(ai8PID));
	      
	      for ( mu16i = 0 ; mu16i < (mu16Len - 8) ; mu16i++ )
	      {
		        if ( mu16i >= LENGTH_PID )
		        {
		          	break;
		        }
		        else
		        {
		          	ai8PID[mu16i] = pBuf[AP_DATA_SITE + mu16i];
		        }
	      }
	      
	      for ( mu16i = 0 ; mu16i < LENGTH_PID ; mu16i++ )
	      {
	        	WifiWriteEep(EEPROM_ADDR_PID + mu16i, ai8PID[mu16i]);
	      }
	  }
    
    SetWifiTxApReply(WIFI_AP_ACK);
    u8WifiApModeState |= 0x04;
    SetWifiBuzzSound(WIFI_BUZZER_PAIRING);
}

// DHCP or Static 수신
static void ApDhcp ( I8 *pBuf, U16 mu16Len )
{
		U16 mu16i = 0;
		
		if ( (U8)(pBuf[AP_DATA_SITE]) != 0xFF )
    {
	      memset(ai8AP_StaticIP, '\0', sizeof(ai8AP_StaticIP));
	      memset(ai8AP_StaticGateway, '\0', sizeof(ai8AP_StaticGateway));
	      memset(ai8AP_StaticSubnet, '\0', sizeof(ai8AP_StaticSubnet));
	      memset(ai8AP_StaticDNS, '\0', sizeof(ai8AP_StaticDNS));
	      
      	if ( (pBuf[AP_DATA_SITE] == '\0') && (pBuf[AP_DATA_SITE+1] == '\0') ) // DHCP
      	{
		        u8WifiDHCP = SET;
		        WifiWriteEep(EEPROM_ADDR_DHCP, 0x01);
		        
		        for ( mu16i = 0 ; mu16i < 8 ; mu16i++ )
		        {
				          WifiWriteEep(EEPROM_ADDR_STATIC_IP + mu16i, ai8AP_StaticIP[mu16i]); 
				          WifiWriteEep(EEPROM_ADDR_STATIC_GATEWAY + mu16i, ai8AP_StaticGateway[mu16i]); 
				          WifiWriteEep(EEPROM_ADDR_STATIC_SUBNET + mu16i, ai8AP_StaticSubnet[mu16i]); 
				          WifiWriteEep(EEPROM_ADDR_STATIC_DNS + mu16i, ai8AP_StaticDNS[mu16i]);
		        }
      	}
	      else // STATIC
	      {
		        u8WifiDHCP = CLEAR;
		        WifiWriteEep(EEPROM_ADDR_DHCP, 0x00);
		        
		        for ( mu16i = 0 ; mu16i < 8 ; mu16i++ )
		        {
			          ai8AP_StaticIP[mu16i] = pBuf[5 + mu16i];
			          ai8AP_StaticGateway[mu16i] = pBuf[14 + mu16i];
			          ai8AP_StaticSubnet[mu16i] = pBuf[23 + mu16i];
			          ai8AP_StaticDNS[mu16i] = pBuf[32 + mu16i];
		        }
		        
		        for ( mu16i = 0 ; mu16i < 8 ; mu16i++ )
		        {
			          WifiWriteEep(EEPROM_ADDR_STATIC_IP + mu16i, ai8AP_StaticIP[mu16i]); 
			          WifiWriteEep(EEPROM_ADDR_STATIC_GATEWAY + mu16i, ai8AP_StaticGateway[mu16i]); 
			          WifiWriteEep(EEPROM_ADDR_STATIC_SUBNET + mu16i, ai8AP_StaticSubnet[mu16i]); 
			          WifiWriteEep(EEPROM_ADDR_STATIC_DNS + mu16i, ai8AP_StaticDNS[mu16i]);
		        }
	      }
    }
    
    SetWifiTxApReply(WIFI_AP_ACK);
    u8WifiApModeState |= 0x08;
    SetWifiBuzzSound(WIFI_BUZZER_PAIRING);
}

// BSSID 수신
static void ApBssid ( I8 *pBuf, U16 mu16Len )
{
		U16 mu16i = 0;
		
		if ( (U8)(pBuf[AP_DATA_SITE]) != 0xFF )
	  {
	  		memset(ai8BSSID, '\0', sizeof(ai8BSSID));
	  		
	      for ( mu16i = 0 ; mu16i < (mu16Len - 8) ; mu16i++ )
	      {
		        ai8BSSID[mu16i] = pBuf[AP_DATA_SITE + mu16i];
		        WifiWriteEep(EEPROM_ADDR_BSSID + mu16i, ai8BSSID[mu16i]);
	      }
	      
	      for ( mu16i = 0 ; mu16i < LENGTH_BSSID ; mu16i++ )
	      {
	        	WifiWriteEep(EEPROM_ADDR_BSSID + mu16i, ai8BSSID[mu16i]);  
	      }
	  }
	  
	  SetWifiTxApReply(WIFI_AP_ACK);
	  u8WifiApModeState |= 0x10;
	  if ( (u8WifiApModeState & 0x1F) == 0x1F )
	  {
	  		SetWifiBuzzSound(WIFI_BUZZER_PAIRING);
	  }
}

// 시간
static void ApTime ( I8 *pBuf, U16 mu16Len )
{
	
}

// 살균
static void ApStrlz ( I8 *pBuf, U16 mu16Len )
{
	
}
*/
#endif
