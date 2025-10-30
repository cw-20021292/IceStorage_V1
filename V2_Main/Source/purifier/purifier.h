/**
 * File : purifier.h
 *
 * Pufifier
*/
#ifndef __PURIFIER_H__
#define __PURIFIER_H__

#include "prj_type.h"

void InitPurifier(void);

void ExportVersion(const char *ver);

void SetMainVerMajor(U8 ver);
U8 GetMainVerMajor(void);
void SetMainVerEvent(U8 ver);
U8 GetMainVerEvent(void);
void SetMainVerPatch(U8 ver);
U8 GetMainVerPatch(void);
void SetMainVerMiner(U8 ver);
U8 GetMainVerMiner(void);

void SetFrontVerMajor(U8 ver);
U8 GetFrontVerMajor(void);
void SetFrontVerEvent(U8 ver);
U8 GetFrontVerEvent(void);
void SetFrontVerPatch(U8 ver);
U8 GetFrontVerPatch(void);
void SetFrontVerMiner(U8 ver);
U8 GetFrontVerMiner(void);

#endif  /* __PURIFIER_H__ */
