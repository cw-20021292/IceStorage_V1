#include "display_clock.h"
#include "rtc.h"


TimeItem_T mMenuItem = 0;       // INDEX

MenuTime_T mMenuTime;   // CLOCK DATA


static void ToggleAmPm(void);
static void CountDownMin(void);
static void CountUpMin(void);
static void CountUpHour(void);
static void CountDownHour(void);

void InitClock(U8 mu8Init)
{
    TimeData_T mTime;


    // Init Index
    mMenuItem = MENU_ITEM_MIN;

    // Get Init Time.
    GetRtcTime( &mTime );

    // SET Init time val
    mMenuTime.u8Year    = mTime.Year;
    mMenuTime.u8Month   = mTime.Month;
    mMenuTime.u8Date    = mTime.Date;
    GetTimeHour12( mTime.Hour, &mMenuTime.u8AmPm, &mMenuTime.u8Hour );
    mMenuTime.u8Min     = mTime.Min;

    if( mu8Init == TRUE )
    {
        mMenuTime.u8Year  = 20;
        mMenuTime.u8Month = 1;
        mMenuTime.u8Date  = 1;
        mMenuTime.u8AmPm  = TIME_AM;
        mMenuTime.u8Hour  = 1;
        mMenuTime.u8Min   = 0;
    }

}

TimeItem_T GetClockItem(void)
{
    return mMenuItem;
}

void GetClockTime(MenuTime_T *pTime)
{
    pTime->u8Year    = mMenuTime.u8Year;
    pTime->u8Month   = mMenuTime.u8Month;
    pTime->u8Date    = mMenuTime.u8Date;
    pTime->u8AmPm    = mMenuTime.u8AmPm;
    pTime->u8Hour    = mMenuTime.u8Hour;
    pTime->u8Min     = mMenuTime.u8Min;
}

void NextClock(void)
{
    mMenuItem++;
    if( mMenuItem > MENU_ITEM_MAX )
    {
        mMenuItem = MENU_ITEM_MIN;
    }
}

void UpClock(void)
{
    switch( mMenuItem )
    {
        case MENU_ITEM_TIME_AMPM:
            ToggleAmPm();
            break;

        case MENU_ITEM_TIME_HOUR:
            CountUpHour();
            break;

        case MENU_ITEM_TIME_MIN:
            CountUpMin();
            break;
    }
}

void DownClock(void)
{
    switch( mMenuItem )
    {
        case MENU_ITEM_TIME_AMPM:
            ToggleAmPm();
            break;

        case MENU_ITEM_TIME_HOUR:
            CountDownHour();
            break;

        case MENU_ITEM_TIME_MIN:
            CountDownMin();
            break;
    }
}


void SaveClock(void)
{
    TimeData_T mTime;


    GetRtcTime( &mTime );
    mTime.Y2K      = 1;
    mTime.Year     = mMenuTime.u8Year;
    mTime.Month    = mMenuTime.u8Month;
    mTime.Date     = mMenuTime.u8Date;

    if( mMenuTime.u8AmPm == TIME_AM )
    {
        if( mMenuTime.u8Hour == 12 )
        {
            mTime.Hour = 0;
        }
        else
        {
           mTime.Hour = mMenuTime.u8Hour;
        }
    }
    else 
    {
        // TIME_PM
        if( mMenuTime.u8Hour == 12 )
        {
            mTime.Hour = mMenuTime.u8Hour;
        }
        else
        {
           mTime.Hour = mMenuTime.u8Hour + 12;
        }
    }

    mTime.Min      = mMenuTime.u8Min;
    mTime.Sec      = 0;

    SetRtcTime( &mTime );

    SetRtcConfigInit( TRUE );
}

// 24시간 표기 법을 12시간 표기법으로 변경
void GetTimeHour12(U8 hour24, U8 *pAm, U8 *pHour12)
{
    if( pAm == NULL || pHour12 == NULL )
    {
        return ;
    }

    if( hour24 == 0 )       // 0
    {
        *pAm = TIME_AM;
        *pHour12 = 12;
    }
    else if( hour24 < 12 )  // 1 ~ 11
    {
        *pAm = TIME_AM;
        *pHour12 = hour24;
    }   
    else if( hour24 == 12 ) // 12
    {
        *pAm = TIME_PM;
        *pHour12 = 12;
    }
    else                    // 13 ~ 23
    {
        *pAm = TIME_PM;
        *pHour12 = hour24 - 12;
    }
}

static void ToggleAmPm(void)
{
    if( mMenuTime.u8AmPm == TIME_AM )
    {
        mMenuTime.u8AmPm = TIME_PM;
    }
    else
    {
        mMenuTime.u8AmPm = TIME_AM;
    }
}

static void CountDownMin(void)
{
    if( mMenuTime.u8Min != 0 )
    {
        mMenuTime.u8Min--;
    }
    else
    {
        mMenuTime.u8Min = 59;
    }
}

static void CountUpMin(void)
{
    if( mMenuTime.u8Min < 59 )
    {
        mMenuTime.u8Min++;
    }
    else
    {
        mMenuTime.u8Min = 0;
    }
}

static void CountUpHour(void)
{
    if( mMenuTime.u8Hour < 12 )
    {
        mMenuTime.u8Hour++;
    }
    else
    {
        mMenuTime.u8Hour = 1;
    }

}

static void CountDownHour(void)
{
    if( mMenuTime.u8Hour != 1 )
    {
        mMenuTime.u8Hour--;
    }
    else
    {
        mMenuTime.u8Hour = 12;
    }
}


