#include "io_suspension.hpp"
<<<<<<< HEAD
#include "hw_adcs.hpp"
#include "hw_gpios.hpp"

=======
// #include "hw_adcs.hpp"
// #include "hw_gpios.hpp"

// Just some dummy functions to pass the Github build checks, NEEDED TO IMPLEMENT THESE FUNCTIONS LATER
>>>>>>> master
namespace io::suspension
{
float getLeftTravel(void)
{
    // Return left suspension travel based on the ADC voltage.
<<<<<<< HEAD
    return hw::adcs::susp_fl.getVoltage();
=======
    return 0;
>>>>>>> master
}

float getRightTravel(void)
{
    // Return right suspension travel based on the ADC voltage.
<<<<<<< HEAD
    return hw::adcs::susp_fr.getVoltage();
=======
    return 0;
>>>>>>> master
}

bool leftSensorOCSC(void)
{
<<<<<<< HEAD
    return !hw::gpios::nsusp_fl_ocsc.readPin();
=======
    return false;
>>>>>>> master
}

bool rightSensorOCSC(void)
{
<<<<<<< HEAD
    return !hw::gpios::nsusp_fr_ocsc.readPin();
}
} // namespace io::suspension
=======
    return false;
}
} // namespace io::suspension
>>>>>>> master
