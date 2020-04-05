#include "fff.h"
#include "gtest/gtest.h"

extern "C"
{
#include "App_VoltageMonitor.h"

    DEFINE_FFF_GLOBALS;
    FAKE_VALUE_FUNC(float, get_voltage);
    FAKE_VALUE_FUNC(float, get_min_voltage);
    FAKE_VALUE_FUNC(float, get_max_voltage);
}

class VoltageMonitorTest : public testing::Test
{
  protected:
    virtual void SetUp()
    {
        RESET_FAKE(get_voltage);
        RESET_FAKE(get_min_voltage);
        RESET_FAKE(get_max_voltage);

        FFF_RESET_HISTORY();

        voltage_monitor = App_VoltageMonitor_Create(
            get_voltage, get_min_voltage, get_max_voltage);
    }
    virtual void TearDown() { App_VoltageMonitor_Destroy(voltage_monitor); }

    struct VoltageMonitor *voltage_monitor;

    const float MIN_VOLTAGE = 5.0f;
    const float MAX_VOLTAGE = 6.0f;
};

TEST_F(VoltageMonitorTest, check_normal_voltage)
{
    get_voltage_fake.return_val     = (MIN_VOLTAGE + MAX_VOLTAGE) / 2.0f;
    get_min_voltage_fake.return_val = MIN_VOLTAGE;
    get_max_voltage_fake.return_val = MAX_VOLTAGE;

    App_VoltageMonitor_Tick(voltage_monitor);

    ASSERT_EQ(
        App_VoltageMonitor_GetStatus(voltage_monitor), VOLTAGEMONITOR_NORMAL);
}

TEST_F(VoltageMonitorTest, check_undervoltage)
{
    get_voltage_fake.return_val     = MIN_VOLTAGE - 0.1f;
    get_min_voltage_fake.return_val = MIN_VOLTAGE;
    get_max_voltage_fake.return_val = MAX_VOLTAGE;

    App_VoltageMonitor_Tick(voltage_monitor);

    ASSERT_EQ(
        App_VoltageMonitor_GetStatus(voltage_monitor),
        VOLTAGEMONITOR_UNDERVOLTAGE);
}

TEST_F(VoltageMonitorTest, check_overvoltage)
{
    get_voltage_fake.return_val     = MAX_VOLTAGE + 0.1f;
    get_min_voltage_fake.return_val = MIN_VOLTAGE;
    get_max_voltage_fake.return_val = MAX_VOLTAGE;

    App_VoltageMonitor_Tick(voltage_monitor);

    ASSERT_EQ(
        App_VoltageMonitor_GetStatus(voltage_monitor),
        VOLTAGEMONITOR_OVERVOLTAGE);
}
