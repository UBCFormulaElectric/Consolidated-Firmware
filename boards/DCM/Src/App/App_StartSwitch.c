#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define ON 1
#define OFF 0

struct StartSwitch
{
    bool initial_switch_position;
    bool switch_toggled_off;
};

struct StartSwitch *App_StartSwitch_Create()
{
    struct StartSwitch *start_switch = malloc(sizeof(struct StartSwitch));
    assert(start_switch != NULL);

    start_switch->initial_switch_position = false;
    start_switch->switch_toggled_off      = false;
    return start_switch;
}

void App_StartSwitch_SetInitialSwitchPosition(
    struct StartSwitch *start_switch,
    bool                state)
{
    start_switch->initial_switch_position = state;
}

void App_StartSwitch_SetSwitchToggledOff(
    struct StartSwitch *start_switch,
    bool                state)
{
    if (state == OFF)
    {
        start_switch->switch_toggled_off = true;
    }
}

bool App_StartSwitch_AbleToTransition(
    struct StartSwitch *start_switch,
    bool                state)
{
    // If the switch started in the up position, it needs to be switched down
    // first
    if (start_switch->initial_switch_position == ON)
    {
        return start_switch->switch_toggled_off && state;
    }

    return state;
}

void App_StartSwitch_Destroy(struct StartSwitch *start_switch)
{
    free(start_switch);
}
