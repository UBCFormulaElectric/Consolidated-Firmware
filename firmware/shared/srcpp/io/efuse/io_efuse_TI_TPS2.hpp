#pragma once

#include <stdint.h>
#include "io_efuse.hpp"

namespace io
{
    class ST_TPS2_Efuse : public Efuse
    {
        private:
          const hw::Gpio& pgood_gpio;

        public:
          explicit ST_TPS2_Efuse(const hw::Gpio &enable_gpio, const hw::Adc &sns_adc_channel, const hw::Gpio& pgood);
          void        reset() override;
          const bool  pgood();
          const bool  ok() override;
    };
}