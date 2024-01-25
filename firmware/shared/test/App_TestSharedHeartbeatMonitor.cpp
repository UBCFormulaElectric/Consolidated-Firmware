#include <stdbool.h>
#include "Test_Shared.h"

extern "C"
{
#include "App_Timer.h"
#include "App_SharedHeartbeatMonitor.h"
}

// fake can states
bool DIM_fakeCanFaultState     = false;
bool BMS_fakeCanFaultState     = false;
bool DIM_fakeCanHeartbeatState = false;
bool BMS_fakeCanHeartbeatState = false;
bool FSM_fakeCanHeartbeatState = false;

// fake can methods
void DIM_fakeCanHeartbeatUpdater(bool val)
{
    DIM_fakeCanHeartbeatState = val;
}

void BMS_fakeCanHeartbeatUpdater(bool val)
{
    BMS_fakeCanHeartbeatState = val;
}

void FSM_fakeCanHeartbeatSetter(bool val)
{
    FSM_fakeCanHeartbeatState = val;
}

void DIM_fakeCanFaultSetter(bool val)
{
    DIM_fakeCanFaultState = val;
}

void BMS_fakeCanFaultSetter(bool val)
{
    BMS_fakeCanFaultState = val;
}

bool DIM_fakeCanFaultGetter(void)
{
    return DIM_fakeCanFaultState;
}

bool BMS_fakeCanFaultGetter(void)
{
    return BMS_fakeCanFaultState;
}

bool DIM_fakeCanHeartbeatGetter(void)
{
    return DIM_fakeCanHeartbeatState;
}

bool BMS_fakeCanHeartbeatGetter(void)
{
    return BMS_fakeCanHeartbeatState;
}

// callback config structs to be passed in to methods later on
void (*faultCanSetters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = &BMS_fakeCanFaultSetter,
                                                         [DCM_HEARTBEAT_BOARD] = NULL,
                                                         [PDM_HEARTBEAT_BOARD] = NULL,
                                                         [FSM_HEARTBEAT_BOARD] = NULL,
                                                         [DIM_HEARTBEAT_BOARD] = &DIM_fakeCanFaultSetter };

void (*heartbeatCanUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = &BMS_fakeCanHeartbeatUpdater,
                                                              [DCM_HEARTBEAT_BOARD] = NULL,
                                                              [PDM_HEARTBEAT_BOARD] = NULL,
                                                              [FSM_HEARTBEAT_BOARD] = NULL,
                                                              [DIM_HEARTBEAT_BOARD] = &DIM_fakeCanHeartbeatUpdater };

bool (*heartbeatCanGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &BMS_fakeCanHeartbeatGetter,
                                                         [DCM_HEARTBEAT_BOARD] = NULL,
                                                         [PDM_HEARTBEAT_BOARD] = NULL,
                                                         [FSM_HEARTBEAT_BOARD] = NULL,
                                                         [DIM_HEARTBEAT_BOARD] = &DIM_fakeCanHeartbeatGetter };

bool (*faultCanGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &BMS_fakeCanFaultGetter,
                                                     [DCM_HEARTBEAT_BOARD] = NULL,
                                                     [PDM_HEARTBEAT_BOARD] = NULL,
                                                     [FSM_HEARTBEAT_BOARD] = NULL,
                                                     [DIM_HEARTBEAT_BOARD] = &DIM_fakeCanFaultGetter };

TEST(SharedHeartbeatMonitorTest, test_check_faults)
{
    /* test check faults */
    // this hypothetical (fake) board only checks the BMS and DIM, and is the FSM
    bool heartbeatsToCheck[HEARTBEAT_BOARD_COUNT];
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeatsToCheck[board] = false;
    }

    heartbeatsToCheck[BMS_HEARTBEAT_BOARD] = true;
    heartbeatsToCheck[DIM_HEARTBEAT_BOARD] = true;

    struct HeartbeatMonitor * heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        App_Timer_GetCurrentTimeMS, 100, heartbeatsToCheck, heartbeatCanGetters, heartbeatCanUpdaters,
        &FSM_fakeCanHeartbeatSetter, faultCanSetters, faultCanGetters);

    // check that there are no faults
    DIM_fakeCanFaultState = false;
    BMS_fakeCanFaultState = false;

    ASSERT_EQ(App_SharedHeartbeatMonitor_CheckFaults(heartbeat_monitor), false);

    // check that if there is a fault, it is detected
    DIM_fakeCanFaultState = false;
    BMS_fakeCanFaultState = true;

    ASSERT_EQ(App_SharedHeartbeatMonitor_CheckFaults(heartbeat_monitor), true);
}

TEST(SharedHeartbeatMonitorTest, test_create)
{
    /* test heartbeat monitor create */

    // this hypothetical (fake) board only checks the BMS and DIM, and is the FSM
    bool heartbeatsToCheck[HEARTBEAT_BOARD_COUNT];
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeatsToCheck[board] = false;
    }

    heartbeatsToCheck[BMS_HEARTBEAT_BOARD] = true;
    heartbeatsToCheck[DIM_HEARTBEAT_BOARD] = true;

    // create and assert time fields work
    struct HeartbeatMonitor * heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        App_Timer_GetCurrentTimeMS, 100, heartbeatsToCheck, heartbeatCanGetters, heartbeatCanUpdaters,
        &FSM_fakeCanHeartbeatSetter, faultCanSetters, faultCanGetters);

    ASSERT_EQ(heartbeat_monitor->get_current_ms, App_Timer_GetCurrentTimeMS);
    ASSERT_EQ(heartbeat_monitor->timeout_period_ms, 100);
    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, 0U);

    // check self heartbeat setter
    ASSERT_EQ(heartbeat_monitor->setter, &FSM_fakeCanHeartbeatSetter);

    // assert status, checked in, to check, getters, updaters, and fault setters all set correctly
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[board]);
        ASSERT_TRUE(heartbeat_monitor->status[board]);
        ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[board], heartbeatsToCheck[board]);

        ASSERT_EQ(heartbeat_monitor->getters[board], heartbeatCanGetters[board]);
        ASSERT_EQ(heartbeat_monitor->updaters[board], heartbeatCanUpdaters[board]);
        ASSERT_EQ(heartbeat_monitor->fault_setters[board], faultCanSetters[board]);
    }
}

TEST(SharedHeartbeatMonitorTest, test_broadcast_faults)
{
    /* test broadcast faults */
    // initialize monitor

    bool heartbeatsToCheck[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = true,
                                                      [DCM_HEARTBEAT_BOARD] = false,
                                                      [PDM_HEARTBEAT_BOARD] = false,
                                                      [FSM_HEARTBEAT_BOARD] = false,
                                                      [DIM_HEARTBEAT_BOARD] = true };

    struct HeartbeatMonitor * heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        App_Timer_GetCurrentTimeMS, 100, heartbeatsToCheck, heartbeatCanGetters, heartbeatCanUpdaters,
        &FSM_fakeCanHeartbeatSetter, faultCanSetters, faultCanGetters);

    // broadcast all good
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeat_monitor->status[board] = true;
    }

    App_SharedHeartbeatMonitor_BroadcastFaults(heartbeat_monitor);

    ASSERT_FALSE(DIM_fakeCanFaultState);
    ASSERT_FALSE(BMS_fakeCanFaultState);

    // broadcast only bms and dim good (should be indistinguishable from all good)
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeat_monitor->status[board] = false;
    }
    heartbeat_monitor->status[DIM_HEARTBEAT_BOARD] = true;
    heartbeat_monitor->status[BMS_HEARTBEAT_BOARD] = true;
    App_SharedHeartbeatMonitor_BroadcastFaults(heartbeat_monitor);

    ASSERT_FALSE(DIM_fakeCanFaultState);
    ASSERT_FALSE(BMS_fakeCanFaultState);

    // broadcast only bms good (should trigger dim fault)
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeat_monitor->status[board] = false;
    }
    heartbeat_monitor->status[DIM_HEARTBEAT_BOARD] = false;
    heartbeat_monitor->status[BMS_HEARTBEAT_BOARD] = true;
    App_SharedHeartbeatMonitor_BroadcastFaults(heartbeat_monitor);

    ASSERT_TRUE(DIM_fakeCanFaultState);
    ASSERT_FALSE(BMS_fakeCanFaultState);

    // broadcast all bad
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeat_monitor->status[board] = false;
    }
    App_SharedHeartbeatMonitor_BroadcastFaults(heartbeat_monitor);

    ASSERT_TRUE(DIM_fakeCanFaultState);
    ASSERT_TRUE(BMS_fakeCanFaultState);
}

TEST(SharedHeartbeatMonitorTest, test_check_in_and_tick)
{
    /* test tick and check in functions*/
    App_Timer_SetCurrentTimeMS(0);
    uint32_t curr_time = App_Timer_GetCurrentTimeMS();

    // our hypothetical FSM board relies on BMS and DIM
    bool heartbeatsToCheck[HEARTBEAT_BOARD_COUNT];
    for (int board = 0; board < HEARTBEAT_BOARD_COUNT; board++)
    {
        heartbeatsToCheck[board] = false;
    }

    heartbeatsToCheck[BMS_HEARTBEAT_BOARD] = true;
    heartbeatsToCheck[DIM_HEARTBEAT_BOARD] = true;

    struct HeartbeatMonitor * heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        App_Timer_GetCurrentTimeMS, 100, heartbeatsToCheck, heartbeatCanGetters, heartbeatCanUpdaters,
        &FSM_fakeCanHeartbeatSetter, faultCanSetters, faultCanGetters);

    // assert nothing changed (0 ms)
    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
    ASSERT_EQ(heartbeat_monitor->get_current_ms, App_Timer_GetCurrentTimeMS);
    ASSERT_EQ(heartbeat_monitor->timeout_period_ms, 100);
    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, 0U);

    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[BMS_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[FSM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[DIM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[PDM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[DCM_HEARTBEAT_BOARD]);

    ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[BMS_HEARTBEAT_BOARD], heartbeatsToCheck[BMS_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[FSM_HEARTBEAT_BOARD], heartbeatsToCheck[FSM_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[DIM_HEARTBEAT_BOARD], heartbeatsToCheck[DIM_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[PDM_HEARTBEAT_BOARD], heartbeatsToCheck[PDM_HEARTBEAT_BOARD]);
    ASSERT_EQ(heartbeat_monitor->heartbeats_to_check[DCM_HEARTBEAT_BOARD], heartbeatsToCheck[DCM_HEARTBEAT_BOARD]);

    // confirm status good
    ASSERT_TRUE(heartbeat_monitor->status[BMS_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[FSM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[DIM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[PDM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[DCM_HEARTBEAT_BOARD]);

    // check in BMS and DIM (50 ms)
    App_Timer_SetCurrentTimeMS(50);
    curr_time = App_Timer_GetCurrentTimeMS();

    DIM_fakeCanHeartbeatState = true;
    BMS_fakeCanHeartbeatState = true;
    FSM_fakeCanHeartbeatState = false;

    App_SharedHeartbeatMonitor_CheckIn(heartbeat_monitor);

    ASSERT_TRUE(FSM_fakeCanHeartbeatState);
    ASSERT_FALSE(DIM_fakeCanHeartbeatState);
    ASSERT_FALSE(BMS_fakeCanHeartbeatState);

    ASSERT_TRUE(heartbeat_monitor->heartbeats_checked_in[BMS_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->heartbeats_checked_in[DIM_HEARTBEAT_BOARD]);

    // progress to timeout period and verify all good (100 ms)
    App_Timer_SetCurrentTimeMS(100);
    curr_time = App_Timer_GetCurrentTimeMS();
    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);

    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, curr_time);

    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[BMS_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[FSM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[DIM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[PDM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[DCM_HEARTBEAT_BOARD]);

    ASSERT_TRUE(heartbeat_monitor->status[BMS_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[FSM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[DIM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[PDM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[DCM_HEARTBEAT_BOARD]);

    // check in DIM (150ms)
    App_Timer_SetCurrentTimeMS(150);
    curr_time = App_Timer_GetCurrentTimeMS();

    DIM_fakeCanHeartbeatState = true;
    BMS_fakeCanHeartbeatState = false;
    FSM_fakeCanHeartbeatState = false;

    App_SharedHeartbeatMonitor_CheckIn(heartbeat_monitor);

    ASSERT_TRUE(FSM_fakeCanHeartbeatState);
    ASSERT_FALSE(DIM_fakeCanHeartbeatState);
    ASSERT_FALSE(BMS_fakeCanHeartbeatState);

    ASSERT_TRUE(heartbeat_monitor->heartbeats_checked_in[DIM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[BMS_HEARTBEAT_BOARD]);

    // progress to timeout period and verify bms missing (200 ms)
    App_Timer_SetCurrentTimeMS(200);
    curr_time = App_Timer_GetCurrentTimeMS();
    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);

    ASSERT_EQ(heartbeat_monitor->previous_timeout_ms, curr_time);

    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[BMS_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[FSM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[DIM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[PDM_HEARTBEAT_BOARD]);
    ASSERT_FALSE(heartbeat_monitor->heartbeats_checked_in[DCM_HEARTBEAT_BOARD]);

    // BMS not checked in returns false
    ASSERT_FALSE(heartbeat_monitor->status[BMS_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[FSM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[DIM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[PDM_HEARTBEAT_BOARD]);
    ASSERT_TRUE(heartbeat_monitor->status[DCM_HEARTBEAT_BOARD]);
}