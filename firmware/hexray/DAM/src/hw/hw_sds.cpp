#include "hw_sds.hpp"
#include "hw_gpios.hpp"
#include "main.h"

const hw::SdCard sd1(&hsd1, 2000, sd_present);
