#include "app_math.h"
#include "app_segments.h"
#include "app_powerLimit.h"
#include "segments/app_segments_internal.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_canUtils.h"

// TODO: use global variables
#define MAX_POWER_LIMIT_W 78.0e3f
#define MIN_POWER_LIMIT_W 0.0f
#define MAX_CURRENT_LIMIT 175.0f
#define MIN_CURRENT_LIMIT 10.0f
#define TEMP_FAULT_THRESHOLD 60.0f
#define TEMP_WARNING_THRESHOLD 40.0f
#define TEMP_HYSTERESIS_THRESHOLD 55.0f

// persistent hysteresis state across calls
static bool temp_hysteresis_engaged = false;

void app_powerLimit_broadcast()
{
    // Get current limits
    float discharge_c_lim = app_powerLimit_getDischargeCurrentLimit();
    float charge_c_lim    = app_powerLimit_getChargeCurrentLimit();

    // Get power limits
    float discharge_p_lim = MIN(app_powerLimit_getDischargePowerLimit(), discharge_c_lim * pack_voltage);
    float charge_p_lim    = MIN(app_powerLimit_getChargePowerLimit(), charge_c_lim * pack_voltage);

    // Determine if power limit is active
    const bool discharge_lim_active = (discharge_p_lim < MAX_POWER_LIMIT_W);
    const bool charge_lim_active    = (charge_p_lim < MAX_POWER_LIMIT_W);

    // Enforce a minimum cutoff for safety
    if (discharge_p_lim / pack_voltage < MIN_CURRENT_LIMIT)
    {
        discharge_p_lim = MIN_CURRENT_LIMIT * pack_voltage;
    }

    // Maintain temperature hysteresis
    update_temperature_hysteresis();
    if (temp_hysteresis_engaged)
    {
        // Broadcast info
        app_canAlerts_BMS_Info_DischargePowerLimitActive_set(true);
        app_canAlerts_BMS_Info_ChargePowerLimitActive_set(true);

        // Broadcast limits
        app_canTx_BMS_DischargePowerLimit_set((uint32_t)(MIN_CURRENT_LIMIT * pack_voltage));
        app_canTx_BMS_ChargePowerLimit_set(MIN_POWER_LIMIT_W);
    }
    else
    {
        // Broadcast info
        app_canAlerts_BMS_Info_DischargePowerLimitActive_set(discharge_lim_active);
        app_canAlerts_BMS_Info_ChargePowerLimitActive_set(charge_lim_active);

        // Broadcast limits
        app_canTx_BMS_DischargePowerLimit_set((uint32_t)discharge_p_lim);
        app_canTx_BMS_ChargePowerLimit_set((uint32_t)charge_p_lim);
    }
}

float app_powerLimit_getDischargePowerLimit()
{
    // Calculate power limit from temperature
    const float temp_power_limit = app_math_linearDerating(
        max_cell_temp.value, MAX_POWER_LIMIT_W, TEMP_WARNING_THRESHOLD, TEMP_FAULT_THRESHOLD, REDUCE_X);

    // Final power limit (capped at max)
    const float power_limit = MIN(temp_power_limit, MAX_POWER_LIMIT_W);

    // Determine limiting condition enum
    uint8_t p_lim_condition = NO_DISCHARGE_POWER_LIMIT;

    if (max_cell_temp.value >= TEMP_WARNING_THRESHOLD)
    {
        p_lim_condition = HIGH_TEMP_DISCHARGE_POWER_LIMIT;
    }

    // Broadcast limit condition over CAN
    app_canTx_BMS_DischargePowerLimitCondition_set(p_lim_condition);

    return power_limit;
}

float app_powerLimit_getChargePowerLimit()
{
    // Calculate power limit from temperature
    const float temp_power_limit = app_math_linearDerating(
        max_cell_temp.value, MAX_POWER_LIMIT_W, TEMP_WARNING_THRESHOLD, TEMP_FAULT_THRESHOLD, REDUCE_X);

    // Final power limit (capped at max)
    const float power_limit = MIN(temp_power_limit, MAX_POWER_LIMIT_W);

    // Determine limiting condition enum
    uint8_t p_lim_condition = NO_CHARGE_POWER_LIMIT;

    if (max_cell_temp.value >= TEMP_WARNING_THRESHOLD)
    {
        p_lim_condition = HIGH_TEMP_CHARGE_POWER_LIMIT;
    }

    // Broadcast limit condition over CAN
    app_canTx_BMS_DischargePowerLimitCondition_set(p_lim_condition);

    return power_limit;
}

float app_powerLimit_getDischargeCurrentLimit()
{
    // float low_v_lim    = app_powerCurrentLimit_calcLowVoltageClampCurrentLimit();
    // float low_soc_lim  = app_powerCurrentLimit_calcLowSOCCurrentLimit();

    // Aggregate current limits
    uint8_t c_lim_condition = NO_DISCHARGE_POWER_LIMIT;
    float   c_lim;
    float   c_lims[1] = {
        MAX_CURRENT_LIMIT, // Default upper limit
    };

    // Find most limiting current condition
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
    }

    return c_lim;
}

float app_powerLimit_getChargeCurrentLimit()
{
    // float high_v_lim   = app_powerCurrentLimit_calchighVoltageClampCurrentLimit();
    // float high_soc_lim = app_powerCurrentLimit_calcHighSOCCurrentLimit();

    // Aggregate current limits
    uint8_t c_lim_condition = NO_CHARGE_POWER_LIMIT;
    float   c_lim;
    float   c_lims[1] = {
        MAX_CURRENT_LIMIT, // Default upper charging limit
    };

    // Find most limiting charging condition
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
    }

    return c_lim;
}

// TODO: implement this once app SOC is merged with master - see quadrina current limit pr
float app_powerLimit_lowVoltageClampCurrentLimit()
{
    return -1;
}

// TODO: implement this once app SOC is merged with master - see quadrina current limit pr
float app_powerLimit_highVoltageClampCurrentLimit()
{
    return -1;
}

// TODO: implement this - see quadrina current limit pr
float app_powerLimit_lowSOCCurrentLimit()
{
    return -1;
}

// TODO: implement this - see quadrina current limit pr
float app_powerLimit_highSOCCurrentLimit()
{
    return -1;
}

// Helper function: call this every time you read the cell temperature
void update_temperature_hysteresis()
{
    // enter limit state if we hit or exceed the high threshold
    if (!temp_hysteresis_engaged && max_cell_temp.value >= TEMP_HYSTERESIS_THRESHOLD)
    {
        temp_hysteresis_engaged = true;
    }
    // exit limit state only once we drop to or below the low threshold
    else if (temp_hysteresis_engaged && max_cell_temp.value <= TEMP_HYSTERESIS_THRESHOLD)
    {
        temp_hysteresis_engaged = false;
    }
}