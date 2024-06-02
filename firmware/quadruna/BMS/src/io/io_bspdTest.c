#include "io_bspdTest.h"

static const BSPDTestConfig *config = NULL;

void io_bspdTest_init(const BSPDTestConfig *bspd_test_config)
{
    config = bspd_test_config;
}

void io_bspdTest_enable(bool enable)
{
    hw_gpio_writePin(&config->test_enable_gpio, enable);
}

bool io_bspdTest_isCurrentThresholdExceeded(void)
{
    return !hw_gpio_readPin(&config->n_high_current_gpio);
}