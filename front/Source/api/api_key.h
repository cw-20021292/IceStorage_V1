/**
 * File : api_key.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#ifndef __API_KEY_H__
#define __API_KEY_H__

#include "prj_type.h"

#define CONFIG_USE_FRONT_KEY    (1)
#define CONFIG_USE_MAIN_KEY     (0)
#define CONFIG_USE_OUT_KEY      (1)

#define KEY_NONE       (0x00000000UL)

/// Front Key
#define KEY_1          (0x00000001UL)      /// Front - LOCK ALL
#define KEY_2          (0x00000002UL)      /// Front - COLD OFF
#define KEY_3          (0x00000004UL)      /// Front - HOT LOCK
#define KEY_4          (0x00000008UL)      /// Front - ICE WATER LOCK
#define KEY_5          (0x00000010UL)      /// Front - ICE LOCK
#define KEY_6          (0x00000020UL)      /// Front - HOT OFF
#define KEY_7          (0x00000040UL)      /// Front - ICE OFF
#define KEY_8          (0x00000080UL)      /// Front - ICE WATER AMOUNT
#define KEY_9          (0x00000100UL)      /// Front - HOT
#define KEY_10         (0x00000200UL)      /// Front - ROOM
#define KEY_11         (0x00000400UL)      /// Front - COLD
#define KEY_12         (0x00000800UL)      /// Front - AMOUNT
#define KEY_13         (0x00001000UL)      /// Front - ICE
#define KEY_14         (0x00002000UL)      /// Front - ICE WATER

/// Out Key
#define KEY_15         (0x00004000UL)      // Front - OUT ICE
#define KEY_16         (0x00008000UL)      // Front - OUT WATER

#define FRONT_KEY      ( \
                       KEY_1|KEY_2|KEY_3|KEY_4|KEY_5 \
                       KEY_6|KEY_7|KEY_8|KEY_9|KEY_10 \
                       KEY_11|KEY_12|KEY_13|KEY_14 \
                       )
#define OUT_KEY        (KEY_15|KEY_16)

#define KEY_UNUSED     (KEY_NONE)
#define KEY_ALL        (0x0000FFFFUL & ~KEY_UNUSED)

void InitApiKey(void);

void SetFrontKey(U32 key);
U32 GetFrontKey(void);

void SetOutKey(U32 xUkey);
U32 GetOutKey(void);

U32 GetKey(void);

#if( CONFIG_USE_FRONT_KEY == 1 )
void ScanKeyFront(void);
#endif

#if( CONFIG_USE_MAIN_KEY == 1 )
void ScanKeyMain(void);
#endif

#if( CONFIG_USE_OUT_KEY == 1 )
void ScanKeyOut(void);
#endif

#endif  /* __API_KEY_H__ */
