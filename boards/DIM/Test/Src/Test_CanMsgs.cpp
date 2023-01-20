#include "Test_Dim.h"

extern "C"
{
#include "App_CanUtils.h"
}

// DIM-12
TEST(CanMsgsTest, state_machine_message_frequency)
{
    //    ASSERT_EQ(HZ_TO_MS(1), CANMSGS_DIM_STATE_MACHINE_CYCLE_TIME_MS);
}

// DIM-10
TEST(CanMsgsTest, heartbeat_message_frequency)
{
    //    ASSERT_EQ(HZ_TO_MS(10), CANMSGS_DIM_VITALS_CYCLE_TIME_MS);
}

// DIM-4
TEST(CanMsgsTest, start_and_traction_control_and_torque_vectoring_switches_message_frequency)
{
    //    ASSERT_EQ(HZ_TO_MS(100), CANMSGS_DIM_SWITCHES_CYCLE_TIME_MS);
}
