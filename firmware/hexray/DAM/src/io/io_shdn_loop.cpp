#include "io_shdn_loop.hpp"
#include "hw_gpios.hpp"

namespace io
{
bool r_shdn_pin_is_high()
{
    return r_shdn_sense_pin.readPin();
}

bool l_shdn_pin_is_high()
{
    return l_shdn_sense_pin.readPin();
}
} // namespace io
