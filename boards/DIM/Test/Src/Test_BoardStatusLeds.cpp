#include "Test_Dim.h"

extern "C"
{
#include "App_BoardStatusLeds.h"
}

FAKE_VOID_FUNC(turn_on_imd_led);
FAKE_VOID_FUNC(turn_off_imd_led);
FAKE_VOID_FUNC(turn_on_bspd_led);
FAKE_VOID_FUNC(turn_off_bspd_led);

FAKE_VOID_FUNC(turn_on_dim_red);
FAKE_VOID_FUNC(turn_on_dim_green);
FAKE_VOID_FUNC(turn_on_dim_blue);

FAKE_VOID_FUNC(turn_on_dcm_red);
FAKE_VOID_FUNC(turn_on_dcm_green);
FAKE_VOID_FUNC(turn_on_dcm_blue);

FAKE_VOID_FUNC(turn_on_fsm_red);
FAKE_VOID_FUNC(turn_on_fsm_green);
FAKE_VOID_FUNC(turn_on_fsm_blue);

FAKE_VOID_FUNC(turn_on_bms_red);
FAKE_VOID_FUNC(turn_on_bms_green);
FAKE_VOID_FUNC(turn_on_bms_blue);

FAKE_VOID_FUNC(turn_on_pdm_red);
FAKE_VOID_FUNC(turn_on_pdm_green);
FAKE_VOID_FUNC(turn_on_pdm_blue);

class BoardStatusLedsTest : public testing::Test
{
  protected:
    void SetUp() override
    {
        board_status_leds = App_BoardStatusLeds_Create(
            turn_on_dim_red, turn_on_dim_green, turn_on_dim_blue,
            turn_on_dcm_red, turn_on_dcm_green, turn_on_dcm_blue,
            turn_on_fsm_red, turn_on_fsm_green, turn_on_fsm_blue,
            turn_on_bms_red, turn_on_bms_green, turn_on_bms_blue,
            turn_on_pdm_red, turn_on_pdm_green, turn_on_pdm_blue);
    }

    void TearDown() override
    {
        TearDownObject(board_status_leds, App_BoardStatusLeds_Destroy);
    }
    struct BoardStatusLeds *board_status_leds;
};

TEST_F(BoardStatusLedsTest, invalid_board)
{
    ASSERT_EQ(
        EXIT_CODE_INVALID_ARGS,
        App_BoardStatusLeds_TurnOnRed(board_status_leds, NUM_BOARDS));
    ASSERT_EQ(
        EXIT_CODE_INVALID_ARGS,
        App_BoardStatusLeds_TurnOnGreen(board_status_leds, NUM_BOARDS));
    ASSERT_EQ(
        EXIT_CODE_INVALID_ARGS,
        App_BoardStatusLeds_TurnOnBlue(board_status_leds, NUM_BOARDS));
}

TEST_F(BoardStatusLedsTest, turn_dim_status_led_red)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnRed(board_status_leds, DIM));
    ASSERT_EQ(1, turn_on_dim_red_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_dim_status_led_green)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnGreen(board_status_leds, DIM));
    ASSERT_EQ(1, turn_on_dim_green_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_dim_status_led_blue)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnBlue(board_status_leds, DIM));
    ASSERT_EQ(1, turn_on_dim_blue_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_dcm_status_led_red)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnRed(board_status_leds, DCM));
    ASSERT_EQ(1, turn_on_dcm_red_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_dcm_status_led_green)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnGreen(board_status_leds, DCM));
    ASSERT_EQ(1, turn_on_dcm_green_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_dcm_status_led_blue)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnBlue(board_status_leds, DCM));
    ASSERT_EQ(1, turn_on_dcm_blue_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_fsm_status_led_red)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnRed(board_status_leds, FSM));
    ASSERT_EQ(1, turn_on_fsm_red_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_fsm_status_led_green)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnGreen(board_status_leds, FSM));
    ASSERT_EQ(1, turn_on_fsm_green_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_fsm_status_led_blue)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnBlue(board_status_leds, FSM));
    ASSERT_EQ(1, turn_on_fsm_blue_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_bms_status_led_red)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnRed(board_status_leds, BMS));
    ASSERT_EQ(1, turn_on_bms_red_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_bms_status_led_green)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnGreen(board_status_leds, BMS));
    ASSERT_EQ(1, turn_on_bms_green_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_bms_status_led_blue)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnBlue(board_status_leds, BMS));
    ASSERT_EQ(1, turn_on_bms_blue_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_pdm_status_led_red)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnRed(board_status_leds, PDM));
    ASSERT_EQ(1, turn_on_pdm_red_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_pdm_status_led_green)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnGreen(board_status_leds, PDM));
    ASSERT_EQ(1, turn_on_pdm_green_fake.call_count);
}

TEST_F(BoardStatusLedsTest, turn_pdm_status_led_blue)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_BoardStatusLeds_TurnOnBlue(board_status_leds, PDM));
    ASSERT_EQ(1, turn_on_pdm_blue_fake.call_count);
}
