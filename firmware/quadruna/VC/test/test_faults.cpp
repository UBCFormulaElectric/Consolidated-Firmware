#include "test_vcBaseStateMachineTest.h"
#include "app_faultCheck.h"
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"
#include <cmath>

class VcFaultsTest : public VcBaseStateMachineTest
{
  protected:
    int regen_to_regularMapping(float apps_pedal_percentage)
    {
        apps_pedal_percentage = apps_pedal_percentage < 0.0f
                                    ? apps_pedal_percentage * PEDAL_SCALE
                                    : apps_pedal_percentage * (MAX_PEDAL_PERCENT - PEDAL_SCALE);

        apps_pedal_percentage = apps_pedal_percentage / MAX_PEDAL_PERCENT + PEDAL_SCALE;

        return round(apps_pedal_percentage * 100.0f);
    }
};

TEST_F(VcFaultsTest, brake_actuated_and_regen_switch_off_sets_torque_to_zero_and_sets_fault_if_papps_pressed)
{
    struct
    {
        float       apps_percentage;
        bool        brake_actuated;
        bool        expect_fault;
        SwitchState switch_state;
    } test_params[10] = {
        { 0, false, false, SWITCH_OFF },  // Papps not pressed and brake not actuated, no fault
        { 24, true, false, SWITCH_OFF },  // Papps not pressed enough, no fault
        { 26, false, false, SWITCH_OFF }, // Brake not actuated, no fault
        { 26, true, true, SWITCH_OFF },   // Papps pressed enough + brake actuated, fault
        { 75, true, true, SWITCH_OFF },   // Papps pressed significantly + brake actuated, fault
        // TV On
        { 0, false, false, SWITCH_ON },  // Papps not pressed and brake not actuated, no fault
        { 24, true, false, SWITCH_ON },  // Papps not pressed enough, no fault
        { 26, false, false, SWITCH_ON }, // Brake not actuated, no fault
        { 26, true, true, SWITCH_ON },   // Papps pressed enough + brake actuated, fault
        { 75, true, true, SWITCH_ON },   // Papps pressed significantly + brake actuated, fault
    };

    for (int i = 0; i < 10; i++)
    {
        TearDown();
        SetUp();

        SetStateToDrive();
        LetTimePass(10);
        EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());

        app_canRx_CRIT_RegenSwitch_update(SWITCH_OFF);
        app_canRx_CRIT_TorqueVecSwitch_update(test_params[i].switch_state);
        app_canRx_FSM_PappsMappedPedalPercentage_update(test_params[i].apps_percentage);
        app_canRx_FSM_SappsMappedPedalPercentage_update(test_params[i].apps_percentage);
        app_canRx_FSM_BrakeActuated_update(test_params[i].brake_actuated);
        app_canRx_FSM_SteeringAngle_update(0.0f);

        LetTimePass(10 + APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT - 1);
        ASSERT_NEAR(test_params[i].apps_percentage, app_canRx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
        ASSERT_NEAR(test_params[i].apps_percentage, app_canRx_FSM_SappsMappedPedalPercentage_get(), 0.5f);
        ASSERT_FALSE(app_canTx_VC_Warning_BrakeAppsDisagreement_get());

        if (test_params[i].expect_fault)
        {
            // Signal debounce has elapsed, expect fault
            LetTimePass(1);
            ASSERT_EQ(0.0f, app_canTx_VC_LeftInverterTorqueCommand_get());
            ASSERT_EQ(0.0f, app_canTx_VC_RightInverterTorqueCommand_get());
            ASSERT_TRUE(app_canAlerts_VC_Warning_BrakeAppsDisagreement_get());

            // Check faulted indefinitely
            LetTimePass(1000);
            ASSERT_FLOAT_EQ(0.0f, app_canTx_VC_LeftInverterTorqueCommand_get());
            ASSERT_FLOAT_EQ(0.0f, app_canTx_VC_RightInverterTorqueCommand_get());
            ASSERT_TRUE(app_canAlerts_VC_Warning_BrakeAppsDisagreement_get());

            // Clear condition, confirm fault resets
            app_canRx_FSM_PappsMappedPedalPercentage_update(4); // Primary must be <5% to clear
            app_canRx_FSM_SappsMappedPedalPercentage_update(4);

            LetTimePass(10 + APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR - 1);
            ASSERT_FLOAT_EQ(0.0f, app_canTx_VC_LeftInverterTorqueCommand_get());
            ASSERT_FLOAT_EQ(0.0f, app_canTx_VC_RightInverterTorqueCommand_get());
            ASSERT_TRUE(app_canAlerts_VC_Warning_BrakeAppsDisagreement_get());

            LetTimePass(1);
            ASSERT_NEAR(4 * 0.01f * 90.0f, app_canTx_VC_LeftInverterTorqueCommand_get(), 0.5f);
            ASSERT_NEAR(4 * 0.01f * 90.0f, app_canTx_VC_RightInverterTorqueCommand_get(), 0.5f);
            ASSERT_FALSE(app_canAlerts_VC_Warning_BrakeAppsDisagreement_get());
        }
        else
        {
            // No fault condition, confirm no fault is set
            LetTimePass(1000);
            float actual_torque = test_params[i].apps_percentage * 0.01f * 90.0f;
            ASSERT_NEAR(actual_torque, app_canTx_VC_LeftInverterTorqueCommand_get(), 0.5f);
            ASSERT_NEAR(actual_torque, app_canTx_VC_RightInverterTorqueCommand_get(), 0.5f);
            ASSERT_FALSE(app_canAlerts_VC_Warning_BrakeAppsDisagreement_get());
        }
    }
}

TEST_F(VcFaultsTest, brake_actuated_and_regen_switch_on_sets_torque_to_zero_and_sets_fault_if_papps_pressed)
{
    struct
    {
        float       apps_percentage;
        bool        brake_actuated;
        bool        expect_fault;
        SwitchState switch_state;
    } test_params[20] = {
        // Regen apps_percentage will get converted from a [0,100] range to [30,70] and then to [-1,1]. So, [-1,0]
        // is the regen range, [0,1] is the accelerating range. This ensures there will be no fault during the regen
        // range and only during > 25% on the accelerating range

        { -1.0f, false, false, SWITCH_OFF }, // Pedal not pressed, max regen, and brake not actuated, no fault
        { -0.75f, true, false, SWITCH_OFF }, // Pedal not pressed enough(in regen range) no fault
        { -0.5f, false, false, SWITCH_OFF }, // Brake not actuated, no fault
        { -0.25f, true, false,
          SWITCH_OFF }, // Pedal originally pressed enough + brake actuated, but in regen range, no fault
        { -0.1f, true, false,
          SWITCH_OFF }, // Pedal originally pressed enough + brake actuated, but in regen range, no fault
        { 0, false, false, SWITCH_OFF },     // Pedal % = 0 and brake not actuated, no fault
        { 0.24f, true, false, SWITCH_OFF },  // Pedal not pressed enough, no fault
        { 0.26f, false, false, SWITCH_OFF }, // Brake not actuated, no fault
        { 0.26f, true, true, SWITCH_OFF },   // Pedal pressed enough + brake actuated, fault
        { 0.75f, true, true, SWITCH_OFF },   // Pedal pressed significantly + brake actuated, fault
        // TV on
        { -1.0f, false, false, SWITCH_ON }, // Pedal not pressed, max regen, and brake not actuated, no fault
        { -0.75f, true, false, SWITCH_ON }, // Pedal not pressed enough(in regen range) no fault
        { -0.5f, false, false, SWITCH_ON }, // Brake not actuated, no fault
        { -0.25f, true, false,
          SWITCH_ON }, // Pedal originally pressed enough + brake actuated, but in regen range, no fault
        { -0.01f, true, false,
          SWITCH_ON }, // Pedal originally pressed enough + brake actuated, but in regen range, no fault
        { 0, false, false, SWITCH_ON },     // Pedal % = 0 and brake not actuated, no fault
        { 0.24f, true, false, SWITCH_ON },  // Pedal not pressed enough, no fault
        { 0.26f, false, false, SWITCH_ON }, // Brake not actuated, no fault
        { 0.26f, true, true, SWITCH_ON },   // Pedal pressed enough + brake actuated, fault
        { 0.75f, true, true, SWITCH_ON },   // Pedal pressed significantly + brake actuated, fault
    };

    for (int i = 0; i < 20; i++)
    {
        TearDown();
        SetUp();

        SetStateToDrive();
        LetTimePass(10);
        EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());

        int   actual_apps_percentage = regen_to_regularMapping(test_params[i].apps_percentage);
        float motor_speed_rpm        = MOTOR_KMH_TO_RPM(30.0f);

        app_canRx_CRIT_RegenSwitch_update(SWITCH_ON);
        app_canRx_CRIT_TorqueVecSwitch_update(test_params[i].switch_state);
        app_canRx_FSM_PappsMappedPedalPercentage_update(actual_apps_percentage);
        app_canRx_FSM_SappsMappedPedalPercentage_update(actual_apps_percentage);
        app_canRx_FSM_BrakeActuated_update(test_params[i].brake_actuated);
        app_canRx_FSM_SteeringAngle_update(0.0f);
        app_canRx_INVR_MotorSpeed_update(-motor_speed_rpm);
        app_canRx_INVL_MotorSpeed_update(motor_speed_rpm);

        LetTimePass(10 + APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT - 1);
        ASSERT_NEAR(actual_apps_percentage, app_canRx_FSM_PappsMappedPedalPercentage_get(), 0.5f);
        ASSERT_NEAR(actual_apps_percentage, app_canRx_FSM_SappsMappedPedalPercentage_get(), 0.5f);
        ASSERT_FALSE(app_canTx_VC_Warning_BrakeAppsDisagreement_get());

        if (test_params[i].expect_fault)
        {
            // Signal debounce has elapsed, expect fault
            LetTimePass(1);
            ASSERT_EQ(0.0f, app_canTx_VC_LeftInverterTorqueCommand_get());
            ASSERT_EQ(0.0f, app_canTx_VC_RightInverterTorqueCommand_get());
            ASSERT_TRUE(app_canAlerts_VC_Warning_BrakeAppsDisagreement_get());

            // Check faulted indefinitely
            LetTimePass(1000);
            ASSERT_FLOAT_EQ(0.0f, app_canTx_VC_LeftInverterTorqueCommand_get());
            ASSERT_FLOAT_EQ(0.0f, app_canTx_VC_RightInverterTorqueCommand_get());
            ASSERT_TRUE(app_canAlerts_VC_Warning_BrakeAppsDisagreement_get());

            // Clear condition, confirm fault resets
            app_canRx_FSM_PappsMappedPedalPercentage_update(
                regen_to_regularMapping(0.04f)); // Primary must be <5% to clear
            app_canRx_FSM_SappsMappedPedalPercentage_update(regen_to_regularMapping(0.04f));

            LetTimePass(10 + APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR - 1);
            ASSERT_FLOAT_EQ(0.0f, app_canTx_VC_LeftInverterTorqueCommand_get());
            ASSERT_FLOAT_EQ(0.0f, app_canTx_VC_RightInverterTorqueCommand_get());
            ASSERT_TRUE(app_canAlerts_VC_Warning_BrakeAppsDisagreement_get());

            LetTimePass(10);
            ASSERT_NEAR(0.04f * 90.0f, app_canTx_VC_LeftInverterTorqueCommand_get(), 0.5f);
            ASSERT_NEAR(0.04f * 90.0f, app_canTx_VC_RightInverterTorqueCommand_get(), 0.5f);
            ASSERT_FALSE(app_canAlerts_VC_Warning_BrakeAppsDisagreement_get());
        }
        else
        {
            // No fault condition, confirm no fault is set
            LetTimePass(1000);
            float expected_torque;
            float power_max_kW = -test_params[i].apps_percentage * POWER_LIMIT_REGEN_kW;
            if (test_params[i].apps_percentage < 0.0f)
            {
                // regular active differential
                float delta =
                    TRACK_WIDTH_mm * tanf(DEG_TO_RAD(0.0f * APPROX_STEERING_TO_WHEEL_ANGLE)) / (2 * WHEELBASE_mm);
                float cl = (1 + delta);
                float cr = (1 - delta);

                float torque_lim_Nm = -(POWER_TO_TORQUE_CONVERSION_FACTOR * power_max_kW) /
                                      (motor_speed_rpm * cl + motor_speed_rpm * cr + SMALL_EPSILON);

                expected_torque = torque_lim_Nm * cl;
            }
            else
            {
                expected_torque = test_params[i].apps_percentage * 90.0f;
            }

            ASSERT_NEAR(expected_torque, app_canTx_VC_LeftInverterTorqueCommand_get(), 0.5f);
            ASSERT_NEAR(expected_torque, app_canTx_VC_RightInverterTorqueCommand_get(), 0.5f);
            ASSERT_FALSE(app_canAlerts_VC_Warning_BrakeAppsDisagreement_get());
        }
    }
}
