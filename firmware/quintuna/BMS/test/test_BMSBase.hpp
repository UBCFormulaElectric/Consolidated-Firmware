#pragma once
#include "ecuTestBase.hpp"

extern "C"
{
#include "jobs.h"
}

class BMSBaseTest : public EcuTestBase
{
    void board_setup() override { jobs_init(); }
    void board_teardown() override {}

    // Need to wrap the state machine tick in the LTC app mutex so don't use jobs.c for 100Hz.
    void tick_100hz() override
    {
        const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
        io_canTx_enableMode_can1(CAN1_MODE_DEBUG, debug_mode_enabled);

        app_stateMachine_tick100Hz();
        app_stateMachine_tickTransitionState();

        io_canTx_enqueue100HzMsgs();
    }

    void tick_1hz() override { jobs_run1Hz_tick(); }
};
