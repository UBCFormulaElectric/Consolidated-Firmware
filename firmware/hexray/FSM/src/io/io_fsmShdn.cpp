#include "io_fsmShdn.hpp"
<<<<<<< HEAD
#include "hw_gpios.hpp"

=======
// #include "hw_gpios.hpp"

// Just some dummy functions to pass the Github build checks, NEEDED TO IMPLEMENT THESE FUNCTIONS LATER
>>>>>>> master
namespace io::fsmShdn
{
bool COCKPIT_SHDN_OK_get(void)
{
<<<<<<< HEAD
    return hw::gpios::cockpit_shdn_3v3.readPin();
=======
    return false;
>>>>>>> master
}

bool BOTS_SHDN_OK_get(void)
{
<<<<<<< HEAD
    return hw::gpios::bots_3v3.readPin();
=======
    return false;
>>>>>>> master
}

bool FL_SHDN_OK_get(void)
{
<<<<<<< HEAD
    return hw::gpios::fl_int_3v3.readPin();
=======
    return false;
>>>>>>> master
}

bool FR_SHDN_OK_get(void)
{
<<<<<<< HEAD
    return hw::gpios::fr_int_3v3.readPin();
}
} // namespace io::fsmShdn
=======
    return false;
}
} // namespace io::fsmShdn
>>>>>>> master
