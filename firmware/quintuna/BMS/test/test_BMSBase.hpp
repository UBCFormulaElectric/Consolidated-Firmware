#pragma once
#include "ecuTestBase.hpp"

#include "test_fakes.h"

extern "C"
{
#include "jobs.h"
#include "states/app_initState.h"
#include "states/app_faultState.h"
#include "states/app_chargeState.h"
#include "states/app_prechargeLatchState.h"
#include "states/app_prechargeDriveState.h"
#include "states/app_prechargeChargeState.h"
#include "states/app_driveState.h"
#include "states/app_balancingState.h"
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

        fakes::segments::setPackVoltageEvenly(550.0f);

        fake_io_time_getCurrentMs_reset();

        jobs_init();
    }
    void board_teardown() override {}
    void tick_100hz() override
    {
        // jobs_runLTCTemperatures();
        // jobs_runLTCVoltages();
        // jobs_runLTCDiagnostics();

        // jobs_run100Hz_tick();
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
    std::array<StateMetadata, 8> state_metadata = { {
        { app_initState_get(), BMS_INIT_STATE, false, false },
        { app_faultState_get(), BMS_FAULT_STATE, false, true },
        { app_prechargeDriveState_get(), BMS_PRECHARGE_DRIVE_STATE, true, false },
        { app_driveState_get(), BMS_DRIVE_STATE, true, false },
        { app_balancingState_get(), BMS_BALANCING_STATE, true, false },
        { app_prechargeLatchState_get(), BMS_PRECHARGE_LATCH_STATE, true, false },
        { app_prechargeChargeState_get(), BMS_PRECHARGE_CHARGE_STATE, true, false },
        { app_chargeState_get(), BMS_CHARGE_STATE, true, false },
    } };
};
