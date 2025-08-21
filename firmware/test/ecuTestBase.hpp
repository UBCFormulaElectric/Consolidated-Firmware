#pragma once

#include <gtest/gtest.h>
#include "fake_io_time.hpp"

class EcuTestBase : public testing::Test
{
#define ASSERT_STATE_EQ(x)                              \
    ASSERT_EQ(app_stateMachine_getCurrentState(), (&x)) \
        << "Expected state: " << (&x)->name << ", but got: " << app_stateMachine_getCurrentState()->name
  public:
    ~EcuTestBase() override = default;

  private:
    virtual void tick_100hz()     = 0;
    virtual void tick_1hz()       = 0;
    virtual void board_setup()    = 0;
    virtual void board_teardown() = 0;

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
        // fake_io_time_getCurrentMs_returns(current_time_ms);
        board_setup();
    }
    void TearDown() override { board_teardown(); };

    void LetTimePass(const uint32_t time_ms)
    {
        for (uint32_t ms = 0; ms < time_ms; ms++)
        {
            if (current_time_ms % 1000 == 0)
            {
                tick_1hz();
            }

            if (current_time_ms % 10 == 0)
            {
                tick_100hz();
            }

            fake_io_time_getCurrentMs_returns(current_time_ms);
            current_time_ms++;
        }
    }

    uint32_t current_time_ms = 0;
};