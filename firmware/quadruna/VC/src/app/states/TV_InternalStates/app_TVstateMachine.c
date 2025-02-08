#include "app_TVstateMachine.h"
#include <stddef.h>


static const TVinternalState *nextState; 
static const TVinternalState *currState; 

void app_TVstateMachine_init(TVinternalState *const initState)
{
    nextState = initState; 
    currState = initState;
    return; 
}

TVinternalState *app_TVstateMachine_getCurrentState(void)
{
    return currState;
}

void app_TVstateMachine_setNextState(TVinternalState *const state)
{
    nextState = state;
    return;
}

void app_TVstateMachine_tick100Hz(void *(tickFunction)())
{
    if(tickFunction != NULL)
    {
        tickFunction();
    }

    if(currState != nextState)
    {
        if(currState->run_on_exit != NULL)
        {
            currState->run_on_exit();
        }

        currState = nextState; 

        if(currState->run_on_entry != NULL)
        {
            currState->run_on_entry();
        }
    }

    // stay in currrent state unless explicitly told to transition; 
    nextState = currState;
}