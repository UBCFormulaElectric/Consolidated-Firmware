#pragma once

#include <stdint.h>
#include "io_efuse.hpp"

namespace io
{
class TI_TPS25_Efuse final : public Efuse
{
  private:
    const hw::Gpio &pgood_gpio;

  public:
    explicit constexpr TI_TPS25_Efuse(
        const hw::Gpio &in_enable_gpio,
        const hw::Adc  &in_sns_adc_channel,
        const hw::Gpio &in_pgood)
      : Efuse(in_enable_gpio, in_sns_adc_channel), pgood_gpio(in_pgood)
    {
    }
    [[nodiscard]] float getChannelCurrent() override final;
    void reset() override final;
    [[nodiscard]] bool ok() override final;
};
} // namespace io
