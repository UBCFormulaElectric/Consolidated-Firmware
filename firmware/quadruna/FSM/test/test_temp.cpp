// TODO: Remove this test
// this is only here to make sure te state machine compiles
#include "test_fsmBaseStateMachineTest.h"

class FsmFaultsTest : public FsmBaseStateMachineTest
{
};

TEST_F(FsmFaultsTest, check_state_transition_fault_state_heartbeat_timeout)
{
    ASSERT_TRUE(true);
}
