#include "user.h"
#include "eeprom.h"
#include "rtc.h"

#include <stdio.h>
#include <string.h>



// 저장 단위 
#define LITER_UNIT  10  // @10ml
//#define LITER_UNIT  100  // @100ml


UserInfo_T User;



void InitUser(void)
{
    MEMSET( (void __FAR *)&User, 0, sizeof( UserInfo_T ) );
}

void GetUserData(UserInfo_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&User, sizeof(UserInfo_T));
}


void SetUser( UserId_T mUserId )
{
    User.SelUser = mUserId;
}

UserId_T GetUser( void )
{
    return User.SelUser;
}


#if 0
Liter_T GetUserUsageWater(UserId_T mId, UsageType_T mUsageType, U8 mu8Month )
{
    Liter_T mAmount = 0;

    if( mUsageType == USAGE_DAILY )
    {
        // User - Daily usage
        mAmount =  User.Usage[ mId ].Daily;
    }
    else
    {
        // User - Monthly usage 
        mAmount = User.Usage[ mId ].Monthly[mu8Month];
    }

    return mAmount;
}
#endif

static void SaveEepromUserUsage(UserId_T mId )
{
#if 0
    if( mId == USER_A || mId == USER_B )
    {
        SaveEepromId( EEP_ID_USER_A );
        SaveEepromId( EEP_ID_USER_B );
    }
    else
    {
        SaveEepromId( EEP_ID_USER_C );
        SaveEepromId( EEP_ID_USER_NONE );
    }
#endif
}



Liter_T GetUserUsageDailyWater(UserId_T mId)
{
    return User.Usage[ mId ].Daily;
}

#if 0
Liter_T GetUserUsageMonthlyWater(UserId_T mId)
{
    return User.Usage[ mId ].Monthly[ 0 ];
}
#endif

void SetUserUsageWater(UserId_T mId, UsageType_T mUsageType, U8 mu8Month, Liter_T mAmount )
{
    if( mUsageType == USAGE_DAILY )
    {
        // User - Daily usage
        User.Usage[ mId ].Daily = mAmount;
    }
    else
    {
        // User - Monthly usage 
        User.Usage[ mId ].Monthly[mu8Month] = mAmount;
    }

    SaveEepromUserUsage( mId );
}

static U8 GetCurrentMonth(void)
{
    TimeData_T  mTime;

    GetRtcTime( &mTime );
    return mTime.Month;
}

void AddUserUsageWater(U32 mu32Amount)
{
    UserId_T mUserId;
    Liter_T mLiter;

    mUserId = User.SelUser;

    // User - Daily/Monthly usage
    mLiter = (Liter_T)(mu32Amount / LITER_UNIT );   

    if( User.Usage[ mUserId ].Daily < MAX_LITER_VAL )
    {
        User.Usage[ mUserId ].Daily += mLiter;
    }
    if( User.Usage[ mUserId ].Monthly[0] < MAX_LITER_VAL )
    {
        User.Usage[ mUserId ].Monthly[0] += mLiter;
    }


    SaveEepromUserUsage( mUserId );
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
//SODA
//
#if CONFIG_USER_SODA
SodaInfo_T SodaInfo;
// 초기화 - 소다 사용량 저장
void InitSodaUsage(void)
{
    MEMSET((void __FAR *)&SodaInfo, 0, sizeof( SodaInfo_T ));
    SodaInfo.Index      = 0;    
    SodaInfo.Start      = 0;    
}


// 년, 월,일이 변경되면, 저장 인덱스를 변경한다.
static U8 CheckSodaUsageDate(void)
{
    TimeData_T mTime;
    SodaUsage_T *pUsage = NULL;


    GetRtcTime( &mTime );
    pUsage = &SodaInfo.Day[ SodaInfo.Index ];

    // 날짜 설정이 처음인 경우,
    if( pUsage->Year == 0 
            && pUsage->Month == 0 
            && pUsage->Date == 0 )
    {
        pUsage->Year    = mTime.Year;
        pUsage->Month   = mTime.Month;
        pUsage->Date    = mTime.Date;
        pUsage->Usage   = 0;

        return FALSE;
    }

    // 날짜가 다른 경우, 
    if( mTime.Year != pUsage->Year
            || mTime.Month != pUsage->Month
            || mTime.Date != pUsage->Date )
    {
        // Different date, next usage
        SodaInfo.Index++;
        if( SodaInfo.Index >= SODA_USAGE_NUM )
        {
            SodaInfo.Index = 0;
        }

        if( SodaInfo.Index == SodaInfo.Start )
        {
            SodaInfo.Start++;
            if( SodaInfo.Start >= SODA_USAGE_NUM )
            {
                SodaInfo.Start = 0;
            }
        }

        SodaInfo.Num++;
        if( SodaInfo.Num >= SODA_USAGE_NUM )
        {
            SodaInfo.Num = SODA_USAGE_NUM - 1;
        }

        // Clean Up next usage
        pUsage = &SodaInfo.Day[ SodaInfo.Index ];
        pUsage->Year    = mTime.Year;
        pUsage->Month   = mTime.Month;
        pUsage->Date    = mTime.Date;
        pUsage->Usage   = 0;

        return TRUE;    // 변경
    }

    return FALSE;   // 유지
}

static void SaveEepromSodaUsage(void)
{
    EepromId_T mBlockId;

    mBlockId = EEP_ID_SODA_USAGE_0 + SodaInfo.Index;
    SaveEepromId( mBlockId );
}

// 탄산 추출량 저장
void AddSodaUsageWater(U32 mu32Amount)
{
    U8 mu8Ret = FALSE;
    Liter_T mLiter;


    // Check date
    mu8Ret = CheckSodaUsageDate();
    if( mu8Ret == TRUE )
    {
        SaveEepromId( EEP_ID_SODA_INFO );
    }

    mLiter = (Liter_T)(mu32Amount / LITER_UNIT );   
    if( SodaInfo.Day[ SodaInfo.Index ].Usage < MAX_LITER_VAL )
    {
        SodaInfo.Day[ SodaInfo.Index ].Usage += mLiter;
    }
    SaveEepromSodaUsage();
}

void GetSodaUsageData(SodaInfo_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&SodaInfo, sizeof(SodaInfo_T));
}

U8 GetSodaUsageDayData(U8 mu8Index, SodaUsage_T *pDay)
{
    if( mu8Index < SODA_USAGE_NUM )
    {
        MEMCPY( (void __FAR *)pDay, (const void __FAR *)&SodaInfo.Day[ mu8Index ], sizeof( SodaUsage_T ));

        return TRUE;
    }

    return FALSE;
}

void SetSodaUsageWater(U8 mu8Index, U8 mu8Year, U8 mu8Month, U8 mu8Date, Liter_T mLiter )
{
    SodaUsage_T *pUsage = NULL;

    if( mu8Index < SODA_USAGE_NUM )
    {
        pUsage = &SodaInfo.Day[ mu8Index ];
        pUsage->Year    = mu8Year;
        pUsage->Month   = mu8Month;
        pUsage->Date    = mu8Date;
        pUsage->Usage   = mLiter;
    }
}

void SetSodaUsageInfoIndex(U8 mu8Index)
{
    if( mu8Index < SODA_USAGE_NUM )
    {
        SodaInfo.Index = mu8Index;
    }
}

U8 GetSodaUsageInfoIndex(void)
{
    return SodaInfo.Index;
}

void SetSodaUsageInfoStart(U8 mu8Start)
{
    if( mu8Start < SODA_USAGE_NUM )
    {
        SodaInfo.Start = mu8Start;
    }
}

U8 GetSodaUsageInfoStart(void)
{
    return SodaInfo.Start;
}

void SetSodaUsageInfoNum(U8 mu8Num)
{
    SodaInfo.Num = mu8Num;
}

U8 GetSodaUsageInfoNum(void)
{
    return SodaInfo.Num;
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////


static void ClearAllDailyUsage(void)
{
    U8 i;

    for( i = 0; i < USER_NUM ; i++ )
    {
        SetUserUsageWater( i, USAGE_DAILY, 0, 0 );
    }
}

static void ClearAllMonthlyUsage(void)
{
    U8 mu8User;
    U8 mu8Month;

    //user
    for( mu8User = 0; mu8User < USER_NUM ; mu8User++ ) 
    {
        // month
        for( mu8Month = 0; mu8Month < MAX_USER_MONTH; mu8Month++ ) 
        {
            SetUserUsageWater( mu8User, USAGE_MONTHLY, mu8Month, 0 );
        }
    }
}

// 현재달 데이터를 이전 달 데이터로 이전
// 그리고 현재 달 데이터를 초기화
#if 0
static void ChangeMonthly(void)
{
    U8 i;
    Liter_T mLiter;


    for( i = 0 ; i < USER_NUM ; i++ )
    {
        mLiter = GetUserUsageWater( i, USAGE_MONTHLY, 0 );  // Get current month data
        SetUserUsageWater( i, USAGE_MONTHLY, 1, mLiter );   // Set previous month data
        SetUserUsageWater( i, USAGE_MONTHLY, 0, 0 );    // clear current month data
    }
}
#endif


void ClearAllUsageWater(void)
{
    ClearAllMonthlyUsage();
    ClearAllDailyUsage();
}

#if 0
void ProcessUser(void)
{
    static U8 mu8Init = FALSE;
    static U8 mu8PrevDay = 0;
    static U8 mu8PrevMonth = 0;
    static U8 mu8PrevYear = 0;
    U8 mu8Month;
    TimeData_T mTime;


    GetRtcTime( &mTime );
    if( mu8Init == FALSE )
    {
        mu8Init = TRUE;

        mu8PrevDay = mTime.Date;
        mu8PrevMonth = mTime.Month;
        mu8PrevYear = mTime.Year;
        return ;
    }

    // 하루가 지나면, Daily를 초기화
    // 달이 변경되면, montly를 이전 달로 이동하고 초기화
    if( mu8PrevDay != mTime.Date )
    {
        mu8PrevDay = mTime.Date;

        ClearAllDailyUsage();

#if 0
        if( mTime.Date == 1 )
        {
            ChangeMonthly();
        }
#endif
    }

    // 사용자에 의해 달이 변경되는 경우.
    if( mu8PrevMonth != mTime.Month )
    {
        mu8PrevMonth = mTime.Month;

        ClearAllMonthlyUsage();
        ClearAllDailyUsage();
    }
    else if( mu8PrevYear != mTime.Year )
    {
        // 사용자에 의해 년도가 변경되는 경우
        mu8PrevYear = mTime.Year;

        ClearAllMonthlyUsage();
        ClearAllDailyUsage();
    }
}
#endif

