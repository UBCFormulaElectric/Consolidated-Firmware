#pragma once

#include <stdbool.h>

struct ErrorTable;
struct Error;
struct CanMsg;

enum ErrorBoard
{
    BMS,
    DCM,
    DIM,
    FSM,
    PDM,
    NUM_ERROR_BOARDS,
};

enum ErrorId
{
    BMS_FOO,
    DCM_BAR,
    FSM_BAZ,
    NUM_ERROR_IDS
};

struct ErrorTable *App_SharedErrorTable_Create(void);
void               App_SharedErrorTable_Destroy(struct ErrorTable *error_table);

bool App_SharedErrorTable_IsBoardInList(
    enum ErrorBoard boards[],
    int             num_boards,
    enum ErrorBoard board);

enum ErrorBoard
     App_SharedErrorTable_GetBoardForError(const struct Error *error);
bool App_SharedErrorTable_IsErrorCritical(const struct Error *error);

bool App_SharedErrorTable_HasError(const struct ErrorTable *error_table);
bool App_SharedErrorTable_HasCriticalError(
    const struct ErrorTable *error_table);
bool App_SharedErrorTable_HasNonCriticalError(
    const struct ErrorTable *error_table);

void App_SharedErrorTable_GetAllCriticalErrors(
    const struct ErrorTable *error_table,
    struct Error *           errors,
    int *                    num_errors);
void App_SharedErrorTable_GetAllErrors(
    const struct ErrorTable *error_table,
    struct Error *           errors,
    int *                    num_errors);
void App_SharedErrorTable_GetAllNonCriticalErrors(
    const struct ErrorTable *error_table,
    struct Error *           errors,
    int *                    num_errors);

void App_SharedErrorTable_GetBoardsWithNoErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    int *                    num_boards);
void App_SharedErrorTable_GetBoardsWithErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    int *                    num_boards);
void App_SharedErrorTable_GetBoardsWithCriticalErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    int *                    num_boards);
void App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    int *                    num_boards);

bool App_SharedErrorTable_IsErrorSet(
    const struct ErrorTable *error_table,
    enum ErrorId             error_id);

void App_SharedErrorTable_SetErrorsBasedOnCanMsg(
    struct ErrorTable *error_table,
    struct CanMsg *    can_msg);