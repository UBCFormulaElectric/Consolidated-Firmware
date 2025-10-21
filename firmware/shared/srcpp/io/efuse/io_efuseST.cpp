#include <assert.h>
#include "io_efuseST.hpp"
#include "hw_gpio.hpp"

namespace io::efuse
{
    ST_Efuse::ST_Efuse(const hw::Gpio &enable_gpio, const hw::Adc &sns_adc_channel, const hw::Gpio &stby_reset_gpio)
    : Efuse(enable_gpio, sns_adc_channel), stby_reset_gpio(stby_reset_gpio) {}

    void        ST_Efuse::setChannel(bool enabled) {
        this->enable_gpio.writePin(enabled);
    }

    const bool  ST_Efuse::isChannelEnabled() {
        return this->enable_gpio.readPin();
    }

    const float ST_Efuse::getChannelCurrent() {
        return this->sns_adc_channel.getVoltage() * ADC_VOLTAGE_TO_CURRENT_A;
    }

    void        ST_Efuse::resetSet(bool set) {
        this->stby_reset_gpio.writePin(set);
    }

    void        ST_Efuse::reset() {
        this->stby_reset_gpio.writePin(false);
        this->stby_reset_gpio.writePin(true);
        this->stby_reset_gpio.writePin(false);
    }

    // const bool  ST_Efuse::pgood() {
        
    // }

    const bool  ST_Efuse::ok() {
        
    }

}