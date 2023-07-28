#include "Test_Pdm.h"

extern "C"
{
#include "App_LowVoltageBattery.h"
}

FAKE_VALUE_FUNC(bool, has_charge_fault);
FAKE_VALUE_FUNC(bool, has_boost_fault);
FAKE_VALUE_FUNC(float, get_bat_voltage);
FAKE_VALUE_FUNC(float, get_acc_voltage);
FAKE_VALUE_FUNC(float, get_boost_voltage);

class LowVoltageBatteryTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        low_voltage_battery = App_LowVoltageBattery_Create(
            has_charge_fault, has_boost_fault, get_bat_voltage, get_acc_voltage, get_boost_voltage);
        RESET_FAKE(has_charge_fault);
        RESET_FAKE(has_boost_fault);
        RESET_FAKE(get_bat_voltage);
        RESET_FAKE(get_acc_voltage);
        RESET_FAKE(get_boost_voltage);
    }

    void TearDown() override { TearDownObject(low_voltage_battery, App_LowVoltageBattery_Destroy); }

    struct LowVoltageBattery *low_voltage_battery;
};

TEST_F(LowVoltageBatteryTest, has_charge_fault)
{
    has_charge_fault_fake.return_val = true;
    ASSERT_TRUE(App_LowVoltageBattery_HasChargeFault(low_voltage_battery));
}

TEST_F(LowVoltageBatteryTest, does_not_have_charge_fault)
{
    has_charge_fault_fake.return_val = false;
    ASSERT_FALSE(App_LowVoltageBattery_HasChargeFault(low_voltage_battery));
}

TEST_F(LowVoltageBatteryTest, has_boost_fault)
{
    has_boost_fault_fake.return_val = true;
    ASSERT_TRUE(App_LowVoltageBattery_HasBoostControllerFault(low_voltage_battery));
}

TEST_F(LowVoltageBatteryTest, does_not_have_boost_fault)
{
    has_boost_fault_fake.return_val = false;
    ASSERT_FALSE(App_LowVoltageBattery_HasBoostControllerFault(low_voltage_battery));
}
