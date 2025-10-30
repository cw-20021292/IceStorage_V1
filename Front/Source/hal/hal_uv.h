#ifndef __HAL_UV_H__
#define __HAL_UV_H__


/* UV ID */
enum
{
    UV_WATER,
    UV_ICE,

    UV_MAX_NUM
};

void HAL_InitUv(void);

void HAL_SetUvOnOffId(U8 id, U8 OnOff);

void HAL_ControlUv(void);

#endif /* __HAL_UV_H__ */
