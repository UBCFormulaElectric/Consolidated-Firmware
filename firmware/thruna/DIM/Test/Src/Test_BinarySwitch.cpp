#include "Test_Dim.h"

extern "C"
{
#include "App_BinarySwitch.h"
}

FAKE_VALUE_FUNC(bool, is_turned_on);

class BinarySwitchTest : public testing::Test
{
  protected:
    void SetUp() override { binary_switch = App_BinarySwitch_Create(is_turned_on); }

    void                 TearDown() override { TearDownObject(binary_switch, App_BinarySwitch_Destroy); }
    struct BinarySwitch *binary_switch;
};

TEST_F(BinarySwitchTest, toggle_switch)
{
    is_turned_on_fake.return_val = false;
    ASSERT_EQ(false, App_BinarySwitch_IsTurnedOn(binary_switch));

    is_turned_on_fake.return_val = true;
    ASSERT_EQ(true, App_BinarySwitch_IsTurnedOn(binary_switch));

    is_turned_on_fake.return_val = false;
    ASSERT_EQ(false, App_BinarySwitch_IsTurnedOn(binary_switch));
}
