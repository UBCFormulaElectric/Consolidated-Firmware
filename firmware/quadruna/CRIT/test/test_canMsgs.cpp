#include "test_critBaseStateMachineTest.h"

class CanBroadcastTest : public CritBaseStateMachineTest
{
};

TEST_F(CanBroadcastTest, check_start_switch_is_broadcasted_over_can_in_drive_state)
{
    fake_io_switch_isClosed_returnsForArgs(&start_switch, false);
    LetTimePass(10);
    ASSERT_EQ(SWITCH_OFF, app_canTx_CRIT_StartSwitch_get());

    fake_io_switch_isClosed_returnsForArgs(&start_switch, true);
    LetTimePass(10);
    ASSERT_EQ(SWITCH_ON, app_canTx_CRIT_StartSwitch_get());
}

TEST_F(CanBroadcastTest, check_regen_switch_is_broadcasted_over_can_in_drive_state)
{
    fake_io_switch_isClosed_returnsForArgs(&regen_switch, false);
    LetTimePass(10);
    ASSERT_EQ(SWITCH_OFF, app_canTx_CRIT_RegenSwitch_get());

    fake_io_switch_isClosed_returnsForArgs(&regen_switch, true);
    LetTimePass(10);
    ASSERT_EQ(SWITCH_ON, app_canTx_CRIT_RegenSwitch_get());
}

TEST_F(CanBroadcastTest, check_torquevec_switch_is_broadcasted_over_can_in_drive_state)
{
    fake_io_switch_isClosed_returnsForArgs(&torquevec_switch, false);
    LetTimePass(10);
    ASSERT_EQ(SWITCH_OFF, app_canTx_CRIT_TorqueVecSwitch_get());

    fake_io_switch_isClosed_returnsForArgs(&torquevec_switch, true);
    LetTimePass(10);
    ASSERT_EQ(SWITCH_ON, app_canTx_CRIT_TorqueVecSwitch_get());
}