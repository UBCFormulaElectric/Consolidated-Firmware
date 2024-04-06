#include <gtest/gtest.h>

extern "C"
{
#include "app_units.h"
#include "app_canUtils.h"
}

TEST(CanMsgsTest, vitals_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_RSM_VITALS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, alert_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_RSM_ALERTS_CONTEXT_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, commit_info_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_RSM_COMMIT_INFO_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, coolant_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_RSM_COOLANT_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, loadcell_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(100), CAN_MSG_RSM_FAULTS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, suspension_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_RSM_SUSPENSION_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, warnings_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_RSM_WARNINGS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, faults_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_RSM_FAULTS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, warnings_counts_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_RSM_WARNINGS_COUNTS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, faults_counts_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_RSM_FAULTS_COUNTS_CYCLE_TIME_MS);
}