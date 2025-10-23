#pragma once
#include <stdbool.h>

#include "hw_gpio.hpp"
#include "hw_adc.hpp"

namespace io::efuse
{
    class Efuse
    {
        protected:
          static constexpr float ADC_VOLTAGE_TO_CURRENT_A = 1.720f;
          const hw::Gpio &enable_gpio;
          const hw::Adc  &sns_adc_channel;

        public:
          Efuse(const hw::Gpio &enable_gpio, const hw::Adc &sns_adc_channel)
            : enable_gpio(enable_gpio), sns_adc_channel(sns_adc_channel) {}
          virtual ~Efuse()                             = default;
          void        setChannel(bool enabled);
          const bool  isChannelEnabled();
          const float getChannelCurrent();
          virtual void        reset()                  = 0;
          virtual const bool  ok()                     = 0;
    };
}