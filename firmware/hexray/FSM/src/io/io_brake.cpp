#include "io_brake.hpp"
// #include "hw_adcs.hpp"
// #include "hw_gpios.hpp"

// Just some dummy functions to pass the Github build checks, NEEDED TO IMPLEMENT THESE FUNCTIONS LATER
namespace io::brake
{
bool isActuated(void)
{
    return false;
}

float getFrontPressurePsi(void)
{
    return 0;
}

bool OCSC(void)
{
    return false;
}
} // namespace io::brake
