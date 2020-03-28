#pragma once

#include "App_SharedStateMachine.h"

// TODO: move function defs into `.c`?
// TODO: proper naming for this struct and function

static void runStateRunOnEnter(struct StateMachine* state_machine){}

static void runStateRunOnTick(struct StateMachine* state_machine){
}

static void runStateRunOnExit(struct StateMachine* state_machine){
}

// TODO: can we do this without a function for initializatino

// TODO: proper name and jdoc for this
struct State* getRunState(){
    static struct State run_state;
    // TODO: figure out how to set name here
    run_state.run_on_enter =runStateRunOnEnter;
    run_state.run_on_tick = runStateRunOnTick;
    run_state.run_on_exit = runStateRunOnExit;

    return &run_state;
}
