#ifndef __VALVE_H__
#define __VALVE_H__

#include "prj_type.h"


#define VALVE_NOS               0x0001
#define VALVE_ROOM_COLD_OUT     0x0002
#define VALVE_HOT_IN            0x0004
#define VALVE_HOT_OUT           0x0008
#define VALVE_HOT_DRAIN         0x0010
#define VALVE_COLD_AIR          0x0020
// #define VALVE_COLD_OUT          0x0040
#define VALVE_ROOM_IN           0x0040          // 정수입수밸브
#define VALVE_COLD_IN           0x0080
#define VALVE_COLD_DRAIN        0x0100
#define VALVE_COLD_FLUSH        0x0200
#define VALVE_ICE_TRAY_IN       0x0400


#define VALVE_ALL ( \
        VALVE_NOS \
        | VALVE_COLD_AIR \
        | VALVE_COLD_DRAIN \
        | VALVE_HOT_IN \
        | VALVE_ROOM_COLD_OUT \
        | VALVE_HOT_OUT \
        | VALVE_ICE_TRAY_IN \
        | VALVE_COLD_IN \
        | VALVE_COLD_FLUSH \
        | VALVE_HOT_DRAIN \
        | VALVE_ROOM_IN \
        )

#define NORMAL_OPEN_VALVES  ( \
        VALVE_NOS \
        | VALVE_HOT_DRAIN \
        | VALVE_COLD_AIR )

void InitValve(void);

void OpenValve(U16 mu16Valve);

void CloseValve(U16 mu16Valve);

U16 GetValveStatus(void);

U8 IsOpenValve(U16 mu16Valve );

void ControlValve(void);

#endif /* __VALVE_H__ */
