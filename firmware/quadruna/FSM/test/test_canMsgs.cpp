#include <gtest/gtest.h>

extern "C"
{
#include "app_units.h"
#include "app_canUtils.h"
}

TEST(CanMsgsTest, state_machine_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_FSM_VITALS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, heartbeat_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_FSM_VITALS_CYCLE_TIME_MS);
}
