#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "App_SharedErrorTable.h"

struct ErrorTable
{
    struct Error *errors[NUM_ERROR_IDS];
};

#define INIT_ERROR(id, board, is_critical)                    \
    App_SharedError_SetBoard(error_table->errors[id], board); \
    App_SharedError_SetId(error_table->errors[id], id);       \
    App_SharedError_SetIsCritical(error_table->errors[id], is_critical)

struct ErrorTable *App_SharedErrorTable_Create(void)
{
    struct ErrorTable *error_table = malloc(sizeof(struct ErrorTable));
    assert(error_table != NULL);

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        error_table->errors[i] = App_SharedError_Create();
    }

    // clang-format off
    INIT_ERROR(BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, BMS, false);
    INIT_ERROR(BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, BMS, false);
    INIT_ERROR(BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, BMS, false);
    INIT_ERROR(BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, BMS, false);
    INIT_ERROR(BMS_NON_CRITICAL_WATCHDOG_TIMEOUT, BMS, false);

    INIT_ERROR(DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, DCM, false);
    INIT_ERROR(DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, DCM, false);
    INIT_ERROR(DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, DCM, false);
    INIT_ERROR(DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, DCM, false);
    INIT_ERROR(DCM_NON_CRITICAL_WATCHDOG_TIMEOUT, DCM, false);

    INIT_ERROR(DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, DIM, false);
    INIT_ERROR(DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK100HZ, DIM, false);
    INIT_ERROR(DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, DIM, false);
    INIT_ERROR(DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, DIM, false);
    INIT_ERROR(DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, DIM, false);
    INIT_ERROR(DIM_NON_CRITICAL_WATCHDOG_TIMEOUT, DIM, false);

    INIT_ERROR(FSM_NON_CRITICAL_PAPPS_OUT_OF_RANGE, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_SAPPS_OUT_OF_RANGE, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_WATCHDOG_TIMEOUT, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_BSPD_FAULT, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_LEFT_WHEEL_SPEED_OUT_OF_RANGE, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_RIGHT_WHEEL_SPEED_OUT_OF_RANGE, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_PRIMARY_FLOW_RATE_OUT_OF_RANGE, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_SECONDARY_FLOW_RATE_OUT_OF_RANGE, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_STEERING_ANGLE_OUT_OF_RANGE, FSM, false);
    INIT_ERROR(FSM_NON_CRITICAL_BRAKE_PRESSURE_OUT_OF_RANGE, FSM, false);

    INIT_ERROR(PDM_NON_CRITICAL_MISSING_HEARTBEAT, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_BOOST_PGOOD_FAULT, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_CELL_BALANCE_OVERVOLTAGE_FAULT, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_CHARGER_FAULT, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_EFUSE_FAULT, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_WATCHDOG_TIMEOUT, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_VBAT_VOLTAGE_OUT_OF_RANGE, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL__24V_AUX_VOLTAGE_OUT_OF_RANGE, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL__24V_ACC_VOLTAGE_OUT_OF_RANGE, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_AUX1_CURRENT_OUT_OF_RANGE, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_AUX2_CURRENT_OUT_OF_RANGE, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_LEFT_INVERTER_CURRENT_OUT_OF_RANGE, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_ENERGY_METER_CURRENT_OUT_OF_RANGE, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_CAN_CURRENT_OUT_OF_RANGE, PDM, false);
    INIT_ERROR(PDM_NON_CRITICAL_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE, PDM, false);

    INIT_ERROR(BMS_AIR_SHUTDOWN_CHARGER_DISCONNECTED_IN_CHARGE_STATE, BMS, true);
    INIT_ERROR(DCM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, DCM, true);
    INIT_ERROR(DIM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, DIM, true);
    INIT_ERROR(FSM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, FSM, true);
    INIT_ERROR(PDM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, PDM, true);

    INIT_ERROR(BMS_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, BMS, true);
    INIT_ERROR(DCM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, DCM, true);
    INIT_ERROR(DIM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, DIM, true);
    INIT_ERROR(FSM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, FSM, true);
    INIT_ERROR(PDM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, PDM, true);
    // clang-format on

    return error_table;
}

void App_SharedErrorTable_Destroy(struct ErrorTable *error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        App_SharedError_Destroy(error_table->errors[i]);
    }

    free(error_table);
}

ExitCode App_SharedErrorTable_SetError(
    struct ErrorTable *error_table,
    enum ErrorId       error_id,
    bool               is_set)
{
    if ((int)error_id >= NUM_ERROR_IDS)
    {
        return EXIT_CODE_OUT_OF_RANGE;
    }
    struct Error *error = error_table->errors[error_id];
    App_SharedError_SetIsSet(error, is_set);

    return EXIT_CODE_OK;
}

ExitCode App_SharedErrorTable_IsErrorSet(
    const struct ErrorTable *error_table,
    enum ErrorId             error_id,
    bool *                   is_set)
{
    if (error_id >= NUM_ERROR_IDS)
    {
        return EXIT_CODE_OUT_OF_RANGE;
    }

    *is_set = App_SharedError_GetIsSet(error_table->errors[error_id]);
    return EXIT_CODE_OK;
}

bool App_SharedErrorTable_HasAnyErrorSet(const struct ErrorTable *error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        if (App_SharedError_GetIsSet(error_table->errors[i]))
        {
            return true;
        }
    }
    return false;
}

bool App_SharedErrorTable_HasAnyCriticalErrorSet(
    const struct ErrorTable *error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = error_table->errors[i];
        if (App_SharedError_GetIsCritical(error) &&
            App_SharedError_GetIsSet(error))
        {
            return true;
        }
    }
    return false;
}

bool App_SharedErrorTable_HasAnyNonCriticalErrorSet(
    const struct ErrorTable *error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = error_table->errors[i];
        if (!App_SharedError_GetIsCritical(error) &&
            App_SharedError_GetIsSet(error))
        {
            return true;
        }
    }
    return false;
}

void App_SharedErrorTable_GetAllErrors(
    struct ErrorTable *error_table,
    struct ErrorList * error_list)
{
    error_list->num_errors = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        struct Error *error = error_table->errors[i];

        if (App_SharedError_GetIsSet(error))
        {
            error_list->errors[error_list->num_errors] = error;
            error_list->num_errors++;
        }
    }
}

void App_SharedErrorTable_GetAllCriticalErrors(
    struct ErrorTable *error_table,
    struct ErrorList * error_list)
{
    error_list->num_errors = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        struct Error *error = error_table->errors[i];

        if (App_SharedError_GetIsSet(error) &&
            App_SharedError_GetIsCritical(error))
        {
            error_list->errors[error_list->num_errors] = error;
            error_list->num_errors++;
        }
    }
}

void App_SharedErrorTable_GetAllNonCriticalErrors(
    struct ErrorTable *error_table,
    struct ErrorList * error_list)
{
    error_list->num_errors = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        struct Error *error = error_table->errors[i];

        if (App_SharedError_GetIsSet(error) &&
            !App_SharedError_GetIsCritical(error))
        {
            error_list->errors[error_list->num_errors] = error;
            error_list->num_errors++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithNoErrors(
    const struct ErrorTable *error_table,
    struct ErrorBoardList *  board_list)
{
    struct ErrorBoardList boards_with_errors;

    App_SharedErrorTable_GetBoardsWithErrors(error_table, &boards_with_errors);

    board_list->num_boards = 0;

    for (enum Board board = 0; board < NUM_BOARDS; board++)
    {
        if (!App_SharedError_IsBoardInList(&boards_with_errors, board))
        {
            board_list->boards[board_list->num_boards] = board;
            board_list->num_boards++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithErrors(
    const struct ErrorTable *error_table,
    struct ErrorBoardList *  board_list)
{
    board_list->num_boards = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = error_table->errors[i];

        if (App_SharedError_GetIsSet(error) &&
            !App_SharedError_IsBoardInList(
                board_list, App_SharedError_GetBoard(error)))
        {
            board_list->boards[board_list->num_boards] =
                App_SharedError_GetBoard(error);
            board_list->num_boards++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithCriticalErrors(
    const struct ErrorTable *error_table,
    struct ErrorBoardList *  board_list)
{
    board_list->num_boards = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = error_table->errors[i];

        if (App_SharedError_GetIsSet(error) &&
            App_SharedError_GetIsCritical(error) &&
            !App_SharedError_IsBoardInList(
                board_list, App_SharedError_GetBoard(error)))
        {
            board_list->boards[board_list->num_boards] =
                App_SharedError_GetBoard(error);
            board_list->num_boards++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
    const struct ErrorTable *error_table,
    struct ErrorBoardList *  board_list)
{
    board_list->num_boards = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = error_table->errors[i];

        if (App_SharedError_GetIsSet(error) &&
            !App_SharedError_GetIsCritical(error) &&
            !App_SharedError_IsBoardInList(
                board_list, App_SharedError_GetBoard(error)))
        {
            board_list->boards[board_list->num_boards] =
                App_SharedError_GetBoard(error);
            board_list->num_boards++;
        }
    }
}
