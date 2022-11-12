#include <assert.h>
#include <stdlib.h>
#include "App_Buzzer.h"
#include "Io_Buzzer.h"

static bool is_on;

void App_Buzzer_Init()
{
    is_on = false;
}

void App_Buzzer_TurnOn()
{
    Io_Buzzer_TurnOn();
    is_on = true;
}

void App_Buzzer_TurnOff()
{
    Io_Buzzer_TurnOff();
    is_on = false;
}

bool App_Buzzer_IsOn()
{
    return is_on;
}
