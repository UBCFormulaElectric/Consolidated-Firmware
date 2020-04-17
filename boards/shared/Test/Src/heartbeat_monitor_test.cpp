#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_SharedHeartbeat.h"

    DEFINE_FFF_GLOBALS;
    FAKE_VALUE_FUNC(uint32_t, get_current_ms);
    FAKE_VOID_FUNC(
        timeout_callback,
        enum HeartbeatOneHot,
        enum HeartbeatOneHot);
}

class SharedHeartbeatMonitorTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        constexpr uint32_t             DEFAULT_TIMEOUT_PERIOD_MS = 500U;
        constexpr enum HeartbeatOneHot DEFAULT_BOARDS_TO_CHECK =
            BMS_HEARTBEAT_ONE_HOT | PDM_HEARTBEAT_ONE_HOT |
            DIM_HEARTBEAT_ONE_HOT;

        struct HeartbeatMonitor *App_SharedHeartbeatMonitor_Create(
            get_current_ms, DEFAULT_TIMEOUT_PERIOD_MS, DEFAULT_BOARDS_TO_CHECK,
            timeout_callback);
    }

    void TearDown() override
    {
        ASSERT_TRUE(heartbeat_monitor != NULL);
        App_SharedHeartbeatMonitor_Destroy(heartbeat_monitor);
        heartbeat_monitor = NULL;
    }

    struct HeartbeatMonitor *heartbeat_monitor;
};

TEST_F(
    SharedHeartbeatMonitorTest,
    check_timeout_callback_is_called_for_no_check_in)
{
    constexpr uint32_t NUM_CONSECUTIVE_TIMEOUTS = 10;

    for (uint32_t i = 0; i < NUM_CONSECUTIVE_TIMEOUTS; i++)
    {
        for (uint32_t j = 0; j < DEFAULT_TIMEOUT_PERIOD_MS; j++)
        {
            App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
            ASSERT_EQ(i, timeout_callback.call_count);
            get_current_ms_fake.return_val++;
        }

        App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
        ASSERT_EQ(i + 1, timeout_callback.call_count);
        get_current_ms_fake.return_val++;
    }
}

TEST_F(
    SharedHeartbeatMonitorTest,
    check_timeout_callback_is_called_for_partial_check_in)
{
    constexpr uint32_t NUM_CONSECUTIVE_TIMEOUTS = 10;

    for (uint32_t i = 0; i < NUM_CONSECUTIVE_TIMEOUTS; i++)
    {
        App_SharedHeartbeatMonitor_CheckIn(BMS_HEARTBEAT_ONE_HOT);

        for (uint32_t j = 0; j < DEFAULT_TIMEOUT_PERIOD_MS; j++)
        {
            App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
            ASSERT_EQ(i, timeout_callback.call_count);
            get_current_ms_fake.return_val++;
        }

        App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
        ASSERT_EQ(i + 1, timeout_callback.call_count);
        get_current_ms_fake.return_val++;
    }
}

TEST_F(
    SharedHeartbeatMonitorTest,
    check_timeout_callback_is_not_called_for_complete_check_in)
{
    constexpr uint32_t NUM_CONSECUTIVE_TIMEOUTS = 10;

    for (uint32_t i = 0; i < NUM_CONSECUTIVE_TIMEOUTS; i++)
    {
        App_SharedHeartbeatMonitor_CheckIn(DEFAULT_BOARDS_TO_CHECK);

        for (uint32_t j = 0; j < DEFAULT_TIMEOUT_PERIOD_MS; j++)
        {
            App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
            ASSERT_EQ(0, timeout_callback.call_count);
            get_current_ms_fake.return_val++;
        }

        App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
        ASSERT_EQ(0, timeout_callback.call_count);
        get_current_ms_fake.return_val++;
    }
}