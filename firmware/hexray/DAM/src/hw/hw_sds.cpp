#include "hw_sd.hpp"
#include "hw_gpios.hpp"

extern "C"
{
#include "main.h"
}

hw::SdCard sd1(&hsd1, 2000, &sd_present);
