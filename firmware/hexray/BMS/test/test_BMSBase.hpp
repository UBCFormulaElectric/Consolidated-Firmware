#pragma once
#include "ecuTestBase.hpp"
#include "test_fakes.hpp"
#include "jobs.hpp"
#include "states/app_states.hpp"
#include "app_stateMachine.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "io_canRx.hpp"

#define ADBMS_CONVERSION_PERIOD_MS (1000U)

using namespace app::states;
using namespace app::can_utils;

class BMSBaseTest : public EcuTestBase
{
  protected:
    void board_setup() override
    {
        fakes::faultLatch::resetFaultLatch(&io::faultLatch::bms_ok_latch);
        fakes::faultLatch::resetFaultLatch(&io::faultLatch::imd_ok_latch);
        fakes::faultLatch::resetFaultLatch(&io::faultLatch::bspd_ok_latch);
        fakes::faultLatch::setCurrentStatus_resetCallCounts();
        fakes::charger::setConnectionStatus(ChargerConnectedType::CHARGER_DISCONNECTED);

        app::can_rx::VC_State_update(VCState::VC_INIT_STATE);
        app::can_tx::BMS_Fault_TESTFAULT_set(false);

        // TODO: Change back to using constants once segments is added
        // fakes::segments::setPackVoltageEvenly(3.8f * NUM_SEGMENTS * CELLS_PER_SEGMENT);
        fakes::segments::setPackVoltageEvenly(3.8f * 10 * 14);
        fakes::segments::SetAuxRegs(15.0f); // Approx. 25C

        fakes::ts::setVoltage(0.0f);

        jobs_init();
        // jobs_initAdbmsVoltages();
        // jobs_initAdbmsTemps();
        // jobs_initAdbmsDiagnostics();

        register_task(jobs_run1Hz_tick, 1000);
        register_task(jobs_run100Hz_tick, 10);
        register_task(jobs_run1kHz_tick, 1);
        // register_task(jobs_runAdbmsVoltages, 500);
        // register_task(jobs_runAdbmsDiagnostics, 500);
        // register_task(jobs_runAdbmsTemperatures, 500);
        // Allow time for all jobs to run at least once for things like voltage arrays to update
        LetTimePass(1000);
    }
    void board_teardown() override {}

    void SetInitialState(const app::State *const initial_state)
    {
        app::StateMachine::init(initial_state);
        ASSERT_EQ(initial_state, app::StateMachine::get_current_state());
    }

    std::vector<const app::State *> GetAllStates()
    {
        return std::vector{ &init_state,        &precharge_drive_state, &precharge_charge_state, &precharge_latch_state,
                            &drive_state,       &charge_state,          &balancing_state,        &fault_state,
                            &charge_init_state, &charge_fault_state };
    }
    void SetImdCondition(const ImdConditionName condition_name)
    {
        const std::map<app::can_utils::ImdConditionName, float> mapping{
            { ImdConditionName::IMD_CONDITION_SHORT_CIRCUIT, 0.0f },
            { ImdConditionName::IMD_CONDITION_NORMAL, 10.0f },
            { ImdConditionName::IMD_CONDITION_UNDERVOLTAGE_DETECTED, 20.0f },
            { ImdConditionName::IMD_CONDITION_SST, 30.0f },
            { ImdConditionName::IMD_CONDITION_DEVICE_ERROR, 40.0f },
            { ImdConditionName::IMD_CONDITION_GROUND_FAULT, 50.0f }
        };
        fakes::imd::setFrequency(mapping.at(condition_name));
    }
};

struct StateMetadata
{
    const app::State *state;
    BmsState          can_state;
    bool              requires_irs_negative_closed;
    bool              requires_fault;
};

constexpr inline std::array<StateMetadata, 10> state_metadata = { {
    { &init_state, BmsState::BMS_INIT_STATE, false, false },
    { &fault_state, BmsState::BMS_FAULT_STATE, false, true },
    { &precharge_drive_state, BmsState::BMS_PRECHARGE_DRIVE_STATE, true, false },
    { &drive_state, BmsState::BMS_DRIVE_STATE, true, false },
    { &balancing_state, BmsState::BMS_BALANCING_STATE, true, false },
    { &precharge_latch_state, BmsState::BMS_PRECHARGE_LATCH_STATE, true, false },
    { &precharge_charge_state, BmsState::BMS_PRECHARGE_CHARGE_STATE, true, false },
    { &charge_state, BmsState::BMS_CHARGE_STATE, true, false },
    { &charge_init_state, BmsState::BMS_CHARGE_INIT_STATE, true, false },
    { &charge_fault_state, BmsState::BMS_CHARGE_FAULT_STATE, true, false },
} };
