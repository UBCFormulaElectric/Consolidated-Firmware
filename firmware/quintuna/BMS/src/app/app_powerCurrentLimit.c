#include "app_math.h"
#include "app_segments.h"
#include "app_powerCurrentLimit.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "app_canUtils.h"

// TODO: use global var
#define MAX_CONTINUOUS_CURRENT 14U
#define MAX_POWER_LIMIT_W 78e3f
#define CELL_ROLL_OFF_TEMP_DEGC 40.0f
#define CELL_FULLY_DERATED_TEMP 60.0f

#define TEMP_FAULT_THRESHOLD 60U
#define TEMP_WARNING_THRESHOLD 40U

// TODO: verify these values
#define LOW_VOLTAGE_FAULT_THRESHOLD (3.0f)
#define LOW_VOLTAGE_WARNING_THRESHOLD (3.62f)
#define HIGH_VOLTAGE_FAULT_THRESHOLD (4.2f)
#define HIGH_VOLTAGE_WARNING_THRESHOLD (4.0f)
#define NUM_PARALLEL_CELLS (3U)
#define INTERNAL_R_PER_CELL_OHMS (2.5e-3f)
#define SERIES_ELEMENT_RESISTANCE ((float)(INTERNAL_R_PER_CELL_OHMS / NUM_PARALLEL_CELLS))

//TODO: verify/discuss these values
#define LOW_SOC_FAULT_THRESHOLD (15.0f) // Limit to stop discharging
#define LOW_SOC_WARNING_THRESHOLD (30.0f)
#define HIGH_SOC_FAULT_THRESHOLD (85.0f) // Limit to stop charging
#define HIGH_SOC_WARNING_THRESHOLD (75.0f)

//TODO: verify this
#define MIN_CURRENT_LIMIT_CUTTOFF (10.0f)

void app_powerCurrentLimit_broadcast()
{
    // Get current limits
    const float discharge_c_lim = app_powerCurrentLimit_getDischargeCurrentLimit();
    const float charge_c_lim    = app_powerCurrentLimit_getChargeCurrentLimit();

    // Determine if current limit is active
    const bool c_lim_active = 
        (discharge_c_lim < MAX_CONTINUOUS_CURRENT) || 
        (charge_c_lim < MAX_CONTINUOUS_CURRENT);

    app_canAlerts_BMS_Warning_CurrentLimitActive_set(c_lim_active);

    // Get power limits
    const float discharge_p_lim = app_powerCurrentLimit_getDischargePowerLimit();
    const float charge_p_lim    = app_powerCurrentLimit_getChargePowerLimit();

    // Determine if power limit is active
    const bool p_lim_active = 
        (discharge_p_lim < MAX_POWER_LIMIT_W) || 
        (charge_p_lim < MAX_POWER_LIMIT_W);

    app_canAlerts_BMS_Warning_PowerLimitActive_set(p_lim_active);
}

float app_powerCurrentLimit_getDischargePowerLimit()
{
    const float max_cell_temp = app_segments_getMaxCellTemp();

    const float temp_based_power_limit = app_math_linearDerating(
        max_cell_temp,
        MAX_POWER_LIMIT_W,
        CELL_ROLL_OFF_TEMP_DEGC,
        CELL_FULLY_DERATED_TEMP,
        REDUCE_X
    );

    // Ensure limit does not exceed the max allowed power
    const float power_limit = MIN(temp_based_power_limit, MAX_POWER_LIMIT_W);

    return power_limit;
}

float app_powerCurrentLimit_getChargePowerLimit()
{
    const float max_cell_temp = app_segments_getMaxCellTemp();
    float power_limit = MAX_POWER_LIMIT_W;

    if (max_cell_temp <= 0.0f || max_cell_temp >= 60.0f)
    {
        power_limit = 0.0f; // No charging outside safe range
    }
    else if (max_cell_temp > 0.0f && max_cell_temp < 15.0f)
    {
        // Ramp up from 0 to full power between 0°C–15°C
        power_limit = app_math_linearDerating(
            max_cell_temp,
            MAX_POWER_LIMIT_W,
            0.0f,
            15.0f,
            INCREASE_X
        );
    }
    else if (max_cell_temp > 45.0f && max_cell_temp < 60.0f)
    {
        // Ramp down from full power to 0 between 45°C–60°C
        power_limit = app_math_linearDerating(
            max_cell_temp,
            MAX_POWER_LIMIT_W,
            45.0f,
            60.0f,
            REDUCE_X
        );
    }

    return MIN(power_limit, MAX_POWER_LIMIT_W);
}

float app_powerCurrentLimit_getDischargeCurrentLimit()
{
    // Get max cell temperature
    float max_cell_temp = app_segments_getMaxCellTemp();

    // Calculate individual current limits
    float t_lim        = app_powerCurrentLimit_calcTempCurrentLimit(max_cell_temp);
    // float low_v_lim    = app_powerCurrentLimit_calcLowVoltageClampCurrentLimit();
    // float high_v_lim   = app_powerCurrentLimit_calchighVoltageClampCurrentLimit();
    // float low_soc_lim  = app_powerCurrentLimit_calcLowSOCCurrentLimit();
    // float high_soc_lim = app_powerCurrentLimit_calcHighSOCCurrentLimit();

    // Aggregate current limits
    float c_lims[2] = {
        MAX_CONTINUOUS_CURRENT, // Default upper limit
        t_lim                   // Temperature-based limit
    };

    // Find most limiting current condition
    float   c_lim           = MAX_CONTINUOUS_CURRENT;
    uint8_t c_lim_condition = 0;

    for (uint8_t i = 0; i < sizeof(c_lims) / sizeof(c_lims[0]); i++)
    {
        if (c_lims[i] < c_lim)
        {
            c_lim = c_lims[i];
            c_lim_condition = i;
        }
    }

    // Report current limiting reason over CAN
    switch (c_lim_condition)
    {
        case 0:
            app_canTx_BMS_DischargeCurrentLimitCondition_set(NO_DISCHARGING_CURRENT_LIMIT);
            break;
        case 1:
            app_canTx_BMS_DischargeCurrentLimitCondition_set(HIGH_TEMP_BASED_DISCHARGING_CURRENT_LIMIT);
            break;
    }

    // Enforce a minimum cutoff for safety
    if (c_lim < MIN_CURRENT_LIMIT_CUTTOFF)
    {
        c_lim = MIN_CURRENT_LIMIT_CUTTOFF;
    }

    // Broadcast the final current limit over CAN
    app_canTx_BMS_DischargeCurrentLimit_set(c_lim);

    return c_lim;
}

float app_powerCurrentLimit_getChargeCurrentLimit()
{
    // Get max cell temperature
    float max_cell_temp = app_segments_getMaxCellTemp();

    // Calculate temperature-based charging limit
    float t_lim = app_powerCurrentLimit_calcTempCurrentLimit(max_cell_temp);

    // Aggregate current limits
    float c_lims[2] = {
        MAX_CONTINUOUS_CURRENT, // Default upper charging limit
        t_lim                   // Temperature-based charging limit
    };

    // Find most limiting charging condition
    float   c_lim           = MAX_CONTINUOUS_CURRENT;
    uint8_t c_lim_condition = 0;

    for (uint8_t i = 0; i < sizeof(c_lims) / sizeof(c_lims[0]); i++)
    {
        if (c_lims[i] < c_lim)
        {
            c_lim = c_lims[i];
            c_lim_condition = i;
        }
    }

    // Report charging current limiting reason over CAN
    switch (c_lim_condition)
    {
        case 0:
            app_canTx_BMS_ChargeCurrentLimitCondition_set(NO_CHARGING_CURRENT_LIMIT);
            break;
        case 1:
            app_canTx_BMS_ChargeCurrentLimitCondition_set(HIGH_TEMP_BASED_CHARGING_CURRENT_LIMIT);
            break;
    }

    // Enforce a minimum cutoff for safety
    if (c_lim < MIN_CURRENT_LIMIT_CUTTOFF)
    {
        c_lim = MIN_CURRENT_LIMIT_CUTTOFF;
    }

    // Broadcast the final charging current limit over CAN
    app_canTx_BMS_ChargeCurrentLimit_set(c_lim);

    return c_lim;
}

float app_powerCurrentLimit_calcTempCurrentLimit(float max_cell_temp)
{
    return app_math_linearDerating(max_cell_temp, MAX_CONTINUOUS_CURRENT, TEMP_WARNING_THRESHOLD, TEMP_FAULT_THRESHOLD, REDUCE_X);
}

// TODO: implement this once app SOC is merged with master - see quadrina current limit pr
float app_powerCurrentLimit_calcLowVoltageClampCurrentLimit()
{
    return -1;
}

 // TODO: implement this once app SOC is merged with master - see quadrina current limit pr
float app_powerCurrentLimit_calcHighVoltageClampCurrentLimit()
{
    return -1;
}

// TODO: implement this - see quadrina current limit pr
float app_powerCurrentLimit_calcLowSOCCurrentLimit()
{
    return -1;
}

// TODO: implement this - see quadrina current limit pr
float app_powerCurrentLimit_calcHighSOCCurrentLimit()
{
    return -1;
}