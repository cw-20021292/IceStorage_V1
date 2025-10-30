
#include "hw.h"

#include <string.h>
#include "prj_type.h"
#include "buzzer.h"

#define D_BUZZER_END 0xFFFF

/* Variable *******************************************************************/
typedef struct _buzzer_
{
    U16 Power; // ON/OFF
    U16 TDR;   // TDR PWM
    U16 Time;  // 출력 시간
    U8 Order;  // 출력 순서
    U8 Output; // 출력음 선택
    U8 Silent; // 무음모드
    U8 CriticalSection;
} Buzzer_T;

Buzzer_T Buzzer;

typedef struct _buzzer_list_
{
    E_BuzzId_T Id;
    U16 *Type;
    U8 Size;
    U8 SilentSet;
} Buzz_list_t;

/* Buzzer List ****************************************************************/
// 0 : OFF
const static U16 au16Buzz_Off[3][1]= 
{
    {0},
    {0},
    {D_BUZZER_END}
};
// 1 : 전원 ON
const static U16 au16Buzz_PowerOn[3][11]=
{
    {ON,  OFF, ON, OFF,  ON,   OFF,  ON,   OFF,  ON,  OFF, OFF},
    {Si5, Si5, Re6, Re6, Sol6, Sol6, Pas6, Pas6, Re7, Re7, 0},
    {4,   19,  4,   19,  4,    19,   5,    65,   5,   112, D_BUZZER_END}
};

// 2 : 추출시작
const static U16 au16Buzz_EffStart[3][5]=
{
    {ON, OFF, ON,  OFF, OFF},
    {Re6,Re6, Re7, Re7, 0},
    {3,  21,  3,   21,  D_BUZZER_END}
};
// 3 : 추출종료
const static U16 au16Buzz_EffStop[3][7]=
{
    {ON,  OFF, ON,   OFF,  ON,  OFF, OFF},
    {La5, La5, Pas6, Pas6, Re6, Re6, 0},
    {3,   21,  3,    21,   3,   30,  D_BUZZER_END}
};
// 4 : 냉각 ON
const static U16 au16Buzz_CoolOn[3][5]=
{
    {ON,   OFF,  ON,  OFF, OFF},
    {Pas6, Pas6, Re7, Re7, 0},
    {5,    19,   10,  38,  D_BUZZER_END}
};
// 5 : 냉각 OFF
const static U16 au16Buzz_CoolOff[3][7]=
{
    {ON,    OFF,   ON,  OFF, ON,  OFF, OFF},
    {Sols5, Sols5, Si5, Si5, Mi5, Mi5, 0},
    {2,     21,    2,   21,  10,  38,  D_BUZZER_END}
};
// 선택 / 페어링 / 메멘토1
const static U16 au16Buzz_Select[3][3]=
{
    {ON,  OFF, OFF},
    {La6, La6, 0},
    {8,   28,  D_BUZZER_END}
};
// 에러(불가)
const static U16 au16Buzz_Error[3][9]=
{
    {ON,  OFF, ON,  OFF, ON,  OFF, ON,  OFF, OFF},
    {Re6, Re6, Re7, Re7, Re6, Re6, Re7, Re7, 0},
    {2,   10,  8,   28,  2,   10,  8,   28,  D_BUZZER_END}
};
// 기능설정
const static U16 au16Buzz_FuncSet[3][5]=
{
    {ON,   OFF,  ON,  OFF, OFF},
    {Sol6, Sol6, Re7, Re7, 0},
    {5,    19,   10,  38,  D_BUZZER_END}
};
// 기능해제
const static U16 au16Buzz_FuncClr[3][5]=
{
    {ON,  OFF, ON,   OFF,  OFF},
    {Re7, Re7, Sol6, Sol6, 0},
    {5,   19,  10,   38,   D_BUZZER_END}
};
// 10 : 연속추출
const static U16 au16Buzz_EffConti[3][17]=
{
    {ON,  OFF, ON,  OFF, ON,   OFF,  ON,  OFF, ON,  OFF, ON,  OFF, ON,  OFF, ON,  OFF, OFF},
    {Re6, Re6, Re7, Re7, Dos7, Dos7, La6, La6, Mi6, Mi6, La5, La5, Re6, Re6, La6, La6, 0},
    {8,   20,  8,   20,  8,    20,   8,   20,  8,   20,  8,   20,  8,   20,  8,   20,  D_BUZZER_END}
};
// 살균시작
const static U16 au16Buzz_SterilizeStart[3][7]=
{
    {ON,  OFF,  ON,   OFF,  ON,   OFF,  OFF},
    {Mi6, Mi6,  Dos6, Dos6, La6,  La6,  0},
    {3,   21,  3,     21,   10,   38,   D_BUZZER_END}
};
// 살균종료
const static U16 au16Buzz_SterilizeEnd[3][11]=
{
    {ON,  OFF,  ON,   OFF,  ON,   OFF,  ON,   OFF,  ON,   OFF,  OFF},
    {La6, La6,  Dos6, Dos6, Dos6, Dos6, Pas6, Pas6, Mi6,  Mi6,  0},
    {8,   28,   8,    28,   3,    21,   10,   38,   14,   50,   D_BUZZER_END}
};
// 메멘토2
const static U16 au16Buzz_Memento2[3][5]=
{
    {ON,  OFF,    ON,  OFF, OFF},
    {La6, La6,  La6, La6, 0},
    {8,   28,   8,   28,  D_BUZZER_END}
};
// 메멘토3
const static U16 au16Buzz_Memento3[3][7]=
{
    {ON, OFF, ON,  OFF, ON,  OFF, OFF},
    {La6, La6,  La6, La6, La6, La6, 0},
    {8,   28,   8,   28,  8,   28,  D_BUZZER_END}
};
// 메멘토4
const static U16 au16Buzz_Memento4[3][9]=
{
    {ON, OFF, ON,  OFF, ON,  OFF, ON,  OFF, OFF},
    {La6, La6,  La6, La6, La6, La6, La6, La6, 0},
    {8,   28,   8,   28,  8,   28,  8,   28,  D_BUZZER_END}
};
// 메멘토5
const static U16 au16Buzz_Memento5[3][11]=
{
    {ON, OFF,   ON,  OFF, ON,  OFF, ON,  OFF, ON,  OFF, OFF},
    {La6, La6,  La6, La6, La6, La6, La6, La6, La6, La6, 0},
    {8,   28,   8,   28,  8,   28,  8,   28,  8,   28,  D_BUZZER_END}
};
// WIFI 서버연결
const static U16 au16Buzz_ServerConnect[3][7]=
{
    {ON, OFF, ON,  OFF, ON,  OFF,  OFF},
    {Do7, Do7, Re7, Re7, Mi7, Mi7,  0},
    {3,   10,  3,   10,  3,   50,   D_BUZZER_END}
};
// AP 접속
const static U16 au16Buzz_APConnect[3][7]=
{
    {ON,  OFF, ON,  OFF, ON,  OFF,  OFF},
    {Do6, Do6, Re6, Re6, Mi6, Mi6,  0},
    {5,   20,  5,   20,  5,   40,   D_BUZZER_END}
};

/* Buzzer List Table **********************************************************/
Buzz_list_t BuzzList[BUZZ_NUM] = 
{ /* Id                       Table                              Size   SilentMode */
    { BUZZER_OFF,                 &au16Buzz_Off[0][0],               1U,    ON  },
    { BUZZER_POWER_ON,            &au16Buzz_PowerOn[0][0],           11U,   ON  },
    { BUZZER_EFFLUENT,            &au16Buzz_EffStart[0][0],          5U,    OFF },
    { BUZZER_EFFLUENT_END,        &au16Buzz_EffStop[0][0],           7U,    OFF },
    { BUZZER_COOL_SETUP,          &au16Buzz_CoolOn[0][0],            5U,    ON  },
    { BUZZER_COOL_CANCEL,         &au16Buzz_CoolOff[0][0],           7U,    ON  },
    { BUZZER_SELECT,              &au16Buzz_Select[0][0],            3U,    OFF },
    { BUZZER_ERROR,               &au16Buzz_Error[0][0],             9U,    ON  },
    { BUZZER_SETUP,               &au16Buzz_FuncSet[0][0],           5U,    ON  },
    { BUZZER_CANCEL,              &au16Buzz_FuncClr[0][0],           5U,    ON  },
    { BUZZER_EFFLUENT_CONTINUE,   &au16Buzz_EffConti[0][0],          17U,   OFF },
    { BUZZER_STERILIZE_START,     &au16Buzz_SterilizeStart[0][0],    7U,    OFF },
    { BUZZER_STERILIZE_END,       &au16Buzz_SterilizeEnd[0][0],      11U,   OFF },
    { BUZZER_MEMENTO_1,           &au16Buzz_Select[0][0],            3U,    ON  },
    { BUZZER_MEMENTO_2,           &au16Buzz_Memento2[0][0],          5U,    ON  },
    { BUZZER_MEMENTO_3,           &au16Buzz_Memento3[0][0],          7U,    ON  },
    { BUZZER_MEMENTO_4,           &au16Buzz_Memento4[0][0],          9U,    ON  },
    { BUZZER_MEMENTO_5,           &au16Buzz_Memento5[0][0],          11U,   ON  },
    { BUZZER_SERVER_CONNECT,      &au16Buzz_ServerConnect[0][0],     7U,    ON  },
    { BUZZER_AP_CONNECT,          &au16Buzz_APConnect[0][0],         7U,    ON  },
    { BUZZER_AP_PAIRING,          &au16Buzz_Select[0][0],            3U,    ON  }
};
#define SZ_BZ_LIST  ( sizeof(BuzzList) / sizeof(Buzz_list_t) )

/* Function *******************************************************************/
static void BuzzStart(void);
static void BuzzStop(void);
static void BuzzerCount(void);
static void BuzzerCheck(void);

void InitializeBuzzer(void)
{
    MEMSET( (void __FAR *)&Buzzer, 0, sizeof( Buzzer_T ));
    TURN_OFF_TDR();
}

void SetBuzzOut( E_BuzzId_T mu8BuzzId )
{
    if( Buzzer.Silent == ON
            && BuzzList[mu8BuzzId].SilentSet == OFF )
    {
        return;
    }
    Buzzer.CriticalSection = ON;
    Buzzer.Output = mu8BuzzId;
    Buzzer.Order = 0;
    Buzzer.Time = 0;
    Buzzer.CriticalSection = OFF;
}

U8 IsSetSilent(void)
{
    return Buzzer.Silent;
}

void SetOnOffSilent( U8 mu8Silent )
{
    if(mu8Silent == ON)
    {
        Buzzer.Silent = ON;
    }
    else
    {
        Buzzer.Silent = OFF;
    }
}

static void Buzzing(U16 mu16Time)
{
    if( mu16Time == 0 )
    {
        BuzzStart();
        BuzzerCheck();
    }
}

void BuzzerControl(void)
{
    //-1- Local Variable Definition
    static U8 mu8Count_BuzzCheckTime = 0;

    if(Buzzer.CriticalSection == ON)
    {
        return;
    }

    //-2- Buzzer Timer 10ms
    BuzzerCount();

    //-3- Buzzer Check & Output Setting
    if(Buzzer.Output == BUZZER_OFF) // Not Choice
    {
        BuzzStop();
        if(Buzzer.Silent == ON )
        {
            InitializeBuzzer();
            Buzzer.Silent = ON;
        }
        else
        {
            InitializeBuzzer();
        }
    }
    else // Choice
    {
        if( Buzzer.Silent == OFF )
        {
            Buzzing( Buzzer.Time );
        }
        else if ( BuzzList[Buzzer.Output].SilentSet==ON )
        {
            Buzzing( Buzzer.Time );
        }
    }

    //-4- TDR, PORT ON/OFF
    if( Buzzer.Power == ON )
    {
        TURN_ON_BUZZER_ONOFF();
        TURN_ON_TDR();
    }
    else
    {
        TURN_OFF_BUZZER_ONOFF();
    }
}

/* Local Function Information ==================================================
   ※ Description : 부저 TDR 레지스터 Start / Stop
   ==============================================================================*/
static void BuzzStart(void)
{
    SoundStart();
}

static void BuzzStop(void)
{
    SoundStop();
}

/* Local Function Information ==================================================
   ※ Description : 부저 시간 카운터
   ==============================================================================*/
static void BuzzerCount(void)
{
    static U8 mu8Count_BuzzCheckTime = 0;

    mu8Count_BuzzCheckTime++;
    if( mu8Count_BuzzCheckTime >= 10 )
    {
        mu8Count_BuzzCheckTime = 0;
        if(Buzzer.Time != 0)
        {
            Buzzer.Time--;
        }
    }
}

/* Local Function Information ==================================================
   ※ Description : 해당부저 Power, TDR, Time 설정
   ==============================================================================*/
static void BuzzerCheck(void)
{
    Buzz_list_t *pBuz = NULL;
    U8 mu8BuzzCount = 0;
    U16 mu16IdxTDR = 0;
    U16 mu16IdxTime = 0;

    for( mu8BuzzCount = 0 ; mu8BuzzCount < SZ_BZ_LIST; mu8BuzzCount++ )
    {
        pBuz = &BuzzList[ mu8BuzzCount ];
        mu16IdxTDR = pBuz->Size + Buzzer.Order;
        mu16IdxTime = (pBuz->Size*2) + Buzzer.Order;

        if( pBuz->Id == Buzzer.Output )
        {
            Buzzer.Power = pBuz->Type[ Buzzer.Order ];
            Buzzer.TDR = pBuz->Type[ mu16IdxTDR ];
            Buzzer.Time = pBuz->Type[ mu16IdxTime ];
            Buzzer.Order++;

            if(Buzzer.Time == D_BUZZER_END)
            {
                Buzzer.Output = BUZZER_OFF;
            }
            break;
        }
    }
}
