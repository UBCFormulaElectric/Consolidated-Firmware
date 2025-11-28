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
    explicit TI_TPS25_Efuse(const hw::Gpio &enable_gpio, const hw::Adc &sns_adc_channel, const hw::Gpio &pgood);
    void       reset() override final;
    const bool pgood();
    const bool ok() override final;
};
} // namespace io
