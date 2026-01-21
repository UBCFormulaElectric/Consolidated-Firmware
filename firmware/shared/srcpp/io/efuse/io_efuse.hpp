#pragma once
#include <stdbool.h>

#include "hw_gpio.hpp"
#include "hw_adc.hpp"

namespace io
{
class Efuse
{
  protected:
    static constexpr float ADC_VOLTAGE_TO_CURRENT_A = 1.720f;
    const hw::Gpio        &enable_gpio;
    const hw::Adc         &sns_adc_channel;

  public:
    explicit constexpr Efuse(const hw::Gpio &in_enable_gpio, const hw::Adc &in_sns_adc_channel)
      : enable_gpio(in_enable_gpio), sns_adc_channel(in_sns_adc_channel)
    {
    }
    virtual ~Efuse() = default;
    void         setChannel(bool enabled) { enable_gpio.writePin(enabled); }
    bool         isChannelEnabled() const { return this->enable_gpio.readPin(); }
    float        getChannelCurrent() { return this->sns_adc_channel.getVoltage() * ADC_VOLTAGE_TO_CURRENT_A; }
    virtual void reset() = 0;
    virtual bool ok()    = 0;
};
} // namespace io