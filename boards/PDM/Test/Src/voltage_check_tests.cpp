#include "fff.h"
#include "gtest/gtest.h"

DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(float, get_voltage);
FAKE_VALUE_FUNC(float, get_min_voltage);
FAKE_VALUE_FUNC(float, get_max_voltage);

extern "C"
{
#include "App_VoltageCheck.h"
}

class VoltageCheckTest : public testing::Test
{
  protected:
    virtual void SetUp()
    {
        RESET_FAKE(get_voltage);
        RESET_FAKE(get_min_voltage);
        RESET_FAKE(get_max_voltage);

        FFF_RESET_HISTORY();

        voltage_check = App_VoltageCheck_Create(
            get_voltage, get_min_voltage, get_max_voltage);
    }
    virtual void TearDown() { App_VoltageCheck_Destroy(voltage_check); }

    struct VoltageCheck *voltage_check;
};

TEST_F(VoltageCheckTest, check_create)
{

}

TEST_F(VoltageCheckTest, check_voltage_normal)
{
    get_voltage_fake.return_val = 5.5f;

    get_min_voltage_fake.return_val = 5.0;
    get_max_voltage_fake.return_val = 6.0;

    App_VoltageCheck_Tick(voltage_check);

    ASSERT_EQ(App_VoltageCheck_GetStatus(voltage_check), VOLTAGECHECK_NORMAL);
}

TEST_F(VoltageCheckTest, check_voltage_underflow)
{
    get_voltage_fake.return_val = 4.9f;

    get_min_voltage_fake.return_val = 5.0f;
    get_max_voltage_fake.return_val = 6.0f;

    App_VoltageCheck_Tick(voltage_check);

    ASSERT_EQ(
        App_VoltageCheck_GetStatus(voltage_check), VOLTAGECHECK_UNDERVOLTAGE);
}

TEST_F(VoltageCheckTest, check_voltage_overflow)
{
    get_voltage_fake.return_val = 6.1f;

    get_min_voltage_fake.return_val = 5.0f;
    get_max_voltage_fake.return_val = 6.0f;

    App_VoltageCheck_Tick(voltage_check);

    ASSERT_EQ(
        App_VoltageCheck_GetStatus(voltage_check), VOLTAGECHECK_OVERVOLTAGE);
}
