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

struct ErrorTable* App_ErrorTable_Create(void);
void App_ErrorTable_Destroy(struct ErrorTable* error_table);

uint32_t App_ErrorTable_HasNonCriticalError(struct ErrorTable* error_table, struct Error errors[]);
uint32_t App_ErrorTable_HasCriticalError(struct ErrorTable* error_table, struct Error errors[]);

void App_ErrorTable_SetBmsStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);
void App_ErrorTable_ClearBmsStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);

void App_ErrorTable_SetDcmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);
void App_ErrorTable_ClearDcmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);

void App_ErrorTable_SetFsmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);
void App_ErrorTable_ClearFsmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);

void App_ErrorTable_SetPdmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);
void App_ErrorTable_ClearPdmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);

void App_ErrorTable_SetDimStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);
void App_ErrorTable_ClearDimStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table);

void App_ErrorTable_SetBmsDummyCriticalError(struct ErrorTable* error_table);
void App_ErrorTable_ClearBmsDummyCriticalError(struct ErrorTable* error_table);

void App_ErrorTable_SetDcmDummyCriticalError(struct ErrorTable* error_table);
void App_ErrorTable_ClearDcmDummyCriticalError(struct ErrorTable* error_table);

void App_ErrorTable_SetFsmDummyCriticalError(struct ErrorTable* error_table);
void App_ErrorTable_ClearFsmDummyCriticalError(struct ErrorTable* error_table);

void App_ErrorTable_SetPdmDummyCriticalError(struct ErrorTable* error_table);
void App_ErrorTable_ClearPdmDummyCriticalError(struct ErrorTable* error_table);

void App_ErrorTable_SetDimDummyCriticalError(struct ErrorTable* error_table);
void App_ErrorTable_ClearDimDummyCriticalError(struct ErrorTable* error_table);

