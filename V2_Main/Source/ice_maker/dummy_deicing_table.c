#include "dummy_deicing_table.h"

/* DUMMY DE-ICING REGION LIST NUM */
#define DUMMY_DEICING_REGION_1_COUNT   14
#define DUMMY_DEICING_REGION_2_COUNT   6
#define DUMMY_DEICING_REGION_3_COUNT   6
#define DUMMY_DEICING_REGION_4_COUNT   4
#define DUMMY_DEICING_REGION_5_COUNT   4
#define	DUMMY_DEICING_REGION_NUM	   5


#define DUMMY_DEICING_COMP_ON_TIME      (3U*600U) /* 3min, @100msec */

#if 0
static const U8 OnOffCountList[ DUMMY_DEICING_REGION_NUM ] = 
{
    DUMMY_DEICING_REGION_1_COUNT,
    DUMMY_DEICING_REGION_2_COUNT,
    DUMMY_DEICING_REGION_3_COUNT,
    DUMMY_DEICING_REGION_4_COUNT,
    DUMMY_DEICING_REGION_5_COUNT
};

// under 10'c
static const U16 OnOffTimeList_Level_1[ DUMMY_DEICING_REGION_1_COUNT ] = 
{ 
    /* ON , OFF */
    70, 60, 
    25, 60, 
    15, 70, 
    15, 80, 
    15, 90, 

    15, 100, 
    15, 30 
};

// over 10'c
static const U16 OnOffTimeList_Level_2[ DUMMY_DEICING_REGION_2_COUNT ] = 
{ 
    /* ON , OFF */
    60, 160, 
    40, 160, 
    35, 25
};

// over 20'c
static const U16 OnOffTimeList_Level_3[ DUMMY_DEICING_REGION_3_COUNT ] = 
{ 
    /* ON , OFF */
    50, 70, 
    17, 70, 
    10, 23
};

// over 30'c
static const U16 OnOffTimeList_Level_4[ DUMMY_DEICING_REGION_4_COUNT ] = 
{ 
    /* ON , OFF */
    50, 70, 
    17, 103, 
};


U8 GetDummyDeIcingRegion( TEMP_T _tAmbi )
{
    if( _tAmbi >= 30 )  // over 30'c
    {
        return DUMMY_DEICING_REGION_4;
    }
    else if( _tAmbi >= 20 )   // over 20'c
    {
        return DUMMY_DEICING_REGION_3;
    }
    else if( _tAmbi >= 10 )   // over 10'c
    {
        return DUMMY_DEICING_REGION_2;
    }
    
    return DUMMY_DEICING_REGION_1;   // under 10'c
}

U8 GetDummyDeIcingRepeatNum(U8 mu8Region )
{
    return OnOffCountList[ mu8Region ];
}

U8 GetDummyDeIcingCmd(U8 mu8Index)
{
    /* Odd:TurnOn, Even:TurnOff */

    if( ( mu8Index % 2 == 0 ) )
    {
        return ON;   // turn on deicing heater
    }

    return OFF;   // turn off deicing heater
}

U16 GetDummyDeIcingTime(U8 mu8Region, U8 mu8Index)
{
    U16 mu16Time;

    // Time
    if( mu8Region == DUMMY_DEICING_REGION_1 )
    {
        mu16Time = OnOffTimeList_Level_1[ mu8Index ];
    }
    else if( mu8Region == DUMMY_DEICING_REGION_2 )
    {
        mu16Time = OnOffTimeList_Level_2[ mu8Index ];
    }
    else if( mu8Region == DUMMY_DEICING_REGION_3 )
    {
        mu16Time = OnOffTimeList_Level_3[ mu8Index ];
    }
    else 
    {
        mu16Time = OnOffTimeList_Level_4[ mu8Index ];
    }

    return mu16Time;
}
#endif

U16 GetDummyDeIcingCompOnTime(U8 mu8Region)
{
    (void)mu8Region;

    return DUMMY_DEICING_COMP_ON_TIME;
}
