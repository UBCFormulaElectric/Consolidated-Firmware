#include <gtest/gtest.h>
#include <stdbool.h>

#include "fake_io_time.hpp"

extern "C"
{
#include "app_heartbeatMonitor.h"
}

// fake can states
bool DIM_fake_can_fault_state     = false;
bool BMS_fake_can_fault_state     = false;
bool DIM_fake_can_heartbeat_state = false;
bool BMS_fake_can_heartbeat_state = false;
bool FSM_fake_can_heartbeat_state = false;

// fake can methods
void DIM_fakeCanHeartbeatUpdater(bool val)
{
    DIM_fake_can_heartbeat_state = val;
}

void BMS_fakeCanHeartbeatUpdater(bool val)
{
    BMS_fake_can_heartbeat_state = val;
}

void FSM_fakeCanHeartbeatSetter(bool val)
{
    FSM_fake_can_heartbeat_state = val;
}

void DIM_fakeCanFaultSetter(bool val)
{
    DIM_fake_can_fault_state = val;
}

void BMS_fakeCanFaultSetter(bool val)
{
    BMS_fake_can_fault_state = val;
}

bool DIM_fakeCanFaultGetter(void)
{
    return DIM_fake_can_fault_state;
}

bool BMS_fakeCanFaultGetter(void)
{
    return BMS_fake_can_fault_state;
}

bool DIM_fakeCanHeartbeatGetter(void)
{
    return DIM_fake_can_heartbeat_state;
}

bool BMS_fakeCanHeartbeatGetter(void)
{
    return BMS_fake_can_heartbeat_state;
}

// callback config structs to be passed in to methods later on
void (*fault_can_setters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD]  = &BMS_fakeCanFaultSetter,
                                                           [VC_HEARTBEAT_BOARD]   = NULL,
                                                           [RSM_HEARTBEAT_BOARD]  = NULL,
                                                           [FSM_HEARTBEAT_BOARD]  = NULL,
                                                           [DIM_HEARTBEAT_BOARD]  = &DIM_fakeCanFaultSetter,
                                                           [CRIT_HEARTBEAT_BOARD] = NULL };

void (*heartbeat_can_updaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD]  = &BMS_fakeCanHeartbeatUpdater,
                                                                [VC_HEARTBEAT_BOARD]   = NULL,
                                                                [RSM_HEARTBEAT_BOARD]  = NULL,
                                                                [FSM_HEARTBEAT_BOARD]  = NULL,
                                                                [DIM_HEARTBEAT_BOARD]  = &DIM_fakeCanHeartbeatUpdater,
                                                                [CRIT_HEARTBEAT_BOARD] = NULL };

bool (*heartbeat_can_getters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD]  = &BMS_fakeCanHeartbeatGetter,
                                                           [VC_HEARTBEAT_BOARD]   = NULL,
                                                           [RSM_HEARTBEAT_BOARD]  = NULL,
                                                           [FSM_HEARTBEAT_BOARD]  = NULL,
                                                           [DIM_HEARTBEAT_BOARD]  = &DIM_fakeCanHeartbeatGetter,
                                                           [CRIT_HEARTBEAT_BOARD] = NULL };

bool (*fault_can_getters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD]  = &BMS_fakeCanFaultGetter,
                                                       [VC_HEARTBEAT_BOARD]   = NULL,
                                                       [RSM_HEARTBEAT_BOARD]  = NULL,
                                                       [FSM_HEARTBEAT_BOARD]  = NULL,
                                                       [DIM_HEARTBEAT_BOARD]  = &DIM_fakeCanFaultGetter,
                                                       [CRIT_HEARTBEAT_BOARD] = NULL };

TEST(HeartbeatMonitor, test_check_faults)
{
    /* test check faults */
    // this hypothetical (fake) board only checks the BMS and DIM, and is the FSM
    bool heartbeats_to_check[HEARTBEAT_BOARD_COUNT];
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeats_to_check[board] = false;
    }

    heartbeats_to_check[BMS_HEARTBEAT_BOARD] = true;
    heartbeats_to_check[DIM_HEARTBEAT_BOARD] = true;

    app_heartbeatMonitor_init(
        heartbeats_to_check, heartbeat_can_getters, heartbeat_can_updaters, &FSM_fakeCanHeartbeatSetter,
        fault_can_setters, fault_can_getters);

    // check that there are no faults
    DIM_fake_can_fault_state = false;
    BMS_fake_can_fault_state = false;

    ASSERT_EQ(app_heartbeatMonitor_checkFaults(), false);

    // check that if there is a fault, it is detected
    DIM_fake_can_fault_state = false;
    BMS_fake_can_fault_state = true;

    ASSERT_EQ(app_heartbeatMonitor_checkFaults(), true);
}

TEST(HeartbeatMonitor, test_create)
{
    /* test heartbeat monitor create */

    // this hypothetical (fake) board only checks the BMS and DIM, and is the FSM
    bool heartbeats_to_check[HEARTBEAT_BOARD_COUNT];
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeats_to_check[board] = false;
    }

    heartbeats_to_check[BMS_HEARTBEAT_BOARD] = true;
    heartbeats_to_check[DIM_HEARTBEAT_BOARD] = true;

    // create and assert time fields work
    app_heartbeatMonitor_init(
        heartbeats_to_check, heartbeat_can_getters, heartbeat_can_updaters, &FSM_fakeCanHeartbeatSetter,
        fault_can_setters, fault_can_getters);
    HeartbeatMonitor *heartbeat_monitor = app_heartbeatMonitor_get();

    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, 0U);

    // check self heartbeat setter
    ASSERT_EQ(heartbeat_monitor->setter, &FSM_fakeCanHeartbeatSetter);

    // assert status, checked in, to check, getters, updaters, and fault setters all set correctly
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[board]);
        ASSERT_TRUE(heartbeat_monitor->status[board]);
        ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[board], heartbeats_to_check[board]);

        ASSERT_EQ(heartbeat_monitor->getters[board], heartbeat_can_getters[board]);
        ASSERT_EQ(heartbeat_monitor->updaters[board], heartbeat_can_updaters[board]);
        ASSERT_EQ(heartbeat_monitor->fault_setters[board], fault_can_setters[board]);
    }
}

TEST(HeartbeatMonitor, test_broadcast_faults)
{
    /* test broadcast faults */
    // initialize monitor

    bool heartbeats_to_check[HEARTBEAT_BOARD_COUNT] = {
        [BMS_HEARTBEAT_BOARD] = true,  [VC_HEARTBEAT_BOARD] = false, [RSM_HEARTBEAT_BOARD] = false,
        [FSM_HEARTBEAT_BOARD] = false, [DIM_HEARTBEAT_BOARD] = true, [CRIT_HEARTBEAT_BOARD] = false
    };

    app_heartbeatMonitor_init(
        heartbeats_to_check, heartbeat_can_getters, heartbeat_can_updaters, &FSM_fakeCanHeartbeatSetter,
        fault_can_setters, fault_can_getters);
    HeartbeatMonitor *heartbeat_monitor = app_heartbeatMonitor_get();

    // broadcast all good
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeat_monitor->status[board] = true;
    }

    app_heartbeatMonitor_broadcastFaults();

    ASSERT_FALSE(DIM_fake_can_fault_state);
    ASSERT_FALSE(BMS_fake_can_fault_state);

    // broadcast only bms and dim good (should be indistinguishable from all good)
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeat_monitor->status[board] = false;
    }
    heartbeat_monitor->status[DIM_HEARTBEAT_BOARD] = true;
    heartbeat_monitor->status[BMS_HEARTBEAT_BOARD] = true;
    app_heartbeatMonitor_broadcastFaults();

    ASSERT_FALSE(DIM_fake_can_fault_state);
    ASSERT_FALSE(BMS_fake_can_fault_state);

    // broadcast only bms good (should trigger dim fault)
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeat_monitor->status[board] = false;
    }
    heartbeat_monitor->status[DIM_HEARTBEAT_BOARD] = false;
    heartbeat_monitor->status[BMS_HEARTBEAT_BOARD] = true;
    app_heartbeatMonitor_broadcastFaults();

    ASSERT_TRUE(DIM_fake_can_fault_state);
    ASSERT_FALSE(BMS_fake_can_fault_state);

    // broadcast all bad
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeat_monitor->status[board] = false;
    }
    app_heartbeatMonitor_broadcastFaults();

    ASSERT_TRUE(DIM_fake_can_fault_state);
    ASSERT_TRUE(BMS_fake_can_fault_state);
}

TEST(HeartbeatMonitor, test_check_in_and_tick)
{
    /* test tick and check in functions*/
    fake_io_time_getCurrentMs_returns(0);

    // our hypothetical FSM board relies on BMS and DIM
    bool heartbeats_to_check[HEARTBEAT_BOARD_COUNT];
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeats_to_check[board] = false;
    }

    heartbeats_to_check[BMS_HEARTBEAT_BOARD] = true;
    heartbeats_to_check[DIM_HEARTBEAT_BOARD] = true;

    app_heartbeatMonitor_init(
        heartbeats_to_check, heartbeat_can_getters, heartbeat_can_updaters, &FSM_fakeCanHeartbeatSetter,
        fault_can_setters, fault_can_getters);
    HeartbeatMonitor *heartbeat_monitor = app_heartbeatMonitor_get();

    // assert nothing changed (0 ms)
    app_heartbeatMonitor_tick();
    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, 0U);

    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[BMS_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[VC_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[FSM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[RSM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[DIM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[CRIT_HEARTBEAT_BOARD]);

    ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[BMS_HEARTBEAT_BOARD], heartbeats_to_check[BMS_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[VC_HEARTBEAT_BOARD], heartbeats_to_check[VC_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[FSM_HEARTBEAT_BOARD], heartbeats_to_check[FSM_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[RSM_HEARTBEAT_BOARD], heartbeats_to_check[RSM_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[DIM_HEARTBEAT_BOARD], heartbeats_to_check[DIM_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[CRIT_HEARTBEAT_BOARD], heartbeats_to_check[CRIT_HEARTBEAT_BOARD]);

    // confirm status good
    ASSERT_TRUE(heartbeat_monitor->status[BMS_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[VC_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[FSM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[RSM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[DIM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[CRIT_HEARTBEAT_BOARD]);

    // check in BMS and DIM midway (150 ms)
    fake_io_time_getCurrentMs_returns(150);

    DIM_fake_can_heartbeat_state = true;
    BMS_fake_can_heartbeat_state = true;
    FSM_fake_can_heartbeat_state = false;

    app_heartbeatMonitor_checkIn();

    ASSERT_TRUE(FSM_fake_can_heartbeat_state);
    ASSERT_FALSE(DIM_fake_can_heartbeat_state);
    ASSERT_FALSE(BMS_fake_can_heartbeat_state);

    ASSERT_TRUE(heartbeat_monitor->heartbeats_checked_in[BMS_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->heartbeats_checked_in[DIM_HEARTBEAT_BOARD]);

    // progress to timeout period and verify all good (300 ms)
    fake_io_time_getCurrentMs_returns(300);
    app_heartbeatMonitor_tick();

    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, 300);

    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[BMS_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[VC_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[FSM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[RSM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[DIM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[CRIT_HEARTBEAT_BOARD]);

    ASSERT_TRUE(heartbeat_monitor->status[BMS_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[VC_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[FSM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[RSM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[DIM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[CRIT_HEARTBEAT_BOARD]);

    // check in DIM midway (450 ms)
    fake_io_time_getCurrentMs_returns(450);

    DIM_fake_can_heartbeat_state = true;
    BMS_fake_can_heartbeat_state = false;
    FSM_fake_can_heartbeat_state = false;

    app_heartbeatMonitor_checkIn();

    ASSERT_TRUE(FSM_fake_can_heartbeat_state);
    ASSERT_FALSE(DIM_fake_can_heartbeat_state);
    ASSERT_FALSE(BMS_fake_can_heartbeat_state);

    ASSERT_TRUE(heartbeat_monitor->heartbeats_checked_in[DIM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[BMS_HEARTBEAT_BOARD]);

    // progress to timeout period and verify bms missing (600 ms)
    fake_io_time_getCurrentMs_returns(600);
    app_heartbeatMonitor_tick();

    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, 600);

    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[BMS_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[VC_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[FSM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[RSM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[DIM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[CRIT_HEARTBEAT_BOARD]);

    // BMS not checked in returns false
    ASSERT_FALSE(heartbeat_monitor->status[BMS_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[VC_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[FSM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[RSM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[DIM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[CRIT_HEARTBEAT_BOARD]);
}