#include "Test_Dim.h"

extern "C"
{
#include "App_RegenPaddle.h"
}

FAKE_VALUE_FUNC(float, get_paddle_position);

class RegenPaddleTest : public DimTest
{
  protected:
    void SetUp() override
    {
        regen_paddle = App_RegenPaddle_Create(
            get_paddle_position, DEFAULT_LOWER_DEADZONE,
            DEFAULT_UPPER_DEADZONE);
    }

    void TearDown() override
    {
        ASSERT_TRUE(regen_paddle != NULL);
        App_RegenPaddle_Destroy(regen_paddle);
        regen_paddle = NULL;
    }
    const float         DEFAULT_LOWER_DEADZONE = 5.0f;
    const float         DEFAULT_UPPER_DEADZONE = 95.0f;
    struct RegenPaddle *regen_paddle;
};

TEST_F(RegenPaddleTest, check_lower_deadzone)
{
    get_paddle_position_fake.return_val = std::nextafter(
        DEFAULT_LOWER_DEADZONE, std::numeric_limits<float>::min());
    App_RegenPaddle_Tick(regen_paddle);
    ASSERT_EQ(0.0f, App_RegenPaddle_GetRegen(regen_paddle));

    get_paddle_position_fake.return_val = DEFAULT_LOWER_DEADZONE;
    App_RegenPaddle_Tick(regen_paddle);
    ASSERT_EQ(0.0f, App_RegenPaddle_GetRegen(regen_paddle));

    get_paddle_position_fake.return_val = std::nextafter(
        DEFAULT_LOWER_DEADZONE, std::numeric_limits<float>::max());
    App_RegenPaddle_Tick(regen_paddle);
    ASSERT_NE(0.0f, App_RegenPaddle_GetRegen(regen_paddle));
}

TEST_F(RegenPaddleTest, check_upper_deadzone)
{
    get_paddle_position_fake.return_val = std::nextafter(
        DEFAULT_UPPER_DEADZONE, std::numeric_limits<float>::min());
    App_RegenPaddle_Tick(regen_paddle);
    ASSERT_NE(100.0f, App_RegenPaddle_GetRegen(regen_paddle));

    get_paddle_position_fake.return_val = DEFAULT_UPPER_DEADZONE;
    App_RegenPaddle_Tick(regen_paddle);
    ASSERT_EQ(100.0f, App_RegenPaddle_GetRegen(regen_paddle));

    get_paddle_position_fake.return_val = std::nextafter(
        DEFAULT_UPPER_DEADZONE, std::numeric_limits<float>::max());
    App_RegenPaddle_Tick(regen_paddle);
    ASSERT_EQ(100.0f, App_RegenPaddle_GetRegen(regen_paddle));
}
