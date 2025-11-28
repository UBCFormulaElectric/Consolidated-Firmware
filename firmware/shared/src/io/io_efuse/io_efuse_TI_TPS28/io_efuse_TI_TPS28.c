#include "app_utils.h"
#include "io_efuse_TI_TPS28.h"
#include "io_efuse/io_efuse_datatypes.h"
#include "hw_gpio.h"
#include <stdbool.h>
#include <assert.h>

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

#define R_PD 24.9f // kOhm
// TODO: is this actualy a linear interpolation??
#define K_CL (((60.0f - 40.0f) / (50.0f - 10.0f)) * R_PD) // A*kOhm
#define I_LIM (K_CL / R_PD)

// There are different V_SNSFH thresholds if DIAG_EN gpio is set to LOw (3.3V) or HIGH (5V)
#define V_SNSFH_MIN 3.5f
#define V_SNSFH 3.95f
#define V_SNSFH_MAX 4.4f

static void  io_TI_TPS28_Efuse_setChannel(const Efuse *channel, bool enabled);
static bool  io_TI_TPS28_Efuse_isChannelEnabled(const Efuse *channel);
static float io_TI_TPS28_Efuse_getChannelCurrent(const Efuse *channel);
static void  io_TI_TPS28_Efuse_reset(const Efuse *efuse);
static bool  io_TI_TPS28_Efuse_pgood(const Efuse *efuse);

const EfuseFunctions TI_TPS28_Efuse_functions = { .set_channel          = io_TI_TPS28_Efuse_setChannel,
                                                  .is_channel_enabled   = io_TI_TPS28_Efuse_isChannelEnabled,
                                                  .get_channel_current  = io_TI_TPS28_Efuse_getChannelCurrent,
                                                  .loadswitch_reset_set = NULL,
                                                  .reset_efuse          = io_TI_TPS28_Efuse_reset,
                                                  .pgood                = NULL,
                                                  .efuse_ok             = io_TI_TPS28_Efuse_ok };

static void io_TI_TPS28_Efuse_setChannel(const Efuse *channel, bool enabled)
{
    assert(channel->enable_gpio != NULL);
    hw_gpio_writePin(channel->enable_gpio, enabled);
}

static bool io_TI_TPS28_Efuse_isChannelEnabled(const Efuse *channel)
{
    assert(channel->enable_gpio != NULL);
    return hw_gpio_readPin(channel->enable_gpio);
}

static bool io_TI_TPS28_Efuse_isDiagEnabled(const Efuse *channel)
{
    assert(channel->ti_tps28->diag_en_gpio != NULL);
    return hw_gpio_readPin(channel->ti_tps28->diag_en_gpio);
}

static float io_TI_TPS28_Efuse_getChannelCurrent(const Efuse *channel)
{
    const AdcChannel *current_sense = channel->sns_adc_channel;
    assert(current_sense != NULL);
    return hw_adc_getVoltage(current_sense) * ADC_VOLTAGE_TO_CURRENT_A;
}
// TODO: verify reset function
static void io_TI_TPS28_Efuse_reset(const Efuse *efuse)
{
    assert(efuse->enable_gpio != NULL);

    hw_gpio_writePin(efuse->enable_gpio, false);
    hw_gpio_writePin(efuse->enable_gpio, true);
    hw_gpio_writePin(efuse->enable_gpio, false);
}

static bool io_TI_TPS28_Efuse_ok(const Efuse *efuse)
{
    assert(efuse != NULL);

    bool  channel_enabled = io_TI_TPS28_Efuse_isChannelEnabled(efuse);
    bool  diag_enabled    = hw_gpio_readPin(efuse->ti_tps28->diag_en_gpio);
    float voltage         = io_TI_TPS28_Efuse_getChannelCurrent(efuse) / ADC_VOLTAGE_TO_CURRENT_A;

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
    bool is_faulted = hw_gpio_readPin(efuse->ti_tps28->fault_gpio);
    if (diag_enabled)
    {
        if (is_faulted)
        {
            // Determine specific faults
            efuse->ti_tps28->faults.flags.overcurrent =
                (channel_enabled && IS_IN_RANGE(V_SNSFH_MIN, V_SNSFH_MAX, voltage));
            efuse->ti_tps28->faults.flags.thermal_shdn =
                (channel_enabled && IS_IN_RANGE(V_SNSFH_MIN, V_SNSFH_MAX, voltage));
        }
        else
        {
            efuse->ti_tps28->faults.raw = 0x00U;
        }
    }
    else
    {
        efuse->ti_tps28->faults.raw = 0x03;
    }

    return !(efuse->ti_tps28->faults.raw > 0);
}
