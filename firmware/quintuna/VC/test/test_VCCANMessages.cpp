#include "test_VCBase.hpp"

extern "C"
{
#include "states/app_states.h"
#include "app_canTx.h"
}

class VCCANMessagesTest : public VCBaseTest
{
};

TEST_F(VCCANMessagesTest, ) {}

TEST_F(VCCANMessagesTest, states_broadcasted_over_can)
{
    app_stateMachine_setCurrentState(&init_state);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_State_get(), VC_INIT_STATE);

    app_stateMachine_setCurrentState(&inverterOn_state);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_State_get(), VC_INVERTER_ON_STATE);

    app_stateMachine_setCurrentState(&bmsOn_state);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_State_get(), VC_BMS_ON_STATE);

    app_stateMachine_setCurrentState(&pcmOn_state);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_State_get(), VC_PCM_ON_STATE);

    app_stateMachine_setCurrentState(&hvInit_state);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_State_get(), VC_HV_INIT_STATE);

    app_stateMachine_setCurrentState(&hv_state);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_State_get(), VC_HV_ON_STATE);

    app_stateMachine_setCurrentState(&drive_state);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_State_get(), VC_DRIVE_STATE);

    app_stateMachine_setCurrentState(&driveWarning_state);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_State_get(), VC_DRIVE_WARNING_STATE);

    app_stateMachine_setCurrentState(&fault_state);
    LetTimePass(10);
    ASSERT_EQ(app_canTx_VC_State_get(), VC_FAULT_STATE);
}
