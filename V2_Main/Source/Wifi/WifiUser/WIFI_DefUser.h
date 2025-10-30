
#ifndef _WIFI_USER_DEFINE_H_
#define _WIFI_USER_DEFINE_H_

/***** USER Use ***************************************************************/
/* User Status */
typedef enum
{
    WIFI_TIME_INI = 0x00,
    WIFI_TIME_SET,
    WIFI_TIME_FACTORY,
    WIFI_TIME_WHILE,
    WIFI_TIME_100MS,
    WIFI_TIME_1SEC,
    WIFI_TIME_1MIN
} E_WIFI_TIME_T;

typedef enum
{
    WIFI_KEY_AP = 0x00,
    WIFI_KEY_BLE,
    WIFI_KEY_ON,
    WIFI_KEY_OFF,
    WIFI_KEY_ONOFF,
    WIFI_KEY_TEST,
    WIFI_KEY_AP_DUAL,
    WIFI_KEY_BLE_CERT,
    WIFI_KEY_BLE_CERT_CANCEL
} E_WIFI_KEY_T;

typedef enum
{
    WIFI_DATA_FUNCTION = 0x00,
    WIFI_DATA_FUNCTION_CONTROL,
    WIFI_DATA_SENSOR,
    WIFI_DATA_ERROR,
    WIFI_DATA_PART,
    WIFI_DATA_EXAMINE,
    WIFI_DATA_INFO
} E_WIFI_DATA_T;

typedef enum
{
    WIFI_STATUS_DISP = 0x00,
    WIFI_STATUS_POWER,
    WIFI_STATUS_TEST,
    WIFI_STATUS_ERROR,
    WIFI_STATUS_AP_FIRST,
    WIFI_STATUS_AP_MODE,
    WIFI_STATUS_AP_BLE,
    WIFI_STATUS_BLE_CERT,
    WIFI_STATUS_AP_STEP,
    WIFI_STATUS_AP_ERROR
} E_WIFI_STATUS_T;

typedef enum
{
    WIFI_DISP_OFF = 0x00,   // off
    WIFI_DISP_AP1,          // on 0.5s - off 0.5s
    WIFI_DISP_AP2,          // on 0.5s - off 0.5s
    WIFI_DISP_AP3,          // on 0.5s - off 0.5s
    WIFI_DISP_AP4,          // on 0.5s - off 0.5s
    WIFI_DISP_AP5,          // on 0.5s - off 0.5s
    WIFI_DISP_TRY,          // on 0.5s - off 0.5s
    WIFI_DISP_APMODE,       // on 0.1s - off 0.1s - on 0.1s - off 0.7s 
    WIFI_DISP_CONNECT,      // on

    WIFI_DISP_AP_START = 0x10, // White LED on 0.1s - off 0.1s - on 0.1s - off 0.7s
    WIFI_DISP_AP_STEP0,
    WIFI_DISP_AP_STEP1,
    WIFI_DISP_AP_STEP2,
    WIFI_DISP_AP_STEP3,

    WIFI_DISP_BLE_START = 0x20, // Blue LED on 0.1s - off 0.1s - on 0.1s - off 0.7s
    WIFI_DISP_BLE_STEP0,
    WIFI_DISP_BLE_STEP1,
    WIFI_DISP_BLE_STEP2,
    WIFI_DISP_BLE_STEP3
} E_WIFI_DISPLAY_T;

typedef enum
{
    WIFI_RQST_YEAR = 0x0000,
    WIFI_RQST_MONTH,
    WIFI_RQST_DATE,
    WIFI_RQST_HOUR,
    WIFI_RQST_MINUTE,
    WIFI_RQST_SECOND,
    WIFI_RQST_DAY,
    WIFI_RQST_PART_PERIOD,
    
    WIFI_RQST_WEATHER = 0x0010,
    WIFI_RQST_AQI,
    WIFI_RQST_PM10,
    WIFI_RQST_PM25,
    WIFI_RQST_CURRENT_TEMP,
    WIFI_RQST_HIGH_TEMP,
    WIFI_RQST_LOW_TEMP,
    
    WIFI_RQST_LAST_HEART_YEAR = 0x0020,
    WIFI_RQST_LAST_HEART_MONTH,
    WIFI_RQST_LAST_HEART_DAY,
    WIFI_RQST_NEXT_HEART_YEAR,
    WIFI_RQST_NEXT_HEART_MONTH,
    WIFI_RQST_NEXT_HEART_DAY,
    WIFI_RQST_FILTER_D_DAY1,
    WIFI_RQST_FILTER_D_DAY2,
    WIFI_RQST_FILTER_CHANGE_CYCLE
    
} E_WIFI_RQST_T;

typedef enum
{
    WIFI_AP_STEP_CLEAR = 0U,
    WIFI_AP_STEP_WAIT,
    WIFI_AP_STEP_START = 25U,
    WIFI_AP_STEP_MODEM = 50U,
    WIFI_AP_STEP_ROOTCA = 75U
} E_WIFI_AP_STEP_T;


#define TDEC_Wifi(a) if (a != 0) a--;

/*==============================================================================
 Timer Decrease
 ==============================================================================*/
#define	TDEC_1ms_WiFi()			{	/*	1ms 타이머 감산			*/	\
								}
#define	TDEC_10ms_WiFi()			{	/*	10ms 타이머 감산		*/	\
								}
#define	TDEC_100ms_WiFi()		{	/*	100ms 타이머 감산		*/	\
								}
#define	TDEC_1s_WiFi()			{	/*	1s 타이머 감산			*/	\
								}
#define	TDEC_10s_WiFi()			{	/*	10s 타이머 감산			*/	\
								}
#define	TDEC_1min_WiFi()		{	/*	1min 타이머 감산		*/	\
								}
#define	TDEC_10min_WiFi()		{	/*	10min 타이머 감산		*/	\
								}
#define	TDEC_1h_WiFi()			{	/*	1h 타이머 감산			*/	\

#endif
