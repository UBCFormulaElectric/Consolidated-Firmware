#pragma once
#include "io_switch.h"
#include "hw_gpios.h"

namespace io::switches
{
const static io::Switch start_switch{ hw::gpio::start_switch_gpio };
const static io::Switch regen_switch{ hw::gpio::regen_switch_gpio };
const static io::Switch torquevec_switch{ hw::gpio::torquevec_switch_gpio };
} // namespace io::switches
