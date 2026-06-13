#include "hw_sds.hpp"
#include "hw_gpios.hpp"
#include "main.h"

#include <cassert>

const hw::SdCard sd1(hsd1, 950, sd_present);

const hw::SdCard &hw::getSdFromHandle(const SD_HandleTypeDef *hsd)
{
    assert(hsd == &hsd1);
    return sd1;
}