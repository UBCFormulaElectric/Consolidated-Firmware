#include "test_pdmBaseStateMachineTest.h"

class StateMachineTest : public PdmBaseStateMachineTest
{
};

TEST_F(StateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(app_initState_get());
    EXPECT_EQ(PDM_INIT_STATE, App_CanTx_PDM_State_Get());
    EXPECT_EQ(app_powerManager_getState(), POWER_MANAGER_SHUTDOWN);
}

TEST_F(StateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(app_driveState_get());
    EXPECT_EQ(PDM_DRIVE_STATE, App_CanTx_PDM_State_Get());
    EXPECT_EQ(app_powerManager_getState(), POWER_MANAGER_DRIVE);
}

TEST_F(StateMachineTest, check_state_transition_from_init_to_drive)
{
    SetInitialState(app_initState_get());
    App_CanRx_BMS_State_Update(BMS_INVERTER_ON_STATE);
    LetTimePass(state_machine, 1000);
    EXPECT_EQ(PDM_DRIVE_STATE, App_CanTx_PDM_State_Get());
    EXPECT_EQ(app_powerManager_getState(), POWER_MANAGER_DRIVE);
}
