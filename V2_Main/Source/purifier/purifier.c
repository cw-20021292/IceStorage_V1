/**
 * File : purifier.c
 * 
 * Pufifier 
**/
#include "purifier.h"

#include "eeprom.h"

typedef struct _purifier_
{
    /* Main Version */
    U8 mainVerMajor;                   // 1 : 양산적용, 0 : 테스트 버전(dpp, lpp), 9 : dummy
    U8 mainVerEvent;                   // 0 : 양산적용, 1 : dpp, 2 : lpp
    U8 mainVerPatch;                   // Version Patch ( 0~255 )
    U8 mainVerMiner;                   // Versioon Number ( 0~255 )

    /* Front Version */
    U8 frontVerMajor;                   // 1 : 양산적용, 0 : 테스트 버전(dpp, lpp), 9 : dummy
    U8 frontVerEvent;                   // 0 : 양산적용, 1 : dpp, 2 : lpp
    U8 frontVerPatch;                   // Version Patch ( 0~255 )
    U8 frontVerMiner;                   // Versioon Number ( 0~255 )
} SPurifier_T;

SPurifier_T purifier;

void InitPurifier(void)
{
    purifier.mainVerMajor = 0;
    purifier.mainVerEvent = 0;
    purifier.mainVerPatch = 0;
    purifier.mainVerMiner = 0;

    purifier.frontVerMajor = 0;
    purifier.frontVerEvent = 0;
    purifier.frontVerPatch = 0;
    purifier.frontVerMiner = 0;
}

/// Export Version
void ExportVersion(const char *ver)
{
    int major = 0U;
    int event = 0U;
    int patch = 0U;
    int miner = 0U;

    if( SSCANF(ver, "V%d.%d.%d.%d", &major, &event, &patch, &miner) == 4 )
    {
        purifier.mainVerMajor = (U8)major;
        purifier.mainVerEvent = (U8)event;
        purifier.mainVerPatch = (U8)patch;
        purifier.mainVerMiner = (U8)miner;
    }                                   
}

/// Main Version
void SetMainVerMajor(U8 ver)
{
    if( purifier.mainVerMajor != ver )
    {
        //SaveEepromId(EEPROM_ID_VERSION);         
    }

    purifier.mainVerMajor = ver;
}

U8 GetMainVerMajor(void)
{
    return purifier.mainVerMajor;
}

void SetMainVerEvent(U8 ver)
{
    purifier.mainVerEvent = ver;
}

U8 GetMainVerEvent(void)
{
    return purifier.mainVerEvent;
}

void SetMainVerPatch(U8 ver)
{
    purifier.mainVerPatch = ver;
}

U8 GetMainVerPatch(void)
{
    return purifier.mainVerPatch;
}

void SetMainVerMiner(U8 ver)
{
    if( purifier.mainVerMiner != ver )
    {
        //SaveEepromId(EEPROM_ID_VERSION);         
    }

    purifier.mainVerMiner = ver;
}

U8 GetMainVerMiner(void)
{
    return purifier.mainVerMiner;
}

/// Front Version
void SetFrontVerMajor(U8 ver)
{
    if( purifier.frontVerMajor != ver )
    {
        //SaveEepromId(EEPROM_ID_VERSION);         
    }

    purifier.frontVerMajor = ver;
}

U8 GetFrontVerMajor(void)
{
    return purifier.frontVerMajor;
}

void SetFrontVerEvent(U8 ver)
{
    purifier.frontVerEvent = ver;
}

U8 GetFrontVerEvent(void)
{
    return purifier.frontVerEvent;
}

void SetFrontVerPatch(U8 ver)
{
    purifier.frontVerPatch = ver;
}

U8 GetFrontVerPatch(void)
{
    return purifier.frontVerPatch;
}
void SetFrontVerMiner(U8 ver)
{
    if( purifier.frontVerMiner != ver )
    {
        //SaveEepromId(EEPROM_ID_VERSION);         
    }

    purifier.frontVerMiner = ver;
}

U8 GetFrontVerMiner(void)
{
    return purifier.frontVerMiner;
}
