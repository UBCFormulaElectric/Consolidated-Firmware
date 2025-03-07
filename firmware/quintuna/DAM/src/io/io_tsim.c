#include "io_tsim.h"
#include "hw_gpio.h"

static const TsimLightConfig *config = NULL;

void io_tsim_init(const TsimLightConfig *tsim_config)
{
    config = tsim_config;
}

void io_tsim_set_red_high(void)
{
    hw_gpio_writePin(config->tsim_red_en, true);
}

void io_tsim_set_red_low(void)
{
    hw_gpio_writePin(config->tsim_red_en, false);
}

void io_tsim_set_green_high(void)
{
    hw_gpio_writePin(config->ntsim_green_en, true);
}

void io_tsim_set_green_low(void)
{
    hw_gpio_writePin(config->ntsim_green_en, false);
}