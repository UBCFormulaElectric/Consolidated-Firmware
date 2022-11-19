#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/App_RailMonitoring.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/App_Efuse.h"


enum ErrorId
{
    PDM_NON_CRITICAL_VBAT_VOLTAGE_OUT_OF_RANGE,
    PDM_NON_CRITICAL__24V_ACC_VOLTAGE_OUT_OF_RANGE,
    PDM_NON_CRITICAL__22V_AUX_VOLTAGE_OUT_OF_RANGE,
    PDM_NON_CRITICAL_EFUSE1_CHANNEL0_OUT_OF_RANGE,
    PDM_NON_CRITICAL_EFUSE1_CHANNEL1_OUT_OF_RANGE,
    PDM_NON_CRITICAL_EFUSE2_CHANNEL0_OUT_OF_RANGE,
    PDM_NON_CRITICAL_EFUSE2_CHANNEL1_OUT_OF_RANGE,
    PDM_NON_CRITICAL_EFUSE3_CHANNEL0_OUT_OF_RANGE,
    PDM_NON_CRITICAL_EFUSE3_CHANNEL1_OUT_OF_RANGE,
    PDM_NON_CRITICAL_EFUSE4_CHANNEL0_OUT_OF_RANGE,
    PDM_NON_CRITICAL_EFUSE4_CHANNEL1_OUT_OF_RANGE,
    NUM_ERROR_IDS
};

struct PdmErrorTable;

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

void App_PdmErrorTable_Destroy(struct PdmErrorTable *pdm_error_table);

void App_PdmErrorTable_CheckErrors(struct PdmErrorTable *pdm_error_table, size_t id, bool status);
void App_PdmErrorTable_FoundError(struct PdmErrorTable *pdm_error_table, size_t id);
bool App_PdmErrorTable_HasAnyErrors(struct PdmErrorTable *pdm_error_table);
void App_PdmErrorTable_GetAllErrors(struct PdmErrorTable *pdm_error_table, size_t all_error_array[NUM_ERROR_IDS]);
