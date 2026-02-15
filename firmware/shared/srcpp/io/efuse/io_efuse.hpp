#pragma once
#include <stdbool.h>

#include "hw_gpio.hpp"
#include "hw_adc.hpp"

namespace io
{
class Efuse
{
  protected:
    const hw::Gpio        &enable_gpio;
    const hw::Adc         &sns_adc_channel;

  public:
    explicit constexpr Efuse(const hw::Gpio &in_enable_gpio, const hw::Adc &in_sns_adc_channel)
      : enable_gpio(in_enable_gpio), sns_adc_channel(in_sns_adc_channel)
    {
    }
    virtual ~Efuse() = default;
    /**
     * @brief Enable the efuse 
     * 
     * @param enabled 
     */
    void         setChannel(bool enabled) { enable_gpio.writePin(enabled); }

    /**
     * @brief Check if efuse is enabled
     * 
     * @return true if enabled, false otherwise
     */
    [[nodiscard]] bool         isChannelEnabled() const { return this->enable_gpio.readPin(); }

    /**
     * @brief Read the current flowing through the efuse
     * 
     * @return current flowing through efuse
     */
    [[nodiscard]] virtual float        getChannelCurrent() = 0;

    /**
     * @brief Reset the efuse
     */
    virtual void reset() = 0;

    /**
     * @brief Check if the efuse has faulted
     * 
     * @return true if efuse is ok, false if faulted
     */
    [[nodiscard]] virtual bool ok()    = 0;
};
} // namespace io