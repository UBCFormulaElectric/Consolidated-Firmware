#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_ErrorCode.h"

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

struct ErrorBoardList
{
    // Number of valid entries in "boards"
    uint32_t num_boards;

    // Storage large enough to store every board
    enum ErrorBoard boards[NUM_ERROR_BOARDS];
};

enum ErrorId
{
    BMS_NON_CRIT,
    DCM_NON_CRIT,
    DIM_NON_CRIT,
    FSM_NON_CRIT,
    PDM_NON_CRIT,
    BMS_CRIT,
    DCM_CRIT,
    DIM_CRIT,
    FSM_CRIT,
    PDM_CRIT,
    NUM_ERROR_IDS
};

struct ErrorList
{
    // Number of valid entries in "errors"
    uint32_t num_errors;

    // Storage large enough to store pointers to every error. Note that the
    // error pointers are borrowed from the error table that is being read. In
    // other words, these error pointers are alive only as long as the error
    // table that is being read.
    struct Error *errors[NUM_ERROR_IDS];
};

/**
 * Initialize and allocate an error table
 * @return The created error table, whose ownership is given to the caller
 */
struct ErrorTable *App_SharedErrorTable_CreateErrorTable(void);

/**
 * Deallocate the memory used by the given error table
 * @param error_table The error table to deallocate
 */
void App_SharedErrorTable_DestroyErrorTable(struct ErrorTable *error_table);

/**
 * Initialize and allocate an error list
 * @return The created error list, whose ownership is given to the caller
 */
struct ErrorList *App_SharedErrorTable_CreateErrorList(void);

/**
 * Deallocate the memory used by the given error list
 * @param error_list The error list to deallocate
 */
void App_SharedErrorTable_DestroyErrorList(struct ErrorList *error_list);


/**
 * Check if an error exists in the given list of error
 *
 */
bool App_SharedErrorTable_IsErrorInList(
    enum ErrorId      error_id,
    struct ErrorList *error_list);

/**
 * Check if a board exists in the given list of boards
 * @param boards The list of boards to check
 * @param num_boards The number of boards in the list of boards
 * @param board The board to check if exists in the list of boards
 * @return true if the board exists in the given list of boards, else false
 */
bool App_SharedErrorTable_IsBoardInList(
    enum ErrorBoard        board,
    struct ErrorBoardList *board_list);

/**
 * Set or clear an error in the given error table
 * @param error_table The error table to set or clear an error
 * @param error_id The ID for the error to set or clear
 * @param is_set true to set the error, or false to clear the error
 */
ExitCode App_SharedErrorTable_SetError(
    struct ErrorTable *error_table,
    enum ErrorId       error_id,
    bool               is_set);

/**
 * Check if an error in the given error table is set
 * @param error_table The error table to check
 * @param error_id The ID for the error to check
 * @return true if the error is set, else false
 */
ExitCode App_SharedErrorTable_IsErrorSet(
    const struct ErrorTable *error_table,
    enum ErrorId             error_id,
    bool *                   is_set);

/**
 * Check if any error in the given error table is set
 * @param error_table The error table to check
 * @return true if any error in the given error table is set, else false
 */
bool App_SharedErrorTable_HasError(const struct ErrorTable *error_table);

/**
 * Check if any critical error in the given error table is set
 * @param error_table The error table to check
 * @return true if any critical error in the given error table is set,
 *         else false
 */
bool App_SharedErrorTable_HasCriticalError(
    const struct ErrorTable *error_table);

/**
 * Check if any non-critical error in the given error table is set
 * @param error_table The error table to check
 * @return true if any non-critical error in the given error table is set,
 *         else false
 */
bool App_SharedErrorTable_HasNonCriticalError(
    const struct ErrorTable *error_table);

/**
 * Get every error that is set in the given error table
 * @param error_table The error table to get errors from
 * @param errors This array will be set to contain every error that is set in
 *               the given error table
 * @param num_errors This will be set to the number of errors stored in errors
 */
void App_SharedErrorTable_GetAllErrors(
    struct ErrorTable *error_table,
    struct ErrorList * error_list);

/**
 * Get every critical error that is set in the given error table
 * @param error_table The error table to get critical errors from
 * @param errors This array will be set to contain every critical error that is
 *               set in the given error table
 * @param num_errors This will be set to the number of critical errors stored in
 *                   errors
 */
void App_SharedErrorTable_GetAllCriticalErrors(
    struct ErrorTable *error_table,
    struct ErrorList * error_list);

/**
 * Get every non-critical error that is set in the given error table
 * @param error_table The error table to get non-critical errors from
 * @param errors This array will be set to contain every non-critical error that
 *               is set in the given error table
 * @param num_errors This will be set to the number of non-critical errors
 *                   stored in errors
 */
void App_SharedErrorTable_GetAllNonCriticalErrors(
    struct ErrorTable *error_table,
    struct ErrorList * error_list);

/**
 * Get every board that does not contain any errors in the given error table
 * @param error_table The error table to check
 * @param board_list This will be set to contain every board that does not
 *                   contain any errors in the given error table
 */
void App_SharedErrorTable_GetBoardsWithNoErrors(
    const struct ErrorTable *error_table,
    struct ErrorBoardList *  board_list);

/**
 * Get every board that contains one or more errors in the given error table
 * @param error_table The error table to check
 * @param board_list This will be set to contain every board that contains one
 *                   or more errors in the given error table
 */
void App_SharedErrorTable_GetBoardsWithErrors(
    const struct ErrorTable *error_table,
    struct ErrorBoardList *  board_list);

/**
 * Get every board that contains one or more critical errors in the given error
 * table
 * @param error_table The error table to check
 * @param board_list This will be set to contain every board that contains one
 *                   or more critical errors in the given error table
 */
void App_SharedErrorTable_GetBoardsWithCriticalErrors(
    const struct ErrorTable *error_table,
    struct ErrorBoardList *  board_list);

/**
 * Get every board that contains one or more non-critical errors in the given
 * error table
 * @param error_table The error table to check
 * @param board_list This will be set to contain every board that contains one
 *                   or more non-critical errors in the given error table
 */
void App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
    const struct ErrorTable *error_table,
    struct ErrorBoardList *  board_list);

/**
 * Get the board that the given error belongs to
 * @param error The error to check
 * @return The board that the given error belongs to
 */
enum ErrorBoard App_SharedError_GetBoard(const struct Error *error);

/**
 * Check if the given error is critical
 * @param error The error to check
 * @return true if the given error is critical, else false
 */
bool App_SharedError_IsCritical(const struct Error *error);

/**
 * Get the ID of the given error
 * @return The ID of the given error
 */
uint32_t App_SharedError_GetId(const struct Error* error);
