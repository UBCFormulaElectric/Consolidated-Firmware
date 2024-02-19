#include "app_currentLimit.h"

#include "app_soc.h"
#include "app_accumulator.h"
#include "app_math.h"

#include "ltc6813/io_ltc6813CellTemps.h"
#include "math.h"
#include "stdbool.h"

#define NOMINAL_CELL_V (3.7f)
#define MAX_PACK_V (NOMINAL_CELL_V * ACCUMULATOR_NUM_SERIES_CELLS_TOTAL)
#define MAX_CELL_DISCHARGE_CURRENT \
    (88.5f) // source -
            // https://ubcformulaelectric.atlassian.net/wiki/spaces/UFE/pages/720972/Software+-+Thruna+FW+-+BMS+-+Current+Limiting
#define MAX_CONTINUOUS_CURRENT ((float)(MAX_CELL_DISCHARGE_CURRENT * NUM_PARALLEL_CELLS))
#define MAX_POWER_LIMIT_W (78e3f)

#define TEMP_FAULT_THRESHOLD (60U)
#define TEMP_WARNING_THRESHOLD (40U)

#define LOW_VOLTAGE_FAULT_THRESHOLD (3.0f)
#define LOW_VOLTAGE_WARNING_THRESHOLD (3.55f)
#define HIGH_VOLTAGR_FAULT_THRESHOLD (4.2f)
#define HIGH_VOLTAGE_WARNING_THRESHOLD (4.0f)
#define NUM_PARALLEL_CELLS (3U)
#define INTERNAL_R_PER_CELL_OHMS (2.5e-3f)
#define SERIES_ELEMENT_RESISTANCE ((float)(INTERNAL_R_PER_CELL_OHMS / NUM_PARALLEL_CELLS))

// need to be updated after discussion -----------------------
#define LOW_SOC_FAULT_THRESHOLD (20.0f) // Limit to stop discharging
#define LOW_SOC_WARNING_THRESHOLD (30.0f)
#define HIGH_SOC_FAULT_THRESHOLD (85.0f) // Limit to stop charging
#define HIGH_SOC_WARNING_THRESHOLD (75.0f)

void app_currentLimit_broadcast(void)
{
    const float dischargingCurrentLimit = app_currentLimit_getDischargeLimit();
    const float chargingCurrentLimit    = app_currentLimit_getChargeLimit();

    const bool currentLimitActive =
        ((dischargingCurrentLimit < MAX_CONTINUOUS_CURRENT) || (chargingCurrentLimit < MAX_CONTINUOUS_CURRENT)) ? true
                                                                                                                : false;

    app_canTx_BMS_Warning_CurrentLimitActive_set(currentLimitActive);
}

float app_currentLimit_getDischargeLimit(void)
{
    uint8_t max_segment            = 0U;
    uint8_t max_loc                = 0U;
    float   max_cell_temp          = ((float)io_ltc6813CellTemps_getMaxTempDegC(&max_segment, &max_loc));
    float   tempDischargeCurrLimit = app_currentLimit_calcTempCurrentLimit(max_cell_temp);

    float lowVoltDischargeCurrLimit = app_currentLimit_calcLowVoltClampCurrentLimit();

    float lowSocDischargeCurrLimit = app_currentLimit_calcLowSocCurrentLimit();

    float currLimits[4] = { MAX_CONTINUOUS_CURRENT, tempDischargeCurrLimit, lowVoltDischargeCurrLimit,
                            lowSocDischargeCurrLimit };

    float   currentLimit      = MAX_CONTINUOUS_CURRENT;
    uint8_t currLimitCondtion = 0;
    for (uint8_t i = 0; i < sizeof(currLimits) / sizeof(float); i++)
    {
        if (currLimits[i] < currentLimit)
        {
            currentLimit      = currLimits[i];
            currLimitCondtion = i;
        }
    }

    switch (currLimitCondtion)
    {
        case 0:
            app_canTx_BMS_DischargingCurrentLimitCondition_set(NO_DISCHARGING_CURRENT_LIMIT);
            break;
        case 1:
            app_canTx_BMS_DischargingCurrentLimitCondition_set(HIGH_TEMP_BASED_DISCHARGING_CURRENT_LIMIT);
            break;
        case 2:
            app_canTx_BMS_DischargingCurrentLimitCondition_set(LOW_VOLT_BASED_DISCHARGING_CURRENT_LIMIT);
            break;
        case 3:
            app_canTx_BMS_DischargingCurrentLimitCondition_set(LOW_SOC_BASED_DISCHARGING_CURRENT_LIMIT);
            break;
    }

    app_canTx_BMS_AvailableDischargingCurrentLimit_set(currentLimit);

    return currentLimit;
}

float app_currentLimit_getChargeLimit(void)
{
    uint8_t max_segment         = 0U;
    uint8_t max_loc             = 0U;
    float   max_cell_temp       = ((float)io_ltc6813CellTemps_getMaxTempDegC(&max_segment, &max_loc));
    float   tempChargeCurrLimit = app_currentLimit_calcTempCurrentLimit(max_cell_temp);

    float currLimits[2] = { MAX_CONTINUOUS_CURRENT, tempChargeCurrLimit };

    float   currentLimit      = MAX_CONTINUOUS_CURRENT;
    uint8_t currLimitCondtion = 0;
    for (uint8_t i = 0; i < sizeof(currLimits) / sizeof(float); i++)
    {
        if (currLimits[i] < currentLimit)
        {
            currentLimit      = currLimits[i];
            currLimitCondtion = i;
        }
    }

    switch (currLimitCondtion)
    {
        case 0:
            app_canTx_BMS_ChargingCurrentLimitCondition_set(NO_CHARGING_CURRENT_LIMIT);
            break;
        case 1:
            app_canTx_BMS_ChargingCurrentLimitCondition_set(HIGH_TEMP_BASED_CHARGING_CURRENT_LIMIT);
            break;
    }

    app_canTx_BMS_AvailableChargingCurrentLimit_set(currentLimit);
    return currentLimit;
}

float app_currentLimit_calcTempCurrentLimit(float measured_max_cell_temp)
{
    return app_math_linearDerating(
        measured_max_cell_temp, MAX_CONTINUOUS_CURRENT, TEMP_WARNING_THRESHOLD, TEMP_FAULT_THRESHOLD, REDUCE_X);
}

float app_currentLimit_calcLowVoltClampCurrentLimit(void)
{
    const float v_cutoff  = LOW_VOLTAGE_WARNING_THRESHOLD;
    float       v_oc      = app_soc_getMinOcvFromSoc();
    float       currLimit = (v_oc - v_cutoff) / SERIES_ELEMENT_RESISTANCE;
    if (currLimit < 0)
    {
        currLimit = 0.0f;
    }
    return currLimit;
}

// TODO: implement app_soc_getMaxSOCPercent() to current limit during charging and then set up the following
// functionality source: Read 2.3 SOC Based Limiting at
// "https://ubcformulaelectric.atlassian.net/wiki/spaces/UFE/pages/720972/Software+-+Thruna+FW+-+BMS+-+Current+Limiting"
//  float app_currentLimit_calcHighVoltClampCurrentLimit(void)
//  {
//      float v_max = HIGH_VOLTAGE_WARNING_THRESHOLD;
//      float v_oc = app_soc_getMaxOcvFromSoc();
//      float currLimit = (v_max - v_oc) / SERIES_ELEMENT_RESISTANCE;
//      if(currLimit < 0)
//      {
//          currLimit = MAX_CONTINUOUS_CURRENT;
//      }
//      return currLimit;
//  }

float app_currentLimit_calcLowSocCurrentLimit(void)
{
    float measured_soc = app_soc_getMinSocPercent();
    return app_math_linearDerating(
        measured_soc, MAX_CONTINUOUS_CURRENT, LOW_SOC_WARNING_THRESHOLD, LOW_SOC_FAULT_THRESHOLD, INCREASE_X);
}

// TODO: implement app_soc_getMaxSOCPercent() to current limit during charging and then set up the following
// functionality source: Read 2.4 SOC Based Limiting at
// "https://ubcformulaelectric.atlassian.net/wiki/spaces/UFE/pages/720972/Software+-+Thruna+FW+-+BMS+-+Current+Limiting"
//  float app_currentLimit_calcHighSocCurrentLimit(void)
//  {
//      float measured_soc = app_soc_getMaxSocPercent(); //This measure the lowest SOC but maybe we want high SOC here
//      return app_math_linearDerating(measured_soc, MAX_CONTINUOUS_CURRENT, HIGH_SOC_WARNING_THRESHOLD,
//      HIGH_SOC_FAULT_THRESHOLD);
//  }
