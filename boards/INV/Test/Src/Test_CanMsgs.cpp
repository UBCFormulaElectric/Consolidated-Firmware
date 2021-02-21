#include "Test_Inv.h"

extern "C"
{
#include "App_CanMsgs.h"
}

// TODO: if we want to do things, properly, each of these test should be
//       associated with a requirement in `requirements.md`. See other tests
//       for examples of how we denote this.

// TODO: requirement here
// INV-??
TEST(CanMsgsTest, state_machine_message_frequency)
{
    ASSERT_EQ(HZ_TO_MS(100), CANMSGS_INV_STATE_MACHINE_CYCLE_TIME_MS);
}

// TODO: requirement here
// INV-??
TEST(CanMsgsTest, heartbeat_message_frequency)
{
    ASSERT_EQ(HZ_TO_MS(10), CANMSGS_INV_HEARTBEAT_CYCLE_TIME_MS);
}

