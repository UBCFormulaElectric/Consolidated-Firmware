#include "Test_Fsm.h"

extern "C"
{
#include "App_CanMsgs.h"
}

// FSM-10
TEST(CanMsgsTest, state_machine_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CANMSGS_FSM_STATE_MACHINE_CYCLE_TIME_MS);
}

// FSM-1
TEST(CanMsgsTest, heartbeat_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CANMSGS_FSM_VITALS_CYCLE_TIME_MS);
}

//// FSM-17
// TEST(CanMsgsTest, brake_actuation_message_frequency)
//{
//    ASSERT_GE(HZ_TO_MS(100), CANMSGS_FSM_BRAKE_CYCLE_TIME_MS);
//}
//
//// FSM-14
// TEST(CanMsgsTest, flow_meter_message_frequency)
//{
//    // This includes the primary and secondary flow meter
//    ASSERT_GE(HZ_TO_MS(1), CANMSGS_FSM_FLOW_METER_CYCLE_TIME_MS);
//}
