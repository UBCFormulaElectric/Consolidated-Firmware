#include "io_brake.hpp"
// #include "hw_adcs.hpp"
// #include "hw_gpios.hpp"
namespace io::brake
{
static float pressureFromVoltage(float voltage)
{
    return 0;
}

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
