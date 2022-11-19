#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/App_PDMError.h"
//#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/App_PDMError.h"

#include "App_SharedExitCode.h"

enum ErrorId
{
    PDM_NON_CRITICAL_MISSING_HEARTBEAT,
    PDM_NON_CRITICAL_BOOST_PGOOD_FAULT,
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
    NUM_ERROR_IDS
};

struct PdmErrorTable
{
    size_t *errors[NUM_ERROR_IDS][2];
};

#define INIT_ERROR(id) App_PdmError_SetId(pdm_error_table->errors[id][0], id);

struct PdmErrorTable *App_PdmErrorTable_Create(void)
{
    struct PdmErrorTable *pdm_error_table = malloc(sizeof(struct PdmErrorTable));
    assert(pdm_error_table != NULL);

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        pdm_error_table->errors[i][0] = &i;
    }

    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        pdm_error_table->errors[i][1] = 0;
    }
    /*
    INIT_ERROR(PDM_NON_CRITICAL_MISSING_HEARTBEAT);
    INIT_ERROR(PDM_NON_CRITICAL_BOOST_PGOOD_FAULT);
    INIT_ERROR(PDM_NON_CRITICAL_CHARGER_FAULT);
    INIT_ERROR(PDM_NON_CRITICAL_EFUSE_FAULT);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1HZ);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASK1KHZ);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANRX);
    INIT_ERROR(PDM_NON_CRITICAL_STACK_WATERMARK_ABOVE_THRESHOLD_TASKCANTX);
    INIT_ERROR(PDM_NON_CRITICAL_WATCHDOG_TIMEOUT);
    INIT_ERROR(PDM_NON_CRITICAL_VBAT_VOLTAGE_OUT_OF_RANGE);
    INIT_ERROR(PDM_NON_CRITICAL__24V_AUX_VOLTAGE_OUT_OF_RANGE);
    INIT_ERROR(PDM_NON_CRITICAL__24V_ACC_VOLTAGE_OUT_OF_RANGE);
    INIT_ERROR(PDM_NON_CRITICAL_AUX1_CURRENT_OUT_OF_RANGE);
    INIT_ERROR(PDM_NON_CRITICAL_AUX2_CURRENT_OUT_OF_RANGE);
    INIT_ERROR(PDM_NON_CRITICAL_LEFT_INVERTER_CURRENT_OUT_OF_RANGE);
    INIT_ERROR(PDM_NON_CRITICAL_RIGHT_INVERTER_CURRENT_OUT_OF_RANGE);
    INIT_ERROR(PDM_NON_CRITICAL_ENERGY_METER_CURRENT_OUT_OF_RANGE);
    INIT_ERROR(PDM_NON_CRITICAL_CAN_CURRENT_OUT_OF_RANGE);
    INIT_ERROR(PDM_NON_CRITICAL_AIR_SHUTDOWN_CURRENT_OUT_OF_RANGE);
    */
    // INIT_ERROR(PDM_AIR_SHUTDOWN_DUMMY_AIR_SHUTDOWN, AIR_SHUTDOWN_ERROR);
    // INIT_ERROR(PDM_MOTOR_SHUTDOWN_DUMMY_MOTOR_SHUTDOWN, MOTOR_SHUTDOWN_ERROR);

    // clang-format on

    return pdm_error_table;
}

void App_PdmErrorTable_Destroy(struct PdmErrorTable *pdm_error_table)
{
    free(pdm_error_table);
}

void App_PdmErrorTable_FoundError(struct PdmErrorTable *pdm_error_table, size_t id)
{
    pdm_error_table->errors[id][1] = (size_t *)1;
}

bool App_PdmErrorTable_HasAnyErrors(struct PdmErrorTable *pdm_error_table)
{
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        if (pdm_error_table->errors[i][1] == (size_t *)1)
            return true;
    }
    return false;
}

void App_PdmErrorTable_GetAllErrors(struct PdmErrorTable *pdm_error_table, size_t all_error_array[NUM_ERROR_IDS])
{
    size_t count = 0;
    for (size_t i = 0; i < NUM_ERROR_IDS; i++)
    {
        if (pdm_error_table->errors[i][1] == (size_t *)1)
        {
            all_error_array[count] = i;
            count++;
        }
    }
}
