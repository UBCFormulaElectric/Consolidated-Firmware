#pragma once

#include <stdint.h>
#include "io_efuse.hpp"

namespace io
{
class ST_VND5_Efuse final : public Efuse
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
    explicit constexpr ST_VND5_Efuse(
        const hw::Gpio &in_enable_gpio,
        const hw::Adc  &in_sns_adc_channel,
        const hw::Gpio &in_stby_reset_gpio)
      : Efuse(in_enable_gpio, in_sns_adc_channel), stby_reset_gpio(in_stby_reset_gpio)
    {
    }
    void reset() override final;
    void resetSet(const bool set);
    bool ok() override final;
};
} // namespace io
