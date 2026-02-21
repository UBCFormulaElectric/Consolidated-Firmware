#include "io_brakeLight.hpp"

namespace io::brakeLight
{
void set(const bool value)
{
    hw::gpio::brake_light_en.writePin(value);
}
} // namespace io::brakeLight