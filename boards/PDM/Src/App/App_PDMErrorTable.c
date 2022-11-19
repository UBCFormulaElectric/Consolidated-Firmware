#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/App_PDMError.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/App_PdmErrorTable.h"
#include "App_SharedExitCode.h"

struct PdmErrorTable
{
    struct PdmError *errors[NUM_ERROR_IDS];
};

#define INIT_ERROR(id, error_type)                       \
    App_PdmError_SetId(pdm_error_table->errors[id], id); \
    App_PdmError_SetErrorType(pdm_error_table->errors[id], error_type);

struct PdmErrorTable *App_PdmErrorTable_Create(void)
{
    struct PdmErrorTable *pdm_error_table = malloc(sizeof(struct PdmErrorTable));
    assert(pdm_error_table != NULL);

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        pdm_error_table->errors[i] = App_PdmError_Create();
    }

    INIT_ERROR(PDM_NON_CRITICAL_MISSING_HEARTBEAT, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_BOOST_PGOOD_FAULT, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_CHARGER_FAULT, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_EFUSE_FAULT, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_WATCHDOG_TIMEOUT, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_VBAT_VOLTAGE_OUT_OF_RANGE, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL__24V_AUX_VOLTAGE_OUT_OF_RANGE, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL__24V_ACC_VOLTAGE_OUT_OF_RANGE, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_AUX1_CURRENT_OUT_OF_RANGE, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_AUX2_CURRENT_OUT_OF_RANGE, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_LEFT_INVERTER_CURRENT_OUT_OF_RANGE, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_ENERGY_METER_CURRENT_OUT_OF_RANGE, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_CAN_CURRENT_OUT_OF_RANGE, NON_CRITICAL_ERROR);
    INIT_ERROR(PDM_NON_CRITICAL_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE, NON_CRITICAL_ERROR);

    // INIT_ERROR(PDM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, AIR_SHUTDOWN_ERROR);
    // INIT_ERROR(PDM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, MOTOR_SHUTDOWN_ERROR);

    // clang-format on

    return pdm_error_table;
}

void App_PdmErrorTable_Destroy(struct PdmErrorTable *pdm_error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        App_PdmError_Destroy(pdm_error_table->errors[i]);
    }

    free(pdm_error_table);
}

ExitCode App_PdmErrorTable_SetError(struct PdmErrorTable *pdm_error_table, enum ErrorId error_id, bool is_set)
{
    if ((int)error_id >= NUM_ERROR_IDS)
    {
        return EXIT_CODE_OUT_OF_RANGE;
    }
    struct PdmError *pdm_error = pdm_error_table->errors[error_id];
    App_PdmError_SetIsSet(pdm_error, is_set);

    return EXIT_CODE_OK;
}

ExitCode App_PdmErrorTable_IsErrorSet(const struct PdmErrorTable *pdm_error_table, enum ErrorId error_id, bool *is_set)
{
    if (error_id >= NUM_ERROR_IDS)
    {
        return EXIT_CODE_OUT_OF_RANGE;
    }

    *is_set = App_PdmError_GetIsSet(pdm_error_table->errors[error_id]);
    return EXIT_CODE_OK;
}

bool App_PdmErrorTable_HasAnyErrorSet(const struct PdmErrorTable *pdm_error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        if (App_PdmError_GetIsSet(pdm_error_table->errors[i]))
        {
            return true;
        }
    }
    return false;
}

bool App_PdmErrorTable_HasAnyCriticalErrorSet(const struct PdmErrorTable *pdm_error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = pdm_error_table->errors[i];
        if (App_PdmError_IsCritical(error) && App_PdmError_GetIsSet(error))
        {
            return true;
        }
    }
    return false;
}

bool App_PdmErrorTable_HasAnyAirShutdownErrorSet(const struct PdmErrorTable *pdm_error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct PdmError *pdm_error = pdm_error_table->errors[i];
        if (App_PdmError_GetErrorType(pdm_error) == AIR_SHUTDOWN_ERROR && App_PdmError_GetIsSet(pdm_error))
        {
            return true;
        }
    }
    return false;
}

bool App_PdmErrorTable_HasAnyMotorShutdownErrorSet(const struct PdmErrorTable *pdm_error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = pdm_error_table->errors[i];
        if (App_PdmError_GetErrorType(error) == MOTOR_SHUTDOWN_ERROR && App_PdmError_GetIsSet(error))
        {
            return true;
        }
    }
    return false;
}

bool App_PdmErrorTable_HasAnyNonCriticalErrorSet(const struct PdmErrorTable *pdm_error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        const struct Error *error = pdm_error_table->errors[i];
        if (App_PdmError_IsNonCritical(error) && App_PdmError_GetIsSet(error))
        {
            return true;
        }
    }
    return false;
}

void App_PdmErrorTable_GetAllErrors(struct PdmErrorTable *pdm_error_table, struct ErrorList *error_list)
{
    error_list->num_errors = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        struct Error *error = pdm_error_table->errors[i];

        if (App_PdmError_GetIsSet(error))
        {
            error_list->errors[error_list->num_errors] = error;
            error_list->num_errors++;
        }
    }
}

void App_PdmErrorTable_GetAllCriticalErrors(struct PdmErrorTable *pdm_error_table, struct ErrorList *error_list)
{
    error_list->num_errors = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        struct Error *error = pdm_error_table->errors[i];

        if (App_PdmError_GetIsSet(error) && App_PdmError_IsCritical(error))
        {
            error_list->errors[error_list->num_errors] = error;
            error_list->num_errors++;
        }
    }
}

void App_PdmErrorTable_GetAllNonCriticalErrors(struct PdmErrorTable *pdm_error_table, struct ErrorList *error_list)
{
    error_list->num_errors = 0;

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        struct PdmError *pdm_error = pdm_error_table->errors[i];

        if (App_PdmError_GetIsSet(pdm_error) && App_PdmError_IsNonCritical(pdm_error))
        {
            error_list->errors[error_list->num_errors] = pdm_error;
            error_list->num_errors++;
        }
    }
}
