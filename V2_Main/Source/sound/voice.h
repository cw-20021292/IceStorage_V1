#ifndef __VOICE_H__
#define __VOICE_H__

#include "prj_type.h"

/* ID LIST */
typedef enum
{
    /* 1 ~ 47 :  OLD VERSION */
    ID_ALARAM_COVER_OPEN_TOP    = 15,   //제품 상단 커버가 열려 있습니다. 커버를 닫아주세요. ( 제품 조작시 )

    /* 48 ~ 40 : EMPTY AREA */

    /* 48 ~ 107 */
    ID_COVER_OPEN_BOTTOM       = 48,    //하단 본체에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
    ID_COVER_OPEN_TOP,                  //제품 상단에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
    ID_COVER_OPEN_FRONT,                //제품 전면에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
    ID_COVER_OPEN_SIDE,                 //제품 측면에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.

    ID_ERROR_COLD              = 52,    //냉수 기능에 이상이 감지되었습니다. 아이오케어 앱가이드를 확인해주시거나 증상이 반복되시면 콜센터에 연락해주세요.
    ID_ERROR_COLD_FILTER,               //냉수 기능에 이상이 감지되었습니다. 제품의 먼지 필터를 세척해주세요. 세척 후에도 냉수가 차갑지 않다면 콜센터에 연락해주세요.
    ID_ERROR_HOT,                       //온수 기능에 이상이 감지되었습니다. 아이오케어 앱가이드를 확인해주시거나 증상이 반복되시면 콜센터에 연락해주세요.
    ID_ERROR_OUTAGE,                    //물 공급에 이상이 감지되었습니다. 원수 공급 밸브가 열려 있는지 확인해주세요. 아이오케어 앱가이드를 확인해주시거나 증상이 반복되시면 콜센터에 연락해주세요.
    ID_ERROR_LEAK,                      //누수가 감지되었습니다. 안전을 위해 원수 공급 밸브를 닫아주세요. 아이오케어 앱가이드를 확인해주시거나 증상이 반복되시면 콜센터에 연락해주세요.
    ID_ERROR_SENSOR_CUP,                //컵 감지 센서 이상이 감지되었습니다. 아이오케어 앱가이드를 확인해주시거나 증상이 반복되시면 콜센터에 연락해주세요.

    // 필터 교체 플러싱
    ID_FILTER_FLUSHING_READY    = 58,  //필터를 교체하셨나요. 교체 하셨다면 추출 버튼을 눌러주세요. 깨끗한 물을 위한 준비 과정이 시작됩니다.
    _ID_FILTER_CHECK,                  //필터가 올바르게 결합되지 않은 것으로 감지되었습니다. 제품 커버를 열어 다시 한번 확인해주세요.
    ID_FILTER_CHECK             = 115, //필터가 올바르게 결합되지 않은 것으로 감지되었습니다. 다시 한번 확인해주세요
    ID_FILTER_FLUSHING          = 60,  //새 필터 장착 후 깨끗한 물을 제공하기 위해 준비중에 있습니다. 잠시만 기다려주세요.
    ID_FILTER_FLUSHED,                 //필터 시스템 준비가 완료되었습니다. 이제 코웨이에 깨끗한 물을 드실 수 있습니다.

    _ID_SMART_DIAG_ERROR,               //스마트 진단 결과 이상이 감지되었습니다. 아이오케어 앱을 통해 상세 내용을 확인해주세요.

    ID_HOT_PREHEATING,                 //온수 온도를 맞추고 있습니다. 잠시만 기다려주세요.

    ID_WIFI_CONNECTING          = 64,   //와이파이 연결을 시작합니다.
    ID_WIFI_CONNECTED,                  //와이파이 연결이 완료되었습니다. 이제부터 아이오케어 앱을 통해 스마트한 관리를 받으실 수 있습니다.
    ID_WIFI_FAIL_CONNECTING,            //와이파이 연결이 실패되었습니다. 인터넷 공유기를 확인해주세요. 문제가 반복되면 콜센터에 연락주세요.
    __ID_WIFI_CONNECTING,               //와이파이 연결을 시작합니다.
    __ID_WIFI_CONNECTED,                //와이파이 연결이 완료되었습니다.
    ID_WIFI_DISCONNECTED,               //와이파이 연결이 해제되었습니다. 다시 연결하시려면 제품 설정 모드에서 와이파이를 켜주세요.

    // 초기 설치 플러싱
    ID_SETUP_FLUSHING_GUIDE     = 70,   //안녕하세요. 고객님. 코웨이 정수기의 설치를 도와드리겠습니다. 추출 버튼을 누르시면 설치를 시작하겠습니다.
    ID_SETUP_FLUSHING,                  //새 정수기 제품에 깨끗한 물을 제공하기 위해 준비중입니다.
    ID_SETUP_FLUSHED,                   //깨끗한 물을 제공하기 위한 준비가 완료되었습니다.

    // BLE 페어링
    ID_BLE_START_CONNECTING     = 73,   //블루투스를 통해 아이오케어 앱에 연결을 시작합니다.
    ID_BLE_CONNTED,                     //블루투스를 통해 아이오케어 앱 연결이 완료되었습니다.
    ID_BLE_FAIL,                        //블루투스를 통한 아이오케어 앱 연결이 실패하였습니다. 핸드폰 블루투스가 켜져 있는지 확인해주세요.

    ID_HOT_CAREFUL              = 76,   // 온수가 뜨거우니 주의하세요
    _ID_HOT_CAREFUL,                    // 온수가 뜨거우니 주의하세요

    // 미사용...
    __ID_COVER_OPEN_BOTTOM      = 78,   //하단 본체에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
    __ID_COVER_OPEN_TOP,                //제품 상단에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
    __ID_COVER_OPEN_FRONT,              //제품 전면에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
    __ID_COVER_OPEN_SIDE,               //제품 측면에 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.

    // 버튼 조작시 알람음
    ID_ALARM_LOCK_HOT           = 82,   //온수 버튼이 잠겨있습니다. 온수 버튼을 3초 누르시면 선택하신 온수를 다시 사용하실 수 있습니다.
    ID_ALARM_COLD_OFF,                  //냉수가 꺼져있습니다. 냉수 버튼을 3초 누르시면 선택하신 냉수를 다시 사용하실 수 있습니다.
    ID_ALARM_LOCK_ALL,                  //CHP 모든 버튼이 잠겨있습니다. 전체 잠금 버튼을 3초 누르시면 모든 기능을 다시 사용하실 수 있습니다.
    ID_ALARM_LOCK_ALL_COMBI,            //CP TYPE 모든 버튼이 잠겨있습니다. 냉수와 용량 버튼을 동시에 3초 누르시면 모든 기능을 다시 사용하실 수 있습니다.

    ID_WATER_PREPARING          = 86,   //깨끗한 물을 준비중입니다.
    ID_WATER_READY,                     //깨끗한 물에 준비가 완료되었습니다. 
    ID_WATER_STER,                      //코웨이 살균 시스템으로 유로를 깨끗하고 안전하게 살균 중입니다.

    ID_LOCK_HOT                 = 89,   //온수 버튼이 잠겼습니다.
    ID_UNLOCK_HOT,                      //온수 버튼을 사용하실 수 있습니다.
    ID_LOCK_ALL,                        //모든 버튼이 잠겼습니다.
    _ID_LOCK_ALL,                       //모든 버튼이 잠겼습니다.
    ID_UNLOCK,                          //모든 버튼을 사용하실 수 있습니다.

    ID_SOUND_VOICE              = 94,   //음성 안내 모드 입니다.
    ID_SOUND_EFFECT,                    //효과음 모드 입니다.
    ID_SOUND_MUTE,                      //무음 모드입니다.

    ID_CANCEL_COLD              = 97,   //냉수 기능이 꺼졌습니다.
    ID_SETUP_COLD,                      //냉수 기능이 켜졌습니다. 설정하신 온도까지 시간이 다소 소요됩니다.
    ID_SETTING,                         //설정 모드를 선택하셨습니다. 코웨이 정수기에 다양한 기능을 설정하실 수 있습니다.

    ID_EFFLUENT_CONTINUE        = 100,  //연속추출음
    ID_EFFLUENT,                        //추출 시작음
    ID_EFFLUENT_END,                    //추출 종료음
    ID_LOGO,                            //효과음 4 - 코오오웨에에이~
    ID_SELECT,                          //땡
    ID_SELECT_ERROR,                    //땡땡
    ID_SETUP,                           //띵동
    ID_CANCEL,                          //띠잉동

    ID_TEST_MODE_EOL            = 109,  // 특수 모드에 진입하였습니다.

    // ICON 2.0 
    ID_B026_1_HOT_OUT           = 135,  // 고온수가 추출 됩니다. 컵을 추출구에 가까이 해주세요.
    ID_B026_2_HOT_OUT,                  // 고온수가 추출 됩니다. 컵을 추출구에 가까이 놓아주세요.
    ID_B009_1_HOT_NO_WARM,              // 온수 첫 잔은 뜨겁지 않을 수 있습니다. 첫 잔을 버린 후 사용해주세요.

    // ICON AIS 
    ID_01_001_OPEN_FRONT        = 150,	// 제품 전면의 커버가 열렸습니다. 안전을 위해 물 공급을 자동으로 차단합니다.
    ID_01_002_01_OPEN_ICE_TANK,	        // 얼음저장고의 커버가 열렸습니다. 안전을 위해 얼음 생성을 중지합니다.
    ID_01_003_01_CLOSE_ICE_TANK,	    // 얼음저장고의 커버가 닫혔습니다.다시 얼음 생성을 시작합니다.

    ID_B_003_ERR_COLD,	                // 냉수 기능에 이상이 감지되었습니다. 증상이 반복되면 ioCare앱 가이드에 따라 확인해주시거나 콜센터에 연락해주세요.
    ID_B_004_ERR_HOT,	                // 온수 기능에 이상이 감지되었습니다. 증상이 반복되면 ioCare앱 가이드에 따라 확인해주시거나 콜센터에 연락해주세요.
    ID_B_005_ERR_ICE,	                // 얼음 기능에 이상이 감지되었습니다.증상이 반복되면 ioCare앱 가이드에 따라 확인해주시거나 콜센터에 연락해주세요.
    ID_B_006_ERR_OUTAGE,	            // 물 공급에 이상이 감지되었습니다. 원수공급밸브가 열려있는지 확인해주세요.증상이 반복되면 ioCare앱 가이드에 따라 확인해주시거나 콜센터에 연락해주세요.
    ID_B_007_ERR_LEAK,	                // 누수가 감지되었습니다. 안전을 위해 원수공급밸브를 닫아주세요.증상이 반복되면 ioCare앱 가이드에 따라 확인해주시거나 콜센터에 연락해주세요.

    ID_02_001_READY_FLUSH_FILTER,	    // 필터를 교체하셨나요? 교체하셨다면 물 추출버튼을 눌러주세요. 교체하지 않으셨다면 '냉수' 버튼을 눌러주세요. 
    ID_02_002_CHECK_FILTER,	            // 필터가 올바르게 결합되지 않은 것으로 감지되었습니다. 제품 커버를 열어 다시 한 번 확인해주세요.
    ID_02_003_FLUSHING_FILTER,	        // 새 필터 장착 후 깨끗한 물을 제공하기 위해 준비 중에 있습니다. 잠시만 기다려 주세요.
    ID_02_004_FLUSHED_FILTER,	        // 필터 시스템 준비가 완료되었습니다. 이제 코웨이의 깨끗한 물을 드실 수 있습니다.

    ID_02_005_START_SMART_CHECK,	    // 제품의 상태를 파악하기 위해 스마트 진단을 시작합니다.잠시만 기다려주세요.
    ID_02_006_DONE_SMART_CHECK,	        // 스마트 진단 결과, 제품이 정상 작동 중입니다. 안심하고 사용하세요.
    ID_02_007_ERR_SMART_CHECK,	        // 스마트 진단 결과 이상이 감지되었습니다. ioCare앱을 통해 상세 내용을 확인해주세요.

    ID_03_004_WIFI_CONNECTING,	        // 와이파이 연결을 시작합니다.
    ID_03_005_WIFI_CONNECTED,	        // 와이파이 연결이 완료되었습니다.이제부터 ioCare 앱을 통해 스마트한 관리를 받으실 수 있습니다.
    ID_B_010_WIFI_FAIL_CONNECTING,	    // 와이파이 연결에 실패하였습니다. 인터넷 공유기를 확인해주시고, 문제가 반복되면 콜센터에 연락해주세요 
    ID_03_005_x_WIFI_CONNECTED,	        // 와이파이 연결이 완료되었습니다.
    ID_03_008_WIFI_DISCONN,	            // 와이파이 연결이 해제되었습니다. 다시 연결하시려면 냉수와 용량 버튼을 동시에 3초 눌러주세요. 

    ID_03_009_01_READY_FLUSH_SETUP,	    // 안녕하세요. 고객님코웨이 정수기의 설치를 도와드리겠습니다. 물 추출버튼을 누르시면 설치를 시작하겠습니다.
    ID_03_010_FLUSHING_STEUP,	        // 새 정수기 제품의 깨끗한 물을 제공하기 위해  준비 중입니다.
    ID_03_011_01_FLUSHED_SETUP,	        // 깨끗한 물을 제공하기 위한 준비가 완료되었습니다

    ID_03_012_BLE_CONNECTING,	        // 블루투스를 통해 ioCare 앱에 연결을 시작합니다.
    ID_03_013_BLE_CONNECTED,	        // 블루투스를 통해 ioCare 앱 연결이 완료되었습니다.
    ID_03_014_BLE_FAIL_CONNECTING,	    // 블루투스 통한 ioCare 앱 연결이 실패하였습니다. 핸드폰 블루투스가 켜져있는지 확인해주세요

    ID_06_017_STER_START,	            // 코웨이의 살균수로 유로 살균 서비스를 시작합니다.
    ID_06_018_STER_GOING,	            // 유로 헹굼을 시작합니다.
    ID_06_019_STER_DONE,	            // 유로 살균 서비스를 완료했습니다.
    ID_07_001_HOT_OUT_USER,	            // 고온수가 추출됩니다. 컵을 추출구에 가까이 해주세요.
    ID_04_001_HOT_OUT,	                // 온수가 뜨거우니 주의하세요
    ID_05_002_LOCK_HOT_OUT,	            // 온수 버튼이 잠겨있습니다. 설정에서 온수잠금을 해제하시면 온수를 다시 사용하실 수 있습니다.
    ID_05_003_OFF_COLD,	                // 냉수 기능이 꺼져있습니다. 설정에서 냉수OFF를 해제하시면 냉수를 다시 사용하실 수 있습니다.
    ID_05_004_LOCK_ALL,	                // 모든 버튼이 잠겨있습니다. 전체 잠금 버튼을 3초 누르시면 모든 기능을 다시 사용하실 수 있습니다.
    ID_05_005_PREPARING_ICE,            // 얼음을 준비하고 있습니다.잠시만 기다려주세요.
    ID_05_006_ALARAM_ICE_OUT,           // 얼음 추출 버튼을 계속 누르고 있는 동안 얼음이 추출됩니다.
    ID_05_007_LOCK_ICE,	                // 얼음 추출 버튼이 잠겨있습니다. 설정에서 얼음잠금을 해제하시면얼음을 다시 사용하실 수 있습니다.
    ID_05_008_OFF_ICE,	                // 얼음 기능이 꺼져있습니다. 설정에서 얼음OFF를 해제하시면 얼음을 다시 만들 수 있습니다. 
    ID_05_009_ALARM_ON_SLEEP,	        // 취침모드 사용을 위해, WIFI를 연결하시거나 제품에서 현재 시간을 설정해주세요 
    ID_B_019_01_ON_SLEEP,	            // 취침모드 시간에는 얼음이 충분하지 않을 수 있습니다.취침모드 해제를 원하시면,설정에서 취침모드 버튼을 눌러주세요. 
    ID_06_001_LOCK_HOT,	                // 온수 버튼이 잠겼습니다.
    ID_06_002_UNLOCK_HOT,	            // 온수 버튼을 사용하실 수 있습니다.
    ID_06_003_LOCK_ALL,	                // 모든 버튼이 잠겼습니다.

    ID_06_005_UNLOCK_ALL        = 194,	// 모든 버튼을 사용하실 수 있습니다
    ID_06_006_SOUND_VOICE,	            // 음성안내 모드입니다.
    ID_06_007_SOUND_EFFECT,	            // 효과음 모드입니다.
    ID_06_008_SOUND_MUTE,	            // 무음 모드 입니다.

    ID_06_009_OFF_COLD,	                // 냉수 기능이 꺼졌습니다.
    ID_06_010_02_OFF_COLD,	            // 냉수 기능이 켜졌습니다. 적정 온도 도달까지 시간이 다소 소요됩니다.
    ID_06_011_LOCK_ICE,	                // 얼음 추출 버튼이 잠겼습니다.
    ID_06_012_UNLOCK_ICE,	            // 얼음 추출 버튼을 사용하실 수 있습니다.
    ID_06_013_ON_ICE,	                // 얼음 기능이 켜졌습니다. 얼음을 만드는데 시간이 다소 소요됩니다
    ID_06_014_OFF_ICE,	                // 얼음 기능이 꺼졌습니다.
    ID_06_015_ON_SLEEP,	                // 취침모드가 설정되었습니다. 설정된 취침시간에는 얼음 생성을 중지합니다.
    ID_06_016_OFF_SLEEP,	            // 취침모드가 해제되었습니다.
    ID_06_021_MY_AMOUNT,	            // 고객님의 My 용량이 설정되었습니다.
    ID_07_003_SETTING,	                // 설정버튼을 3초 누르시면 코웨이 정수기의 다양한 기능을 설정하실 수 있습니다.

    ID_NONE         = 255

} VoiceId_T;


void InitVoice(void);


/* VOICE LEVEL */
#define VOICE_LEVEL_OFF           1   // VOICE OFF, BEEP OFF  ( 무음 )
#define VOICE_LEVEL_BEEP          2   // VOICE OFF, BEEP ON   ( 효과음 )
#define VOICE_LEVEL_VOICE_BEEP    3   // VOICE ON,  BEEP ON   ( 음성 )
#define MIN_VOICE_LEVEL           VOICE_LEVEL_OFF
#define MAX_VOICE_LEVEL           VOICE_LEVEL_VOICE_BEEP 
#define DEFAULT_VOICE_LEVEL       VOICE_LEVEL_VOICE_BEEP

void SetVoiceLevel(U8 mu8Level);
U8 GetVoiceLevel(void);

void PlayVoice(VoiceId_T id);

VoiceId_T GetVoicePlayId(void);

/* VOICE VOLUME */
#define VOICE_VOLUME_1          1       // OFF ( MUTE )
#define VOICE_VOLUME_2          2       // VOL 1
#define VOICE_VOLUME_3          3       // VOL 2 ( DEFAULT )
#define VOICE_VOLUME_4          4       // VOL 3
#define VOICE_VOLUME_5          5       // VOL 4
#define MIN_VOICE_VOLUME        VOICE_VOLUME_1
#define MAX_VOICE_VOLUME        VOICE_VOLUME_5

#define DEFAULT_VOICE_VOLUME    VOICE_VOLUME_3
void SetVoiceVolume(U8 mu8Vol);
U8 GetVoiceVolume(void);
void VolumeUp(void);
void VolumeDown(void);

U8 IsVoiceMaxVol( VoiceId_T mId );

#endif /* __VOICE_H__ */
