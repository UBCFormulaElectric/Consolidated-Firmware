#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "App_SharedErrorTable.h"

struct ErrorTable
{
    struct Error *errors[NUM_ERROR_IDS];
};

static void App_ResetBoardList(struct ErrorBoardList *board_list)
{
    for (size_t i = 0; i < NUM_ERROR_BOARDS; i++)
    {
        board_list->boards[i] = NUM_ERROR_BOARDS;
    }

    board_list->num_boards = 0;
}

struct ErrorTable *App_SharedErrorTable_Create(void)
{
    struct ErrorTable *error_table = malloc(sizeof(struct ErrorTable));
    assert(error_table != NULL);

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        error_table->errors[i] = App_SharedError_Create();
    }

    App_SharedError_SetBoard(error_table->errors[BMS_NON_CRIT], BMS);
    App_SharedError_SetId(error_table->errors[BMS_NON_CRIT], BMS_NON_CRIT);
    App_SharedError_SetIsCritical(error_table->errors[BMS_NON_CRIT], false);

    App_SharedError_SetBoard(error_table->errors[DCM_NON_CRIT], DCM);
    App_SharedError_SetId(error_table->errors[DCM_NON_CRIT], DCM_NON_CRIT);
    App_SharedError_SetIsCritical(error_table->errors[DCM_NON_CRIT], false);

    App_SharedError_SetBoard(error_table->errors[DIM_NON_CRIT], DIM);
    App_SharedError_SetId(error_table->errors[DIM_NON_CRIT], DIM_NON_CRIT);
    App_SharedError_SetIsCritical(error_table->errors[DIM_NON_CRIT], false);

    App_SharedError_SetBoard(error_table->errors[FSM_NON_CRIT], FSM);
    App_SharedError_SetId(error_table->errors[FSM_NON_CRIT], FSM_NON_CRIT);
    App_SharedError_SetIsCritical(error_table->errors[FSM_NON_CRIT], false);

    App_SharedError_SetBoard(error_table->errors[PDM_NON_CRIT], PDM);
    App_SharedError_SetId(error_table->errors[PDM_NON_CRIT], PDM_NON_CRIT);
    App_SharedError_SetIsCritical(error_table->errors[PDM_NON_CRIT], false);

    App_SharedError_SetBoard(error_table->errors[BMS_CRIT], BMS);
    App_SharedError_SetId(error_table->errors[BMS_CRIT], BMS_CRIT);
    App_SharedError_SetIsCritical(error_table->errors[BMS_CRIT], true);

    App_SharedError_SetBoard(error_table->errors[DCM_CRIT], DCM);
    App_SharedError_SetId(error_table->errors[DCM_CRIT], DCM_CRIT);
    App_SharedError_SetIsCritical(error_table->errors[DCM_CRIT], true);

    App_SharedError_SetBoard(error_table->errors[DIM_CRIT], DIM);
    App_SharedError_SetId(error_table->errors[DIM_CRIT], DIM_CRIT);
    App_SharedError_SetIsCritical(error_table->errors[DIM_CRIT], true);

    App_SharedError_SetBoard(error_table->errors[FSM_CRIT], FSM);
    App_SharedError_SetId(error_table->errors[FSM_CRIT], FSM_CRIT);
    App_SharedError_SetIsCritical(error_table->errors[FSM_CRIT], true);

    App_SharedError_SetBoard(error_table->errors[PDM_CRIT], PDM);
    App_SharedError_SetId(error_table->errors[PDM_CRIT], PDM_CRIT);
    App_SharedError_SetIsCritical(error_table->errors[PDM_CRIT], true);

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

bool App_SharedErrorTable_HasError(const struct ErrorTable *error_table)
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

bool App_SharedErrorTable_HasCriticalError(const struct ErrorTable *error_table)
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

bool App_SharedErrorTable_HasNonCriticalError(
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

    for (enum ErrorBoard board = 0; board < NUM_ERROR_BOARDS; board++)
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
    App_ResetBoardList(board_list);

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
    App_ResetBoardList(board_list);

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
    App_ResetBoardList(board_list);

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
