#include <memory>
#include "Test_Shared.h"

extern "C"
{
#include "App_SharedErrorTable.h"
#include "App_SharedMacros.h"
}

class SharedErrorTableTest : public testing::Test
{
  protected:
    void SetUp() override { error_table = App_SharedErrorTable_Create(); }
    void TearDown() override
    {
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
    }

    struct ErrorTable *error_table;
};

TEST_F(SharedErrorTableTest, is_board_in_list)
{
    enum ErrorBoard boards[NUM_ERROR_BOARDS];

    // The board exists in the list
    boards[0] = BMS;
    boards[1] = DCM;
    boards[2] = DIM;
    boards[3] = FSM;
    boards[4] = PDM;
    ASSERT_TRUE(
        App_SharedErrorTable_IsBoardInList(boards, NUM_ERROR_BOARDS, BMS));

    // The board does not exist in the list
    boards[0] = BMS;
    boards[1] = BMS;
    boards[2] = BMS;
    boards[3] = BMS;
    boards[4] = BMS;
    ASSERT_FALSE(
        App_SharedErrorTable_IsBoardInList(boards, NUM_ERROR_BOARDS, DCM));

    // The board we are looking for has multiple entries in the list
    boards[0] = BMS;
    boards[1] = BMS;
    boards[2] = DIM;
    boards[3] = FSM;
    boards[4] = PDM;
    ASSERT_TRUE(
        App_SharedErrorTable_IsBoardInList(boards, NUM_ERROR_BOARDS, BMS));

    // The board exists in the list but is out-of-bound
    boards[0] = BMS;
    boards[1] = DCM;
    boards[2] = DIM;
    boards[3] = FSM;
    boards[4] = PDM;
    ASSERT_FALSE(
        App_SharedErrorTable_IsBoardInList(boards, NUM_ERROR_BOARDS - 1, PDM));
}

TEST_F(SharedErrorTableTest, get_board_for_error)
{
    //    struct Error error.board = BMS;
    //    ASSERT_EQ(BMS, App_SharedErrorTable_GetBoardForError(&error));
}

TEST_F(SharedErrorTableTest, is_error_critical)
{
    //    struct Error error.is_critical = true;
    //    ASSERT_TRUE(App_SharedErrorTable_IsErrorCritical(error));
    //
    //    error.is_critical = false;
    //    ASSERT_FALSE(App_SharedErrorTable_IsErrorCritical(error));
}

TEST_F(SharedErrorTableTest, set_error)
{
    // Set error
    bool is_set = false;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_SetError(error_table, BMS_FOO, true));
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_IsErrorSet(error_table, BMS_FOO, &is_set));
    ASSERT_TRUE(is_set);

    // Clear error
    is_set = true;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_SetError(error_table, BMS_FOO, false));
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_IsErrorSet(error_table, BMS_FOO, &is_set));
    ASSERT_FALSE(is_set);

    // Out-of-range ID
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_SharedErrorTable_SetError(error_table, NUM_ERROR_IDS, true));
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_SharedErrorTable_SetError(error_table, NUM_ERROR_IDS, false));
}

TEST_F(SharedErrorTableTest, is_error_set)
{
    // In-range ID
    bool is_set = false;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_SetError(error_table, BMS_FOO, true));
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_IsErrorSet(error_table, BMS_FOO, &is_set));
    ASSERT_TRUE(is_set);

    // Out-of-range ID
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_SharedErrorTable_IsErrorSet(error_table, NUM_ERROR_IDS, &is_set));
}

TEST_F(SharedErrorTableTest, has_error)
{
    // Set an error
    ASSERT_FALSE(App_SharedErrorTable_HasError(error_table));
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_SetError(error_table, BMS_FOO, true));
    ASSERT_TRUE(App_SharedErrorTable_HasError(error_table));

    // Clear an error
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_SetError(error_table, BMS_FOO, false));
    ASSERT_FALSE(App_SharedErrorTable_HasError(error_table));
}

TEST_F(SharedErrorTableTest, has_critical_error)
{
    // Same as has_error, but with critical error
}

TEST_F(SharedErrorTableTest, has_non_critical_error)
{
    // Same as has_error, but with non-critical error
}

TEST_F(SharedErrorTableTest, get_all_errors)
{
    // TODO: How do we prepare the "errors[]" argument for this function?
    // App_SharedErrorTable_GetAllErrors(error_table, )
}

TEST_F(SharedErrorTableTest, get_all_critical_errors)
{
    // Same as get_all_errors, but with critical error
}

TEST_F(SharedErrorTableTest, get_all_non_critical_errors)
{
    // Same as get_all_errors, but with non-critical error
}

TEST_F(SharedErrorTableTest, get_boards_with_no_errors)
{
    enum ErrorBoard boards[NUM_ERROR_BOARDS];
    memset(boards, 0, NUM_ELEMENTS_IN_ARRAY(boards));
    uint32_t num_boards = 0;

    // No board has any error
    App_SharedErrorTable_GetBoardsWithNoErrors(
        error_table, boards, &num_boards);
    ASSERT_EQ(NUM_ERROR_BOARDS, num_boards);

    // One board has one or more errors

    // Every board has one or more errors
}

TEST_F(SharedErrorTableTest, get_boards_with_errors)
{
    enum ErrorBoard boards[NUM_ERROR_BOARDS];
    memset(boards, 0, NUM_ELEMENTS_IN_ARRAY(boards));
    uint32_t num_boards = 0;

    // No board has any error
    App_SharedErrorTable_GetBoardsWithErrors(error_table, boards, &num_boards);
    ASSERT_EQ(0, num_boards);

    // One board has one or more errors

    // Every board has one or more errors
}

TEST_F(SharedErrorTableTest, get_boards_with_critical_errors)
{
    enum ErrorBoard boards[NUM_ERROR_BOARDS];
    memset(boards, 0, NUM_ELEMENTS_IN_ARRAY(boards));
    uint32_t num_boards = 0;

    // No board has any critical error
    App_SharedErrorTable_GetBoardsWithCriticalErrors(
        error_table, boards, &num_boards);
    ASSERT_EQ(0, num_boards);

    // One board has one or more critical errors

    // Every board has one or more critical errors
}

TEST_F(SharedErrorTableTest, get_boards_with_non_critical_errors)
{
    enum ErrorBoard boards[NUM_ERROR_BOARDS];
    memset(boards, 0, NUM_ELEMENTS_IN_ARRAY(boards));
    uint32_t num_boards = 0;

    // No board has any non-critical error
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, boards, &num_boards);
    ASSERT_EQ(0, num_boards);

    // One board has one or more non-critical errors

    // Every board has one or more non-critical errors
}
