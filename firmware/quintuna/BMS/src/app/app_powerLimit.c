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

/**
 * @brief Gets the min discharge power Limit based on all of the temp
 * limiting condition
 * @return Minimum power current limit value
 */
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

/**
 * @brief Gets the min charge power Limit based on all of the temp
 * limiting condition
 * @return Minimum power current limit value
 */
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

/**
 * @brief Gets the min discharge current Limit based on all of the current
 * limiting conditions
 * @return Minimum Discharge current limit value
 */
float app_powerLimit_getDischargeCurrentLimit()
{
    return MAX_CURRENT_LIMIT;
}

/**
 * @brief Gets the min charge current limit based on all of the current
 * limiting conditions
 * @return Minimum Charge current limit value
 */
float app_powerLimit_getChargeCurrentLimit()
{
    return MAX_CURRENT_LIMIT;
}

// TODO: implement this once app SOC is merged with master - see quadrina current limit pr

/**
 * @brief Gets the Low Voltage Clamp based discharge current limtit
 * @return Low Voltage Clamp based discharge current Limit value
 */
float app_powerLimit_lowVoltageClampCurrentLimit()
{
    return -1;
}

// TODO: implement this once app SOC is merged with master - see quadrina current limit pr

/**
 * @brief Gets the High Voltage Clamp based charge current limit
 * @return High Voltage Clamp based Charge current Limit value
 */
float app_powerLimit_highVoltageClampCurrentLimit()
{
    return -1;
}

// TODO: implement this - see quadrina current limit pr

/**
 * @brief Gets the Low SOC based discharge current limit
 * @return Low SOC based discharge current Limit value
 */
float app_powerLimit_lowSOCCurrentLimit()
{
    return -1;
}

// TODO: implement this - see quadrina current limit pr

/**
 * @brief Gets the High SOC based charge current limit
 * @return High SOC based charge current Limit value
 */
float app_powerLimit_highSOCCurrentLimit()
{
    return -1;
}

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

    // Broadcast info
    app_canAlerts_BMS_Info_DischargePowerLimitActive_set(discharge_lim_active);
    app_canAlerts_BMS_Info_ChargePowerLimitActive_set(charge_lim_active);

    // Broadcast limits
    app_canTx_BMS_DischargePowerLimit_set((uint32_t)discharge_p_lim);
    app_canTx_BMS_ChargePowerLimit_set((uint32_t)charge_p_lim);
}