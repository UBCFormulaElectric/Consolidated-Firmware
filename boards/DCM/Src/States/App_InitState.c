#include <stdbool.h>

#include "App_SharedStateType.h"
#include "States/App_States.h"

struct StateVariables
{
    bool     bad_thing;
    uint32_t init_var2;
    float    init_var3;
};

static struct StateVariables state_variables;

static void App_EntryAction(void)
{
    // Set default values for state variables
    state_variables.bad_thing = false;
    state_variables.init_var2 = 123;
    state_variables.init_var3 = 0.11f;
}

static void App_ExitAction(void) {}

static void App_StateAction(void)
{
    if (state_variables.bad_thing)
        INIT->set_next_state(FAULT);
    else
        INIT->set_next_state(DRIVE);
}

ADD_STATE(
    INIT,
    App_EntryAction,
    App_ExitAction,
    App_StateAction);
