#pragma once

#include <stdint.h>
#include "io_efuse.hpp"

namespace io::efuse
{
    class ST_Efuse : public Efuse
    {
        private:
          const hw::Gpio &stby_reset_gpio;
          union
          {
              struct
              {
                  uint8_t overload : 1;
                  uint8_t ovt_stp : 1;
                  uint8_t under_voltage : 1;
                  uint8_t short_to_vbat : 1;
                  uint8_t open_load_off_stat : 1;
                  uint8_t negative_output_voltage_clamp : 1;
                  uint8_t padding : 2;
              } flags;
              uint8_t raw;
          } faults;

        public:
          explicit ST_Efuse(const hw::Gpio &enable_gpio, const hw::Adc &sns_adc_channel, const hw::Gpio &stby_reset_gpio);
          void        setChannel(bool enabled) override;
          const bool  isChannelEnabled() override;
          const float getChannelCurrent() override;
          void        resetSet(bool set) override;
          void        reset() override;
          const bool  pgood() override;
          const bool  ok() override;
    };
}