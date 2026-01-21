extern "C"
{
#include "app_utils.h"
}

#include "io_efuse_TI_TPS28.hpp"
#include "hw_gpio.hpp"

/**
 * TI_TPS28 Efuse Datasheet:
 * https://www.ti.com/lit/ds/symlink/tps281c30.pdf?ts=1763641186988&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTPS281C30
 */

/**
 * The Current limit (I_LIM) is set attaching the ILIM pinto a pulldown resistor.
 *
 * The pulldown resistor must be between 10kOhm - 50kOhm
 * The current limit ration K_CL is:
 *      40 A*kOhm - 60 A*kOhm (Rev A or C)
 *      80 A*kOhm - 120 A*kOhm (Rev B, D, or C)
 */

namespace io
{
static constexpr float R_PD = 24.9f; // kOhm

static constexpr float K_CL_MIN = 40.0f; // A*kOhm
static constexpr float K_CL_TYP = 50.0f; // A*kOhm
static constexpr float K_CL_MAX = 60.0f; // A*kOhm

static constexpr float I_LIM_MIN = (K_CL_MIN / R_PD);
static constexpr float I_LIM_TYP = (K_CL_TYP / R_PD);
static constexpr float I_LIM_MAX = (K_CL_MAX / R_PD);

// There are different V_SNSFH thresholds if DIAG_EN gpio is set to LOw (3.3V) or HIGH (5V)
static constexpr float V_SNSFH_MIN = 3.5f;
static constexpr float V_SNSFH     = 3.95f;
static constexpr float V_SNSFH_MAX = 4.4f;

void TI_TPS28_Efuse::enableDiagnostics(bool enable)
{
    this->diag_en_gpio.writePin(enable);
}

// TODO: verify reset function
void TI_TPS28_Efuse::reset()
{
    this->enable_gpio.writePin(false);
    this->enable_gpio.writePin(true);
    this->enable_gpio.writePin(false);
}

bool TI_TPS28_Efuse::ok()
{
    const bool  channel_enabled = this->isChannelEnabled();
    const bool  diag_enabled    = this->diag_en_gpio.readPin();
    const float voltage         = this->getChannelCurrent() / ADC_VOLTAGE_TO_CURRENT_A;

    /**
     * Note: Table 8.2 DIAG_EN Logic Table
     *
     * If DIAG_EN is low, FAULT will continue to indicate
     * TSD (thermal_shdn) or ILIM (forgor what this is)
     *
     * So technically we read the same faults regardless if
     * diag_en is asserted high or not. To be safe we will
     * force
     */

    // if diagnostics are not enabled, only check TSD and ILIM
    // fault is asserted low
    // TODO: Add back fault gpio
    // const bool is_ok = this->fault_gpio.readPin();
    const bool is_ok = true;
    if (diag_enabled && (is_ok == false))
    {
        this->faults.flags.overcurrent  = (channel_enabled && IS_IN_RANGE(V_SNSFH_MIN, V_SNSFH_MAX, voltage));
        this->faults.flags.thermal_shdn = (channel_enabled && IS_IN_RANGE(V_SNSFH_MIN, V_SNSFH_MAX, voltage));
    }

    return is_ok;
}
} // namespace io
