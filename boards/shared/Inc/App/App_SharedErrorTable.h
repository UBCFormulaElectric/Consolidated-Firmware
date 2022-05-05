#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "App_SharedExitCode.h"
#include "App_SharedError.h"

struct ErrorTable;

/**
 * Initialize and allocate an error table
 * @return The created error table, whose ownership is given to the caller
 */
struct ErrorTable *App_SharedErrorTable_Create(void);

/**
 * Deallocate the memory used by the given error table
 * @param error_table The error table to deallocate
 */
void App_SharedErrorTable_Destroy(struct ErrorTable *error_table);

/**
 * Set or clear an error in the given error table
 * @param error_table The error table to set or clear an error
 * @param error_id The ID for the error to set or clear
 * @param is_set true to set the error, or false to clear the error
 */
ExitCode App_SharedErrorTable_SetError(struct ErrorTable *error_table, enum ErrorId error_id, bool is_set);

/**
 * Check if an error in the given error table is set
 * @param error_table The error table to check
 * @param error_id The ID for the error to check
 * @return true if the error is set, else false
 */
ExitCode App_SharedErrorTable_IsErrorSet(const struct ErrorTable *error_table, enum ErrorId error_id, bool *is_set);

/**
 * Check if any error in the given error table is set
 * @param error_table The error table to check
 * @return true if any error in the given error table is set, else false
 */
bool App_SharedErrorTable_HasAnyErrorSet(const struct ErrorTable *error_table);

/**
 * Check if any critical error in the given error table is set
 * @param error_table The error table to check
 * @return true if any critical error in the given error table is set,
 *         else false
 */
bool App_SharedErrorTable_HasAnyCriticalErrorSet(const struct ErrorTable *error_table);

/**
 * Check if any AIR shutdown error in the given error table is set
 * @param error_table The error table to check
 * @return true if any AIR shutdown error in the given error table is set,
 *         else false
 */
bool App_SharedErrorTable_HasAnyAirShutdownErrorSet(const struct ErrorTable *error_table);

/**
 * Check if any motor shutdown error in the given error table is set
 * @param error_table The error table to check
 * @return true if any motor shutdown error in the given error table is set,
 *         else false
 */
bool App_SharedErrorTable_HasAnyMotorShutdownErrorSet(const struct ErrorTable *error_table);

/**
 * Check if any non-critical error in the given error table is set
 * @param error_table The error table to check
 * @return true if any non-critical error in the given error table is set,
 *         else false
 */
bool App_SharedErrorTable_HasAnyNonCriticalErrorSet(const struct ErrorTable *error_table);

/**
 * Get every error that is set in the given error table
 * @param error_table The error table to get errors from
 * @param error_list This will be set to contain every error that is set in the
 *                   given error table
 */
void App_SharedErrorTable_GetAllErrors(struct ErrorTable *error_table, struct ErrorList *error_list);

/**
 * Get every critical error that is set in the given error table
 * @param error_table The error table to get critical errors from
 * @param error_list This will be set to contain every critical error that is
 *                   set in the given error table
 */
void App_SharedErrorTable_GetAllCriticalErrors(struct ErrorTable *error_table, struct ErrorList *error_list);

/**
 * Get every non-critical error that is set in the given error table
 * @param error_table The error table to get non-critical errors from
 * @param error_list This will be set to contain every non-critical error that
 *                   is set in the given error table
 */
void App_SharedErrorTable_GetAllNonCriticalErrors(struct ErrorTable *error_table, struct ErrorList *error_list);

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
void App_SharedErrorTable_GetBoardsWithErrors(const struct ErrorTable *error_table, struct ErrorBoardList *board_list);

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
