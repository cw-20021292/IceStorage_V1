#ifndef __HAL_LEVEL_H__
#define __HAL_LEVEL_H__


/* 수위 센서 감지 플래그 */
#define  LEVEL_DETECT_NONE          0x00     // 센서 감지 없음
#define  LEVEL_DETECT_LOW           0x01     // 저수위 센서 감지
#define  LEVEL_DETECT_HIGH          0x02     // 만수위 센서 감지
#define  LEVEL_DETECT_OVF           0x04     // 물넘침 센서 감지

/* 수위 센서 상태 */            
#define  HAL_LEVEL_LOW            ( LEVEL_DETECT_NONE | LEVEL_DETECT_NONE )
#define  HAL_LEVEL_MID            ( LEVEL_DETECT_LOW  | LEVEL_DETECT_NONE )
#define  HAL_LEVEL_ERR_LOW        ( LEVEL_DETECT_NONE | LEVEL_DETECT_HIGH )
#define  HAL_LEVEL_HIGH           ( LEVEL_DETECT_LOW  | LEVEL_DETECT_HIGH )

#define  HAL_LEVEL_ERR_COMPLEX    ( LEVEL_DETECT_OVF  | HAL_LEVEL_LOW     )
#define  HAL_LEVEL_ERR_HIGH       ( LEVEL_DETECT_OVF  | HAL_LEVEL_MID     )
#define  HAL_LEVEL_ERR_OVF_LOW    ( LEVEL_DETECT_OVF  | HAL_LEVEL_ERR_LOW )
#define  HAL_LEVEL_OVF            ( LEVEL_DETECT_OVF  | HAL_LEVEL_HIGH    )

#if 0
/* 정수 탱크 수위 */
U8 HAL_GetLevelRoomTank(void);

/* 온수 탱크 수위 */
U8 HAL_GetLevelHotTank(void);

/* 냉수 탱크 수위 */
U8 HAL_GetLevelColdTank(void);

/* 아이스 트레이 수위 */
U8 HAL_GetLevelIceTray(void);
#endif


/* 드레인 탱크 수위 */
U8 HAL_GetLevelDrainTank(void);


#endif /* __HAL_LEVEL_H__ */
