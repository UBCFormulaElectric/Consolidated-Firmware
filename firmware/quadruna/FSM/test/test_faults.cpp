#include "test_fsmBaseStateMachineTest.h"
#include <cmath>

class FsmFaultsTest : public FsmBaseStateMachineTest
{
};

TEST_F(FsmFaultsTest, check_state_transition_fault_state_heartbeat_timeout)
{
    // Test that a missing heartbeat will put the FSM into fault state. The FSM only monitors the BMS' heartbeat.

    // Re-enable heartbeat module.
    app_heartbeatMonitor_blockFaults(false);

    // Start with a non-zero pedal positions to prevent false positive
    int time_ms = 0;
    fake_io_apps_getPrimary_returns(50);
    fake_io_apps_getSecondary_returns(50);
    LetTimePass(10);

    // Check in all heartbeats within timeout period
    time_ms += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U;
    fake_io_time_getCurrentMs_returns(time_ms);
    LetTimePass(HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState());
    ASSERT_FALSE(app_canAlerts_FSM_Fault_MissingBMSHeartbeat_get());
    ASSERT_NEAR(50, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
    ASSERT_NEAR(50, app_canTx_FSM_SappsMappedPedalPercentage_get(), 0.5f);

    app_canRx_BMS_Heartbeat_update(true); // Check in heartbeat
    time_ms += 10;
    fake_io_time_getCurrentMs_returns(time_ms);
    LetTimePass(10);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState());
    ASSERT_FALSE(app_canAlerts_FSM_Fault_MissingBMSHeartbeat_get());
    ASSERT_NEAR(50, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
    ASSERT_NEAR(50, app_canTx_FSM_SappsMappedPedalPercentage_get(), 0.5f);

    // Fail to check heartbeat, FSM should fault
    time_ms += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U;
    fake_io_time_getCurrentMs_returns(time_ms);
    LetTimePass(HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState());
    ASSERT_FALSE(app_canAlerts_FSM_Fault_MissingBMSHeartbeat_get());
    ASSERT_NEAR(50, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
    ASSERT_NEAR(50, app_canTx_FSM_SappsMappedPedalPercentage_get(), 0.5f);

    time_ms += 20;
    fake_io_time_getCurrentMs_returns(time_ms);
    LetTimePass(20);
    ASSERT_TRUE(app_canAlerts_FSM_Fault_MissingBMSHeartbeat_get());
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_PappsMappedPedalPercentage_get());
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_SappsMappedPedalPercentage_get());

    // Stay faulted indefinitely
    time_ms += 1000;
    fake_io_time_getCurrentMs_returns(time_ms);
    LetTimePass(1000);
    ASSERT_TRUE(app_canAlerts_FSM_Fault_MissingBMSHeartbeat_get());
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_PappsMappedPedalPercentage_get());
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_SappsMappedPedalPercentage_get());

    // Check heartbeat back in, fault should clear and transition back to init
    app_canRx_BMS_Heartbeat_update(true); // Check in heartbeat
    time_ms += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS;
    fake_io_time_getCurrentMs_returns(time_ms);
    LetTimePass(HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS);
    ASSERT_EQ(app_mainState_get(), app_stateMachine_getCurrentState());
    ASSERT_FALSE(app_canAlerts_FSM_Fault_MissingBMSHeartbeat_get());
    ASSERT_NEAR(50, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
    ASSERT_NEAR(50, app_canTx_FSM_SappsMappedPedalPercentage_get(), 0.5f);
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
    LetTimePass(10);

    fake_io_apps_isPrimaryOCSC_returns(true);
    LetTimePass(10);

    // Check before signal time has elapsed
    LetTimePass(PAPPS_OCSC_TIME_TO_FAULT - 1);
    ASSERT_NEAR(50, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
    ASSERT_NEAR(50, app_canTx_FSM_SappsMappedPedalPercentage_get(), 0.5f);
    ASSERT_FALSE(app_canAlerts_FSM_Fault_PappsOCSC_get());

    // Check after signal time has elapsed
    LetTimePass(1);
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_PappsMappedPedalPercentage_get());
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_SappsMappedPedalPercentage_get());
    ASSERT_TRUE(app_canAlerts_FSM_Fault_PappsOCSC_get());

    // Check faulted indefinitely
    LetTimePass(1000);
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_PappsMappedPedalPercentage_get());
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_SappsMappedPedalPercentage_get());
    ASSERT_TRUE(app_canAlerts_FSM_Fault_PappsOCSC_get());

    // Clear condition, confirm fault resets
    fake_io_apps_isPrimaryOCSC_returns(false);

    LetTimePass(10 + PAPPS_OCSC_TIME_TO_CLEAR - 1);
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_PappsMappedPedalPercentage_get());
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_SappsMappedPedalPercentage_get());
    ASSERT_TRUE(app_canAlerts_FSM_Fault_PappsOCSC_get());

    LetTimePass(1);
    ASSERT_NEAR(50, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
    ASSERT_NEAR(50, app_canTx_FSM_SappsMappedPedalPercentage_get(), 0.5f);
    ASSERT_FALSE(app_canAlerts_FSM_Fault_PappsOCSC_get());
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
    LetTimePass(10);

    fake_io_apps_isSecondaryOCSC_returns(true);
    LetTimePass(10);

    // Check before signal time has elapsed
    LetTimePass(SAPPS_OCSC_TIME_TO_FAULT - 1);
    ASSERT_FALSE(app_canAlerts_FSM_Fault_SappsOCSC_get());
    ASSERT_NEAR(50, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
    ASSERT_NEAR(50, app_canTx_FSM_SappsMappedPedalPercentage_get(), 0.5f);

    // Check after signal time has elapsed
    LetTimePass(1);
    ASSERT_TRUE(app_canAlerts_FSM_Fault_SappsOCSC_get());
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_PappsMappedPedalPercentage_get());
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_SappsMappedPedalPercentage_get());

    // Check faulted indefinitely
    LetTimePass(1000);
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_PappsMappedPedalPercentage_get());
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_SappsMappedPedalPercentage_get());
    ASSERT_TRUE(app_canAlerts_FSM_Fault_SappsOCSC_get());

    // Clear condition, confirm fault resets
    fake_io_apps_isSecondaryOCSC_returns(false);

    LetTimePass(10 + PAPPS_OCSC_TIME_TO_CLEAR - 1);
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_PappsMappedPedalPercentage_get());
    ASSERT_FLOAT_EQ(0, app_canTx_FSM_SappsMappedPedalPercentage_get());
    ASSERT_TRUE(app_canAlerts_FSM_Fault_SappsOCSC_get());

    LetTimePass(1);
    ASSERT_NEAR(50, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
    ASSERT_NEAR(50, app_canTx_FSM_SappsMappedPedalPercentage_get(), 0.5f);
    ASSERT_FALSE(app_canAlerts_FSM_Fault_SappsOCSC_get());
}

TEST_F(FsmFaultsTest, apps_disagreement_sets_mapped_pedal_percentage_to_zero_and_sets_fault)
{
    struct
    {
        float papps_percentage;
        float sapps_percentage;
        bool  expect_fault;
    } test_params[] = {
        {
            // Papps not greater than sapps
            .papps_percentage = 10,
            .sapps_percentage = 0,
            .expect_fault     = false,
        },
        // {
        //     // Papps just greater than sapps
        //     .papps_percentage = 11,
        //     .sapps_percentage = 0,
        //     .expect_fault     = true,
        // },
        // {
        //     // Papps much greater than sapps
        //     .papps_percentage = 75,
        //     .sapps_percentage = 25,
        //     .expect_fault     = true,
        // },
        {
            // Sapps not greater than papps
            .papps_percentage = 0,
            .sapps_percentage = 10,
            .expect_fault     = false,
        },
        // {
        //     // Sapps just greater than papps
        //     .papps_percentage = 0,
        //     .sapps_percentage = 11,
        //     .expect_fault     = true,
        // },
        // {
        //     // Sapps much greater than sapps
        //     .papps_percentage = 25,
        //     .sapps_percentage = 75,
        //     .expect_fault     = true,
        // },
        {
            // Control: papps = sapps
            .papps_percentage = 50,
            .sapps_percentage = 50,
            .expect_fault     = false,
        }
    };

    int test_params_len = sizeof(test_params) / sizeof(test_params[0]);

    for (int i = 0; i < test_params_len; i++)
    {
        TearDown();
        SetUp();

        fake_io_apps_getPrimary_returns(test_params[i].papps_percentage);
        fake_io_apps_getSecondary_returns(test_params[i].sapps_percentage);

        LetTimePass(10 + AGREEMENT_TIME_TO_FAULT - 1);
        ASSERT_NEAR(test_params[i].papps_percentage, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
        ASSERT_NEAR(test_params[i].sapps_percentage, app_canTx_FSM_SappsMappedPedalPercentage_get(), 0.5f);
        ASSERT_FALSE(app_canAlerts_FSM_Warning_AppsDisagreement_get());

        if (test_params[i].expect_fault)
        {
            // Signal debounce has elapsed, expect fault
            LetTimePass(1);
            ASSERT_EQ(0, app_canTx_FSM_PappsMappedPedalPercentage_get());
            ASSERT_EQ(0, app_canTx_FSM_SappsMappedPedalPercentage_get());
            ASSERT_TRUE(app_canAlerts_FSM_Warning_AppsDisagreement_get());

            // Check faulted indefinitely
            LetTimePass(1000);
            ASSERT_FLOAT_EQ(0, app_canTx_FSM_PappsMappedPedalPercentage_get());
            ASSERT_FLOAT_EQ(0, app_canTx_FSM_SappsMappedPedalPercentage_get());
            ASSERT_TRUE(app_canAlerts_FSM_Warning_AppsDisagreement_get());

            // Clear condition, confirm fault resets
            fake_io_apps_getPrimary_returns(test_params[i].papps_percentage);
            fake_io_apps_getSecondary_returns(
                test_params[i].papps_percentage); // Set sapps to papps, so there is agreement

            LetTimePass(10 + AGREEMENT_TIME_TO_CLEAR - 1);
            ASSERT_FLOAT_EQ(0, app_canTx_FSM_PappsMappedPedalPercentage_get());
            ASSERT_FLOAT_EQ(0, app_canTx_FSM_SappsMappedPedalPercentage_get());
            ASSERT_TRUE(app_canAlerts_FSM_Warning_AppsDisagreement_get());

            LetTimePass(1);
            ASSERT_NEAR(test_params[i].papps_percentage, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
            ASSERT_NEAR(test_params[i].papps_percentage, app_canTx_FSM_SappsMappedPedalPercentage_get(), 0.5f);
            ASSERT_FALSE(app_canAlerts_FSM_Warning_AppsDisagreement_get());
        }
        else
        {
            // No fault condition, confirm no fault is set
            LetTimePass(1000);
            ASSERT_NEAR(test_params[i].papps_percentage, app_canTx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
            ASSERT_NEAR(test_params[i].sapps_percentage, app_canTx_FSM_SappsMappedPedalPercentage_get(), 0.5f);
            ASSERT_FALSE(app_canAlerts_FSM_Warning_AppsDisagreement_get());
        }
    }
}