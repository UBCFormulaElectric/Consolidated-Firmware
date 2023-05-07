#include "states/App_InitState.h"

#include <math.h>

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"
#include "states/App_AllStates.h"

static void InitStateRunOnEntry(struct StateMachine *const state_machine) {
  UNUSED(state_machine);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine) {
  App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine) {
  App_AllStatesRunOnTick100Hz(state_machine);
}

static void InitStateRunOnExit(struct StateMachine *const state_machine) {
  UNUSED(state_machine);
}

const struct State *App_GetInitState() {
  static struct State init_state = {
      .name = "INIT",
      .run_on_entry = InitStateRunOnEntry,
      .run_on_tick_1Hz = InitStateRunOnTick1Hz,
      .run_on_tick_100Hz = InitStateRunOnTick100Hz,
      .run_on_exit = InitStateRunOnExit,
  };

  return &init_state;
}
