#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "configs/App_StartSwitchConfig.h"

struct StartSwitch
{
    bool initial_position;
    bool current_position;
    bool switch_toggled_off;
};

struct StartSwitch *App_StartSwitch_Create()
{
    struct StartSwitch *start_switch = malloc(sizeof(struct StartSwitch));
    assert(start_switch != NULL);

    start_switch->initial_position   = false;
    start_switch->switch_toggled_off = false;
    start_switch->current_position   = false;
    return start_switch;
}

void App_StartSwitch_SetInitialPosition(
    struct StartSwitch *start_switch,
    bool                state)
{
    start_switch->initial_position = state;
}

void App_StartSwitch_SetPosition(struct StartSwitch *start_switch, bool state)
{
    start_switch->current_position = state;

    if (state == OFF_STATE)
    {
        start_switch->switch_toggled_off = true;
    }
}

bool App_StartSwitch_CanTransitionToDriveState(struct StartSwitch *start_switch)
{
    // If the switch started in the up position, it needs to be switched down
    // first
    if (start_switch->initial_position == ON_STATE)
    {
        return start_switch->switch_toggled_off &&
               start_switch->current_position == ON_STATE;
    }

    return start_switch->current_position == ON_STATE;
}

void App_StartSwitch_Destroy(struct StartSwitch *start_switch)
{
    free(start_switch);
}
