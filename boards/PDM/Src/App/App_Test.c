
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include "../../Inc/App/App_PDMError.h"

INIT_ERROR(PDM_NON_CRITICAL_MISSING_HEARTBEAT, );
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

struct PdmError
{
    enum ErrorId id;
    // The type of this error
};

struct PdmError *App_PdmError_Create(void)
{
    struct PdmError *pdm_error = malloc(sizeof(struct PdmError));
    assert(pdm_error != NULL);

    pdm_error->id = NUM_ERROR_IDS;

    return pdm_error;
}

void App_PdmError_Destroy(struct PdmError *pdm_error)
{
    free(pdm_error);
}

void App_PdmError_SetId(struct PdmError *pdm_error, uint32_t id)
{
    pdm_error->id = id;
}

uint32_t App_PdmError_GetId(const struct PdmError *pdm_error)
{
    return pdm_error->id;
}

bool App_PdmError_IsErrorInList(struct ErrorList *error_list, enum ErrorId error_id)
{
    for (uint32_t i = 0; i < error_list->num_errors; i++)
    {
        if (App_PdmError_GetId(error_list->errors[i]) == error_id)
        {
            return true;
        }
    }
    return false;
}
