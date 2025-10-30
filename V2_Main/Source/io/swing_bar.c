#include "hw.h"
#include "swing_bar.h"


SwingBar_T Swing;


void InitSwingBar(void)
{
    Swing.OnOff      = OFF;
    Swing.OnTime     = 0;
    Swing.OffTime    = 0; 

    Swing.ConfOnTime  = DEFAULT_SWING_BAR_TIME_ON;
    Swing.ConfOffTime = DEFAULT_SWING_BAR_TIME_OFF_1;
}

void GetSwingBarData( SwingBar_T *pData )
{
    MEMCPY( (void __FAR *)pData, (void __FAR *)&Swing, sizeof( SwingBar_T ) );
}

void SetSwingBarConfigOffTime(U16 mu16ConfTime)
{
    Swing.ConfOffTime = mu16ConfTime;
}

void TurnOnSwingBar(void)
{
    // Set new time value when turn on swing bar...
    if( Swing.OnOff == OFF )
    {
        Swing.OnTime  = Swing.ConfOnTime;
        Swing.OffTime = Swing.ConfOffTime;
    }

    Swing.OnOff = ON;;
}

void TurnOffSwingBar(void)
{
    Swing.OnOff = OFF;
}


void ControlSwingBar(void)
{
    // TURN OFF SWING BAR
    if( Swing.OnOff == OFF )
    { 
        TURN_OFF_SWING_BAR();

        return ;
    }

    // TURN ON SWING BAR
    if( Swing.OnTime != 0 )
    {
        Swing.OnTime--;

        TURN_ON_SWING_BAR();
    }
    else if( Swing.OffTime != 0 )
    {
        Swing.OffTime--;
        
        TURN_OFF_SWING_BAR();
    }
    else
    {
       Swing.OnTime  = Swing.ConfOnTime;
       Swing.OffTime = Swing.ConfOffTime;
    }
}
