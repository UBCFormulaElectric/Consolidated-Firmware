#pragma once

#include <stdint.h>

// clang-format off
enum ErrorId
{
    BMS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ = 0,
    BMS_CRITICAL_ERROR_DUMMY_CRITICAL_ERROR = 1,

    DCM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ = 0,
    DCM_CRITICAL_ERROR_DUMMY_CRITICAL_ERROR = 1,

    FSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ = 0,
    FSM_CRITICAL_ERROR_DUMMY_CRITICAL_ERROR = 1,

    PDM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ = 0,
    PDM_CRITICAL_ERROR_DUMMY_CRITICAL_ERROR = 1,

    DIM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ = 0,
    DIM_CRITICAL_ERROR_DUMMY_CRITICAL_ERROR = 1,

    NUM_ERRORS = 10, // Number of non-critical and critical errors combined
                     // across ALL boards
};

struct Error
{
    // CAN message ID
    uint32_t std_id;
    // Each error ID is unique in the error table
    enum ErrorId error_id;
};

struct ErrorTable* App_SharedErrorTable_Create(void);
void App_SharedErrorTable_Destroy(struct ErrorTable* error_table);

/**
 * Check if there is any non-critical errors in the given error table, and if
 * necessary, write the non-critical errors into the given error array
 * @note The array must hold up to NUM_ERRORS of elements
 * @param error_table The error table
 * @param errors Any non-critical errors will be set in this error array
 * @return The number of non-critical errors
 */
uint32_t App_SharedErrorTable_HasNonCriticalError(struct ErrorTable* error_table, struct Error errors[]);

/**
 * Check if there is any critical errors in the given error table, and if
 * necessary, write the critical errors into the given error array
 * @note The array must hold up to NUM_ERRORS of elements
 * @param error_table The error table
 * @param errors Any critical errors will be set in this error array
 * @return The number of critical errors
 */
uint32_t App_SharedErrorTable_HasCriticalError(struct ErrorTable* error_table, struct Error errors[]);

void App_SharedErrorTable_SetBmsStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);
void App_SharedErrorTable_ClearBmsStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);

void App_SharedErrorTable_SetDcmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);
void App_SharedErrorTable_ClearDcmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);

void App_SharedErrorTable_SetFsmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);
void App_SharedErrorTable_ClearFsmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);

void App_SharedErrorTable_SetPdmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);
void App_SharedErrorTable_ClearPdmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);

void App_SharedErrorTable_SetDimStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);
void App_SharedErrorTable_ClearDimStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);

void App_SharedErrorTable_SetBmsDummyCriticalError(struct ErrorTable* error_table);
void App_SharedErrorTable_ClearBmsDummyCriticalError(struct ErrorTable* error_table);

void App_SharedErrorTable_SetDcmDummyCriticalError(struct ErrorTable* error_table);
void App_SharedErrorTable_ClearDcmDummyCriticalError(struct ErrorTable* error_table);

void App_SharedErrorTable_SetFsmDummyCriticalError(struct ErrorTable* error_table);
void App_SharedErrorTable_ClearFsmDummyCriticalError(struct ErrorTable* error_table);

void App_SharedErrorTable_SetPdmDummyCriticalError(struct ErrorTable* error_table);
void App_SharedErrorTable_ClearPdmDummyCriticalError(struct ErrorTable* error_table);

void App_SharedErrorTable_SetDimDummyCriticalError(struct ErrorTable* error_table);
void App_SharedErrorTable_ClearDimDummyCriticalError(struct ErrorTable* error_table);

