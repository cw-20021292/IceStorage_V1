/**
 * File : purifier.c
 * 
 * Pufifier 
**/
#include "purifier.h"

typedef struct _purifier_
{
    /* Main Version */
    U8 MainVerMajor;                   // 1 : 양산적용, 0 : 테스트 버전(dpp, lpp), 9 : dummy
    U8 MainVerEvent;                   // 0 : 양산적용, 1 : dpp, 2 : lpp
    U8 MainVerPatch;                   // Version Patch ( 0~255 )
    U8 MainVerMiner;                   // Versioon Number ( 0~255 )

    /* Front Version */
    U8 FrontVerMajor;                   // 1 : 양산적용, 0 : 테스트 버전(dpp, lpp), 9 : dummy
    U8 FrontVerEvent;                   // 0 : 양산적용, 1 : dpp, 2 : lpp
    U8 FrontVerPatch;                   // Version Patch ( 0~255 )
    U8 FrontVerMiner;                   // Versioon Number ( 0~255 )
} SPurifier_T;

SPurifier_T Purifier;

void InitPurifier(void)
{
    Purifier.MainVerMajor = 0;
    Purifier.MainVerEvent = 0;
    Purifier.MainVerPatch = 0;
    Purifier.MainVerMiner = 0;

    Purifier.FrontVerMajor = 0;
    Purifier.FrontVerEvent = 0;
    Purifier.FrontVerPatch = 0;
    Purifier.FrontVerMiner = 0;
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
        Purifier.FrontVerMajor = (U8)major;
        Purifier.FrontVerEvent = (U8)event;
        Purifier.FrontVerPatch = (U8)patch;
        Purifier.FrontVerMiner = (U8)miner;
    }                                   
}

/// Main Version
void SetMainVerMajor(U8 ver)
{
    Purifier.MainVerMajor = ver;
}

U8 GetMainVerMajor(void)
{
    return Purifier.MainVerMajor;
}

void SetMainVerEvent(U8 ver)
{
    Purifier.MainVerEvent = ver;
}

U8 GetMainVerEvent(void)
{
    return Purifier.MainVerEvent;
}

void SetMainVerPatch(U8 ver)
{
    Purifier.MainVerPatch = ver;
}

U8 GetMainVerPatch(void)
{
    return Purifier.MainVerPatch;
}

void SetMainVerMiner(U8 ver)
{
    Purifier.MainVerMiner = ver;
}

U8 GetMainVerMiner(void)
{
    return Purifier.MainVerMiner;
}

/// Front Version
U8 GetFrontVerMajor(void)
{
    return Purifier.FrontVerMajor;
}

U8 GetFrontVerEvent(void)
{
    return Purifier.FrontVerEvent;
}

U8 GetFrontVerPatch(void)
{
    return Purifier.FrontVerPatch;
}

U8 GetFrontVerMiner(void)
{
    return Purifier.FrontVerMiner;
}
