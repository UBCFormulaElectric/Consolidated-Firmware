#include "io_bspdTest.h"
#include "hw_gpios.h"

void io_bspdTest_enable(const bool enable)
{
    hw_gpio_writePin(&test_enable_gpio, enable);
}

bool io_bspdTest_isCurrentThresholdExceeded(void)
{
    return !hw_gpio_readPin(&n_high_current_gpio);
}