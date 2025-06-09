#include "test_BMSBase.hpp"

extern "C"
{
#include "states/app_states.h"
}

class BmsStateMachineTest : public BMSBaseTest
{
};

#define ASSERT_STATE_EQ(x)                            \
    ASSERT_EQ(app_stateMachine_getCurrentState(), &x) \
        << "Expected state: " << x.name << ", but got: " << app_stateMachine_getCurrentState()->name

TEST_F(BmsStateMachineTest, init_proper_reset)
{
    app_stateMachine_setCurrentState(&drive_state);
    jobs_init();
    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, start_precharge_once_vc_bms_on) {}
TEST_F(BmsStateMachineTest, stops_charging_and_faults_if_charger_disconnects_in_charge_state) {}
TEST_F(BmsStateMachineTest, check_contactors_open_in_fault_state) {}
TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_no_faults_set) {}
TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_air_negative_open) {}
TEST_F(BmsStateMachineTest, faults_after_shutdown_loop_activates_while_charging) {}
TEST_F(BmsStateMachineTest, check_remains_in_fault_state_until_fault_cleared_then_transitions_to_init) {}
TEST_F(BmsStateMachineTest, check_precharge_state_transitions_and_air_plus_status) {}

// charging tests
TEST_F(BmsStateMachineTest, charger_connected_no_can_msg_init_state) {}
TEST_F(BmsStateMachineTest, charger_connected_can_msg_init_state) {}
TEST_F(BmsStateMachineTest, no_charger_connected_missing_hb_init_state) {}
TEST_F(BmsStateMachineTest, charger_connected_successful_precharge_stays) {}
TEST_F(BmsStateMachineTest, keeps_charging_with_no_interrupts) {}
TEST_F(BmsStateMachineTest, stops_charging_after_false_charging_msg) {}
TEST_F(BmsStateMachineTest, fault_from_charger_fault) {}