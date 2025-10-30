#ifndef __HAL_INPUT_H__
#define __HAL_INPUT_H__


/* INPUT ID */
enum
{
    IN_FILTER,

    IN_TANK_OPEN,
    IN_TANK_OPEN_UV,        // uv off하기 위해서 감지 시간이 짧음

    IN_TANK_OPEN_1,
    IN_TANK_OPEN_2,

    IN_HEATER,

    IN_LEVEL_DRAIN_HIGH,
    IN_LEVEL_DRAIN_LOW,

    IN_MAX_NUM
};

#define INPUT_DELAY_30MS   3U        /* 30ms @10ms */
#define INPUT_DELAY_100MS  10U       /* 100ms @10ms */
#define INPUT_DELAY_300MS  30U       /* 300ms @10ms */
#define INPUT_DELAY_1SEC   100U      /* 1sec @10ms */

void InitInputConfDetectTime(void);

void SetInputConfDetectTime(U8 mu8Id, U8 mu16DetectTime);

#if 0
#define MK_INPUT_FILTER             0x00000001UL
#define MK_INPUT_TANK_OPEN          0x00000002UL
#define MK_INPUT_TANK_OPEN_UV       0x00000004UL
#define MK_INPUT_HEATER_PBA         0x00000008UL
U32 HAL_GetInput(void);
#endif

U8   HAL_GetHeaterPBA(void);

U8 HAL_GetInputValue(U8  mu8Id );

void HAL_ProcessInput(void);

#endif /* __HAL_INPUT_H__ */
