#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "App_ErrorTable.h"

// clang-format off
struct ErrorTable
{
    bool bms_stack_watermark_above_threshold_task1hz;
    bool dcm_stack_watermark_above_threshold_task1hz;
    bool fsm_stack_watermark_above_threshold_task1hz;
    bool pdm_stack_watermark_above_threshold_task1hz;
    bool dim_stack_watermark_above_threshold_task1hz;
    bool bms_dummy_critical_error;
    bool dcm_dummy_critical_error;
    bool fsm_dummy_critical_error;
    bool pdm_dummy_critical_error;
    bool dim_dummy_critical_error;
};

struct ErrorTable* App_ErrorTable_Create(void)
{
    struct ErrorTable* error_table = malloc(sizeof(struct ErrorTable));
    assert(error_table != NULL);

    return error_table;
}

void App_ErrorTable_Destroy(struct ErrorTable* error_table)
{
    free(error_table);
}

uint32_t App_ErrorTable_HasNonCriticalError(struct ErrorTable* error_table, struct Error errors[])
{
    uint32_t num_errors = 0;

    if (error_table->bms_stack_watermark_above_threshold_task1hz)
    {
        errors[num_errors].error_id = BMS_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ;
        errors[num_errors].std_id = 104;
        num_errors++;
    }

    if (error_table->dcm_stack_watermark_above_threshold_task1hz)
    {
        errors[num_errors].error_id = DCM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ;
        errors[num_errors].std_id = 204;
        num_errors++;
    }

    if (error_table->fsm_stack_watermark_above_threshold_task1hz)
    {
        errors[num_errors].error_id = FSM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ;
        errors[num_errors].std_id = 300;
        num_errors++;
    }

    if (error_table->pdm_stack_watermark_above_threshold_task1hz)
    {
        errors[num_errors].error_id = PDM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ;
        errors[num_errors].std_id = 400;
        num_errors++;
    }

    if (error_table->dim_stack_watermark_above_threshold_task1hz)
    {
        errors[num_errors].error_id = DIM_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ;
        errors[num_errors].std_id = 508;
        num_errors++;
    }

    return num_errors;
}

uint32_t App_ErrorTable_HasCriticalError(struct ErrorTable* error_table, struct Error errors[])
{
    uint32_t num_errors = 0;

    if (error_table->bms_dummy_critical_error)
    {
        errors[num_errors].error_id = BMS_CRITICAL_ERROR_DUMMY_CRITICAL_ERROR;
        errors[num_errors].std_id = 109;
        num_errors++;
    }

    if (error_table->dcm_dummy_critical_error)
    {
        errors[num_errors].error_id = DCM_CRITICAL_ERROR_DUMMY_CRITICAL_ERROR;
        errors[num_errors].std_id = 207;
        num_errors++;
    }

    if (error_table->fsm_dummy_critical_error)
    {
        errors[num_errors].error_id = FSM_CRITICAL_ERROR_DUMMY_CRITICAL_ERROR;
        errors[num_errors].std_id = 309;
        num_errors++;
    }

    if (error_table->pdm_dummy_critical_error)
    {
        errors[num_errors].error_id = PDM_CRITICAL_ERROR_DUMMY_CRITICAL_ERROR;
        errors[num_errors].std_id = 414;
        num_errors++;
    }

    if (error_table->dim_dummy_critical_error)
    {
        errors[num_errors].error_id = DIM_CRITICAL_ERROR_DUMMY_CRITICAL_ERROR;
        errors[num_errors].std_id = 509;
        num_errors++;
    }

    return num_errors;
}

void App_ErrorTable_SetBmsStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table)
{
    error_table->bms_stack_watermark_above_threshold_task1hz = true;
}

void App_ErrorTable_ClearBmsStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table)
{
    error_table->bms_stack_watermark_above_threshold_task1hz = false;
}

void App_ErrorTable_SetDcmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table)
{
    error_table->dcm_stack_watermark_above_threshold_task1hz = true;
}

void App_ErrorTable_ClearDcmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table)
{
    error_table->dcm_stack_watermark_above_threshold_task1hz = false;
}

void App_ErrorTable_SetFsmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table)
{
    error_table->fsm_stack_watermark_above_threshold_task1hz = true;
}

void App_ErrorTable_ClearFsmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table)
{
    error_table->fsm_stack_watermark_above_threshold_task1hz = false;
}

void App_ErrorTable_SetPdmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table)
{
    error_table->pdm_stack_watermark_above_threshold_task1hz = true;
}

void App_ErrorTable_ClearPdmStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table)
{
    error_table->pdm_stack_watermark_above_threshold_task1hz = false;
}

void App_ErrorTable_SetDimStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table)
{
    error_table->dim_stack_watermark_above_threshold_task1hz = true;
}

void App_ErrorTable_ClearDimStackWaterMarkAboveThresholdTask1Hz(struct ErrorTable* error_table)
{
    error_table->dim_stack_watermark_above_threshold_task1hz = false;
}

void App_ErrorTable_SetBmsDummyCriticalError(struct ErrorTable* error_table)
{
    error_table->bms_dummy_critical_error = true;
}

void App_ErrorTable_ClearBmsDummyCriticalError(struct ErrorTable* error_table)
{
    error_table->bms_dummy_critical_error = false;
}

void App_ErrorTable_SetDcmDummyCriticalError(struct ErrorTable* error_table)
{
    error_table->dcm_dummy_critical_error = true;
}

void App_ErrorTable_ClearDcmDummyCriticalError(struct ErrorTable* error_table)
{
    error_table->dcm_dummy_critical_error = false;
}

void App_ErrorTable_SetFsmDummyCriticalError(struct ErrorTable* error_table)
{
    error_table->fsm_dummy_critical_error = true;
}

void App_ErrorTable_ClearFsmDummyCriticalError(struct ErrorTable* error_table)
{
    error_table->fsm_dummy_critical_error = false;
}

void App_ErrorTable_SetPdmDummyCriticalError(struct ErrorTable* error_table)
{
    error_table->pdm_dummy_critical_error = true;
}

void App_ErrorTable_ClearPdmDummyCriticalError(struct ErrorTable* error_table)
{
    error_table->pdm_dummy_critical_error = false;
}

void App_ErrorTable_SetDimDummyCriticalError(struct ErrorTable* error_table)
{
    error_table->dim_dummy_critical_error = true;
}

void App_ErrorTable_ClearDimDummyCriticalError(struct ErrorTable* error_table)
{
    error_table->dim_dummy_critical_error = false;
}
