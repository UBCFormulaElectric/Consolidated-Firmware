#include <assert.h>
#include "io_efuse_TI_TPS25.hpp"
#include "hw_gpio.hpp"

namespace io
{
void TI_TPS25_Efuse::reset()
{
    this->enable_gpio.writePin(false);
    this->enable_gpio.writePin(true);
    this->enable_gpio.writePin(false);
}

bool TI_TPS25_Efuse::pgood() const
{
    return this->pgood_gpio.readPin();
}

bool TI_TPS25_Efuse::ok()
{
    return this->pgood();
}
} // namespace io
