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

    void ResetWithOneCriticalErrorForOneBoard(void)
    {
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
        error_table = App_SharedErrorTable_Create();

        ASSERT_EQ(
            EXIT_CODE_OK, App_SharedErrorTable_SetError(
                              error_table, DEFAULT_CRITICAL_ERROR, true));
    }

    void ResetWithOneCriticalErrorForEveryBoard(void)
    {
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
        error_table = App_SharedErrorTable_Create();

        ASSERT_EQ(
            EXIT_CODE_OK, App_SharedErrorTable_SetError(
                              error_table, DEFAULT_BMS_CRITICAL_ERROR, true));
        ASSERT_EQ(
            EXIT_CODE_OK, App_SharedErrorTable_SetError(
                              error_table, DEFAULT_DCM_CRITICAL_ERROR, true));
        ASSERT_EQ(
            EXIT_CODE_OK, App_SharedErrorTable_SetError(
                              error_table, DEFAULT_DIM_CRITICAL_ERROR, true));
        ASSERT_EQ(
            EXIT_CODE_OK, App_SharedErrorTable_SetError(
                              error_table, DEFAULT_FSM_CRITICAL_ERROR, true));
        ASSERT_EQ(
            EXIT_CODE_OK, App_SharedErrorTable_SetError(
                              error_table, DEFAULT_PDM_CRITICAL_ERROR, true));
    }

    void ResetWithOneNonCriticalErrorForOneBoard(void)
    {
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
        error_table = App_SharedErrorTable_Create();

        ASSERT_EQ(
            EXIT_CODE_OK, App_SharedErrorTable_SetError(
                              error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    }

    void ResetWithOneNonCriticalErrorForEveryBoard(void)
    {
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
        error_table = App_SharedErrorTable_Create();

        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_SetError(
                error_table, DEFAULT_BMS_NON_CRITICAL_ERROR, true));
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_SetError(
                error_table, DEFAULT_DCM_NON_CRITICAL_ERROR, true));
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_SetError(
                error_table, DEFAULT_DIM_NON_CRITICAL_ERROR, true));
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_SetError(
                error_table, DEFAULT_FSM_NON_CRITICAL_ERROR, true));
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_SetError(
                error_table, DEFAULT_PDM_NON_CRITICAL_ERROR, true));
    }

    std::vector<enum Board> GetAllBoards(void)
    {
        return std::vector<enum Board>{
            BMS, DCM, DIM, FSM, PDM,
        };
    }

    // The error choices here are arbitrary. We just need to pick something for
    // the helper functions.
    const enum ErrorId DEFAULT_CRITICAL_ERROR = BMS_CRITICAL_DUMMY;
    const enum ErrorId DEFAULT_NON_CRITICAL_ERROR =
        BMS_NON_CRITICAL_WATCHDOG_TIMEOUT;
    const enum Board DEFAULT_CRITICAL_ERROR_BOARD     = BMS;
    const enum Board DEFAULT_NON_CRITICAL_ERROR_BOARD = BMS;

    const enum ErrorId DEFAULT_BMS_CRITICAL_ERROR = BMS_CRITICAL_DUMMY;
    const enum ErrorId DEFAULT_DCM_CRITICAL_ERROR = DCM_CRITICAL_DUMMY;
    const enum ErrorId DEFAULT_DIM_CRITICAL_ERROR = DIM_CRITICAL_DUMMY;
    const enum ErrorId DEFAULT_FSM_CRITICAL_ERROR = FSM_CRITICAL_DUMMY;
    const enum ErrorId DEFAULT_PDM_CRITICAL_ERROR = PDM_CRITICAL_DUMMY;

    const enum ErrorId DEFAULT_BMS_NON_CRITICAL_ERROR =
        BMS_NON_CRITICAL_WATCHDOG_TIMEOUT;
    const enum ErrorId DEFAULT_DCM_NON_CRITICAL_ERROR =
        DCM_NON_CRITICAL_WATCHDOG_TIMEOUT;
    const enum ErrorId DEFAULT_DIM_NON_CRITICAL_ERROR =
        DIM_NON_CRITICAL_WATCHDOG_TIMEOUT;
    const enum ErrorId DEFAULT_FSM_NON_CRITICAL_ERROR =
        FSM_NON_CRITICAL_WATCHDOG_TIMEOUT;
    const enum ErrorId DEFAULT_PDM_NON_CRITICAL_ERROR =
        PDM_NON_CRITICAL_WATCHDOG_TIMEOUT;

    struct ErrorTable *   error_table;
    struct ErrorList      error_list;
    struct ErrorBoardList board_list;
};

TEST_F(SharedErrorTableTest, board_does_not_exist_in_list)
{
    // The board to look for does not exist in the list
    board_list.boards[0]  = BMS;
    board_list.boards[1]  = BMS;
    board_list.boards[2]  = BMS;
    board_list.boards[3]  = BMS;
    board_list.boards[4]  = BMS;
    board_list.num_boards = 5;
    ASSERT_FALSE(App_SharedError_IsBoardInList(&board_list, DCM));
}

TEST_F(SharedErrorTableTest, board_exists_in_list)
{
    // The board to look for exists in the list
    board_list.boards[0]  = BMS;
    board_list.boards[1]  = DCM;
    board_list.boards[2]  = DIM;
    board_list.boards[3]  = FSM;
    board_list.boards[4]  = PDM;
    board_list.num_boards = 5;
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, BMS));
}

TEST_F(SharedErrorTableTest, board_exists_more_than_once_in_list)
{
    // The board to look for has multiple entries in the list
    board_list.boards[0] = BMS;
    board_list.boards[1] = BMS;
    board_list.boards[2] = DIM;
    board_list.boards[3] = FSM;
    board_list.boards[4] = PDM;
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, BMS));
}

TEST_F(SharedErrorTableTest, board_in_list_is_out_of_bound)
{
    // The board to look for exists in the list but is out-of-bound
    board_list.boards[0]  = BMS;
    board_list.boards[1]  = DCM;
    board_list.boards[2]  = DIM;
    board_list.boards[3]  = FSM;
    board_list.boards[4]  = PDM;
    board_list.num_boards = 4;
    ASSERT_FALSE(App_SharedError_IsBoardInList(&board_list, PDM));
}

TEST_F(SharedErrorTableTest, get_board_for_error)
{
    // Set an error in the error table
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));

    // Get the error from the error table
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // Make sure the error belongs to the correct board
    ASSERT_EQ(
        DEFAULT_CRITICAL_ERROR_BOARD,
        App_SharedError_GetBoard(error_list.errors[0]));
}

TEST_F(SharedErrorTableTest, is_error_critical)
{
    // Set a critical error in the error table
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));

    // Get errors from the error table
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);

    // Make sure there is only one error and it's a critical one
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_TRUE(App_SharedError_GetIsCritical(error_list.errors[0]));
}

TEST_F(SharedErrorTableTest, set_error)
{
    // Set an error
    bool is_set = false;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_SetError(
            error_table, BMS_NON_CRITICAL_WATCHDOG_TIMEOUT, true));

    // Make sure the error is indeed set
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_IsErrorSet(
            error_table, BMS_NON_CRITICAL_WATCHDOG_TIMEOUT, &is_set));
    ASSERT_TRUE(is_set);
}

TEST_F(SharedErrorTableTest, clear_error)
{
    // Clear an error
    bool is_set = true;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_SetError(
            error_table, BMS_NON_CRITICAL_WATCHDOG_TIMEOUT, false));

    // Make sure the error is indeed cleared
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_IsErrorSet(
            error_table, BMS_NON_CRITICAL_WATCHDOG_TIMEOUT, &is_set));
    ASSERT_FALSE(is_set);
}

TEST_F(SharedErrorTableTest, set_error_using_out_of_range_id)
{
    // Set an error using an out-of-range error ID
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_SharedErrorTable_SetError(error_table, NUM_ERROR_IDS, true));
}

TEST_F(SharedErrorTableTest, clear_error_using_out_of_range_id)
{
    // Clear an error using an out-of-range error ID
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_SharedErrorTable_SetError(error_table, NUM_ERROR_IDS, false));
}

TEST_F(SharedErrorTableTest, has_any_error_set_using_critical_error)
{
    // Set a critical error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyErrorSet(error_table));

    // Clear the critical error that was just set
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
}

TEST_F(SharedErrorTableTest, has_any_error_set_using_non_critical_error)
{
    // Set a non-critical error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyErrorSet(error_table));

    // Clear the non-critical error that was just set
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
}

TEST_F(SharedErrorTableTest, has_any_critical_error_set)
{
    // Set a critical error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyCriticalErrorSet(error_table));

    // Clear the critical error that was just set
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasAnyCriticalErrorSet(error_table));
}

TEST_F(SharedErrorTableTest, has_any_non_critical_error_set)
{
    // Set a non-critical error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyNonCriticalErrorSet(error_table));
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyNonCriticalErrorSet(error_table));

    // Clear the non-critical error that was just set
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasAnyNonCriticalErrorSet(error_table));
}

TEST_F(SharedErrorTableTest, get_all_errors_using_no_error)
{
    // No error
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);

    // Make sure we retrieve no error
    ASSERT_EQ(0, error_list.num_errors);
}

TEST_F(SharedErrorTableTest, get_all_errors_using_one_critical_error)
{
    // Set a critical error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));

    // Make sure the critical error that was just set can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_TRUE(
        App_SharedError_IsErrorInList(&error_list, DEFAULT_CRITICAL_ERROR));

    // Setting the same critical error should not modify the error list
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));

    // Again, make sure the critical error that was just set can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_TRUE(
        App_SharedError_IsErrorInList(&error_list, DEFAULT_CRITICAL_ERROR));
}

TEST_F(
    SharedErrorTableTest,
    get_all_errors_using_one_critical_error_and_one_non_critical_error)
{
    // Set a critical error and a non-critical_error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));

    // Make sure the critical error and non-critical error that were just set
    // can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(2, error_list.num_errors);
    ASSERT_TRUE(
        App_SharedError_IsErrorInList(&error_list, DEFAULT_CRITICAL_ERROR));
    ASSERT_TRUE(
        App_SharedError_IsErrorInList(&error_list, DEFAULT_NON_CRITICAL_ERROR));

    // Setting the same critical error and non-critical error should not modify
    // the error list
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));

    // Again, make sure the critical error and non-critical error that were just
    // set can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(2, error_list.num_errors);
    ASSERT_TRUE(
        App_SharedError_IsErrorInList(&error_list, DEFAULT_CRITICAL_ERROR));
    ASSERT_TRUE(
        App_SharedError_IsErrorInList(&error_list, DEFAULT_NON_CRITICAL_ERROR));
}

TEST_F(SharedErrorTableTest, get_all_errors_using_every_error)
{
    // Set every error
    for (size_t id = 0; id < NUM_ERROR_IDS; id++)
    {
        bool is_set = false;
        App_SharedErrorTable_SetError(error_table, (enum ErrorId)id, true);
        EXPECT_EQ(
            EXIT_CODE_OK, App_SharedErrorTable_IsErrorSet(
                              error_table, (enum ErrorId)id, &is_set));
        ASSERT_TRUE(is_set);
    }

    // Make sure every error can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(NUM_ERROR_IDS, error_list.num_errors);
    for (size_t id = 0; id < NUM_ERROR_IDS; id++)
    {
        EXPECT_TRUE(
            App_SharedError_IsErrorInList(&error_list, (enum ErrorId)id));
    }
}

TEST_F(SharedErrorTableTest, get_all_critical_errors_using_no_error)
{
    // No error
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);

    // Make sure we retrieve no error
    ASSERT_EQ(0, error_list.num_errors);
}

TEST_F(SharedErrorTableTest, get_all_critical_errors_using_one_critical_error)
{
    // Set a critical error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));

    // Make sure the critical error that was just set can be retrieved
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // Setting the same critical error should not modify the error list
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));

    // Again, make sure the critical error that was just set can be retrieved
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
}

TEST_F(
    SharedErrorTableTest,
    get_all_critical_errors_using_one_critical_error_and_one_non_critical_error)
{
    // Set a critical error and a non-critical_error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));

    // Make sure we only retrieve one critical error, even though there are
    // two errors that are set
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
}

TEST_F(
    SharedErrorTableTest,
    get_all_critical_errors_using_one_critical_error_per_board_and_one_non_critical_error)
{
    // Set a critical error for each board plus an additional non-critical error
    ResetWithOneCriticalErrorForEveryBoard();
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));

    // Make sure we only retrieve as many critical errors as there are boards,
    // even though there are (boards + 1) errors that are set
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(NUM_BOARDS, error_list.num_errors);
}

TEST_F(SharedErrorTableTest, get_all_non_critical_errors_using_no_errors)
{
    // No error
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);

    // Make sure we retrieve no error
    ASSERT_EQ(0, error_list.num_errors);
}

TEST_F(
    SharedErrorTableTest,
    get_all_non_critical_errors_using_one_non_critical_error)
{
    // Set a non-critical error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // Setting the same non-critical error should not modify the error list
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
}

TEST_F(
    SharedErrorTableTest,
    get_all_non_critical_errors_using_one_non_critical_and_one_critical_error)
{
    // Set a non-critical error and a critical error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));

    // Make sure we only retrieve one non-critical error, even though there are
    // two errors that are set
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
}

TEST_F(
    SharedErrorTableTest,
    get_all_non_critical_errors_using_one_non_critical_per_board_and_one_critical_error)
{
    // Set a non-critical error per board plus an additional critical error
    ResetWithOneNonCriticalErrorForEveryBoard();
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));

    // Make sure we only retrieve as many non-critical errors as there are
    // boards, even though there are (boards + 1) errors that are set
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(NUM_BOARDS, error_list.num_errors);
}

TEST_F(SharedErrorTableTest, get_boards_with_no_errors_using_no_errors)
{
    // No board has any error
    App_SharedErrorTable_GetBoardsWithNoErrors(error_table, &board_list);

    // Make sure we retrieve every borad
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
}

TEST_F(SharedErrorTableTest, get_boards_with_no_errors_using_one_critical_error)
{
    // One board has a critical error
    ResetWithOneCriticalErrorForOneBoard();

    // Make sure we retrieve (num_boards - 1) boards
    App_SharedErrorTable_GetBoardsWithNoErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS - 1, board_list.num_boards);
    for (auto board : GetAllBoards())
    {
        if (board != DEFAULT_CRITICAL_ERROR_BOARD)
        {
            ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
        }
    }
}

TEST_F(
    SharedErrorTableTest,
    get_boards_with_no_errors_using_one_critical_error_per_board)
{
    // Every board has a critical error
    ResetWithOneCriticalErrorForEveryBoard();
    App_SharedErrorTable_GetBoardsWithNoErrors(error_table, &board_list);
    ASSERT_EQ(0, board_list.num_boards);
}

TEST_F(
    SharedErrorTableTest,
    get_boards_with_no_errors_using_one_non_critical_error)
{
    // One board has a non-critical error
    ResetWithOneNonCriticalErrorForOneBoard();

    // Make sure we retrieve (num_boards - 1) boards
    App_SharedErrorTable_GetBoardsWithNoErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS - 1, board_list.num_boards);
    for (auto board : GetAllBoards())
    {
        if (board != DEFAULT_NON_CRITICAL_ERROR_BOARD)
        {
            ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
        }
    }
}
TEST_F(
    SharedErrorTableTest,
    get_boards_with_no_errors_using_one_non_critical_error_per_board)
{
    // Every board has a non-critical error
    ResetWithOneNonCriticalErrorForEveryBoard();

    // Make sure we retrieve no boards
    App_SharedErrorTable_GetBoardsWithNoErrors(error_table, &board_list);
    ASSERT_EQ(0, board_list.num_boards);
}

TEST_F(SharedErrorTableTest, get_boards_with_errors_using_no_errors)
{
    // No board has any error
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);

    // Make sure we retrieve no boards
    ASSERT_EQ(0, board_list.num_boards);
}

TEST_F(SharedErrorTableTest, get_boards_with_errors_using_one_critical_error)
{
    // One board has a critical error
    ResetWithOneCriticalErrorForOneBoard();

    // Make sure we retrieve only the one correct board
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(
        &board_list, DEFAULT_CRITICAL_ERROR_BOARD));
}

TEST_F(
    SharedErrorTableTest,
    get_boards_with_errors_using_one_critical_error_per_board)
{
    // Every board has a critical error
    ResetWithOneCriticalErrorForEveryBoard();

    // Make sure we retrieve every board
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
}

TEST_F(
    SharedErrorTableTest,
    get_boards_with_errors_using_one_non_critical_error)
{
    // One board has anon-critical error
    ResetWithOneNonCriticalErrorForOneBoard();

    // Make sure we retrieve only the one correct board
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(
        &board_list, DEFAULT_NON_CRITICAL_ERROR_BOARD));
}

TEST_F(
    SharedErrorTableTest,
    get_boards_with_errors_using_one_non_critical_error_per_board)
{
    // Every board has anon-critical error
    ResetWithOneNonCriticalErrorForEveryBoard();

    // Make sure we retrieve every board
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
    {
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
    }
}

TEST_F(SharedErrorTableTest, get_boards_with_critical_errors_using_no_errors)
{
    // No board has any critical error
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);

    // Make sure we retrieve no boards
    ASSERT_EQ(0, board_list.num_boards);
}

TEST_F(
    SharedErrorTableTest,
    get_boards_with_critical_errors_using_one_critical_error)
{
    // One board has a critical error
    ResetWithOneCriticalErrorForOneBoard();

    // Make sure we retrieve only the one correct board
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(
        &board_list, DEFAULT_CRITICAL_ERROR_BOARD));
}

TEST_F(
    SharedErrorTableTest,
    get_boards_with_critical_errors_using_one_critical_error_per_board)
{
    // Every board has a critical error
    ResetWithOneCriticalErrorForEveryBoard();

    // Make sure we retrieve every board
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
    {
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
    }
}

TEST_F(
    SharedErrorTableTest,
    get_boards_with_non_critical_errors_using_no_errors)
{
    // No board has any non-critical error
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, &board_list);

    // Make sure we retrieve no boards
    ASSERT_EQ(0, board_list.num_boards);
}

TEST_F(
    SharedErrorTableTest,
    get_boards_with_non_critical_errors_using_one_critical_error)
{
    // One board has a non-critical error
    ResetWithOneNonCriticalErrorForOneBoard();

    // Make sure we retrieve only the one correct board
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(
        &board_list, DEFAULT_NON_CRITICAL_ERROR_BOARD));
}

TEST_F(
    SharedErrorTableTest,
    get_boards_with_non_critical_errors_using_one_critical_error_per_board)
{
    // Every board has a non-critical error
    ResetWithOneNonCriticalErrorForEveryBoard();

    // Make sure we retrieve every board
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
    {
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
    }
}
