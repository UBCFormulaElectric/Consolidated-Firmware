extern "C"
{
    #include "app_utils.h"
}

#include "io_efuse_ST_VND5.hpp"
#include "hw_gpio.hpp"

namespace io
{
static constexpr float NOMINAL_V = 3.0f;
// Vsenseh upper threshold
static constexpr float V_SENSE_H_H = 9.5f;
// Vsenseh lower threshold
static constexpr float V_SENSE_H_L = 7.5f;

ST_VND5_Efuse::ST_VND5_Efuse(
    const hw::Gpio &enable_gpio,
    const hw::Adc  &sns_adc_channel,
    const hw::Gpio &stby_reset_gpio)
  : Efuse(enable_gpio, sns_adc_channel), stby_reset_gpio(stby_reset_gpio)
{
}

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
        // TODO: WTF is output???
        const bool  input = this->enable_gpio.readPin();
        const bool  fault_reset_stby = this->stby_reset_gpio.readPin();
        const float voltage         = this->sns_adc_channel.getVoltage() / ADC_VOLTAGE_TO_CURRENT_A;
        
        this->faults.flags.overload = (input && (voltage > NOMINAL_V));
        this->faults.flags.ovt_stg = (input && (IS_IN_RANGE(V_SENSE_H_L, V_SENSE_H_H, voltage)));
        this->faults.flags.under_voltage = (voltage <= 0.0f);
        
        if (fault_reset_stby)
            this->faults.flags.short_to_vbat = (!input && IS_IN_RANGE(V_SENSE_H_L, V_SENSE_H_H, voltage));
        else
            this->faults.flags.short_to_vbat = (input && (voltage < NOMINAL_V));

        if (fault_reset_stby)
            this->faults.flags.open_load_off_stat = (!input && IS_IN_RANGE(V_SENSE_H_L, V_SENSE_H_H, voltage));
        else
            this->faults.flags.open_load_off_stat = (input && voltage <= 0.0f);

        this->faults.flags.negative_output_voltage_clamp = (!input && voltage <= 0.0f);

        return true;
    }
}