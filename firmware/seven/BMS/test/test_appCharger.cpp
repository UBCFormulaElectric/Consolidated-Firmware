#include <gtest/gtest.h>

#include "ecuTestBase.hpp"
#include "app_charger.hpp"
#include "test_fakes.hpp"
#include "app_canRx.hpp"
#include "app_canTx.hpp"

class AppChargerTest : public EcuTestBase
{
  protected:
    void board_setup() override
    {
        ClearElconFaults();
        app::can_rx::Elcon_OutputCurrent_update(0.0f);
        app::can_rx::Elcon_OutputVoltage_update(0.0f);

        fakes::charger::setCPDutyCycle(0.0f);
        fakes::charger::setCPFrequency(0.0f);
    }
    void board_teardown() override {}

    void ClearElconFaults()
    {
        app::can_rx::Elcon_HardwareFailure_update(false);
        app::can_rx::Elcon_ChargerOverTemperature_update(false);
        app::can_rx::Elcon_InputVoltageError_update(false);
        app::can_rx::Elcon_ChargingDisabled_update(false);
        app::can_rx::Elcon_CommunicationTimeout_update(false);
    }
};

// =============================================================================
// getOutputCurrent() tests
// =============================================================================

TEST_F(AppChargerTest, getOutputCurrent_returns_elcon_value) 
{
        app::can_rx::Elcon_OutputCurrent_update(10.0f);

        const float output_current = app::charger::getOutputCurrent();

        EXPECT_FLOAT_EQ(output_current, 10.0f);
}

// =============================================================================
// getFaultStatus() tests
// =============================================================================

TEST_F(AppChargerTest, get_fault_status_returns_all_elcon_fault_signals)
{
    app::can_rx::Elcon_HardwareFailure_update(true);
    app::can_rx::Elcon_ChargerOverTemperature_update(false);
    app::can_rx::Elcon_InputVoltageError_update(true);
    app::can_rx::Elcon_ChargingDisabled_update(false);
    app::can_rx::Elcon_CommunicationTimeout_update(true);

    const app::charger::ElconFaultStatus fault_status = app::charger::getFaultStatus();

    EXPECT_TRUE(fault_status.hardware_failure);
    EXPECT_FALSE(fault_status.over_temperature);
    EXPECT_TRUE(fault_status.input_voltage_fault);
    EXPECT_FALSE(fault_status.charging_disabled);
    EXPECT_TRUE(fault_status.comm_timeout);
}

TEST_F(AppChargerTest, fault_status_is_false_when_no_faults_are_active)
{
    ClearElconFaults();

    const app::charger::ElconFaultStatus fault_status = app::charger::getFaultStatus();

    EXPECT_FALSE(static_cast<bool>(fault_status));
}

TEST_F(AppChargerTest, getFaultStatus_returns_true_when_any_faults_are_active) 
{
        ClearElconFaults();
        app::can_rx::Elcon_HardwareFailure_update(true);

        const app::charger::ElconFaultStatus fault_status = app::charger::getFaultStatus();

        EXPECT_TRUE(static_cast<bool>(fault_status));
}

// =============================================================================
// setChargingConfig
// =============================================================================

TEST_F(AppChargerTest, setChargingConfig_updates_can_tx_signals) 
{
    // Arbitrarily chosen values for testing
    app::charger::ElconChargingConfig command;
    command.max_voltage_v = 400.0f;
    command.max_current_a = 8.0f;
    command.stop_charging = true;

    app::charger::setChargingConfig(command);

    EXPECT_FLOAT_EQ(app::can_tx::BMS_MaxChargingVoltage_get(), 400.0f);
    EXPECT_FLOAT_EQ(app::can_tx::BMS_MaxChargingCurrent_get(), 8.0f);
    EXPECT_TRUE(app::can_tx::BMS_StopCharging_get());
}

TEST_F(AppChargerTest, setChargingConfig_writes_stop_charging_cmd) 
{
    app::charger::ElconChargingConfig command;
    command.max_voltage_v = 0.0f;
    command.max_current_a = 0.0f;
    command.stop_charging = true;

    app::charger::setChargingConfig(command);

    EXPECT_FLOAT_EQ(app::can_tx::BMS_MaxChargingVoltage_get(), 0.0f);
    EXPECT_FLOAT_EQ(app::can_tx::BMS_MaxChargingCurrent_get(), 0.0f);
    EXPECT_TRUE(app::can_tx::BMS_StopCharging_get());
}

// =============================================================================
// getAvailableCurrent()
// =============================================================================

TEST_F(AppChargerTest, getAvailableCurrent_returns_zero_when_duty_cycle_outbounds)
{
    const float duty_cycle_out_of_bounds[] = {0.0f, 0.05f, 0.0999f, 0.9601f, 1.0f};
    for (const float duty_cycle : duty_cycle_out_of_bounds) 
    {
        fakes::charger::setCPDutyCycle(duty_cycle);
        const float available_current = app::charger::getAvailableCurrent();
        EXPECT_FLOAT_EQ(available_current, 0.0f);
    }
}

TEST_F(AppChargerTest, getAvailableCurrent_lower_range_values) 
{
    // 10 - 85% inclusive tests: i = duty% * 0.6
    fakes::charger::setCPDutyCycle(0.10f);
    EXPECT_NEAR(app::charger::getAvailableCurrent(), 6.0f, 1e-3f);

    fakes::charger::setCPDutyCycle(0.50f);
    EXPECT_NEAR(app::charger::getAvailableCurrent(), 30.0f, 1e-3f);

    fakes::charger::setCPDutyCycle(0.85f);
    EXPECT_NEAR(app::charger::getAvailableCurrent(), 51.0f, 1e-3f);
}

TEST_F(AppChargerTest, getAvailableCurrent_upper_range_values) 
{
    // 85 - 96% inclusive tests: i = (duty% - 64) * 2.5
    fakes::charger::setCPDutyCycle(0.8501f);
    EXPECT_NEAR(app::charger::getAvailableCurrent(), (85.01f - 64.0f) * 2.5f, 1e-3f);

    fakes::charger::setCPDutyCycle(0.90f);
    EXPECT_NEAR(app::charger::getAvailableCurrent(), 65.0f, 1e-3f);

    fakes::charger::setCPDutyCycle(0.96f);
    EXPECT_NEAR(app::charger::getAvailableCurrent(), 80.0f, 1e-3f);
}

// =============================================================================
// broadcastEvseStatus()
// =============================================================================

TEST_F(AppChargerTest, broadcastEvseStatus_writes_expected_can_tx_signals)
{
    fakes::charger::setCPDutyCycle(0.5f);
    fakes::charger::setCPFrequency(1000.0f);

    app::charger::broadcastEvseStatus();

    EXPECT_FLOAT_EQ(app::can_tx::BMS_EvseAvailableCurrent_get(), 30.0f);
    EXPECT_FLOAT_EQ(app::can_tx::BMS_EvseDutyCycle_get(), 0.5f);
    EXPECT_FLOAT_EQ(app::can_tx::BMS_EvseFrequency_get(), 1000.0f);
}

TEST_F(AppChargerTest, broadcastEvseStatus_reports_zero_current_for_invalid_values) 
{
    fakes::charger::setCPDutyCycle(0.0f);
    fakes::charger::setCPFrequency(0.0f);

    app::charger::broadcastEvseStatus();

    EXPECT_FLOAT_EQ(app::can_tx::BMS_EvseAvailableCurrent_get(), 0.0f);
    EXPECT_FLOAT_EQ(app::can_tx::BMS_EvseDutyCycle_get(), 0.0f);
}