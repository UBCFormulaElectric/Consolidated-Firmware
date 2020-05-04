#include "Test_BinarySwitch.h"

extern "C"
{
#include "App_BinarySwitch.h"
}

FAKE_VALUE_FUNC(bool, _is_turned_on);

struct BinarySwitch *
    BinarySwitchTest::SetUpBinarySwitch(bool (*is_turned_on)(void))
{
    return App_BinarySwitch_Create(is_turned_on);
}

void BinarySwitchTest::TearDownBinarySwitch(struct BinarySwitch *&binary_switch)
{
    ASSERT_TRUE(binary_switch != NULL);
    App_BinarySwitch_Destroy(binary_switch);
    binary_switch = NULL;
}

void BinarySwitchTest::SetUp()
{
    _binary_switch = App_BinarySwitch_Create(_is_turned_on);
}

void BinarySwitchTest::TearDown()
{
    ASSERT_TRUE(_binary_switch != NULL);
    App_BinarySwitch_Destroy(_binary_switch);
    _binary_switch = NULL;
}

TEST_F(BinarySwitchTest, toggle_switch)
{
    _is_turned_on_fake.return_val = false;
    ASSERT_EQ(false, App_BinarySwitch_IsTurnedOn(_binary_switch));

    _is_turned_on_fake.return_val = true;
    ASSERT_EQ(true, App_BinarySwitch_IsTurnedOn(_binary_switch));

    _is_turned_on_fake.return_val = false;
    ASSERT_EQ(false, App_BinarySwitch_IsTurnedOn(_binary_switch));
}