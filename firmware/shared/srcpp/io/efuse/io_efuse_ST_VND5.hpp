#pragma once

#include <stdint.h>
#include "io_efuse.hpp"

namespace io
{
class ST_VND5_Efuse : public Efuse
{
  private:
    const hw::Gpio &stby_reset_gpio;
    union
    {
        struct
        {
            uint8_t overload : 1;
            uint8_t ovt_stg : 1;
            uint8_t under_voltage : 1;
            uint8_t short_to_vbat : 1;
            uint8_t open_load_off_stat : 1;
            uint8_t negative_output_voltage_clamp : 1;
            uint8_t padding : 2;
        } flags;
        uint8_t raw;
    } faults;

  public:
    explicit ST_VND5_Efuse(
        const hw::Gpio &enable_gpio,
        const hw::Adc  &sns_adc_channel,
        const hw::Gpio &stby_reset_gpio);
    void       reset() override;
    void       resetSet(const bool set);
    const bool ok() override;
};
} // namespace io