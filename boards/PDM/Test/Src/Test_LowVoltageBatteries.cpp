#include "Test_Pdm.h"

extern "C"
{
#include "App_LowVoltageBatteries.h"
}

FAKE_VALUE_FUNC(bool, is_overvoltage);
FAKE_VALUE_FUNC(bool, has_charge_fault);
FAKE_VALUE_FUNC(bool, has_boost_fault);

class LowVoltageBatteriesTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        low_voltage_batteries = App_LowVoltageBatteries_Create(
            is_overvoltage, has_charge_fault, has_boost_fault);
        RESET_FAKE(is_overvoltage);
        RESET_FAKE(has_charge_fault);
        RESET_FAKE(has_boost_fault);
    }

    void TearDown() override
    {
        TearDownObject(low_voltage_batteries, App_LowVoltageBatteries_Destroy);
    }

    struct LowVoltageBatteries *low_voltage_batteries;
};

TEST_F(LowVoltageBatteriesTest, is_overvoltage)
{
    is_overvoltage_fake.return_val = true;
    ASSERT_TRUE(App_LowVoltageBatteries_IsOvervoltage(low_voltage_batteries));
}

TEST_F(LowVoltageBatteriesTest, is_not_overvoltage)
{
    is_overvoltage_fake.return_val = false;
    ASSERT_FALSE(App_LowVoltageBatteries_IsOvervoltage(low_voltage_batteries));
}

TEST_F(LowVoltageBatteriesTest, has_charge_fault)
{
    has_charge_fault_fake.return_val = true;
    ASSERT_TRUE(App_LowVoltageBatteries_HasChargeFault(low_voltage_batteries));
}

TEST_F(LowVoltageBatteriesTest, does_not_have_charge_fault)
{
    has_charge_fault_fake.return_val = false;
    ASSERT_FALSE(App_LowVoltageBatteries_HasChargeFault(low_voltage_batteries));
}

TEST_F(LowVoltageBatteriesTest, has_boost_fault)
{
    has_boost_fault_fake.return_val = true;
    ASSERT_TRUE(
        App_LowVoltageBatteries_HasBoostControllerFault(low_voltage_batteries));
}

TEST_F(LowVoltageBatteriesTest, does_not_have_boost_fault)
{
    has_boost_fault_fake.return_val = false;
    ASSERT_FALSE(
        App_LowVoltageBatteries_HasBoostControllerFault(low_voltage_batteries));
}
