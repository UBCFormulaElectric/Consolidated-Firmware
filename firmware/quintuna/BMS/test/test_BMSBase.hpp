#pragma once
#include "ecuTestBase.hpp"
#include "test_fakes.h"

extern "C"
{
#include "jobs.h"
#include "states/app_states.h"
#include "app_canRx.h"
#include "io_canRx.h"
}

class BMSBaseTest : public EcuTestBase
{
    void board_setup() override
    {
        fakes::faultLatches::resetFaultLatch(&bms_ok_latch);
        fakes::faultLatches::resetFaultLatch(&imd_ok_latch);
        fakes::faultLatches::resetFaultLatch(&bspd_ok_latch);

        jobs_init();
    }
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

  public:
    struct StateMetadata
    {
        const State *state;
        BmsState     can_state;
        bool         requires_irs_negative_closed;
        bool         requires_fault;
    };
    std::array<StateMetadata, 10> state_metadata = { {
        { &init_state, BMS_INIT_STATE, false, false },
        { &fault_state, BMS_FAULT_STATE, false, true },
        { &precharge_state, BMS_PRECHARGE_DRIVE_STATE, true, false },
        { &drive_state, BMS_DRIVE_STATE, true, false },
        { &balancing_state, BMS_BALANCING_STATE, true, false },
        { &precharge_latch_state, BMS_PRECHARGE_LATCH_STATE, true, false },
        { &precharge_charge_state, BMS_PRECHARGE_CHARGE_STATE, true, false },
        { &charge_state, BMS_CHARGE_STATE, true, false },
        { &charge_init_state, BMS_CHARGE_INIT_STATE, true, false },
        { &charge_fault_state, BMS_CHARGE_FAULT_STATE, true, false },
    } };
};
