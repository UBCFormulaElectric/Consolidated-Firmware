#include "test_dimBaseStateMachineTest.h"

class CanBroadcastTest : public DimBaseStateMachineTest
{
};

TEST_F(CanBroadcastTest, check_start_switch_is_broadcasted_over_can_in_drive_state)
{
    fake_io_switch_isClosed_returnsForArgs(&start_switch, false);
    LetTimePass(10);
    ASSERT_EQ(SWITCH_OFF, app_canTx_DIM_StartSwitch_get());

    fake_io_switch_isClosed_returnsForArgs(&start_switch, true);
    LetTimePass(10);
    ASSERT_EQ(SWITCH_ON, app_canTx_DIM_StartSwitch_get());
}

TEST_F(CanBroadcastTest, check_aux_switch_is_broadcasted_over_can_in_drive_state)
{
    fake_io_switch_isClosed_returnsForArgs(&aux_switch, false);
    LetTimePass(10);
    ASSERT_EQ(SWITCH_OFF, app_canTx_DIM_AuxSwitch_get());

    fake_io_switch_isClosed_returnsForArgs(&aux_switch, true);
    LetTimePass(10);
    ASSERT_EQ(SWITCH_ON, app_canTx_DIM_AuxSwitch_get());
}
