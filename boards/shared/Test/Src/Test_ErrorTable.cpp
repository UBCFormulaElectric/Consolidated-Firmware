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

    void TearDown() override { TearDownObject(error_table, App_SharedErrorTable_Destroy); }

    void ResetWithOneCriticalErrorForOneBoard(void)
    {
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
        error_table = App_SharedErrorTable_Create();

        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));
    }

    void ResetWithOneCriticalErrorForEveryBoard(void)
    {
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
        error_table = App_SharedErrorTable_Create();

        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_BMS_CRITICAL_ERROR, true));
        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_DCM_CRITICAL_ERROR, true));
        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_DIM_CRITICAL_ERROR, true));
        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_FSM_CRITICAL_ERROR, true));
        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_PDM_CRITICAL_ERROR, true));
    }

    void ResetWithOneNonCriticalErrorForOneBoard(void)
    {
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
        error_table = App_SharedErrorTable_Create();

        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    }

    void ResetWithOneNonCriticalErrorForEveryBoard(void)
    {
        TearDownObject(error_table, App_SharedErrorTable_Destroy);
        error_table = App_SharedErrorTable_Create();

        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_BMS_NON_CRITICAL_ERROR, true));
        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_DCM_NON_CRITICAL_ERROR, true));
        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_DIM_NON_CRITICAL_ERROR, true));
        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_FSM_NON_CRITICAL_ERROR, true));
        ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_PDM_NON_CRITICAL_ERROR, true));
    }

    template <typename T>
    void TestRoutineForSetErrorsFromCanMsg(
        enum Board                board,
        std::vector<enum ErrorId> error_ids,
        uint32_t                  can_stdid,
        uint32_t                  can_dlc,
        int (*pack_can_msg)(uint8_t *dst_p, const T *src_p, size_t size),
        void (*get_boards_from_error_table)(const struct ErrorTable *, struct ErrorBoardList *),
        void (*get_errors_from_error_table)(struct ErrorTable *, struct ErrorList *))
    {
        // Each struct member that has a non-zero value indicates an error that
        // is set. For this test, we want every error to be set. Instead of
        // setting each struct member manually, we're going to "cheat" using
        // memset().
        T can_data;
        memset(&can_data, 1, sizeof(T));

        // Prepare the CAN message containing the errors
        can_msg.std_id = can_stdid;
        can_msg.dlc    = can_dlc;
        pack_can_msg(can_msg.data, &can_data, can_dlc);

        // Update the error table using the given CAN message
        Io_SharedErrorTable_SetErrorsFromCanMsg(error_table, &can_msg);

        // Check that we can retrieve the correct board from the error table
        get_boards_from_error_table(error_table, &board_list);
        ASSERT_EQ(1, board_list.num_boards);
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));

        // Check that we can retrieve the correct errors from the error table
        get_errors_from_error_table(error_table, &error_list);
        ASSERT_EQ(error_ids.size(), error_list.num_errors);
        for (auto &error_id : error_ids)
        {
            ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_IsErrorSet(error_table, error_id, &is_set));
            ASSERT_TRUE(is_set);
        }
    }

    std::vector<enum Board> GetAllBoards(void)
    {
        return std::vector<enum Board>{
            BMS, DCM, DIM, FSM, PDM,
        };
    }

    // The error choices here are arbitrary. We just need to pick something for
    // the helper functions.
    const enum ErrorId DEFAULT_CRITICAL_ERROR           = BMS_AIR_SHUTDOWN_CHARGER_DISCONNECTED_IN_CHARGE_STATE;
    const enum ErrorId DEFAULT_AIR_SHUTDOWN_ERROR       = BMS_AIR_SHUTDOWN_CHARGER_DISCONNECTED_IN_CHARGE_STATE;
    const enum ErrorId DEFAULT_MOTOR_SHUTDOWN_ERROR     = BMS_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN;
    const enum ErrorId DEFAULT_NON_CRITICAL_ERROR       = BMS_NON_CRITICAL_WATCHDOG_TIMEOUT;
    const enum Board   DEFAULT_CRITICAL_ERROR_BOARD     = BMS;
    const enum Board   DEFAULT_NON_CRITICAL_ERROR_BOARD = BMS;

    const enum ErrorId DEFAULT_BMS_CRITICAL_ERROR = BMS_AIR_SHUTDOWN_CHARGER_DISCONNECTED_IN_CHARGE_STATE;
    const enum ErrorId DEFAULT_DCM_CRITICAL_ERROR = DCM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN;
    const enum ErrorId DEFAULT_DIM_CRITICAL_ERROR = DIM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN;
    const enum ErrorId DEFAULT_FSM_CRITICAL_ERROR = FSM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN;
    const enum ErrorId DEFAULT_PDM_CRITICAL_ERROR = PDM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN;

    const enum ErrorId DEFAULT_BMS_NON_CRITICAL_ERROR = BMS_NON_CRITICAL_WATCHDOG_TIMEOUT;
    const enum ErrorId DEFAULT_DCM_NON_CRITICAL_ERROR = DCM_NON_CRITICAL_WATCHDOG_TIMEOUT;
    const enum ErrorId DEFAULT_DIM_NON_CRITICAL_ERROR = DIM_NON_CRITICAL_WATCHDOG_TIMEOUT;
    const enum ErrorId DEFAULT_FSM_NON_CRITICAL_ERROR = FSM_NON_CRITICAL_WATCHDOG_TIMEOUT;
    const enum ErrorId DEFAULT_PDM_NON_CRITICAL_ERROR = PDM_NON_CRITICAL_WATCHDOG_TIMEOUT;

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
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));

    // Get the error from the error table
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // Make sure the error belongs to the correct board
    ASSERT_EQ(DEFAULT_CRITICAL_ERROR_BOARD, App_SharedError_GetBoard(error_list.errors[0]));
}

TEST_F(SharedErrorTableTest, is_error_critical)
{
    // Set a critical error in the error table
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));

    // Get errors from the error table
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);

    // Make sure there is only one error and it's a critical one
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_TRUE(App_SharedError_IsCritical(error_list.errors[0]));
}

TEST_F(SharedErrorTableTest, set_error)
{
    // Set an error
    bool is_set = false;
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, BMS_NON_CRITICAL_WATCHDOG_TIMEOUT, true));

    // Make sure the error is indeed set
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_IsErrorSet(error_table, BMS_NON_CRITICAL_WATCHDOG_TIMEOUT, &is_set));
    ASSERT_TRUE(is_set);
}

TEST_F(SharedErrorTableTest, clear_error)
{
    // Clear an error
    bool is_set = true;
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, BMS_NON_CRITICAL_WATCHDOG_TIMEOUT, false));

    // Make sure the error is indeed cleared
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_IsErrorSet(error_table, BMS_NON_CRITICAL_WATCHDOG_TIMEOUT, &is_set));
    ASSERT_FALSE(is_set);
}

TEST_F(SharedErrorTableTest, set_error_using_out_of_range_id)
{
    // Set an error using an out-of-range error ID
    ASSERT_EQ(EXIT_CODE_OUT_OF_RANGE, App_SharedErrorTable_SetError(error_table, NUM_ERROR_IDS, true));
}

TEST_F(SharedErrorTableTest, clear_error_using_out_of_range_id)
{
    // Clear an error using an out-of-range error ID
    ASSERT_EQ(EXIT_CODE_OUT_OF_RANGE, App_SharedErrorTable_SetError(error_table, NUM_ERROR_IDS, false));
}

TEST_F(SharedErrorTableTest, has_any_error_set_using_critical_error)
{
    // Set a critical error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyErrorSet(error_table));

    // Clear the critical error that was just set
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
}

TEST_F(SharedErrorTableTest, has_any_error_set_using_air_shutdown_error)
{
    // Set a AIR shutdown error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_AIR_SHUTDOWN_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyErrorSet(error_table));

    // Clear the AIR shutdown error that was just set
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_AIR_SHUTDOWN_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
}

TEST_F(SharedErrorTableTest, has_any_error_set_using_motor_shutdown_error)
{
    // Set a motor shutdown error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_MOTOR_SHUTDOWN_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyErrorSet(error_table));

    // Clear the motor shutdown error that was just set
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_MOTOR_SHUTDOWN_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
}

TEST_F(SharedErrorTableTest, has_any_error_set_using_non_critical_error)
{
    // Set a non-critical error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyErrorSet(error_table));

    // Clear the non-critical error that was just set
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_NON_CRITICAL_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
}

TEST_F(SharedErrorTableTest, has_any_critical_error_set)
{
    // Set a critical error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyCriticalErrorSet(error_table));

    // Clear the critical error that was just set
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasAnyCriticalErrorSet(error_table));
}

TEST_F(SharedErrorTableTest, has_any_air_shutdown_error_set)
{
    // Set a AIR shutdown error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_AIR_SHUTDOWN_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyAirShutdownErrorSet(error_table));

    // Clear the AIR shutdown error that was just set
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_AIR_SHUTDOWN_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasAnyAirShutdownErrorSet(error_table));
}

TEST_F(SharedErrorTableTest, has_any_motor_shutdown_error_set)
{
    // Set a motor shutdown error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyErrorSet(error_table));
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_MOTOR_SHUTDOWN_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyMotorShutdownErrorSet(error_table));

    // Clear the motor shutdown error that was just set
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_MOTOR_SHUTDOWN_ERROR, false));
    ASSERT_FALSE(App_SharedErrorTable_HasAnyMotorShutdownErrorSet(error_table));
}
TEST_F(SharedErrorTableTest, has_any_non_critical_error_set)
{
    // Set a non-critical error
    ASSERT_FALSE(App_SharedErrorTable_HasAnyNonCriticalErrorSet(error_table));
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    ASSERT_TRUE(App_SharedErrorTable_HasAnyNonCriticalErrorSet(error_table));

    // Clear the non-critical error that was just set
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_NON_CRITICAL_ERROR, false));
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
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));

    // Make sure the critical error that was just set can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_TRUE(App_SharedError_IsErrorInList(&error_list, DEFAULT_CRITICAL_ERROR));

    // Setting the same critical error should not modify the error list
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));

    // Again, make sure the critical error that was just set can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_TRUE(App_SharedError_IsErrorInList(&error_list, DEFAULT_CRITICAL_ERROR));
}

TEST_F(SharedErrorTableTest, get_all_errors_using_one_air_shutdown_error)
{
    // Set a AIR shutdown error
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_AIR_SHUTDOWN_ERROR, true));

    // Make sure the AIR shutdown error that was just set can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_TRUE(App_SharedError_IsErrorInList(&error_list, DEFAULT_AIR_SHUTDOWN_ERROR));

    // Setting the same AIR shutdown error should not modify the error list
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_AIR_SHUTDOWN_ERROR, true));

    // Again, make sure the AIR shutdown error that was just set can be
    // retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_TRUE(App_SharedError_IsErrorInList(&error_list, DEFAULT_AIR_SHUTDOWN_ERROR));
}

TEST_F(SharedErrorTableTest, get_all_errors_using_one_motor_shutdown_error)
{
    // Set a motor shutdown error
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_MOTOR_SHUTDOWN_ERROR, true));

    // Make sure the motor shutdown error that was just set can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_TRUE(App_SharedError_IsErrorInList(&error_list, DEFAULT_MOTOR_SHUTDOWN_ERROR));

    // Setting the same motor shutdown error should not modify the error list
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_MOTOR_SHUTDOWN_ERROR, true));

    // Again, make sure the motor shutdown error that was just set can be
    // retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
    ASSERT_TRUE(App_SharedError_IsErrorInList(&error_list, DEFAULT_MOTOR_SHUTDOWN_ERROR));
}

TEST_F(SharedErrorTableTest, get_all_errors_using_one_critical_error_and_one_non_critical_error)
{
    // Set a critical error and a non-critical_error
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_NON_CRITICAL_ERROR, true));

    // Make sure the critical error and non-critical error that were just set
    // can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(2, error_list.num_errors);
    ASSERT_TRUE(App_SharedError_IsErrorInList(&error_list, DEFAULT_CRITICAL_ERROR));
    ASSERT_TRUE(App_SharedError_IsErrorInList(&error_list, DEFAULT_NON_CRITICAL_ERROR));

    // Setting the same critical error and non-critical error should not modify
    // the error list
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));

    // Again, make sure the critical error and non-critical error that were just
    // set can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(2, error_list.num_errors);
    ASSERT_TRUE(App_SharedError_IsErrorInList(&error_list, DEFAULT_CRITICAL_ERROR));
    ASSERT_TRUE(App_SharedError_IsErrorInList(&error_list, DEFAULT_NON_CRITICAL_ERROR));
}

TEST_F(SharedErrorTableTest, get_all_errors_using_every_error)
{
    // Set every error
    for (size_t id = 0; id < NUM_ERROR_IDS; id++)
    {
        bool is_set = false;
        App_SharedErrorTable_SetError(error_table, (enum ErrorId)id, true);
        EXPECT_EQ(EXIT_CODE_OK, App_SharedErrorTable_IsErrorSet(error_table, (enum ErrorId)id, &is_set));
        ASSERT_TRUE(is_set);
    }

    // Make sure every error can be retrieved
    App_SharedErrorTable_GetAllErrors(error_table, &error_list);
    ASSERT_EQ(NUM_ERROR_IDS, error_list.num_errors);
    for (size_t id = 0; id < NUM_ERROR_IDS; id++)
    {
        EXPECT_TRUE(App_SharedError_IsErrorInList(&error_list, (enum ErrorId)id));
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
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));

    // Make sure the critical error that was just set can be retrieved
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // Setting the same critical error should not modify the error list
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));

    // Again, make sure the critical error that was just set can be retrieved
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
}

TEST_F(SharedErrorTableTest, get_all_critical_errors_using_one_critical_error_and_one_non_critical_error)
{
    // Set a critical error and a non-critical_error
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_NON_CRITICAL_ERROR, true));

    // Make sure we only retrieve one critical error, even though there are
    // two errors that are set
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
}

TEST_F(SharedErrorTableTest, get_all_critical_errors_using_one_critical_error_per_board_and_one_non_critical_error)
{
    // Set a critical error for each board plus an additional non-critical error
    ResetWithOneCriticalErrorForEveryBoard();
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_NON_CRITICAL_ERROR, true));

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

TEST_F(SharedErrorTableTest, get_all_non_critical_errors_using_one_non_critical_error)
{
    // Set a non-critical error
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);

    // Setting the same non-critical error should not modify the error list
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
}

TEST_F(SharedErrorTableTest, get_all_non_critical_errors_using_one_non_critical_and_one_critical_error)
{
    // Set a non-critical error and a critical error
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_NON_CRITICAL_ERROR, true));
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));

    // Make sure we only retrieve one non-critical error, even though there are
    // two errors that are set
    App_SharedErrorTable_GetAllNonCriticalErrors(error_table, &error_list);
    ASSERT_EQ(1, error_list.num_errors);
}

TEST_F(SharedErrorTableTest, get_all_non_critical_errors_using_one_non_critical_per_board_and_one_critical_error)
{
    // Set a non-critical error per board plus an additional critical error
    ResetWithOneNonCriticalErrorForEveryBoard();
    ASSERT_EQ(EXIT_CODE_OK, App_SharedErrorTable_SetError(error_table, DEFAULT_CRITICAL_ERROR, true));

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

TEST_F(SharedErrorTableTest, get_boards_with_no_errors_using_one_critical_error_per_board)
{
    // Every board has a critical error
    ResetWithOneCriticalErrorForEveryBoard();
    App_SharedErrorTable_GetBoardsWithNoErrors(error_table, &board_list);
    ASSERT_EQ(0, board_list.num_boards);
}

TEST_F(SharedErrorTableTest, get_boards_with_no_errors_using_one_non_critical_error)
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
TEST_F(SharedErrorTableTest, get_boards_with_no_errors_using_one_non_critical_error_per_board)
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
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, DEFAULT_CRITICAL_ERROR_BOARD));
}

TEST_F(SharedErrorTableTest, get_boards_with_errors_using_one_critical_error_per_board)
{
    // Every board has a critical error
    ResetWithOneCriticalErrorForEveryBoard();

    // Make sure we retrieve every board
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
}

TEST_F(SharedErrorTableTest, get_boards_with_errors_using_one_non_critical_error)
{
    // One board has anon-critical error
    ResetWithOneNonCriticalErrorForOneBoard();

    // Make sure we retrieve only the one correct board
    App_SharedErrorTable_GetBoardsWithErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, DEFAULT_NON_CRITICAL_ERROR_BOARD));
}

TEST_F(SharedErrorTableTest, get_boards_with_errors_using_one_non_critical_error_per_board)
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

TEST_F(SharedErrorTableTest, get_boards_with_critical_errors_using_one_critical_error)
{
    // One board has a critical error
    ResetWithOneCriticalErrorForOneBoard();

    // Make sure we retrieve only the one correct board
    App_SharedErrorTable_GetBoardsWithCriticalErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, DEFAULT_CRITICAL_ERROR_BOARD));
}

TEST_F(SharedErrorTableTest, get_boards_with_critical_errors_using_one_critical_error_per_board)
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

TEST_F(SharedErrorTableTest, get_boards_with_non_critical_errors_using_no_errors)
{
    // No board has any non-critical error
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(error_table, &board_list);

    // Make sure we retrieve no boards
    ASSERT_EQ(0, board_list.num_boards);
}

TEST_F(SharedErrorTableTest, get_boards_with_non_critical_errors_using_one_critical_error)
{
    // One board has a non-critical error
    ResetWithOneNonCriticalErrorForOneBoard();

    // Make sure we retrieve only the one correct board
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(error_table, &board_list);
    ASSERT_EQ(1, board_list.num_boards);
    ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, DEFAULT_NON_CRITICAL_ERROR_BOARD));
}

TEST_F(SharedErrorTableTest, get_boards_with_non_critical_errors_using_one_critical_error_per_board)
{
    // Every board has a non-critical error
    ResetWithOneNonCriticalErrorForEveryBoard();

    // Make sure we retrieve every board
    App_SharedErrorTable_GetBoardsWithNonCriticalErrors(error_table, &board_list);
    ASSERT_EQ(NUM_BOARDS, board_list.num_boards);
    for (auto board : GetAllBoards())
    {
        ASSERT_TRUE(App_SharedError_IsBoardInList(&board_list, board));
    }
}

TEST_F(SharedErrorTableTest, process_bms_non_critical_errors)
{
    std::vector<enum ErrorId> bms_non_critical_error_ids = { BMS_NON_CRITICAL_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        BMS, bms_non_critical_error_ids, CANMSGS_BMS_NON_CRITICAL_ERRORS_FRAME_ID,
        CANMSGS_BMS_NON_CRITICAL_ERRORS_LENGTH, App_CanMsgs_bms_non_critical_errors_pack,
        App_SharedErrorTable_GetBoardsWithNonCriticalErrors, App_SharedErrorTable_GetAllNonCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_bms_air_shutdown_errors)
{
    std::vector<enum ErrorId> bms_air_shutdown_error_ids = { BMS_AIR_SHUTDOWN_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        BMS, bms_air_shutdown_error_ids, CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_FRAME_ID,
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_LENGTH, App_CanMsgs_bms_air_shutdown_errors_pack,
        App_SharedErrorTable_GetBoardsWithCriticalErrors, App_SharedErrorTable_GetAllCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_bms_motor_shutdown_errors)
{
    std::vector<enum ErrorId> bms_motor_shutdown_error_ids = { BMS_MOTOR_SHUTDOWN_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        BMS, bms_motor_shutdown_error_ids, CANMSGS_BMS_MOTOR_SHUTDOWN_ERRORS_FRAME_ID,
        CANMSGS_BMS_MOTOR_SHUTDOWN_ERRORS_LENGTH, App_CanMsgs_bms_motor_shutdown_errors_pack,
        App_SharedErrorTable_GetBoardsWithCriticalErrors, App_SharedErrorTable_GetAllCriticalErrors);
}
TEST_F(SharedErrorTableTest, process_dcm_non_critical_errors)
{
    std::vector<enum ErrorId> dcm_non_critical_error_ids = { DCM_NON_CRITICAL_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        DCM, dcm_non_critical_error_ids, CANMSGS_DCM_NON_CRITICAL_ERRORS_FRAME_ID,
        CANMSGS_DCM_NON_CRITICAL_ERRORS_LENGTH, App_CanMsgs_dcm_non_critical_errors_pack,
        App_SharedErrorTable_GetBoardsWithNonCriticalErrors, App_SharedErrorTable_GetAllNonCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_dcm_air_shutdown_errors)
{
    std::vector<enum ErrorId> dcm_air_shutdown_error_ids = { DCM_AIR_SHUTDOWN_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        DCM, dcm_air_shutdown_error_ids, CANMSGS_DCM_AIR_SHUTDOWN_ERRORS_FRAME_ID,
        CANMSGS_DCM_AIR_SHUTDOWN_ERRORS_LENGTH, App_CanMsgs_dcm_air_shutdown_errors_pack,
        App_SharedErrorTable_GetBoardsWithCriticalErrors, App_SharedErrorTable_GetAllCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_dcm_motor_shutdown_errors)
{
    std::vector<enum ErrorId> dcm_motor_shutdown_error_ids = { DCM_MOTOR_SHUTDOWN_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        DCM, dcm_motor_shutdown_error_ids, CANMSGS_DCM_MOTOR_SHUTDOWN_ERRORS_FRAME_ID,
        CANMSGS_DCM_MOTOR_SHUTDOWN_ERRORS_LENGTH, App_CanMsgs_dcm_motor_shutdown_errors_pack,
        App_SharedErrorTable_GetBoardsWithCriticalErrors, App_SharedErrorTable_GetAllCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_dim_non_critical_errors)
{
    std::vector<enum ErrorId> dim_non_critical_error_ids = { DIM_NON_CRITICAL_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        DIM, dim_non_critical_error_ids, CANMSGS_DIM_NON_CRITICAL_ERRORS_FRAME_ID,
        CANMSGS_DIM_NON_CRITICAL_ERRORS_LENGTH, App_CanMsgs_dim_non_critical_errors_pack,
        App_SharedErrorTable_GetBoardsWithNonCriticalErrors, App_SharedErrorTable_GetAllNonCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_dim_air_shutdown_errors)
{
    std::vector<enum ErrorId> dim_air_shutdown_error_ids = { DIM_AIR_SHUTDOWN_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        DIM, dim_air_shutdown_error_ids, CANMSGS_DIM_AIR_SHUTDOWN_ERRORS_FRAME_ID,
        CANMSGS_DIM_AIR_SHUTDOWN_ERRORS_LENGTH, App_CanMsgs_dim_air_shutdown_errors_pack,
        App_SharedErrorTable_GetBoardsWithCriticalErrors, App_SharedErrorTable_GetAllCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_dim_motor_shutdown_errors)
{
    std::vector<enum ErrorId> dim_motor_shutdown_error_ids = { DIM_MOTOR_SHUTDOWN_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        DIM, dim_motor_shutdown_error_ids, CANMSGS_DIM_MOTOR_SHUTDOWN_ERRORS_FRAME_ID,
        CANMSGS_DIM_MOTOR_SHUTDOWN_ERRORS_LENGTH, App_CanMsgs_dim_motor_shutdown_errors_pack,
        App_SharedErrorTable_GetBoardsWithCriticalErrors, App_SharedErrorTable_GetAllCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_fsm_non_critical_errors)
{
    std::vector<enum ErrorId> fsm_non_critical_error_ids = { FSM_NON_CRITICAL_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        FSM, fsm_non_critical_error_ids, CANMSGS_FSM_NON_CRITICAL_ERRORS_FRAME_ID,
        CANMSGS_FSM_NON_CRITICAL_ERRORS_LENGTH, App_CanMsgs_fsm_non_critical_errors_pack,
        App_SharedErrorTable_GetBoardsWithNonCriticalErrors, App_SharedErrorTable_GetAllNonCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_fsm_air_shutdown_errors)
{
    std::vector<enum ErrorId> fsm_air_shutdown_error_ids = { FSM_AIR_SHUTDOWN_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        FSM, fsm_air_shutdown_error_ids, CANMSGS_FSM_AIR_SHUTDOWN_ERRORS_FRAME_ID,
        CANMSGS_FSM_AIR_SHUTDOWN_ERRORS_LENGTH, App_CanMsgs_fsm_air_shutdown_errors_pack,
        App_SharedErrorTable_GetBoardsWithCriticalErrors,

        App_SharedErrorTable_GetAllCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_fsm_motor_shutdown_errors)
{
    std::vector<enum ErrorId> fsm_motor_shutdown_error_ids = { FSM_MOTOR_SHUTDOWN_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        FSM, fsm_motor_shutdown_error_ids, CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_FRAME_ID,
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_LENGTH, App_CanMsgs_fsm_motor_shutdown_errors_pack,
        App_SharedErrorTable_GetBoardsWithCriticalErrors, App_SharedErrorTable_GetAllCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_pdm_non_critical_errors)
{
    std::vector<enum ErrorId> pdm_non_critical_error_ids = { PDM_NON_CRITICAL_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        PDM, pdm_non_critical_error_ids, CANMSGS_PDM_NON_CRITICAL_ERRORS_FRAME_ID,
        CANMSGS_PDM_NON_CRITICAL_ERRORS_LENGTH, App_CanMsgs_pdm_non_critical_errors_pack,
        App_SharedErrorTable_GetBoardsWithNonCriticalErrors, App_SharedErrorTable_GetAllNonCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_pdm_air_shutdown_errors)
{
    std::vector<enum ErrorId> pdm_air_shutdown_error_ids = { PDM_AIR_SHUTDOWN_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        PDM, pdm_air_shutdown_error_ids, CANMSGS_PDM_AIR_SHUTDOWN_ERRORS_FRAME_ID,
        CANMSGS_PDM_AIR_SHUTDOWN_ERRORS_LENGTH, App_CanMsgs_pdm_air_shutdown_errors_pack,
        App_SharedErrorTable_GetBoardsWithCriticalErrors, App_SharedErrorTable_GetAllCriticalErrors);
}

TEST_F(SharedErrorTableTest, process_pdm_motor_shutdown_errors)
{
    std::vector<enum ErrorId> pdm_motor_shutdown_error_ids = { PDM_MOTOR_SHUTDOWN_ERRORS };

    TestRoutineForSetErrorsFromCanMsg(
        PDM, pdm_motor_shutdown_error_ids, CANMSGS_PDM_MOTOR_SHUTDOWN_ERRORS_FRAME_ID,
        CANMSGS_PDM_MOTOR_SHUTDOWN_ERRORS_LENGTH, App_CanMsgs_pdm_motor_shutdown_errors_pack,
        App_SharedErrorTable_GetBoardsWithCriticalErrors, App_SharedErrorTable_GetAllCriticalErrors);
}
