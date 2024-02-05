#include "app_soc.h"
#include "app_accumulator.h"

#include "math.h"

#define NOMINAL_CELL_V 3.7f
#define MAX_PACK_V (NOMINAL_CELL_V * ACCUMULATOR_NUM_SERIES_CELLS_TOTAL)
#define MAX_CONTINUOUS_CURRENT 59.0f //source - https://ubcformulaelectric.atlassian.net/wiki/spaces/UFE/pages/720972/Software+-+Thruna+FW+-+BMS+-+Current+Limiting

#define TEMP_FAULT_THRESHOLD 60U
#define TEMP_WARNING_THRESHOLD 40U

#define LOW_VOLTAGE_FAULT_THRESHOLD 3.0f
#define LOW_VOLTAGE_WARNING_THRESHOLD 3.2f
#define HIGH_VOLTAGR_FAULT_THRESHOLD 4.2f
#define HIGH_VOLTAGE_WARNING_THRESHOLD 4.0f
#define NUM_PARALLEL_CELLS (3U)
#define INTERNAL_R_PER_CELL_OHMS (2.5e-3f)
#define SERIES_ELEMENT_RESISTANCE (INTERNAL_R_PER_CELL_OHMS / NUM_PARALLEL_CELLS)

//need to be updated after discussion -----------------------
#define LOW_SOC_FAULT_THRESHOLD 10.0f //Limit to stop discharging
#define LOW_SOC_WARNING_THRESHOLD 20.0f
#define HIGH_SOC_FAULT_THRESHOLD 95.0f //Limit to stop charging
#define HIGH_SOC_WARNING_THRESHOLD 80.0f 

float app_currentLimit_calcTempCurrentLimit(float measured_max_cell_temp)
{
    return app_math_linearDerating(measured_max_cell_temp,  MAX_CONTINUOUS_CURRENT, TEMP_WARNING_THRESHOLD, TEMP_FAULT_THRESHOLD);
}

float app_currentLimit_calcLowVoltClampCurrentLimit(void)
{
    float v_cutoff = LOW_VOLTAGE_WARNING_THRESHOLD;
    float v_oc = app_soc_getMinOcvFromSoc();
    return -(v_oc - v_cutoff) / SERIES_ELEMENT_RESISTANCE;
}

float app_currentLimit_calcHighVoltClampCurrentLimit(void)
{
    float v_max = HIGH_VOLTAGE_WARNING_THRESHOLD;
    float v_oc = app_soc_getMinOcvFromSoc();
    return (v_max - v_oc) / SERIES_ELEMENT_RESISTANCE;
}

float app_currentLimit_calcLowSocCurrentLimit(void)
{
    float measured_soc = app_soc_getMinSocPercent();
    return app_math_linearDerating(measured_soc, MAX_CONTINUOUS_CURRENT, ) //Is there opposite side of derating/uprating
}

float app_currentLimit_calcHighSocCurrentLimit(void)
{
    float measured_soc = app_soc_getMinSocPercent(); //This measure the lowest SOC but maybe we want high SOC here
    return app_math_linearDerating(measured_soc, MAX_CONTINUOUS_CURRENT, HIGH_SOC_WARNING_THRESHOLD, HIGH_SOC_FAULT_THRESHOLD);
}