#include "app_math.h"
#include "app_segments.h"
#include "app_powerLimit.h"
#include "app_canTx.h"
#include "app_canUtils.h"
#include "app_tractiveSystem.h"
#include "app_utils.h"

#define MAX_DISCHARGE_POWER_LIMIT_W 78.0e3f
#define MAX_CHARGE_POWER_LIMIT_W 15.0e3f
#define MAX_DISCHARGE_CURRENT_LIMIT (- MAX_TS_DISCHARGE_CURRENT_AMPS)
#define MIN_DISCHARGE_CURRENT_LIMIT 10.0f
#define TEMP_DERATING_THRESHOLD 50.0f

/**
 * @brief Gets the min discharge power Limit based on all of the temp
 * limiting condition
 * @return Minimum power current limit value
 */
float app_powerLimit_getDischargePowerLimit()
{
    const float max_cell_temp = app_segments_getMaxCellTemp().value;

    // Calculate power limit from temperature
    const float temp_power_limit = app_math_linearDerating(
        max_cell_temp, MAX_DISCHARGE_POWER_LIMIT_W, TEMP_DERATING_THRESHOLD, MAX_CELL_TEMP_WARNING_C, REDUCE_X);

    // Final power limit (capped at max)
    const float power_limit = MIN(temp_power_limit, MAX_DISCHARGE_POWER_LIMIT_W);

    // Determine limiting condition enum
    DischargePowerLimitCondition p_lim_condition = NO_DISCHARGE_POWER_LIMIT;

    if (max_cell_temp >= TEMP_DERATING_THRESHOLD)
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
    const float max_cell_temp = app_segments_getMaxCellTemp().value;

    // Calculate power limit from temperature
    const float temp_power_limit = app_math_linearDerating(
        max_cell_temp, MAX_CHARGE_POWER_LIMIT_W, TEMP_DERATING_THRESHOLD, MAX_CELL_TEMP_WARNING_C, REDUCE_X);

    // Final power limit (capped at max)
    const float power_limit = MIN(temp_power_limit, MAX_CHARGE_POWER_LIMIT_W);

    // Determine limiting condition enum
    ChargePowerLimitCondition p_lim_condition = NO_CHARGE_POWER_LIMIT;

    if (max_cell_temp >= TEMP_DERATING_THRESHOLD)
    {
        p_lim_condition = HIGH_TEMP_CHARGE_POWER_LIMIT;
    }

    // Broadcast limit condition over CAN
    app_canTx_BMS_ChargePowerLimitCondition_set(p_lim_condition);

    return power_limit;
}

/**
 * @brief Gets the min discharge current Limit based on all of the current
 * limiting conditions
 * @return Minimum Discharge current limit value
 */
float app_powerLimit_getDischargeCurrentLimit()
{
    return MAX_TS_DISCHARGE_CURRENT_AMPS;
}

/**
 * @brief Gets the min charge current limit based on all of the current
 * limiting conditions. Note discharge current here is positive.
 * @return Minimum Charge current limit value
 */
float app_powerLimit_getChargeCurrentLimit()
{
    return MAX_DISCHARGE_CURRENT_LIMIT;
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
    const float ts_voltage = app_tractiveSystem_getVoltage(); // Pack voltage would be better here but the sample rate
                                                              // is low so TS voltage is a good proxy
    float discharge_p_lim = MIN(app_powerLimit_getDischargePowerLimit(), discharge_c_lim * ts_voltage);
    float charge_p_lim    = MIN(app_powerLimit_getChargePowerLimit(), charge_c_lim * ts_voltage);

    // Enforce a minimum cutoff for safety
    if (discharge_p_lim / ts_voltage < MIN_DISCHARGE_CURRENT_LIMIT)
    {
        discharge_p_lim = MIN_DISCHARGE_CURRENT_LIMIT * ts_voltage;
    }

    // Broadcast limits
    app_canTx_BMS_DischargePowerLimit_set((uint32_t)discharge_p_lim);
    app_canTx_BMS_ChargePowerLimit_set((uint32_t)charge_p_lim);
}
