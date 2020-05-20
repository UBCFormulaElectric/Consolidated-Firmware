#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "App_SharedErrorTable.h"

struct Error
{
    // The board this error is from
    enum ErrorBoard board;

    // The id of this error
    enum ErrorId id;

    // Whether or not this is a critical error
    bool is_critical;

    // If this error is enabled or not
    bool is_set;
};

struct ErrorTable
{
    struct Error errors[NUM_ERROR_IDS];
};

static void App_ResetBoardList(struct ErrorBoardList *board_list)
{
    for (size_t i = 0; i < NUM_ERROR_BOARDS; i++)
    {
        board_list->boards[i] = NUM_ERROR_BOARDS;
    }

    board_list->num_boards = 0;
}

struct ErrorTable *App_SharedErrorTable_CreateErrorTable(void)
{
    struct ErrorTable *error_table = malloc(sizeof(struct ErrorTable));
    assert(error_table != NULL);

    error_table->errors[BMS_NON_CRIT].board       = BMS;
    error_table->errors[BMS_NON_CRIT].id          = BMS_NON_CRIT;
    error_table->errors[BMS_NON_CRIT].is_critical = false;
    error_table->errors[BMS_NON_CRIT].is_set      = false;

    error_table->errors[DCM_NON_CRIT].board       = DCM;
    error_table->errors[DCM_NON_CRIT].id          = DCM_NON_CRIT;
    error_table->errors[DCM_NON_CRIT].is_critical = false;
    error_table->errors[DCM_NON_CRIT].is_set      = false;

    error_table->errors[DIM_NON_CRIT].board       = DIM;
    error_table->errors[DIM_NON_CRIT].id          = DIM_NON_CRIT;
    error_table->errors[DIM_NON_CRIT].is_critical = false;
    error_table->errors[DIM_NON_CRIT].is_set      = false;

    error_table->errors[FSM_NON_CRIT].board       = FSM;
    error_table->errors[FSM_NON_CRIT].id          = FSM_NON_CRIT;
    error_table->errors[FSM_NON_CRIT].is_critical = false;
    error_table->errors[FSM_NON_CRIT].is_set      = false;

    error_table->errors[PDM_NON_CRIT].board       = PDM;
    error_table->errors[PDM_NON_CRIT].id          = PDM_NON_CRIT;
    error_table->errors[PDM_NON_CRIT].is_critical = false;
    error_table->errors[PDM_NON_CRIT].is_set      = false;

    error_table->errors[BMS_CRIT].board       = BMS;
    error_table->errors[BMS_CRIT].id          = BMS_CRIT;
    error_table->errors[BMS_CRIT].is_critical = true;
    error_table->errors[BMS_CRIT].is_set      = false;

    error_table->errors[DCM_CRIT].board       = DCM;
    error_table->errors[DCM_CRIT].id          = DCM_CRIT;
    error_table->errors[DCM_CRIT].is_critical = true;
    error_table->errors[DCM_CRIT].is_set      = false;

    error_table->errors[DIM_CRIT].board       = DIM;
    error_table->errors[DIM_CRIT].id          = DIM_CRIT;
    error_table->errors[DIM_CRIT].is_critical = true;
    error_table->errors[DIM_CRIT].is_set      = false;

    error_table->errors[FSM_CRIT].board       = FSM;
    error_table->errors[FSM_CRIT].id          = FSM_CRIT;
    error_table->errors[FSM_CRIT].is_critical = true;
    error_table->errors[FSM_CRIT].is_set      = false;

    error_table->errors[PDM_CRIT].board       = PDM;
    error_table->errors[PDM_CRIT].id          = PDM_CRIT;
    error_table->errors[PDM_CRIT].is_critical = true;
    error_table->errors[PDM_CRIT].is_set      = false;

    return error_table;
}

void App_SharedErrorTable_DestroyErrorTable(struct ErrorTable *error_table)
{
    free(error_table);
}

struct ErrorList *App_SharedErrorTable_CreateErrorList(void)
{
    struct ErrorList *error_list = malloc(sizeof(struct ErrorList));
    assert(error_list != NULL);

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        error_list->errors[i] = NULL;
    }

    error_list->num_errors = 0;

    return error_list;
}

void App_SharedErrorTable_DestroyErrorList(struct ErrorList *error_list)
{
    free(error_list);
}


bool App_SharedErrorTable_IsErrorInList(
    enum ErrorId      error_id,
    struct ErrorList *error_list)
{
    for (uint32_t i = 0; i < error_list->num_errors; i++)
    {
        if (error_list->errors[i]->id == error_id)
        {
            return true;
        }
    }
    return false;
}

bool App_SharedErrorTable_IsBoardInList(
    enum ErrorBoard        board,
    struct ErrorBoardList *board_list)
{
    for (uint32_t i = 0; i < board_list->num_boards; i++)
    {
        if (board_list->boards[i] == board)
        {
            return true;
        }
    }
    return false;
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

    error_table->errors[error_id].is_set = is_set;
    return EXIT_CODE_OK;
}

bool App_SharedErrorTable_HasError(const struct ErrorTable *error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        if (error_table->errors[i].is_set)
        {
            return true;
        }
    }
    return false;
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

    *is_set = error_table->errors[error_id].is_set;
    return EXIT_CODE_OK;
}

bool App_SharedErrorTable_HasCriticalError(const struct ErrorTable *error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = &error_table->errors[i];
        if (error->is_critical && error->is_set)
        {
            return true;
        }
    }
    return false;
}

bool App_SharedErrorTable_HasNonCriticalError(
    const struct ErrorTable *error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = &error_table->errors[i];
        if (!error->is_critical && error->is_set)
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
        struct Error *error = &error_table->errors[i];

        if (error->is_set)
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
        struct Error *error = &error_table->errors[i];

        if (error->is_set && error->is_critical)
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
        struct Error *error = &error_table->errors[i];

        if (error->is_set && !error->is_critical)
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

    for (enum ErrorBoard board = 0; board < NUM_ERROR_BOARDS; board++)
    {
        if (!App_SharedErrorTable_IsBoardInList(board, &boards_with_errors))
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
    App_ResetBoardList(board_list);

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = &error_table->errors[i];

        if (error->is_set &&
            !App_SharedErrorTable_IsBoardInList(error->board, board_list))
        {
            board_list->boards[board_list->num_boards] = error->board;
            board_list->num_boards++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithCriticalErrors(
    const struct ErrorTable *error_table,
    struct ErrorBoardList *  board_list)
{
    App_ResetBoardList(board_list);

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = &error_table->errors[i];

        if (error->is_set && error->is_critical &&
            !App_SharedErrorTable_IsBoardInList(error->board, board_list))
        {
            board_list->boards[board_list->num_boards] = error->board;
            board_list->num_boards++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
    const struct ErrorTable *error_table,
    struct ErrorBoardList *  board_list)
{
    App_ResetBoardList(board_list);

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = &error_table->errors[i];

        if (error->is_set && !error->is_critical &&
            !App_SharedErrorTable_IsBoardInList(error->board, board_list))
        {
            board_list->boards[board_list->num_boards] = error->board;
            board_list->num_boards++;
        }
    }
}
enum ErrorBoard App_SharedError_GetBoard(const struct Error *error)
{
    return error->board;
}

bool App_SharedError_IsCritical(const struct Error *error)
{
    return error->is_critical;
}

uint32_t App_SharedError_GetId(const struct Error* error)
{
    return error->id;
}
