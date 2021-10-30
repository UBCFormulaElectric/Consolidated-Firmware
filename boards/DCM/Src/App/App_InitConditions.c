#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

struct InitConditions {
    bool initial_switch_position;
    bool switch_toggled;
};

struct InitConditions *App_InitConditions_Create() {
    struct InitConditions *init_conditions = malloc(sizeof(struct InitConditions));
    assert(init_conditions != NULL);

    init_conditions->initial_switch_position = false;
    init_conditions->switch_toggled = false;
    return init_conditions;
}

void App_InitConditions_SetInitialSwitchPosition(struct InitConditions *init_conditions, bool position) {
    init_conditions->initial_switch_position = position;
}

void App_InitConditions_SetSwitchToggled(struct InitConditions *init_conditions, bool start_switch) {
    if (!start_switch) {
        init_conditions->switch_toggled = true;
    }
}

bool App_InitConditions_CanTransition(struct InitConditions *const init_conditions, bool start_switch) {
    // If the switch started in the up position, it needs to be switched down first
    if (init_conditions->initial_switch_position) {
        return init_conditions->switch_toggled && start_switch;
    }

    return start_switch;
}

void App_InitConditions_Destroy(struct InitConditions *init_conditions) {
    free(init_conditions);
}
