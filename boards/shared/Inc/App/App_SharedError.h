#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "App_BoardEnum.h"

struct Error;

struct ErrorBoardList
{
    // Number of valid entries in "boards"
    uint32_t num_boards;

    // Only the first num_boards elements are valid
    enum Board boards[NUM_BOARDS];
};

enum ErrorId
{
    // BMS non-critical errors
    BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
    BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
    BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
    BMS_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
    BMS_NON_CRITICAL_WATCHDOG_TIMEOUT,

    // DCM non-critical errors
    DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
    DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
    DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
    DCM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
    DCM_NON_CRITICAL_WATCHDOG_TIMEOUT,

    // DIM non-critical errors
    DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
    DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
    DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
    DIM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
    DIM_NON_CRITICAL_WATCHDOG_TIMEOUT,

    // FSM non-critical errors
    FSM_NON_CRITICAL_PAPPS_OUT_OF_RANGE,
    FSM_NON_CRITICAL_SAPPS_OUT_OF_RANGE,
    FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
    FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
    FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
    FSM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
    FSM_NON_CRITICAL_WATCHDOG_TIMEOUT,
    FSM_NON_CRITICAL_BSPD_FAULT,
    FSM_NON_CRITICAL_LEFT_WHEEL_SPEED_OUT_OF_RANGE,
    FSM_NON_CRITICAL_RIGHT_WHEEL_SPEED_OUT_OF_RANGE,
    FSM_NON_CRITICAL_PRIMARY_FLOW_RATE_OUT_OF_RANGE,
    FSM_NON_CRITICAL_SECONDARY_FLOW_RATE_OUT_OF_RANGE,

    // PDM non-critical errors
    PDM_NON_CRITICAL_MISSING_HEARTBEAT,
    PDM_NON_CRITICAL_BOOST_PGOOD_FAULT,
    PDM_NON_CRITICAL_CELL_BALANCE_OVERVOLTAGE_FAULT,
    PDM_NON_CRITICAL_CHARGER_FAULT,
    PDM_NON_CRITICAL_EFUSE_FAULT,
    PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ,
    PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ,
    PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX,
    PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX,
    PDM_NON_CRITICAL_WATCHDOG_TIMEOUT,
    PDM_NON_CRITICAL_VBAT_VOLTAGE_OUT_OF_RANGE,
    PDM_NON_CRITICAL__24V_AUX_VOLTAGE_OUT_OF_RANGE,
    PDM_NON_CRITICAL__24V_ACC_VOLTAGE_OUT_OF_RANGE,
    PDM_NON_CRITICAL_AUX1_CURRENT_OUT_OF_RANGE,
    PDM_NON_CRITICAL_AUX2_CURRENT_OUT_OF_RANGE,
    PDM_NON_CRITICAL_LEFT_INVERTER_CURRENT_OUT_OF_RANGE,
    PDM_NON_CRITICAL_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE,
    PDM_NON_CRITICAL_ENERGY_METER_CURRENT_OUT_OF_RANGE,
    PDM_NON_CRITICAL_CAN_CURRENT_OUT_OF_RANGE,
    PDM_NON_CRITICAL_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE,

    // BMS critical errors
    BMS_CRITICAL_DUMMY,

    // DCM critical errors
    DCM_CRITICAL_DUMMY,

    // DIM critical errors
    DIM_CRITICAL_DUMMY,

    // FSM critical errors
    FSM_CRITICAL_DUMMY,

    // PDM critical errors
    PDM_CRITICAL_DUMMY,

    NUM_ERROR_IDS
};

struct ErrorList
{
    // Number of valid entries in "errors"
    uint32_t num_errors;

    // Only the first num_errors elements are valid
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
void App_SharedError_SetBoard(struct Error *error, enum Board board);

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
enum Board App_SharedError_GetBoard(const struct Error *error);

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
    enum Board             board);
