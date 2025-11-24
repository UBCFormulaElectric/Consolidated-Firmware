#include "io_efuse.hpp"
#include "hw_gpio.hpp"
#include "hw_adc.hpp"

namespace io::efuse
{
Efuse::Efuse(const hw::Gpio &enable_gpio, const hw::Adc &sns_adc_channel)
  : enable_gpio(enable_gpio), sns_adc_channel(sns_adc_channel)
{
}

void Efuse::setChannel(bool enabled)
{
    this->enable_gpio.writePin(enabled);
}

const bool Efuse::isChannelEnabled()
{
    return this->enable_gpio.readPin();
}

const float Efuse::getChannelCurrent()
{
    return this->sns_adc_channel.getVoltage() * ADC_VOLTAGE_TO_CURRENT_A;
}
} // namespace io::efuse