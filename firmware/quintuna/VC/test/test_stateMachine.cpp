#include "test_VCBase.hpp"

class VCStateMachineTest : public VCBaseTest
{
};

TEST_F(VCStateMachineTest, test_SetStateToDrive) {}

TEST_F(VCStateMachineTest, check_init_transitions_to_drive_if_conditions_met_and_start_switch_pulled_up) {}

TEST_F(VCStateMachineTest, check_init_state_is_broadcasted_over_can) {}

TEST_F(VCStateMachineTest, check_state_transition_from_init_to_inverter_on) {}

TEST_F(VCStateMachineTest, check_drive_state_is_broadcasted_over_can) {}

TEST_F(VCStateMachineTest, check_inverter_on_state_is_broadcasted_over_can) {}

TEST_F(VCStateMachineTest, disable_inverters_in_init_state) {}

TEST_F(VCStateMachineTest, start_switch_off_transitions_drive_state_to_inverter_on_state) {}

TEST_F(VCStateMachineTest, check_if_buzzer_stays_on_for_two_seconds_only_after_entering_drive_state) {}

TEST_F(VCStateMachineTest, no_torque_requests_when_accelerator_pedal_is_not_pressed) {}

TEST_F(VCStateMachineTest, exit_drive_state_on_left_inverter_fault) {}

TEST_F(VCStateMachineTest, exit_drive_state_on_right_inverter_fault) {}

TEST_F(VCStateMachineTest, exit_drive_state_on_bms_fault) {}

TEST_F(VCStateMachineTest, exit_drive_state_on_fsm_fault) {}

TEST_F(VCStateMachineTest, exit_drive_state_on_VC_fault) {}

TEST_F(VCStateMachineTest, exit_drive_state_on_CRIT_fault) {}

TEST_F(VCStateMachineTest, BMS_causes_drive_to_inverter_on) {}

TEST_F(VCStateMachineTest, BMS_causes_drive_to_inverter_on_to_init) {}
