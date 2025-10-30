/**
 * File : purifier.h
 *
 * Pufifier
**/
#ifndef __PURIFIER_H__
#define __PURIFIER_H__

#include "prj_type.h"

void InitPurifier(void);

void ExportVersion(char *ver);

/// Main Version
void SetMainVerMajor(U8 ver);
U8 GetMainVerMajor(void);
void SetMainVerEvent(U8 ver);
U8 GetMainVerEvent(void);
void SetMainVerPatch(U8 ver);
U8 GetMainVerPatch(void);
void SetMainVerMiner(U8 ver);
U8 GetMainVerMiner(void);

/// Front Version
U8 GetFrontVerMajor(void);
U8 GetFrontVerEvent(void);
U8 GetFrontVerPatch(void);
U8 GetFrontVerMiner(void);

#endif  /* __PURIFIER_H__ */
