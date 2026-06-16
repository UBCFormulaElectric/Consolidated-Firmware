#include "test_fakes.hpp"
#include "test_BMSBase.hpp"
#include "app_states.hpp"
#include "app_precharge.hpp"
#include "app_irs.hpp"
#include "io_time.hpp"
#include "io_irs.hpp"
#include "app_canAlerts.hpp"
#include <iostream>

class BmsStateMachineTest : public BMSBaseTest
{
};
static constexpr int target_voltage =
    static_cast<int>(static_cast<float>((NUM_SEGMENTS * CELLS_PER_SEGMENT)) * 3.8f); // V
static constexpr int undervoltage            = 200.0f;                               // V
static constexpr int tolerance_time          = 500;                                  // ms
static constexpr int precharge_completion_ms = static_cast<int>(app::precharge::PRECHARGE_COMPLETION_MS_F);
static constexpr int latch_timeout           = app::precharge::PRECHARGE_LATCH_TIMEOUT_MS + 100; // ms

// Init-PrechargeDrive tests
TEST_F(BmsStateMachineTest, enter_precharge_drive)
{
    ASSERT_STATE_EQ(app::states::init_state);

    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    app::can_rx::VC_State_update(app::can_utils::VCState::VC_BMS_ON_STATE);
    app::can_rx::Debug_StartCharging_update(false);
    fakes::charger::setConnectionStatus(app::can_utils::ChargerConnectedType::CHARGER_DISCONNECTED);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::precharge_drive_state);
    ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    ASSERT_EQ(io::irs::positiveState(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
    ASSERT_EQ(io::irs::negativeState(), app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_negative_open)
{
    ASSERT_STATE_EQ(app::states::init_state);
    app::can_rx::VC_State_update(app::can_utils::VCState::VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::init_state);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_vc_state_not_bms_on)
{
    ASSERT_STATE_EQ(app::states::init_state);
    app::can_rx::VC_State_update(app::can_utils::VCState::VC_INIT_STATE);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::init_state);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_charger_connected)
{
    ASSERT_STATE_EQ(app::states::init_state);
    app::can_rx::VC_State_update(app::can_utils::VCState::VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::charger::setConnectionStatus(app::can_utils::ChargerConnectedType::CHARGER_CONNECTED_WALL);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::init_state);
}

// PrechargeDrive-Drive tests

TEST_F(BmsStateMachineTest, precharge_drive_success)
{
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(0.0f);
    app::can_rx::VC_State_update(app::can_utils::VCState::VC_BMS_ON_STATE);
    LetTimePass(20);

    for (int i = 0; i < precharge_completion_ms; i += 10)
    {
        ASSERT_STATE_EQ(app::states::precharge_drive_state);
        ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
        ASSERT_EQ(app::can_tx::BMS_PrechargeRelay_get(), app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
        LetTimePass(10);
    }
    fakes::ts::setVoltage(target_voltage);
    LetTimePass(10);
    ASSERT_STATE_EQ(app::states::drive_state);
    ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
    ASSERT_EQ(app::can_tx::BMS_PrechargeRelay_get(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
}

TEST_F(BmsStateMachineTest, precharge_rises_too_slowly_then_latches_after_max_retries)
{
    fakes::adbms::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    app::can_rx::VC_State_update(app::can_utils::VCState::VC_BMS_ON_STATE);
    LetTimePass(20);
    // Set voltage to undervoltage level after entering a precharge state
    fakes::ts::setVoltage(undervoltage);

    for (uint8_t retry = 0; retry < app::precharge::MAX_PRECHARGE_ATTEMPTS; retry++)
    {
        unsigned int closed_time = 0;
        while (io::irs::prechargeState() == app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED &&
               closed_time < app::precharge::PRECHARGE_COMPLETION_UPPER_BOUND + tolerance_time)
        {
            ASSERT_STATE_EQ(app::states::precharge_drive_state);
            ASSERT_EQ(app::can_tx::BMS_PrechargeRelay_get(), app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
            LetTimePass(10);
            closed_time += 10;
        }
        ASSERT_NEAR(closed_time, app::precharge::PRECHARGE_COMPLETION_UPPER_BOUND, 100)
            << "Precharge relay stayed closed for " << closed_time << "ms (expected ~"
            << app::precharge::PRECHARGE_COMPLETION_UPPER_BOUND << "ms)"
            << ", time=" << io::time::getCurrentMs();

        // Set voltage back to 0V since contactors opened.
        ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
        fakes::ts::setVoltage(0.0f);

        if (retry == app::precharge::MAX_PRECHARGE_ATTEMPTS - 1)
            break;

        unsigned int open_time = 0;
        while (io::irs::prechargeState() == app::can_utils::ContactorState::CONTACTOR_STATE_OPEN &&
               open_time < app::precharge::PRECHARGE_COOLDOWN_MS + tolerance_time)
        {
            ASSERT_EQ(app::can_tx::BMS_PrechargeRelay_get(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
            LetTimePass(10);
            open_time += 10;
        }
        ASSERT_NEAR(open_time, app::precharge::PRECHARGE_COOLDOWN_MS, 100)
            << "Precharge relay stayed open for " << open_time << "ms (expected ~"
            << app::precharge::PRECHARGE_COOLDOWN_MS << "ms)"
            << ", time=" << io::time::getCurrentMs();
    }
    ASSERT_STATE_EQ(app::states::precharge_latch_state);
}

TEST_F(BmsStateMachineTest, precharge_rises_too_quickly_then_latches_after_max_retries)
{
    fakes::adbms::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    app::can_rx::VC_State_update(app::can_utils::VCState::VC_BMS_ON_STATE);
    LetTimePass(20);
    // Set voltage to undervoltage level after entering a precharge state
    for (uint8_t retry = 0; retry < app::precharge::MAX_PRECHARGE_ATTEMPTS; retry++)
    {
        unsigned int closed_time = 0;
        while (closed_time < app::precharge::PRECHARGE_COMPLETION_LOWER_BOUND - tolerance_time)
        {
            ASSERT_STATE_EQ(app::states::precharge_drive_state);
            ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
            LetTimePass(10);
            closed_time += 10;
        }
        ASSERT_LT(closed_time, app::precharge::PRECHARGE_COMPLETION_LOWER_BOUND)
            << "Precharge relay stayed closed for " << closed_time << "ms (expected ~"
            << app::precharge::PRECHARGE_COMPLETION_LOWER_BOUND - tolerance_time << "ms)"
            << ", time=" << io::time::getCurrentMs();

        fakes::ts::setVoltage(target_voltage);
        LetTimePass(10);

        // Set voltage back to 0V since contactors opened.
        ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
        fakes::ts::setVoltage(0.0f);

        if (retry == app::precharge::MAX_PRECHARGE_ATTEMPTS - 1)
            break;

        unsigned int open_time = 0;
        while (open_time < app::precharge::PRECHARGE_COOLDOWN_MS)
        {
            ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
            LetTimePass(10);
            open_time += 10;
        }
        ASSERT_NEAR(open_time, app::precharge::PRECHARGE_COOLDOWN_MS, 100)
            << "Precharge relay stayed open for " << open_time << "ms (expected ~"
            << app::precharge::PRECHARGE_COOLDOWN_MS << "ms)"
            << ", time=" << io::time::getCurrentMs();
    }
    ASSERT_STATE_EQ(app::states::precharge_latch_state);
}

// PrechargeLatch-Init test

TEST_F(BmsStateMachineTest, precharge_latch_timouts_to_init)
{
    fakes::adbms::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(undervoltage);
    app::StateMachine::set_current_state(&app::states::precharge_latch_state);
    LetTimePass(latch_timeout);

    ASSERT_STATE_EQ(app::states::init_state);
}

// Init-Balance tests

TEST_F(BmsStateMachineTest, enter_balance)
{
    ASSERT_STATE_EQ(app::states::init_state);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    app::can_rx::Debug_CellBalancing_Request_update(true);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::balancing_state);
    EXPECT_TRUE(app::can_rx::Debug_CellBalancing_Request_get());
}

TEST_F(BmsStateMachineTest, exit_balance)
{
    app::StateMachine::set_current_state(&app::states::balancing_state);
    app::can_rx::Debug_CellBalancing_Request_update(false);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::init_state);
}

TEST_F(BmsStateMachineTest, exits_balance_when_negative_opens_and_clears_request_on_exit)
{
    app::StateMachine::set_current_state(&app::states::balancing_state);
    app::can_rx::Debug_CellBalancing_Request_update(true);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::init_state);
    EXPECT_FALSE(app::can_rx::Debug_CellBalancing_Request_get());
}

// Init-PrechargeCharge tests

TEST_F(BmsStateMachineTest, enter_precharge_charge)
{
    ASSERT_STATE_EQ(app::states::init_state);

    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::charger::setConnectionStatus(app::can_utils::ChargerConnectedType::CHARGER_CONNECTED_EVSE);
    app::can_rx::Debug_StartCharging_update(true);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::precharge_charge_state);
    ASSERT_EQ(app::can_tx::BMS_State_get(), app::can_utils::BmsState::BMS_PRECHARGE_CHARGE_STATE);
}

TEST_F(BmsStateMachineTest, precharge_charge_success_to_charge)
{
    fakes::adbms::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    app::can_rx::Debug_StartCharging_update(true);
    fakes::charger::setConnectionStatus(app::can_utils::ChargerConnectedType::CHARGER_CONNECTED_EVSE);
    LetTimePass(20);

    // Precharge for charging, we don't care about precharging too quickly
    ASSERT_STATE_EQ(app::states::precharge_charge_state);
    ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(10);

    fakes::ts::setVoltage(target_voltage);
    LetTimePass(20);
    ASSERT_STATE_EQ(app::states::charge_state);
    ASSERT_EQ(io::irs::positiveState(), app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
}

TEST_F(BmsStateMachineTest, precharge_charge_latches_after_retries)
{
    // NOTE: Timing is a bit tight in this test.
    fakes::adbms::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    app::can_rx::Debug_StartCharging_update(true);
    app::StateMachine::set_current_state(&app::states::precharge_charge_state);
    fakes::charger::setConnectionStatus(app::can_utils::ChargerConnectedType::CHARGER_CONNECTED_EVSE);
    // Simulate closing precharge contactor before voltage rise.
    LetTimePass(10);

    for (uint8_t retry = 0; retry < app::precharge::MAX_PRECHARGE_ATTEMPTS; retry++)
    {
        ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
        fakes::ts::setVoltage(undervoltage);
        LetTimePass(app::precharge::PRECHARGE_COMPLETION_UPPER_BOUND);

        if (retry == app::precharge::MAX_PRECHARGE_ATTEMPTS - 1)
            break;

        ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
        fakes::ts::setVoltage(0.0f);
        LetTimePass(app::precharge::PRECHARGE_COOLDOWN_MS);
        ASSERT_STATE_EQ(app::states::precharge_charge_state);
    }

    ASSERT_STATE_EQ(app::states::precharge_latch_state);
    ASSERT_FALSE(app::can_rx::Debug_StartCharging_get());
    ASSERT_EQ(io::irs::prechargeState(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
}

// Drive tests

TEST_F(BmsStateMachineTest, drive_to_init_on_ts_undervoltage_debounced)
{
    constexpr float pack_voltage = 400.0f;

    fakes::adbms::setPackVoltageEvenly(pack_voltage);
    LetTimePass(1300);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    io::irs::setPositive(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(pack_voltage);
    app::StateMachine::set_current_state(&app::states::drive_state);
    LetTimePass(20);
    ASSERT_STATE_EQ(app::states::drive_state);

    fakes::ts::setVoltage(pack_voltage - 30.0f);
    LetTimePass(400);
    ASSERT_STATE_EQ(app::states::drive_state);
    LetTimePass(200);

    ASSERT_STATE_EQ(app::states::init_state);
}

TEST_F(BmsStateMachineTest, drive_to_init_on_ts_undervoltage_immediate)
{
    constexpr float pack_voltage = 400.0f;

    fakes::adbms::setPackVoltageEvenly(pack_voltage);
    LetTimePass(1300);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    io::irs::setPositive(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(pack_voltage);
    app::StateMachine::set_current_state(&app::states::drive_state);
    LetTimePass(20);
    ASSERT_STATE_EQ(app::states::drive_state);

    fakes::ts::setVoltage(pack_voltage - 60.0f);
    LetTimePass(20);

    ASSERT_STATE_EQ(app::states::init_state);
}

TEST_F(BmsStateMachineTest, drive_stays_drive_when_pack_voltage_invalid)
{
    fakes::adbms::setSegmentVoltageError(ErrorCode::INVALID_READING);
    LetTimePass(1300);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    io::irs::setPositive(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(0.0f);
    app::StateMachine::set_current_state(&app::states::drive_state);
    LetTimePass(1000);

    ASSERT_STATE_EQ(app::states::drive_state);
}

TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_no_faults_set)
{
    app::can_tx::BMS_Fault_TESTFAULT_set(true);
    LetTimePass(20);
    ASSERT_STATE_EQ(app::states::fault_state);

    app::can_tx::BMS_Fault_TESTFAULT_set(false);
    ASSERT_FALSE(app::can_alerts::AnyBoardHasFault());
    LetTimePass(20);
    ASSERT_STATE_EQ(app::states::init_state);
}

// Faultlatch StateMachine Tests

TEST_F(BmsStateMachineTest, imd_fault_latches_then_reset_to_init_state)
{
    app::StateMachine::set_current_state(&app::states::drive_state);
    fakes::faultLatch::resetFaultLatch(&imd_ok_latch);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(3.8f * NUM_SEGMENTS * CELLS_PER_SEGMENT);
    LetTimePass(10);

    ASSERT_TRUE(app::can_tx::BMS_ImdLatchOk_get());
    ASSERT_STATE_EQ(app::states::drive_state);

    fakes::faultLatch::updateFaultLatch(&imd_ok_latch, io::FaultLatch::FaultLatchState::FAULT);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
    fakes::ts::setVoltage(0);
    LetTimePass(20);
    for (int i = 0; i < 30; i++)
    {
        LetTimePass(10);
        ASSERT_STATE_EQ(app::states::init_state);
        ASSERT_FALSE(app::can_tx::BMS_ImdLatchOk_get());
    }
    fakes::faultLatch::updateFaultLatch(&imd_ok_latch, io::FaultLatch::FaultLatchState::OK);
    for (int i = 0; i < 30; i++)
    {
        LetTimePass(10);
        ASSERT_STATE_EQ(app::states::init_state);
        ASSERT_FALSE(app::can_tx::BMS_ImdLatchOk_get());
    }
    fakes::faultLatch::resetFaultLatch(&imd_ok_latch);
    LetTimePass(20); // Need to wait for 100Hz task to run twice since jobs100hz runs after statemachine
    for (int i = 0; i < 30; i++)
    {
        LetTimePass(10);
        ASSERT_STATE_EQ(app::states::init_state);
        ASSERT_TRUE(app::can_tx::BMS_ImdLatchOk_get());
    }
}

TEST_F(BmsStateMachineTest, bms_fault_latches_then_reset_to_init_state)
{
    app::StateMachine::set_current_state(&app::states::drive_state);
    fakes::faultLatch::resetFaultLatch(&bms_ok_latch);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(3.8f * NUM_SEGMENTS * CELLS_PER_SEGMENT);
    LetTimePass(10);

    ASSERT_TRUE(app::can_tx::BMS_BmsLatchOk_get());
    ASSERT_STATE_EQ(app::states::drive_state);

    fakes::faultLatch::updateFaultLatch(&bms_ok_latch, io::FaultLatch::FaultLatchState::FAULT);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
    fakes::ts::setVoltage(0);
    LetTimePass(20);

    for (int i = 0; i < 30; i++)
    {
        LetTimePass(10);
        ASSERT_STATE_EQ(app::states::init_state);
        ASSERT_FALSE(app::can_tx::BMS_BmsLatchOk_get());
    }
    fakes::faultLatch::updateFaultLatch(&bms_ok_latch, io::FaultLatch::FaultLatchState::OK);
    for (int i = 0; i < 30; i++)
    {
        LetTimePass(10);
        ASSERT_STATE_EQ(app::states::init_state);
        ASSERT_FALSE(app::can_tx::BMS_BmsLatchOk_get());
    }
    fakes::faultLatch::resetFaultLatch(&bms_ok_latch);
    LetTimePass(20); // Need to wait for 100Hz task to run twice since jobs100hz runs after statemachine
    for (int i = 0; i < 30; i++)
    {
        LetTimePass(10);
        ASSERT_STATE_EQ(app::states::init_state);
        ASSERT_TRUE(app::can_tx::BMS_BmsLatchOk_get());
    }
}

TEST_F(BmsStateMachineTest, bspd_fault_latches_then_reset_to_init_state)
{
    app::StateMachine::set_current_state(&app::states::drive_state);
    fakes::faultLatch::resetFaultLatch(&bspd_ok_latch);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(3.8f * NUM_SEGMENTS * CELLS_PER_SEGMENT);
    LetTimePass(10);

    ASSERT_TRUE(app::can_tx::BMS_BspdLatchOk_get());
    ASSERT_STATE_EQ(app::states::drive_state);

    fakes::faultLatch::updateFaultLatch(&bspd_ok_latch, io::FaultLatch::FaultLatchState::FAULT);
    fakes::irs::setNegativeState(app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
    fakes::ts::setVoltage(0);
    LetTimePass(20);
    for (int i = 0; i < 30; i++)
    {
        LetTimePass(10);
        ASSERT_STATE_EQ(app::states::init_state);
        ASSERT_FALSE(app::can_tx::BMS_BspdLatchOk_get());
    }
    fakes::faultLatch::updateFaultLatch(&bspd_ok_latch, io::FaultLatch::FaultLatchState::OK);
    for (int i = 0; i < 30; i++)
    {
        LetTimePass(10);
        ASSERT_STATE_EQ(app::states::init_state);
        ASSERT_FALSE(app::can_tx::BMS_BspdLatchOk_get());
    }
    fakes::faultLatch::resetFaultLatch(&bspd_ok_latch);
    LetTimePass(20); // Need to wait for 100Hz task to run twice since jobs100hz runs after statemachine
    for (int i = 0; i < 30; i++)
    {
        LetTimePass(10);
        ASSERT_STATE_EQ(app::states::init_state);
        ASSERT_TRUE(app::can_tx::BMS_BspdLatchOk_get());
    }
}

TEST_F(BmsStateMachineTest, check_contactors_open_in_init_states)
{
    io::irs::setPositive(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    app::StateMachine::set_current_state(&app::states::fault_state);
    ASSERT_EQ(io::irs::positiveState(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);

    io::irs::setPositive(app::can_utils::ContactorState::CONTACTOR_STATE_CLOSED);
    app::StateMachine::set_current_state(&app::states::init_state);
    ASSERT_EQ(io::irs::positiveState(), app::can_utils::ContactorState::CONTACTOR_STATE_OPEN);
}
