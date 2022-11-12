#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "App_InverterSwitches.h"
#include "Io_InverterSwitches.h"

void App_InverterSwitches_TurnOnRight()
{
    Io_InverterSwitches_TurnOnRight();
}

void App_InverterSwitches_TurnOffRight()
{
    Io_InverterSwitches_TurnOffRight();
}

void App_InverterSwitches_TurnOnLeft()
{
    Io_InverterSwitches_TurnOnLeft();
}

void App_InverterSwitches_TurnOffLeft()
{
    Io_InverterSwitches_TurnOffLeft();
}

bool App_InverterSwitches_IsRightOn()
{
    return Io_InverterSwitches_IsRightInverterOn();
}

bool App_InverterSwitches_IsLeftOn()
{
    return Io_InverterSwitches_IsLeftInverterOn();
}
