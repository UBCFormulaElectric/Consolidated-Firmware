#pragma once

#include <stdbool.h>
#include <stdint.h>

struct Error;
struct ErrorList;
struct ErrorBoardList;

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
 * Initialize and allocate an error
 * @return The created error, whose ownership is given to the caller
 */
struct Error *App_SharedError_Create(void);

/**
 * Deallocate the memory used by the given error
 * @param error The error to deallocate
 */
void App_SharedError_Destroy(struct Error *error);

/**
 * Set the board for the given error
 * @param error The error to set
 * @param board The board to set
 */
void App_SharedError_SetBoard(struct Error *error, enum ErrorBoard board);

/**
 * Set the given error as non-critical or critical
 * @param error The error to set
 * @param is_critical true to set the error as critical, or false to set the
 *                    error as non-critical
 */
void App_SharedError_SetIsCritical(struct Error *error, bool is_critical);

/**
 * Set the ID for the given error
 * @param error The error to set
 * @param id The ID to set the error to
 */
void App_SharedError_SetId(struct Error *error, uint32_t id);

/**
 * Set or clear the given error
 * @param error The error to set
 * @param is_set true to set the error, or false to clear the error
 */
void App_SharedError_SetIsSet(struct Error *error, bool is_set);

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
bool App_SharedError_GetIsCritical(const struct Error *error);

/**
 * Get the ID of the given error
 * @return The ID of the given error
 */
uint32_t App_SharedError_GetId(const struct Error *error);

/**
 * Check if the given error is set
 * @param error The error to get
 * @return true if the error is set, else false
 */
bool App_SharedError_GetIsSet(const struct Error *error);

/**
 * Initialize and allocate an error list
 * @return The created error list, whose ownership is given to the caller
 */
struct ErrorList *App_SharedError_CreateErrorList(void);

/**
 * Deallocate the memory used by the given error list
 * @param error_list The error list to deallocate
 */
void App_SharedError_DestroyErrorList(struct ErrorList *error_list);

/**
 * Check if the given error exists in the given error list
 * @param error_list The error list to check
 * @param error_id The ID for the error to check
 * @return true if the given error exists in the given list of error
 */
bool App_SharedError_IsErrorInList(
    struct ErrorList *error_list,
    enum ErrorId      error_id);

/**
 * Check if a board exists in the given board list
 * @param board_list The board list to check
 * @param board The board to check
 * @return true if the board exists in the given board list, else false
 */
bool App_SharedError_IsBoardInList(
    struct ErrorBoardList *board_list,
    enum ErrorBoard        board);
