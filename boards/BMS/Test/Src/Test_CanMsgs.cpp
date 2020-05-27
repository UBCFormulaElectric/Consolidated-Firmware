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

// BMS-9
TEST(CanMsgsTest, charger_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CANMSGS_BMS_CHARGER_CYCLE_TIME_MS);
}

// BMS-37
TEST(CanMsgsTest, ok_status_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(100), CANMSGS_BMS_OK_STATUSES_CYCLE_TIME_MS);
}

// BMS-38
TEST(CanMsgsTest, airs_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(100), CANMSGS_BMS_AIRS_CYCLE_TIME_MS);
}
