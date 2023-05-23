#include "Test_Dcm.h"

extern "C"
{
#include "App_CanUtils.h"
}

// DCM-21
TEST(CanMsgsTest, state_machine_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_DCM_VITALS_CYCLE_TIME_MS);
}

// DCM-2
TEST(CanMsgsTest, heartbeat_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_DCM_VITALS_CYCLE_TIME_MS);
}
