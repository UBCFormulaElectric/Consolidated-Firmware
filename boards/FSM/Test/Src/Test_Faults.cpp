#include <cmath>
#include "Test_Fsm.h"
#include "Test_BaseStateMachineTest.h"

extern "C"
{
#include "App_SharedMacros.h"
#include "App_SharedStateMachine.h"
#include "App_SharedHeartbeatMonitor.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "App_Timer.h"
#include "configs/App_AcceleratorSignalTimings.h"
#include "configs/App_FlowRateThresholds.h"
#include "configs/App_CoolantSignalTimings.h"
#include "configs/App_WheelSpeedThresholds.h"
#include "configs/App_SteeringAngleThresholds.h"
#include "configs/App_BrakePressureThresholds.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
}

namespace FaultTest
{
FAKE_VALUE_FUNC(uint32_t, get_current_ms)
FAKE_VALUE_FUNC(float, coolant_get_flow_rate)
FAKE_VALUE_FUNC(float, coolant_get_temp_a)
FAKE_VALUE_FUNC(float, coolant_get_temp_b)
FAKE_VALUE_FUNC(float, coolant_get_pressure_a)
FAKE_VALUE_FUNC(float, coolant_get_pressure_b)
FAKE_VALUE_FUNC(float, wheel_get_left_speed)
FAKE_VALUE_FUNC(float, wheel_get_right_speed)
FAKE_VALUE_FUNC(float, get_steering_angle)
FAKE_VALUE_FUNC(bool, steering_OCSC)
FAKE_VALUE_FUNC(float, brake_get_front_pressure)
FAKE_VALUE_FUNC(float, brake_get_rear_pressure)
FAKE_VALUE_FUNC(float, brake_get_pedal_travel)
FAKE_VALUE_FUNC(bool, brake_front_pressure_ocsc)
FAKE_VALUE_FUNC(bool, brake_rear_pressure_ocsc)
FAKE_VALUE_FUNC(bool, brake_pedal_ocsc)
FAKE_VALUE_FUNC(bool, is_brake_actuated)
FAKE_VALUE_FUNC(float, get_papps)
FAKE_VALUE_FUNC(bool, papps_ocsc)
FAKE_VALUE_FUNC(float, get_sapps)
FAKE_VALUE_FUNC(bool, sapps_ocsc)

class FsmFaultTest : public BaseStateMachineTest
{
  protected:
    void SetUp() override
    {
        BaseStateMachineTest::SetUp();

        App_CanTx_Init();
        App_CanRx_Init();

        heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
            get_current_ms, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, HEARTBEAT_MONITOR_BOARDS_TO_CHECK);

        papps_and_sapps = App_AcceleratorPedals_Create(get_papps, papps_ocsc, get_sapps, sapps_ocsc);
        brake           = App_Brake_Create(
            brake_get_front_pressure, brake_front_pressure_ocsc, brake_get_rear_pressure, brake_rear_pressure_ocsc,
            brake_get_pedal_travel, brake_pedal_ocsc, is_brake_actuated);
        coolant = App_Coolant_Create(
            coolant_get_flow_rate, coolant_get_temp_a, coolant_get_temp_b, coolant_get_pressure_a,
            coolant_get_pressure_b);
        steering = App_Steering_Create(get_steering_angle, steering_OCSC);
        wheels   = App_Wheels_Create(wheel_get_left_speed, wheel_get_right_speed);
        world    = App_FsmWorld_Create(heartbeat_monitor, papps_and_sapps, brake, coolant, steering, wheels);

        // Default to starting the state machine in the Drive state
        state_machine = App_SharedStateMachine_Create(world, App_GetDriveState());

        // Reset fake functions
        RESET_FAKE(get_current_ms)
        RESET_FAKE(coolant_get_flow_rate)
        RESET_FAKE(coolant_get_temp_a)
        RESET_FAKE(coolant_get_temp_b)
        RESET_FAKE(coolant_get_pressure_a)
        RESET_FAKE(coolant_get_pressure_b)
        RESET_FAKE(wheel_get_left_speed)
        RESET_FAKE(wheel_get_right_speed)
        RESET_FAKE(get_steering_angle)
        RESET_FAKE(steering_OCSC)
        RESET_FAKE(brake_get_front_pressure)
        RESET_FAKE(brake_get_rear_pressure)
        RESET_FAKE(brake_get_pedal_travel)
        RESET_FAKE(brake_front_pressure_ocsc)
        RESET_FAKE(brake_rear_pressure_ocsc)
        RESET_FAKE(brake_pedal_ocsc)
        RESET_FAKE(is_brake_actuated)
        RESET_FAKE(get_papps)
        RESET_FAKE(papps_ocsc)
        RESET_FAKE(get_sapps)
        RESET_FAKE(sapps_ocsc)

        App_CanRx_BMS_Vitals_Heartbeat_Update(true);
    }

    void TearDown() override
    {
        TearDownObject(world, App_FsmWorld_Destroy);
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        TearDownObject(heartbeat_monitor, App_SharedHeartbeatMonitor_Destroy);
        TearDownObject(papps_and_sapps, App_AcceleratorPedals_Destroy);
        TearDownObject(brake, App_Brake_Destroy);
        TearDownObject(coolant, App_Coolant_Destroy);
        TearDownObject(steering, App_Steering_Destroy);
        TearDownObject(wheels, App_Wheels_Destroy);
    }

    void UpdateClock(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        UNUSED(state_machine);
        App_Timer_SetCurrentTimeMS(current_time_ms);
    }

    void UpdateSignals(struct StateMachine *state_machine, uint32_t current_time_ms) override
    {
        UNUSED(state_machine);
        UNUSED(current_time_ms);
    };

    void SetInitialState(const struct State *const initial_state)
    {
        TearDownObject(state_machine, App_SharedStateMachine_Destroy);
        state_machine = App_SharedStateMachine_Create(world, initial_state);
        ASSERT_EQ(initial_state, App_SharedStateMachine_GetCurrentState(state_machine));
    }

    struct World *            world;
    struct StateMachine *     state_machine;
    struct HeartbeatMonitor * heartbeat_monitor;
    struct AcceleratorPedals *papps_and_sapps;
    struct Brake *            brake;
    struct Coolant *          coolant;
    struct Steering *         steering;
    struct Wheels *           wheels;
};

TEST_F(FsmFaultTest, check_state_transition_fault_state_heartbeat_timeout)
{
    // Test that a missing heartbeat will put the FSM into fault state. The FSM only monitors the BMS' heartbeat.

    // Start with a non-zero pedal positions to prevent false positive
    get_papps_fake.return_val = 50;
    get_sapps_fake.return_val = 50;
    LetTimePass(state_machine, 10);

    // Check in all heartbeats within timeout period
    get_current_ms_fake.return_val += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U;
    LetTimePass(state_machine, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U);
    ASSERT_EQ(App_GetDriveState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_MISSING_HEARTBEAT));
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);

    App_CanRx_BMS_Vitals_Heartbeat_Update(true); // Check in heartbeat
    get_current_ms_fake.return_val += 10;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(App_GetDriveState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_MISSING_HEARTBEAT));
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);

    // Fail to check heartbeat, FSM should fault
    get_current_ms_fake.return_val += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U;
    LetTimePass(state_machine, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS - 10U);
    ASSERT_EQ(App_GetDriveState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_MISSING_HEARTBEAT));
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);

    get_current_ms_fake.return_val += 20;
    LetTimePass(state_machine, 20);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_MISSING_HEARTBEAT));
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());

    // Stay faulted indefinitely
    get_current_ms_fake.return_val += 1000;
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(App_GetFaultState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_MISSING_HEARTBEAT));
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());

    // Check heartbeat back in, fault should clear and transition back to init
    App_CanRx_BMS_Vitals_Heartbeat_Update(true); // Check in heartbeat
    get_current_ms_fake.return_val += HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS;
    LetTimePass(state_machine, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS);
    ASSERT_EQ(App_GetDriveState(), App_SharedStateMachine_GetCurrentState(state_machine));
    ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_MISSING_HEARTBEAT));
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);
}

TEST_F(FsmFaultTest, papps_ocsc_sets_mapped_pedal_percentage_to_zero_and_sets_fault)
{
    // For the following test we will select a secondary APPS sensor
    // value such that the difference between the primary and secondary APPS
    // is within 10%. This prevents the APPS has disagreement callback from
    // being triggered

    // Start with a non-zero pedal positions to prevent false positive
    get_papps_fake.return_val = 50;
    get_sapps_fake.return_val = 50;
    LetTimePass(state_machine, 10);

    papps_ocsc_fake.return_val = true;
    LetTimePass(state_machine, 10);

    // Check before signal time has elapsed
    LetTimePass(state_machine, PAPPS_OCSC_TIME_TO_FAULT - 1);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE));

    // Check after signal time has elapsed
    LetTimePass(state_machine, 1);
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE));

    // Check faulted indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE));

    // Clear condition, confirm fault resets
    papps_ocsc_fake.return_val = false;

    LetTimePass(state_machine, 10 + PAPPS_OCSC_TIME_TO_CLEAR - 1);
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE));

    LetTimePass(state_machine, 1);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_PAPPS_IS_OCSC_IS_ACTIVE));
}

TEST_F(FsmFaultTest, sapps_ocsc_sets_mapped_pedal_percentage_to_zero_and_sets_fault)
{
    // For the following test we will select a secondary APPS sensor
    // value such that the difference between the primary and secondary APPS
    // is within 10%. This prevents the APPS has disagreement callback from
    // being triggered

    // Start with a non-zero pedal position to avoid false positives
    get_papps_fake.return_val = 50;
    get_sapps_fake.return_val = 50;
    LetTimePass(state_machine, 10);

    sapps_ocsc_fake.return_val = true;
    LetTimePass(state_machine, 10);

    // Check before signal time has elapsed
    LetTimePass(state_machine, SAPPS_OCSC_TIME_TO_FAULT - 1);
    ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE));
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);

    // Check after signal time has elapsed
    LetTimePass(state_machine, 1);
    ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE));
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());

    // Check faulted indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE));

    // Clear condition, confirm fault resets
    sapps_ocsc_fake.return_val = false;

    LetTimePass(state_machine, 10 + PAPPS_OCSC_TIME_TO_CLEAR - 1);
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
    ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());
    ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE));

    LetTimePass(state_machine, 1);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_NEAR(50, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);
    ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_SAPPS_IS_OCSC_IS_ACTIVE));
}

TEST_F(FsmFaultTest, apps_disagreement_sets_mapped_pedal_percentage_to_zero_and_sets_fault)
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

        get_papps_fake.return_val = test_params[i].papps_percentage;
        get_sapps_fake.return_val = test_params[i].sapps_percentage;

        LetTimePass(state_machine, 10 + AGREEMENT_TIME_TO_FAULT - 1);
        ASSERT_NEAR(test_params[i].papps_percentage, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
        ASSERT_NEAR(test_params[i].sapps_percentage, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);
        ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_APPS_HAS_DISAGREEMENT));

        if (test_params[i].expect_fault)
        {
            // Signal debounce has elapsed, expect fault
            LetTimePass(state_machine, 1);
            ASSERT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
            ASSERT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_APPS_HAS_DISAGREEMENT));

            // Check faulted indefinitely
            LetTimePass(state_machine, 1000);
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_APPS_HAS_DISAGREEMENT));

            // Clear condition, confirm fault resets
            get_papps_fake.return_val = test_params[i].papps_percentage;
            get_sapps_fake.return_val = test_params[i].papps_percentage; // Set sapps to papps, so there is agreement

            LetTimePass(state_machine, 10 + AGREEMENT_TIME_TO_CLEAR - 1);
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_APPS_HAS_DISAGREEMENT));

            LetTimePass(state_machine, 1);
            ASSERT_NEAR(test_params[i].papps_percentage, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_NEAR(test_params[i].papps_percentage, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_APPS_HAS_DISAGREEMENT));
        }
        else
        {
            // No fault condition, confirm no fault is set
            LetTimePass(state_machine, 1000);
            ASSERT_NEAR(test_params[i].papps_percentage, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_NEAR(test_params[i].sapps_percentage, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_APPS_HAS_DISAGREEMENT));
        }
    }
}

TEST_F(FsmFaultTest, brake_actuated_sets_mapped_pedal_percentage_to_zero_and_sets_fault_if_papps_pressed)
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
        get_papps_fake.return_val         = test_params[i].apps_percentage;
        get_sapps_fake.return_val         = test_params[i].apps_percentage;
        is_brake_actuated_fake.return_val = test_params[i].brake_actuated;
        LetTimePass(state_machine, 10 + APP_BRAKE_TIME_TO_FAULT - 1);
        ASSERT_NEAR(test_params[i].apps_percentage, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
        ASSERT_NEAR(test_params[i].apps_percentage, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);
        ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_BRAKE_ACC_DISAGREEMENT));

        if (test_params[i].expect_fault)
        {
            // Signal debounce has elapsed, expect fault
            LetTimePass(state_machine, 1);
            ASSERT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
            ASSERT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_BRAKE_ACC_DISAGREEMENT));

            // Check faulted indefinitely
            LetTimePass(state_machine, 1000);
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_BRAKE_ACC_DISAGREEMENT));

            // Clear condition, confirm fault resets
            get_papps_fake.return_val = 4; // Primary must be <5% to clear
            get_sapps_fake.return_val = 4;

            LetTimePass(state_machine, 10 + APP_BRAKE_TIME_TO_CLEAR - 1);
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get());
            ASSERT_FLOAT_EQ(0, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get());
            ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_BRAKE_ACC_DISAGREEMENT));

            LetTimePass(state_machine, 1);
            ASSERT_NEAR(4, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_NEAR(4, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_BRAKE_ACC_DISAGREEMENT));
        }
        else
        {
            // No fault condition, confirm no fault is set
            LetTimePass(state_machine, 1000);
            ASSERT_NEAR(test_params[i].apps_percentage, App_CanTx_FSM_Apps_PappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_NEAR(test_params[i].apps_percentage, App_CanTx_FSM_Apps_SappsMappedPedalPercentage_Get(), 0.5f);
            ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_BRAKE_ACC_DISAGREEMENT));
        }
    }
}

TEST_F(FsmFaultTest, primary_flow_rate_underflow_sets_fault)
{
    // Flow rate underflow threshold is 1.0 L/min
    const float underflow_threshold       = 1.0f;
    coolant_get_flow_rate_fake.return_val = std::nextafter(underflow_threshold, std::numeric_limits<float>::lowest());

    LetTimePass(state_machine, 10 + FLOW_METER_TIME_TO_FAULT - 1);
    ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_FLOW_METER_HAS_UNDERFLOW));
    LetTimePass(state_machine, 1);
    ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_FLOW_METER_HAS_UNDERFLOW));

    // Confirm fault set indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_FLOW_METER_HAS_UNDERFLOW));

    // Clear condition, confirm fault resets
    coolant_get_flow_rate_fake.return_val = std::nextafter(underflow_threshold, std::numeric_limits<float>::max());

    LetTimePass(state_machine, 10 + FLOW_METER_TIME_TO_CLEAR - 1);
    ASSERT_TRUE(App_CanAlerts_GetFault(FSM_FAULT_FLOW_METER_HAS_UNDERFLOW));
    LetTimePass(state_machine, 1);
    ASSERT_FALSE(App_CanAlerts_GetFault(FSM_FAULT_FLOW_METER_HAS_UNDERFLOW));
}

TEST_F(FsmFaultTest, brake_pedal_ocsc_sets_warning_and_brake_travel_to_zero)
{
    brake_get_pedal_travel_fake.return_val = 30;
    LetTimePass(state_machine, 10);
    ASSERT_NEAR(30, App_CanTx_FSM_Brake_BrakePedalPercentage_Get(), 0.5f);
    ASSERT_FALSE(App_CanTx_FSM_Brake_PedalOpenShortCircuit_Get());

    // Set brake OCSC
    brake_pedal_ocsc_fake.return_val = true;
    LetTimePass(state_machine, 10);
    ASSERT_EQ(0, App_CanTx_FSM_Brake_BrakePedalPercentage_Get());
    ASSERT_TRUE(App_CanTx_FSM_Brake_PedalOpenShortCircuit_Get());

    // Confirm set indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_EQ(0, App_CanTx_FSM_Brake_BrakePedalPercentage_Get());
    ASSERT_TRUE(App_CanTx_FSM_Brake_PedalOpenShortCircuit_Get());

    // Clear condition, confirm warning resets
    brake_pedal_ocsc_fake.return_val = false;
    LetTimePass(state_machine, 10);
    ASSERT_NEAR(30, App_CanTx_FSM_Brake_BrakePedalPercentage_Get(), 0.5f);
    ASSERT_FALSE(App_CanTx_FSM_Brake_PedalOpenShortCircuit_Get());
}

TEST_F(FsmFaultTest, steering_sensor_ocsc_sets_warning)
{
    LetTimePass(state_machine, 10);
    ASSERT_FALSE(App_CanAlerts_GetWarning(FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC));

    // Set steering wheel OCSC
    steering_OCSC_fake.return_val = true;
    LetTimePass(state_machine, 10);
    ASSERT_TRUE(App_CanAlerts_GetWarning(FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC));

    // Confirm set indefinitely
    LetTimePass(state_machine, 1000);
    ASSERT_TRUE(App_CanAlerts_GetWarning(FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC));

    // Clear condition, confirm warning resets
    steering_OCSC_fake.return_val = false;
    LetTimePass(state_machine, 10);
    ASSERT_FALSE(App_CanAlerts_GetWarning(FSM_WARNING_STEERING_ANGLE_SENSOR_OCSC));
}

} // namespace FaultTest
