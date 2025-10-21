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
          virtual void        setChannel(bool enabled) = 0;
          virtual const bool  isChannelEnabled()       = 0;
          virtual const float getChannelCurrent()      = 0;
          virtual void        resetSet(bool set)       = 0;
          virtual void        reset()                  = 0;
          virtual const bool  pgood()                  = 0;
          virtual const bool  ok()                     = 0;
    };
}