#include "sound.h"
#include "eeprom.h"
#include "util.h"

#define CONFIG_BUZZER       0   /* 0: BUZZER, 1:VOICE */

#if CONFIG_BUZZER
#include "buzzer.h"
#else
#include "voice.h"
#endif

typedef struct _sound_
{
    U8   Mute;
    U16  HoldTime;
} Sound_T;

Sound_T sound;


void InitSound(void)
{
    sound.Mute      = OFF;
    sound.HoldTime  = 0;
}

typedef struct _sound_list_t
{
    SoundId_T   SoundId;
    U16         ConfHoldTime;       // 플레이 유지 시간@100ms

#if CONFIG_BUZZER
    E_BuzzId_T  BuzzerId;
#else
    VoiceId_T   VoiceId[ MAX_VOICE_LEVEL ];
#endif

} SoundList_T;

#if CONFIG_BUZZER
SoundList_T  SoundList[] = 
{
    /* SOUND ID */             /* BUZZER ID */
    { SOUND_POWER_OFF         , BUZZER_OFF                  },
    { SOUND_POWER_ON          , BUZZER_POWER_ON             },
    { SOUND_EFFLUENT          , BUZZER_EFFLUENT             },
    { SOUND_EFFLUENT_END      , BUZZER_EFFLUENT_END         },       
    { SOUND_CONFIG_SETUP      , BUZZER_COOL_SETUP           },         
    { SOUND_CONFIG_CANCEL     , BUZZER_COOL_CANCEL          },        
    { SOUND_SELECT            , BUZZER_SELECT               },             
    { SOUND_ERROR             , BUZZER_ERROR                },              
    { SOUND_SETUP             , BUZZER_SETUP                },              
    { SOUND_CANCEL            , BUZZER_CANCEL               },             
    { SOUND_LOCK_SETUP        , BUZZER_SETUP                },              
    { SOUND_LOCK_CANCEL       , BUZZER_CANCEL               },             
    { SOUND_EFFLUENT_CONTINUE , BUZZER_EFFLUENT_CONTINUE    },  
    { SOUND_STERILIZE_START   , BUZZER_STERILIZE_START      },    
    { SOUND_STERILIZE_END     , BUZZER_STERILIZE_END        },      
    { SOUND_MEMENTO_1         , BUZZER_MEMENTO_1            },          
    { SOUND_MEMENTO_2         , BUZZER_MEMENTO_2            },          
    { SOUND_MEMENTO_3         , BUZZER_MEMENTO_3            },          
    { SOUND_MEMENTO_4         , BUZZER_MEMENTO_4            },          
    { SOUND_MEMENTO_5         , BUZZER_MEMENTO_5            },          
    { SOUND_MUTE              , BUZZER_SETUP },               

    { SOUND_AP_CONNECT        , BUZZER_AP_CONNECT },
    { SOUND_AP_PAIRING        , BUZZER_AP_PAIRING },
    { SOUND_SERVER_CONNECT    , BUZZER_SERVER_CONNECT }

    //{ SOUND_COOK_ALARM        , BUZZER_MEMENTO_1_MUTE },               
    //{ SOUND_COOK_ALARM_DONE   , BUZZER_MEMENTO_5_MUTE },               
//    { SOUND_NONE	      , BUZZER_NONE }					
};
#else

const static SoundList_T  SoundList[] = 
{
                                            /* 무음 모드,                   효과음 모드,                     음성 안내 */
    // 1 - 제품이상감지
    { SOUND_COVER_OPEN_FRONT,      0U,      { ID_01_001_OPEN_FRONT,          ID_01_001_OPEN_FRONT,           ID_01_001_OPEN_FRONT        }},
    { SOUND_COVER_OPEN_TOP,        0U,      { ID_01_002_01_OPEN_ICE_TANK,    ID_01_002_01_OPEN_ICE_TANK,     ID_01_002_01_OPEN_ICE_TANK  }},
    { SOUND_COVER_CLOSE_TOP,       0U,      { ID_01_003_01_CLOSE_ICE_TANK,   ID_01_003_01_CLOSE_ICE_TANK,    ID_01_003_01_CLOSE_ICE_TANK  }},
    { SOUND_ERROR_COLD,            30U,      { ID_B_003_ERR_COLD,             ID_B_003_ERR_COLD,              ID_B_003_ERR_COLD           }},   
    { SOUND_ERROR_HOT,             30U,      { ID_B_004_ERR_HOT,              ID_B_004_ERR_HOT,               ID_B_004_ERR_HOT            }},   
    { SOUND_ERROR_ICE,             30U,      { ID_B_005_ERR_ICE,              ID_B_005_ERR_ICE,               ID_B_005_ERR_ICE            }},
    { SOUND_ERROR_OUTAGE,          30U,      { ID_B_006_ERR_OUTAGE,           ID_B_006_ERR_OUTAGE,            ID_B_006_ERR_OUTAGE         }},   
    { SOUND_ERROR_LEAK,            30U,      { ID_B_007_ERR_LEAK,             ID_B_007_ERR_LEAK,              ID_B_007_ERR_LEAK           }},   

    // 2 - 자가관리(필터교체/스마트진단)
    // 필터 교체 플러싱
    { SOUND_FILTER_FLUSHING_READY, 0U,      { ID_02_001_READY_FLUSH_FILTER,      ID_02_001_READY_FLUSH_FILTER,   ID_02_001_READY_FLUSH_FILTER }},
    { SOUND_FILTER_CHECK,          0U,      { ID_02_002_CHECK_FILTER,            ID_02_002_CHECK_FILTER,         ID_02_002_CHECK_FILTER       }}, 
    { SOUND_FILTER_FLUSHING,       0U,      { ID_02_003_FLUSHING_FILTER,         ID_02_003_FLUSHING_FILTER,      ID_02_003_FLUSHING_FILTER    }}, 
    { SOUND_FILTER_FLUSHED,        0U,      { ID_02_004_FLUSHED_FILTER,          ID_02_004_FLUSHED_FILTER,       ID_02_004_FLUSHED_FILTER     }}, 

    // 스마트 고장 진단
    { SOUND_SMART_DIAG_START,      0U,      { ID_02_005_START_SMART_CHECK,       ID_02_005_START_SMART_CHECK,    ID_02_005_START_SMART_CHECK  }},          
    { SOUND_SMART_DIAG_NO_ERR_DONE,0U,      { ID_02_006_DONE_SMART_CHECK,        ID_02_006_DONE_SMART_CHECK,     ID_02_006_DONE_SMART_CHECK   }},          
    { SOUND_SMART_DIAG_ERR_DONE,   0U,      { ID_02_007_ERR_SMART_CHECK,         ID_02_007_ERR_SMART_CHECK,      ID_02_007_ERR_SMART_CHECK    }},          

    // 3 - 특수모드

    // WIFI 안내
    { SOUND_WIFI_CONNECTING,       0U,      { ID_03_004_WIFI_CONNECTING,       ID_03_004_WIFI_CONNECTING,       ID_03_004_WIFI_CONNECTING      }},          
    { SOUND_WIFI_CONNECTED_FIRST,  0U,      { ID_03_005_WIFI_CONNECTED,        ID_03_005_WIFI_CONNECTED,        ID_03_005_WIFI_CONNECTED       }},          
    { SOUND_WIFI_CONNECTED,        0U,      { ID_03_005_x_WIFI_CONNECTED,      ID_03_005_x_WIFI_CONNECTED,      ID_03_005_x_WIFI_CONNECTED     }},          
    { SOUND_WIFI_FAIL_CONNECTING,  0U,      { ID_B_010_WIFI_FAIL_CONNECTING,   ID_B_010_WIFI_FAIL_CONNECTING,   ID_B_010_WIFI_FAIL_CONNECTING  }},    
    { SOUND_WIFI_DISCONNECTED,     0U,      { ID_03_008_WIFI_DISCONN,          ID_03_008_WIFI_DISCONN,          ID_03_008_WIFI_DISCONN         }},       

    // 초기 설치 플러싱
    { SOUND_SETUP_FLUSHING_GUIDE,  0U,      { ID_03_009_01_READY_FLUSH_SETUP,  ID_03_009_01_READY_FLUSH_SETUP,  ID_03_009_01_READY_FLUSH_SETUP }},     
    { SOUND_SETUP_FLUSHING,        0U,      { ID_03_010_FLUSHING_STEUP,        ID_03_010_FLUSHING_STEUP,        ID_03_010_FLUSHING_STEUP       }},          
    { SOUND_SETUP_FLUSHED,         0U,      { ID_03_011_01_FLUSHED_SETUP,      ID_03_011_01_FLUSHED_SETUP,      ID_03_011_01_FLUSHED_SETUP     }},           

    // BLE 페어링
    { SOUND_BLE_START_CONNECTING,  0U,      { ID_03_012_BLE_CONNECTING,      ID_03_012_BLE_CONNECTING,        ID_03_012_BLE_CONNECTING       }},     
    { SOUND_BLE_CONNTED,           0U,      { ID_03_013_BLE_CONNECTED,       ID_03_013_BLE_CONNECTED,         ID_03_013_BLE_CONNECTED        }},             
    { SOUND_BLE_FAIL,              0U,      { ID_03_014_BLE_FAIL_CONNECTING, ID_03_014_BLE_FAIL_CONNECTING,   ID_03_014_BLE_FAIL_CONNECTING  }},                
    
    // 살균서비스
    { SOUND_STER_START,            30U,      { ID_06_017_STER_START,      ID_06_017_STER_START,                  ID_06_017_STER_START        }},
    { SOUND_STER_RINSE,            30U,      { ID_06_018_STER_GOING,      ID_06_018_STER_GOING,                  ID_06_018_STER_GOING,       }}, 
    { SOUND_STER_DONE,             30U,     { ID_06_019_STER_DONE,       ID_06_019_STER_DONE,                   ID_06_019_STER_DONE,        }},

    // 4- 위험 주의 - none
    { SOUND_HOT_OUT_USER,          0U,      { ID_NONE,   ID_EFFLUENT,       ID_07_001_HOT_OUT_USER    }}, 
    { SOUND_HOT_OUT_USER_OUT,      0U,      { ID_NONE,   ID_NONE,           ID_04_001_HOT_OUT         }}, 
    { SOUND_HOT_OUT,               0U,      { ID_NONE,   ID_EFFLUENT,       ID_04_001_HOT_OUT         }}, 
   

    // 5 - 의도한대로 동작하지 않을 시
    // 안내음
    { SOUND_ALARM_COVER_OPEN_FRONT,0U,      { ID_01_001_OPEN_FRONT,          ID_01_001_OPEN_FRONT,       ID_01_001_OPEN_FRONT        }},    
    { SOUND_ALARM_LOCK_HOT,        0U,      { ID_05_002_LOCK_HOT_OUT,        ID_05_002_LOCK_HOT_OUT,     ID_05_002_LOCK_HOT_OUT      }},  
    { SOUND_ALARM_COLD_OFF,        0U,      { ID_05_003_OFF_COLD,            ID_05_003_OFF_COLD,         ID_05_003_OFF_COLD          }},      
    { SOUND_ALARM_LOCK_ALL,        0U,      { ID_05_004_LOCK_ALL,            ID_05_004_LOCK_ALL,         ID_05_004_LOCK_ALL          }},    
    { SOUND_ALARM_ICE,             0U,      { ID_NONE,       				 ID_SELECT,    				 ID_05_005_PREPARING_ICE     }},  
    { SOUND_ALARM_ICE_OUT,         30U,     { ID_NONE,      				 ID_SELECT,   				 ID_05_006_ALARAM_ICE_OUT    }},  
    { SOUND_ALARM_ICE_LOCK,        0U,      { ID_05_007_LOCK_ICE,            ID_05_007_LOCK_ICE,         ID_05_007_LOCK_ICE          }},    
    { SOUND_ALARM_ICE_OFF,         0U,      { ID_05_008_OFF_ICE,             ID_05_008_OFF_ICE,          ID_05_008_OFF_ICE           }},    
    { SOUND_ALARM_SLEEP_TIME,      0U,      { ID_05_009_ALARM_ON_SLEEP,      ID_05_009_ALARM_ON_SLEEP,   ID_05_009_ALARM_ON_SLEEP    }},  
    { SOUND_ALARM_SLEEP_NO_ICE,    0U,      { ID_B_019_01_ON_SLEEP,          ID_B_019_01_ON_SLEEP,       ID_B_019_01_ON_SLEEP        }},    


    // 6 - 설정 완료 후
    { SOUND_LOCK_HOT,              0U,      { ID_NONE,    ID_SETUP,           ID_06_001_LOCK_HOT           }},                 
    { SOUND_UNLOCK_HOT,            0U,      { ID_NONE,    ID_CANCEL,          ID_06_002_UNLOCK_HOT         }},              
    { SOUND_LOCK_ALL,              0U,      { ID_NONE,    ID_SETUP,           ID_06_003_LOCK_ALL           }},                
    { SOUND_UNLOCK_ALL,            0U,      { ID_NONE,    ID_CANCEL,          ID_06_005_UNLOCK_ALL         }},                  
    
    // 음성 타입 변경
    { SOUND_SETTING_VOICE,         30U,     { ID_06_006_SOUND_VOICE,          ID_06_006_SOUND_VOICE,      ID_06_006_SOUND_VOICE   }}, 
    { SOUND_SETTING_EFFECT,        30U,     { ID_06_007_SOUND_EFFECT,         ID_06_007_SOUND_EFFECT,     ID_06_007_SOUND_EFFECT  }},
    { SOUND_SETTING_MUTE,          30U,     { ID_06_008_SOUND_MUTE,           ID_06_008_SOUND_MUTE,       ID_06_008_SOUND_MUTE    }},  

    { SOUND_CANCEL_COLD,           0U,      { ID_NONE,    ID_CANCEL,          ID_06_009_OFF_COLD         }},  
    { SOUND_SETUP_COLD,            0U,      { ID_NONE,    ID_SETUP,           ID_06_010_02_OFF_COLD      }},

    { SOUND_LOCK_ICE,              0U,      { ID_NONE,    ID_SETUP,           ID_06_011_LOCK_ICE         }},  
    { SOUND_UNLOCK_ICE,            0U,      { ID_NONE,    ID_CANCEL,          ID_06_012_UNLOCK_ICE       }},  
    { SOUND_CANCEL_ICE,            0U,      { ID_NONE,    ID_CANCEL,          ID_06_014_OFF_ICE          }},  
    { SOUND_SETUP_ICE,             0U,      { ID_NONE,    ID_SETUP,           ID_06_013_ON_ICE           }},  
    { SOUND_CANCEL_SLEEP,          0U,      { ID_NONE,    ID_CANCEL,          ID_06_016_OFF_SLEEP        }},  
    { SOUND_SETUP_SLEEP,           0U,      { ID_NONE,    ID_SETUP,           ID_06_015_ON_SLEEP         }},  
    { SOUND_SETUP_MY_AMOUNT,       0U,      { ID_NONE,    ID_SETUP,           ID_06_021_MY_AMOUNT        }},  
    { SOUND_SETTING_MENU,          5U,      { ID_NONE,    ID_SELECT,          ID_07_003_SETTING          }},  


    // 8- 일반
    { SOUND_EFFLUENT_CONTINUE,     0U,      { ID_NONE,    ID_EFFLUENT_CONTINUE,  ID_EFFLUENT_CONTINUE   }},  
    { SOUND_EFFLUENT,              0U,      { ID_NONE,    ID_EFFLUENT,           ID_EFFLUENT            }},
    { SOUND_EFFLUENT_END,          0U,      { ID_NONE,    ID_EFFLUENT_END,       ID_EFFLUENT_END        }},       

    // 일반 기능
    { SOUND_SELECT,                0U,      { ID_NONE,    ID_SELECT,          ID_SELECT             }},             
    { SOUND_ERROR,                 0U,      { ID_NONE,    ID_SELECT_ERROR,    ID_SELECT_ERROR       }},              
    { SOUND_SETUP,                 0U,      { ID_NONE,    ID_SETUP,           ID_SETUP              }},              
    { SOUND_CANCEL,                0U,      { ID_NONE,    ID_CANCEL,          ID_CANCEL             }},             

    //{ SOUND_MEMENTO_1,             0U,      { ID_NONE,    ID_SELECT,          ID_SELECT             }},          
    //{ SOUND_MEMENTO_2,             0U,      { ID_NONE,    ID_SELECT,          ID_SELECT             }},          
    //{ SOUND_MEMENTO_3,             0U,      { ID_NONE,    ID_SELECT,          ID_SELECT             }},          
    //{ SOUND_MEMENTO_4,             0U,      { ID_NONE,    ID_SELECT,          ID_SELECT             }},          
    //{ SOUND_MEMENTO_5,             0U,      { ID_NONE,    ID_SELECT,          ID_SELECT             }},          

    { SOUND_POWER_ON,              50U,      { ID_LOGO,                  ID_LOGO,                  ID_LOGO                }},
    { SOUND_EOL,                   50U,      { ID_TEST_MODE_EOL,         ID_TEST_MODE_EOL,         ID_TEST_MODE_EOL       }},          

};

#endif


#define	SZ_LIST		( sizeof( SoundList ) / sizeof( SoundList_T ) )

void Sound(SoundId_T mId)
{
    U8 i;
    U8 mVoiceIndex;
    SoundList_T *pList = NULL;

    if( sound.HoldTime != 0 )
    {
        return ;
    }

    mVoiceIndex = SetMinMax8( GetVoiceLevel(), MIN_VOICE_LEVEL, MAX_VOICE_LEVEL );
    mVoiceIndex = mVoiceIndex - 1U /* Index offset */ ;
    for( i = 0; i < SZ_LIST ; i++ )
    {
        pList = &SoundList[ i ];
        if( pList->SoundId == mId )
        {
#if CONFIG_BUZZER
            SetBuzzOut( pList->BuzzerId );
#else
            if( pList->VoiceId[ mVoiceIndex ] != ID_NONE )
            {
                PlayVoice( pList->VoiceId[ mVoiceIndex ] );
                sound.HoldTime =  pList->ConfHoldTime;
            }
#endif
            break;
        }
    }

}

U8	IsSetSoundMute(void)
{
#if CONFIG_BUZZER
    return IsSetSilent();
#else
    return FALSE;
#endif
}

void SetSoundMute(void)
{
    sound.Mute = ON;
    SaveEepromId( EEP_ID_CONF_MUTE );

#if CONFIG_BUZZER
    SetOnOffSilent( ON );
#endif
}

U8    GetSoundMute(void)
{
    return sound.Mute;
}

void	ClearSoundMute(void)
{
    sound.Mute = OFF;
    SaveEepromId( EEP_ID_CONF_MUTE );

#if CONFIG_BUZZER
    SetOnOffSilent( OFF );
#endif
}

void TEST_Sound(void)
{
    static U8 count = 0;


    Sound( count );
    count++;
    if( count >= MAX_SOUND_NUM )
    {
        count = 0;
    }
}

void UpdateSoundTimer(void)
{
    if( sound.HoldTime != 0 )
    {
        sound.HoldTime--;
    }

}
