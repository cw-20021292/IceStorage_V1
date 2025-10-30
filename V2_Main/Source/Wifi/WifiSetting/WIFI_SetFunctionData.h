
#ifndef _WIFI_SetFunctionData_H_
#define _WIFI_SetFunctionData_H_

typedef enum
{
    WIFI_FUNC_0001_POWER_SEL = 0x0001,
    WIFI_FUNC_0002_COLD_SEL,    // $)C3C<v<3A$
    WIFI_FUNC_0003_HOT_LOCK_SEL,// $)C?B<v@a1]<3A$
    WIFI_FUNC_0004_SAVE_SEL,    // $)C@}@|<3A$
    WIFI_FUNC_0005_LOCK_SEL,    // $)C@a1]<3A$
    WIFI_FUNC_0006_MUTE_SEL,    // $)C9+@=<3A$
    WIFI_FUNC_0007_STERILIZE_SEL,   // $)C;l1U<3A$
    WIFI_FUNC_0008_ICE_SEL,     // $)CA&:y<3A$
    WIFI_FUNC_0009_HOT_TEMP,    // $)C?B<v?B55G%=C
    WIFI_FUNC_000A_COLD_TEMP,   // $)C3C<v?B55G%=C
    WIFI_FUNC_000B_WATER_SEL,   // $)C90 <1EC
    WIFI_FUNC_000C_USER_SEL,    // $)C;g?k@Z
    WIFI_FUNC_000D_WATER_OUTQUANTITY,// $)CC_Cb907.
    WIFI_FUNC_000E_TIME_YEAR,       // $)CGv@g=C0# ?,
    WIFI_FUNC_000F_TIME_MONTHDAY,   // $)CGv@g=C0# ?y@O
    WIFI_FUNC_0010_TIME_MINSEC,     // $)CGv@g=C0# =C:P
    WIFI_FUNC_0011_STERILIZE_CYCLE, // $)C;l1UAV1b
    WIFI_FUNC_0012_STERILIZE_MODE,  // $)C;l1UAV1b8p5e
    WIFI_FUNC_0013_STERILIZE_TIME,  // $)C;l1U=C0#<3A$
    WIFI_FUNC_0014_STERILIZE_STATUS, // $)C;l1U5?@[;sEB
    WIFI_FUNC_0015_TDS_IN,  // $)C@T<v TDS
    WIFI_FUNC_0016_TDS_OUT, // $)CCb<v TDS
    WIFI_FUNC_0017_WATER_PRESSURE,  // $)CFs <v>P
    WIFI_FUNC_0018_TDS_INTEMP,  // TDS $)C@T<v?B55
    WIFI_FUNC_0019_TDS_OUTTEMP, // TDS $)CCb<v?B55
    WIFI_FUNC_001A_HOT_INTEMP,  // $)C<x0#?B<v ?x<v?B55
    WIFI_FUNC_001B_HOT_MEANTEMP,// $)C<x0#?B<v Fr1U?B55
    WIFI_FUNC_001C_HOT_SELTEMP, // $)C<x0#?B<v <3A$?B55
    WIFI_FUNC_001D_WATER_INQUANTITY,// $)C@T<v@/7.
    WIFI_FUNC_001E_AUTODRAIN_SEL,   // $)C@Z5?9h<v <3A$
    WIFI_FUNC_001F_DRAIN_STATUS,     // $)C9h<v5?@[
    WIFI_FUNC_0020_WATER_REQUANTITY,// $)C@g A$<v @/7.
    WIFI_FUNC_0021_HEART_TIME1, // $)CGOF.<-:q=:=C0#
    WIFI_FUNC_0022_HEART_TIME2, //
    WIFI_FUNC_0023_HEART_TIME3, //
    WIFI_FUNC_0024_HEART_TIME4, //
    WIFI_FUNC_0025_FILTER_NOTICE1,  // $)CGJEM13H/>K82
    WIFI_FUNC_0026_FILTER_NOTICE2,
    WIFI_FUNC_0027_FILTER_NOTICE3,
    WIFI_FUNC_0028_FILTER_NOTICE4,
    WIFI_FUNC_0029_SERVICE_TIME,    // $)C<-:q=: 0f0z@O
    WIFI_FUNC_002A_REMOVE_STATE,    // $)CA&0E@2 ;sEB
    WIFI_FUNC_002B_SAVE_DETECT,     // $)C@}@|Ax@TX
    WIFI_FUNC_002C_SILVER_CARE,     // $)C=G9vDI>n
    WIFI_FUNC_002D_DRAIN_QUANTITY,  // $)C9h<v907.
    WIFI_FUNC_002E_STERILIZE_QUANTITY,  // $)C;l1U907.
    WIFI_FUNC_002F_SPARKLING_SEL,   // $)CE:;j<v <1EC
    WIFI_FUNC_0030_ICE_MAKETIME,    // $)C>s@=C_Cb=C0#
    WIFI_FUNC_0031_VOICE_SEL,   // $)C@=<:>H3;<3A$
    WIFI_FUNC_0032_ICELOCK_SEL, // $)C>s@=@a1] <3A$
    WIFI_FUNC_0033_VOICE_VOL_SEL,    // $)C@=<::<7} <3A$
    WIFI_FUNC_0034_LIGHT_SEL,   // $)CA68m<3A$
    WIFI_FUNC_0035_COVER1_OPEN, // $)CD?9v1 ?-82
    WIFI_FUNC_0036_COVER2_OPEN, // $)CD?9v2 ?-82
    WIFI_FUNC_0037_SLEEP_SEL,  // $)C==838p5e <1EC
    WIFI_FUNC_0038_MASH_CLEAN,  // $)C8E=,8A C;<R>K6w
    WIFI_FUNC_0039_HOT_SEL,     // $)C?B<v<3A$
    WIFI_FUNC_003A_FREEZE_BURST,
    WIFI_FUNC_003B_HOT_TEMP_SEL,    // $)C3*@G ?B<v?B55 <3A$
    WIFI_FUNC_003C_COLD_STRENGTH_SEL,   // $)C3C<v 0->`A6@}
    WIFI_FUNC_003D_HEIGHT_SEL,          // $)C0m558p5e
    WIFI_FUNC_003E_WATER_COURSE_STATUS, // $)C@/7N8p5b @eBx;sEB
    WIFI_FUNC_003F_WATER_COURSE_RESET,  // $)C@/7N8p5b 8.<B
    WIFI_FUNC_0040_SMART_CHECK_RQST,    // $)C=:86F. 0m@eAx4\ <3A$
    WIFI_FUNC_0041_SMART_CHECK_STATUS,  // $)C=:86F. 0m@eAx4\ ;sEB
    WIFI_FUNC_0042_SMART_CHECK_PROGRESS,// $)C=:86F. 0m@eAx4\ AxG`7|
    WIFI_FUNC_0043_FILTER_STATUS1,      // $)CGJEM @eBx;sEB
    WIFI_FUNC_0044_FILTER_STATUS2,
    WIFI_FUNC_0045_FILTER_STATUS3,
    WIFI_FUNC_0046_FILTER_STATUS4,
    WIFI_FUNC_0047_WATER_QUANTITY_SEL,  // $)C907. <1EC
    WIFI_FUNC_0048_HOT_TEMP_SEL,
    WIFI_FUNC_0049_OUT_BUTTON_LIGHT,
    WIFI_FUNC_004A_WELCOME_LIGHT,
    WIFI_FUNC_004B_HIDDEN_LCD_SEL,
    WIFI_FUNC_004C_FAUCET_MOVE_SEL,
    WIFI_FUNC_004D_FAUCET_MOVE_STATUS,
    WIFI_FUNC_004E_CUP_SENSING_SEL,
    WIFI_FUNC_004F_SOUND_VOL_SEL,
    WIFI_FUNC_0050_UV_STERILIZE_STATUS,
    WIFI_FUNC_0051_QUANTITY_USE_SEL,
    WIFI_FUNC_0052_VOICE_MODE_SEL,
    WIFI_FUNC_0053_FILTER_RESET1,
    WIFI_FUNC_0054_FILTER_RESET2,
    WIFI_FUNC_0055_FILTER_RESET3,
    WIFI_FUNC_0056_FILTER_RESET4,
    WIFI_FUNC_0057_CUP_SENSING_SENSITIVITY,
    WIFI_FUNC_0058_ICE_SIZE_SEL,
    WIFI_FUNC_0059_ICE_ONOFF_SEL,
    WIFI_FUNC_005A_UV_ICE_TANK_STATUS,
    WIFI_FUNC_005B_UV_ICE_TRAY_STATUS,
    WIFI_FUNC_005C_SMART_CHECK_RESULT,
    WIFI_FUNC_005D_UV_ICE_FAUCET_STATUS,
    WIFI_FUNC_005E_SLEEP_MODE_SEL,
    WIFI_FUNC_005F_SLEEP_START_HOUR,
    WIFI_FUNC_0060_SLEEP_START_MIN,
    WIFI_FUNC_0061_SLEEP_END_HOUR,
    WIFI_FUNC_0062_SLEEP_END_MIN,
    WIFI_FUNC_0063_DEFAULT_QUANTITY_ML_SEL,		//	�⺻ �뷮 ����
    WIFI_FUNC_0064_STERILIZE_EXPECTED_TIME,		//	EW ��� �ҿ� ���� �ð�
    WIFI_FUNC_0065_DEFAULT_TEMP_SEL,			//	�⺻ �¼� �µ� ���� ���� 
    WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL,		//	�⺻ ���� ���� ���� ���� 
    WIFI_FUNC_0067_WATER_QUANTITY_USE_SEL0,		//	4oz ���� ��� ���� ���� 
    WIFI_FUNC_0068_WATER_QUANTITY_USE_SEL1,		//	6oz ���� ��� ���� ���� 
    WIFI_FUNC_0069_WATER_QUANTITY_USE_SEL2, 	//	8oz ���� ��� ���� ���� 
    WIFI_FUNC_006A_WATER_QUANTITY_USE_SEL3, 	//	10oz ���� ��� ���� ���� 
    WIFI_FUNC_006B_WATER_QUANTITY_USE_SEL4, 	//	12oz ���� ��� ���� ���� 
    WIFI_FUNC_006C_WATER_QUANTITY_USE_SEL5, 	//	14oz ���� ��� ���� ���� 
    WIFI_FUNC_006D_WATER_QUANTITY_USE_SEL6, 	//	16oz ���� ��� ���� ���� 
    WIFI_FUNC_006E_WATER_QUANTITY_USE_SEL7, 	//	20oz ���� ��� ���� ���� 
    WIFI_FUNC_006F_WATER_QUANTITY_USE_SEL8, 	//	32oz ���� ��� ���� ���� 
    WIFI_FUNC_0070_WATER_QUANTITY_USE_SEL9,		//	���� ���� ��� ���� ���� 
    WIFI_FUNC_0071_WATER_QUANTITY_USE_SEL10,	//	reserve2 ���� ��� ���� ����
    WIFI_FUNC_0072_WATER_QUANTITY_SORT_SEL,		//	���� ���� ���� ���� 
    WIFI_FUNC_0073_WATER_TEMP_SORT_SEL,			//	�¼� �µ� ���� ���� ���� 
    WIFI_FUNC_0074_UNUSE_POWER_SAVING_SEL,		//	�̻�� ���� ����
    WIFI_FUNC_0075_TEMP_UNIT_SEL,				//	�µ� ���� ǥ�� ���� ���� 
    WIFI_FUNC_0076_WATER_QUANTITY_UNIT_SEL,		//	���� ���� ���� ǥ�� ���� ���� 
    WIFI_FUNC_0077_HOT_TARGET_TEMP_TIME,        // 	�¼� ��ǥ �µ� ���� �ð�
    WIFI_FUNC_0078_COLD_TARGET_TEMP_TIME,       // 	�ü� ��ǥ �µ� ���� �ð�
    WIFI_FUNC_0079_MIXED_WATER_AVE_TEMP,		//	ȥ�� ���� ��� �µ�
    WIFI_FUNC_007A_MIXED_WATER_MIN_TEMP,		//	ȥ�� ���� �ּ� �µ�
    WIFI_FUNC_007B_MIXED_WATER_MAX_TEMP,		//	ȥ�� ���� �ִ� �µ�
    WIFI_FUNC_007C_COLDTANK_LOW_WATERLEVEL,		//	�ü� ��ũ ������ ����
    WIFI_FUNC_007D_COLDTANK_HIGH_WATERLEVEL,	//	�ü� ��ũ ������ ����
    WIFI_FUNC_007E_HIGH_TEMP_STL_SEL,			//	��¼� ��� ����
    WIFI_FUNC_007F_DEFAULT_TEMP_SET,            //  �⺻�¼��µ�����(�µ���)

    WIFI_FUNC_0080_HIGH_TEMP_STL_TIME,			//	��¼� ��� ���� �ð�
    WIFI_FUNC_0081_HIGH_TEMP_STL_TIME_M,		//	��¼� ��� ���� �ð� (��)
    WIFI_FUNC_0082_UNUSE_POW_SAVING_STATUS,		//	�̻�� ���� ���� ����
    WIFI_FUNC_0083_POWER_SAVING_STATUS,			//	���� ���� ����
    WIFI_FUNC_0084_HOT_RESTARTING_TEMP,			//	�¼� ��⵿ �µ�
    WIFI_FUNC_0085_HOT_STOP_TEMP,				//	�¼� ���� �µ�
    WIFI_FUNC_0086_COLD_RESTARTING_TEMP,		//	�ü� ��⵿ �µ�
    WIFI_FUNC_0087_COLD_STOP_TEMP,				//	�ü� ���� �µ�
    WIFI_FUNC_0088_COLD_ADDITIONAL_TIME,    	//	�ü� �ð� �߰� �ð�
    WIFI_FUNC_0089_BOOST_PUMP_STATUS,			//	�ν�Ʈ ���� ���� ����
    WIFI_FUNC_008A_COLDTANK_UV_STL_STATUS,		//	�ü� ��ũ ��� ���� ���� (UV)
    WIFI_FUNC_008B_HEATER_STATUS,				//	���� ���� ���� (������)
    WIFI_FUNC_008C_COMP_STATUS,					//	COMP ���� ����
    WIFI_FUNC_008D_FILTER_WATER_USAGE,			//	���� �� ��뷮
    WIFI_FUNC_008E_FILTER_REMAINING_DAY,		//	���� �ܿ� ����� (�ð�)
    WIFI_FUNC_008F_ICE_FULL_STATUS,				//	���� ���� ����

    WIFI_FUNC_0090_ICE_TRAY_POSITION_STATUS,	//	���� Ʈ���� ��ġ ����
    WIFI_FUNC_0091_REFRIGERANT_SWC_STATUS,		//	�ø� ��ȯ ��� ����
    WIFI_FUNC_0092_DEICING_STAUTS,				//	Ż�� ��� ����
    WIFI_FUNC_0093_ICEMAKING_STAUTS,			//	���� ���� ����
    WIFI_FUNC_0094_DUMMY_DEICING_STAUTS,		//	���� Ż�� ���� ����
    WIFI_FUNC_0095_ICEMAKING_COMPLET_TIME,  	// 	���� �Ϸ� �ð�
    WIFI_FUNC_0096_DEICING_COMPLET_TIME,    	// 	Ż�� �Ϸ� �ð�
    WIFI_FUNC_0097_TARY_WATER_COMPLET_TIME, 	// 	Ʈ���� �Լ� �Ϸ� �ð�
    WIFI_FUNC_0098_TARY_WATER_QUANTITY,     	//	Ʈ���� �Լ� ����
    WIFI_FUNC_0099_COMP_TARGET_RPS,				//	����� ��ǥ RPS
    WIFI_FUNC_009A_COMP_CURRENT_RPS,			//	����� ���� RPS
    WIFI_FUNC_009B_DRAINTANK_LOW_WLEVEL,		//	�巹�� ��ũ ������ ���� ����
    WIFI_FUNC_009C_DRAINTANK_HIGH_WLEVEL,		//	�巹�� ��ũ ������ ���� ����
    WIFI_FUNC_009D_DRAINTANK_DRAIN_STAUTS,		//	�巹�� ��� ���� ����
    WIFI_FUNC_009E_COLD_EVA_TEMP,				//	�� ���� ���߱� �µ� ����
    WIFI_FUNC_009F_ICE_EVA_TEMP,				//	���� ���߱� �µ� ����

    WIFI_FUNC_00A0_COLD_TANK_TEMP,				//	�ü� ��ũ �µ�
    WIFI_FUNC_00A1_HOT_TANK_TEMP,				//	�¼� ��ũ �µ�
    WIFI_FUNC_00A2_CHILD_LOCK,					//	��ü ��ư �ڵ� ��� 
    WIFI_FUNC_00A3_FLOWPATH_EMPTY,				//	���� ��� ����
    WIFI_FUNC_00A4_HOTTANK_LOW_W_LEVEL,			//	�¼� ��ũ ������ ����
    WIFI_FUNC_00A5_HOTTANK_HIGH_W_LEVEL,		//	�¼� ��ũ ������ ����
    WIFI_FUNC_00A6_PURETANK_LOW_W_LEVEL,		//	���� ��ũ ������ ����
    WIFI_FUNC_00A7_PURETANK_HIGH_W_LEVEL,		//	���� ��ũ ������ ����
	WIFI_FUNC_00A8_COLDTANK_MID_W_LEVEL,		//	�ü� ��ũ �߼��� ����
	WIFI_FUNC_00A9_UNUSED_TIME,					//	��ǰ �̻�� �ð�
	WIFI_FUNC_00AA_SMART_SAVING_SET_TIME,		//	����Ʈ ���� �̻�� �����ð�
	WIFI_FUNC_00AB_SAMRT_SAVING_STATE,			//	����Ʈ ���� ����
	WIFI_FUNC_00AC_FILTER1_WATER_USAGE,			//	����1 ���� �� ��뷮 
	WIFI_FUNC_00AD_FILTER1_UASGE_DAY,			//	����1 �ܿ� ��� �ð� 
	WIFI_FUNC_00AE_FILTER2_WATER_USAGE,			//	����2 ���� �� ��뷮
	WIFI_FUNC_00AF_FILTER2_UASGE_DAY,			//	����2 �ܿ� ��� �ð�
	WIFI_FUNC_00B0_FILTER3_WATER_USAGE,			//	����3 ���� �� ��뷮
	WIFI_FUNC_00B1_FILTER3_UASGE_DAY,			//	����3 �ܿ� ��� �ð�
	WIFI_FUNC_00B2_FILTER_NOTICE1,				//	���� ��ü �˸� 
	WIFI_FUNC_00B3_FILTER_NOTICE2,				//	���� ��ü �˸� 
	WIFI_FUNC_00B4_FILTER_NOTICE3,				//	���� ��ü �˸� 
    WIFI_FUNC_00B5_FILTER_FLUSHING,             //  ���� �÷��� ����
    WIFI_FUNC_00B6_VOICE_LANGUAGE,              //  ��������
    WIFI_FUNC_00B7_FILTER_SEQ,                  //  ���� ��ü ����
    // WIFI_FUNC_00B8_DEFAULT_TEMP_SET,        //  �⺻ �¼��µ� ���� (�µ������� ����)
    WIFI_FUNC_00B8_FILTER1_DDAY,                //  ����1 ��ü ������
    WIFI_FUNC_00B9_FILTER2_DDAY,                //  ����2 ��ü ������
    WIFI_FUNC_00BA_FILTER3_DDAY,                //  ����3 ��ü ������
    WIFI_FUNC_00BB_FLOWPATH_STL_STATUS,         //  ���� ��¼� ��ջ���
    WIFI_FUNC_00BC_FAUCE_STL_STATUS,            //  �Ŀ�� ��¼� ��ջ���
    WIFI_FUNC_00BD_ICETANK_STL_STATUS,          //  ������ũ ��¼� ��ջ���
    WIFI_FUNC_00BE_QUANTITY_HALFCUP,            //  ���� �뷮
    WIFI_FUNC_00BF_QUANTITY_ONECUP,             //  ���� �뷮

    WIFI_FUNC_00C0_QUANTITY_TWOCUPS,            //  ���� �뷮
    WIFI_FUNC_00C1_HOTTEMP_USE_SEL0,            //  �¼��µ� ��� ���� 0 (45)
    WIFI_FUNC_00C2_HOTTEMP_USE_SEL1,            //  �¼��µ� ��� ���� 1 (50)
    WIFI_FUNC_00C3_HOTTEMP_USE_SEL2,            //  �¼��µ� ��� ���� 2 (55)
    WIFI_FUNC_00C4_HOTTEMP_USE_SEL3,            //  �¼��µ� ��� ���� 3 (60)
    WIFI_FUNC_00C5_HOTTEMP_USE_SEL4,            //  �¼��µ� ��� ���� 4 (65)
    WIFI_FUNC_00C6_HOTTEMP_USE_SEL5,            //  �¼��µ� ��� ���� 5 (70)
    WIFI_FUNC_00C7_HOTTEMP_USE_SEL6,            //  �¼��µ� ��� ���� 6 (75)
    WIFI_FUNC_00C8_HOTTEMP_USE_SEL7,            //  �¼��µ� ��� ���� 7 (80)
    WIFI_FUNC_00C9_HOTTEMP_USE_SEL8,            //  �¼��µ� ��� ���� 8 (85)
    WIFI_FUNC_00CA_HOTTEMP_USE_SEL9,            //  �¼��µ� ��� ���� 9 (90)
    WIFI_FUNC_00CB_WATER_EXTRACT_STATUS,        //  �� �������
    WIFI_FUNC_00CC_ICE_EXTRACT_STATUS,          //  ���� �������
    WIFI_FUNC_00CD_SETUP_MODE_STATUS,           //  ��ǰ ���� ����
    WIFI_FUNC_00CE_HOTTEMP_USE_SEL10,           //  �¼��µ� ��� ���� 10 (95)
    WIFI_FUNC_00CF_HOTTEMP_USE_SEL11,           //  �¼��µ� ��� ���� 11 (100)




    WIFI_FUNC_NUM
}E_API_A1010_T;

void* GetWifiFuncEventList ( void );
U16 GetWifiFuncListSize ( void );

U8 IsExceptId ( U16 mId );
U8 IsExceptHotTempUseID ( U16 mId );
U8 IsExceptSleepTimeID ( U16 mId );
U8 IsExceptWaterAmountID ( U16 mId );
U8 IsExceptWaterAmountID_List ( U16 mId );



#endif


