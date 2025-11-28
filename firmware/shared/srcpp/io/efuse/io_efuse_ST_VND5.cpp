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
static constexpr float V_THRES     = 0.1f;

static constexpr uint8_t ST_VND5_Efuse_FAULT_FLAGS = 0x3F;

// {fault_reset_stby, channel_enabled}
static constexpr uint8_t L_L = 0x00U;
static constexpr uint8_t L_H = 0x01U;
static constexpr uint8_t H_L = 0x02U;
static constexpr uint8_t H_H = 0x03U;

ST_VND5_Efuse::ST_VND5_Efuse(
    const hw::Gpio &enable_gpio,
    const hw::Adc  &sns_adc_channel,
    const hw::Gpio &stby_reset_gpio)
  : Efuse(enable_gpio, sns_adc_channel), stby_reset_gpio(stby_reset_gpio), faults{}
{
}

void ST_VND5_Efuse::reset()
{
    this->stby_reset_gpio.writePin(false);
    this->stby_reset_gpio.writePin(true);
    this->stby_reset_gpio.writePin(false);
}

void ST_VND5_Efuse::resetSet(const bool set)
{
    this->stby_reset_gpio.writePin(set);
}

// TODO: Do we want to return a boolean or should we just return the faults union and add additional methods to check
// specific faults
const bool ST_VND5_Efuse::ok()
{
    const float   voltage          = this->getChannelCurrent() / ADC_VOLTAGE_TO_CURRENT_A;
    const bool    channel_enabled  = this->isChannelEnabled();
    const bool    fault_reset_stby = this->stby_reset_gpio.readPin();
    const uint8_t fault_table_idx =
        static_cast<uint8_t>((static_cast<uint8_t>(fault_reset_stby) << 1U) | static_cast<uint8_t>(channel_enabled));

    // Setting faults for st_vnd5 efuse
    // TODO: verify these
    switch (fault_table_idx)
    {
        case L_L:
            this->faults.flags.overload                      = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
            this->faults.flags.ovt_stp                       = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
            this->faults.flags.short_to_vbat                 = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
            this->faults.flags.open_load_off_stat            = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
            this->faults.flags.negative_output_voltage_clamp = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
            break;
        case L_H:
            this->faults.flags.overload           = APPROX_EQUAL_FLOAT(voltage, NOMINAL_V, V_THRES);
            this->faults.flags.ovt_stp            = IS_IN_RANGE(V_SENSE_H_L, V_SENSE_H_H, voltage);
            this->faults.flags.short_to_vbat      = APPROX_EQUAL_FLOAT(voltage, NOMINAL_V, V_THRES);
            this->faults.flags.open_load_off_stat = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
            break;
        case H_L:
            this->faults.flags.overload                      = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
            this->faults.flags.ovt_stp                       = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
            this->faults.flags.short_to_vbat                 = IS_IN_RANGE(V_SENSE_H_L, V_SENSE_H_H, voltage);
            this->faults.flags.open_load_off_stat            = IS_IN_RANGE(V_SENSE_H_L, V_SENSE_H_H, voltage);
            this->faults.flags.negative_output_voltage_clamp = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
            break;
        case H_H:
            this->faults.flags.overload           = APPROX_EQUAL_FLOAT(voltage, NOMINAL_V, V_THRES);
            this->faults.flags.ovt_stp            = IS_IN_RANGE(V_SENSE_H_L, V_SENSE_H_H, voltage);
            this->faults.flags.short_to_vbat      = APPROX_EQUAL_FLOAT(voltage, NOMINAL_V, V_THRES);
            this->faults.flags.open_load_off_stat = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);
            break;
        default:
            break;
    }

    this->faults.flags.under_voltage = APPROX_EQUAL_FLOAT(voltage, 0.0f, V_THRES);

    const uint8_t flags = this->faults.raw & ST_VND5_Efuse_FAULT_FLAGS;

    return !(flags > 0U);
}
} // namespace io
