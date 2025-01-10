#include "hw_sd.h"
#include "main.h"
#include "hw_gpios.h"

SdCard sd1 = { .hsd = &hsd1, .timeout = 0x20, .present_gpio = &present_gpio };