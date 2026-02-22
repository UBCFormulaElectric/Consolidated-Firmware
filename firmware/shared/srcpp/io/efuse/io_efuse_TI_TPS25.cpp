#include <assert.h>
#include "io_efuse_TI_TPS25.hpp"
#include "hw_gpio.hpp"

namespace io
{

static constexpr float ADC_VOLTAGE_TO_CURRENT_A = 1.720f;

[[nodiscard]] float TI_TPS25_Efuse::getChannelCurrent()
{
    return this->sns_adc_channel.getVoltage() * ADC_VOLTAGE_TO_CURRENT_A;
}

void TI_TPS25_Efuse::reset()
{
    this->enable_gpio.writePin(false);
    this->enable_gpio.writePin(true);
    this->enable_gpio.writePin(false);
}

[[nodiscard]] bool TI_TPS25_Efuse::ok()
{
    return this->pgood_gpio.readPin();
}
} // namespace io
