#include "Test_RegenPaddle.h"

extern "C"
{
#include "App_RegenPaddle.h"
}

DEFINE_FAKE_VALUE_FUNC(uint32_t, get_raw_paddle_position);

void RegenPaddleTest::SetUp()
{
    regen_paddle = App_RegenPaddle_Create(
        get_raw_paddle_position, DEFAULT_LOWER_DEADZONE,
        DEFAULT_UPPER_DEADZONE);
}

void RegenPaddleTest::TearDown()
{
    ASSERT_TRUE(regen_paddle != NULL);
    App_RegenPaddle_Destroy(regen_paddle);
    regen_paddle = NULL;
}

TEST_F(RegenPaddleTest, lower_deadzone)
{
    uint32_t buffer;
    get_raw_paddle_position_fake.return_val = DEFAULT_LOWER_DEADZONE - 1;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetRawPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(get_raw_paddle_position_fake.return_val, buffer);
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetMappedPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(0, buffer);

    get_raw_paddle_position_fake.return_val = DEFAULT_LOWER_DEADZONE;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetRawPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(get_raw_paddle_position_fake.return_val, buffer);
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetMappedPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(0, buffer);

    get_raw_paddle_position_fake.return_val = DEFAULT_LOWER_DEADZONE + 1;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetRawPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(get_raw_paddle_position_fake.return_val, buffer);
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetMappedPaddlePosition(regen_paddle, &buffer));
    ASSERT_NE(0, buffer);
}

TEST_F(RegenPaddleTest, upper_deadzone)
{
    uint32_t buffer;
    get_raw_paddle_position_fake.return_val = DEFAULT_UPPER_DEADZONE - 1;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetRawPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(get_raw_paddle_position_fake.return_val, buffer);
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetMappedPaddlePosition(regen_paddle, &buffer));
    ASSERT_NE(100, buffer);

    get_raw_paddle_position_fake.return_val = DEFAULT_UPPER_DEADZONE;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetRawPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(get_raw_paddle_position_fake.return_val, buffer);
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetMappedPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(100, buffer);

    get_raw_paddle_position_fake.return_val = DEFAULT_UPPER_DEADZONE + 1;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetRawPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(get_raw_paddle_position_fake.return_val, buffer);
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetMappedPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(100, buffer);
}

TEST_F(RegenPaddleTest, invalid_paddle_positions)
{
    uint32_t buffer;
    get_raw_paddle_position_fake.return_val = 100;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetRawPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(get_raw_paddle_position_fake.return_val, buffer);
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_RegenPaddle_GetMappedPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(100, buffer);

    get_raw_paddle_position_fake.return_val = 101;
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_RegenPaddle_GetRawPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_RegenPaddle_GetMappedPaddlePosition(regen_paddle, &buffer));

    get_raw_paddle_position_fake.return_val = 102;
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_RegenPaddle_GetRawPaddlePosition(regen_paddle, &buffer));
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_RegenPaddle_GetMappedPaddlePosition(regen_paddle, &buffer));
}
