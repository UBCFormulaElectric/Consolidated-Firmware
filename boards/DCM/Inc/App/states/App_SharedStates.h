#pragma once

#include "App_SharedStateMachine.h"
#include "configs/App_RegenThresholds.h"

/**
 * Check if an inverter has faulted
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if an inverter has faulted, false otherwise
 */
static inline bool App_SharedStates_HasInverterFaulted(const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_INVL_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVL(can_rx_interface) ==
               CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_BLINK_FAULT_CODE_STATE_CHOICE ||
           App_CanRx_INVR_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVR(can_rx_interface) ==
               CANMSGS_INVR_INTERNAL_STATES_D1_VSM_STATE_INVR_BLINK_FAULT_CODE_STATE_CHOICE;
}

/**
 * Check if the start switch is pulled up to the on position
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if the start switch is on, false otherwise
 */
static inline bool App_SharedStates_IsStartSwitchOn(const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_DIM_SWITCHES_GetSignal_START_SWITCH(can_rx_interface) ==
           CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE;
}

/**
 * On-tick 1Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void App_SharedStatesRunOnTick1Hz(struct StateMachine *state_machine);

/**
 * On-tick 100Hz function for every state in the given state machine
 * @param state_machine The state machine to run on-tick function for
 */
void App_SharedStatesRunOnTick100Hz(struct StateMachine *state_machine);
