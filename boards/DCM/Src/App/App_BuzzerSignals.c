#include "App_DcmWorld.h"
#include "App_BuzzerSignals.h"

bool App_BuzzerSignals_IsOn(struct DcmWorld *world)
{
    return App_Buzzer_IsOn();
}

void App_BuzzerSignals_Callback(struct DcmWorld *world)
{
    App_Buzzer_TurnOff();
}
