#include <gtest/gtest.h>
#include "fake_io_time.hpp"
extern "C"
{
#include "app_heartbeatMonitorBoard.h"
}

// fake can states
bool                  MOCK_BOARD_CAN_FAULT     = false;
bool                  MOCK_CAN_HEARTBEAT_STATE = false;
uint8_t               timeout_ms               = 200;
HeartbeatMonitorBoard a_hbmonitor              = {
                 .timeout_ms = timeout_ms,
                 .getter     = []() { return MOCK_CAN_HEARTBEAT_STATE; },
    .resetter                                  = [](bool v) { MOCK_CAN_HEARTBEAT_STATE = v; },
    .fault_setter                              = [](bool v) { MOCK_BOARD_CAN_FAULT = v; },
    .fault_getter                              = []() { return MOCK_BOARD_CAN_FAULT; },
};

class HeartbeatMonitorTest : public testing::Test
{
  protected:
    void SetUp() override { app_heartbeatMonitorBoard_init(&a_hbmonitor); }
};

TEST_F(HeartbeatMonitorTest, test_create)
{
    // assert status, checked in, to check, getters, updaters, and fault setters all set correctly
    ASSERT_FALSE(a_hbmonitor.status);
    ASSERT_FALSE(a_hbmonitor.heartbeat_checked_in);
    ASSERT_EQ(a_hbmonitor.timer.state, TIMER_STATE_EXPIRED);
    ASSERT_TRUE(a_hbmonitor.fault_getter());
}

TEST_F(HeartbeatMonitorTest, test_broadcast_faults)
{
    // broadcast all good
    a_hbmonitor.status = true;
    app_heartbeatMonitorBoard_broadcastFaults(&a_hbmonitor);
    ASSERT_FALSE(MOCK_BOARD_CAN_FAULT);
    // broadcast all bad
    a_hbmonitor.status = false;
    app_heartbeatMonitorBoard_broadcastFaults(&a_hbmonitor);
    ASSERT_TRUE(MOCK_BOARD_CAN_FAULT);
}

TEST_F(HeartbeatMonitorTest, test_check_in_and_tick)
{
    /* test tick and check in functions*/
    fake_io_time_getCurrentMs_returns(0);

    // assert nothing changed (0 ms)
    app_heartbeatMonitorBoard_checkIn(&a_hbmonitor);
    ASSERT_FALSE(a_hbmonitor.heartbeat_checked_in);
    ASSERT_FALSE(a_hbmonitor.status);

    MOCK_CAN_HEARTBEAT_STATE = true;
    app_heartbeatMonitorBoard_checkIn(&a_hbmonitor);
    ASSERT_TRUE(a_hbmonitor.heartbeat_checked_in);
    ASSERT_TRUE(a_hbmonitor.status);

    // after a checkin, the CAN table should be reset to false
    app_heartbeatMonitorBoard_checkIn(&a_hbmonitor);
    ASSERT_FALSE(a_hbmonitor.heartbeat_checked_in);
    ASSERT_TRUE(a_hbmonitor.status);

    // right before the timer expires, the status should still be fine
    fake_io_time_getCurrentMs_returns(timeout_ms - 1);
    app_heartbeatMonitorBoard_checkIn(&a_hbmonitor);
    ASSERT_FALSE(a_hbmonitor.heartbeat_checked_in);
    ASSERT_TRUE(a_hbmonitor.status);

    // only after the timeout, should the status be false
    fake_io_time_getCurrentMs_returns(timeout_ms);
    app_heartbeatMonitorBoard_checkIn(&a_hbmonitor);
    ASSERT_FALSE(a_hbmonitor.heartbeat_checked_in);
    ASSERT_FALSE(a_hbmonitor.status);

    // with one checkin, the status should be fine again
    fake_io_time_getCurrentMs_returns(timeout_ms + 1);
    MOCK_CAN_HEARTBEAT_STATE = true;
    app_heartbeatMonitorBoard_checkIn(&a_hbmonitor);
    ASSERT_TRUE(a_hbmonitor.heartbeat_checked_in);
    ASSERT_TRUE(a_hbmonitor.status);
}