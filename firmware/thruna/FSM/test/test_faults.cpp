#include "test_fsmBaseStateMachineTest.h"
#include <cmath>

class FsmFaultsTest : public FsmBaseStateMachineTest
{
};

TEST_F(FsmFaultsTest, check_state_transition_fault_state_heartbeat_timeout)
{
    // Test that a missing heartbeat will put the FSM into fault state. The FSM only monitors the BMS' heartbeat.

    // Start with a non-zero pedal positions to prevent false positive
    int time_ms = 0;
    fake_io_apps_getPrimary_returns(50);
    fake_io_apps_getSecondary_returns(50);
    LetTimePass(state_machine, 10);

    // Check in all heartbeats within timeout period
    time_ms += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U;
    fake_io_time_getCurrentMs_returns(time_ms);
    LetTimePass(state_machine, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_FSM_Fault_MissingHeartbeat_Get());
    ASSERT_NEAR(50, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);

    App_CanRx_BMS_Heartbeat_Update(true); // Check in heartbeat
    get_current_ms_fake.return_val += 10;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_FSM_Fault_MissingHeartbeat_Get());
    ASSERT_NEAR(50, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);

    // Fail to check heartbeat, FSM should fault
    get_current_ms_fake.return_val += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U;
    LetTimePass(state_machine, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_FSM_Fault_MissingHeartbeat_Get());
    ASSERT_NEAR(50, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);

    time_ms += 20;
    fake_io_time_getCurrentMs_returns(time_ms);
    LetTimePass(state_machine, 20);
    ASSERT_EQ(App_GetFaultState(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_FSM_Fault_MissingHeartbeat_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());

    // Stay faulted indefinitely
    time_ms += 1000;
    fake_io_time_getCurrentMs_returns(time_ms);
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetFaultState(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_FSM_Fault_MissingHeartbeat_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());

    // Check heartbeat back in, fault should clear and transition back to init
    App_CanRx_BMS_Heartbeat_Update(true); // Check in heartbeat
    get_current_ms_fake.return_val += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS;
    LetTimePass(state_machine, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_FSM_Fault_MissingHeartbeat_Get());
    ASSERT_NEAR(50, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);
}

TEST_F(FsmFaultsTest, papps_ocsc_sets_mapped_pedal_percentage_to_zero)
{
    // For the following test we will select a secondary APPS sensor
    // value such that the difference between the primary and secondary APPS
    // is within 10%. This prevents the APPS has disagreement callback from
    // being triggered

    // Start with a non-zero pedal positions to prevent false positive
    fake_io_apps_getPrimary_returns(50);
    fake_io_apps_getSecondary_returns(50);
    LetTimePass(state_machine, 10);

    fake_io_apps_isPrimaryOCSC_returns(true);
    LetTimePass(state_machine, 10);

    // Check before signal time has elapsed
    LetTimePass(state_machine, PAPPS_OCSC_TIME_TO_FAULT - 1);
    ASSERT_NEAR(50, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_FALSE(App_CanAlerts_FSM_Fault_PappsOCSC_Get());

    // Check after signal time has elapsed
    LetTimePass(state_machine, 1);
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanAlerts_FSM_Fault_PappsOCSC_Get());

    // Check faulted indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanAlerts_FSM_Fault_PappsOCSC_Get());

    // Clear condition, confirm fault resets
    fake_io_apps_isPrimaryOCSC_returns(false);

    LetTimePass(state_machine, 10 + PAPPS_OCSC_TIME_TO_CLEAR - 1);
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanAlerts_FSM_Fault_PappsOCSC_Get());

    LetTimePass(state_machine, 1);
    ASSERT_NEAR(50, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_FALSE(App_CanAlerts_FSM_Fault_PappsOCSC_Get());
}

TEST_F(FsmFaultsTest, sapps_ocsc_sets_mapped_pedal_percentage_to_zero)
{
    // For the following test we will select a secondary APPS sensor
    // value such that the difference between the primary and secondary APPS
    // is within 10%. This prevents the APPS has disagreement callback from
    // being triggered

    // Start with a non-zero pedal position to avoid false positives
    fake_io_apps_getPrimary_returns(50);
    fake_io_apps_getSecondary_returns(50);
    LetTimePass(state_machine, 10);

    fake_io_apps_isSecondaryOCSC_returns(true);
    LetTimePass(state_machine, 10);

    // Check before signal time has elapsed
    LetTimePass(state_machine, SAPPS_OCSC_TIME_TO_FAULT - 1);
    ASSERT_FALSE(App_CanAlerts_FSM_Fault_SappsOCSCFault_Get());
    ASSERT_NEAR(50, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);

    // Check after signal time has elapsed
    LetTimePass(state_machine, 1);
    ASSERT_TRUE(App_CanAlerts_FSM_Fault_SappsOCSCFault_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());

    // Check faulted indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanAlerts_FSM_Fault_SappsOCSCFault_Get());

    // Clear condition, confirm fault resets
    fake_io_apps_isSecondaryOCSC_returns(false);

    LetTimePass(state_machine, 10 + PAPPS_OCSC_TIME_TO_CLEAR - 1);
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanAlerts_FSM_Fault_SappsOCSCFault_Get());

    LetTimePass(state_machine, 1);
    ASSERT_NEAR(50, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_FALSE(App_CanAlerts_FSM_Fault_SappsOCSCFault_Get());
}

TEST_F(FsmFaultsTest, apps_disagreement_sets_mapped_pedal_percentage_to_zero_and_sets_fault)
{
    struct
    {
        float papps_percentage;
        float sapps_percentage;
        bool  expect_fault;
    } test_params[7] = {
        {
            // Papps not greater than sapps
            .papps_percentage = 10,
            .sapps_percentage = 0,
            .expect_fault     = false,
        },
        {
            // Papps just greater than sapps
            .papps_percentage = 11,
            .sapps_percentage = 0,
            .expect_fault     = true,
        },
        {
            // Papps much greater than sapps
            .papps_percentage = 75,
            .sapps_percentage = 25,
            .expect_fault     = true,
        },
        {
            // Sapps not greater than papps
            .papps_percentage = 0,
            .sapps_percentage = 10,
            .expect_fault     = false,
        },
        {
            // Sapps just greater than papps
            .papps_percentage = 0,
            .sapps_percentage = 11,
            .expect_fault     = true,
        },
        {
            // Sapps much greater than sapps
            .papps_percentage = 25,
            .sapps_percentage = 75,
            .expect_fault     = true,
        },
        {
            // Control: papps = sapps
            .papps_percentage = 50,
            .sapps_percentage = 50,
            .expect_fault     = false,
        },
    };

    for (int i = 0; i < 5; i++)
    {
        TearDown();
        SetUp();

        fake_io_apps_getPrimary_returns(test_params[i].papps_percentage);
        fake_io_apps_getSecondary_returns(test_params[i].sapps_percentage);

        LetTimePass(state_machine, 10 + AGREEMENT_TIME_TO_FAULT - 1);
        ASSERT_NEAR(test_params[i].papps_percentage, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
        ASSERT_NEAR(test_params[i].sapps_percentage, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);
        ASSERT_FALSE(App_CanAlerts_FSM_Warning_AppsDisagreement_Get());

        if (test_params[i].expect_fault)
        {
            // Signal debounce has elapsed, expect fault
            LetTimePass(state_machine, 1);
            ASSERT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
            ASSERT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_FSM_Warning_AppsDisagreement_Get());

            // Check faulted indefinitely
            LetTimePass(state_machine, 1000);
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_FSM_Warning_AppsDisagreement_Get());

            // Clear condition, confirm fault resets
            fake_io_apps_getPrimary_returns(test_params[i].papps_percentage);
            fake_io_apps_getSecondary_returns(
                test_params[i].papps_percentage); // Set sapps to papps, so there is agreement

            LetTimePass(state_machine, 10 + AGREEMENT_TIME_TO_CLEAR - 1);
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_FSM_Warning_AppsDisagreement_Get());

            LetTimePass(state_machine, 1);
            ASSERT_NEAR(test_params[i].papps_percentage, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_NEAR(test_params[i].papps_percentage, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_FALSE(App_CanAlerts_FSM_Warning_AppsDisagreement_Get());
        }
        else
        {
            // No fault condition, confirm no fault is set
            LetTimePass(state_machine, 1000);
            ASSERT_NEAR(test_params[i].papps_percentage, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_NEAR(test_params[i].sapps_percentage, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_FALSE(App_CanAlerts_FSM_Warning_AppsDisagreement_Get());
        }
    }
}

TEST_F(FsmFaultsTest, brake_actuated_sets_mapped_pedal_percentage_to_zero_and_sets_fault_if_papps_pressed)
{
    struct
    {
        float apps_percentage;
        bool  brake_actuated;
        bool  expect_fault;
    } test_params[5] = {
        { 0, false, false },  // Papps not pressed and brake not actuated, no fault
        { 24, true, false },  // Papps not pressed enough, no fault
        { 26, false, false }, // Brake not actuated, no fault
        { 26, true, true },   // Papps pressed enough + brake actuated, fault
        { 75, true, true },   // Papps pressed significantly + brake actuated, fault
    };

    for (int i = 0; i < 5; i++)
    {
        TearDown();
        SetUp();

        // Actuate brake, and set peddal percentage
        fake_io_apps_getPrimary_returns(test_params[i].apps_percentage);
        fake_io_apps_getSecondary_returns(test_params[i].apps_percentage);
        fake_io_brake_isActuated_returns(test_params[i].brake_actuated);
        LetTimePass(state_machine, 10 + APP_BRAKE_TIME_TO_FAULT - 1);
        ASSERT_NEAR(test_params[i].apps_percentage, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
        ASSERT_NEAR(test_params[i].apps_percentage, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);
        ASSERT_FALSE(App_CanAlerts_FSM_Warning_BrakeAppsDisagreement_Get());

        if (test_params[i].expect_fault)
        {
            // Signal debounce has elapsed, expect fault
            LetTimePass(state_machine, 1);
            ASSERT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
            ASSERT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_FSM_Warning_BrakeAppsDisagreement_Get());

            // Check faulted indefinitely
            LetTimePass(state_machine, 1000);
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_FSM_Warning_BrakeAppsDisagreement_Get());

            // Clear condition, confirm fault resets
            fake_io_apps_getPrimary_returns(4); // Primary must be <5% to clear
            fake_io_apps_getSecondary_returns(4);

            LetTimePass(state_machine, 10 + APP_BRAKE_TIME_TO_CLEAR - 1);
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_PappsMappedPedalPercentage_Get());
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_FSM_Warning_BrakeAppsDisagreement_Get());

            LetTimePass(state_machine, 1);
            ASSERT_NEAR(4, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_NEAR(4, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_FALSE(App_CanAlerts_FSM_Warning_BrakeAppsDisagreement_Get());
        }
        else
        {
            // No fault condition, confirm no fault is set
            LetTimePass(state_machine, 1000);
            ASSERT_NEAR(test_params[i].apps_percentage, App_CanTx_FSM_PappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_NEAR(test_params[i].apps_percentage, App_CanTx_FSM_SappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_FALSE(App_CanAlerts_FSM_Warning_BrakeAppsDisagreement_Get());
        }
    }
}

TEST_F(FsmFaultsTest, primary_flow_rate_underflow_sets_fault)
{
    App_CanRx_DCM_State_Update(DCM_DRIVE_STATE);
    // Flow rate underflow threshold is 1.0 L/min
    const float underflow_threshold = 1.0f;
    fake_io_coolant_getFlowRate_returns(std::nextafter(underflow_threshold, std::numeric_limits<float>::lowest()));

    LetTimePass(state_machine, 10 + FLOW_METER_TIME_TO_FAULT - 1);
    ASSERT_FALSE(App_CanAlerts_FSM_Fault_FlowMeterUnderflow_Get());
    LetTimePass(state_machine, 1);
    ASSERT_TRUE(App_CanAlerts_FSM_Fault_FlowMeterUnderflow_Get());

    // Confirm fault set indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_TRUE(App_CanAlerts_FSM_Fault_FlowMeterUnderflow_Get());

    // Clear condition, confirm fault resets
    fake_io_coolant_getFlowRate_returns(std::nextafter(underflow_threshold, std::numeric_limits<float>::max()));

    LetTimePass(state_machine, 10 + FLOW_METER_TIME_TO_CLEAR - 1);
    ASSERT_TRUE(App_CanAlerts_FSM_Fault_FlowMeterUnderflow_Get());
    LetTimePass(state_machine, 1);
    ASSERT_FALSE(App_CanAlerts_FSM_Fault_FlowMeterUnderflow_Get());
}

TEST_F(FsmFaultsTest, brake_pedal_ocsc_sets_warning_and_brake_travel_to_zero)
{
    fake_io_brake_getPedalPercentTravel_returns(30);
    LetTimePass(state_machine, 10);
    ASSERT_NEAR(30, App_CanTx_FSM_BrakePedalPercentage_Get(), 0.5f);
    ASSERT_FALSE(App_CanTx_FSM_BrakePedalSensorOCSC_Get());

    // Set brake OCSC
    fake_io_brake_pedalSensorOCSC_returns(true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(0, App_CanTx_FSM_BrakePedalPercentage_Get());
    ASSERT_TRUE(App_CanTx_FSM_BrakePedalSensorOCSC_Get());

    // Confirm set indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(0, App_CanTx_FSM_BrakePedalPercentage_Get());
    ASSERT_TRUE(App_CanTx_FSM_BrakePedalSensorOCSC_Get());

    // Clear condition, confirm warning resets
    fake_io_brake_pedalSensorOCSC_returns(false);
    LetTimePass(state_machine, 10);
    ASSERT_NEAR(30, App_CanTx_FSM_BrakePedalPercentage_Get(), 0.5f);
    ASSERT_FALSE(App_CanTx_FSM_BrakePedalSensorOCSC_Get());
}

TEST_F(FsmFaultsTest, steering_sensor_ocsc_sets_warning)
{
    LetTimePass(state_machine, 10);
    ASSERT_FALSE(App_CanAlerts_FSM_Warning_SteeringAngleOCSC_Get());

    // Set steering wheel OCSC
    fake_io_steering_sensorOCSC_returns(true);
    LetTimePass(state_machine, 10);
    ASSERT_TRUE(App_CanAlerts_FSM_Warning_SteeringAngleOCSC_Get());

    // Confirm set indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_TRUE(App_CanAlerts_FSM_Warning_SteeringAngleOCSC_Get());

    // Clear condition, confirm warning resets
    fake_io_steering_sensorOCSC_returns(false);
    LetTimePass(state_machine, 10);
    ASSERT_FALSE(App_CanAlerts_FSM_Warning_SteeringAngleOCSC_Get());
}
