/**
 * File : led.h
 * 
 * Led
**/
#ifndef __LED_H__
#define __LED_H__

#include "prj_type.h"

#define LED_TYPE_DUTY_DIM   (0x00)
#define LED_TYPE_DUTY       (0x10)

#define PROTOCOL_LED_DUTY_0   (0x00)
#define PROTOCOL_LED_DUTY_10  (0x01)
#define PROTOCOL_LED_DUTY_20  (0x02)
#define PROTOCOL_LED_DUTY_30  (0x03)
#define PROTOCOL_LED_DUTY_40  (0x04)
#define PROTOCOL_LED_DUTY_50  (0x05)
#define PROTOCOL_LED_DUTY_60  (0x06)
#define PROTOCOL_LED_DUTY_70  (0x07)
#define PROTOCOL_LED_DUTY_80  (0x08)
#define PROTOCOL_LED_DUTY_90  (0x09)
#define PROTOCOL_LED_DUTY_100 (0x0A)

typedef enum _led_id_
{
    LED_ID_SEG_1A,                      /// 14 :
    LED_ID_SEG_1B,                      /// 15 :
    LED_ID_SEG_1C,                      /// 16 :
    LED_ID_SEG_1D,                      /// 17 :
    LED_ID_SEG_1E,                      /// 18 :
    LED_ID_SEG_1F,                      /// 19 :
    LED_ID_SEG_1G,                      /// 20 :

    LED_ID_SEG_2A,                      /// 14 :
    LED_ID_SEG_2B,                      /// 15 :
    LED_ID_SEG_2C,                      /// 16 :
    LED_ID_SEG_2D,                      /// 17 :
    LED_ID_SEG_2E,                      /// 18 :
    LED_ID_SEG_2F,                      /// 19 :
    LED_ID_SEG_2G,                      /// 20 :

    LED_ID_SEG_3A,                      /// 21 :
    LED_ID_SEG_3B,                      /// 22 :
    LED_ID_SEG_3C,                      /// 23 :
    LED_ID_SEG_3D,                      /// 24 :
    LED_ID_SEG_3E,                      /// 25 :
    LED_ID_SEG_3F,                      /// 26 :
    LED_ID_SEG_3G,                      /// 27 :

    LED_ID_SEG_4A,                      /// 28 :
    LED_ID_SEG_4B,                      /// 29 :
    LED_ID_SEG_4C,                      /// 30 :
    LED_ID_SEG_4D,                      /// 31 :
    LED_ID_SEG_4E,                      /// 32 :
    LED_ID_SEG_4F,                      /// 33 :
    LED_ID_SEG_4G,                      /// 34 :

    LED_ID_SEG_5A,                      /// 28 :
    LED_ID_SEG_5B,                      /// 29 :
    LED_ID_SEG_5C,                      /// 30 :
    LED_ID_SEG_5D,                      /// 31 :
    LED_ID_SEG_5E,                      /// 32 :
    LED_ID_SEG_5F,                      /// 33 :
    LED_ID_SEG_5G,                      /// 34 :

    LED_ID_SEG_6A,                      /// 28 :
    LED_ID_SEG_6B,                      /// 29 :
    LED_ID_SEG_6C,                      /// 30 :
    LED_ID_SEG_6D,                      /// 31 :
    LED_ID_SEG_6E,                      /// 32 :
    LED_ID_SEG_6F,                      /// 33 :
    LED_ID_SEG_6G,                      /// 34 :

    LED_ID_MAX,
} ELedId_T;

void InitLed(void);
void OnOffLed(ELedId_T ledId, U8 duty);
void AllOffLed(void);

U8 ConvertProtocol2Duty(U8 data);
U8 ConvertDuty2Protocol(ELedId_T led);

void ControlLed(void);

#endif  /* __LED_H__ */
