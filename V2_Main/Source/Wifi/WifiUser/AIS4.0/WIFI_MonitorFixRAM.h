/***** Type Define *****************************************************************/ 
// typedef unsigned char       U8;
// typedef signed char         I8;
// typedef unsigned int        U16; 
// typedef signed int          I16; 

/******** Define Func *********/
void GetRamData (void);
U8 *SetRamData (U16);

/******** typedef *********/
typedef U8 *(*WifiWriteFun_T)(U16); 
typedef struct 
{ 
    U16 AddressList;  
    U8 *TargetVariable;  
    U8 VarSize; 
    WifiWriteFun_T WriteFunc; 
} WifiFixRamList_T; 

/***** Directmap *****************************************************************/ 
 
__directmap     U8     gu8_A301_COLD_WATER_LEV_LOW       = {0xFFC00}; // �ü�������
__directmap     U8     gu8_A303_COLD_WATER_LEV_HIGH      = {0xFFC01}; // �ü�������
__directmap     U8     gu8_A603_VALVE_PURE_OUT           = {0xFFC02}; // ����������
__directmap     U8     gu8_A605_VALVE_HOT_OUT            = {0xFFC03}; // �¼�������
__directmap     U8     gu8_A609_VALVE_COLD_OUT           = {0xFFC04}; // �ü�������
__directmap     U16     gu16_A701_COLD_TEMP              = {0xFFC05}; // �ü��µ�
__directmap     U16     gu16_A705_HOT_TEMP               = {0xFFC07}; // �¼��µ�
__directmap     U16     gu16_A708_OUT_TEMP               = {0xFFC09}; // �ܱ�µ�
__directmap     U8     gu8_AA01_FAN_STATUS               = {0xFFC0B}; // FAN����
__directmap     U8    gu8_AF01_COLD_STATUS               = {0xFFC0C};  // �ð���������
__directmap     U8    gu8_FF_HOT_STATUS                  = {0xFFC0D};  // �¼���������
__directmap     U8    gu8_FF_OPERATION                   = {0xFFC0E};  // ��������
__directmap     U8     gu8_AF04_COMP_OPERATION_STATUS    = {0xFFC0F};  // COMP ����
__directmap     U8     gu8_B201_HOT_OPERATION_STATUS     = {0xFFC10};  // ���� ����
__directmap     U8      gu8_A901_ERROR_CODE              = {0xFFC11};  // �����ڵ�
__directmap     U8     gu8_FF_HOT_Heater_ControlNum      = {0xFFC12};  // �����
__directmap     U16    gu16_FF_HOT_AddOperation          = {0xFFC13};  // �¼��߰��ð�
__directmap     U16    gu16_FF_COLD_AddOperation         = {0xFFC15};  // �ð��߰��ð�
__directmap     U16    gu16_A709_EVA_TEMP                = {0xFFC17};  // ���߱�µ�����
__directmap     U16    gu16_AC02_ICE_FULL_SENSOR         = {0xFFC19};  // ��������
__directmap     U8    gu16_B001_ICEMAKE_STATUS           = {0xFFC1B};  // ��������
__directmap     U16    gu16_A707_PURE_WATER_TEMP         = {0xFFC1C};  // �����µ�����
__directmap     U16    gu16_FF_MIX_OUT_TEMP              = {0xFFC1E};  // ����µ�����
__directmap     U16    gu16_FF_TRAY_IN_TEMP              = {0xFFC20};  // Ʈ�����Լ��µ�����
__directmap     U16    gu16_FF_OUT2_TEMP                 = {0xFFC22};  // �ܱ�µ�����2
__directmap     U8     gu8_FF_SMART_SAMRT_SAVING_STATE   = {0xFFC24};  // ����Ʈ��������
__directmap     U16    gu16_A703_HOT_IN_TEMP             = {0xFFC25};  // �¼��Լ��µ�
__directmap     U16    gu16_A704_HEATER_TEMP             = {0xFFC27};  // �¼����Ϳµ�
__directmap     U8     gu16_A401_DRAIN_TANK_LEV_LOW      = {0xFFC29};  // �巹����ũ ������
__directmap     U8     gu16_A402_DRAIN_TANK_LEV_HIGH     = {0xFFC2A};  // �巹����ũ ������
__directmap     U8     gu16_A_U16_B001_ICEMAKE_STATUS    = {0xFFC2C};  // ��������(2����Ʈ)
__directmap     U8     gu16_A_U16_FF_COMP_ONOFF_TIME     = {0xFFC2E};  // COMP �簡�� �ð�

/***** Define Address *****************************************************************/ 
 
#define     A_U8_A301_COLD_WATER_LEV_LOW      (0xFC00)       // �ü�������
#define     A_U8_A303_COLD_WATER_LEV_HIGH      (0xFC01)      // �ü�������
#define     A_U8_A603_VALVE_PURE_OUT      (0xFC02)           // ����������
#define     A_U8_A605_VALVE_HOT_OUT      (0xFC03)            // �¼�������
#define     A_U8_A609_VALVE_COLD_OUT      (0xFC04)           // �ü�������
#define     A_U16_A701_COLD_TEMP      (0xFC05)               // �ü��µ�
#define     A_U16_A705_HOT_TEMP      (0xFC07)                // �¼��µ�
#define     A_U16_A708_OUT_TEMP      (0xFC09)                // �ܱ�µ�
#define     A_U8_AA01_FAN_STATUS      (0xFC0B)               // FAN����
#define     A_U8_AF04_COMP_OPERATION_STATUS      (0xFC0F)    // COMP ����
#define     A_U8_B201_HOT_OPERATION_STATUS      (0xFC10)     // ���� ����
#define     A_U8_A901_ERROR_CODE      (0xFC11)               // ERROR CODE
#define     A_FF_HOT_Heater_ControlNum      (0xFC12)         // �����
#define     A_FF_HOT_AddOperation      (0xFC13)              // �¼��߰��ð�
#define     A_FF_COLD_AddOperation      (0xFC15)             // �ð��߰��ð�
#define     A_AF01_COLD_STATUS      (0xFC0C)                 // �ð���������
#define     A_FF_HOT_STATUS      (0xFC0D)                    // �¼���������
#define     A_FF_OPERATION      (0xFC0E)                     // ��������
#define     A_U16_A709_EVA_TEMP      (0xFC17)                    // ���߱�µ�����
#define     A_U16_AC02_ICE_FULL_SENSOR      (0xFC19)             // ��������
#define     A_U8_B001_ICEMAKE_STATUS      (0xFC1B)                // ��������
#define     A_U16_A707_PURE_WATER_TEMP          (0xFC1C)      // �����µ�����
#define     A_U16_FF_MIX_OUT_TEMP               (0xFC1E)      // ����µ�����
#define     A_U16_FF_TRAY_IN_TEMP               (0xFC20)      // Ʈ�����Լ��µ�����
#define     A_U16_FF_OUT2_TEMP                  (0xFC22)      // �ܱ�µ�����2
#define     A_U8_FF_SMART_SAMRT_SAVING_STATE    (0xFC24)      // ����Ʈ��������
#define     A_U16_A703_HOT_IN_TEMP    (0xFC25)      // �¼��Լ��µ�
#define     A_U16_A704_HEATER_TEMP    (0xFC27)      // �¼����Ϳµ�
#define     A_U16_A401_DRAIN_TANK_LEV_LOW     (0xFC29)      // �巹����ũ ������
#define     A_U16_A402_DRAIN_TANK_LEV_HIGH    (0xFC2A)      // �巹����ũ ������
#define     A_U16_B001_ICEMAKE_STATUS         (0xFC2C)      // ��������(2����Ʈ)
#define     A_U16_FF_COMP_ONOFF_TIME         (0xFC2C)      // COMP �簡�� �ð�



