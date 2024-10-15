#include "io_efuse.h"
#include <assert.h>

#include "hw_gpio.h"

constexpr float ADC_VOLTAGE_TO_CURRENT_A             = 1.720f;
static bool     enabled_channels[NUM_EFUSE_CHANNELS] = { false };

namespace io::efuse
{
const Efuse shdn{ hw::gpio::shdn_pwr_en, hw::gpio::fr_stby1, hw::adc::shdn_pwr_i_sns_adc };
const Efuse lv{ hw::gpio::shdn_pwr_en, hw::gpio::fr_stby1, hw::adc::shdn_pwr_i_sns_adc };
const Efuse pump{ hw::gpio::pump_pwr_en, hw::gpio::fr_stby_2, hw::adc::pump_pwr_i_sns_adc };
const Efuse aux{ hw::gpio::aux_pwr_en, hw::gpio::fr_stby2, hw::adc::aux_pwr_i_sns_adc };
const Efuse inv_r{ hw::gpio::inv_r_pwr_en, hw::gpio::fr_stby3, hw::adc::inv_r_pwr_i_sns_adc };
const Efuse inv_l{ hw::gpio::inv_l_pwr_en, hw::gpio::fr_stby3, hw::adc::inv_l_pwr_i_sns_adc };
const Efuse telem{ hw::gpio::telem_pwr_en, nullptr, hw::adc::AdcChannel::NO_ADC_CHANNEL };
const Efuse buzzer{ hw::gpio::buzzer_pwr_en, nullptr, hw::adc::AdcChannel::NO_ADC_CHANNEL };

Efuse &getEfuseConfig(EfuseChannel channel)
{
    switch (channel)
    {
        case EFUSE_SHDN:
            return shdn;
        case EFUSE_LV:
            return lv;
        case EFUSE_PUMP:
            return pump;
        case EFUSE_AUX:
            return aux;
        case EFUSE_INV_R:
            return inv_r;
        case EFUSE_INV_L:
            return inv_l;
        case EFUSE_TELEM:
            return telem;
        case EFUSE_BUZZER:
            return buzzer;
        default:
            assert(false && "Invalid Efuse Channel!");
            return nullptr;
    }
}

void setChannel(EfuseChannel channel, bool enabled)
{
    assert(channel < NUM_EFUSE_CHANNELS);
    enabled_channels[channel] = enabled;
    const Efuse &config       = getEfuseConfig(channel);

    if (config.enable_gpio != nullptr)
    {
        hw::gpio::config.enable_gpio.writePin(enabled);
    }
}

bool isChannelEnabled(const EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);

    return enabled_channels[channel];
}

float getChannelCurrent(const EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);
    const Efuse &config = getEfuseConfig(channel);

    return hw::adc::(configs[channel].cur_sns_adc_channel) * ADC_VOLTAGE_TO_CURRENT_A;
}

void standbyReset(EfuseChannel channel)
{
    assert(channel < NUM_EFUSE_CHANNELS);
    const Efuse &config = getEfuseConfig(channel);

    // Low pulse on standby reset line delatches faults
    hw::gpio::config.stby_reset_gpio.writePin(true);
    hw::gpio::config.stby_reset_gpio.writePin(false);
    hw::gpio::config.stby_reset_gpio.writePin(true);
}
} // namespace io::efuse