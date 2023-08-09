#include "Test_Fsm.h"

extern "C"
{
#include "App_CanUtils.h"
}

TEST(CanMsgsTest, state_machine_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_FSM_VITALS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, heartbeat_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_FSM_VITALS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, brake_actuation_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_FSM_BRAKE_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, flow_meter_message_frequency)
{
    // This includes the primary and secondary flow meter
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_FSM_COOLANT_CYCLE_TIME_MS);
}
