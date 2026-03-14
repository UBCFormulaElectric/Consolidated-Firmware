#include "io_suspension.hpp"
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"

namespace io::suspension
{
float getLeftTravel()
{
    // Return left suspension travel based on the ADC voltage.
    return hw::adcs::susp_fl.getVoltage();
}

float getRightTravel()
{
    // Return right suspension travel based on the ADC voltage.
    return hw::adcs::susp_fr.getVoltage();
}

bool leftSensorOCSC()
{
    return !nsusp_fl_ocsc.readPin();
}

bool rightSensorOCSC()
{
    return !nsusp_fr_ocsc.readPin();
}
} // namespace io::suspension
