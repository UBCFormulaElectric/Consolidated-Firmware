#include "io_suspension.h"
#include "hw_adcs.h"
#include "hw_gpios.h"

static const SuspensionConfig *config = NULL;

void io_suspension_init(const SuspensionConfig *suspension_config)
{
    config = suspension_config;
}

float io_suspension_getLeftTravel(void)
{
    return hw_adc_getVoltage(config->front_left_suspension); // Return left suspension travel based on the ADC voltage.
}

float io_suspension_getRightTravel(void)
{
    return hw_adc_getVoltage(
        config->front_right_suspension); // Return right suspension travel based on the ADC voltage.
}

bool io_suspension_leftSensorOCSC(void)
{
    return hw_gpio_readPin(config->nsusp_fl_ocsc);
}

bool io_suspension_rightSensorOCSC(void)
{
    return hw_gpio_readPin(config->nsusp_fr_ocsc);
}
