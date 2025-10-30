#include "voice.h"
#include "timer.h"
#include "eeprom.h"


/*
 * AIS 1.0 시나리오
 *
 * 1. 공장 테스트 모드에서는 볼륨 최대
 * 2. 오후 10시 부터 ~ 오전 6시까지는 강제로 최소 볼륨
 *
 */

typedef struct _voice_
{
    U8 Level;       // System Level
    U8 Volume;      // Volume
    VoiceId_T   Id;
} Voice_T;

Voice_T Voice;


void InitVoice(void)
{
    Voice.Level  = DEFAULT_VOICE_LEVEL;
    Voice.Volume = DEFAULT_VOICE_VOLUME;
    Voice.Id     = ID_NONE;
}

void SetVoiceLevel(U8 mu8Level )
{
    Voice.Level = mu8Level;

    SaveEepromId( EEP_ID_VOICE_LEVEL );
}


U8 GetVoiceLevel(void)
{
    return Voice.Level;
}


void PlayVoice(VoiceId_T id)
{
    Voice.Id = id;
}

VoiceId_T GetVoicePlayId(void)
{
    return Voice.Id;
}


void SetVoiceVolume(U8 mu8Vol)
{
    Voice.Volume = mu8Vol;
    SaveEepromId( EEP_ID_VOICE_VOLUME );
}

U8 GetVoiceVolume(void)
{
    return Voice.Volume;
}

void VolumeUp(void)
{
    if( Voice.Volume < MAX_VOICE_VOLUME )
    {
        Voice.Volume++;
        SaveEepromId( EEP_ID_VOICE_VOLUME );
    }
}

void VolumeDown(void)
{
    if( Voice.Volume > MIN_VOICE_VOLUME )
    {
        Voice.Volume--;
        SaveEepromId( EEP_ID_VOICE_VOLUME );
    }
}

U8 IsVoiceMaxVol( VoiceId_T mId )
{
    if( mId == ID_TEST_MODE_EOL 
            || mId == ID_LOGO )
    {
        return TRUE;    // max vol
    }

    return FALSE;   
}
