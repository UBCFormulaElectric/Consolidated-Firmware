#include "io_suspension.hpp"
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"

namespace io::suspension
{
float getLeftTravel(void)
{
    // Return left suspension travel based on the ADC voltage.
    return hw::adcs::susp_fl.getVoltage();
}

float getRightTravel(void)
{
    // Return right suspension travel based on the ADC voltage.
    return hw::adcs::susp_fr.getVoltage();
}

bool leftSensorOCSC(void)
{
    return !hw::gpios::nsusp_fl_ocsc.readPin();
}

bool rightSensorOCSC(void)
{
    return !hw::gpios::nsusp_fr_ocsc.readPin();
}
} // namespace io::suspension
