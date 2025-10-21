#pragma once

#include <stdint.h>
#include "io_efuse.hpp"

namespace io::efuse
{
    class TI_Efuse : public Efuse
    {
        private:
          const hw::Gpio& pgood_gpio;

        public:
          explicit TI_Efuse(const hw::Gpio &enable_gpio, const hw::Adc &sns_adc_channel, const hw::Gpio& pgood);
          void        setChannel(bool enabled) override;
          const bool  isChannelEnabled() override;
          const float getChannelCurrent() override;
          void        resetSet(bool set) override;
          void        reset() override;
          const bool  pgood() override;
          const bool  ok() override;
    };
}