#include "Test_BinarySwitch.h"

extern "C"
{
#include "App_BinarySwitch.h"
}

FAKE_VALUE_FUNC(bool, is_turned_on);

void BinarySwitchTest::TearDownBinarySwitch(
    struct BinarySwitch *&binary_switch_to_teardown)
{
    ASSERT_TRUE(binary_switch_to_teardown != NULL);
    App_BinarySwitch_Destroy(binary_switch_to_teardown);
    binary_switch_to_teardown = NULL;
}

void BinarySwitchTest::SetUp()
{
    binary_switch = App_BinarySwitch_Create(is_turned_on);
}

void BinarySwitchTest::TearDown()
{
    ASSERT_TRUE(binary_switch != NULL);
    App_BinarySwitch_Destroy(binary_switch);
    binary_switch = NULL;
}

TEST_F(BinarySwitchTest, toggle_switch)
{
    is_turned_on_fake.return_val = false;
    ASSERT_EQ(false, App_BinarySwitch_IsTurnedOn(binary_switch));

    is_turned_on_fake.return_val = true;
    ASSERT_EQ(true, App_BinarySwitch_IsTurnedOn(binary_switch));

    is_turned_on_fake.return_val = false;
    ASSERT_EQ(false, App_BinarySwitch_IsTurnedOn(binary_switch));
}
