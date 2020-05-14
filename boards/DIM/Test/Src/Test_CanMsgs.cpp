#include "Test_Dim.h"

extern "C"
{
#include "App_CanMsgs.h"
}

class CanMsgsTest : public testing::Test
{
  protected:
    __inline__ uint32_t HZ_TO_MS(uint32_t hz)
    {
        return 1000 / hz;
    }

};

// DIM-12
TEST_F(CanMsgsTest, state_machine_message_frequency)
{
    ASSERT_EQ(HZ_TO_MS(100), CANMSGS_DIM_STATE_MACHINE_CYCLE_TIME_MS);
}

// DIM-10
TEST_F(CanMsgsTest, heartbeat_message_frequency)
{
    ASSERT_EQ(HZ_TO_MS(10), CANMSGS_DIM_HEARTBEAT_CYCLE_TIME_MS);
}

// DIM-3
TEST_F(CanMsgsTest, drive_mode_switch_message_frequency)
{
    ASSERT_EQ(HZ_TO_MS(100), CANMSGS_DIM_DRIVE_MODE_SWITCH_CYCLE_TIME_MS);
}

// DIM-4
TEST_F(CanMsgsTest, start_and_traction_control_and_torque_vectoring_switches_message_frequency)
{
    ASSERT_EQ(HZ_TO_MS(100), CANMSGS_DIM_SWITCHES_CYCLE_TIME_MS);
}

// DIM-7
TEST_F(CanMsgsTest, mapped_regen_paddle_percentage_message_frequency)
{
    ASSERT_EQ(HZ_TO_MS(100), CANMSGS_DIM_REGEN_PADDLE_CYCLE_TIME_MS);
}
