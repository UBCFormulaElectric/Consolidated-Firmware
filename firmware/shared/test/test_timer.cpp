#include <gtest/gtest.h>

#include "fake_io_time.hpp"

extern "C"
{
#include "app_timer.c"
}

class TimerTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        current_time_ms = 0;
        fake_io_time_getCurrentMs_returns(0);
    }

    void PassTime(uint32_t delta_time_ms)
    {
        current_time_ms += delta_time_ms;
        fake_io_time_getCurrentMs_returns(current_time_ms);
    }

    uint32_t     current_time_ms;
    TimerChannel timer;
};

TEST_F(TimerTest, test_start)
{
    // Init timer, should be in idle
    app_timer_init(&timer, 1000); // 1s duration
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_IDLE);

    // Start timer
    app_timer_restart(&timer);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_RUNNING);

    // Confirm timer is running for 1s
    for (int t = 0; t < 1000; t += 100)
    {
        ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Duration has elapsed, timer should be expired
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_EXPIRED);

    // Confirm timer stays expired
    PassTime(100);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_EXPIRED);
}

TEST_F(TimerTest, test_stop_while_running)
{
    // Init timer, should be in idle
    app_timer_init(&timer, 1000); // 1s duration
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_IDLE);

    // Stop timer, still idle
    app_timer_stop(&timer);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_IDLE);

    // Start timer, don't let duration elapse
    app_timer_restart(&timer);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_RUNNING);

    for (int t = 0; t < 900; t += 100)
    {
        ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Stop timer before duration elapses, should go back to idle
    app_timer_stop(&timer);

    PassTime(100);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_IDLE);

    PassTime(100);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_IDLE);
}

TEST_F(TimerTest, test_stop_while_expired)
{
    // Init timer, should be in idle
    app_timer_init(&timer, 500); // 0.5s duration
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_IDLE);

    // Start timer, let duration elapse
    app_timer_restart(&timer);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_RUNNING);

    for (int t = 0; t < 500; t += 100)
    {
        ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Duration has elapsed, timer should be expired
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_EXPIRED);
    PassTime(100);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_EXPIRED);

    // Stop timer, should go back to idle even though already expired
    app_timer_stop(&timer);

    PassTime(100);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_IDLE);

    PassTime(100);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_IDLE);
}

TEST_F(TimerTest, test_run_if_condition)
{
    // Init timer, should be in idle
    app_timer_init(&timer, 5000); // 5s duration
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_IDLE);

    // Run w/ true condition, don't allow to complete
    for (int t = 0; t < 5000; t += 100)
    {
        ASSERT_EQ(app_timer_runIfCondition(&timer, true), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Set false condition, timer stays stopped
    ASSERT_EQ(app_timer_runIfCondition(&timer, false), TIMER_STATE_IDLE);
    PassTime(100);

    ASSERT_EQ(app_timer_runIfCondition(&timer, false), TIMER_STATE_IDLE);
    PassTime(100);

    // Run w/ true condition again, but let complete
    for (int t = 0; t < 5000; t += 100)
    {
        ASSERT_EQ(app_timer_runIfCondition(&timer, true), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    ASSERT_EQ(app_timer_runIfCondition(&timer, true), TIMER_STATE_EXPIRED);
    PassTime(100);

    // Confirm timer stays expired indefinitely
    for (int t = 0; t < 10000; t += 100)
    {
        ASSERT_EQ(app_timer_runIfCondition(&timer, true), TIMER_STATE_EXPIRED);
        PassTime(100);
    }

    // Finally, stop timer
    ASSERT_EQ(app_timer_runIfCondition(&timer, false), TIMER_STATE_IDLE);
}

TEST_F(TimerTest, test_get_elapsed_time)
{
    // Init timer, should be in idle
    app_timer_init(&timer, 2000); // 2s duration
    ASSERT_EQ(app_timer_getElapsedTime(&timer), 0);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_IDLE);

    // Check elapsed time w/ restart
    app_timer_restart(&timer);
    for (int t = 0; t < 1000; t += 100)
    {
        ASSERT_EQ(app_timer_getElapsedTime(&timer), t);
        ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Check restart actually restarts the elapsed time
    app_timer_restart(&timer);
    for (int t = 0; t < 1000; t += 100)
    {
        ASSERT_EQ(app_timer_getElapsedTime(&timer), t);
        ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Stop timer, elapsed time set back to 0
    app_timer_stop(&timer);
    ASSERT_EQ(app_timer_getElapsedTime(&timer), 0);
    ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_IDLE);

    // Check elapsed time w/ run if condition
    for (int t = 0; t < 1000; t += 100)
    {
        app_timer_runIfCondition(&timer, true);
        ASSERT_EQ(app_timer_getElapsedTime(&timer), t);
        ASSERT_EQ(app_timer_updateAndGetState(&timer), TIMER_STATE_RUNNING);
        PassTime(100);
    }

    // Check that if timer expires, elapsed time is clamped to duration
    for (int t = 1000; t < 3000; t += 100)
    {
        app_timer_runIfCondition(&timer, true);
        ASSERT_EQ(
            app_timer_getElapsedTime(&timer),
            (t >= 2000) ? 2000 : t); // If t >= 2000, timer expired, so clamp elapsed time to duration
        ASSERT_EQ(app_timer_updateAndGetState(&timer), (t >= 2000) ? TIMER_STATE_EXPIRED : TIMER_STATE_RUNNING);
        PassTime(100);
    }
}
