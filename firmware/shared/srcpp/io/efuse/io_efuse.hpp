#pragma once
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_gpio.hpp"
#include "hw_adc.hpp"
#endif

namespace io
{
class Efuse
{
  protected:
#ifdef TARGET_EMBEDDED
    const hw::Gpio &enable_gpio;
    const hw::Adc  &sns_adc_channel;
#endif

  public:
#ifdef TARGET_EMBEDDED
    explicit constexpr Efuse(const hw::Gpio &in_enable_gpio, const hw::Adc &in_sns_adc_channel)
      : enable_gpio(in_enable_gpio), sns_adc_channel(in_sns_adc_channel)
    {
    }

    /**
     * @brief Enable the efuse
     *
     * @param enabled
     */
    void setChannel(bool enabled) { enable_gpio.writePin(enabled); }

    /**
     * @brief Check if efuse is enabled
     *
     * @return true if enabled, false otherwise
     */
    [[nodiscard]] bool isChannelEnabled() const { return this->enable_gpio.readPin(); }

    /**
     * @brief Read the current flowing through the efuse
     *
     * @return current flowing through efuse
     */
    [[nodiscard]] virtual float getChannelCurrent() = 0;

    /**
     * @brief Reset the efuse
     */
    virtual void reset() = 0;

    /**
     * @brief Check if the efuse has faulted
     *
     * @return true if efuse is ok, false if faulted
     */
    [[nodiscard]] virtual bool ok() = 0;
#else
    explicit constexpr Efuse() {}
    bool  enabled = false;
    bool  fault   = false;
    float current = 0.0f;

    void setChannelCurrent(float fake_current) { current = fake_current; }

    void setFault(bool fake_fault) { fault = fake_fault; }

    void setChannel(bool fake_enabled) { enabled = fake_enabled; }

    [[nodiscard]] bool isChannelEnabled() const { return enabled; }

    [[nodiscard]] float getChannelCurrent() { return current; }

    void reset() { return; }

    [[nodiscard]] bool ok() { return !fault; }
#endif

    virtual ~Efuse() = default;
};
} // namespace io