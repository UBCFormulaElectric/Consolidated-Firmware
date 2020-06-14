#pragma once

#include <gtest/gtest.h>

extern "C"
{
#include "App_SharedStateMachine.h"
}

class BaseStateMachineTest : public testing::Test
{
  protected:
    void SetUp(void) override { current_time_ms = 0; }

    virtual void UpdateClock(
        struct StateMachine *state_machine,
        uint32_t             current_time_ms) = 0;

    virtual void UpdateSignals(
        struct StateMachine *state_machine,
        uint32_t             current_time_ms) = 0;

    void LetTimePass(struct StateMachine *state_machine, uint32_t time_ms)
    {
        for (uint32_t ms = 0; ms < time_ms; ms++)
        {
            UpdateClock(state_machine, current_time_ms);
            UpdateSignals(state_machine, current_time_ms);

            if (current_time_ms % 1000 == 0)
            {
                App_SharedStateMachine_Tick1Hz(state_machine);
            }

            if (current_time_ms % 10 == 0)
            {
                App_SharedStateMachine_Tick100Hz(state_machine);
            }

            current_time_ms++;
        }
    }
    uint32_t current_time_ms;
};
