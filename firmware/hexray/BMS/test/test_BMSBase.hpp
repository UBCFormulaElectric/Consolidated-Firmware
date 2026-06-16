#pragma once
#include "ecuTestBase.hpp"
#include "test_fakes.hpp"
#include "jobs.hpp"
#include "app_states.hpp"
#include "app_stateMachine.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"
#include "app_canUtils.hpp"
#include "io_canRx.hpp"
#include "io_canTx.hpp"
#include "app_segments.hpp"

#define ADBMS_CONVERSION_PERIOD_MS (1000U)

class BMSBaseTest : public EcuTestBase
{
  protected:
    void board_setup() override
    {
        SetInitialState(&app::states::init_state);
        fakes::faultLatch::resetFaultLatch(&bms_ok_latch);
        fakes::faultLatch::resetFaultLatch(&imd_ok_latch);
        fakes::faultLatch::resetFaultLatch(&bspd_ok_latch);
        fakes::faultLatch::setCurrentStatus_resetCallCounts();
        fakes::charger::setConnectionStatus(app::can_utils::ChargerConnectedType::CHARGER_DISCONNECTED);
        ResetCanAlerts();

        app::can_rx::VC_State_update(app::can_utils::VCState::VC_INIT_STATE);
        app::can_rx::Debug_CellBalancing_Request_update(false);

        fakes::adbms::setPackVoltageEvenly(3.8f * NUM_SEGMENTS * CELLS_PER_SEGMENT);
        fakes::adbms::setHealthyConfigs();
        fakes::ts::setVoltage(0.0f);

        jobs_init();

        register_task(jobs_run1Hz_tick, 1000);
        register_task(jobs_run100Hz_tick, 10);
        register_task(jobs_run1kHz_tick, 1);
        register_task(jobs_runAdbmsVoltages_tick, 500);
        register_task(jobs_runAdbmsConfigs_tick, 100);
        register_task(jobs_runAdbmsAux_tick, 1200);
        register_task(jobs_runAdbmsCellOwc_tick, 1000);

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
        return std::vector{ &app::states::init_state,
                            &app::states::precharge_drive_state,
                            &app::states::precharge_charge_state,
                            &app::states::precharge_latch_state,
                            &app::states::drive_state,
                            &app::states::charge_state,
                            &app::states::balancing_state,
                            &app::states::fault_state };
    }

    void ResetCanAlerts()
    {
        app::can_tx::BMS_Fault_CellOpenWire_set(false);
        app::can_tx::BMS_Fault_CellOvervoltage_set(false);
        app::can_tx::BMS_Fault_CellUndervoltage_set(false);
        app::can_tx::BMS_Fault_CellOvertemp_set(false);
        app::can_tx::BMS_Fault_HealthError_set(false);

        app::can_tx::BMS_Fault_CellOvervoltage_Count_set(0);
        app::can_tx::BMS_Fault_CellOpenWire_Count_set(0);
        app::can_tx::BMS_Fault_CellUndervoltage_Count_set(0);
        app::can_tx::BMS_Fault_CellOvertemp_Count_set(0);
        app::can_tx::BMS_Fault_HealthError_Count_set(0);
    }

    void SetImdCondition(const app::can_utils::ImdConditionName condition_name)
    {
        const std::map<app::can_utils::ImdConditionName, float> mapping{
            { app::can_utils::ImdConditionName::IMD_CONDITION_SHORT_CIRCUIT, 0.0f },
            { app::can_utils::ImdConditionName::IMD_CONDITION_NORMAL, 10.0f },
            { app::can_utils::ImdConditionName::IMD_CONDITION_UNDERVOLTAGE_DETECTED, 20.0f },
            { app::can_utils::ImdConditionName::IMD_CONDITION_SST, 30.0f },
            { app::can_utils::ImdConditionName::IMD_CONDITION_DEVICE_ERROR, 40.0f },
            { app::can_utils::ImdConditionName::IMD_CONDITION_GROUND_FAULT, 50.0f }
        };
        fakes::imd::setFrequency(mapping.at(condition_name));
    }
};

struct StateMetadata
{
    const app::State        *state;
    app::can_utils::BmsState can_state;
    bool                     requires_irs_negative_closed;
    bool                     requires_fault;
};

constexpr inline std::array<StateMetadata, 10> state_metadata = {
    { { &app::states::init_state, app::can_utils::BmsState::BMS_INIT_STATE, false, false },
      { &app::states::fault_state, app::can_utils::BmsState::BMS_FAULT_STATE, false, true },
      { &app::states::precharge_drive_state, app::can_utils::BmsState::BMS_PRECHARGE_DRIVE_STATE, true, false },
      { &app::states::drive_state, app::can_utils::BmsState::BMS_DRIVE_STATE, true, false },
      { &app::states::balancing_state, app::can_utils::BmsState::BMS_BALANCING_STATE, true, false },
      { &app::states::precharge_latch_state, app::can_utils::BmsState::BMS_PRECHARGE_LATCH_STATE, true, false },
      { &app::states::precharge_charge_state, app::can_utils::BmsState::BMS_PRECHARGE_CHARGE_STATE, true, false },
      { &app::states::charge_state, app::can_utils::BmsState::BMS_CHARGE_STATE, true, false } }
};
