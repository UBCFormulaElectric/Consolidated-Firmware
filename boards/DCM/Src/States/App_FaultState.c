#include "App_SharedStateType.h"
#include "States/App_States.h"

static void App_EntryAction(void) {}

static void App_ExitAction(void) {}

static void App_StateAction(void) {}

ADD_STATE(
    FAULT,
    App_EntryAction,
    App_ExitAction,
    App_StateAction);
