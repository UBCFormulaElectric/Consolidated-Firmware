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

struct ErrorTable *App_SharedErrorTable_Create(void)
{
    struct ErrorTable *error_table = malloc(sizeof(struct ErrorTable));
    assert(error_table != NULL);

    error_table->errors[BMS_FOO].board       = BMS;
    error_table->errors[BMS_FOO].id          = BMS_FOO;
    error_table->errors[BMS_FOO].is_critical = false;
    error_table->errors[BMS_FOO].is_set      = false;

    error_table->errors[DCM_BAR].board       = DCM;
    error_table->errors[DCM_BAR].id          = DCM_BAR;
    error_table->errors[DCM_BAR].is_critical = false;
    error_table->errors[DCM_BAR].is_set      = false;

    error_table->errors[FSM_BAZ].board       = FSM;
    error_table->errors[FSM_BAZ].id          = FSM_BAZ;
    error_table->errors[FSM_BAZ].is_critical = false;
    error_table->errors[FSM_BAZ].is_set      = false;

    return error_table;
}

void App_SharedErrorTable_Destroy(struct ErrorTable *error_table)
{
    free(error_table);
}

bool App_SharedErrorTable_IsBoardInList(
    enum ErrorBoard boards[],
    int             num_boards,
    enum ErrorBoard board)
{
    for (int i = 0; i < num_boards; i++)
    {
        if (boards[i] == board)
        {
            return true;
        }
    }
    return false;
}

enum ErrorBoard App_SharedErrorTable_GetBoardForError(const struct Error *error)
{
    return error->board;
}

bool App_SharedErrorTable_IsErrorCritical(const struct Error *error)
{
    return error->is_critical;
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
    bool* is_set)
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
    const struct ErrorTable *error_table,
    struct Error *           errors,
    uint32_t *                    num_errors)
{
    *num_errors = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = &error_table->errors[i];

        if (error->is_set)
        {
            errors[*num_errors] = *error;
            (*num_errors)++;
        }
    }
}

void App_SharedErrorTable_GetAllCriticalErrors(
    const struct ErrorTable *error_table,
    struct Error *           errors,
    uint32_t *                    num_errors)
{
    *num_errors = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = &error_table->errors[i];

        if (error->is_set && error->is_critical)
        {
            errors[*num_errors] = *error;
            (*num_errors)++;
        }
    }
}

void App_SharedErrorTable_GetAllNonCriticalErrors(
    const struct ErrorTable *error_table,
    struct Error *           errors,
    uint32_t *                    num_errors)
{
    *num_errors = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = &error_table->errors[i];

        if (error->is_set && !error->is_critical)
        {
            errors[*num_errors] = *error;
            (*num_errors)++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithNoErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    uint32_t *                    num_boards)
{
    enum ErrorBoard boards_with_errors[NUM_ERROR_BOARDS] = { 0 };
    uint32_t num_boards_with_errors               = 0;

    App_SharedErrorTable_GetBoardsWithErrors(
        error_table, boards_with_errors, &num_boards_with_errors);

    *num_boards = 0;

    for (enum ErrorBoard board = 0; board < NUM_ERROR_BOARDS; board++)
    {
        if (!App_SharedErrorTable_IsBoardInList(
                boards_with_errors, num_boards_with_errors, board))
        {
            boards[*num_boards] = board;
            (*num_boards)++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    uint32_t *                    num_boards)
{
    for (size_t i = 0; i < NUM_ERROR_BOARDS; i++)
    {
        boards[i] = NUM_ERROR_BOARDS;
    }

    *num_boards = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = &error_table->errors[i];

        if (error->is_set && App_SharedErrorTable_IsBoardInList(
                                 boards, NUM_ERROR_BOARDS, error->board))
        {
            boards[*num_boards] = error->board;
            (*num_boards)++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithCriticalErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    uint32_t *                    num_boards)
{
    for (size_t i = 0; i < NUM_ERROR_BOARDS; i++)
    {
        boards[i] = NUM_ERROR_BOARDS;
    }

    *num_boards = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = &error_table->errors[i];

        if (error->is_set && error->is_critical &&
            App_SharedErrorTable_IsBoardInList(
                boards, NUM_ERROR_BOARDS, error->board))
        {
            boards[*num_boards] = error->board;
            (*num_boards)++;
        }
    }
}

void App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    uint32_t *                    num_boards)
{
    for (size_t i = 0; i < NUM_ERROR_BOARDS; i++)
    {
        boards[i] = NUM_ERROR_BOARDS;
    }

    *num_boards = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = &error_table->errors[i];

        if (error->is_set && !error->is_critical &&
            App_SharedErrorTable_IsBoardInList(
                boards, NUM_ERROR_BOARDS, error->board))
        {
            boards[*num_boards] = error->board;
            (*num_boards)++;
        }
    }
}
