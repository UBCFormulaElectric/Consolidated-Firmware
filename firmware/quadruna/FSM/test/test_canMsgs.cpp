#include <gtest/gtest.h>

extern "C"
{
#include "app_units.h"
#include "app_canUtils.h"
}

TEST(CanMsgsTest, vitals_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_FSM_VITALS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, alert_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_FSM_ALERTS_CONTEXT_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, commit_info_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_FSM_COMMIT_INFO_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, wheels_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_FSM_WHEELS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, apps_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_FSM_APPS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, brake_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_FSM_BRAKE_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, suspension_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_FSM_SUSPENSION_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, warnings_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_FSM_WARNINGS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, faults_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_FSM_FAULTS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, warnings_counts_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_FSM_WARNINGS_COUNTS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, faults_counts_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_FSM_FAULTS_COUNTS_CYCLE_TIME_MS);
}