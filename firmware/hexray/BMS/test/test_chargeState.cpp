#include "test_fakes.hpp"
#include "test_BMSBase.hpp"
#include "app_states.hpp"
#include "app_precharge.hpp"
#include "app_segments.hpp"
#include "io_irs.hpp"
#include "app_canAlerts.hpp"

using namespace app::can_utils;
using namespace app::states;
using namespace app::precharge;

class BmsChargeStateTest : public BMSBaseTest
{
  protected:
    // Drives the charge state into a "clean" precondition: charger connected, user enabled,
    // contactors closed, all Elcon faults cleared, and cells in a mid-charge state.
    // Individual tests override whichever knobs they care about.
    void EnterChargeStateClean()
    {
        fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
        fakes::charger::setConnectionStatus(ChargerConnectedType::CHARGER_CONNECTED_WALL);
        app::can_rx::Debug_StartCharging_update(true);

        app::can_rx::Elcon_HardwareFailure_update(false);
        app::can_rx::Elcon_ChargerOverTemperature_update(false);
        app::can_rx::Elcon_InputVoltageError_update(false);
        app::can_rx::Elcon_ChargingDisabled_update(false);
        app::can_rx::Elcon_CommunicationTimeout_update(false);
        app::can_rx::Elcon_OutputCurrent_update(5.0f);
        app::can_rx::Elcon_OutputVoltage_update(400.0f);

        fakes::adbms::setCellVoltage(0, 0, 3.8f);
        // fakes::segments::setMaxCellTemp(25.0f);

        app::StateMachine::set_current_state(&charge_state);
    }
};

// Expected pack voltage command on every non-stop tx frame.
static constexpr float kPackVoltageMax = 5.0f * 28.0f * 4.20f; // 588V (matches PACK_VOLTAGE_MAX)

// Expected DC current for an AC supply of CAC_MAX * CIRC_EFF (= 16A).
// pin  = 208 * 16 = 3328W
// pout = pin * 0.93 = 3095.04W
// idc  = pout / 588 = 5.264f, < MAX_DC_CURRENT(12), so returned as-is.
static constexpr float kPMaxFullAcSupply = 208.0f * 16.0f * 0.93f;

// =============================================================================
// runOnEntry
// =============================================================================

TEST_F(BmsChargeStateTest, entry_sets_can_state_and_resets_flags_and_closes_positive)
{
    // Pre-dirty the flags so we know entry actually clears them.
    app::can_tx::BMS_ChargingFaulted_set(true);
    app::can_tx::BMS_ChargingDone_set(true);
    io::irs::setPositive(ContactorState::CONTACTOR_STATE_OPEN);

    EnterChargeStateClean();

    ASSERT_STATE_EQ(charge_state);
    ASSERT_EQ(app::can_tx::BMS_State_get(), BmsState::BMS_CHARGE_STATE);
    ASSERT_FALSE(app::can_tx::BMS_ChargingFaulted_get());
    ASSERT_FALSE(app::can_tx::BMS_ChargingDone_get());
    ASSERT_EQ(io::irs::positiveState(), ContactorState::CONTACTOR_STATE_CLOSED);
}

// =============================================================================
// runOnExit (transition charge_state -> init_state via !user_enable)
// =============================================================================

TEST_F(BmsChargeStateTest, exit_opens_positive_clears_debug_charging_and_sends_stop_frame)
{
    EnterChargeStateClean();
    LetTimePass(10); // settle one tick in charge state

    app::can_rx::Debug_StartCharging_update(false);
    LetTimePass(10); // tick decides to leave; runOnExit runs on the SAME tick

    ASSERT_STATE_EQ(init_state);
    ASSERT_EQ(io::irs::positiveState(), ContactorState::CONTACTOR_STATE_OPEN);
    ASSERT_FALSE(app::can_rx::Debug_StartCharging_get());
    ASSERT_NEAR(app::can_tx::BMS_MaxChargingVoltage_get(), 0.0f, 0.02f);
    ASSERT_NEAR(app::can_tx::BMS_MaxChargingCurrent_get(), 0.0f, 0.02f);
    ASSERT_TRUE(app::can_tx::BMS_StopCharging_get());
}

// =============================================================================
// Fault paths
//
// Two-step behavior:
//   Tick 1 (in chargeState): runOnTick100Hz detects the fault, calls the charger module
//          to see if there was a charger fault. If there was, there is a stop command sent to the charger
//          and the state machine queues a transition to init_state.
//   Tick 2 (transition):    statemachine runs the run on exit and runOn100Hz
//          function for the new state on the next tick.
// =============================================================================

static void ExpectStopFrameSent()
{
    ASSERT_NEAR(app::can_tx::BMS_MaxChargingVoltage_get(), 0.0f, 0.02f);
    ASSERT_NEAR(app::can_tx::BMS_MaxChargingCurrent_get(), 0.0f, 0.02f);
    ASSERT_TRUE(app::can_tx::BMS_StopCharging_get());
}

TEST_F(BmsChargeStateTest, exit_charging_when_negative_contactor_opens)
{
    constexpr uint32_t ir_negative_debounce_period = 200U;
    EnterChargeStateClean();
    fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_OPEN);
    // Need to wait for the debounce period for irs- in order for jobs to send state machine to init
    // added +20 is because the first tick the timer for debounce is started.
    LetTimePass(ir_negative_debounce_period + 10);
    // The state machine is called before jobs in the 100Hz task so runOnExit is called the tick after.
    LetTimePass(10);
    ExpectStopFrameSent();
    ASSERT_FALSE(app::can_rx::Debug_StartCharging_get());
    // Allow runOnEntry of init_state to set current state status.
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);
    ASSERT_EQ(io::irs::positiveState(), ContactorState::CONTACTOR_STATE_OPEN);
}

// Charger physical disconnect is NOT a fault — it's a clean exit to init, mirrors the
// user-disable path.
TEST_F(BmsChargeStateTest, charger_disconnect_returns_to_init_without_fault)
{
    EnterChargeStateClean();
    fakes::charger::setConnectionStatus(ChargerConnectedType::CHARGER_DISCONNECTED);
    LetTimePass(10);

    ASSERT_STATE_EQ(init_state);
    ASSERT_FALSE(app::can_tx::BMS_ChargingDone_get());
    ASSERT_EQ(io::irs::positiveState(), ContactorState::CONTACTOR_STATE_OPEN);
    ASSERT_FALSE(app::can_rx::Debug_StartCharging_get());
    ASSERT_TRUE(app::can_tx::BMS_StopCharging_get());
}

TEST_F(BmsChargeStateTest, exit_charging_on_elcon_hardware_failure)
{
    EnterChargeStateClean();
    app::can_rx::Elcon_HardwareFailure_update(true);
    LetTimePass(10);
    ExpectStopFrameSent();

    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsChargeStateTest, exit_charging_on_elcon_over_temperature)
{
    EnterChargeStateClean();
    app::can_rx::Elcon_ChargerOverTemperature_update(true);
    LetTimePass(10);
    ExpectStopFrameSent();

    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsChargeStateTest, exit_charging_on_elcon_input_voltage_error)
{
    EnterChargeStateClean();
    app::can_rx::Elcon_InputVoltageError_update(true);
    LetTimePass(10);
    ExpectStopFrameSent();

    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsChargeStateTest, exit_charging_on_elcon_charging_disabled)
{
    EnterChargeStateClean();
    app::can_rx::Elcon_ChargingDisabled_update(true);
    LetTimePass(10);
    ExpectStopFrameSent();

    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsChargeStateTest, exit_charging_on_elcon_comm_timeout)
{
    EnterChargeStateClean();
    app::can_rx::Elcon_CommunicationTimeout_update(true);
    LetTimePass(10);
    ExpectStopFrameSent();

    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);
}

// Ensure we exit charging even for a transient fault.
TEST_F(BmsChargeStateTest, transient_charger_fault_still_exits_charge_state)
{
    EnterChargeStateClean();
    app::can_rx::Elcon_HardwareFailure_update(true);
    LetTimePass(10);
    ExpectStopFrameSent();

    // Clear the Elcon fault before the transition tick.
    app::can_rx::Elcon_HardwareFailure_update(false);
    LetTimePass(10);

    // But the queued fault_state transition (from the previous tick) still completes.
    ASSERT_STATE_EQ(init_state);
}

// =============================================================================
// User-disable path — transition to init, stop sent
// =============================================================================

TEST_F(BmsChargeStateTest, transition_to_init_when_user_disables)
{
    EnterChargeStateClean();
    LetTimePass(10);
    ASSERT_STATE_EQ(charge_state);

    app::can_rx::Debug_StartCharging_update(false);
    LetTimePass(10);

    ASSERT_STATE_EQ(init_state);
}

// =============================================================================
// Termination — max_cell_v >= CELL_V_TERMINATE AND output_current < I_TERMINATE_A
// =============================================================================

TEST_F(BmsChargeStateTest, terminates_when_cells_full_and_current_low)
{
    EnterChargeStateClean();
    fakes::adbms::setCellVoltage(0, 0, 4.19f);
    LetTimePass(500); // Need the adbms task to run to update max cell voltage
    app::can_rx::Elcon_OutputCurrent_update(0.4f);
    LetTimePass(20);

    ASSERT_STATE_EQ(init_state);
    ASSERT_TRUE(app::can_tx::BMS_ChargingDone_get());
}

TEST_F(BmsChargeStateTest, no_terminate_when_cells_full_but_current_high)
{
    EnterChargeStateClean();
    fakes::adbms::setCellVoltage(0, 0, 4.19f);
    LetTimePass(500); // Need the adbms task to run to update max cell voltage
    app::can_rx::Elcon_OutputCurrent_update(2.0f);
    LetTimePass(20);

    ASSERT_STATE_EQ(charge_state);
    ASSERT_FALSE(app::can_tx::BMS_ChargingDone_get());
}

TEST_F(BmsChargeStateTest, no_terminate_when_current_low_but_cells_not_full)
{
    EnterChargeStateClean();
    fakes::adbms::setCellVoltage(0, 0, 4.0f);
    LetTimePass(500); // Need the adbms task to run to update max cell voltage
    app::can_rx::Elcon_OutputCurrent_update(0.5f);
    LetTimePass(20);

    ASSERT_STATE_EQ(charge_state);
    ASSERT_FALSE(app::can_tx::BMS_ChargingDone_get());
}

// =============================================================================
// Current command — CC region (low cell voltage)
// =============================================================================

TEST_F(BmsChargeStateTest, cc_region_wall_commands_max_dc_current)
{
    EnterChargeStateClean();
    fakes::charger::setConnectionStatus(ChargerConnectedType::CHARGER_CONNECTED_WALL);
    LetTimePass(500); // Need the adbms task to run to update voltage stats
    LetTimePass(20);

    ASSERT_STATE_EQ(charge_state);
    ASSERT_FALSE(app::can_tx::BMS_StopCharging_get());
    ASSERT_FLOAT_EQ(app::can_tx::BMS_MaxChargingVoltage_get(), kPackVoltageMax);
    {
        io::unique_semaphore s{ shared_lock };
        ASSERT_NEAR(
            app::can_tx::BMS_MaxChargingCurrent_get(),
            kPMaxFullAcSupply / app::segments::shared::getPackVoltage().value(), 0.02f);
    }
}

TEST_F(BmsChargeStateTest, cc_region_evse_high_duty_clamps_to_full_ac_supply)
{
    EnterChargeStateClean();
    fakes::charger::setConnectionStatus(ChargerConnectedType::CHARGER_CONNECTED_EVSE);
    fakes::charger::setCPDutyCycle(85.0f); // 85*0.6 = 51A available -> clamped to 16A
    fakes::adbms::setCellVoltage(0, 0, 3.8f);
    LetTimePass(500); // Need the adbms task to run to update max cell voltage
    LetTimePass(20);

    {
        io::unique_semaphore s{ shared_lock };
        ASSERT_STATE_EQ(charge_state);
        ASSERT_NEAR(
            app::can_tx::BMS_MaxChargingCurrent_get(),
            kPMaxFullAcSupply / app::segments::shared::getPackVoltage().value(), 0.02f);
    }
}

TEST_F(BmsChargeStateTest, cc_region_evse_low_duty_reduces_current)
{
    // duty=20% -> 20*0.6 = 12A available (< 16 clamp).
    // idc = 208 * 12 * 0.93 / pack_voltage
    EnterChargeStateClean();
    fakes::charger::setConnectionStatus(ChargerConnectedType::CHARGER_CONNECTED_EVSE);
    fakes::charger::setCPDutyCycle(20.0f);
    fakes::adbms::setCellVoltage(0, 0, 3.8f);
    LetTimePass(500); // Need the adbms task to run to update max cell voltage
    LetTimePass(20);
    float expected;
    {
        io::unique_semaphore s{ shared_lock };
        expected = 208.0f * 12.0f * 0.93f / app::segments::shared::getPackVoltage().value();
    }
    ASSERT_NEAR(app::can_tx::BMS_MaxChargingCurrent_get(), expected, 0.02f);
}

// =============================================================================
// Current command — taper region (CELL_V_TAPER_START <= v < CELL_V_MAX_CHARGE)
// =============================================================================

TEST_F(BmsChargeStateTest, taper_at_midpoint_halves_current)
{
    // max_cell_v = 4.15 -> frac = (4.20-4.15)/(4.20-4.10) = 0.5
    EnterChargeStateClean();
    fakes::adbms::setCellVoltage(0, 0, 4.15f);
    LetTimePass(500); // Need the adbms task to run to update max cell voltage
    LetTimePass(20);
    {
        io::unique_semaphore s{ shared_lock };
        ASSERT_STATE_EQ(charge_state);
        ASSERT_NEAR(
            app::can_tx::BMS_MaxChargingCurrent_get(),
            kPMaxFullAcSupply / app::segments::shared::getPackVoltage().value() * 0.5f, 0.02f);
    }
}

TEST_F(BmsChargeStateTest, taper_at_taper_start_commands_full_current)
{
    // max_cell_v == 4.10 -> still CC region (strict > guard), full current.
    EnterChargeStateClean();
    fakes::adbms::setCellVoltage(0, 0, 4.1f);
    LetTimePass(500); // Need the adbms task to run to update max cell voltage
    LetTimePass(20);

    {
        io::unique_semaphore s{ shared_lock };
        ASSERT_STATE_EQ(charge_state);
        ASSERT_NEAR(
            app::can_tx::BMS_MaxChargingCurrent_get(),
            kPMaxFullAcSupply / app::segments::shared::getPackVoltage().value(), 0.02f);
    }
}

TEST_F(BmsChargeStateTest, current_clamped_to_zero_at_per_cell_max)
{
    // max_cell_v == CELL_V_MAX_CHARGE -> clamp to 0, no negative command.
    EnterChargeStateClean();
    fakes::adbms::setCellVoltage(0, 0, 4.20f);
    LetTimePass(500); // Need the adbms task to run to update max cell voltage
    LetTimePass(20);

    ASSERT_STATE_EQ(charge_state);
    ASSERT_NEAR(app::can_tx::BMS_MaxChargingCurrent_get(), 0.0f, 0.02f);
    ASSERT_FALSE(app::can_tx::BMS_StopCharging_get());
}

TEST_F(BmsChargeStateTest, goes_fault_state_after_exceeding_max_cell_voltage)
{
    EnterChargeStateClean();
    fakes::adbms::setCellVoltage(0, 0, 4.25f);
    app::can_rx::Elcon_OutputCurrent_update(2.0f);
    // adbms task to update max cell voltage, then 100Hz tick to detect and transition
    LetTimePass(1520);

    ASSERT_STATE_EQ(fault_state);
    ASSERT_TRUE(app::can_tx::BMS_StopCharging_get());
    ASSERT_NEAR(app::can_tx::BMS_MaxChargingVoltage_get(), 0.0f, 0.02f);
    ASSERT_NEAR(app::can_tx::BMS_MaxChargingCurrent_get(), 0.0f, 0.02f);
}

// TEST_F(BmsChargeStateTest, voltage_command_is_pack_voltage_max_in_normal_operation)
// {
//     EnterChargeStateClean();
//     fakes::adbms::setCellVoltage(0, 0, 3.8f);
//     LetTimePass(10);

//     ASSERT_FLOAT_EQ(app::can_tx::BMS_MaxChargingVoltage_get(), kPackVoltageMax);
//     ASSERT_FALSE(app::can_tx::BMS_StopCharging_get());
// }

// =============================================================================
// End-to-end: init -> precharge_charge -> charge -> (terminated) -> init
// =============================================================================

// TEST_F(BmsChargeStateTest, full_charge_cycle_init_precharge_charge_terminate_back_to_init)
// {
//     // Pack at a realistic mid-charge: 14 cells * 10 segments * 3.8V = 532V.
//     constexpr float pack_voltage = 3.8f * 10.0f * 14.0f;

//     // ----- 1. init -----
//     ASSERT_STATE_EQ(init_state);

//     // ----- 2. trigger init -> precharge_charge -----
//     fakes::segments::setPackVoltageEvenly(pack_voltage);
//     fakes::irs::setNegativeState(ContactorState::CONTACTOR_STATE_CLOSED);
//     fakes::charger::setConnectionStatus(ChargerConnectedType::CHARGER_CONNECTED_EVSE);
//     fakes::charger::setCPDutyCycle(85.0f);
//     fakes::ts::setVoltage(0.0f);
//     fakes::segments::setMaxCellVoltage(3.8f);
//     fakes::segments::setMaxCellTemp(25.0f);
//     app::can_rx::Debug_StartCharging_update(true);
//     LetTimePass(20);
//     ASSERT_STATE_EQ(precharge_charge_state);
//     ASSERT_EQ(io::irs::prechargeState(), ContactorState::CONTACTOR_STATE_CLOSED);

//     // ----- 3. wait for precharge lower bound, drive TS up, precharge_charge -> charge -----
//     constexpr uint32_t tolerance_ms = 500;
//     LetTimePass(PRECHARGE_COMPLETION_LOWER_BOUND + tolerance_ms);
//     ASSERT_STATE_EQ(precharge_charge_state); // still running until TS rises
//     fakes::ts::setVoltage(pack_voltage);
//     LetTimePass(20);
//     ASSERT_STATE_EQ(charge_state);
//     ASSERT_EQ(io::irs::positiveState(), ContactorState::CONTACTOR_STATE_CLOSED);
//     ASSERT_EQ(io::irs::prechargeState(), ContactorState::CONTACTOR_STATE_OPEN);
//     ASSERT_FALSE(app::can_tx::BMS_ChargingDone_get());
//     ASSERT_FALSE(app::can_tx::BMS_ChargingFaulted_get());

//     // Sanity: charge state actually commanding non-stop frame
//     LetTimePass(10);
//     ASSERT_FALSE(app::can_tx::BMS_StopCharging_get());
//     ASSERT_FLOAT_EQ(app::can_tx::BMS_MaxChargingVoltage_get(), kPackVoltageMax);
//     ASSERT_GT(app::can_tx::BMS_MaxChargingCurrent_get(), 0.0f);

//     // ----- 4. cells reach termination -> charge -> init -----
//     fakes::segments::setMaxCellVoltage(4.18f);
//     app::can_rx::Elcon_OutputCurrent_update(0.5f);
//     LetTimePass(10);

//     ASSERT_STATE_EQ(init_state);
//     ASSERT_TRUE(app::can_tx::BMS_ChargingDone_get());
//     ASSERT_EQ(io::irs::positiveState(), ContactorState::CONTACTOR_STATE_OPEN);
//     ASSERT_FALSE(app::can_rx::Debug_StartCharging_get()); // cleared on exit

//     // ----- 5. stay in init: Debug_StartCharging was cleared, so no re-entry -----
//     LetTimePass(50);
//     ASSERT_STATE_EQ(init_state);
// }
