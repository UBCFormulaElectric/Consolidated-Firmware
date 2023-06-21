#pragma once

#include "App_SharedStateMachine.h"

/**
 * Check if an inverter has faulted
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if an inverter has faulted, false otherwise
 */
static inline bool App_HasInverterFault(void)
{
    return App_CanRx_INVL_InternalStates_VsmState_Get() == INVERTER_VSM_BLINK_FAULT_CODE_STATE ||
           App_CanRx_INVR_InternalStates_VsmState_Get() == INVERTER_VSM_BLINK_FAULT_CODE_STATE;
}

/**
 * Check if the start switch is pulled up to the on position
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if the start switch is on, false otherwise
 */
static inline bool App_IsStartSwitchOn(void)
{
    return App_CanRx_DIM_Switches_StartSwitch_Get() == SWITCH_ON;
}

static inline bool App_IsBmsInDriveState(void)
{
    return App_CanRx_BMS_Vitals_CurrentState_Get() == BMS_DRIVE_STATE;
}
/**
 * On-tick 1Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void App_AllStatesRunOnTick1Hz(struct StateMachine *state_machine);

/**
 * On-tick 100Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 * @return True if the next state is not the fault state, otherwise false
 */
bool App_AllStatesRunOnTick100Hz(struct StateMachine *state_machine);

/**
 * On-tick 1kHz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void App_AllStatesRunOnTick1kHz(struct StateMachine *const state_machine);
