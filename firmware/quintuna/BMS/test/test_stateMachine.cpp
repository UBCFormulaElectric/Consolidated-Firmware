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
    fakes::irs::setNegativeState(IRS_OPEN);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    app_canRx_VC_State_update(VC_INVERTER_ON_STATE);
    fakes::irs::setNegativeState(IRS_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    app_canRx_VC_State_update(VC_BMS_ON_STATE);
    fakes::irs::setNegativeState(IRS_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(precharge_state);

    app_canRx_VC_State_update(VC_INIT_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(precharge_state); // surely precharge state is stable for at least 20ms
}

TEST_F(BmsStateMachineTest, irs_negative_open_to_init)
{
    app_stateMachine_setCurrentState(&drive_state);
    fakes::irs::setNegativeState(IRS_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(drive_state);

    fakes::irs::setNegativeState(IRS_OPEN);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, check_contactors_open_in_inert_states)
{
    io_irs_setPositive(IRS_OPEN);
    app_stateMachine_setCurrentState(&fault_state);
    ASSERT_FALSE(io_irs_positiveState());

    io_irs_setPositive(IRS_CLOSED);
    app_stateMachine_setCurrentState(&init_state);
    ASSERT_FALSE(io_irs_positiveState());
}

TEST_F(BmsStateMachineTest, check_state_transition_from_fault_to_init_with_no_faults_set)
{
    app_canAlerts_BMS_Fault_AMSFault_set(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(fault_state);

    app_canAlerts_BMS_Fault_AMSFault_set(false);
    ASSERT_FALSE(app_canAlerts_AnyBoardHasFault());
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);
}

TEST_F(BmsStateMachineTest, stays_in_fault_state_if_ir_negative_opens)
{
    app_stateMachine_setCurrentState(&fault_state);
    fakes::irs::setNegativeState(IRS_CLOSED);
    LetTimePass(100);
    ASSERT_STATE_EQ(fault_state);
}

// precharge tests
// TODO set these values
static constexpr float undervoltage = 200.0f, target_voltage = 600.0f;
static constexpr int   too_fast_time = 20, just_good_time = 100, precharge_timeout = 300, precharge_cooldown = 150;
static constexpr int   precharge_retries = 3;
TEST_F(BmsStateMachineTest, precharge_success_test)
{
    app_stateMachine_setCurrentState(&precharge_state);
    fakes::irs::setNegativeState(IRS_CLOSED);

    for (int i = 0; i < just_good_time; i += 10)
    {
        ASSERT_STATE_EQ(precharge_state);
        ASSERT_EQ(io_irs_prechargeState(), IRS_CLOSED);
        LetTimePass(10);
    }

    fakes::tractiveSystem::setVoltage(target_voltage);
    LetTimePass(10);
    ASSERT_STATE_EQ(drive_state);
}

TEST_F(BmsStateMachineTest, precharge_retry_test_and_undervoltage_rising_slowly)
{
    app_stateMachine_setCurrentState(&precharge_state);
    fakes::irs::setNegativeState(IRS_CLOSED);
    fakes::tractiveSystem::setVoltage(undervoltage);

    for (int retry = 0; retry < precharge_retries; retry++)
    {
        for (int i = 0; i < precharge_timeout; i += 10)
        {
            ASSERT_STATE_EQ(precharge_state);
            ASSERT_EQ(io_irs_prechargeState(), IRS_CLOSED);
            LetTimePass(10);
        }

        // cooldown
        for (int i = 0; i < precharge_cooldown; i += 10)
        {
            ASSERT_STATE_EQ(precharge_state);
            ASSERT_EQ(io_irs_prechargeState(), IRS_OPEN);
            LetTimePass(10);
        }
    }

    LetTimePass(10);
    ASSERT_STATE_EQ(precharge_latch_state);
}

TEST_F(BmsStateMachineTest, precharge_rising_too_quickly)
{
    app_stateMachine_setCurrentState(&precharge_state);
    fakes::irs::setNegativeState(IRS_CLOSED);
    for (int i = 0; i < too_fast_time; i += 10)
    {
        ASSERT_STATE_EQ(precharge_state);
        ASSERT_EQ(io_irs_prechargeState(), IRS_CLOSED);
        LetTimePass(10);
    }

    fakes::tractiveSystem::setVoltage(target_voltage);
    LetTimePass(10);
    // ASSERT_FALSE(io_irs_isPrechargeClosed());
    ASSERT_EQ(io_irs_prechargeState(), IRS_OPEN);
    // we presume that it is in the retry phase as described above now
}

// charging tests
TEST_F(BmsStateMachineTest, faults_after_shutdown_loop_activates_while_charging) {}
TEST_F(BmsStateMachineTest, stops_charging_and_faults_if_charger_disconnects_in_charge_state) {}
TEST_F(BmsStateMachineTest, charger_connected_no_can_msg_init_state) {}
TEST_F(BmsStateMachineTest, charger_connected_can_msg_init_state) {}
TEST_F(BmsStateMachineTest, no_charger_connected_missing_hb_init_state) {}
TEST_F(BmsStateMachineTest, charger_connected_successful_precharge_stays) {}
TEST_F(BmsStateMachineTest, keeps_charging_with_no_interrupts) {}
TEST_F(BmsStateMachineTest, stops_charging_after_false_charging_msg) {}
TEST_F(BmsStateMachineTest, fault_from_charger_fault) {}