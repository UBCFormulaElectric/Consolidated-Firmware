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
    const enum ErrorId    DEFAULT_CRITICAL_ERROR           = BMS_CRIT;
    const enum ErrorId    DEFAULT_NON_CRITICAL_ERROR       = BMS_NON_CRIT;
    const enum Board DEFAULT_CRITICAL_ERROR_BOARD     = BMS;
    const enum Board DEFAULT_NON_CRITICAL_ERROR_BOARD = BMS;

    const enum ErrorId DEFAULT_BMS_CRITICAL_ERROR = BMS_CRIT;
    const enum ErrorId DEFAULT_DCM_CRITICAL_ERROR = DCM_CRIT;
    const enum ErrorId DEFAULT_DIM_CRITICAL_ERROR = DIM_CRIT;
    const enum ErrorId DEFAULT_FSM_CRITICAL_ERROR = FSM_CRIT;
    const enum ErrorId DEFAULT_PDM_CRITICAL_ERROR = PDM_CRIT;

    const enum ErrorId DEFAULT_BMS_NON_CRITICAL_ERROR = BMS_NON_CRIT;
    const enum ErrorId DEFAULT_DCM_NON_CRITICAL_ERROR = DCM_NON_CRIT;
    const enum ErrorId DEFAULT_DIM_NON_CRITICAL_ERROR = DIM_NON_CRIT;
    const enum ErrorId DEFAULT_FSM_NON_CRITICAL_ERROR = FSM_NON_CRIT;
    const enum ErrorId DEFAULT_PDM_NON_CRITICAL_ERROR = PDM_NON_CRIT;

    struct ErrorTable *error_table;
    struct ErrorList   error_list;
};

TEST_F(SharedErrorTableTest, is_board_in_list)
{
    struct ErrorBoardList board_list;

    // The board exists in the list
    board_list.boards[0]  = BMS;
    board_list.boards[1]  = DCM;
    board_list.boards[2]  = DIM;
    board_list.boards[3]  = FSM;
    board_list.boards[4]  = PDM;
    board_list.num_boards = 5;
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, BMS));

    // The board does not exist in the list
    board_list.boards[0]  = BMS;
    board_list.boards[1]  = BMS;
    board_list.boards[2]  = BMS;
    board_list.boards[3]  = BMS;
    board_list.boards[4]  = BMS;
    board_list.num_boards = 5;
    ASSERT_FALSE(App_SharedError_IsBoardInList(&board_list, DCM));

    // The board we are looking for has multiple entries in the list
    board_list.boards[0] = BMS;
    board_list.boards[1] = BMS;
    board_list.boards[2] = DIM;
    board_list.boards[3] = FSM;
    board_list.boards[4] = PDM;
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, BMS));

    // The board exists in the list but is out-of-bound
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
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_EQ(
        DEFAULT_CRITICAL_ERROR_BOARD,
        App_SharedError_GetBoard(error_list.errors[0]));
}

TEST_F(SharedErrorTableTest, is_error_critical)
{
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_TRUE(App_SharedError_GetIsCritical(error_list.errors[0]));
}

TEST_F(SharedErrorTableTest, set_error)
{
    // Set error
    bool is_set = false;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_SetError(error_table, BMS_NON_CRIT, true));
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_IsErrorSet(error_table, BMS_NON_CRIT, &is_set));
    ASSERT_TRUE(is_set);

    // Clear error
    is_set = true;
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_SetError(error_table, BMS_NON_CRIT, false));
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_IsErrorSet(error_table, BMS_NON_CRIT, &is_set));
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
        App_SharedErrorTable_SetError(error_table, BMS_NON_CRIT, true));
    ASSERT_EQ(
        EXIT_CODE_OK,
        App_SharedErrorTable_IsErrorSet(error_table, BMS_NON_CRIT, &is_set));
    ASSERT_TRUE(is_set);

    // Out-of-range ID
    ASSERT_EQ(
        EXIT_CODE_OUT_OF_RANGE,
        App_SharedErrorTable_IsErrorSet(error_table, NUM_ERROR_IDS, &is_set));
}

TEST_F(SharedErrorTableTest, has_error)
{
    // Set a critical error
    ASSERT_FALSE(App_SharedErrorTable_HasError(error_table));
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasError(error_table));

    // Clear a critical error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasError(error_table));

    // Set a non-critical error
    ASSERT_FALSE(App_SharedErrorTable_HasError(error_table));
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasError(error_table));

    // Clear a non-critical error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasError(error_table));
}

TEST_F(SharedErrorTableTest, has_critical_error)
{
    // Set a critical error
    ASSERT_FALSE(App_SharedErrorTable_HasError(error_table));
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasCriticalError(error_table));

    // Clear a critical error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasCriticalError(error_table));
}

TEST_F(SharedErrorTableTest, has_non_critical_error)
{
    // Set a non-critical error
    ASSERT_FALSE(App_SharedErrorTable_HasNonCriticalError(error_table));
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasNonCriticalError(error_table));

    // Clear a non-critical error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasNonCriticalError(error_table));
}

TEST_F(SharedErrorTableTest, get_all_errors)
{
    // No error
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(0, error_list.num_errors);

    // One critical error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // Setting the same critical error should not modify the error list
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // One critical error + one non-critical_error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(2, error_list.num_errors);

    // Every error
    for (size_t id = 0; id < NUM_ERROR_IDS; id++)
    {
        App_SharedErrorTable_SetError(error_table, (enum ErrorId)id, true);
    }
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(NUM_ERROR_IDS, error_list.num_errors);
    for (size_t id = 0; id < NUM_ERROR_IDS; id++)
    {
        ASSERT_TRUE(
            App_SharedError_IsErrorInList(&error_list, (enum ErrorId)id));
    }
}

TEST_F(SharedErrorTableTest, get_all_critical_errors)
{
    // No error
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(0, error_list.num_errors);

    // One critical error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // Setting the same critical error should not modify the error list
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // One critical error + one non-critical_error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // One critical error per board + one non-critical error
    ResetWithOneCriticalErrorForEveryBoard();
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(NUM_BOARDS, error_list.num_errors);
}

TEST_F(SharedErrorTableTest, get_all_non_critical_errors)
{
    // No error
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(0, error_list.num_errors);

    // One non-critical error
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

    // One non-critical error + one critical_error
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // One non-critical error per board + one critical error
    ResetWithOneNonCriticalErrorForEveryBoard();
    ASSERT_EQ(
        EXIT_CODE_OK, App_SharedErrorTable_SetError(
                          error_table, DEFAULT_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(NUM_BOARDS, error_list.num_errors);
}

TEST_F(SharedErrorTableTest, get_boards_with_no_errors)
{
    struct ErrorBoardList board_list;

    // No board has any error
    App_SharedErrorTable_GetBoardsWithNoErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));

    // One board has one critical error
    ResetWithOneCriticalErrorForOneBoard();
    App_SharedErrorTable_GetBoardsWithNoErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS - 1, board_list.num_boards);
    for (auto board : GetAllBoards())
    {
        if (board != DEFAULT_CRITICAL_ERROR_BOARD)
        {
            ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
        }
    }

    // Every board has one critical error
    ResetWithOneCriticalErrorForEveryBoard();
    App_SharedErrorTable_GetBoardsWithNoErrors(error_table, &board_list);
    ASSERT_EQ(0, board_list.num_boards);

    // One board has one non-critical error
    ResetWithOneNonCriticalErrorForOneBoard();
    App_SharedErrorTable_GetBoardsWithNoErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS - 1, board_list.num_boards);
    for (auto board : GetAllBoards())
    {
        if (board != DEFAULT_NON_CRITICAL_ERROR_BOARD)
        {
            ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
        }
    }

    // Every board has one non-critical error
    ResetWithOneNonCriticalErrorForEveryBoard();
    App_SharedErrorTable_GetBoardsWithNoErrors(error_table, &board_list);
    ASSERT_EQ(0, board_list.num_boards);
}

TEST_F(SharedErrorTableTest, get_boards_with_errors)
{
    struct ErrorBoardList board_list;

    // No board has any error
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);
    ASSERT_EQ(0, board_list.num_boards);

    // One board has one critical error
    ResetWithOneCriticalErrorForOneBoard();
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(
        &board_list, DEFAULT_CRITICAL_ERROR_BOARD));

    // Every board has one critical error
    ResetWithOneCriticalErrorForEveryBoard();
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));

    // One board has one non-critical error
    ResetWithOneNonCriticalErrorForOneBoard();
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(
        &board_list, DEFAULT_NON_CRITICAL_ERROR_BOARD));

    // Every board has one non-critical error
    ResetWithOneNonCriticalErrorForEveryBoard();
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
    {
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
    }
}

TEST_F(SharedErrorTableTest, get_boards_with_critical_errors)
{
    struct ErrorBoardList board_list;

    // No board has any critical error
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);
    ASSERT_EQ(0, board_list.num_boards);

    // One board has one critical error
    ResetWithOneCriticalErrorForOneBoard();
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(
        &board_list, DEFAULT_CRITICAL_ERROR_BOARD));

    // Every board has one critical error
    ResetWithOneCriticalErrorForEveryBoard();
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
    {
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
    }
}

TEST_F(SharedErrorTableTest, get_boards_with_non_critical_errors)
{
    struct ErrorBoardList board_list;

    // No board has any non-critical error
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, &board_list);
    ASSERT_EQ(0, board_list.num_boards);

    // One board has one non-critical error
    ResetWithOneNonCriticalErrorForOneBoard();
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(
        &board_list, DEFAULT_NON_CRITICAL_ERROR_BOARD));

    // Every board has one non-critical error
    ResetWithOneNonCriticalErrorForEveryBoard();
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
        error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
    {
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
    }
}
