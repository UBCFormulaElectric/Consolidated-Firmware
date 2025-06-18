#include "test_VCBase.hpp"

extern "C"
{
#include "states/app_states.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_pcm.h"
}

class VCStateMachineTest : public VCBaseTest
{
};

#define ASSERT_STATE_EQ(expected)                            \
    ASSERT_EQ(app_stateMachine_getCurrentState(), &expected) \
        << "Expected state: " << expected.name << ", but got: " << app_stateMachine_getCurrentState()->name

// Helper to set state and invoke its entry action
static void SetStateWithEntry(const State *s)
{
    app_stateMachine_setCurrentState(s);
    s->run_on_entry();
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
}

TEST_F(VCStateMachineTest, init_state_management)
{
    app_stateMachine_setCurrentState(&drive_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);

    jobs_init();

    ASSERT_EQ(app_canRx_BMS_IrNegative_get(), CONTACTOR_STATE_OPEN);
    ASSERT_STATE_EQ(init_state);
}

TEST_F(VCStateMachineTest, air_minus_close_to_inv_on_state)
{
    app_stateMachine_setCurrentState(&init_state);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_STATE_EQ(inverterOn_state);
}

TEST_F(VCStateMachineTest, inverter_on_leave_condition_test)
{
    app_stateMachine_setCurrentState(&inverterOn_state);
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

TEST_F(VCStateMachineTest, bms_drive_state_transition_out_of_bms_on)
{
    app_stateMachine_setCurrentState(&bmsOn_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(100);
    ASSERT_STATE_EQ(bmsOn_state);

    app_canRx_BMS_State_update(BMS_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_STATE_EQ(pcmOn_state);
}

TEST_F(VCStateMachineTest, pcm_on_tests)
{
    // TODO when io power monitor is implemented

    app_stateMachine_setCurrentState(&pcmOn_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    // TODO set power monitor to 0v

    // testing retries
    for (int retry = 0; retry < 10; ++retry)
    {
        // TODO determine the true timings
        for (int i = 0; i < 30; i++) // 300ms
        {
            ASSERT_TRUE(io_pcm_enabled());
            LetTimePass(10);
        }
        for (int i = 0; i < 10; i++) // 100ms
        {
            ASSERT_FALSE(io_pcm_enabled());
            LetTimePass(10);
        }
        ASSERT_STATE_EQ(pcmOn_state) << "Failed after " << (retry + 1) * 100 << "ms";
    }
}

TEST_F(VCStateMachineTest, air_minus_open_in_all_states_to_init)
{
    app_stateMachine_setCurrentState(&inverterOn_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_OPEN);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    app_stateMachine_setCurrentState(&bmsOn_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_OPEN);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);

    // etc.
}

TEST_F(VCStateMachineTest, torque_request_zero_when_leave_drive) {}

TEST_F(VCStateMachineTest, fault_and_open_irs_gives_fault_state)
{
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    app_stateMachine_setCurrentState(&drive_state);
    LetTimePass(10);
    ASSERT_STATE_EQ(drive_state);

    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_OPEN);
    app_canAlerts_VC_Warning_FrontLeftInverterFault_set(true);
    LetTimePass(10);
    ASSERT_STATE_EQ(init_state);
}

TEST_F(VCStateMachineTest, start_button_operation)
{
    app_stateMachine_setCurrentState(&hv_state);
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);

    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    for (int i = 0; i < 100; ++i)
    {
        LetTimePass(10);
        ASSERT_STATE_EQ(drive_state) << " failed after " << (i + 1) * 10 << "ms";
    }

    app_canRx_CRIT_StartSwitch_update(SWITCH_OFF);
    LetTimePass(10);
    ASSERT_STATE_EQ(drive_state);

    app_canRx_CRIT_StartSwitch_update(SWITCH_ON);
    LetTimePass(10);
    ASSERT_STATE_EQ(hv_state);
}

TEST_F(VCStateMachineTest, UnderVoltageRetryThenFault)
{
    // Enter PCM ON
    SetStateWithEntry(&pcmOn_state);

    // Always return 0 V
    app_canTx_VC_ChannelOneVoltage_set(0.0f);

    // should trigger retry and turn PCM off
    LetTimePass(100);
    EXPECT_FALSE(io_pcm_enabled());

    // should turn PCM back ON
    LetTimePass(110);
    EXPECT_TRUE(io_pcm_enabled());

    // should fault out
    LetTimePass(100);

    EXPECT_TRUE(app_canAlerts_VC_Info_PcmUnderVoltage_get());

    ASSERT_STATE_EQ(hvInit_state);
}
