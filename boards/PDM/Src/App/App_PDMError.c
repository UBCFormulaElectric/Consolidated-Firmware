#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/App_RailMonitoring.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/App_PDMError.h"
//#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/App_PDMError.h"

#include "App_SharedExitCode.h"
/*
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
*/
struct PdmErrorTable
{
    size_t *errors[NUM_ERROR_IDS][2];
    bool (*VBAT_check);
    bool (*__24V_ACC_check);
    bool (*__22V_AUX_check);
    bool (*efuse1_channel0_check);
    bool (*efuse1_channel1_check);
    bool (*efuse2_channel0_check);
    bool (*efuse2_channel1_check);
    bool (*efuse3_channel0_check);
    bool (*efuse3_channel1_check);
    bool (*efuse4_channel0_check);
    bool (*efuse4_channel1_check);
};

#define INIT_ERROR(id, status) \
    App_PdmErrorTable_CheckErrors(pdm_error_table->errors, id, status);

struct PdmErrorTable *App_PdmErrorTable_Create(
        bool (*VBAT_check),
        bool (*__24V_ACC_check),
        bool (*__22V_AUX_check),
        bool (*efuse1_channel0_check),
        bool (*efuse1_channel1_check),
        bool (*efuse2_channel0_check),
        bool (*efuse2_channel1_check),
        bool (*efuse3_channel0_check),
        bool (*efuse3_channel1_check),
        bool (*efuse4_channel0_check),
        bool (*efuse4_channel1_check));
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

    pdm_error_table->VBAT_check = VBAT_check;
    pdm_error_table->__24V_ACC_check = __24V_ACC_check;
    pdm_error_table->__22V_AUX_check= __22V_AUX_check;

    pdm_error_table->efuse1_channel0_check = efuse1_channel0_check;
    pdm_error_table->efuse1_channel1_check = efuse1_channel1_check;
    pdm_error_table->efuse2_channel0_check = efuse2_channel0_check;
    pdm_error_table->efuse2_channel1_check = efuse2_channel1_check;
    pdm_error_table->efuse3_channel0_check = efuse3_channel0_check;
    pdm_error_table->efuse3_channel1_check = efuse3_channel1_check;
    pdm_error_table->efuse4_channel0_check = efuse4_channel0_check;
    pdm_error_table->efuse4_channel1_check = efuse4_channel1_check;


    /*
    INIT_ERROR(PDM_NON_CRITICAL_VBAT_VOLTAGE_OUT_OF_RANGE, pdm_error_table->VBAT_check);
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

void App_PdmErrorTable_CheckIndividualErrors(struct PdmErrorTable *pdm_error_table, size_t id, bool status)
{
    if (status == true)
        pdm_error_table->errors[id][1] = (size_t *)0;
    else
        pdm_error_table->errors[id][1] = (size_t *)1;
}

void App_PdmErrorTable_CheckAllErrors(struct PdmErrorTable *pdm_error_table)
{
    App_PdmErrorTable_CheckIndividualErrors(pdm_error_table, PDM_NON_CRITICAL_VBAT_VOLTAGE_OUT_OF_RANGE, pdm_error_table->VBAT_check);
    App_PdmErrorTable_CheckIndividualErrors(pdm_error_table, PDM_NON_CRITICAL__24V_ACC_VOLTAGE_OUT_OF_RANGE, pdm_error_table->__24V_ACC_check);
    App_PdmErrorTable_CheckIndividualErrors(pdm_error_table, PDM_NON_CRITICAL__22V_AUX_VOLTAGE_OUT_OF_RANGE, pdm_error_table->__22V_AUX_check);

    App_PdmErrorTable_CheckIndividualErrors(pdm_error_table, PDM_NON_CRITICAL_EFUSE1_CHANNEL0_OUT_OF_RANGE, pdm_error_table->efuse1_channel0_check);
    App_PdmErrorTable_CheckIndividualErrors(pdm_error_table, PDM_NON_CRITICAL_EFUSE1_CHANNEL1_OUT_OF_RANGE, pdm_error_table->efuse1_channel1_check);
    App_PdmErrorTable_CheckIndividualErrors(pdm_error_table, PDM_NON_CRITICAL_EFUSE2_CHANNEL0_OUT_OF_RANGE, pdm_error_table->efuse2_channel0_check);
    App_PdmErrorTable_CheckIndividualErrors(pdm_error_table, PDM_NON_CRITICAL_EFUSE2_CHANNEL1_OUT_OF_RANGE, pdm_error_table->efuse2_channel1_check);
    App_PdmErrorTable_CheckIndividualErrors(pdm_error_table, PDM_NON_CRITICAL_EFUSE3_CHANNEL0_OUT_OF_RANGE, pdm_error_table->efuse3_channel0_check);
    App_PdmErrorTable_CheckIndividualErrors(pdm_error_table, PDM_NON_CRITICAL_EFUSE3_CHANNEL1_OUT_OF_RANGE, pdm_error_table->efuse3_channel1_check);
    App_PdmErrorTable_CheckIndividualErrors(pdm_error_table, PDM_NON_CRITICAL_EFUSE4_CHANNEL0_OUT_OF_RANGE, pdm_error_table->efuse4_channel1_check);
    App_PdmErrorTable_CheckIndividualErrors(pdm_error_table, PDM_NON_CRITICAL_EFUSE4_CHANNEL1_OUT_OF_RANGE, pdm_error_table->efuse4_channel1_check);

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
