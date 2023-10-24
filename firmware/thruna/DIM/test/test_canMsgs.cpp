#include "test_dimBaseStateMachineTest.h"

class CanBroadcastTest : public DimBaseStateMachineTest
{
};

TEST_F(CanBroadcastTest, check_drive_state_is_broadcasted_over_can)
{
    EXPECT_EQ(DIM_STATE_DRIVE, App_CanTx_DIM_State_Get());
}

TEST_F(CanBroadcastTest, check_start_switch_is_broadcasted_over_can_in_drive_state)
{
    fake_io_switch_isClosed_returnsForArgs(&start_switch, false);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_OFF, App_CanTx_DIM_StartSwitch_Get());

    fake_io_switch_isClosed_returnsForArgs(&start_switch, true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_ON, App_CanTx_DIM_StartSwitch_Get());
}

TEST_F(CanBroadcastTest, check_aux_switch_is_broadcasted_over_can_in_drive_state)
{
    fake_io_switch_isClosed_returnsForArgs(&aux_switch, false);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_OFF, App_CanTx_DIM_AuxSwitch_Get());

    fake_io_switch_isClosed_returnsForArgs(&aux_switch, true);
    LetTimePass(state_machine, 10);
    ASSERT_EQ(SWITCH_ON, App_CanTx_DIM_AuxSwitch_Get());
}
