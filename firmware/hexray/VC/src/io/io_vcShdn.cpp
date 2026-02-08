#include "io_vcShdn.hpp"
#include "hw_gpios.hpp"
#include "main.h"

bool io::vcShdn::TsmsFault_get(void)
{
    return tsms_3v3_out.readPin();
}

bool io::vcShdn::RearRightMotorInterlock_get(void)
{
    return rr_ilck_3v3_out.readPin();
}

bool io::vcShdn::MSDOrEMeterFault_get(void)
{
    return sb_shdn_3v3_out.readPin();
}

