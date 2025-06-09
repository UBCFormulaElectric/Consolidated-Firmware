#include "test_BMSBase.hpp"

#include "test_fakes.h"

extern "C"
{
#include "states/app_states.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "io_irs.h"
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

TEST_F(BmsStateMachineTest, start_precharge_once_vc_bms_on_AND_irs_negative_closed)
{
    ASSERT_STATE_EQ(init_state);
    LetTimePass(100);
    ASSERT_STATE_EQ(init_state);

    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    fakes::irs::setNegativeClosed(false);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    app_canRx_VC_State_update(VC_INVERTER_ON_STATE);
    fakes::irs::setNegativeClosed(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    fakes::irs::setNegativeClosed(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(precharge_state);
}

TEST_F(BmsStateMachineTest, check_contactors_open_in_fault_state)
{
    io_irs_setPositive(true);
    app_stateMachine_setCurrentState(&fault_state);
    ASSERT_FALSE(io_irs_isPositiveClosed());
}

TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_no_faults_set)
{
    app_canAlerts_BMS_Fault_BMSFault_set(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(fault_state);

    app_canAlerts_BMS_Fault_BMSFault_set(false);
    ASSERT_FALSE(app_canAlerts_AnyBoardHasFault());
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, stays_in_fault_state_if_ir_negative_opens)
{
    app_stateMachine_setCurrentState(&fault_state);
    fakes::irs::setNegativeClosed(true);
    LetTimePass(100);
    ASSERT_STATE_EQ(fault_state);
}

TEST_F(BmsStateMachineTest, faults_after_shutdown_loop_activates_while_charging) {}
TEST_F(BmsStateMachineTest, check_remains_in_fault_state_until_fault_cleared_then_transitions_to_init) {}
TEST_F(BmsStateMachineTest, check_precharge_state_transitions_and_air_plus_status) {}

// charging tests
TEST_F(BmsStateMachineTest, stops_charging_and_faults_if_charger_disconnects_in_charge_state) {}
TEST_F(BmsStateMachineTest, charger_connected_no_can_msg_init_state) {}
TEST_F(BmsStateMachineTest, charger_connected_can_msg_init_state) {}
TEST_F(BmsStateMachineTest, no_charger_connected_missing_hb_init_state) {}
TEST_F(BmsStateMachineTest, charger_connected_successful_precharge_stays) {}
TEST_F(BmsStateMachineTest, keeps_charging_with_no_interrupts) {}
TEST_F(BmsStateMachineTest, stops_charging_after_false_charging_msg) {}
TEST_F(BmsStateMachineTest, fault_from_charger_fault) {}