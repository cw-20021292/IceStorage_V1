#ifndef __HW__H__
#define __HW__H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
#include "r_cg_adc.h"
#include "r_cg_serial.h"
#include "r_cg_macrodriver.h"


/* Watchdog timer reset */
#define	RESET_WDT()					R_WDT_Restart()

/* INPUT */
#define  P_MODEL_CHPI               P12.1 //P2.6
#define  GET_STATUS_MODEL_CHPI()    ( P_MODEL_CHPI )

#define  P_FILTER_COVER             P12.2 //P2.5
#define  GET_STATUS_FILTER()        ( P_FILTER_COVER )

/* UV */
#define  P_UV_ICE                   P13.0
#define  TURN_ON_UV_ICE()           do{ P_UV_ICE = 1; }while(0)
#define  TURN_OFF_UV_ICE()          do{ P_UV_ICE = 0; }while(0)

#define  P_UV_WATER                 P10.2
#define  TURN_ON_UV_WATER()         do{ P_UV_WATER = 1; }while(0)
#define  TURN_OFF_UV_WATER()        do{ P_UV_WATER = 0; }while(0)

#define  P_24V                      P10.6
#define  TURN_ON_24V()              do{ P_24V = 1; }while(0)
#define  TURN_OFF_24V()             do{ P_24V = 0; }while(0)


/* SPI - VOICE IC */
#define  P_SPI_RESET      P3.6
#define  TURN_ON_SPI_RESET()        do{ P_SPI_RESET = 1; }while(0)
#define  TURN_OFF_SPI_RESET()       do{ P_SPI_RESET = 0; }while(0)

#define  P_SPI_CSB        P3.5
#define  TURN_ON_SPI_CSB()          do{ P_SPI_CSB = 1; }while(0)
#define  TURN_OFF_SPI_CSB()         do{ P_SPI_CSB = 0; }while(0)

#define  P_SPI_CLK        P3.4
#define  TURN_ON_SPI_CLK()          do{ P_SPI_CLK = 1; }while(0)
#define  TURN_OFF_SPI_CLK()         do{ P_SPI_CLK = 0; }while(0)

#define  P_SPI_SDI        P3.3
#define  WRITE_SPI_SDI( val )       do{ P_SPI_SDI = val; }while(0)

#define  P_SPI_SEL        P3.2
#define  TURN_ON_SPI_SEL()          do{ P_SPI_SEL = 1; }while(0)
#define  TURN_OFF_SPI_SEL()         do{ P_SPI_SEL = 0; }while(0)


/* KEY */

#define  P_KEY_1                       P4.7     // 설정
#define  P_KEY_2                       P10.3    // 얼음
#define  P_KEY_3                       P10.5    // 온수(빠른제빙)
#define  P_KEY_4                       P15.2    // 정수
#define  P_KEY_5                       P15.3    // 냉수
#define  P_KEY_6                       P15.5    // 터치용량

#define  P_KEY_7                       P12.4    // 얼음off
#define  P_KEY_8                       P4.6     // 냉수 off
#define  P_KEY_9                       P10.4    // 점검 모드
#define  P_KEY_10                      P15.1    // 온수잠금
#define  P_KEY_11                      P15.4    // 얼음잠금
#define  P_KEY_12                      P15.6    // 취침

#define  P_KEY_19                       P13.7    // WATER OUT 
#define  P_KEY_20                       P12.3    // ICE OUT 


/* LED */
#define  P_LED_11                       P8.5
#define  P_LED_12                       P8.4
#define  P_LED_13                       P8.3
#define  P_LED_14                       P8.2
#define  P_LED_15                       P8.1
#define  P_LED_16                       P8.0
#define  P_LED_17                       P0.5

#define  P_LED_21                       P0.6
#define  P_LED_22                       P7.0
#define  P_LED_23                       P7.1
#define  P_LED_24                       P7.2
#define  P_LED_25                       P7.3
#define  P_LED_26                       P7.4
#define  P_LED_27                       P7.5
                       
#define  P_LED_31                       P7.6
#define  P_LED_32                       P7.7
#define  P_LED_33                       P6.7
#define  P_LED_34                       P6.6
#define  P_LED_35                       P6.5
#define  P_LED_36                       P6.4
#define  P_LED_37                       P3.1
                                        
#define  P_LED_41                       P12.6
#define  P_LED_42                       P12.7
#define  P_LED_43                       P4.1
#define  P_LED_44                       P4.2           
#define  P_LED_45                       P4.3            
#define  P_LED_46                       P4.4
#define  P_LED_47                       P4.5           

#define  P_LED_51                       P3.7
#define  P_LED_52                       P12.0
#define  P_LED_53                       P14.0
#define  P_LED_54                       P14.1
#define  P_LED_55                       P14.2
#define  P_LED_56                       P14.3
#define  P_LED_57                       P14.4

#define  P_LED_61                       P14.5
#define  P_LED_62                       P0.0
#define  P_LED_63                       P0.1
#define  P_LED_64                       P0.2
#define  P_LED_65                       P0.3
#define  P_LED_66                       P0.4
#define  P_LED_67                       P0.7

#define  P_LED_71                       P5.3
#define  P_LED_72                       P5.2
#define  P_LED_73                       P5.1
#define  P_LED_74                       P5.0
#define  P_LED_75                       P3.0
#define  P_LED_76                       P8.7
#define  P_LED_77                       P8.6

#define  P_LED_81                       P1.5
#define  P_LED_82                       P1.6
#define  P_LED_83                       P1.7
#define  P_LED_84                       P5.7
#define  P_LED_85                       P5.6
#define  P_LED_86                       P5.5
#define  P_LED_87                       P5.4

#define  P_LED_91                       P9.3
#define  P_LED_92                       P9.2
#define  P_LED_93                       P9.1
#define  P_LED_94                       P9.0
#define  P_LED_95                       P1.0
#define  P_LED_96                       P1.1
#define  P_LED_97                       P1.2

#define  P_LED_101                      P11.4
#define  P_LED_102                      P11.3
#define  P_LED_103                      P11.2
#define  P_LED_104                      P9.7
#define  P_LED_105                      P9.6
#define  P_LED_106                      P9.5
#define  P_LED_107                      P9.4

#define  P_LED_111                      P14.6
#define  P_LED_112                      P11.1 
#define  P_LED_113                      P11.0 
#define  P_LED_114                      P10.1 
#define  P_LED_115                      P11.7 
#define  P_LED_116                      P11.6 
#define  P_LED_117                      P11.5

#define  P_LED_121                      P6.2
#define  P_LED_122                      P6.3
//#define  P_LED_123                      P4.1
//#define  P_LED_124                      P12.7
//#define  P_LED_125                      P12.6
//#define  P_LED_126                      P12.5
//#define  P_LED_127                      P4.5

#endif /* __HW__H__ */

