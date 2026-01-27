#include "io_suspension.hpp"
// #include "hw_adcs.hpp"
// #include "hw_gpios.hpp"

// Just some dummy functions to pass the Github build checks, NEEDED TO IMPLEMENT THESE FUNCTIONS LATER
namespace io::suspension
{
float getLeftTravel(void)
{
    // Return left suspension travel based on the ADC voltage.
    return 0;
}

float getRightTravel(void)
{
    // Return right suspension travel based on the ADC voltage.
    return 0;
}

bool leftSensorOCSC(void)
{
    return false;
}

bool rightSensorOCSC(void)
{
    return false;
}
} // namespace io::suspension
