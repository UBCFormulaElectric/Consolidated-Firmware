#include "test_VCBase.hpp"

extern "C"
{
    // TODO remove this is temporary
#include "io_canTx.h"
    void io_canTx_init(
        void (*transmit_can1_msg_func)(const JsonCanMsg *),
        void (*transmit_can2_msg_func)(const JsonCanMsg *),
        void (*transmit_can3_msg_func)(const JsonCanMsg *))
    {
        (void)transmit_can1_msg_func;
        (void)transmit_can2_msg_func;
        (void)transmit_can3_msg_func;
    }
}

extern "C"
{
#include "states/app_states.h"
#include "app_canRx.h"
}

class VCPowerManagerTest : public VCBaseTest
{
};

TEST_F(VCPowerManagerTest, test_sequencingStateMachine)
{
    // this tests that the efuse sequencing happens
    ASSERT_EQ(app_stateMachine_getCurrentState(), &init_state)
        << app_stateMachine_getCurrentState()->name << " != init_state";

    // close ir negative
    app_canRx_BMS_IrNegative_update(CONTACTOR_STATE_CLOSED);
    LetTimePass(10);
    ASSERT_EQ(app_stateMachine_getCurrentState(), &inverterOn_state)
        << app_stateMachine_getCurrentState()->name << " != inverterOn_state";

    // turn on inverters
    app_canRx_INVFL_bSystemReady_update(true);
    app_canRx_INVFR_bSystemReady_update(true);
    app_canRx_INVRL_bSystemReady_update(true);
    app_canRx_INVRR_bSystemReady_update(true);
    LetTimePass(10);
    ASSERT_EQ(app_stateMachine_getCurrentState(), &bmsOn_state)
        << app_stateMachine_getCurrentState()->name << " != bmsOn_state";

    // turn on the bms
    app_canRx_BMS_State_update(BMS_DRIVE_STATE);
    LetTimePass(10);
    ASSERT_EQ(app_stateMachine_getCurrentState(), &pcmOn_state)
        << app_stateMachine_getCurrentState()->name << " != pcmOn_state";
}

TEST_F(VCPowerManagerTest, test_steadyStateFailure)
{
    // this tests that in steady state, if an efuse fails, that the failure is handled
}

TEST_F(VCPowerManagerTest, test_startupFailure)
{
    // this tests that while starting up, if an efuse fails, that it is handled in order
}
