#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "App_SharedErrorTable.h"

struct ErrorTable
{
    struct Error *errors[NUM_ERROR_IDS];
};

#define INIT_ERROR(id, board, error_type)                     \
    App_SharedError_SetBoard(error_table->errors[id], board); \
    App_SharedError_SetId(error_table->errors[id], id);       \
    App_SharedError_SetErrorType(error_table->errors[id], error_type)

struct ErrorTable *App_SharedErrorTable_Create(void)
{
    struct ErrorTable *error_table = malloc(sizeof(struct ErrorTable));
    assert(error_table != NULL);

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        error_table->errors[i] = App_SharedError_Create();
    }

    // clang-format off
    INIT_ERROR(BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, BMS, WARNING);
    INIT_ERROR(BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, BMS, WARNING);
    INIT_ERROR(BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, BMS, WARNING);
    INIT_ERROR(BMS_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, BMS, WARNING);
    INIT_ERROR(BMS_WARNING_WATCHDOG_TIMEOUT, BMS, WARNING);

    INIT_ERROR(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, DCM, WARNING);
    INIT_ERROR(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, DCM, WARNING);
    INIT_ERROR(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, DCM, WARNING);
    INIT_ERROR(DCM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, DCM, WARNING);
    INIT_ERROR(DCM_WARNING_WATCHDOG_TIMEOUT, DCM, WARNING);
    INIT_ERROR(DCM_WARNING_ACCELERATION_X_OUT_OF_RANGE, DCM, WARNING);
    INIT_ERROR(DCM_WARNING_ACCELERATION_Y_OUT_OF_RANGE, DCM, WARNING);
    INIT_ERROR(DCM_WARNING_ACCELERATION_Z_OUT_OF_RANGE, DCM, WARNING);

    INIT_ERROR(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, DIM, WARNING);
    INIT_ERROR(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK100HZ, DIM, WARNING);
    INIT_ERROR(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, DIM, WARNING);
    INIT_ERROR(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, DIM, WARNING);
    INIT_ERROR(DIM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, DIM, WARNING);
    INIT_ERROR(DIM_WARNING_WATCHDOG_TIMEOUT, DIM, WARNING);

    INIT_ERROR(FSM_WARNING_PAPPS_OUT_OF_RANGE, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_SAPPS_OUT_OF_RANGE, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_WATCHDOG_TIMEOUT, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_BSPD_FAULT, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_LEFT_WHEEL_SPEED_OUT_OF_RANGE, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_RIGHT_WHEEL_SPEED_OUT_OF_RANGE, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_FLOW_RATE_OUT_OF_RANGE, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_STEERING_ANGLE_OUT_OF_RANGE, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_BRAKE_PRESSURE_OUT_OF_RANGE, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_BRAKE_PRESSURE_OPEN_SC, FSM, WARNING);
    INIT_ERROR(FSM_WARNING_BRAKE_PRESSURE_OPEN_OC, FSM, WARNING);

    INIT_ERROR(PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, PDM, WARNING);
    INIT_ERROR(PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, PDM, WARNING);
    INIT_ERROR(PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, PDM, WARNING);
    INIT_ERROR(PDM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, PDM, WARNING);
    INIT_ERROR(PDM_WARNING_WATCHDOG_TIMEOUT, PDM, WARNING);

    INIT_ERROR(GSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, GSM, WARNING);
    INIT_ERROR(GSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, GSM, WARNING);
    INIT_ERROR(GSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, GSM, WARNING);
    INIT_ERROR(GSM_WARNING_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, GSM, WARNING);
    INIT_ERROR(GSM_WARNING_WATCHDOG_TIMEOUT, GSM, WARNING);

    INIT_ERROR(BMS_FAULTS_CHARGER_DISCONNECTED_IN_CHARGE_STATE, BMS, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(BMS_FAULTS_CELL_OVERVOLTAGE_FAULT, BMS, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(BMS_FAULTS_CELL_UNDERVOLTAGE_FAULT, BMS, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(BMS_FAULTS_MODULE_COMM_ERROR, BMS, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(BMS_FAULTS_CELL_UNDERTEMP_FAULT, BMS, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(BMS_FAULTS_CELL_OVERTEMP_FAULT, BMS, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(BMS_FAULTS_CHARGER_FAULT_DETECTED, BMS, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(BMS_FAULTS_HAS_REACHED_MAX_V, BMS, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(BMS_FAULTS_CHARGING_EXT_SHUTDOWN_OCCURRED, BMS, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(BMS_FAULTS_TS_OVERCURRENT_FAULT, BMS, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(BMS_FAULTS_PRECHARGE_ERROR, BMS, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(DIM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, DIM, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(DCM_AIR_SHUTDOWN_MISSING_HEARTBEAT, DCM, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(FSM_AIR_SHUTDOWN_MISSING_HEARTBEAT, FSM, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(PDM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, PDM, AIR_SHUTDOWN_ERROR);
    INIT_ERROR(GSM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, GSM, AIR_SHUTDOWN_ERROR);

    INIT_ERROR(BMS_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, BMS, MOTOR_SHUTDOWN_ERROR);
    INIT_ERROR(DCM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, DCM, MOTOR_SHUTDOWN_ERROR);
    INIT_ERROR(DIM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, DIM, MOTOR_SHUTDOWN_ERROR);

    INIT_ERROR(FSM_MOTOR_SHUTDOWN_APPS_HAS_DISAGREEMENT, FSM, MOTOR_SHUTDOWN_ERROR);
    INIT_ERROR(FSM_MOTOR_SHUTDOWN_PAPPS_ALARM_IS_ACTIVE, FSM, MOTOR_SHUTDOWN_ERROR);
    INIT_ERROR(FSM_MOTOR_SHUTDOWN_SAPPS_ALARM_IS_ACTIVE, FSM, MOTOR_SHUTDOWN_ERROR);
    INIT_ERROR(FSM_MOTOR_SHUTDOWN_FLOW_METER_HAS_UNDERFLOW, FSM, MOTOR_SHUTDOWN_ERROR);
    INIT_ERROR(FSM_MOTOR_SHUTDOWN_TORQUE_PLAUSIBILITY_CHECK_FAILED, FSM, MOTOR_SHUTDOWN_ERROR);

    INIT_ERROR(PDM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, PDM, MOTOR_SHUTDOWN_ERROR);
    INIT_ERROR(GSM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, GSM, MOTOR_SHUTDOWN_ERROR);

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

ExitCode App_SharedErrorTable_SetError(struct ErrorTable *error_table, enum ErrorId error_id, bool is_set)
{
    if ((int)error_id >= NUM_ERROR_IDS)
    {
        return EXIT_CODE_OUT_OF_RANGE;
    }
    struct Error *error = error_table->errors[error_id];
    App_SharedError_SetIsSet(error, is_set);

    return EXIT_CODE_OK;
}

ExitCode App_SharedErrorTable_IsErrorSet(const struct ErrorTable *error_table, enum ErrorId error_id, bool *is_set)
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

bool App_SharedErrorTable_HasAnyCriticalErrorSet(const struct ErrorTable *error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = error_table->errors[i];
        if (App_SharedError_IsCritical(error) && App_SharedError_GetIsSet(error))
        {
            return true;
        }
    }
    return false;
}

bool App_SharedErrorTable_HasAnyAirShutdownErrorSet(const struct ErrorTable *error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = error_table->errors[i];
        if (App_SharedError_GetErrorType(error) == AIR_SHUTDOWN_ERROR && App_SharedError_GetIsSet(error))
        {
            return true;
        }
    }
    return false;
}

bool App_SharedErrorTable_HasAnyMotorShutdownErrorSet(const struct ErrorTable *error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = error_table->errors[i];
        if (App_SharedError_GetErrorType(error) == MOTOR_SHUTDOWN_ERROR && App_SharedError_GetIsSet(error))
        {
            return true;
        }
    }
    return false;
}

bool App_SharedErrorTable_HasAnyWarningSet(const struct ErrorTable *error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = error_table->errors[i];
        if (App_SharedError_IsWarning(error) && App_SharedError_GetIsSet(error))
        {
            return true;
        }
    }
    return false;
}

void App_SharedErrorTable_GetAllErrors(struct ErrorTable *error_table, struct ErrorList *error_list)
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

void App_SharedErrorTable_GetAllCriticalErrors(struct ErrorTable *error_table, struct ErrorList *error_list)
{
    error_list->num_errors = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        struct Error *error = error_table->errors[i];
        if (App_SharedError_GetIsSet(error) && App_SharedError_IsCritical(error))
        {
            error_list->errors[error_list->num_errors] = error;
            error_list->num_errors++;
        }
    }
}

void App_SharedErrorTable_GetAllWarnings(struct ErrorTable *error_table, struct ErrorList *error_list)
{
    error_list->num_errors = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        struct Error *error = error_table->errors[i];

        if (App_SharedError_GetIsSet(error) && App_SharedError_IsWarning(error))
        {
            error_list->errors[error_list->num_errors] = error;
            error_list->num_errors++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithNoErrors(const struct ErrorTable *error_table, struct ErrorBoardList *board_list)
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

void App_SharedErrorTable_GetBoardsWithErrors(const struct ErrorTable *error_table, struct ErrorBoardList *board_list)
{
    board_list->num_boards = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = error_table->errors[i];

        if (App_SharedError_GetIsSet(error) &&
            !App_SharedError_IsBoardInList(board_list, App_SharedError_GetBoard(error)))
        {
            board_list->boards[board_list->num_boards] = App_SharedError_GetBoard(error);
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

        if (App_SharedError_GetIsSet(error) && App_SharedError_IsCritical(error) &&
            !App_SharedError_IsBoardInList(board_list, App_SharedError_GetBoard(error)))
        {
            board_list->boards[board_list->num_boards] = App_SharedError_GetBoard(error);
            board_list->num_boards++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithWarnings(const struct ErrorTable *error_table, struct ErrorBoardList *board_list)
{
    board_list->num_boards = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = error_table->errors[i];

        if (App_SharedError_GetIsSet(error) && App_SharedError_IsWarning(error) &&
            !App_SharedError_IsBoardInList(board_list, App_SharedError_GetBoard(error)))
        {
            board_list->boards[board_list->num_boards] = App_SharedError_GetBoard(error);
            board_list->num_boards++;
        }
    }
}
