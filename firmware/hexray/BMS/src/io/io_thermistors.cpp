#include <array>

#include "io_thermistors.hpp"
#include "hw_gpios.hpp"
#include "hw_adcs.hpp"
#include "app_thermistor.hpp"

namespace
{
constexpr float  BIAS_RESISTOR_OHM       = 10000.0f;
constexpr float  REFERENCE_VOLTAGE       = 3.3f;
constexpr size_t SIZE_OF_TEMPERATURE_LUT = 21U;

constexpr std::array<float, SIZE_OF_TEMPERATURE_LUT> lut_resistances = {
    { 32650.0f, 25390.0f, 19900.0f, 15710.0f, 12490.0f, 10000.0f, 8057.0f, 6531.0f, 5327.0f, 4369.0f, 3603.0f,
      2986.0f,  2488.0f,  2083.0f,  1752.0f,  1481.0f,  1258.0f,  1072.0f, 917.7f,  788.5f,  680.0f }
};
} // namespace

// TODO: Consider moving around LUT/thermistor code to better fit io/app logic split.
// Otherwise we are using app classes in io layer. Keep LUTs are also used for app_thermistors.cpp fo adbms.
namespace io::therm
{
constexpr app::therm::ThermistorLUT b57861s_lut(lut_resistances.data(), 0.0f, 5.0f, SIZE_OF_TEMPERATURE_LUT);

bool muxSelect(const uint8_t channel)
{
    if (channel > 7)
    {
        return false;
    }

    tsense_sel0.writePin((channel >> 0) & 0x1);
    tsense_sel1.writePin((channel >> 1) & 0x1);
    tsense_sel2.writePin((channel >> 2) & 0x1);

    return true;
}

float readSelectedTemp(void)
{
    const float raw_voltage           = hw::adc::aux_tsns.getVoltage();
    const float thermistor_resistance = (raw_voltage * BIAS_RESISTOR_OHM) / (REFERENCE_VOLTAGE - raw_voltage);

    return b57861s_lut.resistanceToTemp(thermistor_resistance);
}
} // namespace io::therm
