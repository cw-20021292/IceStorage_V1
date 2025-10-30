#ifndef __CONFIG_H__
#define __CONFIG_H__


/* TEST
 *  -. ����� ���ӽð� �ʱ�ȭ ��� Ȱ��ȭ
 *  -. ���� Ż�� ��� ��� Ȱ��ȭ
 */
#define  CONFIG_FOTA_LIB            1  // FOTA ��� Ȱ��ȭ( ǥ�� ���̺귯�� xxx_f ���)
#define  CONFIG_TEST                0  // �õ�, Skip �����, ����Ż�� ��� Ȱ��ȭ

#define  CONFIG_TEST_8585           0  // �ŷڼ� ������ �׽�Ʈ  
#define  CONFIG_TEST_8585_ICE       0  // ������ - ���α׷� - ���� ���� ���
#define  CONFIG_TEST_8585_FILTER    0  // ������ - ���� 

#define  CONFIG_TEST_HOT_WATER      0  // �ŷڼ�, �¼� ���� ���� �׽�Ʈ ���α׷� 
#define  CONFIG_TEST_GAS_SWITCH     0  // �ø� ��ȯ ��� �̵� �ݺ� �׽�Ʈ 
#define  CONFIG_TEST_LED            0  // LED LIGHTING TEST ( �ְ�/�߰� ��� )
#define  CONFIG_TEST_TEMP_READ      0  // �µ� ���� ��� �б� ��� ( �׽�Ʈ ��� )
#define  CONFIG_TEST_FLOW_METER     0  // FLOW METER AUTO COUNTING
#define  CONFIG_TEST_HEATER         0  // �¼� ���� OFF
#define  CONFIG_GAS_FILLING         0  // �ø� ä���
#define  CONFIG_UV_MANUAL           0  // UV ���� �׽�Ʈ
#define  CONFIG_TEST_HOT_DRAIN      0  // �¼� ��� ��� �׽�Ʈ �ڵ�
#define  CONFIG_TEST_FILTER         0  // ���� ���� �׽�Ʈ 
#define  CONFIG_TANK_REED           0  // ��ũ ���� ����ġ  ( 1: 1��, 0: 2�� )
#define  CONFIG_TEST_OUT_CONTY      0  // ���� ����.. 2�ð� ����
#define  CONFIG_TEST_HOT_OUT        0  // �¼� ���� �µ� ǥ��
#define  CONFIG_TEST_PREHEAT        0  // �¼� ���� �µ� ǥ��


#define  CONFIG_COMP_ERROR_CODE     0  // ����� ���� �ڵ� ǥ�� 50~
#define  CONFIG_COMP_ERROR_ONESHOT  0  // ����� ���� �ڵ� ǥ�� 50~

// �ü� ���� ����ġ/����ġ
#define  CONFIG_TEST_COLD_TEMP_UP      0    // ����ġ      
#define  CONFIG_TEST_COLD_TEMP_DOWN    0    // ����ġ

#define  CONFIG_TEST_AMBI_TEMP_UP      0    // ����ġ      
#define  CONFIG_TEST_AMBI_TEMP_DOWN    0    // ����ġ

/* CHP or CP */
#define CONFIG_SYSTEM_CHP      0
#define CONFIG_SYSTEM_CP       1
#define CONFIG_SYSTEM_AUTO     2
#define CONFIG_SYSTEM_TYPE     CONFIG_SYSTEM_AUTO  // 0: CHP, 1:CP

/* FRONT JIG - WOORIM, FRONT PBA */
#define  CONFIG_JIG_FRONT_UNUSED_UV     0  // FORNT JIG UV2�� �˻�
#define  CONFIG_JIG_RBK_FRONT           0  // FRONT JIG PROGRAM( FOR RBK )
#define  CONFIG_JIG_GAS_SWITCH          0  // GAS SIWTCH JIG 

/* ��� ��� ����/���� */
#define  CONFIG_STER                0   /* 0: ����, 1: ���� */

/* RTC �׽�Ʈ - 1sec */
#define  CONFIG_RTC_TEST            0   /* 0: ����, 1: ���� */

/* Ż�� �� ������ũ�� ������ ���� �ϱ� ���� �������� �Ǵ����� ��ȸ�� */
#define CONFIG_BACK_FEEDER                1   // �Ǵ����� ��ȸ��

/* ���� �켱 ��忡�� Ż�� �� �ð� ��� Ȱ��ȭ */
#define  CONFIG_ENABLE_DEICING_COOLING    0  /* 0: ����, 1: ���� */

/* ���� ���� ��� ����/���� */
#define  CONFIG_ERR_ALL             1   /* 0: ����, 1: ���� */

/* ��� �ۼ��� ����͸� ��� ����/���� */
#define  DEBUG_COMM                 0  /* 0: ����, 1:���� */


// ���� ���� ���� ī��Ʈ �ʱ�ȭ�� ���� ������ �ɼ�
#define TANK_RO     0
#define TANK_NT     1
#define CONFIG_TANK_TYPE  TANK_NT

/* ���� ǥ�� */
#define VERSION_MAJOR   17
#define VERSION_MINOR   0       

/* ���� �ڵ� */
#define COUNTRY_CODE_KOR    0

/* WiFi Hot Temp Config */
#define CONFIG_WIFI_HEAT_TEMP		1

/* WiFi Water Amount Config */
#define CONFIG_WIFI_WATER_AMOUNT	1

#endif /* __CONFIG_H__ */

