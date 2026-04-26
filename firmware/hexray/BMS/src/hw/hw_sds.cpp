#include "hw_sds.hpp"

#include "main.h"
#include "hw_gpios.hpp"

hw::SdCard sd1(&hsd1, 1000U, sd_cd);

namespace hw
{
const SdCard &getSdFromHandle(SD_HandleTypeDef *hsd)
{
    if (hsd == sd1.getHsd())
    {
        return sd1;
    }

    Error_Handler();
    return sd1;
}
} // namespace hw
