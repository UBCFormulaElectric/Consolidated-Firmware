#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "App_InverterSwitches.h"

struct InverterSwitches
{
    void (*turn_on_right_inverter)(void);
    void (*turn_off_right_inverter)(void);
    void (*turn_on_left_inverter)(void);
    void (*turn_off_left_inverter)(void);
    bool (*is_right_inverter_on)(void);
    bool (*is_left_inverter_on)(void);
};

struct InverterSwitches *App_InverterSwitches_Create(
    void (*turn_on_right_inverter)(void),
    void (*turn_off_right_inverter)(void),
    void (*turn_on_left_inverter)(void),
    void (*turn_off_left_inverter)(void),
    bool (*is_right_inverter_on)(void),
    bool (*is_left_inverter_on)(void))
{
    struct InverterSwitches *inverter_switches = malloc(sizeof(struct InverterSwitches));
    assert(inverter_switches != NULL);

    inverter_switches->turn_on_right_inverter  = turn_on_right_inverter;
    inverter_switches->turn_off_right_inverter = turn_off_right_inverter;
    inverter_switches->turn_on_left_inverter   = turn_on_left_inverter;
    inverter_switches->turn_off_left_inverter  = turn_off_left_inverter;
    inverter_switches->is_right_inverter_on    = is_right_inverter_on;
    inverter_switches->is_left_inverter_on     = is_left_inverter_on;

    return inverter_switches;
}

void App_InverterSwitches_Destroy(struct InverterSwitches *inverter_switches)
{
    free(inverter_switches);
}

void App_InverterSwitches_TurnOnRight(struct InverterSwitches *inverter_switches)
{
    inverter_switches->turn_on_right_inverter();
}

void App_InverterSwitches_TurnOffRight(struct InverterSwitches *inverter_switches)
{
    inverter_switches->turn_off_right_inverter();
}

void App_InverterSwitches_TurnOnLeft(struct InverterSwitches *inverter_switches)
{
    inverter_switches->turn_on_left_inverter();
}

void App_InverterSwitches_TurnOffLeft(struct InverterSwitches *inverter_switches)
{
    inverter_switches->turn_off_left_inverter();
}

bool App_InverterSwitches_IsRightOn(struct InverterSwitches *inverter_switches)
{
    return inverter_switches->is_right_inverter_on();
}

bool App_InverterSwitches_IsLeftOn(struct InverterSwitches *inverter_switches)
{
    return inverter_switches->is_left_inverter_on();
}
