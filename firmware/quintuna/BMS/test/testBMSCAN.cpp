#include "test_BMSBase.hpp"

class BMSCanTest : public BMSBaseTest
{
};

TEST_F(BMSCanTest, check_init_state_is_broadcasted_over_can) {}
TEST_F(BMSCanTest, check_drive_state_is_broadcasted_over_can) {}
TEST_F(BMSCanTest, check_fault_state_is_broadcasted_over_can) {}
TEST_F(BMSCanTest, check_charge_state_is_broadcasted_over_can) {}
TEST_F(BMSCanTest, check_inverter_on_state_is_broadcasted_over_can) {}
TEST_F(BMSCanTest, check_bms_ok_is_broadcasted_over_can_in_all_states) {}
TEST_F(BMSCanTest, check_imd_ok_is_broadcasted_over_can_in_all_states) {}
TEST_F(BMSCanTest, check_bspd_ok_is_broadcasted_over_can_in_all_states) {}
TEST_F(BMSCanTest, check_airs_can_signals_for_all_states) {}
TEST_F(BMSCanTest, charger_connection_status_in_all_states) {}
TEST_F(BMSCanTest, check_imd_info_is_broadcasted_over_can_in_all_states)
{
    // frequency
    // duty cycle
    // insulation resistance 10hz, 20hz
    // speed_start_status_30hz
    // seconds_since_power_on
}
