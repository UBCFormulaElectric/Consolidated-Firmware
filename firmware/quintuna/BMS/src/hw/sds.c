#include "hw/sd.h"
#include "main.h"
#include "hw/gpios.h"

SdCard sd1 = { .hsd = &hsd1, .timeout = 1000, .present_gpio = &sd_cd_pin };
