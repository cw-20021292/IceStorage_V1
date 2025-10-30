#include "cody_kit.h"
#include "water_out.h"



CodyKit_T   Cody;

void InitCodyKit(void)
{
    Cody.Status = 0;    /* Tank Open, FALSE:CLOSED, TRUE: OPEN */
    Cody.Step   = 0;
}


void SetCodyKitStatus(U8 mu8Status)
{
    Cody.Status = mu8Status;
}

U8 GetCodyKitStatus(void)
{
    return Cody.Status;
}


void SetCodyKitStep(U8 mu8Step)
{
    Cody.Step = mu8Step;
}

U8 GetCodyKitStep(void)
{
    return Cody.Step;
}
