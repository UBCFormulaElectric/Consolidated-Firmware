#pragma once
#include "ecuTestBase.hpp"

#include "test_fakes.h"
#include "fake_io_irs.hpp"
#include "fake_io_tractiveSystem.hpp"
#include "fake_io_tractiveSystem.hpp"
#include "fake_io_imd.hpp"
#include "fake_io_faultLatch.hpp"

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
#include "jobs.h"
#include "app_stateMachine.h"
#include "app_segments.h"
#include "app_imd.h"
}

#define LTC_CONVERSION_PERIOD_MS (1000U)

class BMSBaseTest : public EcuTestBase
{
  protected:
    void board_setup() override
    {
        fakes::segments::SetPackVoltageEvenly(3.8 * NUM_SEGMENTS * CELLS_PER_SEGMENT);
        fakes::segments::SetAuxRegs(1.5f); // Approx. 25C

        jobs_init();
    }
    void board_teardown() override {}
    void tick_100hz() override
    {
        app_stateMachine_tick100Hz();
        app_stateMachine_tickTransitionState();
    }
    void tick_1hz() override
    {
        jobs_run1Hz_tick();

        // These run in a separate task on the micro but at 1Hz.

        app_segments_runVoltageConversion();
        app_segments_broadcastCellVoltages();
        app_segments_broadcastVoltageStats();

        app_segments_runAuxConversion();
        app_segments_broadcastTempsVRef();
        app_segments_broadcastTempStats();
    }

    void TearDown() override
    {
        fake_io_time_getCurrentMs_reset();
        fake_io_tractiveSystem_getVoltage_reset();
        fake_io_irs_isNegativeClosed_reset();
        fake_io_irs_isPositiveClosed_reset();
        fake_io_imd_getFrequency_reset();
        fake_io_imd_getDutyCycle_reset();
        fake_io_faultLatch_getCurrentStatus_reset();
        fake_io_faultLatch_getLatchedStatus_reset();
        fake_io_tractiveSystem_getCurrentHighResolution_reset();
        fake_io_tractiveSystem_getCurrentLowResolution_reset();
        fake_io_irs_closePositive_reset();
        fake_io_faultLatch_setCurrentStatus_reset();
    }

    void SetInitialState(const State *const initial_state)
    {
        app_stateMachine_init(initial_state);
        ASSERT_EQ(initial_state, app_stateMachine_getCurrentState());
    }

    std::vector<const State *> GetAllStates(void)
    {
        return std::vector<const State *>{
            app_initState_get(),           app_prechargeDriveState_get(), app_prechargeChargeState_get(),
            app_prechargeLatchState_get(), app_driveState_get(),          app_chargeState_get(),
            app_balancingState_get(),      app_faultState_get(),
        };
    }

    void SetImdCondition(const ImdConditionName condition_name)
    {
        const std::map<ImdConditionName, float> mapping{
            { IMD_CONDITION_SHORT_CIRCUIT, 0.0f },          { IMD_CONDITION_NORMAL, 10.0f },
            { IMD_CONDITION_UNDERVOLTAGE_DETECTED, 20.0f }, { IMD_CONDITION_SST, 30.0f },
            { IMD_CONDITION_DEVICE_ERROR, 40.0f },          { IMD_CONDITION_GROUND_FAULT, 50.0f }
        };

        fake_io_imd_getFrequency_returns(mapping.at(condition_name));
        ASSERT_EQ(condition_name, app_imd_getCondition().name);
    }
};

inline const FaultLatch bms_ok_latch  = {};
inline const FaultLatch imd_ok_latch  = {};
inline const FaultLatch bspd_ok_latch = {};
