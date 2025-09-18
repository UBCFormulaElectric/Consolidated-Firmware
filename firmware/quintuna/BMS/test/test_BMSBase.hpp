#pragma once
#include "ecuTestBase.hpp"
#include "test_fakes.hpp"

extern "C"
{
#include "jobs.h"
#include "states/app_states.h"
#include "app_canRx.h"
#include "io_canRx.h"
}

#define LTC_CONVERSION_PERIOD_MS (1000U)

class BMSBaseTest : public EcuTestBase
{
  protected:
    void board_setup() override
    {
        fakes::faultLatches::resetFaultLatch(&bms_ok_latch);
        fakes::faultLatches::resetFaultLatch(&imd_ok_latch);
        fakes::faultLatches::resetFaultLatch(&bspd_ok_latch);

        fakes::segments::setPackVoltageEvenly(3.8 * NUM_SEGMENTS * CELLS_PER_SEGMENT);
        fakes::segments::SetAuxRegs(1.5f); // Approx. 25C

        jobs_init();
        jobs_initLTCVoltages();
        jobs_initLTCTemps();
        jobs_initLTCDiagnostics();

        register_task(jobs_run1Hz_tick, 1000);
        register_task(jobs_run100Hz_tick, 10);
        register_task(jobs_run1kHz_tick, 1);
        register_task(jobs_runLTCVoltages, 500);
        register_task(jobs_runLTCDiagnostics, 500);
        register_task(jobs_runLTCTemperatures, 10000);
    }
    void board_teardown() override {}

    static void tick_100hz()
    {
        jobs_runLTCTemperatures();
        jobs_runLTCVoltages();
        jobs_runLTCDiagnostics();

        jobs_run100Hz_tick();
    }
    static void tick_1hz()
    {
        jobs_run1Hz_tick();

        // These run in a separate task on the micro but at 1Hz.

        // app_segments_runVoltageConversion();
        // app_segments_broadcastCellVoltages();
        // app_segments_broadcastVoltageStats();
        // app_segments_runAuxConversion();
        // app_segments_broadcastTempsVRef();
        // app_segments_broadcastTempStats();
    }

    void SetInitialState(const State *const initial_state)
    {
        app_stateMachine_init(initial_state);
        ASSERT_EQ(initial_state, app_stateMachine_getCurrentState());
    }

    std::vector<const State *> GetAllStates()
    {
        return std::vector{ &init_state,        &precharge_drive_state, &precharge_charge_state, &precharge_latch_state,
                            &drive_state,       &charge_state,          &balancing_state,        &fault_state,
                            &charge_init_state, &charge_fault_state };
    }
    void SetImdCondition(const ImdConditionName condition_name)
    {
        const std::map<ImdConditionName, float> mapping{
            { IMD_CONDITION_SHORT_CIRCUIT, 0.0f },          { IMD_CONDITION_NORMAL, 10.0f },
            { IMD_CONDITION_UNDERVOLTAGE_DETECTED, 20.0f }, { IMD_CONDITION_SST, 30.0f },
            { IMD_CONDITION_DEVICE_ERROR, 40.0f },          { IMD_CONDITION_GROUND_FAULT, 50.0f }
        };
        fakes::imd::setFrequency(mapping.at(condition_name));
    }
};

struct StateMetadata
{
    const State *state;
    BmsState     can_state;
    bool         requires_irs_negative_closed;
    bool         requires_fault;
};

constexpr inline std::array<StateMetadata, 10> state_metadata = { {
    { &init_state, BMS_INIT_STATE, false, false },
    { &fault_state, BMS_FAULT_STATE, false, true },
    { &precharge_drive_state, BMS_PRECHARGE_DRIVE_STATE, true, false },
    { &drive_state, BMS_DRIVE_STATE, true, false },
    { &balancing_state, BMS_BALANCING_STATE, true, false },
    { &precharge_latch_state, BMS_PRECHARGE_LATCH_STATE, true, false },
    { &precharge_charge_state, BMS_PRECHARGE_CHARGE_STATE, true, false },
    { &charge_state, BMS_CHARGE_STATE, true, false },
    { &charge_init_state, BMS_CHARGE_INIT_STATE, true, false },
    { &charge_fault_state, BMS_CHARGE_FAULT_STATE, true, false },
} };
