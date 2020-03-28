#pragma once

#include "App_SharedStateMachine.h"
#include "states/App_RunState.h"

// TODO: move function defs into `.c`?
// TODO: proper naming for this struct and function

static void initStateRunOnEnter(struct StateMachine* state_machine){
    struct DcmWorld* world = App_SharedStateMachine_GetWorld(state_machine
        );
    struct CanTxInterface* can_tx_interface = App_DcmWorld_GetCanTx(world);

    struct CanMsgs_dcm_startup_t payload = {
        .dummy = 13
    };
    App_CanTx_SendNonPeriodicMsg_DCM_STARTUP(can_tx_interface, &payload);
}

static void initStateRunOnTick(struct StateMachine* state_machine){
    // No need for any safety checks, just run! (this is a demo)

    App_SharedStateMachine_TransitionState(state_machine, getRunState());
}

static void initStateRunOnExit(struct StateMachine* state_machine){
}

// TODO: can we do this without a function for initializatino

// TODO: proper name and jdoc for this
struct State* getInitState(){
    static struct State initial_state;
    // TODO: figure out how to set name here
    initial_state.run_on_enter =initStateRunOnEnter;
    initial_state.run_on_tick = initStateRunOnTick;
    initial_state.run_on_exit = initStateRunOnExit;

    return &initial_state;
}
