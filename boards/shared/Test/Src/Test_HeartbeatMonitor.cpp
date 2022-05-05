#include "Test_Shared.h"

extern "C"
{
#include "App_SharedHeartbeatMonitor.h"
}

FAKE_VALUE_FUNC(uint32_t, get_current_ms);
FAKE_VOID_FUNC(timeout_callback, enum HeartbeatOneHot, enum HeartbeatOneHot);

class SharedHeartbeatMonitorTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, DEFAULT_TIMEOUT_PERIOD_MS, DEFAULT_BOARDS_TO_CHECK, timeout_callback);

        RESET_FAKE(get_current_ms);
        RESET_FAKE(timeout_callback);
    }

    void TearDown() override { TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy); }

    const enum HeartbeatOneHot DEFAULT_BOARDS_TO_CHECK =
        (enum HeartbeatOneHot)(BMS_HEARTBEAT_ONE_HOT | PDM_HEARTBEAT_ONE_HOT | DCM_HEARTBEAT_ONE_HOT);
    const uint32_t DEFAULT_TIMEOUT_PERIOD_MS = 500U;

    struct HeartbeatMonitor *heartbeat_monitor;
};

TEST_F(SharedHeartbeatMonitorTest, check_timeout_callback_is_called_for_no_check_in)
{
    constexpr uint32_t NUM_CONSECUTIVE_TIMEOUTS = 10;

    for (uint32_t i = 0; i < NUM_CONSECUTIVE_TIMEOUTS; i++)
    {
        for (uint32_t j = 0; j < DEFAULT_TIMEOUT_PERIOD_MS; j++)
        {
            App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
#ifdef NDEBUG
            ASSERT_EQ(i, timeout_callback_fake.call_count);
#else
            ASSERT_EQ(0, timeout_callback_fake.call_count);
#endif
            get_current_ms_fake.return_val++;
        }
    }
}

TEST_F(SharedHeartbeatMonitorTest, check_timeout_callback_is_called_for_partial_check_in)
{
    constexpr uint32_t NUM_CONSECUTIVE_TIMEOUTS = 10;

    for (uint32_t i = 0; i < NUM_CONSECUTIVE_TIMEOUTS; i++)
    {
        App_SharedHeartbeatMonitor_CheckIn(heartbeat_monitor, BMS_HEARTBEAT_ONE_HOT);

        for (uint32_t j = 0; j < DEFAULT_TIMEOUT_PERIOD_MS; j++)
        {
            App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
#ifdef NDEBUG
            ASSERT_EQ(i, timeout_callback_fake.call_count);
#else
            ASSERT_EQ(0, timeout_callback_fake.call_count);
#endif
            get_current_ms_fake.return_val++;
        }
    }
}

TEST_F(SharedHeartbeatMonitorTest, check_timeout_callback_is_not_called_for_complete_check_in)
{
    constexpr uint32_t NUM_CONSECUTIVE_TIMEOUTS = 10;

    for (uint32_t i = 0; i < NUM_CONSECUTIVE_TIMEOUTS; i++)
    {
        App_SharedHeartbeatMonitor_CheckIn(heartbeat_monitor, DEFAULT_BOARDS_TO_CHECK);

        for (uint32_t j = 0; j < DEFAULT_TIMEOUT_PERIOD_MS; j++)
        {
            App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
#ifdef NDEBUG
            ASSERT_EQ(0, timeout_callback_fake.call_count);
#else
            ASSERT_EQ(0, timeout_callback_fake.call_count);
#endif
            get_current_ms_fake.return_val++;
        }
    }
}

TEST_F(SharedHeartbeatMonitorTest, check_timing_does_not_drift_if_we_miss_hardware_timer_tick)
{
    constexpr uint32_t NUM_CONSECUTIVE_TIMEOUTS = 10;

    // First period: No skipped timer tick
    for (uint32_t j = 0; j < DEFAULT_TIMEOUT_PERIOD_MS; j++)
    {
        App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
#ifdef NDEBUG
        ASSERT_EQ(0, timeout_callback_fake.call_count);
#else
        ASSERT_EQ(0, timeout_callback_fake.call_count);
#endif
        get_current_ms_fake.return_val++;
    }

    // Second period: Skip one timer tick
    get_current_ms_fake.return_val++;
    for (uint32_t j = 0; j < DEFAULT_TIMEOUT_PERIOD_MS - 1; j++)
    {
        App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
#ifdef NDEBUG
        ASSERT_EQ(1, timeout_callback_fake.call_count);
#else
        ASSERT_EQ(0, timeout_callback_fake.call_count);
#endif
        get_current_ms_fake.return_val++;
    }

    // Third period onward: No skipped timer tick
    for (uint32_t i = 2; i < NUM_CONSECUTIVE_TIMEOUTS - 1; i++)
    {
        for (uint32_t j = 0; j < DEFAULT_TIMEOUT_PERIOD_MS; j++)
        {
            App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
#ifdef NDEBUG
            ASSERT_EQ(i, timeout_callback_fake.call_count);
#else
            ASSERT_EQ(0, timeout_callback_fake.call_count);
#endif
            get_current_ms_fake.return_val++;
        }
    }
}
