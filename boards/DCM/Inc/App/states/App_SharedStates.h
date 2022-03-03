#pragma once

#include "App_SharedStateMachine.h"

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

/**
 * Check if both AIRs are closed
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if both AIRs are closed, false otherwise
 */
static inline bool App_SharedStates_AreBothAIRsClosed(
    const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_BMS_AIR_STATES_GetSignal_AIR_POSITIVE(can_rx_interface) ==
               CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE &&
           App_CanRx_BMS_AIR_STATES_GetSignal_AIR_NEGATIVE(can_rx_interface) ==
               CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE;
}

/**
 * Check if an inverter has faulted
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if an inverter has faulted, false otherwise
 */
static inline bool App_SharedStates_HasInverterFaulted(
    const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_INVL_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVL(
               can_rx_interface) ==
               CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_BLINK_FAULT_CODE_STATE_CHOICE ||
           App_CanRx_INVR_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVR(
               can_rx_interface) ==
               CANMSGS_INVR_INTERNAL_STATES_D1_VSM_STATE_INVR_BLINK_FAULT_CODE_STATE_CHOICE;
}

/**
 * Check if the brake pedal is actuated
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if the brake is actuated, false otherwise
 */
static inline bool App_SharedStates_IsBrakeActuated(
    const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_FSM_BRAKE_GetSignal_BRAKE_IS_ACTUATED(can_rx_interface) ==
           CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_TRUE_CHOICE;
}

/**
 * Check if the start switch is pulled up to the on position
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if the start switch is on, false otherwise
 */
static inline bool App_SharedStates_IsStartSwitchOn(
    const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_DIM_SWITCHES_GetSignal_START_SWITCH(can_rx_interface) ==
           CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE;
}

/**
 * Check if the start switch was pulled up
 * @param prev_start_switch_position Start switch position from previous cycle
 * @param current_start_switch_position Start switch position on current cycle
 * @return true if the start switch was pulled up, false otherwise
 */
static inline bool App_SharedStates_WasStartSwitchPulledUp(
    bool prev_start_switch_position,
    bool current_start_switch_position)
{
    return !prev_start_switch_position && current_start_switch_position;
}

/**
 * Check if the start switch is off
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if the start switch is off, false otherwise
 */
static inline bool App_SharedStates_IsStartSwitchOff(
    const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_DIM_SWITCHES_GetSignal_START_SWITCH(can_rx_interface) ==
           CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE;
}

/**
 * Check if both inverters are OK
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if both inverters are OK, false otherwise
 */
static inline bool App_SharedStates_AreInvertersOK(
    const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_INVL_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVL(
               can_rx_interface) !=
               CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_BLINK_FAULT_CODE_STATE_CHOICE &&
           App_CanRx_INVR_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVR(
               can_rx_interface) !=
               CANMSGS_INVR_INTERNAL_STATES_D1_VSM_STATE_INVR_BLINK_FAULT_CODE_STATE_CHOICE;
}
