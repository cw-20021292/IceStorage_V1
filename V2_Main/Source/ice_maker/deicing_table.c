#include "deicing_table.h"

/* DE-ICING REGION LIST NUM */
#define  DEICING_REGION_1_COUNT   2        // ~10'c
#define  DEICING_REGION_2_COUNT   2        // ~15'c
#define  DEICING_REGION_3_COUNT   2        // ~20'c        
#define  DEICING_REGION_4_COUNT   2        // ~25'c    
#define  DEICING_REGION_5_COUNT   2        // ~30'c    
#define  DEICING_REGION_6_COUNT   2        // 30'c ~
#define	 DEICING_REGION_NUM	      6


static U8 OnOffCountList[ DEICING_REGION_NUM ] = 
{
    DEICING_REGION_1_COUNT,
    DEICING_REGION_2_COUNT,
    DEICING_REGION_3_COUNT,
    DEICING_REGION_4_COUNT,
    DEICING_REGION_5_COUNT,
    DEICING_REGION_6_COUNT,
};

// COMP ON/OFF TIME 
// TIME TICK : @100ms..
// ~ 10'c 
static U16 OnOffTimeList_Level_1[ DEICING_REGION_1_COUNT ] =  
{ 
    /* ON , OFF */
    7650U, 0U,     // 765sec
};

// 10'c ~ 20'c
static U16 OnOffTimeList_Level_2[ DEICING_REGION_2_COUNT ] = 
{ 
    /* ON , OFF */
    6000U, 0U,      // 600sec
};

// 20'c ~ 25'c 
static U16 OnOffTimeList_Level_3[ DEICING_REGION_3_COUNT ] = 
{ 
    /* ON , OFF */
    1800U, 0U,       // 180sec
};
static U16 OnOffTimeList_Level_4[ DEICING_REGION_4_COUNT ] = 
{ 
    300U, 0U,       // 30sec
};

// 25'c ~ 30'C
static U16 OnOffTimeList_Level_5[ DEICING_REGION_5_COUNT ] = 
{ 
    /* ON , OFF */
    200U, 0U,      // 20sec
};

// 30'c ~ 
static U16 OnOffTimeList_Level_6[ DEICING_REGION_6_COUNT ] = 
{ 
    /* ON , OFF */
    150U, 0U,       // 15sec
};


U8 GetDeIcingRegion( TEMP_T _tAmbi )
{
    if( _tAmbi >= 30.0f )  // over 30'c
    {
        return DEICING_REGION_6;
    }
    else if( _tAmbi >= 25.0f )  // over 30'c
    {
        return DEICING_REGION_5;
    }
    else if( _tAmbi >= 20.0f )  // over 25'c
    {
        return DEICING_REGION_4;
    }
    else if( _tAmbi >= 14.0f )   // over 20'c
    {
        return DEICING_REGION_3;
    }
    else if( _tAmbi >= 9.0f )   // over 14'c
    {
        return DEICING_REGION_2;
    }
    
    return DEICING_REGION_1;   // under 10'c
}

U8 GetDeIcingRepeatNum(U8 mu8Region )
{
    return OnOffCountList[ mu8Region ];
}

U8 GetDeIcingCmd(U8 mu8Index)
{
    /* Odd:TurnOn, Even:TurnOff */

    if( ( mu8Index % 2 == 0 ) )
    {
        return ON;   // turn on deicing heater
    }

    return OFF;   // turn off deicing heater
}

U16 GetDeIcingTime(U8 mu8Region, U8 mu8Index)
{
    U16 mu16Time = 0;

    // Time
    if( mu8Region == DEICING_REGION_1 )
    {
        mu16Time = OnOffTimeList_Level_1[ mu8Index ];
    }
    else if( mu8Region == DEICING_REGION_2 )
    {
        mu16Time = OnOffTimeList_Level_2[ mu8Index ];
    }
    else if( mu8Region == DEICING_REGION_3 )
    {
        mu16Time = OnOffTimeList_Level_3[ mu8Index ];
    }
    else if( mu8Region == DEICING_REGION_4 )
    {
        mu16Time = OnOffTimeList_Level_4[ mu8Index ];
    }
    else if( mu8Region == DEICING_REGION_5 )
    {
        mu16Time = OnOffTimeList_Level_5[ mu8Index ];
    }
    else if( mu8Region == DEICING_REGION_6 )
    {
        mu16Time = OnOffTimeList_Level_6[ mu8Index ];
    }
    else
    {
        // 25'c ~ 30'c
        mu16Time = OnOffTimeList_Level_5[ mu8Index ];
    }

    return mu16Time;
}

U8 GetDeIcingCompRps(U8 mu8Region)
{
    U8 mu8Rps;

    // Time
    if( mu8Region == DEICING_REGION_1 )
    {
        mu8Rps  = 50;
    }
    else if( mu8Region == DEICING_REGION_2 )
    {
        mu8Rps  = 50;
    }
    else if( mu8Region == DEICING_REGION_3 )
    {
        mu8Rps  = 50;
    }
    else if( mu8Region == DEICING_REGION_4 )
    {
        mu8Rps  = 47;
    }
    else if( mu8Region == DEICING_REGION_5 )
    {
        mu8Rps  = 47;
    }
    else if( mu8Region == DEICING_REGION_6 )
    {
        mu8Rps  = 43;
    }
    else 
    {
        //if( mu8Region == DEICING_REGION_5 )
        mu8Rps  = 43;
    }

    return mu8Rps;
}

U8 GetDeIcingFanOnOff(U8 mu8Region)
{
    U8 mu8FanOnOff = OFF;

    if( mu8Region == DEICING_REGION_1 )
    {
        mu8FanOnOff  = OFF;
    }
    else if( mu8Region == DEICING_REGION_2 )
    {
        mu8FanOnOff  = OFF;
    }
    else if( mu8Region == DEICING_REGION_3 )
    {
        mu8FanOnOff  = OFF;
    }
    else if( mu8Region == DEICING_REGION_4 )
    {
        mu8FanOnOff  = ON;
    }
    else if( mu8Region == DEICING_REGION_5 )
    {
        mu8FanOnOff  = ON;
    }
    else if( mu8Region == DEICING_REGION_6 )
    {
        mu8FanOnOff  = ON;
    }
    else
    {
        mu8FanOnOff  = ON;
    }

    return mu8FanOnOff;
}


