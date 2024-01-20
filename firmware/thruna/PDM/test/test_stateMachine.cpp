#include "test_pdmBaseStateMachineTest.h"

class StateMachineTest : public PdmBaseStateMachineTest
{
};

TEST_F(StateMachineTest, check_init_state_is_broadcasted_over_can)
{
    SetInitialState(app_initState_get());
    EXPECT_EQ(PDM_INIT_STATE, App_CanTx_PDM_State_Get());
}

TEST_F(StateMachineTest, check_drive_state_is_broadcasted_over_can)
{
    SetInitialState(app_driveState_get());
    EXPECT_EQ(BMS_DRIVE_STATE, App_CanTx_PDM_State_Get());
}

TEST_F(StateMachineTest, check_state_transition_from_init_to_drive)
{
    SetInitialState(app_initState_get());
    App_CanTx_BMS_State_Set(BMS_INVERTER_ON_STATE);
    EXPECT_EQ(PDM_DRIVE_STATE, App_CanTx_PDM_State_Get());
}
