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
    explicit TI_TPS28_Efuse(
        const hw::Gpio &enable_gpio,
        const hw::Adc  &sns_adc_channel,
        const hw::Gpio &fault_gpio,
        const hw::Gpio &diag_en_gpio);
    void       reset() override final;
    const bool ok() override final;
};
} // namespace io
