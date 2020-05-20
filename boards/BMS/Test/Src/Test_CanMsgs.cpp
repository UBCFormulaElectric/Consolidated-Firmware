#include "Test_Bms.h"

extern "C"
{
#include "App_CanMsgs.h"
}

// BMS-31
TEST(CanMsgsTest, state_machine_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(100), CANMSGS_BMS_STATE_MACHINE_CYCLE_TIME_MS);
}

// BMS-1
TEST(CanMsgsTest, heartbeat_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CANMSGS_BMS_HEARTBEAT_CYCLE_TIME_MS);
}

// BMS-34
TEST(CanMsgsTest, state_of_charge_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(100), CANMSGS_BMS_STATE_OF_CHARGE_CYCLE_TIME_MS);
}

// BMS-37
TEST(CanMsgsTest, ok_status_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(100), CANMSGS_BMS_LATCH_STATUSES_CYCLE_TIME_MS);
}
