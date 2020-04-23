#include "Test_RegenPaddle.h"

extern "C"
{
#include "App_RegenPaddle.h"
}

DEFINE_FAKE_VALUE_FUNC(uint32_t, get_paddle_position);

void RegenPaddleTest::SetUp()
{
    regen_paddle = App_RegenPaddle_Create(
        get_paddle_position, DEFAULT_LOWER_DEADZONE, DEFAULT_UPPER_DEADZONE);
}

void RegenPaddleTest::TearDown()
{
    ASSERT_TRUE(regen_paddle != NULL);
    App_RegenPaddle_Destroy(regen_paddle);
    regen_paddle = NULL;
}

TEST_F(RegenPaddleTest, lower_deadzone)
{
    get_paddle_position_fake.return_val = DEFAULT_LOWER_DEADZONE - 1;
    ASSERT_EQ(ERROR_CODE_OK, App_RegenPaddle_Tick(regen_paddle));
    ASSERT_EQ(
        DEFAULT_LOWER_DEADZONE - 1,
        App_RegenPaddle_GetPaddlePosition(regen_paddle));
    ASSERT_EQ(0, App_RegenPaddle_GetRegen(regen_paddle));

    get_paddle_position_fake.return_val = DEFAULT_LOWER_DEADZONE;
    ASSERT_EQ(ERROR_CODE_OK, App_RegenPaddle_Tick(regen_paddle));
    ASSERT_EQ(
        DEFAULT_LOWER_DEADZONE,
        App_RegenPaddle_GetPaddlePosition(regen_paddle));
    ASSERT_EQ(0, App_RegenPaddle_GetRegen(regen_paddle));

    get_paddle_position_fake.return_val = DEFAULT_LOWER_DEADZONE + 1;
    ASSERT_EQ(ERROR_CODE_OK, App_RegenPaddle_Tick(regen_paddle));
    ASSERT_EQ(
        DEFAULT_LOWER_DEADZONE + 1,
        App_RegenPaddle_GetPaddlePosition(regen_paddle));
    ASSERT_NE(0, App_RegenPaddle_GetRegen(regen_paddle));
}

TEST_F(RegenPaddleTest, upper_deadzone)
{
    get_paddle_position_fake.return_val = DEFAULT_UPPER_DEADZONE - 1;
    ASSERT_EQ(ERROR_CODE_OK, App_RegenPaddle_Tick(regen_paddle));
    ASSERT_EQ(
        DEFAULT_UPPER_DEADZONE - 1,
        App_RegenPaddle_GetPaddlePosition(regen_paddle));
    ASSERT_NE(100, App_RegenPaddle_GetRegen(regen_paddle));

    get_paddle_position_fake.return_val = DEFAULT_UPPER_DEADZONE;
    ASSERT_EQ(ERROR_CODE_OK, App_RegenPaddle_Tick(regen_paddle));
    ASSERT_EQ(
        DEFAULT_UPPER_DEADZONE,
        App_RegenPaddle_GetPaddlePosition(regen_paddle));
    ASSERT_EQ(100, App_RegenPaddle_GetRegen(regen_paddle));

    get_paddle_position_fake.return_val = DEFAULT_UPPER_DEADZONE + 1;
    ASSERT_EQ(ERROR_CODE_OK, App_RegenPaddle_Tick(regen_paddle));
    ASSERT_EQ(
        DEFAULT_UPPER_DEADZONE + 1,
        App_RegenPaddle_GetPaddlePosition(regen_paddle));
    ASSERT_EQ(100, App_RegenPaddle_GetRegen(regen_paddle));
}

TEST_F(RegenPaddleTest, invalid_paddle_positions)
{
    get_paddle_position_fake.return_val = 100;
    ASSERT_EQ(ERROR_CODE_OK, App_RegenPaddle_Tick(regen_paddle));
    ASSERT_EQ(100, App_RegenPaddle_GetPaddlePosition(regen_paddle));
    ASSERT_EQ(100, App_RegenPaddle_GetRegen(regen_paddle));

    get_paddle_position_fake.return_val = 101;
    ASSERT_EQ(ERROR_CODE_OUT_OF_RANGE, App_RegenPaddle_Tick(regen_paddle));
    ASSERT_EQ(100, App_RegenPaddle_GetPaddlePosition(regen_paddle));
    ASSERT_EQ(100, App_RegenPaddle_GetRegen(regen_paddle));

    get_paddle_position_fake.return_val = 102;
    ASSERT_EQ(ERROR_CODE_OUT_OF_RANGE, App_RegenPaddle_Tick(regen_paddle));
    ASSERT_EQ(100, App_RegenPaddle_GetPaddlePosition(regen_paddle));
    ASSERT_EQ(100, App_RegenPaddle_GetRegen(regen_paddle));
}
