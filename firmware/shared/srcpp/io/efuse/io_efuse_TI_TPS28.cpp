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
static constexpr float R_PD  = 24.9f; // kOhm
// TODO: is this actualy a linear interpolation??
static constexpr float K_CL  = (((60.0f - 40.0f) / (50.0f - 10.0f)) * R_PD); // A*kOhm
static constexpr float I_LIM = (K_CL / R_PD);

// There are different V_SNSFH thresholds if DIAG_EN gpio is set to LOw (3.3V) or HIGH (5V)
static constexpr float V_SNSFH_MIN = 3.5f;
static constexpr float V_SNSFH     = 3.95f;
static constexpr float V_SNSFH_MAX = 4.4f;

TI_TPS28_Efuse::TI_TPS28_Efuse(
    const hw::Gpio &enable_gpio,
    const hw::Adc  &sns_adc_channel,
    const hw::Gpio &fault_gpio,
    const hw::Gpio &diag_en_gpio)
  : Efuse(enable_gpio, sns_adc_channel), fault_gpio(fault_gpio), diag_en_gpio(diag_en_gpio), faults{}
{
}

// TODO: verify reset function
void TI_TPS28_Efuse::reset()
{
    this->enable_gpio.writePin(false);
    this->enable_gpio.writePin(true);
    this->enable_gpio.writePin(false);
}

const bool TI_TPS28_Efuse::ok()
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
     * Thus we only dont check for overcurrent if it's disabled
     *
     * TODO: but Table 8-3 contradicts this
     */

    // if diagnostics are not enabled, only check TSD and ILIM
    const bool is_faulted = this->fault_gpio.readPin();
    if (diag_enabled)
    {
        if (is_faulted)
        {
            this->faults.flags.overcurrent = (channel_enabled && IS_IN_RANGE(V_SNSFH_MIN, V_SNSFH_MAX, voltage));
            this->faults.flags.thermal_shdn =
                (channel_enabled && IS_IN_RANGE(V_SNSFH_MIN, V_SNSFH_MAX, voltage));
        }
        else
        {
            this->faults.raw = 0x00U;
        }
    }
    else
    {
        this->faults.raw = 0x03U;
    }

    return !(this->faults.raw > 0U);
}
} // namespace io
