#ifndef __HAL_INPUT_H__
#define __HAL_INPUT_H__


/* INPUT ID */
enum
{
    IN_MODEL,
    IN_FILTER,

    IN_MAX_NUM
};


#define MK_INPUT_MODEL      0x01
#define MK_INPUT_FILTER     0x02
U8 HAL_GetInput(void);

U8 HAL_GetInputValue(U8  mu8Id );

void HAL_ProcessInput(void);

#endif /* __HAL_INPUT_H__ */
