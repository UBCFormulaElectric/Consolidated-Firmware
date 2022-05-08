#include <stdlib.h>
#include <assert.h>
#include "App_SharedError.h"

struct Error
{
    // The board this error is from
    enum Board board;

    // The id of this error
    enum ErrorId id;

    // The type of this error
    enum ErrorType error_type;

    // If this error is enabled or not
    bool is_set;
};

struct Error *App_SharedError_Create(void)
{
    struct Error *error = malloc(sizeof(struct Error));
    assert(error != NULL);

    error->board      = NUM_BOARDS;
    error->id         = NUM_ERROR_IDS;
    error->error_type = NUM_ERROR_TYPES;
    error->is_set     = false;

    return error;
}

void App_SharedError_Destroy(struct Error *error)
{
    free(error);
}

void App_SharedError_SetBoard(struct Error *error, enum Board board)
{
    error->board = board;
}

void App_SharedError_SetErrorType(struct Error *error, enum ErrorType error_type)
{
    error->error_type = error_type;
}

void App_SharedError_SetId(struct Error *error, uint32_t id)
{
    error->id = id;
}

void App_SharedError_SetIsSet(struct Error *error, bool is_set)
{
    error->is_set = is_set;
}

enum Board App_SharedError_GetBoard(const struct Error *error)
{
    return error->board;
}

enum ErrorType App_SharedError_GetErrorType(const struct Error *error)
{
    return error->error_type;
}

uint32_t App_SharedError_GetId(const struct Error *error)
{
    return error->id;
}

bool App_SharedError_GetIsSet(const struct Error *error)
{
    return error->is_set;
}

bool App_SharedError_IsCritical(const struct Error *error)
{
    enum ErrorType error_type = App_SharedError_GetErrorType(error);

    if (error_type == AIR_SHUTDOWN_ERROR || error_type == MOTOR_SHUTDOWN_ERROR)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool App_SharedError_IsNonCritical(const struct Error *error)
{
    enum ErrorType error_type = App_SharedError_GetErrorType(error);

    return (error_type == NON_CRITICAL_ERROR) ? true : false;
}

bool App_SharedError_IsErrorInList(struct ErrorList *error_list, enum ErrorId error_id)
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

bool App_SharedError_IsBoardInList(struct ErrorBoardList *board_list, enum Board board)
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
