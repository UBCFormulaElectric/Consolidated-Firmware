#include "app_math.hpp"
// #include "app_segments.hpp"
#include "app_powerLimit.hpp"
#include "app_tractiveSystem.hpp"
#include <algorithm>

extern "C"
{
    #include "app_canTx.h"
    #include "app_canUtils.h"
}
#define MAX_DISCHARGE_POWER_LIMIT_W 78.0e3f
#define MAX_CHARGE_POWER_LIMIT_W 15.0e3f
#define MAX_DISCHARGE_CURRENT_LIMIT (-MAX_TS_DISCHARGE_CURRENT_AMPS)
#define MIN_DISCHARGE_CURRENT_LIMIT 10.0f
#define TEMP_DERATING_THRESHOLD 50.0f

// Definition placeholders until segments module is included
#define MAX_CELL_TEMP_WARNING_C (59.0f)


namespace app::plim
{

    /**
     * @brief Gets the min discharge power Limit based on all of the temp
     * limiting condition
     * @return Minimum power current limit value
     */
    float getDischargePowerLimit()
    {
        // const float max_cell_temp = app::segments::getMaxCellTemp().value;
        const float max_cell_temp = 0.0f; // PLACEHOLDER until segments included

        // Calculate power limit from temperature
        const float temp_power_limit = app::math::linearDerating(
            max_cell_temp, 
            MAX_DISCHARGE_POWER_LIMIT_W, 
            TEMP_DERATING_THRESHOLD, 
            MAX_CELL_TEMP_WARNING_C, 
            REDUCE_X
        );

        // Final power limit (capped at max) using std::min
        const float power_limit = std::min(temp_power_limit, MAX_DISCHARGE_POWER_LIMIT_W);

        // Determine limiting condition enum
        DischargePowerLimitCondition p_lim_condition = NO_DISCHARGE_POWER_LIMIT;

        if (max_cell_temp >= TEMP_DERATING_THRESHOLD)
        {
            p_lim_condition = HIGH_TEMP_DISCHARGE_POWER_LIMIT;
        }

        // Broadcast limit condition over CAN
        // Keeping C CAN API is standard
        app::canTx::BMS_DischargePowerLimitCondition_set(p_lim_condition);

        return power_limit;
    }

    /**
     * @brief Gets the min charge power Limit based on all of the temp
     * limiting condition
     * @return Minimum power current limit value
     */
    float getChargePowerLimit()
    {
        // const float max_cell_temp = app::segments::getMaxCellTemp().value;
        const float max_cell_temp = 0.0f; // Placeholder until segments included

        // Calculate power limit from temperature
        const float temp_power_limit = app::math::linearDerating(
            max_cell_temp, 
            MAX_CHARGE_POWER_LIMIT_W, 
            TEMP_DERATING_THRESHOLD, 
            MAX_CELL_TEMP_WARNING_C, 
            REDUCE_X
        );

        // Final power limit (capped at max)
        const float power_limit = std::min(temp_power_limit, MAX_CHARGE_POWER_LIMIT_W);

        // Determine limiting condition enum
        ChargePowerLimitCondition p_lim_condition = NO_CHARGE_POWER_LIMIT;

        if (max_cell_temp >= TEMP_DERATING_THRESHOLD)
        {
            p_lim_condition = HIGH_TEMP_CHARGE_POWER_LIMIT;
        }

        // Broadcast limit condition over CAN
        app::canTx::BMS_ChargePowerLimitCondition_set(p_lim_condition);

        return power_limit;
    }

    /**
     * @brief Gets the min discharge current Limit based on all of the current
     * limiting conditions
     * @return Minimum Discharge current limit value
     */
    float getDischargeCurrentLimit()
    {
        return MAX_TS_DISCHARGE_CURRENT_AMPS;
    }

    /**
     * @brief Gets the min charge current limit based on all of the current
     * limiting conditions. Note discharge current here is positive.
     * @return Minimum Charge current limit value
     */
    float getChargeCurrentLimit()
    {
        return MAX_DISCHARGE_CURRENT_LIMIT;
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
                                                                
        float discharge_p_lim = std::min(getDischargePowerLimit(), discharge_c_lim * ts_voltage);
        float charge_p_lim    = std::min(getChargePowerLimit(), charge_c_lim * ts_voltage);

        // Enforce a minimum cutoff for safety
        // Guard against potential divide by zero if voltage read is 0
        if (ts_voltage > 0.1f)
        {
            if ((discharge_p_lim / ts_voltage) < MIN_DISCHARGE_CURRENT_LIMIT)
            {
                discharge_p_lim = MIN_DISCHARGE_CURRENT_LIMIT * ts_voltage;
            }
        }

        // Broadcast limits
        // static_cast used for explicit type conversion
        app::canTx::BMS_DischargePowerLimit_set(static_cast<uint32_t>(discharge_p_lim));
        app::canTx::BMS_ChargePowerLimit_set(static_cast<uint32_t>(charge_p_lim));
    }

} // namespace app::plim