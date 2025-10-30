#include "eco.h"

#include "temp.h"


#define DEFAULT_START_TIME          108000U     //30 hours
#define DEFAULT_STOP_TIME           43200U      //12 hours

#define DEFAULT_COLD_OUT_COUNT      4U      
#define DEFAULT_COLD_OUT_TIME       120U    // 2min


Eco_T Eco;


static void StartEco(void);
static void StopEco(void);
static void ResetEcoStartTime(void);

void InitEco(void)
{
    Eco.Status       = ECO_STOP;

    Eco.StartTime   = DEFAULT_START_TIME;
    Eco.StopTime    = DEFAULT_STOP_TIME;

    Eco.IsColdOut       = FALSE;
    Eco.ColdOutCount    = DEFAULT_COLD_OUT_COUNT;
}

EcoStatus_T GetEcoStatus(void)
{
    return Eco.Status;
}

static void StartEco(void)
{
    Eco.Status       = ECO_START;

    Eco.IsColdOut           = FALSE;
    Eco.IsColdOutStopTime   = FALSE;
    Eco.ColdOutTime  = DEFAULT_COLD_OUT_TIME;
    Eco.ColdOutCount = DEFAULT_COLD_OUT_COUNT;
    Eco.StopTime     = DEFAULT_STOP_TIME;
}

static void StopEco(void)
{
    Eco.Status = ECO_STOP;

    Eco.IsColdOut        = FALSE;
    Eco.IsColdOutStopTime = FALSE;
    ResetEcoStartTime();
}

static void ResetEcoStartTime(void)
{
    Eco.StartTime = DEFAULT_START_TIME;
}

void ResetEco(void)
{
    if( Eco.Status == ECO_STOP )
    {
        ResetEcoStartTime();
    }
}


void CountEcoColdOut(void)
{
    if( Eco.Status == ECO_START )
    {
        // 냉수 추출 됨.
        Eco.IsColdOutStopTime = TRUE;

        Eco.IsColdOut = TRUE;

        // 냉수 추출 타이머 초기화
        if( Eco.ColdOutTime == 0 )
        {
            Eco.ColdOutTime = DEFAULT_COLD_OUT_TIME;
        }

        if( Eco.ColdOutCount != 0 )
        {
            Eco.ColdOutCount--;
        }
    }
}


void ProcessEco(void)
{
    TEMP_T tAmbi;


    tAmbi = GetTemp( TEMP_ID_AMBIENT );
    // 진입 조건
    if( Eco.Status == ECO_STOP )
    {
        if( tAmbi >= 23.0f && tAmbi <= 28.0f )
        {
            if( --Eco.StartTime == 0 )
            {
                StartEco();
            }
        }
        else
        {
            ResetEcoStartTime();
        }
    }
    else
    {
        if( tAmbi < 23.0f || tAmbi > 28.0f )
        {
            StopEco();
        }

        // 첫 추출 후 2식나 초과시 중지..
        if( Eco.IsColdOutStopTime == TRUE )
        {
            if( --Eco.StopTime == 0 )
            {
                StopEco();
            }
        }

        // 추출 시작 후 2분내에 4회 미만 추출시
        if( Eco.IsColdOut == TRUE )
        {
            if( Eco.ColdOutTime != 0 )
            {
                Eco.ColdOutTime--;
            }
            else 
            {
                if( Eco.ColdOutCount > 0 )
                {
                    StopEco();
                }
                else
                {
                    Eco.IsColdOut    = FALSE;
                    Eco.ColdOutTime  = DEFAULT_COLD_OUT_TIME;
                    Eco.ColdOutCount = DEFAULT_COLD_OUT_COUNT;
                }
            }
        }
    }

}
