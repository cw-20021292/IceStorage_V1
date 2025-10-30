#include "voice.h"
#include "voice_nxd1005.h"


/*
 * AIS 1.0 시나리오
 *
 * 1. 공장 테스트 모드에서는 볼륨 최대
 * 2. 오후 10시 부터 ~ 오전 6시까지는 강제로 최소 볼륨
 *
 */

/* VOICE LEVEL */
#define VOICE_LEVEL_0           0   // L0 - Voice x, Beep x
#define VOICE_LEVEL_1           1   // L1 - Voice level 1, Beep O
#define VOICE_LEVEL_2           2   // L2 - Voice level 2, Beep O
#define VOICE_LEVEL_3           3   // L3 - Voice level 3, Beep O
#define VOICE_LEVEL_DEFAULT     VOICE_LEVEL_2

typedef struct _voice_
{
    U8 Level;       // System Level
    U8 Volume;      // Volume
} Voice_T;


typedef struct _voice_device_
{
    void (*Init)(void);
    void (*Play)(int id);
    void (*Stop)(void);
    void (*Volume)(int vol);

} VoiceDevice_T;

Voice_T Voice;
VoiceDevice_T Device;


void InitVoice(void)
{
    Device.Init     = NXD_Init;
    Device.Play     = NXD_Play;
    Device.Stop     = NXD_Stop;
    Device.Volume   = NXD_Volume;
    
    NXD_Init();
}

void SetVoiceLevel(U8 mu8Level )
{
    Voice.Level = mu8Level ;
}


U8 GetVoiceLevel(void)
{
    return Voice.Level;
}


void PlayVoice(VoiceId_T id)
{
    Device.Play( id );
}

void StopVoice(void)
{
    Device.Stop();
}

void SetVoiceVolume(U8 mu8Vol)
{
    Voice.Volume = mu8Vol;

    Device.Volume( Voice.Volume );
}

U8 GetVoiceVolume(void)
{
    return Voice.Volume;
}
