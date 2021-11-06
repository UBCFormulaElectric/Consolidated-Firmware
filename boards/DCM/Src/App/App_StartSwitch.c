#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

struct StartSwitch
{
    bool initial_switch_position;
    bool switch_toggled;
};

struct StartSwitch *App_StartSwitch_Create() {
    struct StartSwitch *start_switch = malloc(sizeof(struct StartSwitch));
    assert(start_switch != NULL);

    start_switch->initial_switch_position = false;
    start_switch->switch_toggled = false;
    return start_switch;
}

void App_StartSwitch_SetInitialSwitchPosition(struct StartSwitch *start_switch, bool position) {
    start_switch->initial_switch_position = position;
}

void App_StartSwitch_SetSwitchToggled(struct StartSwitch *start_switch, bool state) {
    if (!state) {
        start_switch->switch_toggled = true;
    }
}

bool App_StartSwitch_CanTransition(struct StartSwitch *start_switch, bool state) {
    // If the switch started in the up position, it needs to be switched down first
    if (start_switch->initial_switch_position) {
        return start_switch->switch_toggled && state;
    }

    return state;
}

void App_StartSwitch_Destroy(struct StartSwitch *start_switch) {
    free(start_switch);
}
