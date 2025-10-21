#include "hw/sd.h"
#include "main.h"
#include "hw/gpios.h"

SdCard sd1 = { .hsd = &hsd1, .timeout = 2000, .present_gpio = &sd_present };