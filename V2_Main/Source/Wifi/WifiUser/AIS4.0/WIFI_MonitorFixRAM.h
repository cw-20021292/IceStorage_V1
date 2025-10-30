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
 
__directmap     U8     gu8_A301_COLD_WATER_LEV_LOW       = {0xFFC00}; // 냉수저수위
__directmap     U8     gu8_A303_COLD_WATER_LEV_HIGH      = {0xFFC01}; // 냉수만수위
__directmap     U8     gu8_A603_VALVE_PURE_OUT           = {0xFFC02}; // 정수추출밸브
__directmap     U8     gu8_A605_VALVE_HOT_OUT            = {0xFFC03}; // 온수추출밸브
__directmap     U8     gu8_A609_VALVE_COLD_OUT           = {0xFFC04}; // 냉수추출밸브
__directmap     U16     gu16_A701_COLD_TEMP              = {0xFFC05}; // 냉수온도
__directmap     U16     gu16_A705_HOT_TEMP               = {0xFFC07}; // 온수온도
__directmap     U16     gu16_A708_OUT_TEMP               = {0xFFC09}; // 외기온도
__directmap     U8     gu8_AA01_FAN_STATUS               = {0xFFC0B}; // FAN상태
__directmap     U8    gu8_AF01_COLD_STATUS               = {0xFFC0C};  // 냉각운전상태
__directmap     U8    gu8_FF_HOT_STATUS                  = {0xFFC0D};  // 온수운전상태
__directmap     U8    gu8_FF_OPERATION                   = {0xFFC0E};  // 운전상태
__directmap     U8     gu8_AF04_COMP_OPERATION_STATUS    = {0xFFC0F};  // COMP 상태
__directmap     U8     gu8_B201_HOT_OPERATION_STATUS     = {0xFFC10};  // 히터 상태
__directmap     U8      gu8_A901_ERROR_CODE              = {0xFFC11};  // 에러코드
__directmap     U8     gu8_FF_HOT_Heater_ControlNum      = {0xFFC12};  // 고도모드
__directmap     U16    gu16_FF_HOT_AddOperation          = {0xFFC13};  // 온수추가시간
__directmap     U16    gu16_FF_COLD_AddOperation         = {0xFFC15};  // 냉각추가시간
__directmap     U16    gu16_A709_EVA_TEMP                = {0xFFC17};  // 증발기온도센서
__directmap     U16    gu16_AC02_ICE_FULL_SENSOR         = {0xFFC19};  // 만빙감지
__directmap     U8    gu16_B001_ICEMAKE_STATUS           = {0xFFC1B};  // 제빙상태
__directmap     U16    gu16_A707_PURE_WATER_TEMP         = {0xFFC1C};  // 정수온도센서
__directmap     U16    gu16_FF_MIX_OUT_TEMP              = {0xFFC1E};  // 추출온도센서
__directmap     U16    gu16_FF_TRAY_IN_TEMP              = {0xFFC20};  // 트레이입수온도센서
__directmap     U16    gu16_FF_OUT2_TEMP                 = {0xFFC22};  // 외기온도센서2
__directmap     U8     gu8_FF_SMART_SAMRT_SAVING_STATE   = {0xFFC24};  // 스마트절전상태
__directmap     U16    gu16_A703_HOT_IN_TEMP             = {0xFFC25};  // 온수입수온도
__directmap     U16    gu16_A704_HEATER_TEMP             = {0xFFC27};  // 온수히터온도
__directmap     U8     gu16_A401_DRAIN_TANK_LEV_LOW      = {0xFFC29};  // 드레인탱크 저수위
__directmap     U8     gu16_A402_DRAIN_TANK_LEV_HIGH     = {0xFFC2A};  // 드레인탱크 만수위
__directmap     U8     gu16_A_U16_B001_ICEMAKE_STATUS    = {0xFFC2C};  // 제빙상태(2바이트)
__directmap     U8     gu16_A_U16_FF_COMP_ONOFF_TIME     = {0xFFC2E};  // COMP 재가동 시간

/***** Define Address *****************************************************************/ 
 
#define     A_U8_A301_COLD_WATER_LEV_LOW      (0xFC00)       // 냉수저수위
#define     A_U8_A303_COLD_WATER_LEV_HIGH      (0xFC01)      // 냉수만수위
#define     A_U8_A603_VALVE_PURE_OUT      (0xFC02)           // 정수추출밸브
#define     A_U8_A605_VALVE_HOT_OUT      (0xFC03)            // 온수추출밸브
#define     A_U8_A609_VALVE_COLD_OUT      (0xFC04)           // 냉수추출밸브
#define     A_U16_A701_COLD_TEMP      (0xFC05)               // 냉수온도
#define     A_U16_A705_HOT_TEMP      (0xFC07)                // 온수온도
#define     A_U16_A708_OUT_TEMP      (0xFC09)                // 외기온도
#define     A_U8_AA01_FAN_STATUS      (0xFC0B)               // FAN상태
#define     A_U8_AF04_COMP_OPERATION_STATUS      (0xFC0F)    // COMP 상태
#define     A_U8_B201_HOT_OPERATION_STATUS      (0xFC10)     // 히터 상태
#define     A_U8_A901_ERROR_CODE      (0xFC11)               // ERROR CODE
#define     A_FF_HOT_Heater_ControlNum      (0xFC12)         // 고도모드
#define     A_FF_HOT_AddOperation      (0xFC13)              // 온수추가시간
#define     A_FF_COLD_AddOperation      (0xFC15)             // 냉각추가시간
#define     A_AF01_COLD_STATUS      (0xFC0C)                 // 냉각운전상태
#define     A_FF_HOT_STATUS      (0xFC0D)                    // 온수운전상태
#define     A_FF_OPERATION      (0xFC0E)                     // 운전상태
#define     A_U16_A709_EVA_TEMP      (0xFC17)                    // 증발기온도센서
#define     A_U16_AC02_ICE_FULL_SENSOR      (0xFC19)             // 만빙감지
#define     A_U8_B001_ICEMAKE_STATUS      (0xFC1B)                // 제빙상태
#define     A_U16_A707_PURE_WATER_TEMP          (0xFC1C)      // 정수온도센서
#define     A_U16_FF_MIX_OUT_TEMP               (0xFC1E)      // 추출온도센서
#define     A_U16_FF_TRAY_IN_TEMP               (0xFC20)      // 트레이입수온도센서
#define     A_U16_FF_OUT2_TEMP                  (0xFC22)      // 외기온도센서2
#define     A_U8_FF_SMART_SAMRT_SAVING_STATE    (0xFC24)      // 스마트절전상태
#define     A_U16_A703_HOT_IN_TEMP    (0xFC25)      // 온수입수온도
#define     A_U16_A704_HEATER_TEMP    (0xFC27)      // 온수히터온도
#define     A_U16_A401_DRAIN_TANK_LEV_LOW     (0xFC29)      // 드레인탱크 저수위
#define     A_U16_A402_DRAIN_TANK_LEV_HIGH    (0xFC2A)      // 드레인탱크 만수위
#define     A_U16_B001_ICEMAKE_STATUS         (0xFC2C)      // 제빙상태(2바이트)
#define     A_U16_FF_COMP_ONOFF_TIME         (0xFC2C)      // COMP 재가동 시간



