#include "test_fakes.hpp"
#include "test_BMSBase.hpp"
#include "states/app_states.hpp"
#include "app_precharge.hpp"
#include "io_time.hpp"
// #include "app.segments.hpp"
#include "app_canAlerts.hpp"

using namespace app::can_utils;
using namespace app::states;
using namespace app::precharge;

class BmsStateMachineTest : public BMSBaseTest
{
};
static constexpr int target_voltage          = (float)(NUM_SEGMENTS * CELLS_PER_SEGMENT) * 4.2f; // V
static constexpr int undervoltage            = 200.0f;                                           // V
static constexpr int tolerance_time          = 500;                                              // ms
static constexpr int precharge_completion_ms = static_cast<int>(PRECHARGE_COMPLETION_MS_F);
static constexpr int latch_timeout           = PRECHARGE_LATCH_TIMEOUT_MS + 100; // ms
namespace fakes
{
}

// Init-PrechargeDrive tests

TEST_F(BmsStateMachineTest, enter_precharge_drive)
{
    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);

    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    app::can_rx::VC_State_update(VCState::VC_BMS_ON_STATE);
    app::can_rx::Debug_StartCharging_update(false);
    fakes::charger::setConnectionStatus(ChargerConnectedType::CHARGER_DISCONNECTED);
    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &precharge_drive_state);
    ASSERT_EQ(io::irs::prechargeState(), ContactorState::CONTACTOR_STATE_CLOSED);
    ASSERT_EQ(io::irs::positiveState(), ContactorState::CONTACTOR_STATE_OPEN);
    ASSERT_EQ(io::irs::negativeState(), ContactorState::CONTACTOR_STATE_CLOSED);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_negative_open)
{
    SetInitialState(&init_state);
    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);
    app::can_rx::VC_State_update(VCState::VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_OPEN);
    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_vc_state_not_bms_on)
{
    SetInitialState(&init_state);
    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);
    app::can_rx::VC_State_update(VCState::VC_INIT_STATE);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);
}

TEST_F(BmsStateMachineTest, no_precharge_drive_if_charging_requested)
{
    SetInitialState(&init_state);
    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);
    app::can_rx::VC_State_update(VCState::VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    app::can_rx::Debug_StartCharging_update(true);
    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);
}

// PrechargeDrive-Drive tests

TEST_F(BmsStateMachineTest, precharge_drive_success)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(0.0f);
    app::StateMachine::set_current_state(&precharge_drive_state);
    LetTimePass(10);

    for (int i = 0; i < precharge_completion_ms; i += 10)
    {
        ASSERT_EQ(app::StateMachine::get_current_state(), &precharge_drive_state);
        ASSERT_EQ(io::irs::prechargeState(), ContactorState::CONTACTOR_STATE_CLOSED);
        ASSERT_EQ(app::can_tx::BMS_PrechargeRelay_get(), ContactorState::CONTACTOR_STATE_CLOSED);
        LetTimePass(10);
    }

    fakes::ts::setVoltage(target_voltage);
    LetTimePass(10);
    ASSERT_EQ(app::StateMachine::get_current_state(), &drive_state);
    ASSERT_EQ(io::irs::prechargeState(), ContactorState::CONTACTOR_STATE_OPEN);
    ASSERT_EQ(app::can_tx::BMS_PrechargeRelay_get(), ContactorState::CONTACTOR_STATE_OPEN);
}

TEST_F(BmsStateMachineTest, precharge_rises_too_slowly_then_latches_after_max_retries)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(undervoltage);
    app::StateMachine::set_current_state(&precharge_drive_state);
    LetTimePass(10);

    for (uint8_t retry = 0; retry < MAX_PRECHARGE_ATTEMPTS; retry++)
    {
        unsigned int closed_time = 0;
        while (io::irs::prechargeState() == ContactorState::CONTACTOR_STATE_CLOSED &&
               closed_time < PRECHARGE_COMPLETION_UPPER_BOUND + tolerance_time)
        {
            ASSERT_EQ(app::StateMachine::get_current_state(), &precharge_drive_state);
            ASSERT_EQ(app::can_tx::BMS_PrechargeRelay_get(), ContactorState::CONTACTOR_STATE_CLOSED);
            LetTimePass(10);
            closed_time += 10;
        }
        ASSERT_NEAR(closed_time, PRECHARGE_COMPLETION_UPPER_BOUND, 100)
            << "Precharge relay stayed closed for " << closed_time << "ms (expected ~"
            << PRECHARGE_COMPLETION_UPPER_BOUND << "ms)"
            << ", time=" << io::time::getCurrentMs();

        if (retry == MAX_PRECHARGE_ATTEMPTS - 1)
            break;

        unsigned int open_time = 0;
        while (io::irs::prechargeState() == ContactorState::CONTACTOR_STATE_OPEN &&
               open_time < PRECHARGE_COOLDOWN_MS + tolerance_time)
        {
            ASSERT_EQ(app::can_tx::BMS_PrechargeRelay_get(), ContactorState::CONTACTOR_STATE_OPEN);
            LetTimePass(10);
            open_time += 10;
        }
        ASSERT_NEAR(open_time, PRECHARGE_COOLDOWN_MS, 100)
            << "Precharge relay stayed open for " << open_time << "ms (expected ~" << PRECHARGE_COOLDOWN_MS << "ms)"
            << ", time=" << io::time::getCurrentMs();
    }
    ASSERT_EQ(app::StateMachine::get_current_state(), &precharge_latch_state);
}

TEST_F(BmsStateMachineTest, precharge_rises_too_quickly_then_latches_after_max_retries)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(undervoltage);
    app::StateMachine::set_current_state(&precharge_drive_state);
    LetTimePass(10);

    for (uint8_t retry = 0; retry < MAX_PRECHARGE_ATTEMPTS; retry++)
    {
        unsigned int closed_time = 0;
        while (closed_time < PRECHARGE_COMPLETION_LOWER_BOUND - tolerance_time)
        {
            ASSERT_EQ(app::StateMachine::get_current_state(), &precharge_drive_state);
            ASSERT_EQ(io::irs::prechargeState(), ContactorState::CONTACTOR_STATE_CLOSED);
            LetTimePass(10);
            closed_time += 10;
        }
        ASSERT_LT(closed_time, PRECHARGE_COMPLETION_LOWER_BOUND)
            << "Precharge relay stayed closed for " << closed_time << "ms (expected ~"
            << PRECHARGE_COMPLETION_LOWER_BOUND - tolerance_time << "ms)"
            << ", time=" << io::time::getCurrentMs();

        fakes::ts::setVoltage(target_voltage);
        LetTimePass(10);
        ASSERT_NE(io::irs::prechargeState(), ContactorState::CONTACTOR_STATE_CLOSED);

        if (retry == MAX_PRECHARGE_ATTEMPTS - 1)
            break;

        unsigned int open_time = 0;
        while (open_time < PRECHARGE_COOLDOWN_MS)
        {
            ASSERT_EQ(io::irs::prechargeState(), ContactorState::CONTACTOR_STATE_OPEN);
            LetTimePass(10);
            open_time += 10;
        }
        ASSERT_NEAR(open_time, PRECHARGE_COOLDOWN_MS, 100)
            << "Precharge relay stayed open for " << open_time << "ms (expected ~" << PRECHARGE_COOLDOWN_MS << "ms)"
            << ", time=" << io::time::getCurrentMs();
    }
    ASSERT_EQ(app::StateMachine::get_current_state(), &precharge_latch_state);
}

// PrechargeLatch-Init test

TEST_F(BmsStateMachineTest, precharge_latch_timouts_to_init)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(undervoltage);
    app::StateMachine::set_current_state(&precharge_latch_state);
    LetTimePass(latch_timeout);

    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);
}

// Init-Balance tests

TEST_F(BmsStateMachineTest, enter_balance)
{
    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    app::can_rx::Debug_CellBalancingRequest_update(true);
    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &balancing_state);
    EXPECT_TRUE(app::can_rx::Debug_CellBalancingRequest_get());
}

TEST_F(BmsStateMachineTest, exit_balance)
{
    SetInitialState(&balancing_state);
    app::can_rx::Debug_CellBalancingRequest_update(false);
    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);
}

TEST_F(BmsStateMachineTest, exits_balance_when_negative_opens_and_clears_request_on_exit)
{
    SetInitialState(&balancing_state);
    app::can_rx::Debug_CellBalancingRequest_update(true);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_OPEN);
    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);
    EXPECT_FALSE(app::can_rx::Debug_CellBalancingRequest_get());
}

// Init-PrechargeCharge tests

TEST_F(BmsStateMachineTest, enter_precharge_charge)
{
    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);

    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::charger::setConnectionStatus(ChargerConnectedType::CHARGER_CONNECTED_EVSE);
    app::can_rx::Debug_StartCharging_update(true);
    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &precharge_charge_state);
    ASSERT_EQ(app::can_tx::BMS_State_get(), BmsState::BMS_PRECHARGE_CHARGE_STATE);
}

TEST_F(BmsStateMachineTest, precharge_charge_success_to_charge_init)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(target_voltage);

    app::StateMachine::set_current_state(&precharge_charge_state);
    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &charge_init_state);
    ASSERT_EQ(io::irs::positiveState(), ContactorState::CONTACTOR_STATE_CLOSED);
    ASSERT_EQ(io::irs::prechargeState(), ContactorState::CONTACTOR_STATE_OPEN);
}

TEST_F(BmsStateMachineTest, precharge_charge_latches_after_retries)
{
    fakes::segments::setPackVoltageEvenly(target_voltage);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    fakes::ts::setVoltage(undervoltage);
    app::can_rx::Debug_StartCharging_update(true);

    app::StateMachine::set_current_state(&precharge_charge_state);
    LetTimePass(20);

    for (uint8_t retry = 0; retry < MAX_PRECHARGE_ATTEMPTS; retry++)
    {
        LetTimePass(PRECHARGE_COMPLETION_UPPER_BOUND + tolerance_time);

        if (retry == MAX_PRECHARGE_ATTEMPTS - 1)
            break;

        LetTimePass(PRECHARGE_COOLDOWN_MS + tolerance_time);
        ASSERT_EQ(app::StateMachine::get_current_state(), &precharge_charge_state);
    }

    ASSERT_EQ(app::StateMachine::get_current_state(), &precharge_latch_state);
    EXPECT_FALSE(app::can_rx::Debug_StartCharging_get());
    ASSERT_EQ(io::irs::prechargeState(), ContactorState::CONTACTOR_STATE_OPEN);
}

// Drive tests

TEST_F(BmsStateMachineTest, drive_to_init_on_negative_open)
{
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    SetInitialState(&drive_state);
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_OPEN);
    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &init_state);
}

TEST_F(BmsStateMachineTest, drive_to_fault_on_positive_open)
{
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    SetInitialState(&drive_state);
    io::irs::setPositive(ContactorState::CONTACTOR_STATE_OPEN);
    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &fault_state);
}

TEST_F(BmsStateMachineTest, drive_to_fault_from_board_faults)
{
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    SetInitialState(&drive_state);
    // app::can_alerts::AnyBoardHasFault_set(true);
    app::can_tx::BMS_Fault_TESTFAULT_set(true);
    LetTimePass(20);
    ASSERT_TRUE(app::can_alerts::AnyBoardHasFault());
    ASSERT_EQ(app::StateMachine::get_current_state(), &fault_state);
}

TEST_F(BmsStateMachineTest, drive_to_fault_on_overtemp)
{
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
    SetInitialState(&drive_state);

    fakes::segments::setPackVoltageEvenly(MAX_CELL_VOLTAGE_FAULT_V - 0.1f);
    std::array<std::array<float, AUX_REGS_PER_SEGMENT>, NUM_SEGMENTS> temperatures{};
    for (auto &segment_temperatures : temperatures)
    {
        segment_temperatures.fill(25.0f);
    }
    temperatures[0][0] = MAX_CELL_TEMP_FAULT_C;
    fakes::segments::setCellTemperatures(temperatures);

    LetTimePass(20);

    ASSERT_EQ(app::StateMachine::get_current_state(), &fault_state);
}

// imd
// bspd
// overtemp
// overvoltage
// undervoltage
// other board faulting
