#pragma once
#include "ecuTestBase.hpp"

extern "C"
{
#include "jobs.h"
#include "states/app_states.h"
#include "app_canTx.h"
}

class BMSBaseTest : public EcuTestBase
{
    void board_setup() override { jobs_init(); }
    void board_teardown() override {}
    void tick_100hz() override { jobs_run100Hz_tick(); }
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