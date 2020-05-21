#include <stdlib.h>
#include <assert.h>
#include "App_SharedError.h"

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

struct Error *App_SharedError_Create(void)
{
    struct Error *error = malloc(sizeof(error));
    assert(error != NULL);

    error->board       = NUM_ERROR_BOARDS;
    error->id          = NUM_ERROR_IDS;
    error->is_critical = false;
    error->is_set      = false;

    return error;
}

void App_SharedError_Destroy(struct Error *error)
{
    free(error);
}

void App_SharedError_SetBoard(struct Error *error, enum ErrorBoard board)
{
    error->board = board;
}

void App_SharedError_SetIsCritical(struct Error *error, bool is_critical)
{
    error->is_critical = is_critical;
}

void App_SharedError_SetId(struct Error *error, uint32_t id)
{
    error->id = id;
}

void App_SharedError_SetIsSet(struct Error *error, bool is_set)
{
    error->is_set = is_set;
}

enum ErrorBoard App_SharedError_GetBoard(const struct Error *error)
{
    return error->board;
}

bool App_SharedError_GetIsCritical(const struct Error *error)
{
    return error->is_critical;
}

uint32_t App_SharedError_GetId(const struct Error *error)
{
    return error->id;
}

bool App_SharedError_GetIsSet(const struct Error *error)
{
    return error->is_set;
}

struct ErrorList *App_SharedError_CreateErrorList(void)
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

void App_SharedError_DestroyErrorList(struct ErrorList *error_list)
{
    free(error_list);
}

bool App_SharedError_IsErrorInList(
    struct ErrorList *error_list,
    enum ErrorId      error_id)
{
    for (uint32_t i = 0; i < error_list->num_errors; i++)
    {
        if (App_SharedError_GetId(error_list->errors[i]) == error_id)
        {
            return true;
        }
    }
    return false;
}

bool App_SharedError_IsBoardInList(
    struct ErrorBoardList *board_list,
    enum ErrorBoard        board)
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
