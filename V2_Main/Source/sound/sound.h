#ifndef __SOUND_H__
#define __SOUND_H__

#include "prj_type.h"


typedef enum
{
    // 1 - 제품이상감지
    SOUND_COVER_OPEN_FRONT,        // 1-1
    SOUND_COVER_OPEN_TOP,          // 1-2
    SOUND_COVER_CLOSE_TOP,         // 1-3
    SOUND_ERROR_COLD,              // 1-4
    SOUND_ERROR_HOT,               // 1-5
    SOUND_ERROR_ICE,               // 1-6
    SOUND_ERROR_OUTAGE,            // 1-7
    SOUND_ERROR_LEAK,              // 1-8


    // 2 - 자가관리(필터교체/스마트진단)
    SOUND_FILTER_FLUSHING_READY,   
    SOUND_FILTER_CHECK,
    SOUND_FILTER_FLUSHING,         
    SOUND_FILTER_FLUSHED,          

    SOUND_SMART_DIAG_START,
    SOUND_SMART_DIAG_NO_ERR_DONE,
    SOUND_SMART_DIAG_ERR_DONE,


    // 3 - 특수모드
    SOUND_HOT_OUT,             

    SOUND_WIFI_CONNECTING,         
    SOUND_WIFI_CONNECTED_FIRST,          
    SOUND_WIFI_CONNECTED,          
    SOUND_WIFI_FAIL_CONNECTING,    
    SOUND_WIFI_DISCONNECTED,       

    SOUND_SETUP_FLUSHING_GUIDE,    
    SOUND_SETUP_FLUSHING,          
    SOUND_SETUP_FLUSHED,           

    SOUND_BLE_START_CONNECTING,    
    SOUND_BLE_CONNTED,             
    SOUND_BLE_FAIL,                

    SOUND_STER_START, 
    SOUND_STER_RINSE, 
    SOUND_STER_DONE,  

    // 4- 위험 주의 - none

    // 5 - 의도한대로 동작하지 않을 시
    SOUND_ALARM_COVER_OPEN_FRONT,  
    SOUND_ALARM_LOCK_HOT,          
    SOUND_ALARM_COLD_OFF,          
    SOUND_ALARM_LOCK_ALL,          
    SOUND_ALARM_ICE,
    SOUND_ALARM_ICE_OUT,
    SOUND_ALARM_ICE_LOCK,
    SOUND_ALARM_ICE_OFF,
    SOUND_ALARM_SLEEP_TIME,
    SOUND_ALARM_SLEEP_NO_ICE,


    // 6 - 설정 완료 후
    SOUND_LOCK_HOT,                 
    SOUND_UNLOCK_HOT,              
    SOUND_LOCK_ALL,                
    SOUND_UNLOCK_ALL,                  

    SOUND_SETTING_MUTE,              
    SOUND_SETTING_EFFECT,            
    SOUND_SETTING_VOICE,             

    SOUND_CANCEL_COLD ,            
    SOUND_SETUP_COLD ,             

    SOUND_LOCK_ICE,  
    SOUND_UNLOCK_ICE,

    SOUND_CANCEL_ICE ,            
    SOUND_SETUP_ICE ,             
    SOUND_CANCEL_SLEEP ,            
    SOUND_SETUP_SLEEP ,             
    SOUND_SETUP_MY_AMOUNT ,             


    // 7 - ETC
    SOUND_HOT_OUT_USER,      /* 고온수 추출 안내 */
    SOUND_HOT_OUT_USER_OUT,  /* 고온수 추출 - 효과음 모드에서 소리 두번 나는거 개선 목적 */
    SOUND_SETTING_MENU,


    // 8 - 효과음
    SOUND_EFFLUENT_CONTINUE,       
    SOUND_EFFLUENT,                
    SOUND_EFFLUENT_END,            

    SOUND_SELECT,                   
    SOUND_ERROR,                   
    SOUND_SETUP,                   
    SOUND_CANCEL,                  

    SOUND_MEMENTO_1,         	/* 메멘토 1 */
    SOUND_MEMENTO_2,         	/* 메멘토 2 */
    SOUND_MEMENTO_3,         	/* 메멘토 3 */
    SOUND_MEMENTO_4,         	/* 메멘토 4 */
    SOUND_MEMENTO_5,         	/* 메멘토 5 */

    SOUND_POWER_ON,                
    SOUND_EOL,         	        /* 특수 모드*/

    SOUND_NONE,			      	/* 부저음 발생 없음 */
    MAX_SOUND_NUM              
}SoundId_T;

void InitSound(void);

/* 무음 설정 확인 */
U8	IsSetSoundMute(void);

/* 무음 설정  */
void	SetSoundMute(void);
U8    GetSoundMute(void);

/* 무음 해제 */
void	ClearSoundMute(void);

/* 소리 출력 */
void  Sound(SoundId_T mId);

/* 사운드 테스트 */
void TEST_Sound(void);

void UpdateSoundTimer(void);


#endif /* _SOUND_H_ */
