#include "test_BMSBase.hpp"

class BmsStateMachineTest : public BMSBaseTest
{
};

TEST_F(BmsStateMachineTest, check_init_state_is_broadcasted_over_can) {}

TEST_F(BmsStateMachineTest, check_drive_state_is_broadcasted_over_can) {}

TEST_F(BmsStateMachineTest, check_fault_state_is_broadcasted_over_can) {}

TEST_F(BmsStateMachineTest, check_charge_state_is_broadcasted_over_can) {}

TEST_F(BmsStateMachineTest, check_inverter_on_state_is_broadcasted_over_can) {}

TEST_F(BmsStateMachineTest, check_state_transition_from_init_to_inverter_to_precharge) {}

TEST_F(BmsStateMachineTest, check_imd_frequency_is_broadcasted_over_can_in_all_states) {}

TEST_F(BmsStateMachineTest, check_imd_duty_cycle_is_broadcasted_over_can_in_all_states) {}

TEST_F(BmsStateMachineTest, check_imd_insulation_resistance_10hz_is_broadcasted_over_can_in_all_states) {}

TEST_F(BmsStateMachineTest, check_imd_insulation_resistance_20hz_is_broadcasted_over_can_in_all_states) {}

TEST_F(BmsStateMachineTest, check_imd_speed_start_status_30hz_is_broadcasted_over_can_in_all_states) {}

TEST_F(BmsStateMachineTest, check_imd_seconds_since_power_on_is_broadcasted_over_can_in_all_states) {}

TEST_F(BmsStateMachineTest, charger_connection_status_in_all_states) {}

TEST_F(BmsStateMachineTest, check_bms_ok_is_broadcasted_over_can_in_all_states) {}

TEST_F(BmsStateMachineTest, check_imd_ok_is_broadcasted_over_can_in_all_states) {}

TEST_F(BmsStateMachineTest, check_bspd_ok_is_broadcasted_over_can_in_all_states) {}

TEST_F(BmsStateMachineTest, stops_charging_and_faults_if_charger_disconnects_in_charge_state) {}

TEST_F(BmsStateMachineTest, check_airs_can_signals_for_all_states) {}

TEST_F(BmsStateMachineTest, check_contactors_open_in_fault_state) {}

TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_no_faults_set) {}

TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_air_negative_open) {}

TEST_F(BmsStateMachineTest, charger_connected_no_can_msg_init_state) {}

TEST_F(BmsStateMachineTest, charger_connected_can_msg_init_state) {}

TEST_F(BmsStateMachineTest, no_charger_connected_missing_hb_init_state) {}

TEST_F(BmsStateMachineTest, charger_connected_successful_precharge_stays) {}

TEST_F(BmsStateMachineTest, keeps_charging_with_no_interrupts) {}

TEST_F(BmsStateMachineTest, stops_charging_after_false_charging_msg) {}

TEST_F(BmsStateMachineTest, fault_from_charger_fault) {}

TEST_F(BmsStateMachineTest, faults_after_shutdown_loop_activates_while_charging) {}

TEST_F(BmsStateMachineTest, check_remains_in_fault_state_until_fault_cleared_then_transitions_to_init) {}

TEST_F(BmsStateMachineTest, check_precharge_state_transitions_and_air_plus_status) {}