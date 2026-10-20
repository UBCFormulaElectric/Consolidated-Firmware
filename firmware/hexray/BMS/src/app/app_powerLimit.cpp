#include "app_math.hpp"
#include "app_segments.hpp"
#include "app_powerLimit.hpp"
#include "app_tractiveSystem.hpp"
#include "io_semaphore.hpp"
#include <algorithm>
#include <cstdint>
#include "app_canTx.hpp"
#include "app_canUtils.hpp"

#define MAX_DISCHARGE_POWER_LIMIT_W 78.0e3f
#define MAX_CHARGE_POWER_LIMIT_W 15.0e3f
#define TEMP_DERATING_THRESHOLD_C 50.0f
#define MAX_CELL_TEMP_WARNING_C 55.0f
#define LOW_VOLT_DERATING_THRESHOLD_V 3.0f
#define MIN_CELL_VOLTAGE_WARNING_V 2.7f
#define HIGH_VOLT_DERATING_THRESHOLD_V 4.1f
#define MAX_CELL_VOLTAGE_WARNING_V 4.17f

using namespace app::can_utils;

namespace app::plim
{

/**
 * @brief Gets the min discharge power Limit based on all of the temp and
 * low cell voltage limiting conditions
 * @return Minimum power current limit value
 */
float getDischargePowerLimit()
{
    float max_cell_temp;
    float min_cell_voltage;
    {
        const io::unique_semaphore s{ shared_lock };
        max_cell_temp    = app::segments::shared::getMaxCellTemperature().value;
        min_cell_voltage = app::segments::shared::getMinCellVoltage().value;
    }

    // Calculate power limit from temperature
    const float temp_power_limit = app::math::linearDerating(
        max_cell_temp, MAX_DISCHARGE_POWER_LIMIT_W, TEMP_DERATING_THRESHOLD_C, MAX_CELL_TEMP_WARNING_C, REDUCE_X);

    // Calculate power limit from min cell voltage
    const float volt_power_limit = app::math::linearDerating(
        min_cell_voltage, MAX_DISCHARGE_POWER_LIMIT_W, LOW_VOLT_DERATING_THRESHOLD_V, MIN_CELL_VOLTAGE_WARNING_V,
        INCREASE_X);

    // Final power limit (capped at max) using std::min
    const float power_limit = std::min({ temp_power_limit, volt_power_limit });

    // Report which derating is holding power below max, if any
    DischargePowerLimitCondition p_lim_condition = DischargePowerLimitCondition::NO_DISCHARGE_POWER_LIMIT;

    if (power_limit < MAX_DISCHARGE_POWER_LIMIT_W)
    {
        p_lim_condition = volt_power_limit < temp_power_limit
                              ? DischargePowerLimitCondition::LOW_VOLT_DISCHARGE_POWER_LIMIT
                              : DischargePowerLimitCondition::HIGH_TEMP_DISCHARGE_POWER_LIMIT;
    }

    app::can_tx::BMS_DischargePowerLimitCondition_set(p_lim_condition);

    return power_limit;
}

/**
 * @brief Gets the min charge power Limit based on all of the temp and
 * high cell voltage limiting conditions
 * @return Minimum power current limit value
 */
float getChargePowerLimit()
{
    float max_cell_temp;
    float max_cell_voltage;
    {
        const io::unique_semaphore s{ shared_lock };
        max_cell_temp    = app::segments::shared::getMaxCellTemperature().value;
        max_cell_voltage = app::segments::shared::getMaxCellVoltage().value;
    }

    // Calculate power limit from temperature
    const float temp_power_limit = app::math::linearDerating(
        max_cell_temp, MAX_CHARGE_POWER_LIMIT_W, TEMP_DERATING_THRESHOLD_C, MAX_CELL_TEMP_WARNING_C, REDUCE_X);

    // Calculate power limit from max cell voltage
    const float volt_power_limit = app::math::linearDerating(
        max_cell_voltage, MAX_CHARGE_POWER_LIMIT_W, HIGH_VOLT_DERATING_THRESHOLD_V, MAX_CELL_VOLTAGE_WARNING_V,
        REDUCE_X);

    // Final power limit (capped at max)
    const float power_limit = std::min({ temp_power_limit, volt_power_limit });

    // Report which derating is holding power below max, if any
    ChargePowerLimitCondition p_lim_condition = ChargePowerLimitCondition::NO_CHARGE_POWER_LIMIT;

    if (power_limit < MAX_CHARGE_POWER_LIMIT_W)
    {
        p_lim_condition = volt_power_limit < temp_power_limit
                              ? ChargePowerLimitCondition::HIGH_VOLT_CHARGE_POWER_LIMIT
                              : ChargePowerLimitCondition::HIGH_TEMP_CHARGE_POWER_LIMIT;
    }

    // Broadcast limit condition over CAN
    app::can_tx::BMS_ChargePowerLimitCondition_set(p_lim_condition);

    return power_limit;
}

/**
 * @brief Gets the min discharge current Limit based on all of the current
 * limiting conditions
 * @return Minimum Discharge current limit value
 */
float getDischargeCurrentLimit()
{
    return -MAX_TS_DISCHARGE_CURRENT_AMPS;
}

/**
 * @brief Gets the min charge current limit based on all of the current
 * limiting conditions
 * @return Minimum Charge current limit value
 */
float getChargeCurrentLimit()
{
    return MAX_TS_CHARGE_CURRENT_AMPS;
}

// TODO: implement this once app SOC is merged with master - see quadrina current limit pr

/**
 * @brief Gets the Low Voltage Clamp based discharge current limtit
 * @return Low Voltage Clamp based discharge current Limit value
 */
float lowVoltageClampCurrentLimit()
{
    return -1.0f;
}

// TODO: implement this once app SOC is merged with master - see quadrina current limit pr

/**
 * @brief Gets the High Voltage Clamp based charge current limit
 * @return High Voltage Clamp based Charge current Limit value
 */
float highVoltageClampCurrentLimit()
{
    return -1.0f;
}

// TODO: implement this - see quadrina current limit pr

/**
 * @brief Gets the Low SOC based discharge current limit
 * @return Low SOC based discharge current Limit value
 */
float lowSOCCurrentLimit()
{
    return -1.0f;
}

// TODO: implement this - see quadrina current limit pr

/**
 * @brief Gets the High SOC based charge current limit
 * @return High SOC based charge current Limit value
 */
float highSOCCurrentLimit()
{
    return -1.0f;
}

void broadcast()
{
    // Get current limits
    float discharge_c_lim = getDischargeCurrentLimit();
    float charge_c_lim    = getChargeCurrentLimit();

    // Get power limits
    // Assuming app::ts or app_tractiveSystem for getVoltage
    const float ts_voltage = app::ts::getVoltage();

    const float discharge_p_lim = std::min(getDischargePowerLimit(), discharge_c_lim * ts_voltage);
    const float charge_p_lim    = std::min(getChargePowerLimit(), charge_c_lim * ts_voltage);

    // Broadcast limits
    app::can_tx::BMS_DischargePowerLimit_set(static_cast<uint32_t>(discharge_p_lim));
    app::can_tx::BMS_ChargePowerLimit_set(static_cast<uint32_t>(charge_p_lim));
}

} // namespace app::plim