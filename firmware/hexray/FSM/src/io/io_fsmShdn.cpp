#include "io_fsmShdn.hpp"
#include "hw_gpios.hpp"

namespace io::fsmShdn
{
bool COCKPIT_SHDN_OK_get(void)
{
    return hw::gpios::cockpit_shdn_3v3.readPin();
}

bool BOTS_SHDN_OK_get(void)
{
    return hw::gpios::bots_3v3.readPin();
}

bool FL_SHDN_OK_get(void)
{
    return hw::gpios::fl_int_3v3.readPin();
}

bool FR_SHDN_OK_get(void)
{
    return hw::gpios::fr_int_3v3.readPin();
}
} // namespace io::fsmShdn
