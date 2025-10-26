#include <assert.h>
#include "io_efuse_TI_TPS2.hpp"
#include "hw_gpio.hpp"

namespace io
{
    ST_TPS2_Efuse::ST_TPS2_Efuse(const hw::Gpio &enable_gpio, const hw::Adc &sns_adc_channel, const hw::Gpio& pgood)
    : Efuse(enable_gpio, sns_adc_channel) , pgood_gpio(pgood) {}

    void        ST_TPS2_Efuse::reset() {
        this->enable_gpio.writePin(false);
        this->enable_gpio.writePin(true);
        this->enable_gpio.writePin(false);
    }

    const bool  ST_TPS2_Efuse::pgood() {
        return this->pgood_gpio.readPin();
    }

    const bool  ST_TPS2_Efuse::ok() {
        return this->pgood();
    }
}