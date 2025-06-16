#include "app_math.h"
#include "app_segments.h"
#include "app_powerCurrentLimit.h"
#include "segments/app_segments_internal.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_canUtils.h"

// TODO: use global variables
#define MAX_POWER_LIMIT_W 78.0e3f
#define MAX_CURRENT_LIMIT 175.0f
#define MIN_CURRENT_LIMIT 10.0f
#define TEMP_FAULT_THRESHOLD 60.0f
#define TEMP_WARNING_THRESHOLD 40.0f
#define TEMP_HYSTERESIS_THRESHOLD 55.0f

// persistent hysteresis state across calls
static bool temp_hysteresis_engaged = false;

void app_powerCurrentLimit_broadcast()
{
    // Get current limits
    const float discharge_c_lim = app_powerCurrentLimit_getDischargeCurrentLimit();
    const float charge_c_lim    = app_powerCurrentLimit_getChargeCurrentLimit();

    // Get power limits
    const float discharge_p_lim = min(app_powerCurrentLimit_getDischargePowerLimit(), discharge_c_lim * pack_voltage);
    const float charge_p_lim    = min(app_powerCurrentLimit_getChargePowerLimit(), charge_c_lim * pack_voltage);

    // Determine if power limit is active
    const bool p_lim_active = (discharge_p_lim < MAX_POWER_LIMIT_W) || (charge_p_lim < MAX_POWER_LIMIT_W);

    // Maintain temperature hysteresis
    update_temperature_hysteresis();
    if (temp_hysteresis_engaged)
    {
        // Broadcast info
        app_canAlerts_BMS_Warning_PowerLimitActive_set(true);

        // Broadcast limits
        app_canTx_BMS_DischargePowerLimit_set(0);
        app_canTx_BMS_ChargePowerLimit_set(0);
    }
    else
    {
          // Broadcast info
        app_canAlerts_BMS_Warning_PowerLimitActive_set(p_lim_active);

        // Broadcast limits
        app_canTx_BMS_DischargePowerLimit_set((uint32_t)discharge_p_lim);
        app_canTx_BMS_ChargePowerLimit_set((uint32_t)charge_p_lim);
    }
}

float app_powerCurrentLimit_getDischargePowerLimit()
{
    const float max_cell_temp = app_segments_getMaxCellTemp().value;

    const float temp_based_power_limit = app_math_linearDerating(
        max_cell_temp, MAX_POWER_LIMIT_W, TEMP_WARNING_THRESHOLD, TEMP_FAULT_THRESHOLD, REDUCE_X);

    // Final power limit (capped at max)
    const float power_limit = MIN(temp_based_power_limit, MAX_POWER_LIMIT_W);

    // Determine limiting condition enum
    uint8_t p_lim_condition = NO_DISCHARGE_POWER_LIMIT;

    if (max_cell_temp >= TEMP_WARNING_THRESHOLD)
    {
        p_lim_condition = HIGH_TEMP_BASED_DISCHARGE_POWER_LIMIT;
    }

    // Broadcast limit condition over CAN
    app_canTx_BMS_DischargePowerLimitCondition_set(p_lim_condition);

    return power_limit;
}

float app_powerCurrentLimit_getChargePowerLimit()
{
    const float max_cell_temp   = app_segments_getMaxCellTemp().value;
    float       power_limit     = MAX_POWER_LIMIT_W;
    uint8_t     p_lim_condition = NO_CHARGE_POWER_LIMIT;

    if (max_cell_temp <= 0.0f || max_cell_temp >= 60.0f)
    {
        power_limit     = 0.0f;
        p_lim_condition = HIGH_TEMP_BASED_CHARGE_POWER_LIMIT;
    }
    else if (max_cell_temp > 0.0f && max_cell_temp < 15.0f)
    {
        power_limit     = app_math_linearDerating(max_cell_temp, MAX_POWER_LIMIT_W, 0.0f, 15.0f, INCREASE_X);
        p_lim_condition = LOW_TEMP_BASED_CHARGE_POWER_LIMIT;
    }
    else if (max_cell_temp > 45.0f && max_cell_temp < 60.0f)
    {
        power_limit     = app_math_linearDerating(max_cell_temp, MAX_POWER_LIMIT_W, 45.0f, 60.0f, REDUCE_X);
        p_lim_condition = HIGH_TEMP_BASED_CHARGE_POWER_LIMIT;
    }
    else
    {
        p_lim_condition = NO_CHARGE_POWER_LIMIT;
    }

    // Broadcast limit condition over CAN
    app_canTx_BMS_ChargePowerLimitCondition_set(p_lim_condition);

    return min(power_limit, MAX_POWER_LIMIT_W);
}

float app_powerCurrentLimit_getDischargeCurrentLimit()
{
    // Get max cell temperature
    float max_cell_temp = app_segments_getMaxCellTemp().value;

    // Calculate individual current limits
    float t_lim = app_powerCurrentLimit_calcTempCurrentLimit(max_cell_temp);
    // float low_v_lim    = app_powerCurrentLimit_calcLowVoltageClampCurrentLimit();
    // float low_soc_lim  = app_powerCurrentLimit_calcLowSOCCurrentLimit();

    // Aggregate current limits
    float c_lims[2] = {
        MAX_CURRENT_LIMIT, // Default upper limit
        t_lim                   // Temperature-based limit
    };

    // Find most limiting current condition
    float   c_lim           = MAX_CURRENT_LIMIT;
    uint8_t c_lim_condition = 0;

    for (uint8_t i = 0; i < sizeof(c_lims) / sizeof(c_lims[0]); i++)
    {
        if (c_lims[i] < c_lim)
        {
            c_lim           = c_lims[i];
            c_lim_condition = i;
        }
    }

    // Report current limiting reason over CAN
    switch (c_lim_condition)
    {
        case 0:
            app_canTx_BMS_DischargePowerLimitCondition_set(NO_DISCHARGE_POWER_LIMIT);
            break;
        case 1:
            app_canTx_BMS_DischargePowerLimitCondition_set(HIGH_TEMP_BASED_DISCHARGE_POWER_LIMIT);
            break;
    }

    // Enforce a minimum cutoff for safety
    if (c_lim < MIN_CURRENT_LIMIT)
    {
        c_lim = MIN_CURRENT_LIMIT;
    }

    return c_lim;
}

float app_powerCurrentLimit_getChargeCurrentLimit()
{
    // Get max cell temperature
    float max_cell_temp = app_segments_getMaxCellTemp().value;

    // Calculate temperature-based charging limit
    float t_lim = app_powerCurrentLimit_calcTempCurrentLimit(max_cell_temp);
    // float high_v_lim   = app_powerCurrentLimit_calchighVoltageClampCurrentLimit();
    // float high_soc_lim = app_powerCurrentLimit_calcHighSOCCurrentLimit();

    // Aggregate current limits
    float c_lims[2] = {
        MAX_CURRENT_LIMIT, // Default upper charging limit
        t_lim                   // Temperature-based charging limit
    };

    // Find most limiting charging condition
    float   c_lim           = MAX_CURRENT_LIMIT;
    uint8_t c_lim_condition = 0;

    for (uint8_t i = 0; i < sizeof(c_lims) / sizeof(c_lims[0]); i++)
    {
        if (c_lims[i] < c_lim)
        {
            c_lim           = c_lims[i];
            c_lim_condition = i;
        }
    }

    // Report charging current limiting reason over CAN
    switch (c_lim_condition)
    {
        case 0:
            app_canTx_BMS_ChargePowerLimitCondition_set(NO_CHARGE_POWER_LIMIT);
            break;
        case 1:
            app_canTx_BMS_ChargePowerLimitCondition_set(HIGH_TEMP_BASED_CHARGE_POWER_LIMIT);
            break;
    }

    // Enforce a minimum cutoff for safety
    if (c_lim < MIN_CURRENT_LIMIT)
    {
        c_lim = MIN_CURRENT_LIMIT;
    }

    return c_lim;
}

float app_powerCurrentLimit_calcTempCurrentLimit(float max_cell_temp)
{
    return app_math_linearDerating(
        max_cell_temp, MAX_CURRENT_LIMIT, TEMP_WARNING_THRESHOLD, TEMP_FAULT_THRESHOLD, REDUCE_X);
}

// TODO: implement this once app SOC is merged with master - see quadrina current limit pr
float app_powerCurrentLimit_lowVoltageClampCurrentLimit()
{
    return -1;
}

// TODO: implement this once app SOC is merged with master - see quadrina current limit pr
float app_powerCurrentLimit_highVoltageClampCurrentLimit()
{
    return -1;
}

// TODO: implement this - see quadrina current limit pr
float app_powerCurrentLimit_lowSOCCurrentLimit()
{
    return -1;
}

// TODO: implement this - see quadrina current limit pr
float app_powerCurrentLimit_highSOCCurrentLimit()
{
    return -1;
}

// call this every time you read the cell temperature
void update_temperature_hysteresis() {
    // enter limit state if we hit or exceed the high threshold
    if (!temp_hysteresis_engaged && max_cell_temp.value >= TEMP_HYSTERESIS_THRESHOLD) {
        temp_hysteresis_engaged = true;
    }
    // exit limit state only once we drop to or below the low threshold
    else if (temp_hysteresis_engaged && max_cell_temp.value <= TEMP_HYSTERESIS_THRESHOLD) {
        temp_hysteresis_engaged = false;
    }
}