/**
 * File : config.h
 * 
 * Project Config Define
**/
#ifndef __CONFIG_H__
#define __CONFIG_H__

/**
 * Version Infomation
 * 
 * 프로그램 버전 표시
 * 1) 첫번째 자리 : 1 - 양산 적용, 0 - 테스트 버전 (DPP, LPP)
 * 2) 두번째 자리 : 0 - 양산 적용, 1 - DPP, 2 - LPP
 * 3) 네번째 자리 : 버전 표시 0-16
*/
#define PROGRAM_VERSION         "V0.1.0.1"

/**
 * Model Infomation
*/



/**
 * Feature
**/
/* Fota */
#define  CONFIG_FOTA_LIB        (1)  // FOTA 湲곕뒫 ?쒖꽦?? ?쒖? ?쇱씠釉뚮윭由?xxx_f ?ъ슜)

/* Debug */
#define CONFIG_DEBUG_COMM       (0)   // Communication Debug Mode ( 0 : Normal, 1: Debug )

#endif  /* __CONFIG_H__ */
