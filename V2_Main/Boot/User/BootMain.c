/*'""FILE COMMENT""****************************************************
@ FILE Name : BootMain.c
@ FILE ID    : 
@ Programmer : Junbeen Son
@ Description : 
/*""FILE COMMENT END ""************************************************/

#include "BootMain.h"

__near U16 gu16BootModuleInitCnt = 100;	// 1sec. 모듈 전원/리셋핀 제어를 위한 카운트
__near U16 gu16BootInitTimeoutCnt = 6000; // 1min.
__near U8 gu8BootModuleInit_F;
__near U8 gu8BootFailCnt; 
 
extern __near U16 gu16BootModuleRxCheck; 
extern __near U8 gu8BootAuConValue;
extern __near U8 gu8BootEndResultCode;

__near void BOOT_R_CGC_Create(void)
{ 
    /* Set fMX */
    CMC = 0x00; 
    MSTOP = 1U;
    /* Set fMAIN */
    MCM0 = 0U; 
    /* Set fSUB */
    XTSTOP = 1U;
    OSMC = 0x10;;
    /* Set fCLK */
    CSS = 0U;
    /* Set fIH */
    HIOSTOP = 0U;
}

__near void BOOT_R_PORT_Create(void)
{
	// Boot 영역에서 포트 Default Input으로 사용 
	// Wi-Fi 전원, 리셋 포트 Output Low로 아래와 같이 설정
	#ifdef BOOT_PM_MODULE_POWER
		BOOT_PM_MODULE_POWER = LOW;			// LOW : Output
	#endif

	#ifdef BOOT_PM_MODULE_RESET
		BOOT_PM_MODULE_RESET = LOW;			// LOW : Output
	#endif
	
	#ifdef BOOT_PM_MODULE_LED_SINK
		BOOT_PM_MODULE_LED_SINK = LOW;		// LOW : Output
	#endif

	#ifdef BOOT_P_MODULE_LED_SINK
		BOOT_P_MODULE_LED_SINK = LOW;
	#endif
	
	#ifdef BOOT_PM_MODULE_LED_DRIVE
		BOOT_PM_MODULE_LED_DRIVE = LOW;		// LOW : Output
	#endif

	#ifdef BOOT_P_MODULE_LED_DRIVE  
		BOOT_P_MODULE_LED_DRIVE = BOOT_P_MODULE_LED_DRIVE_LEVEL;
	#endif

	BOOT_P_MODULE_RESET = LOW; 
	BOOT_P_MODULE_POWER = LOW;  
	
}

__near void BOOT_R_TAU0_Create(void) 
{
	TAU0EN = 1U;	/* supplies input clock */
	#ifdef OCO_CLOCK_16MHz
		TPS0 = 0x0002 | 0x0000 | 0x0000 | 0x0000;
	#endif
	#ifdef OCO_CLOCK_32MHz
		TPS0 = 0x0003 | 0x0000 | 0x0000 | 0x0000;
	#endif

	/* Stop all channels */
	TT0 = 0x0001 | 0x0002 | 0x0004 |
		  0x0008 | 0x0010 | 0x0020|
		  0x0040 | 0x0080 | 0x0200|
		  0x0800;
	
	/* Channel 0 used as interval timer */
	//TMR00 = 0x0000 | 0x0000 | 0x0000 |
	//		0x0000 | 0x0000 | 0x0000;
	TDR00 = 0x9C3F;
	TO0 &= ~0x0001;
	TOE0 &= ~0x0001;
 
}

__near void BOOT_R_TAU0_Channel0_Start(void)
{
    //TMIF00 = 0U;    /* clear INTTM00 interrupt flag */
    //TMMK00 = 0U;    /* enable INTTM00 interrupt */
    TS0 |= 0x0001;
}

__near void BOOT_R_SAU0_Create(void)
{
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = 0x0001 | 0x0000;
    BOOT_R_UART1_Create();
}

__near void BOOT_R_UART1_Create(void)
{
    ST0 |= 0x0008 | 0x0004;    /* disable UART1 receive and transmit */
    //STMK1 = 1U;    /* disable INTST1 interrupt */ 
    //STIF1 = 0U;    /* clear INTST1 interrupt flag */
    //SRMK1 = 1U;    /* disable INTSR1 interrupt */
    //SRIF1 = 0U;    /* clear INTSR1 interrupt flag */
    //SREMK1 = 1U;   /* disable INTSRE1 interrupt */
    //SREIF1 = 0U;   /* clear INTSRE1 interrupt flag */
    /* Set INTST1 low priority */
    //STPR11 = 1U;
    //STPR01 = 1U;
    /* Set INTSR1 hign priority */
    //SRPR11 = 0U;
    //SRPR01 = 0U;
    /* Set INTSRE1 hign priority */
    //SREPR11 = 0U;
    //SREPR01 = 0U;
    
	#ifdef OCO_CLOCK_16MHz
	    SMR02 = 0x0020 | 0x8000 | 0x0000| 0x0002 | 0x0000;
	#endif
	#ifdef  OCO_CLOCK_32MHz
		SMR02 = 0x0020 | 0x0000 | 0x0000| 0x0002 | 0x0000;
	#endif
    SCR02 = 0x8000| 0x0000 | 0x0000 | 0x0080 | 0x0010 | 0x0007;
    SDR02 = 0x8800;
    NFEN0 |= 0x04;
    SIR03 = 0x0004 | 0x0002 | 0x0001;    /* clear error flag */
	#ifdef OCO_CLOCK_16MHz
	    SMR03 = 0x0020 | 0x8000 | 0x0100 | 0x0000 |
	            0x0002 | 0x0000;
	#endif
	#ifdef OCO_CLOCK_32MHz
		SMR03 = 0x0020 | 0x0000 | 0x0100 | 0x0000 |
	            0x0002 | 0x0000;
	#endif 
    SCR03 = 0x4000 | 0x0000 | 0x0000 | 0x0080 | 0x0010 | 0x0007;
    SDR03 = 0x8800;
    SO0 |= 0x0004;
    //SOL0 |= 0x0000;    /* output level normal */
    SOE0 |= 0x0004;      /* enable UART1 output */
	
    /* Set RxD1 pin */
    PMC0 &= 0xF7U;
    PM0 |= 0x08U;
    /* Set TxD1 pin */
    PMC0 &= 0xFBU;
    //P0 |= 0x04U;		// Module 전원 딜레이 제어를 위한 TXD1(P0.2) 주석 처리
    PM0 &= 0xFBU;  
}

__near void BOOT_R_UART1_Start(void)
{
    SO0 |= 0x0004;    /* output level normal */
    SOE0 |= 0x0004;    /* enable UART1 output */
    SS0 |= 0x0008 | 0x0004;    /* enable UART1 receive and transmit */
    //STIF1 = 0U;    /* clear INTST1 interrupt flag */
    //SRIF1 = 0U;    /* clear INTSR1 interrupt flag */
    //SREIF1 = 0U;   /* clear INTSRE1 interrupt flag */
    //STMK1 = 0U;    /* enable INTST1 interrupt */
    //SRMK1 = 0U;    /* enable INTSR1 interrupt */
    //SREMK1 = 0U;   /* enable INTSRE1 interrupt */
}

__near void BOOT_R_UART1_Stop(void) 
{
    //STMK1 = 1U;    /* disable INTST1 interrupt */
    //SRMK1 = 1U;    /* disable INTSR1 interrupt */
    //SREMK1 = 1U;   /* disable INTSRE1 interrupt */
    ST0 |= 0x0008 | 0x0004;    /* disable UART1 receive and transmit */
    SOE0 &= ~0x0004;    /* disable UART1 output */
    //STIF1 = 0U;    /* clear INTST1 interrupt flag */
    //SRIF1 = 0U;    /* clear INTSR1 interrupt flag */
    //SREIF1 = 0U;   /* clear INTSRE1 interrupt flag */  
}

__near void BOOT_BaseTimer(void)	// 10ms
{
	if(TMIF00 != 1) return;
	TMIF00  = 0;	

	if(gu8BootModuleInit_F == CLEAR)
	{
		if(gu16BootModuleInitCnt) gu16BootModuleInitCnt--;
		else{}
		if(gu16BootInitTimeoutCnt) gu16BootInitTimeoutCnt--;
		else{}
	}
	else
	{
		if(gu16BootModuleRxCheck) gu16BootModuleRxCheck--;
		else{}
	}
	
	BOOT_FotaLEDControl();
} 

__near void BOOT_FotaLEDControl(void)	
{
	__near static U16 mu16FotaLEDCnt = 0; 
	mu16FotaLEDCnt++;	
	if(mu16FotaLEDCnt < 301) 
	{
		#ifdef BOOT_P_MODULE_LED_SINK
			BOOT_P_MODULE_LED_SINK = HIGH;		// ON
		#endif
	}
	else if(mu16FotaLEDCnt < 401) 
	{ 
		#ifdef BOOT_P_MODULE_LED_SINK
			BOOT_P_MODULE_LED_SINK = LOW;		// OFF 
		#endif
	}
	else mu16FotaLEDCnt = 0; 
}

__near void hdwinit(void)
{
	DI();
	
	PIOR = 0x00U;
    BOOT_R_CGC_Create();
    BOOT_R_PORT_Create();
    BOOT_R_SAU0_Create();
    BOOT_R_TAU0_Create();
    IAWCTL = 0x00U;
	
	BOOT_R_UART1_Start();			// Module(Wi-Fi, LTE)
	BOOT_R_TAU0_Channel0_Start();   // Base Timer
}  

__near static U8 BOOT_initModulePort(void)
{
	if(gu16BootModuleInitCnt <= 50) // 500ms 경과
	{
		if(BOOT_P_MODULE_POWER == CLEAR) BOOT_P_UART_TX = SET;		// 포트 전원 딜레이 제어 종료 시 , TXD1 출력은 High 1회 적용(positive edge trigger 형태)
		else{}	
		BOOT_P_MODULE_POWER = SET;
	}
	else BOOT_P_UART_TX = CLEAR;									// 포트 전원 딜레이 제어 진행 시, TXD1(P02) 출력은 Low로 고정
	
	if(gu16BootModuleInitCnt == 0) // 1초 경과
	{
		BOOT_P_MODULE_RESET = SET;
		return SET;
	}
	else{} 
	return CLEAR;
}

__near static U8 BOOT_checkOTARequestCondition(__near U8* mu8RxData)
{
	BOOT_FDLReadByte(DATAFLASH_ADDR_OTASTATUS, mu8RxData, 3);
	if(mu8RxData[0] == 0x77 && mu8RxData[1] == 0x88 && mu8RxData[2] == 0x99) return SET;
	else return CLEAR;
}

__near static U8 BOOT_checkOldBootErase(__near U8* mu8RxData)
{
	BOOT_FDLReadByte(DATAFLASH_ADDR_OTASTATUS, mu8RxData, 3);
	if(mu8RxData[0] == 0xAA && mu8RxData[1] == 0xBB && mu8RxData[2] == 0xCC) return SET;
	else return CLEAR;
}

__near void BOOT_setOTARequest(void)
{
	__near U8 mu8Var[3] = {0x77, 0x88, 0x99};
	BOOT_FDLWriteByte(DATAFLASH_ADDR_OTASTATUS, mu8Var, 3);
	while(1){}	// for Reset
} 

void boot_main(void)
{	
	__near U8 mu8FDLRxData[3];
	__near U8 mu8index; 
	__near U8 mu8RxData; 
	__near U8 mu8RxDataSaved;
	__near U8 mu8DeviceReady = BOOT_RX_NONE;
	__near U8 mu8BootState = 0x00;  
	 
	if(BOOT_startFDL() == (pfdl_status_t)PFDL_OK){}
	else{}
	if(BOOT_startFSL() == FSL_OK){}
	else{}
	
	BOOT_FDLReadByte(DATAFLASH_ADDR_BOOTFAILCNT, &gu8BootFailCnt, 1);
	
	if(BOOT_checkOTARequestCondition(mu8FDLRxData) == SET)
	{ 
		while(1)
		{
			Watchdog();
			BOOT_BaseTimer();
			if(BOOT_initModulePort() == SET) 
			{
				if(BOOT_UartRxCheck() == SET) 
				{
					mu8RxDataSaved = mu8RxData;
					mu8RxData = BOOT_UartRxData(); 
					if(mu8DeviceReady == BOOT_RX_NONE && mu8RxData == 'Y') mu8DeviceReady = BOOT_RX_DOING; 			// *ICT*DEVICEREAD'Y' 수신 체크
					else if(mu8DeviceReady == BOOT_RX_DOING && mu8RxData == 0x0A) mu8DeviceReady = BOOT_RX_DONE;	// DEVICEREADY명령의 ETX(0x0A)까지 확인
					else{}
					
					if(mu8DeviceReady == BOOT_RX_DONE)
					{
						if(gu8BootFailCnt <= 0xFA) 							// 0xFF - 5 = 0xFA
						{
							gu8BootEndResultCode = 32;			 			
							BOOT_UartTxMsg(AT_ICT_OTA_SEND_END);
							gu8BootAuConValue = AUCON_ACTIVATE;
							BOOT_UartTxMsg(AT_ICT_AUCONMODE);
							gu8BootFailCnt = 0xFF; 
							BOOT_FDLWriteByte(DATAFLASH_ADDR_BOOTFAILCNT, &gu8BootFailCnt, 1);
							mu8BootState = 0xF0;
						} 
						else if(mu8BootState == 0x00)
						{	
							gu8BootAuConValue = AUCON_DEACTIVATE;
							BOOT_UartTxMsg(AT_ICT_AUCONMODE); 				// Auto Connection 비활성화 명령 송신
							gu16BootInitTimeoutCnt = BOOT_RxCheckTime;		// 응답 대기를 위한 30초 할당
							mu8BootState++; 
						}
						else if(mu8BootState == 0x01 && mu8RxData == 'K')	mu8BootState++; 	// AUCONMODE:O'K' 수신 체크
						else if(mu8BootState == 0x02 && mu8RxData == 0x0A)						// ETX(0x0A)까지 확인
						{
							gu16BootInitTimeoutCnt = 6000;
							#ifdef BOOT_MODULE_WIFI 
								mu8BootState++; 
							#endif
							#ifdef BOOT_MODULE_LTE
								mu8BootState = 0x04;			// LTE 모듈의 경우 AUCONMODE:OK 수신 이후, Reset 미수행
								gu8BootModuleInit_F = SET;
							break;
							#endif
						}
						else if(mu8BootState == 0x04) 
						{
							gu8BootModuleInit_F = SET;
							break;
						} 
						else if(mu8BootState == 0xF0 && mu8RxDataSaved == 'R' && mu8RxData == 'T') mu8BootState++;	// *ICT*OTA_SEND_STA'RT'=513 수신 확인
						else if(mu8BootState == 0xF1 && mu8RxData == 0x0A) // ETX(0x0A)까지 확인
						{
							BOOT_UartTxMsg(AT_ICT_OTA_SEND_DATA);	 // 송신 이후, gu16BootInitTimeoutCnt 변수에 의해 Reset 
							mu8BootState = 0xF2;
						}
						else{}
					}		 
					else{}
				}
				else{}	
				if(mu8BootState == 0x03 && (gu16BootInitTimeoutCnt < 5900)) // 1초 경과 시 모듈 리셋(60초->59초)
				{
						gu16BootModuleInitCnt = 100;
						BOOT_P_MODULE_POWER = BOOT_P_MODULE_RESET = CLEAR; 
						mu8DeviceReady = BOOT_RX_NONE;
						mu8BootState++; 
				}
				else if(mu8BootState == 0xF0) gu16BootInitTimeoutCnt = 300;	// *ICT*OTA_SEND_START=513 수신 대기 (3초 초기화 지속)
				else{} 
			}
			else{}
			if(!gu16BootInitTimeoutCnt) // 1) DEVICEREADY 미수신 1분 경과 시, Reset	2) DEVICEREADY 수신 후, AUCONMODE:OK 미수신 30초 경과 시 
			{
				if(mu8BootState != 0xF2)	// FOTA 5회 실패 후, 리셋하는 경우에는 실패카운트를 적용하지 않음
				{
					gu8BootFailCnt--;
					BOOT_FDLWriteByte(DATAFLASH_ADDR_BOOTFAILCNT, &gu8BootFailCnt, 1);	// 실패 카운트 상승	
				}
				else{} 
				while(1){}	
			}
			else{}
		}

		BOOT_UartTxMsg(AT_ICT_OTA_SEND_DATA);

		while(1)
		{
			Watchdog();
			BOOT_BaseTimer();
			if(mu8BootState == 0x04) 
			{
				if(BOOT_writeProgram() == SET)
				{
					#ifdef BOOT_MODULE_WIFI
						gu8BootAuConValue = AUCON_ACTIVATE;
						BOOT_UartTxMsg(AT_ICT_AUCONMODE); 		// Auto Connection 활성화 명령 송신
						mu8BootState++;
					#endif
					#ifdef BOOT_MODULE_LTE
						while(1){}								// LTE 모듈의 경우, Reset 수행 시 Auto Connection 활성화상태로 동작
					#endif
				}
				else{} 		
			}
			else if(mu8BootState == 0x05)
			{
				mu8RxData = BOOT_UartRxData();
				if(mu8RxData == 'K') while(1){}				// *ICT*AUCONMODE:O'K' 수신 체크 및 Reset 수행
				else{}
			}
			else{}
			if(gu16BootModuleRxCheck == 0) 
			{
				gu8BootFailCnt--;
				BOOT_FDLWriteByte(DATAFLASH_ADDR_BOOTFAILCNT, &gu8BootFailCnt, 1);	// 실패 카운트 상승
				while(1){}	// 에러 발생 시, 리셋
			}
			else{}
		}
	}
	else if(BOOT_checkOldBootErase(mu8FDLRxData) == SET)
	{
		BOOT_P_MODULE_POWER = BOOT_P_MODULE_RESET = CLEAR;
		for(mu8index=0; mu8index<4; mu8index++) 
		{
			Watchdog(); 
			if(FSL_Erase(START_WRITEBLOCK + mu8index) != 0)
			{
				gu8BootFailCnt--;
				BOOT_FDLWriteByte(DATAFLASH_ADDR_BOOTFAILCNT, &gu8BootFailCnt, 1);
				BOOT_setOTARequest();	// Erase 실패 시, 실패카운트 상승 후, FOTA 재시작
			}
		}
		mu8FDLRxData[0] = mu8FDLRxData[1] = mu8FDLRxData[2] = 0xFF;			// CLEAR
		BOOT_FDLWriteByte(DATAFLASH_ADDR_OTASTATUS, mu8FDLRxData, 3);		// OTA 상태 초기화
		BOOT_FDLWriteByte(DATAFLASH_ADDR_BOOTFAILCNT, mu8FDLRxData, 1);		// 실패 카운트 초기화 
	}
	else 
	{
		FSL_Close();
		return;	
	}
} 

