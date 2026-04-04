// BIG TODO: Negative for discharge, positive for charge
// BIG TODO: just change naming in BMS_tx.json and BMS_enum.json

#include "app_math.hpp"
#include "app_segments.hpp"
#include "app_clim.hpp"
#include "app_tractiveSystem.hpp"
#include <algorithm>
#include <cstdint>
#include "app_canTx.hpp"
#include "app_canUtils.hpp"

#define MAX_DISCHARGE_POWER_LIMIT_W (78.0e3f)

// Definition placeholders until segments module is included
#define TEMP_DERATING_THRESHOLD (50.0f)
#define MAX_CELL_TEMP_WARNING_C (60.0f)

// Molicel INR-18650-P30B cell constants
#define INTERNAL_R_PER_CELL_OHMS (17.0e-3f)
#define SERIES_ELEMENT_RESISTANCE (INTERNAL_R_PER_CELL_OHMS / CELLS_IN_PARALLEL)
#define LOW_VOLTAGE_CUTOFF_V (2.5f)
#define LOW_VOLTAGE_DERATING_THRESHOLD_V (3.0f)
#define HIGH_VOLTAGE_CUTOFF_V (4.2f) 
#define HIGH_VOLTAGE_DERATING_THRESHOLD_V (4.0f)

using namespace app::can_utils;

namespace app::clim
{

/**
 * @todo implement SOC into clim algo
 * @brief Gets the min discharge power Limit based on all of the temp
 * limiting condition
 * @return Minimum power current limit value
 */
float getDischargeClim()
{
    float max_cell_temp = app::segments::getMaxCellTemp();
    float min_cell_voltage = app::segments::getMinCellVoltage();
    float pack_voltage = app::ts::getVoltage();

    // Default limited by max power or max current
    float max_discharge_clim = std::min(MAX_TS_DISCHARGE_CURRENT_AMPS, MAX_DISCHARGE_POWER_LIMIT_W / pack_voltage);

    float active_clim = max_discharge_clim;
    DischargeClimCondition active_condition = DischargeClimCondition::NO_LIMIT;

    // limited by temp?
    float temp_clim = app::math::linearDerating(
        max_cell_temp, MAX_DISCHARGE_POWER_LIMIT_W, TEMP_DERATING_THRESHOLD, MAX_CELL_TEMP_WARNING_C, REDUCE_X) / pack_voltage;

    if (temp_clim < active_clim) {
        active_clim= temp_clim;
        active_condition = DischargeClimCondition::HIGH_TEMPERATURE_LIMIT;
    }

    // limited by voltage?
    float voltage_clim = app::math::linearDerating(
        min_cell_voltage, -MAX_TS_DISCHARGE_CURRENT_AMPS, LOW_VOLTAGE_DERATING_THRESHOLD_V, LOW_VOLTAGE_CUTOFF_V, !REDUCE_X);

    if (voltage_clim < active_clim) {
        active_clim = voltage_clim;
        active_condition = DischargeClimCondition::LOW_VOLTAGE_LIMIT;

    // Broadcase enum
    app::can_tx::BMS_DischargePowerLimitCondition_set(active_condition);

    return active_clim;
}

/**
 * @todo implement SOC into limiting algo
 * @brief Gets the min charge power Limit based on all of the temp
 * limiting condition
 * @return Minimum power current limit value
 */
float getChargeClim()
{
    float max_cell_temp = app::segments::getMaxCellTemp();
    float max_cell_voltage = app::segments::getMaxCellVoltage()
    float pack_voltage = app::ts::getVoltage();

    // Default limited by max power or max current
    float max_charge_clim = MAX_TS_CHARGE_CURRENT_AMPS;

    float active_clim = max_charge_clim;
    ChargeClimCondition active_condition = ChargeClimCondition::NO_LIMIT;

    // limited by temp?
    float temp_clim = app::math::linearDerating(
        max_cell_temp, MAX_TS_CHARGE_CURRENT_AMPS, TEMP_DERATING_THRESHOLD, MAX_CELL_TEMP_WARNING_C, REDUCE_X);

    if (temp_clim < active_clim) {
        active_clim = temp_clim;
        active_condition = ChargeClimCondition::HIGH_TEMPERATURE_LIMIT;
    }

    // limited by voltage?
    float voltage_clim = app::math::linearDerating(
        max_cell_voltage, MAX_TS_CHARGE_CURRENT_AMPS,
        HIGH_VOLTAGE_DERATING_THRESHOLD_V, HIGH_VOLTAGE_CUTOFF_V, REDUCE_X);

    if (voltage_clim < active_clim) {
        active_clim = voltage_clim;
        active_condition = ChargeClimCondition::HIGH_VOLTAGE_LIMIT;
    }

    // Broadcast enum
    app::can_tx::BMS_ChargeClimCondition_set(active_condition);

    return active_clim;
}

void broadcast()
{
    // Broadcast discharge clim
    app::can_tx::BMS_DischargeClim_set(static_cast<uint32_t>(getDischargeClim())); 
    // Broadcast charge clim
    app::can_tx::BMS_ChargeClim_set(static_cast<uint32_t>(getChargeClim()));
}

} // namespace app::clim