#include <memory>
#include "Test_Shared.h"

extern "C"
{
#include "Io_SharedErrorTable.h"
#include "App_SharedErrorTable.h"
#include "App_SharedMacros.h"
#include "App_CanMsgs.h"
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
    const enum ErrorId DEFAULT_CRITICAL_ERROR =
        BMS_CRITICAL_CHARGER_DISCONNECTED_IN_CHARGE_STATE;
    const enum ErrorId DEFAULT_NON_CRITICAL_ERROR =
        BMS_NON_CRITICAL_WATCHDOG_TIMEOUT;
    const enum Board DEFAULT_CRITICAL_ERROR_BOARD     = BMS;
    const enum Board DEFAULT_NON_CRITICAL_ERROR_BOARD = BMS;

    const enum ErrorId DEFAULT_BMS_CRITICAL_ERROR =
        BMS_CRITICAL_CHARGER_DISCONNECTED_IN_CHARGE_STATE;
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
    struct CanMsg         can_msg;
    bool                  is_set;
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
    board_list.boards[0]  = BMS;
    board_list.boards[1]  = BMS;
    board_list.boards[2]  = DIM;
    board_list.boards[3]  = FSM;
    board_list.boards[4]  = PDM;
    board_list.num_boards = 5;
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

TEST_F(SharedErrorTableTest, process_bms_non_critical_errors)
{
    // A list of BMS non-critical error IDs. This must be maintained manually as
    // errors are added to/removed from the DBC!
    std::vector<enum ErrorId> bms_non_critical_error_ids = {
        BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        BMS_NON_CRITICAL_WATCHDOG_TIMEOUT
    };

    // Each struct member that has a non-zero value indicates an error that is
    // set. For this test, we want every error to be set. Instead of setting
    // each struct member manually, we're going to "cheat" using memset().
    struct CanMsgs_bms_non_critical_errors_t data;
    memset(&data, 1, sizeof(struct CanMsgs_bms_non_critical_errors_t));

    // Prepare the CAN message containing the errors
    can_msg.std_id = CANMSGS_BMS_NON_CRITICAL_ERRORS_FRAME_ID;
    can_msg.dlc    = CANMSGS_BMS_NON_CRITICAL_ERRORS_LENGTH;
    App_CanMsgs_bms_non_critical_errors_pack(
        can_msg.data, &data, CANMSGS_BMS_NON_CRITICAL_ERRORS_LENGTH);

    // Update the error table using the given CAN message
    Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &can_msg);

    // Check that we can retrieve the correct board from the error table
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, BMS));

    // Check that we can retrieve the correct errors from the error table
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(bms_non_critical_error_ids.size(), error_list.num_errors);
    for (auto &error_id : bms_non_critical_error_ids)
    {
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_IsErrorSet(error_table, error_id, &is_set));
        ASSERT_TRUE(is_set);
    }
}

TEST_F(SharedErrorTableTest, process_bms_critical_errors)
{
    // A list of BMS critical error IDs. This must be maintained manually as
    // errors are added to/removed from the DBC!
    std::vector<enum ErrorId> bms_critical_error_ids = {
        BMS_CRITICAL_CHARGER_DISCONNECTED_IN_CHARGE_STATE,
    };

    // Each struct member that has a non-zero value indicates an error that is
    // set. For this test, we want every error to be set. Instead of setting
    // each struct member manually, we're going to "cheat" using memset().
    struct CanMsgs_bms_critical_errors_t data;
    memset(&data, 1, sizeof(struct CanMsgs_bms_critical_errors_t));

    // Prepare the CAN message containing the errors
    can_msg.std_id = CANMSGS_BMS_CRITICAL_ERRORS_FRAME_ID;
    can_msg.dlc    = CANMSGS_BMS_CRITICAL_ERRORS_LENGTH;
    App_CanMsgs_bms_critical_errors_pack(
        can_msg.data, &data, CANMSGS_BMS_CRITICAL_ERRORS_LENGTH);

    // Update the error table using the given CAN message
    Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &can_msg);

    // Check that we can retrieve the correct board from the error table
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, BMS));

    // Check that we can retrieve the correct errors from the error table
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(bms_critical_error_ids.size(), error_list.num_errors);
    for (auto &error_id : bms_critical_error_ids)
    {
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_IsErrorSet(error_table, error_id, &is_set));
        ASSERT_TRUE(is_set);
    }
}

TEST_F(SharedErrorTableTest, process_dcm_non_critical_errors)
{
    // A list of DCM non-critical error IDs. This must be maintained manually as
    // errors are added to/removed from the DBC!
    std::vector<enum ErrorId> dcm_non_critical_error_ids = {
        DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        DCM_NON_CRITICAL_WATCHDOG_TIMEOUT
    };

    // Each struct member that has a non-zero value indicates an error that is
    // set. For this test, we want every error to be set. Instead of setting
    // each struct member manually, we're going to "cheat" using memset().
    struct CanMsgs_dcm_non_critical_errors_t data;
    memset(&data, 1, sizeof(struct CanMsgs_dcm_non_critical_errors_t));

    // Prepare the CAN message containing the errors
    can_msg.std_id = CANMSGS_DCM_NON_CRITICAL_ERRORS_FRAME_ID;
    can_msg.dlc    = CANMSGS_DCM_NON_CRITICAL_ERRORS_LENGTH;
    App_CanMsgs_dcm_non_critical_errors_pack(
        can_msg.data, &data, CANMSGS_DCM_NON_CRITICAL_ERRORS_LENGTH);

    // Update the error table using the given CAN message
    Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &can_msg);

    // Check that we can retrieve the correct board from the error table
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, DCM));

    // Check that we can retrieve the correct errors from the error table
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(dcm_non_critical_error_ids.size(), error_list.num_errors);
    for (auto &error_id : dcm_non_critical_error_ids)
    {
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_IsErrorSet(error_table, error_id, &is_set));
        ASSERT_TRUE(is_set);
    }
}

TEST_F(SharedErrorTableTest, process_dcm_critical_errors)
{
    // A list of DCM critical error IDs. This must be maintained manually as
    // errors are added to/removed from the DBC!
    std::vector<enum ErrorId> dcm_critical_error_ids = {
        DCM_CRITICAL_DUMMY,
    };

    // Each struct member that has a non-zero value indicates an error that is
    // set. For this test, we want every error to be set. Instead of setting
    // each struct member manually, we're going to "cheat" using memset().
    struct CanMsgs_dcm_critical_errors_t data;
    memset(&data, 1, sizeof(struct CanMsgs_dcm_critical_errors_t));

    // Prepare the CAN message containing the errors
    can_msg.std_id = CANMSGS_DCM_CRITICAL_ERRORS_FRAME_ID;
    can_msg.dlc    = CANMSGS_DCM_CRITICAL_ERRORS_LENGTH;
    App_CanMsgs_dcm_critical_errors_pack(
        can_msg.data, &data, CANMSGS_DCM_CRITICAL_ERRORS_LENGTH);

    // Update the error table using the given CAN message
    Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &can_msg);

    // Check that we can retrieve the correct board from the error table
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, DCM));

    // Check that we can retrieve the correct errors from the error table
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(dcm_critical_error_ids.size(), error_list.num_errors);
    for (auto &error_id : dcm_critical_error_ids)
    {
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_IsErrorSet(error_table, error_id, &is_set));
        ASSERT_TRUE(is_set);
    }
}

TEST_F(SharedErrorTableTest, process_dim_non_critical_errors)
{
    // A list of DIM non-critical error IDs. This must be maintained manually as
    // errors are added to/removed from the DBC!
    std::vector<enum ErrorId> dim_non_critical_error_ids = {
        DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        DIM_NON_CRITICAL_WATCHDOG_TIMEOUT,
    };

    // Each struct member that has a non-zero value indicates an error that is
    // set. For this test, we want every error to be set. Instead of setting
    // each struct member manually, we're going to "cheat" using memset().
    struct CanMsgs_dim_non_critical_errors_t data;
    memset(&data, 1, sizeof(struct CanMsgs_dim_non_critical_errors_t));

    // Prepare the CAN message containing the errors
    can_msg.std_id = CANMSGS_DIM_NON_CRITICAL_ERRORS_FRAME_ID;
    can_msg.dlc    = CANMSGS_DIM_NON_CRITICAL_ERRORS_LENGTH;
    App_CanMsgs_dim_non_critical_errors_pack(
        can_msg.data, &data, CANMSGS_DIM_NON_CRITICAL_ERRORS_LENGTH);

    // Update the error table using the given CAN message
    Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &can_msg);

    // Check that we can retrieve the correct board from the error table
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, DIM));

    // Check that we can retrieve the correct errors from the error table
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(dim_non_critical_error_ids.size(), error_list.num_errors);
    for (auto &error_id : dim_non_critical_error_ids)
    {
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_IsErrorSet(error_table, error_id, &is_set));
        ASSERT_TRUE(is_set);
    }
}

TEST_F(SharedErrorTableTest, process_dim_critical_errors)
{
    // A list of DIM critical error IDs. This must be maintained manually as
    // errors are added to/removed from the DBC!
    std::vector<enum ErrorId> dim_critical_error_ids = {
        DIM_CRITICAL_DUMMY,
    };

    // Each struct member that has a non-zero value indicates an error that is
    // set. For this test, we want every error to be set. Instead of setting
    // each struct member manually, we're going to "cheat" using memset().
    struct CanMsgs_dim_critical_errors_t data;
    memset(&data, 1, sizeof(struct CanMsgs_dim_critical_errors_t));

    // Prepare the CAN message containing the errors
    can_msg.std_id = CANMSGS_DIM_CRITICAL_ERRORS_FRAME_ID;
    can_msg.dlc    = CANMSGS_DIM_CRITICAL_ERRORS_LENGTH;
    App_CanMsgs_dim_critical_errors_pack(
        can_msg.data, &data, CANMSGS_DIM_CRITICAL_ERRORS_LENGTH);

    // Update the error table using the given CAN message
    Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &can_msg);

    // Check that we can retrieve the correct board from the error table
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, DIM));

    // Check that we can retrieve the correct errors from the error table
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(dim_critical_error_ids.size(), error_list.num_errors);
    for (auto &error_id : dim_critical_error_ids)
    {
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_IsErrorSet(error_table, error_id, &is_set));
        ASSERT_TRUE(is_set);
    }
}

TEST_F(SharedErrorTableTest, process_fsm_non_critical_errors)
{
    // A list of FSM non-critical error IDs. This must be maintained manually as
    // errors are added to/removed from the DBC!
    std::vector<enum ErrorId> fsm_non_critical_error_ids = {
        FSM_NON_CRITICAL_PAPPS_OUT_OF_RANGE,
        FSM_NON_CRITICAL_SAPPS_OUT_OF_RANGE,
        FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        FSM_NON_CRITICAL_WATCHDOG_TIMEOUT,
        FSM_NON_CRITICAL_BSPD_FAULT,
        FSM_NON_CRITICAL_LEFT_WHEEL_SPEED_OUT_OF_RANGE,
        FSM_NON_CRITICAL_RIGHT_WHEEL_SPEED_OUT_OF_RANGE,
        FSM_NON_CRITICAL_PRIMARY_FLOW_RATE_OUT_OF_RANGE,
        FSM_NON_CRITICAL_SECONDARY_FLOW_RATE_OUT_OF_RANGE,
    };

    // Each struct member that has a non-zero value indicates an error that is
    // set. For this test, we want every error to be set. Instead of setting
    // each struct member manually, we're going to "cheat" using memset().
    struct CanMsgs_fsm_non_critical_errors_t data;
    memset(&data, 1, sizeof(struct CanMsgs_fsm_non_critical_errors_t));

    // Prepare the CAN message containing the errors
    can_msg.std_id = CANMSGS_FSM_NON_CRITICAL_ERRORS_FRAME_ID;
    can_msg.dlc    = CANMSGS_FSM_NON_CRITICAL_ERRORS_LENGTH;
    App_CanMsgs_fsm_non_critical_errors_pack(
        can_msg.data, &data, CANMSGS_FSM_NON_CRITICAL_ERRORS_LENGTH);

    // Update the error table using the given CAN message
    Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &can_msg);

    // Check that we can retrieve the correct board from the error table
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, FSM));

    // Check that we can retrieve the correct errors from the error table
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(fsm_non_critical_error_ids.size(), error_list.num_errors);
    for (auto &error_id : fsm_non_critical_error_ids)
    {
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_IsErrorSet(error_table, error_id, &is_set));
        ASSERT_TRUE(is_set);
    }
}

TEST_F(SharedErrorTableTest, process_fsm_critical_errors)
{
    // A list of FSM critical error IDs. This must be maintained manually as
    // errors are added to/removed from the DBC!
    std::vector<enum ErrorId> fsm_critical_error_ids = {
        FSM_CRITICAL_DUMMY,
    };

    // Each struct member that has a non-zero value indicates an error that is
    // set. For this test, we want every error to be set. Instead of setting
    // each struct member manually, we're going to "cheat" using memset().
    struct CanMsgs_fsm_critical_errors_t data;
    memset(&data, 1, sizeof(struct CanMsgs_fsm_critical_errors_t));

    // Prepare the CAN message containing the errors
    can_msg.std_id = CANMSGS_FSM_CRITICAL_ERRORS_FRAME_ID;
    can_msg.std_id = CANMSGS_FSM_CRITICAL_ERRORS_LENGTH;
    App_CanMsgs_fsm_critical_errors_pack(
        can_msg.data, &data, CANMSGS_FSM_CRITICAL_ERRORS_LENGTH);

    // Update the error table using the given CAN message
    Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &can_msg);

    // Check that we can retrieve the correct board from the error table
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, FSM));

    // Check that we can retrieve the correct errors from the error table
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(fsm_critical_error_ids.size(), error_list.num_errors);
    for (auto &error_id : fsm_critical_error_ids)
    {
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_IsErrorSet(error_table, error_id, &is_set));
        ASSERT_TRUE(is_set);
    }
}

TEST_F(SharedErrorTableTest, process_pdm_non_critical_errors)
{
    // A list of PDM non-critical error IDs. This must be maintained manually as
    // errors are added to/removed from the DBC!
    std::vector<enum ErrorId> pdm_non_critical_error_ids = {
        PDM_NON_CRITICAL_MISSING_HEARTBEAT,
        PDM_NON_CRITICAL_BOOST_PGOOD_FAULT,
        PDM_NON_CRITICAL_CELL_BALANCE_OVERVOLTAGE_FAULT,
        PDM_NON_CRITICAL_CHARGER_FAULT,
        PDM_NON_CRITICAL_EFUSE_FAULT,
        PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
        PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
        PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
        PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
        PDM_NON_CRITICAL_WATCHDOG_TIMEOUT,
        PDM_NON_CRITICAL_VBAT_VOLTAGE_OUT_OF_RANGE,
        PDM_NON_CRITICAL__24V_AUX_VOLTAGE_OUT_OF_RANGE,
        PDM_NON_CRITICAL__24V_ACC_VOLTAGE_OUT_OF_RANGE,
        PDM_NON_CRITICAL_AUX1_CURRENT_OUT_OF_RANGE,
        PDM_NON_CRITICAL_AUX2_CURRENT_OUT_OF_RANGE,
        PDM_NON_CRITICAL_LEFT_INVERTER_CURRENT_OUT_OF_RANGE,
        PDM_NON_CRITICAL_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE,
        PDM_NON_CRITICAL_ENERGY_METER_CURRENT_OUT_OF_RANGE,
        PDM_NON_CRITICAL_CAN_CURRENT_OUT_OF_RANGE,
        PDM_NON_CRITICAL_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE,
    };

    // Each struct member that has a non-zero value indicates an error that is
    // set. For this test, we want every error to be set. Instead of setting
    // each struct member manually, we're going to "cheat" using memset().
    struct CanMsgs_pdm_non_critical_errors_t data;
    memset(&data, 1, sizeof(struct CanMsgs_pdm_non_critical_errors_t));

    // Prepare the CAN message containing the errors
    can_msg.std_id = CANMSGS_PDM_NON_CRITICAL_ERRORS_FRAME_ID;
    can_msg.dlc    = CANMSGS_PDM_NON_CRITICAL_ERRORS_LENGTH;
    App_CanMsgs_pdm_non_critical_errors_pack(
        can_msg.data, &data, CANMSGS_PDM_NON_CRITICAL_ERRORS_LENGTH);

    // Update the error table using the given CAN message
    Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &can_msg);

    // Check that we can retrieve the correct board from the error table
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, PDM));

    // Check that we can retrieve the correct errors from the error table
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(pdm_non_critical_error_ids.size(), error_list.num_errors);
    for (auto &error_id : pdm_non_critical_error_ids)
    {
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_IsErrorSet(error_table, error_id, &is_set));
        ASSERT_TRUE(is_set);
    }
}

TEST_F(SharedErrorTableTest, process_pdm_critical_errors)
{
    // A list of PDM critical error IDs. This must be maintained manually as
    // errors are added to/removed from the DBC!
    std::vector<enum ErrorId> pdm_critical_error_ids = {
        PDM_CRITICAL_DUMMY,
    };

    // Each struct member that has a non-zero value indicates an error that is
    // set. For this test, we want every error to be set. Instead of setting
    // each struct member manually, we're going to "cheat" using memset().
    struct CanMsgs_pdm_critical_errors_t data;
    memset(&data, 1, sizeof(struct CanMsgs_pdm_critical_errors_t));

    // Prepare the CAN message containing the errors
    can_msg.std_id = CANMSGS_PDM_CRITICAL_ERRORS_FRAME_ID;
    can_msg.dlc    = CANMSGS_PDM_CRITICAL_ERRORS_LENGTH;
    App_CanMsgs_pdm_critical_errors_pack(
        can_msg.data, &data, CANMSGS_PDM_CRITICAL_ERRORS_LENGTH);

    // Update the error table using the given CAN message
    Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &can_msg);

    // Check that we can retrieve the correct board from the error table
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, PDM));

    // Check that we can retrieve the correct errors from the error table
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(pdm_critical_error_ids.size(), error_list.num_errors);
    for (auto &error_id : pdm_critical_error_ids)
    {
        ASSERT_EQ(
            EXIT_CODE_OK,
            App_SharedErrorTable_IsErrorSet(error_table, error_id, &is_set));
        ASSERT_TRUE(is_set);
    }
}
