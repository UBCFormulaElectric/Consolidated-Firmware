#include "io_efuse_ST_VND5.hpp"
#include "hw_gpio.hpp"

namespace io::efuse
{
    ST_VND5_Efuse::ST_VND5_Efuse(const hw::Gpio &enable_gpio, const hw::Adc &sns_adc_channel, const hw::Gpio &stby_reset_gpio)
    : Efuse(enable_gpio, sns_adc_channel), stby_reset_gpio(stby_reset_gpio) {}

    void        ST_VND5_Efuse::reset() 
    {
        this->stby_reset_gpio.writePin(false);
        this->stby_reset_gpio.writePin(true);
        this->stby_reset_gpio.writePin(false);
    }

    void        ST_VND5_Efuse::resetSet(const bool set)
    {
        this->stby_reset_gpio.writePin(set);
    }

    const bool  ST_VND5_Efuse::ok() 
    {
        // TODO: implement this function
        
        return true;
    }

}