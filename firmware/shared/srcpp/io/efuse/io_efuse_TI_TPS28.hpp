#pragma once

#include <stdint.h>
#include "io_efuse.hpp"

namespace io
{
class TI_TPS28_Efuse final : public Efuse
{
  private:
    const hw::Gpio &fault_gpio;
    const hw::Gpio &diag_en_gpio;

    /* Portable bit-fields: use 'unsigned' and name the subgroup */
    union
    {
        struct
        {
            uint8_t overcurrent : 1;
            uint8_t thermal_shdn : 1;
            uint8_t padding : 6;
        } flags;
        uint8_t raw;
    } faults;

  public:
    explicit consteval TI_TPS28_Efuse(
        const hw::Gpio &in_enable_gpio,
        const hw::Adc  &in_sns_adc_channel,
        const hw::Gpio &in_fault_gpio,
        const hw::Gpio &in_diag_en_gpio)
      : Efuse(in_enable_gpio, in_sns_adc_channel), fault_gpio(in_fault_gpio), diag_en_gpio(in_diag_en_gpio)
    {
    }
    void reset() override final;
    bool ok() override final;
};
} // namespace io
