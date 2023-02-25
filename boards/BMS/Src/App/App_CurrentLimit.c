#include "App_CurrentLimit.h"

#define MAX_POWER_LIMIT_W (78e3f)
// TODO: Update max current
#define MAX_CURRENT (MAX_POWER_LIMIT_W / 3.7f) // 3.7v is nominal cell voltage

#define TEMP_FAULT_THRESHOLD_DC 60U
#define TEMP_WARNING_THRESHOLD_DC 40U
#define VOLTAGE_FAULT_THRESHOLD_DC 3.0f
#define VOLTAGE_WARNING_THRESHOLD_DC 3.2f
#define SOC_FAULT_THRESHOLD_DC 0.0f
#define SOC_WARNING_THRESHOLD_DC 10.0f

float App_CurrentLimit_GetDischargeLimit(struct Accumulator *accumulator)
{
    uint8_t     throwaway_segment = 0U;
    uint8_t     throwaway_loc     = 0U;
    const float max_cell_temp     = App_Accumulator_GetMaxCellTempDegC(accumulator, &throwaway_segment, &throwaway_loc);
    const float min_cell_soc      = 0.0f;
    const float min_cell_voltage  = App_Accumulator_GetMinVoltage(accumulator, &throwaway_segment, &throwaway_loc);

    // Calculate Current Limits
    float currentLimits[3] = {
        App_CurrentLimit_CalculateTempBasedDischargeLimit(max_cell_temp),
        //                               App_CurrentLimit_CalculateSOCBasedDischargeLimit(min_cell_soc),
        App_CurrentLimit_CalculateVoltageBasedDischargeLimit(min_cell_voltage)
    };

    // Return the minimum limit
    float current_limit = currentLimits[0];

    for (unsigned int i = 1; i < sizeof(currentLimits); i++)
    {
        if (currentLimits[i] < current_limit)
        {
            current_limit = currentLimits[i];
        }
    }

    return current_limit;
}

float App_CurrentLimit_GetChargeLimit(void)
{
    return 0.0f;
}

float App_CurrentLimit_CalculateTempBasedDischargeLimit(float max_cell_temp)
{
    return App_SharedProcessing_LinearDerating(
        max_cell_temp, MAX_CURRENT, TEMP_WARNING_THRESHOLD_DC, TEMP_FAULT_THRESHOLD_DC);
}

float App_CurrentLimit_CalculateSOCBasedDischargeLimit(float min_cell_soc)
{
    return App_SharedProcessing_LinearDerating(
        min_cell_soc, MAX_CURRENT, SOC_WARNING_THRESHOLD_DC, SOC_FAULT_THRESHOLD_DC);
}

float App_CurrentLimit_CalculateVoltageBasedDischargeLimit(float min_cell_voltage)
{
    return App_SharedProcessing_InverseLinearDerating(
        min_cell_voltage, MAX_CURRENT, VOLTAGE_WARNING_THRESHOLD_DC, VOLTAGE_FAULT_THRESHOLD_DC);
}
