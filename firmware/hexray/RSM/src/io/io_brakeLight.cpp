#include "io_brakeLight.hpp"
#include "hw_gpios.hpp"

namespace io::brakeLight
{
void set(const bool value)
{
    brake_light_en.writePin(value);
}
} // namespace io::brakeLight