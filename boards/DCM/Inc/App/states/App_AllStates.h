#pragma once

#include "App_SharedStateMachine.h"

/**
 * Check if an inverter has faulted
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if an inverter has faulted, false otherwise
 */
static inline bool App_HasInverterFault(void)
{
    // TODO: JSONCAN
    // return App_CanRx_INVL_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVL(can_rx_interface) ==
    //            CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_BLINK_FAULT_CODE_STATE_CHOICE ||
    //        App_CanRx_INVR_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVR(can_rx_interface) ==
    //            CANMSGS_INVR_INTERNAL_STATES_D1_VSM_STATE_INVR_BLINK_FAULT_CODE_STATE_CHOICE;

    return false;
}

/**
 * Check if the start switch is pulled up to the on position
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if the start switch is on, false otherwise
 */
static inline bool App_IsStartSwitchOn(void)
{
    // TODO: JSONCAN
    // return App_CanRx_DIM_SWITCHES_GetSignal_START_SWITCH(can_rx_interface) ==
    //        CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE;

    return false;
}

static inline bool App_IsBmsInDriveState(void)
{
    // TODO: JSONCAN
    // return App_CanRx_BMS_STATE_MACHINE_GetSignal_STATE(can_rx) == CANMSGS_BMS_STATE_MACHINE_STATE_DRIVE_CHOICE;
    return false;
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
