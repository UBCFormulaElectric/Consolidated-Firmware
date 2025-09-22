#pragma once

#include <gtest/gtest.h>
#include "io_timeFake.hpp"

#include <queue>

class EcuTestBase : public testing::Test
{
#define ASSERT_STATE_EQ(x)                              \
    ASSERT_EQ(app_stateMachine_getCurrentState(), (&x)) \
        << "Expected state: " << (&x)->name << ", but got: " << app_stateMachine_getCurrentState()->name
  public:
    ~EcuTestBase() override = default;

  private:
    virtual void board_setup()    = 0;
    virtual void board_teardown() = 0;

    struct EcuTask
    {
        uint32_t              next_run_time_ms;
        uint32_t              task_period_ms;
        std::function<void()> task_function;

        auto operator<=>(const EcuTask &other) const { return next_run_time_ms <=> other.next_run_time_ms; }
    };

    std::priority_queue<EcuTask, std::vector<EcuTask>, std::greater<EcuTask>> task_queue;

  protected:
    void SetUp() final
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
        fakes::time::setTime(current_time_ms);
        board_setup();
    }
    void TearDown() final { board_teardown(); };

    uint32_t current_time_ms = 0;

    void LetTimePass(const uint32_t time_ms)
    {
        for (uint32_t ms = 0; ms < time_ms; ms++)
        {
            while (task_queue.top().next_run_time_ms <= current_time_ms)
            {
                EcuTask t = task_queue.top();
                task_queue.pop();
                t.task_function();
                t.next_run_time_ms = current_time_ms + t.task_period_ms;
                task_queue.push(t);
            }

            assert(current_time_ms + 1 > current_time_ms); // overflow check
            current_time_ms++;
            fakes::time::setTime(current_time_ms);
        }
    }

    void register_task(std::function<void()> task_tick, const uint32_t task_period_ms)
    {
        task_queue.push({ 0, task_period_ms, std::move(task_tick) });
    }
};