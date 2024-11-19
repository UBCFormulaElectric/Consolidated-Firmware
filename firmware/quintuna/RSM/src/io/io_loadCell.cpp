#include "io_loadCell.h"
#include "hw_adc.h"
#include "hw_adcs.h"

// TODO: Find actual max and min values
#define LOADCELL_MINVOLT (0.0f)
#define LOADCELL_MAXVOLT (0.0f)

inline static bool OCSC(const adcPin_toCheck)
{
    const float voltage = hw::adc::adcPin_toCheck.getVoltage();
    return !(LOADCELL_MINVOLT <= voltage && voltage <= LOADCELL_MAXVOLT);
}

inline static float voltageToMechancialLoad(float voltage)
{
    // TODO: find transfer function (no spec sheet so far just this amazon link:
    // https://caltsensor.com/product/s-type-load-cells-dyly-103/
    return 0.0;
}

namespace io::loadCell
{

float getMechanicalLoad3(void)
{
    return voltageToMechanical(hw::adc::lc3_out.getVoltage());
}

float getMechanicalLoad4(void)
{
    return voltageToMechancialLoad(hw::adc::regen_3v3_lc3_out.getVoltage());
}

bool sensor3OCSC(void)
{
    return OCSC(hw::adc::lc3_out);
}

bool sensor4OCSC(void)
{
    return OCSC(hw::adc::regen_3v3_lc3_out);
}

} // namespace io::loadCell

// namespace io::loadCell