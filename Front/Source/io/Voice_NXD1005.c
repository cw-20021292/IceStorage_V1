#include "prj_type.h"
#include "voice_nxd1005.h"
#include "hal_spi.h"


/* REGISTER */
#define VOICE_COMMAND_CONTROL0      0xE260 
#define VOICE_COMMAND_CONTROL1      0xE305 /*Disable, 비동기, 18bit, disable*/

#define VOICE_COMMAND_ADDRESS_0     0xE000
#define VOICE_COMMAND_ADDRESS_256   0xE400
#define VOICE_COMMAND_ADDRESS_512   0xE401

#define VOICE_COMMAND_STOP          0xE210

/* VOLUME */
//#define REG_VOLUME_5                0x001E  /* -6 dB */
//#define REG_VOLUME_4                0x0021  /* -9 dB */
//#define REG_VOLUME_3                0x0024  /* -12 dB */
//#define REG_VOLUME_2                0x0027  /* -15 dB */
//#define REG_VOLUME_1                0x002A  /* -3 dB */

#define REG_VOLUME_5                0x0024  
#define REG_VOLUME_4                0x0027  
#define REG_VOLUME_3                0x002A  
#define REG_VOLUME_2                0x0030  
#define REG_VOLUME_1                0x0036  
const static U16 gu16VoiceVolumeRegList[] = 
{ 
    REG_VOLUME_1,
    REG_VOLUME_2,
    REG_VOLUME_3,
    REG_VOLUME_4,
    REG_VOLUME_5
};


#define PLAY_STOP               0
#define PLAY_START              1

#define DEFAULT_VOLUME          2
#define MAX_VOLUME              4

#define DEFAULT_DELAY_TIME      0   // 300ms @100ms
typedef struct _nxd1005_
{
    U8  Play;
    U16 Address;
    U16 Delay;
    U8  Volume;
} NXD_T;

NXD_T   Nxd;


void NXD_Init(void)
{
    HAL_SPI_Init();

    HAL_SPI_Write( VOICE_COMMAND_CONTROL0 ); 
    HAL_SPI_Write( VOICE_COMMAND_CONTROL1 );

    Nxd.Play    = PLAY_STOP;
    Nxd.Delay   = DEFAULT_DELAY_TIME;
    Nxd.Volume  = DEFAULT_VOLUME;
}


void NXD_Stop( void )
{
    HAL_SPI_Write( VOICE_COMMAND_STOP );
}

/*key 입력시 Paly_A_Voice(VOICE_XXXX); 선언*/
void NXD_Play(int id) 
{
    Nxd.Address = (U16)id;
    Nxd.Play    = PLAY_START;
    Nxd.Delay   = DEFAULT_DELAY_TIME;

    /*Voice Stop*/
    NXD_Stop();
}


static void SetVolume(void)
{
    U16 gu16Command;

    /* Set Volume */
    gu16Command = 0xE100 + gu16VoiceVolumeRegList[ Nxd.Volume ];
    HAL_SPI_Write( gu16Command );
}

void NXD_Volume(int volume)
{
    Nxd.Volume = (U8)volume;
    if( Nxd.Volume >= MAX_VOLUME )
    {
        Nxd.Volume = MAX_VOLUME;
    }

//    SetVolume();
}


void NXD_ProcessVoice(void)
{
    U16 gu16Command;

    if( Nxd.Delay != 0 ) 
    {
        Nxd.Delay--;
        return ;
    }

    if ( Nxd.Play == PLAY_START )
    {
        Nxd.Play = PLAY_STOP;

        SetVolume();

        /* Play */
        if (Nxd.Address < 256)
        {                                       
            HAL_SPI_Write( VOICE_COMMAND_ADDRESS_256 );

            HAL_SPI_Write( VOICE_COMMAND_ADDRESS_0 + Nxd.Address );
        }
        else if ( Nxd.Address < 512)
        {   
            HAL_SPI_Write( VOICE_COMMAND_ADDRESS_512 );

            gu16Command = VOICE_COMMAND_ADDRESS_0 + (Nxd.Address - 256);
            HAL_SPI_Write( gu16Command );
        }
    }
}

