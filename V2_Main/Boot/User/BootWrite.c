/*'""FILE COMMENT""****************************************************
@ FILE Name : BootWrite.c
@ FILE ID    : 
@ Programmer : Junbeen Son
@ Description : 
/*""FILE COMMENT END ""************************************************/
#include "BootMain.h"

__near U8 gu8BootMsgBuffer[291];
__near static U16 gu16RxBinCRC;
__near U16 gu16BootModuleRxCheck = BOOT_RxCheckTime;
__near U16 gu16BootPacketCnt;
__near U8 gu8BootEndResultCode;
__near U8 gu8BootAuConValue; 

extern __near U8 gu8BootFailCnt;

static __far const BOOT_COMMAND_T sTxCommand[] =
{
	{AT_ICT_AUCONMODE,            (__far U8*)"AT*ICT*AUCONMODE=0 ",      19},
	{AT_ICT_OTA_SEND_DATA,        (__far U8*)"AT*ICT*OTA_SEND_DATA=",    21},   
	{AT_ICT_OTA_SEND_END,         (__far U8*)"AT*ICT*OTA_SEND_END=",     20}
};

static __far const BOOT_COMMAND_T sRxCommand[] = 
{
	{ICT_OTA_SEND_DATA,        (__far U8*)"*ICT*OTA_SEND_DATA=",       287},
	{ICT_OTA_SEND_END,         (__far U8*)"*ICT*OTA_SEND_END=",        19}
}; 
  
  
__near U8 BOOT_writeProgram(void)
{
	__near static U8 mu8Command;
	__near static U32 mu32WriteAddressIndex = START_WRITEADDR;
	__near fsl_write_t msWriteBuff;
	__near static U16 mu16index; 
	__near U16 mu16RxCRC;
	__near static U16 mu16BlockCnt = START_WRITEBLOCK; 
	__near static U8 mu8VerifyCnt = 0; 
	__near U16 mu16IndexValue = 0; 

	if(BOOT_UartRxCheck() == CLEAR) return; 
	else{} 
	
	if(BOOT_UartRxMsg(&gu8BootMsgBuffer[0], &mu8Command) == CLEAR) return;
	else{}
		
	switch(mu8Command)													
	{
		case ICT_OTA_SEND_DATA:	 
			//========== Refresh Communication Check =========//
			gu16BootModuleRxCheck = BOOT_RxCheckTime;
			
			//============== Index Correct Check =============//
			for(mu16index=0; mu16index<4; mu16index++)
			{
				mu16IndexValue = mu16IndexValue*10;
				mu16IndexValue += ASCII_TO_HEX_ONE(0,gu8BootMsgBuffer[PARAM_INDEX_START+mu16index]); 
			}
			if(gu16BootPacketCnt != mu16IndexValue)
			{
				BOOT_UartTxMsg(AT_ICT_OTA_SEND_DATA);	// Index 불일치 시, 해당 패킷 재요청
				break;
			}
			else{}
			//============== CRC Check ===============//
			mu16RxCRC = ((U16)gu8BootMsgBuffer[PARAM_CRC_START]<<8) + gu8BootMsgBuffer[PARAM_CRC_START+1];
			if(mu16RxCRC != CRC16_CCITT_FALSE(gu8BootMsgBuffer+PARAM_DATA_START, 256))					
			{
				gu8BootFailCnt--;
				BOOT_FDLWriteByte(DATAFLASH_ADDR_BOOTFAILCNT, &gu8BootFailCnt, 1);
				while(1){} 
			}				
			else{}
					
			//============== Data Check or Writing ===============//
			if(mu32WriteAddressIndex < END_WRITEADDRESS)
			{
				if(gu16BootPacketCnt < HEADER_BLOCK_SIZE)
				{
					gu16RxBinCRC = ((U16)ASCII_TO_HEX_ONE(gu8BootMsgBuffer[PARAM_DATA_START], gu8BootMsgBuffer[PARAM_DATA_START+1]))<<8;
					gu16RxBinCRC |= ASCII_TO_HEX_ONE(gu8BootMsgBuffer[PARAM_DATA_START+2], gu8BootMsgBuffer[PARAM_DATA_START+3]);
					 
					//============== Erase ===============//
					for(mu16index=0; mu16index < (END_WRITEBLOCK-START_WRITEBLOCK)+1; mu16index++)
					{
						if(FSL_Erase(START_WRITEBLOCK + mu16index) != 0){}  
					}  
				}
				else if((gu16BootPacketCnt > START_SKIP_NEWBOOT_BLOCK) && (gu16BootPacketCnt <= END_SKIP_NEWBOOT_BLOCK)){}
				else
				{
					msWriteBuff.fsl_data_buffer_p_u08 = &gu8BootMsgBuffer[PARAM_DATA_START];		// index 추가에 따른 수정 23->27
					msWriteBuff.fsl_destination_address_u32 = mu32WriteAddressIndex;
					msWriteBuff.fsl_word_count_u08 = 0x40;
					if(FSL_Write(&msWriteBuff) != 0){}
					mu32WriteAddressIndex += PR_MSG_PACKET_SIZE;
					mu8VerifyCnt++;	  
					if(mu8VerifyCnt == 4)
					{
						if(FSL_IVerify(mu16BlockCnt) != 0){}  
						mu16BlockCnt++;
						mu8VerifyCnt = 0;
					}
				}				
			}
			else{}
			gu16BootPacketCnt++;	
			BOOT_UartTxMsg(AT_ICT_OTA_SEND_DATA);
			break;

		case ICT_OTA_SEND_END:
			//========== Refresh Communication Check =========//
			gu16BootModuleRxCheck = BOOT_RxCheckTime;

			//========== CRC Check for Whole Image ===========// //=============== ResultCode Check ===============//
			if((CRC16_CCITT_FALSE_BIN() != gu16RxBinCRC) || (gu8BootMsgBuffer[PARAM_RESULTCODE] != '0'))
			{ 
				gu8BootEndResultCode = 1;
				BOOT_UartTxMsg(AT_ICT_OTA_SEND_END);
				gu8BootFailCnt--;
				BOOT_FDLWriteByte(DATAFLASH_ADDR_BOOTFAILCNT, &gu8BootFailCnt, 1);
				while(1){}
			}
			else
			{
				if(FSL_InvertBootFlag() != 0)
				{	
					gu8BootFailCnt--;
					BOOT_FDLWriteByte(DATAFLASH_ADDR_BOOTFAILCNT, &gu8BootFailCnt, 1);  // boot swap 실패 시, Reset 
					while(1){}
				}
				else
				{
					gu8BootMsgBuffer[0] = 0xAA;	// SET
					gu8BootMsgBuffer[1] = 0xBB;
					gu8BootMsgBuffer[2] = 0xCC;
					BOOT_FDLWriteByte(DATAFLASH_ADDR_OTASTATUS, gu8BootMsgBuffer, 3);
					gu8BootEndResultCode = 0;
					BOOT_UartTxMsg(AT_ICT_OTA_SEND_END);
					return SET;
				}
			}
			break;

		default:
			break;
	}
	return CLEAR;
}

__near U8 BOOT_startFSL(void)
{
	static __far const fsl_descriptor_t fsl_descriptor_pstr = {0x00, 0x10, 0x01};
	__near U8 gu8RetValue;
	
	gu8RetValue = FSL_Init(&fsl_descriptor_pstr);
	if(gu8RetValue == FSL_OK)
	{
		FSL_Open();
		FSL_PrepareFunctions();
		FSL_PrepareExtFunctions();
	}
	else{}
	return gu8RetValue;	
}

__near U8 BOOT_UartRxMsg(__near U8* pmu8DataBuffer, __near U8* pmu8Command)
{
	__near U8 mu8index;
	__near static U8 mu8STXRx_F = 0;
	__near static U16 mu16index = 0;
	__near static U16 mu16RxLength = 0xFFFD;
	__near static U8 mu8ParsingDone_F = CLEAR;
	__near U8 mu8RxData;

	mu8RxData = BOOT_UartRxData();
	
	if(mu8RxData == '*' && mu8STXRx_F == CLEAR)	
	{
		*pmu8DataBuffer = mu8RxData;
		mu8STXRx_F = SET;
		mu16index = 1;
	}
	else
	{
		pmu8DataBuffer[mu16index] = mu8RxData;	
		if(mu8ParsingDone_F == CLEAR)
		{
			if(mu8RxData == '=') 
			{
				for(mu8index=0; mu8index<ICT_OTA_MAX; mu8index++)
				{
					if(pmu8DataBuffer[mu16index-1] == sRxCommand[mu8index].pmu8Command[mu16index-1])	// DAT'A', EN'D' 구분을 위함
					{
						mu16RxLength = sRxCommand[mu8index].mu16length;
						*pmu8Command = sRxCommand[mu8index].mu8num;
						mu8ParsingDone_F = SET;
						break;
					}
					else{}
				}
				if(mu8ParsingDone_F == CLEAR) 
				{
					mu8STXRx_F = CLEAR; 
					return CLEAR; 
				}
				else{}
			} 		
			else{}
		}
		else
		{
			if(mu16index >= mu16RxLength+1)
			{
				mu8STXRx_F = CLEAR;
				mu8ParsingDone_F = CLEAR; 
				return SET;
			}
			else{}
		}
		mu16index++;
	}
	return CLEAR;
}

__near U8 BOOT_UartRxData(void)
{
	__near U8 mu8RxData;
	
	mu8RxData = RXD1;
	SRIF1 = 0; 
	return mu8RxData;
}

__near U8 BOOT_UartRxCheck(void)
{ 
	if(SRIF1 == 1) 
	{
		if(SSR03 & 0x0007 != 0) SIR03 = 0x0007; 
		else{}
		return SET;
	}
	else return CLEAR;
}

__near void BOOT_UartTxMsg(__near U8 mu8num)
{
	__near U16 mu16index;
	__near U8 mu8index;	
	 
	for(mu16index=0; mu16index<sTxCommand[mu8num].mu16length; mu16index++)
	{
		mu8index = sTxCommand[mu8num].pmu8Command[mu16index];
		BOOT_UartTxData(&mu8index);
	}

	switch(sTxCommand[mu8num].mu8num)
	{	
		case AT_ICT_AUCONMODE:
			BOOT_UartTxData(&gu8BootAuConValue);
			break;
			
		case AT_ICT_OTA_SEND_DATA:	 
			mu16index = gu16BootPacketCnt/10;  
			mu8index = (U8)(mu16index/100)+0x30; // index 4자리 변경으로 인한 수정
			BOOT_UartTxData(&mu8index);
			mu8index = (U8)((mu16index/10)%10)+0x30; 
			BOOT_UartTxData(&mu8index);
			mu8index = (U8)((mu16index)%10)+0x30;
			BOOT_UartTxData(&mu8index);
			mu8index = (U8)(gu16BootPacketCnt%10)+0x30; 
			BOOT_UartTxData(&mu8index);    
			break; 

		case AT_ICT_OTA_SEND_END:	
			if(gu8BootEndResultCode == 32) 
			{
				mu8index = '3'; 
				BOOT_UartTxData(&mu8index);
				mu8index = '2';
				BOOT_UartTxData(&mu8index);
			}	
			else 
			{
				mu8index = gu8BootEndResultCode+0x30;
				BOOT_UartTxData(&mu8index); 
			}
			break;
 
		default: 
			break;
	}
	mu8index = 0x0D;	// ETX
	BOOT_UartTxData(&mu8index);
}

__near void BOOT_UartTxData(__near U8* pmu8SendData)
{
	TXD1 = *pmu8SendData; 
	while(1)
	{
		if(STIF1 == 1)
		{
			STIF1 = 0;
			break;
		}
		else{}
		Watchdog();
	}
}

__near U16 CRC16_CCITT_FALSE(__near U8* puchMsg, __near U16 usDataLen)
{
	__near U16 wCRCin = 0xFFFF;
	__near U16 wCPoly = 0x1021;
	__near U16 wChar;
	__near U16 i;
	
	
	while (usDataLen--)
	{
		wChar = *(puchMsg++);
		wCRCin ^= (wChar << 8);
		for(i = 0;i < 8;i++)
		{
			Watchdog();
			if(wCRCin & 0x8000) wCRCin = (wCRCin << 1) ^ wCPoly;
			else wCRCin<<=1;
		}
	}
	return (wCRCin) ;
}

__near U16 CRC16_CCITT_FALSE_BIN(void)
{
	__near U16 wCRCin = 0xFFFF;
	__near U16 wCPoly = 0x1021;
	__near U16 wChar;
	__near U16 i;
	__near U32 cnt = 0x00000000;
	__far const U8* pmu8Msg_1 = (__far U8*)0x00001000;
	__far const U8* pmu8Msg_2 = (__far U8*)0x00010000; 
	__far const U8* pmu8Msg_3 = (__far U8*)0x00020000;
	__far const U8* pmu8Msg_4 = (__far U8*)0x00030000;

	while(cnt != END_WRITEADDRESS) 
	{
		if(cnt >= 0x00030000) wChar = *(pmu8Msg_4++); 
		else if(cnt >= 0x00020000) wChar = *(pmu8Msg_3++); 
		else if(cnt >= 0x00010000) wChar = *(pmu8Msg_2++); 
		else if(cnt >= 0x00002000) wChar = *(pmu8Msg_1++);
		else if(cnt >= 0x00001000) wChar = 0xFF;
		else wChar = *(pmu8Msg_1++);
		cnt++;
		wCRCin ^= (wChar << 8);
		for(i = 0;i < 8;i++)
		{
			Watchdog(); 
			if(wCRCin & 0x8000) wCRCin = (wCRCin << 1) ^ wCPoly;
			else wCRCin<<=1; 
		}
	}
	return (wCRCin) ; 
}

__near U8 ASCII_TO_HEX_ONE(__near U8 Upper, __near U8 Lower)
{ 
  if(Upper >= 0x61) Upper = Upper - 0x27;
  else if(Upper >= 0x41) Upper = Upper - 0x07;
  
  if(Lower >= 0x61) Lower = Lower - 0x27;
  else if(Lower >= 0x41) Lower = Lower - 0x07;
  
  return(((Upper-0x30)<<4)|(Lower-0x30));
}
/*
__near U16 CRC16_CCITT_FALSE_BIN(void)
{
	__near U16 wCRCin = 0xFFFF;
	__near U16 wCPoly = 0x1021;
	__near U16 wChar;
	__near U16 i;
	__near U32 cnt = 0x00001000;
	__far const U8* pmu8Msg_1 = (__far U8*)0x00001000;
	__far const U8* pmu8Msg_2 = (__far U8*)0x00010000; 
	__far const U8* pmu8Msg_3 = (__far U8*)0x00020000;

	while(cnt != (END_WRITEADDRESS + 0x00001000)) 
	{
		if(cnt>=0x00001000 && cnt<0x00002000) wChar = *(pmu8Msg_1++);
		else if(cnt>=0x00002000 && cnt<0x00003000) wChar = 0xFF;
		else if(cnt>=0x00003000 && cnt<0x00011000) wChar = *(pmu8Msg_1++);
		else if(cnt>=0x00011000 && cnt<0x00021000) wChar = *(pmu8Msg_2++); 
		else wChar = *(pmu8Msg_3++);
		cnt++;
		wCRCin ^= (wChar << 8);
		for(i = 0;i < 8;i++)
		{
			Watchdog(); 
			if(wCRCin & 0x8000) wCRCin = (wCRCin << 1) ^ wCPoly;
			else wCRCin<<=1;
		}
	}
	return (wCRCin) ;
}

__near U8 ASCII_TO_HEX_ONE(__near U8 Upper, __near U8 Lower)
{
  if(Upper >= 0x61) Upper = Upper - 0x27;
  else if(Upper >= 0x41) Upper = Upper - 0x07;
  
  if(Lower >= 0x61) Lower = Lower - 0x27;
  else if(Lower >= 0x41) Lower = Lower - 0x07;
  
  return(((Upper-0x30)<<4)|(Lower-0x30));
}
*/

