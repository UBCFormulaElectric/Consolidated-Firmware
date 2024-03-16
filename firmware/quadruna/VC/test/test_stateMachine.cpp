#include "test_vcBaseStateMachineTest.h"

class StateMachineTest : public VcBaseStateMachineTest
{
};

TEST_F(StateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(app_initState_get());
    EXPECT_EQ(VC_INIT_STATE, app_canTx_VC_State_get());
    EXPECT_EQ(app_powerManager_getState(), POWER_MANAGER_SHUTDOWN);
}

TEST_F(StateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(app_driveState_get());
    EXPECT_EQ(VC_DRIVE_STATE, app_canTx_VC_State_get());
    EXPECT_EQ(app_powerManager_getState(), POWER_MANAGER_DRIVE);
}

TEST_F(StateMachineTest, check_state_transition_from_init_to_inverter_on)
{
    SetInitialState(app_initState_get());
    app_canRx_BMS_State_update(BMS_INVERTER_ON_STATE);
    LetTimePass(1000);
    EXPECT_EQ(VC_INVERTER_ON_STATE, app_canTx_VC_State_get());
    EXPECT_EQ(app_powerManager_getState(), POWER_MANAGER_DRIVE);
}

// TODO: add inverter_on to drive state test
