#pragma once

#include <gtest/gtest.h>
#include "fake_io_time.hpp"

extern "C"
{
#include "app_utils.h"
#include "app_stateMachine.h"
#include "app_timer.h"
}

class BaseStateMachineTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        // At t = 0ms, every on-tick function will be executed because of how we
        // decide which on-tick function(s) to run at each tick:
        //
        // if ( current_time_ms % task_period) {
        //     App_SharedStateMachine_TickXXXHz(state_machine);
        // }
        //
        // Take the 100Hz on-tick function for example, it will be called at
        // t = 0ms, 10ms, 20ms, etc.
        //
        // If a test case needs to call the 100Hz on-tick function multiple
        // times, we would need to write:
        //
        // LetTimePass(1);
        // LetTimePass(10);
        // LetTimePass(10);
        //
        // This is slightly confusing and ideally we would write:
        //
        // LetTimePass(10);
        // LetTimePass(10);
        // LetTimePass(10);
        //
        // A simple way to get around this is to start the state machine at
        // t = 1ms rather than t = 0ms.
        current_time_ms = 1;
        fake_io_time_getCurrentMs_returns(current_time_ms);
    }

    void LetTimePass(uint32_t time_ms)
    {
        for (uint32_t ms = 0; ms < time_ms; ms++)
        {
            if (current_time_ms % 1000 == 0)
            {
                app_stateMachine_tick1Hz();
            }

            if (current_time_ms % 10 == 0)
            {
                app_stateMachine_tick100Hz();
            }

            fake_io_time_getCurrentMs_returns(current_time_ms);
            current_time_ms++;
        }
    }

    uint32_t current_time_ms;
};
