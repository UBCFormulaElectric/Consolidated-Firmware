#pragma once

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

enum ErrorId
{
    BMS_FOO,
    DCM_BAR,
    FSM_BAZ,
    NUM_ERROR_IDS
};

/**
 * Initialize and allocate an error table
 e @return The created error table, whose ownership is given to the caller
 */
struct ErrorTable *App_SharedErrorTable_Create(void);

/**
 * Deallocate the memory used by the given error table
 * @param error_table The error table to deallocate
 */
void App_SharedErrorTable_Destroy(struct ErrorTable *error_table);

/**
 * Check if a board exists in the given list of boards
 * @param boards The list of boards to check
 * @param num_boards The number of boards in the list of boards
 * @param board The board to check if exists in the list of boards
 * @return true if the board exists in the given list of boards, else false
 */
bool App_SharedErrorTable_IsBoardInList(
    enum ErrorBoard boards[],
    int             num_boards,
    enum ErrorBoard board);

/**
 * Get the board that the given error belongs to
 * @param error The error to check
 * @return The board that the given error belongs to
 */
enum ErrorBoard
    App_SharedErrorTable_GetBoardForError(const struct Error *error);

/**
 * Check if the given error is critical
 * @param error The error to check
 * @return true if the given error is critical, else false
 */
bool App_SharedErrorTable_IsErrorCritical(const struct Error *error);

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
    const struct ErrorTable *error_table,
    struct Error *           errors,
    uint32_t *               num_errors);

/**
 * Get every critical error that is set in the given error table
 * @param error_table The error table to get critical errors from
 * @param errors This array will be set to contain every critical error that is
 *               set in the given error table
 * @param num_errors This will be set to the number of critical errors stored in
 *                   errors
 */
void App_SharedErrorTable_GetAllCriticalErrors(
    const struct ErrorTable *error_table,
    struct Error *           errors,
    uint32_t *               num_errors);

/**
 * Get every non-critical error that is set in the given error table
 * @param error_table The error table to get non-critical errors from
 * @param errors This array will be set to contain every non-critical error that
 *               is set in the given error table
 * @param num_errors This will be set to the number of non-critical errors
 *                   stored in errors
 */
void App_SharedErrorTable_GetAllNonCriticalErrors(
    const struct ErrorTable *error_table,
    struct Error *           errors,
    uint32_t *               num_errors);

/**
 * Get every board that does not contain any errors in the given error table
 * @param error_table The error table to check
 * @param boards This array will be set to contain every board that does not
 *               contain any errors in the given error table
 * @param num_boards This will be set to the number of boards stored in boards
 */
void App_SharedErrorTable_GetBoardsWithNoErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    uint32_t *               num_boards);

/**
 * Get every board that contains one or more errors in the given error table
 * @param error_table The error table to check
 * @param boards This array will be set to contain every board that contains
 *               one or more errors in the given error table
 * @param num_boards This will be set to the number of boards stored in boards
 */
void App_SharedErrorTable_GetBoardsWithErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    uint32_t *               num_boards);

/**
 * Get every board that contains one or more critical errors in the given error
 * table
 * @param error_table The error table to check
 * @param boards This array will be set to contain every board that contains
 *               one or more critical errors in the given error table
 * @param num_boards This will be set to the number of boards stored in boards
 */
void App_SharedErrorTable_GetBoardsWithCriticalErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    uint32_t *               num_boards);

/**
 * Get every board that contains one or more non-critical errors in the given
 * error table
 * @param error_table The error table to check
 * @param boards This array will be set to contain every board that contains
 *               one or more non-critical errors in the given error table
 * @param num_boards This will be set to the number of boards stored in boards
 */
void App_SharedErrorTable_GetBoardsWithNonCriticalErrors(
    const struct ErrorTable *error_table,
    enum ErrorBoard          boards[],
    uint32_t *               num_boards);
