#include <assert.h>
#include "io_efuseTI.hpp"
#include "hw_gpio.hpp"

namespace io::efuse
{
    TI_Efuse::TI_Efuse(const hw::Gpio &enable_gpio, const hw::Adc &sns_adc_channel, const hw::Gpio& pgood)
    : Efuse(enable_gpio, sns_adc_channel) , pgood_gpio(pgood) {}

    void        TI_Efuse::setChannel(bool enabled) {
        this->enable_gpio.writePin(enabled);
    }

    const bool  TI_Efuse::isChannelEnabled() {
        return this->enable_gpio.readPin();
    }

    const float TI_Efuse::getChannelCurrent() {
        return this->sns_adc_channel.getVoltage() * ADC_VOLTAGE_TO_CURRENT_A;
    }

    // void        TI_Efuse::resetSet(bool set) {
        
    // }

    void        TI_Efuse::reset() {
        this->enable_gpio.writePin(false);
        this->enable_gpio.writePin(true);
        this->enable_gpio.writePin(false);
    }

    const bool  TI_Efuse::pgood() {
        return this->pgood_gpio.readPin();
    }

    const bool  TI_Efuse::ok() {
        return this->pgood();
    }
}