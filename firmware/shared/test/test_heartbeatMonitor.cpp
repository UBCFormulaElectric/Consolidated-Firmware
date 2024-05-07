#include <gtest/gtest.h>

#include "fake_io_time.hpp"

extern "C"
{
#include "app_heartbeatBoardsEnum.h"
#include "app_heartbeatMonitor.c"
}

// fake can states
bool E_fake_can_fault_state = false;
bool A_fake_can_fault_state = false;

bool E_fake_can_heartbeat_state = false;
bool A_fake_can_heartbeat_state = false;
bool D_fake_can_heartbeat_state = false;

// fake can methods
void E_fakeCanHeartbeatUpdater(bool val)
{
    E_fake_can_heartbeat_state = val;
}

void A_fakeCanHeartbeatUpdater(bool val)
{
    A_fake_can_heartbeat_state = val;
}

void D_fakeCanHeartbeatSetter(bool val)
{
    D_fake_can_heartbeat_state = val;
}

void E_fakeCanFaultSetter(bool val)
{
    E_fake_can_fault_state = val;
}

void A_fakeCanFaultSetter(bool val)
{
    A_fake_can_fault_state = val;
}

bool E_fakeCanFaultGetter()
{
    return E_fake_can_fault_state;
}

bool A_fakeCanFaultGetter()
{
    return A_fake_can_fault_state;
}

bool E_fakeCanHeartbeatGetter()
{
    return E_fake_can_heartbeat_state;
}

bool A_fakeCanHeartbeatGetter()
{
    return A_fake_can_heartbeat_state;
}

// callback config structs to be passed in to methods later on
void (*fault_can_setters[HEARTBEAT_BOARD_COUNT])(bool) = { [A_HEARTBEAT_BOARD] = &A_fakeCanFaultSetter,
                                                           [B_HEARTBEAT_BOARD] = NULL,
                                                           [C_HEARTBEAT_BOARD] = NULL,
                                                           [D_HEARTBEAT_BOARD] = NULL,
                                                           [E_HEARTBEAT_BOARD] = &E_fakeCanFaultSetter };

void (*heartbeat_can_updaters[HEARTBEAT_BOARD_COUNT])(bool) = { [A_HEARTBEAT_BOARD] = &A_fakeCanHeartbeatUpdater,
                                                                [B_HEARTBEAT_BOARD] = NULL,
                                                                [C_HEARTBEAT_BOARD] = NULL,
                                                                [D_HEARTBEAT_BOARD] = NULL,
                                                                [E_HEARTBEAT_BOARD] = &E_fakeCanHeartbeatUpdater };

bool (*heartbeat_can_getters[HEARTBEAT_BOARD_COUNT])() = { [A_HEARTBEAT_BOARD] = &A_fakeCanHeartbeatGetter,
                                                           [B_HEARTBEAT_BOARD] = NULL,
                                                           [C_HEARTBEAT_BOARD] = NULL,
                                                           [D_HEARTBEAT_BOARD] = NULL,
                                                           [E_HEARTBEAT_BOARD] = &E_fakeCanHeartbeatGetter };

bool (*fault_can_getters[HEARTBEAT_BOARD_COUNT])() = { [A_HEARTBEAT_BOARD] = &A_fakeCanFaultGetter,
                                                       [B_HEARTBEAT_BOARD] = NULL,
                                                       [C_HEARTBEAT_BOARD] = NULL,
                                                       [D_HEARTBEAT_BOARD] = NULL,
                                                       [E_HEARTBEAT_BOARD] = &E_fakeCanFaultGetter };

TEST(HeartbeatMonitor, test_check_faults)
{
    /* test check faults */
    // this hypothetical (fake) board only checks the A and E, and is the D
    bool heartbeats_to_check[HEARTBEAT_BOARD_COUNT];
    for (bool &board : heartbeats_to_check)
    {
        board = false;
    }

    heartbeats_to_check[A_HEARTBEAT_BOARD] = true;
    heartbeats_to_check[E_HEARTBEAT_BOARD] = true;

    app_heartbeatMonitor_init(
        heartbeats_to_check, heartbeat_can_getters, heartbeat_can_updaters, &D_fakeCanHeartbeatSetter,
        fault_can_setters, fault_can_getters);

    // check that there are no faults
    E_fake_can_fault_state = false;
    A_fake_can_fault_state = false;

    ASSERT_EQ(app_heartbeatMonitor_isSendingMissingHeartbeatFault(), false);

    // check that if there is a fault, it is detected
    E_fake_can_fault_state = false;
    A_fake_can_fault_state = true;

    ASSERT_EQ(app_heartbeatMonitor_isSendingMissingHeartbeatFault(), true);
}

TEST(HeartbeatMonitor, test_create)
{
    /* test heartbeat monitor create */

    // this hypothetical (fake) board only checks the A and E, and is the D
    bool heartbeats_to_check[HEARTBEAT_BOARD_COUNT];
    for (bool &board : heartbeats_to_check)
    {
        board = false;
    }

    heartbeats_to_check[A_HEARTBEAT_BOARD] = true;
    heartbeats_to_check[E_HEARTBEAT_BOARD] = true;

    // create and assert time fields work
    app_heartbeatMonitor_init(
        heartbeats_to_check, heartbeat_can_getters, heartbeat_can_updaters, &D_fakeCanHeartbeatSetter,
        fault_can_setters, fault_can_getters);
    HeartbeatMonitor *heartbeat_monitor = app_heartbeatMonitor_get();

    //    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, 0U);

    // check self heartbeat setter
    ASSERT_EQ(heartbeat_monitor->self_checkin, &D_fakeCanHeartbeatSetter);

    // assert status, checked in, to check, getters, updaters, and fault setters all set correctly
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[board]);
        ASSERT_TRUE(heartbeat_monitor->status[board]);
        ASSERT_EQ(heartbeat_monitor->is_watching_heartbeat_for[board], heartbeats_to_check[board]);

        ASSERT_EQ(heartbeat_monitor->getters[board], heartbeat_can_getters[board]);
        ASSERT_EQ(heartbeat_monitor->resetters[board], heartbeat_can_updaters[board]);
        ASSERT_EQ(heartbeat_monitor->fault_setters[board], fault_can_setters[board]);
    }
}

TEST(HeartbeatMonitor, test_broadcast_faults)
{
    /* test broadcast faults */
    // initialize monitor

    bool heartbeats_to_check[HEARTBEAT_BOARD_COUNT] = { [A_HEARTBEAT_BOARD] = true,
                                                        [B_HEARTBEAT_BOARD] = false,
                                                        [C_HEARTBEAT_BOARD] = false,
                                                        [D_HEARTBEAT_BOARD] = false,
                                                        [E_HEARTBEAT_BOARD] = true };

    app_heartbeatMonitor_init(
        heartbeats_to_check, heartbeat_can_getters, heartbeat_can_updaters, &D_fakeCanHeartbeatSetter,
        fault_can_setters, fault_can_getters);
    HeartbeatMonitor *heartbeat_monitor = app_heartbeatMonitor_get();

    // broadcast all good
    for (bool &status : heartbeat_monitor->status)
    {
        status = true;
    }

    app_heartbeatMonitor_broadcastFaults();

    ASSERT_FALSE(E_fake_can_fault_state);
    ASSERT_FALSE(A_fake_can_fault_state);

    // broadcast only A and E good (should be indistinguishable from all good)
    for (bool &statu : heartbeat_monitor->status)
    {
        statu = false;
    }
    heartbeat_monitor->status[E_HEARTBEAT_BOARD] = true;
    heartbeat_monitor->status[A_HEARTBEAT_BOARD] = true;
    app_heartbeatMonitor_broadcastFaults();

    ASSERT_FALSE(E_fake_can_fault_state);
    ASSERT_FALSE(A_fake_can_fault_state);

    // broadcast only A good (should trigger E fault)
    for (bool &status : heartbeat_monitor->status)
    {
        status = false;
    }
    heartbeat_monitor->status[E_HEARTBEAT_BOARD] = false;
    heartbeat_monitor->status[A_HEARTBEAT_BOARD] = true;
    app_heartbeatMonitor_broadcastFaults();

    ASSERT_TRUE(E_fake_can_fault_state);
    ASSERT_FALSE(A_fake_can_fault_state);

    // broadcast all bad
    for (bool &status : heartbeat_monitor->status)
    {
        status = false;
    }
    app_heartbeatMonitor_broadcastFaults();

    ASSERT_TRUE(E_fake_can_fault_state);
    ASSERT_TRUE(A_fake_can_fault_state);
}

TEST(HeartbeatMonitor, test_check_in_and_tick)
{
    /* test tick and check in functions*/
    fake_io_time_getCurrentMs_returns(0);

    // our hypothetical D board relies on A and E
    bool heartbeats_to_check[HEARTBEAT_BOARD_COUNT];
    for (bool &board : heartbeats_to_check)
    {
        board = false;
    }

    heartbeats_to_check[A_HEARTBEAT_BOARD] = true;
    heartbeats_to_check[E_HEARTBEAT_BOARD] = true;

    app_heartbeatMonitor_init(
        heartbeats_to_check, heartbeat_can_getters, heartbeat_can_updaters, &D_fakeCanHeartbeatSetter,
        fault_can_setters, fault_can_getters);
    HeartbeatMonitor *heartbeat_monitor = app_heartbeatMonitor_get();

    // assert nothing changed (0 ms)
    app_heartbeatMonitor_tick();
    //    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, 0U);

    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[A_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[B_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[C_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[D_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[E_HEARTBEAT_BOARD]);

    ASSERT_EQ(heartbeat_monitor->is_watching_heartbeat_for[A_HEARTBEAT_BOARD], heartbeats_to_check[A_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->is_watching_heartbeat_for[B_HEARTBEAT_BOARD], heartbeats_to_check[B_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->is_watching_heartbeat_for[C_HEARTBEAT_BOARD], heartbeats_to_check[C_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->is_watching_heartbeat_for[D_HEARTBEAT_BOARD], heartbeats_to_check[D_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->is_watching_heartbeat_for[E_HEARTBEAT_BOARD], heartbeats_to_check[E_HEARTBEAT_BOARD]);

    // confirm status good
    ASSERT_TRUE(heartbeat_monitor->status[A_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[B_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[C_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[D_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[E_HEARTBEAT_BOARD]);

    // check in A and E midway (150 ms)
    fake_io_time_getCurrentMs_returns(150);

    E_fake_can_heartbeat_state = true;
    A_fake_can_heartbeat_state = true;
    D_fake_can_heartbeat_state = false;

    app_heartbeatMonitor_checkIn();

    ASSERT_TRUE(D_fake_can_heartbeat_state);
    ASSERT_FALSE(E_fake_can_heartbeat_state);
    ASSERT_FALSE(A_fake_can_heartbeat_state);

    ASSERT_TRUE(heartbeat_monitor->heartbeats_checked_in[A_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->heartbeats_checked_in[E_HEARTBEAT_BOARD]);

    // progress to timeout period and verify all good (300 ms)
    fake_io_time_getCurrentMs_returns(300);
    app_heartbeatMonitor_tick();

    //    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, 300);

    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[A_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[B_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[C_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[D_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[E_HEARTBEAT_BOARD]);

    ASSERT_TRUE(heartbeat_monitor->status[A_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[B_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[C_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[D_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[E_HEARTBEAT_BOARD]);

    // check in E midway (450 ms)
    fake_io_time_getCurrentMs_returns(450);

    E_fake_can_heartbeat_state = true;
    A_fake_can_heartbeat_state = false;
    D_fake_can_heartbeat_state = false;

    app_heartbeatMonitor_checkIn();

    ASSERT_TRUE(D_fake_can_heartbeat_state);
    ASSERT_FALSE(E_fake_can_heartbeat_state);
    ASSERT_FALSE(A_fake_can_heartbeat_state);

    ASSERT_TRUE(heartbeat_monitor->heartbeats_checked_in[E_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[A_HEARTBEAT_BOARD]);

    // progress to timeout period and verify A missing (600 ms)
    fake_io_time_getCurrentMs_returns(600);
    app_heartbeatMonitor_tick();

    //    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, 600);

    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[A_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[B_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[C_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[D_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[E_HEARTBEAT_BOARD]);

    // A not checked in returns false
    ASSERT_FALSE(heartbeat_monitor->status[A_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[B_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[C_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[D_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[E_HEARTBEAT_BOARD]);
}