#ifndef __HAL_KEY_H__
#define __HAL_KEY_H__

#define  HAL_KEY_NONE   0x00000000UL
#define  HAL_KEY_1      0x00000001UL
#define  HAL_KEY_2      0x00000002UL
#define  HAL_KEY_3      0x00000004UL
#define  HAL_KEY_4      0x00000008UL

#define  HAL_KEY_5      0x00000010UL
#define  HAL_KEY_6      0x00000020UL
#define  HAL_KEY_7      0x00000040UL
#define  HAL_KEY_8      0x00000080UL

#define  HAL_KEY_9      0x00000100UL
#define  HAL_KEY_10     0x00000200UL
#define  HAL_KEY_11     0x00000400UL
#define  HAL_KEY_12     0x00000800UL
                                 
#define  HAL_KEY_13     0x00001000UL
#define  HAL_KEY_14     0x00002000UL
#define  HAL_KEY_15     0x00004000UL
#define  HAL_KEY_16     0x00008000UL
                                
#define  HAL_KEY_17     0x00010000UL
#define  HAL_KEY_18     0x00020000UL
#define  HAL_KEY_19     0x00040000UL
#define  HAL_KEY_20     0x00080000UL

#define  HAL_KEY_ALL    0x000FFFFFUL
U32 HAL_GetKeyVal(void);


void HAL_ScanKey(void);
#endif /* __HAL_KEY_H__ */
