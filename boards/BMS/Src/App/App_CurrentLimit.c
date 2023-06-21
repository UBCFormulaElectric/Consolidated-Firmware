#include "App_CurrentLimit.h"

#define MAX_POWER_LIMIT_W (78e3f)
// TODO: Update max current
#define NOMINAL_CELL_V 3.7f
#define MAX_PACK_V (NOMINAL_CELL_V * ACCUMULATOR_NUM_SERIES_CELLS_TOTAL)
#define MAX_CURRENT (MAX_POWER_LIMIT_W / MAX_PACK_V)

#define TEMP_FAULT_THRESHOLD_DC 60U
#define TEMP_WARNING_THRESHOLD_DC 40U
#define VOLTAGE_FAULT_THRESHOLD_DC 3.0f
#define VOLTAGE_WARNING_THRESHOLD_DC 3.2f
#define NUM_PARALLEL_CELLS (3U)
#define INTERNAL_R_PER_CELL_OHMS (2.5e-3f)
#define SERIES_ELEMENT_RESISTANCE (INTERNAL_R_PER_CELL_OHMS / NUM_PARALLEL_CELLS)

float App_CurrentLimit_GetDischargeLimit(struct Accumulator *accumulator, struct SocStats *soc_stats)
{
    uint8_t     throwaway_segment = 0U;
    uint8_t     throwaway_loc     = 0U;
    const float max_cell_temp     = App_Accumulator_GetMaxCellTempDegC(accumulator, &throwaway_segment, &throwaway_loc);

    // Calculate Current Limits
    float currentLimits[2] = { App_CurrentLimit_CalculateTempBasedDischargeLimit(max_cell_temp),
                               App_CurrentLimit_CalculateLowCellVoltageClampDischargeLimit(soc_stats) };

    // Return the minimum limit
    float current_limit = currentLimits[0];

    for (unsigned int i = 0; i < sizeof(currentLimits) / sizeof(float); i++)
    {
        if (currentLimits[i] < current_limit)
        {
            current_limit = currentLimits[i];
        }
    }

    return current_limit;
}

float App_CurrentLimit_CalculateTempBasedDischargeLimit(float max_cell_temp)
{
    return App_SharedProcessing_LinearDerating(
        max_cell_temp, MAX_CURRENT, TEMP_WARNING_THRESHOLD_DC, TEMP_FAULT_THRESHOLD_DC);
}

float App_CurrentLimit_CalculateLowCellVoltageClampDischargeLimit(struct SocStats *soc_stats)
{
    float Vcuttoff = VOLTAGE_WARNING_THRESHOLD_DC;
    float Voc      = App_SOC_GetMinVocFromSoc(soc_stats);
    return -(Voc - Vcuttoff) / SERIES_ELEMENT_RESISTANCE;
}
