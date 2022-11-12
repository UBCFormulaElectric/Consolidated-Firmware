#include "App_BrakeLight.h"
#include "Io_BrakeLight.h"

static bool is_turned_on;

void App_BrakeLight_Init()
{
    is_turned_on = false;
}

void App_BrakeLight_SetLightStatus(bool is_brake_actuated)
{
    if (is_brake_actuated)
    {
        Io_BrakeLight_TurnOn();
        is_turned_on = true;
    }
    else
    {
        Io_BrakeLight_TurnOff();
        is_turned_on = false;
    }
}

bool App_BrakeLight_IsTurnedOn()
{
    return is_turned_on;
}
