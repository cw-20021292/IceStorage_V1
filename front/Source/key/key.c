/**
 * File : key.c
 * 
 * Key Scan Control
**/
#include "key.h"

#include "api_key.h"

void InitKey(void)
{
    InitApiKey();
}

/**
 * @ brief          Function For Scan Key
 * @ detail         none
 * @ param          none
 * @ return         none
**/
void ProcessScanKey(void)
{
    ScanKeyFront();
    ScanKeyOut();
}
