#include "io_bspdTest.h"
#include "hw_gpios.h"

void io_bspdTest_enable(const bool enable)
{
    hw_gpio_writePin(&bspd_test_enable_pin, enable);
}

bool io_bspdTest_isCurrentThresholdExceeded(void)
{
    return !hw_gpio_readPin(&n_high_current_bspd_pin);
}

bool io_bspdTest_isBrakePressureThresholdExceeded(void)
{
    return !hw_gpio_readPin(&n_brake_press_3v3_pin);
}
