#include "Test_Bms.h"

extern "C"
{
#include "App_CanUtils.h"
}

TEST(CanMsgsTest, vitals_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_VITALS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, fifo_overflow_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(0.2), CAN_MSG_BMS_CAN_FIFO_OVERFLOW_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, watchdog_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_BMS_WATCHDOG_TIMEOUT_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, warnings_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_BMS_WARNINGS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, imd_status_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_BMS_IMD_STATUS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, imd_data_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_BMS_IMD_DATA_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, imd_pwm_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_BMS_IMD_PWM_OUTPUT_CYCLE_TIME_MS);
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
    ASSERT_GE(HZ_TO_MS(100), CAN_MSG_BMS_OK_STATUSES_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, air_states_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(100), CAN_MSG_BMS_AIR_STATES_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, mortor_shutdown_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(1), CAN_MSG_BMS_MOTOR_SHUTDOWN_ERRORS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, cell_temp_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_CELL_TEMPERATURES_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, cell_voltage_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_CELL_VOLTAGES_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, cell_stats_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_CELL_STATS_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, tractive_system_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_TRACTIVE_SYSTEM_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, pack_voltage_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_PACK_VOLTAGE_CYCLE_TIME_MS);
}

TEST(CanMsgsTest, availible_power_message_frequency)
{
    ASSERT_GE(HZ_TO_MS(10), CAN_MSG_BMS_AVAILABLE_POWER_CYCLE_TIME_MS);
}
