#ifndef __USER_H__
#define __USER_H__

#include "prj_type.h"
#include "water_out.h"

#define CONFIG_USER_SODA    0

// User ID
typedef enum
{
    USER_NONE,  // None Selected User
    USER_A,     // user a
    USER_B,     // user b
    USER_C,     // user c
    USER_NUM
} UserId_T;

// Usage Type
typedef enum
{
    USAGE_DAILY,
    USAGE_MONTHLY,
    USAGE_NUM
} UsageType_T;


// Amount Unit 
#define MAX_LITER_VAL   60000U
typedef U16  Liter_T;

#define MAX_USER_MONTH  1
typedef struct _usage_
{
    Liter_T Daily;
    Liter_T Monthly[1];
} Usage_T;


typedef struct _user_
{
    UserId_T SelUser;
    Usage_T Usage[ USER_NUM ];
} UserInfo_T;

///////////////////////////////////////////////////////////////////////////////////////////////////////
//SODA
//
#if CONFIG_USER_SODA

// SODA 사용
#define SODA_USAGE_NUM      30
typedef struct _soda_usage_
{
    U8 Year;   
    U8 Month;  // 1 ~ 12 
    U8 Date;    // 1 ~ 31
    Liter_T Usage;
} SodaUsage_T;
typedef struct _soda_info_
{
    U8 Start;
    U8 Index;
    U8 Num;
    SodaUsage_T Day[ SODA_USAGE_NUM ];
} SodaInfo_T;

void InitSodaUsage(void);
void AddSodaUsageWater(U32 mu32Amount);
void GetSodaUsageData(SodaInfo_T *pData);
U8 GetSodaUsageDayData(U8 mu8Index, SodaUsage_T *pDay);
void SetSodaUsageWater(U8 mu8Index, U8 mu8Year, U8 mu8Month, U8 mu8Date, Liter_T mLiter );

void SetSodaUsageInfoIndex(U8 mu8Index);
U8 GetSodaUsageInfoIndex(void);

void SetSodaUsageInfoStart(U8 mu8Start);
U8 GetSodaUsageInfoStart(void);

void SetSodaUsageInfoNum(U8 mu8Num);
U8 GetSodaUsageInfoNum(void);
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////

void InitUser(void);
void GetUserData(UserInfo_T *pData);

#if 0
void SetUser(UserId_T mUserId );
UserId_T GetUser(void);
#endif


// mId - User ID
// mUsageType - UsageType ( daily monthly )
// mu8Month - month. when usage montly
#if 0
Liter_T GetUserUsageWater(UserId_T mId, UsageType_T mUsageType, U8 mu8Month );
#endif

Liter_T GetUserUsageDailyWater(UserId_T mId);

#if 0
Liter_T GetUserUsageMonthlyWater(UserId_T mId);
#endif

// mId - User ID
// mu8Water - room, cold, soda.. 
// mUsage - UsageType ( daily monthly )
// mu8Month - month. when usage montly
// mAmount - water usage amount... @ml
void SetUserUsageWater(UserId_T mId, UsageType_T mUsageType, U8 mu8Month, Liter_T mAmount );


// mAmount - water usage amount... 
void AddUserUsageWater(U32 mu32Amount);

// Clear all user's water usages...
void ClearAllUsageWater(void);

#if 0
void ProcessUser(void);
#endif

#endif /* __USER_H__ */
