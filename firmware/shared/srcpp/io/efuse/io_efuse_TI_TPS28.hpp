#pragma once

#include <stdint.h>
#include "io_efuse.hpp"

namespace io
{
class TI_TPS28_Efuse final : public Efuse
{
  public:
    /* Portable bit-fields: use 'unsigned' and name the subgroup */
    union TPS28_Faults
    {
        struct __attribute__((packed))
        {
            uint8_t overcurrent : 1;
            uint8_t thermal_shdn : 1;
            uint8_t padding : 6;
        } flags;
        uint8_t raw = 0U;
    };

  private:
#ifdef TARGET_EMBEDDED
    const hw::Gpio &pgood_gpio;
    const hw::Gpio &diag_en_gpio;
#endif
    TPS28_Faults faults{};

  public:
#ifdef TARGET_EMBEDDED
    explicit constexpr TI_TPS28_Efuse(
        const hw::Gpio &in_enable_gpio,
        const hw::Adc  &in_sns_adc_channel,
        const hw::Gpio &in_pgood_gpio,
        const hw::Gpio &in_diag_en_gpio)
      : Efuse(in_enable_gpio, in_sns_adc_channel), pgood_gpio(in_pgood_gpio), diag_en_gpio(in_diag_en_gpio)
    {
    }
    [[nodiscard]] float getChannelCurrent() override final;
    void                reset() override final;
    [[nodiscard]] bool  ok() override final;

    /**
     * @brief Read the specific faults of the efuse
     *
     * @return
     */
    [[nodiscard]] TPS28_Faults readFaults() const;
#else
    explicit constexpr TI_TPS28_Efuse() {}
#endif
};
} // namespace io
