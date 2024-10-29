#include "io_loadCell.h"
#include "hw_adc.h"
#include "hw_adcs.h"

// TODO: Find actual max and min values
#define LOADCELL_MINVOLT (0.0f)
#define LOADCELL_MAXVOLT (0.0f)

namespace io::loadCell
{

float loadCell_voltageToMechancialLoad(float voltage)
{
    // TODO: find transfer function (no spec sheet so far just this amazon link:
    // https://caltsensor.com/product/s-type-load-cells-dyly-103/
    return 0.0;
}

bool loadCell_OCSC(Adc adcPin_toCheck)
{
    float voltage = hw::adc::adcPin_toCheck.getVoltage();
    return !(LOADCELL_MINVOLT <= voltage && voltage <= LOADCELL_MAXVOLT);
}

float io_loadCell_getMechanicalLoad3(void)
{
    return loadCell_voltageToMechanical(hw::adc::lc3_out.getVoltage());
}

float io_loadCell_getMechanicalLoad4(void)
{
    return loadCell_voltageToMechancialLoad(hw::adc::regen_3v3_lc3_out.getVoltage());
}

bool io_loadCell_sensor3OCSC(void)
{
    return loadCell_OCSC(hw::adc::lc3_out);
}

bool io_loadCell_sensor4OCSC(void)
{
    return loadCell_OCSC(hw::adc::regen_3v3_lc3_out);
}

}

} // namespace io::loadCell