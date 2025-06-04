#include "test_VCBase.hpp"

extern "C"
{
#include "states/app_states.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
}

class VCStateMachineTest : public VCBaseTest
{
};

#define ASSERT_STATE_EQ(expected)                            \
    ASSERT_EQ(app_stateMachine_getCurrentState(), &expected) \
        << "Expected state: " << expected.name << ", but got: " << app_stateMachine_getCurrentState()->name

TEST_F(VCStateMachineTest, starts_in_init_state_contactors_open)
{
    ASSERT_EQ(app_canRx_BMS_IrNegative_get(), CONTACTOR_STATE_OPEN);
    ASSERT_STATE_EQ(init_state);
}

TEST_F(VCStateMachineTest, air_minus_close_to_inv_on_state)
{
    app_stateMachine_setNextState(&init_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverterOn_state);
}

TEST_F(VCStateMachineTest, inverter_on_leave_condition_test)
{
    app_stateMachine_setNextState(&inverterOn_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);

    app_canRx_INVFL_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverterOn_state);

    app_canRx_INVFR_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverterOn_state);

    app_canRx_INVRL_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverterOn_state);

    app_canRx_INVRR_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(bmsOn_state);
}

TEST_F(VCStateMachineTest, bms_drive_state_transition)
{
    app_stateMachine_setNextState(&bmsOn_state);
    LetTimePass(100);
    ASSERT_STATE_EQ(bmsOn_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(100);
    ASSERT_STATE_EQ(bmsOn_state);
    app_canRx_BMS_State_update(BMS_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(pcmOn_state);
}

TEST_F(VCStateMachineTest, air_minus_open_in_all_states_to_init)
{
    app_stateMachine_setNextState(&inverterOn_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_OPEN);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    app_stateMachine_setNextState(&bmsOn_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_OPEN);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    // etc.
}

TEST_F(VCStateMachineTest, torque_request_zero_when_leave_drive) {}

TEST_F(VCStateMachineTest, values_reset_when_no_heartbeat)
{
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    ASSERT_FALSE(app_canAlerts_VC_Fault_MissingBMSHeartbeat_get());
    ASSERT_FALSE(app_canAlerts_VC_Fault_MissingFSMHeartbeat_get());
    ASSERT_FALSE(app_canAlerts_VC_Fault_MissingRSMHeartbeat_get());
    LetTimePass(100);

    suppress_heartbeat = true;
    LetTimePass(10);

    // tests
    ASSERT_EQ(app_canRx_BMS_IrNegative_get(), CONTACTOR_STATE_OPEN);
    ASSERT_STATE_EQ(fault_state);
    ASSERT_TRUE(app_canAlerts_VC_Fault_MissingBMSHeartbeat_get());
    ASSERT_TRUE(app_canAlerts_VC_Fault_MissingFSMHeartbeat_get());
    ASSERT_TRUE(app_canAlerts_VC_Fault_MissingRSMHeartbeat_get());
    // TODO more concequences of heartbeat failure
}

// TEST_F(VCStateMachineTest, test_SetStateToDrive) {}
//
// TEST_F(VCStateMachineTest, check_init_transitions_to_drive_if_conditions_met_and_start_switch_pulled_up) {}
//
// TEST_F(VCStateMachineTest, check_init_state_is_broadcasted_over_can) {}
//
// TEST_F(VCStateMachineTest, check_state_transition_from_init_to_inverter_on) {}
//
// TEST_F(VCStateMachineTest, check_drive_state_is_broadcasted_over_can) {}
//
// TEST_F(VCStateMachineTest, check_inverter_on_state_is_broadcasted_over_can) {}
//
// TEST_F(VCStateMachineTest, disable_inverters_in_init_state) {}
//
// TEST_F(VCStateMachineTest, start_switch_off_transitions_drive_state_to_inverter_on_state) {}
//
// TEST_F(VCStateMachineTest, check_if_buzzer_stays_on_for_two_seconds_only_after_entering_drive_state) {}
//
// TEST_F(VCStateMachineTest, no_torque_requests_when_accelerator_pedal_is_not_pressed) {}
//
// TEST_F(VCStateMachineTest, BMS_causes_drive_to_inverter_on) {}
//
// TEST_F(VCStateMachineTest, BMS_causes_drive_to_inverter_on_to_init) {}
