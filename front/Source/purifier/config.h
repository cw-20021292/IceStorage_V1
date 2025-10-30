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
 * ���α׷� ���� ǥ��
 * 1) ù��° �ڸ� : 1 - ��� ����, 0 - �׽�Ʈ ���� (DPP, LPP)
 * 2) �ι�° �ڸ� : 0 - ��� ����, 1 - DPP, 2 - LPP
 * 3) �׹�° �ڸ� : ���� ǥ�� 0-16
*/
#define PROGRAM_VERSION         "V0.1.0.1"

/**
 * Model Infomation
*/



/**
 * Feature
**/
/* Fota */
#define  CONFIG_FOTA_LIB        (1)  // FOTA 기능 ?�성?? ?��? ?�이브러�?xxx_f ?�용)

/* Debug */
#define CONFIG_DEBUG_COMM       (0)   // Communication Debug Mode ( 0 : Normal, 1: Debug )

#endif  /* __CONFIG_H__ */
