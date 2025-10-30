
#ifndef _BUZZER_H_
#define _BUZZER_H_

/* Header Information***********************************************************
   �� Clock : 32MHz
   �� Timer : TDRxx (Square wave output) 7CFF
   �� Period of square wave output = Period of count clock(0.000125) X (value of TDRxx + 1) X 2
   �� ex) 880Hz = 32MHz / ((TDR+1)*2)
 *******************************************************************************/

//==============================================================================
// Buzzer Setting
//==============================================================================
#define TDR_USE		1U			// ����ϴ� ä��
#define TDR_SET		TDR11		// ����ϴ� TDR
#define	OSC_16MHZ	0U			// ��� 1 / �̻�� 0
#define	OSC_32MHZ	1U			// ��� 1 / �̻�� 0

/******************************************************************************/
//==============================================================================
// Buzzer TDR
//==============================================================================
#if (TDR_USE==0)
#define	SoundStart()		do{R_TAU0_Channel0_Start();}while(0)
#define	SoundStop()			do{R_TAU0_Channel0_Stop(); TO0 &= ~_0001_TAU_CH0_OUTPUT_VALUE_1;}while(0)
#elif (TDR_USE==1)
#define	SoundStart()		do{R_TAU1_Channel1_Start();}while(0)
#define	SoundStop()			do{R_TAU1_Channel1_Stop(); TO0 &= ~_0002_TAU_CH1_OUTPUT_VALUE_1;}while(0)
#elif (TDR_USE==2)
#define	SoundStart()		do{R_TAU0_Channel2_Start();}while(0)
#define	SoundStop()			do{R_TAU0_Channel2_Stop(); TO0 &= ~_0004_TAU_CH2_OUTPUT_VALUE_1;}while(0)
#elif (TDR_USE==3)
#define	SoundStart()		do{R_TAU0_Channel3_Start();}while(0)
#define	SoundStop()			do{R_TAU0_Channel3_Stop(); TO0 &= ~_0008_TAU_CH3_OUTPUT_VALUE_1;}while(0)
#elif (TDR_USE==4)
#define	SoundStart()		do{R_TAU0_Channel4_Start();}while(0)
#define	SoundStop()			do{R_TAU0_Channel4_Stop(); TO0 &= ~_0010_TAU_CH4_OUTPUT_VALUE_1;}while(0)
#elif (TDR_USE==5)
#define	SoundStart()		do{R_TAU0_Channel5_Start();}while(0)
#define	SoundStop()			do{R_TAU0_Channel5_Stop(); TO0 &= ~_0020_TAU_CH5_OUTPUT_VALUE_1;}while(0)
#elif (TDR_USE==6)
#define	SoundStart()		do{R_TAU0_Channel6_Start();}while(0)
#define	SoundStop()			do{R_TAU0_Channel6_Stop(); TO0 &= ~_0040_TAU_CH6_OUTPUT_VALUE_1;}while(0)
#else //(TDR_USE==7)
#define	SoundStart()		do{R_TAU0_Channel7_Start();}while(0)
#define	SoundStop()			do{R_TAU0_Channel7_Stop(); TO0 &= ~_0080_TAU_CH7_OUTPUT_VALUE_1;}while(0)
#endif

#define TURN_ON_TDR()		do{TDR_SET = Buzzer.TDR;}while(0)
#define TURN_OFF_TDR() 	    do{TDR_SET = OFF;}while(0)

//==============================================================================
// Buzzer ID
//==============================================================================
typedef enum
{
    BUZZER_OFF = 0,
    BUZZER_POWER_ON,
    BUZZER_EFFLUENT,
    BUZZER_EFFLUENT_END,
    BUZZER_COOL_SETUP,
    BUZZER_COOL_CANCEL,
    BUZZER_SELECT,
    BUZZER_ERROR,
    BUZZER_SETUP,
    BUZZER_CANCEL,
    BUZZER_EFFLUENT_CONTINUE,
    BUZZER_STERILIZE_START,
    BUZZER_STERILIZE_END,
    BUZZER_MEMENTO_1,
    BUZZER_MEMENTO_2,
    BUZZER_MEMENTO_3,
    BUZZER_MEMENTO_4,
    BUZZER_MEMENTO_5,
    BUZZER_SERVER_CONNECT,
    BUZZER_AP_CONNECT,
    BUZZER_AP_PAIRING,

    BUZZ_NUM
} E_BuzzId_T;

//==============================================================================
// Buzzer Note Definition
//==============================================================================

#if (OSC_16MHZ == 1)
/* 4��Ÿ�� */
#define Do4       (30577U)
#define Dos4      (28861U)
#define Re4       (27241U)
#define Mi4       (24269U)
#define Pa4       (22907U)
#define Pas4      (21621U)
#define Sol4      (20407U)
#define Sols4     (19262U)
#define La4       (18181U)
#define Si4       (16197U)

#else // OSC_32MHZ
/* 4��Ÿ�� */
#define Do4       (61155U)
#define Dos4      (57723U)
#define Re4       (54483U)
#define Mi4       (48539U)
#define Pa4       (45814U)
#define Pas4      (43243U)
#define Sol4      (40816U)
#define Sols4     (38525U)
#define La4       (36363U)
#define Si4       (32395U)
#endif

// 5��Ÿ��
#define Do5   (Do4/2)  //(((7647+1)*8)-1)    // �� : 523.2511Hz = 1.912ms
#define Dos5  (Dos4/2)  //(14432*4)           // ��#: 554.3653Hz = 1.804ms
#define Re5   (Re4/2)  //(((6815+1)*8)-1)    // �� : 587.3295Hz = 1.704ms
#define Mi5   (Mi4/2)  //(((6067+1)*8)-1)    // �� : 659.2551Hz = 1.517ms
#define Pa5   (Pa4/2)  //(((5731+1)*8)-1)    // �� : 698.4565Hz = 1.433ms
#define Pas5  (Pas4/2)  //(((5403+1)*8)-1)    // ��#: 739.9888Hz = 1.351ms
#define Sol5  (Sol4/2)  //(((5103+1)*8)-1)    // �� : 783.9909Hz = 1.276ms
#define Sols5 (Sols4/2)  //(9087*4)            // ��#: 830.6094Hz = 1.204ms
#define La5   (La4/2)  //(((4543+1)*8)-1)    // �� : 880.0000Hz = 1.136ms
#define Si5   (Si4/2)  //(((3999+1)*8)-1)    // �� : 987.7666Hz = 1.012ms
// 6��Ÿ��
#define Do6   (Do5/2)  	// �� : 1046.502Hz = 0.955ms
#define Dos6  (Dos5/2)  //(7215*4)            // ��#: 1108.731Hz = 0.902ms
#define Re6   (Re5/2)  	//(((3403+1)*8)-1)    // �� : 1174.659Hz = 0.851ms
#define Mi6   (Mi5/2)  	//(((3031+1)*8)-1)    // �� : 1318.510Hz = 0.758ms
#define Pa6   (Pa5/2)		//(((2863+1)*8)-1)    // �� : 1396.913Hz = 0.716ms 
#define Pas6  (Pas5/2)	//(((2703+1)*8)-1)    // ��#: 1479.978Hz = 0.676ms
#define Sol6  (Sol5/2)  //(((2551+1)*8)-1)    // �� : 1567.982Hz = 0.638ms
#define Sols6 (Sols5/2) //(4815*4)            // ��#: 1661.219Hz = 0.602ms
#define La6   (La5/2)		//(((2271+1)*8)-1)    // �� : 1760.000Hz = 0.568ms
#define Si6   (Si5/2)		//(((2023+1)*8)-1)    // �� : 1975.533Hz = 0.506ms
// 7��Ÿ��
#define Do7   (Do6/2) //(((1911+1)*8)-1)    // �� : 2093.005Hz = 0.478ms
#define Dos7  (Dos6/2) //(3607*4)            // ��#: 2217.461Hz = 0.451ms
#define Re7   (Re6/2) //(((1703+1)*8)-1)    // �� : 2349.318Hz = 0.426ms
#define Mi7   (Mi6/2)

/*******************************************************************************
  FUNCTION NAME: InitializeBuzzer

DESCRIPTION: ���� �ʱ�ȭ

PARAMETERS: X

RETURN VALUE: X

NOTES: While�� ���� �ʱ�ȭ �� ���
 *******************************************************************************/
void InitializeBuzzer(void);

/*******************************************************************************
  FUNCTION NAME: SetBuzzOut

DESCRIPTION: ���� ��¼���

PARAMETERS: mu8BuzzId

RETURN VALUE: X

NOTES: ������� ȣ���Լ�
 *******************************************************************************/
void SetBuzzOut( E_BuzzId_T mu8BuzzId );

/*******************************************************************************
  FUNCTION NAME: IsSetSilent

DESCRIPTION: ���� ������� ����Ȯ��

PARAMETERS: X

RETURN VALUE: Buzzer.Silent (0:CLEAR / 1:SET)

NOTES: �������� ȣ���Լ�
 *******************************************************************************/
U8 IsSetSilent(void);

/*******************************************************************************
  FUNCTION NAME: SetOnOffSilent

DESCRIPTION: ���� ������� ����

PARAMETERS: mu8Silent (ON/OFF)

RETURN VALUE: X

NOTES: �������� ȣ���Լ�
 *******************************************************************************/
void SetOnOffSilent( U8 mu8Silent );

/*******************************************************************************
  FUNCTION NAME: BuzzerControl

DESCRIPTION: ���� ����

PARAMETERS: X

RETURN VALUE: X

NOTES: 1ms �ֱ�
 *******************************************************************************/
void BuzzerControl(void);

#endif
