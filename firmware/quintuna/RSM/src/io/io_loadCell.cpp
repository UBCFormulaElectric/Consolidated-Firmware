#include "io_loadCell.h"
#include "hw_adc.h"
#include "hw_adcs.h"

// TODO: Find actual max and min values
#define LOADCELL_MINVOLT (0.0f)
#define LOADCELL_MAXVOLT (0.0f)

namespace io::loadCell
{

static const LoadCell sensor3();
static const LoadCell sensor4();

bool LoadCell::OCSC() {
    const float voltage = hw::Adc::src.getVoltage();
    return !(LOADCELL_MINVOLT <= voltage && voltage <= LOADCELL_MAXVOLT);
}

float LoadCell::mechanicalLoad() {
    return voltageToMechanical(hw::Adc::src.getVoltage());
}

float LoadCell::voltageToMechancialLoad(float voltage)
{
    // TODO: find transfer function (no spec sheet so far just this amazon link:
    // https://caltsensor.com/product/s-type-load-cells-dyly-103/
    return 0.0;
}





} // namespace io::loadCell

// namespace io::loadCell