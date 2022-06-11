#include "Test_Dcm.h"

extern "C"
{
#include "App_CanMsgs.h"
}

// DCM-21
TEST(CanMsgsTest, state_machine_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CANMSGS_DCM_STATE_MACHINE_CYCLE_TIME_MS);
}

// DCM-2
TEST(CanMsgsTest, heartbeat_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CANMSGS_DCM_VITALS_CYCLE_TIME_MS);
}

// DCM-19
TEST(CanMsgsTest, torque_request_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CANMSGS_DCM_TORQUE_REQUEST_CYCLE_TIME_MS);
}
