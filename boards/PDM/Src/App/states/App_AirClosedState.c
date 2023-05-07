#include "states/App_AirClosedState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"
#include "states/App_AirOpenState.h"
#include "states/App_AllStates.h"

static void AirClosedStateRunOnEntry(struct StateMachine *const state_machine) {
  UNUSED(state_machine);
}

static void AirClosedStateRunOnTick1Hz(
    struct StateMachine *const state_machine) {
  App_AllStatesRunOnTick1Hz(state_machine);
}

static void AirClosedStateRunOnTick100Hz(
    struct StateMachine *const state_machine) {
  App_AllStatesRunOnTick100Hz(state_machine);

  const bool air_positive_open =
      App_CanRx_BMS_Contactors_AirPositive_Get() == CONTACTOR_STATE_OPEN;
  const bool air_negative_open =
      App_CanRx_BMS_Contactors_AirNegative_Get() == CONTACTOR_STATE_OPEN;

  struct PdmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
  struct HeartbeatMonitor *hb_monitor = App_PdmWorld_GetHeartbeatMonitor(world);
  const bool missing_hb = !App_SharedHeartbeatMonitor_Tick(hb_monitor);

  if (air_positive_open || air_negative_open || missing_hb) {
    App_SharedStateMachine_SetNextState(state_machine, App_GetAirOpenState());
  }
}

static void AirClosedStateRunOnExit(struct StateMachine *const state_machine) {
  UNUSED(state_machine);
}

const struct State *App_GetAirClosedState(void) {
  static struct State air_closed_state = {
      .name = "AIR CLOSED",
      .run_on_entry = AirClosedStateRunOnEntry,
      .run_on_tick_1Hz = AirClosedStateRunOnTick1Hz,
      .run_on_tick_100Hz = AirClosedStateRunOnTick100Hz,
      .run_on_exit = AirClosedStateRunOnExit,
  };

  return &air_closed_state;
}
