#include "hw_sd.hpp"
#include "hw_gpios.hpp"

extern "C"
{
#include "main.h"
}

hw::SdCard sd1 = { .hsd = &hsd1, .timeout = 2000, .present_gpio = &sd_present };