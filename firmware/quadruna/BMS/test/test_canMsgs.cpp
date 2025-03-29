#include <gtest/gtest.h>

extern "C"
{
#include "app_units.h"
#include "app_canUtils.h"
}

TEST(CanMsgsTest, vitals_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_VITALS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, imd_status_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_BMS_IMD_STATUS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, imd_data_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_BMS_IMD_DATA_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, faults_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_BMS_FAULTS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, charger_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_BMS_CHARGER_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, ok_status_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_OK_STATUSES_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, contactors_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_CONTACTORS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, cell_temp_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_BMS_CELL_TEMPERATURES_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, cell_voltage_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_VOLTAGE_AND_CHARGE_STATS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, cell_stats_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_CELL_STATS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, tractive_system_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_TRACTIVE_SYSTEM_CYCLE_TIME_MS);
}
