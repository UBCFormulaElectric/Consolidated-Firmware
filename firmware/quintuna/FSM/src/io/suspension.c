#include "io/suspension.h"
#include "hw/adcs.h"
#include "hw/gpios.h"

float io_suspension_getLeftTravel(void)
{
    // Return left suspension travel based on the ADC voltage.
    return hw_adc_getVoltage(&susp_fl);
}

float io_suspension_getRightTravel(void)
{
    // Return right suspension travel based on the ADC voltage.
    return hw_adc_getVoltage(&susp_fr);
}

bool io_suspension_leftSensorOCSC(void)
{
    return !hw_gpio_readPin(&nsusp_fl_ocsc);
}

bool io_suspension_rightSensorOCSC(void)
{
    return !hw_gpio_readPin(&nsusp_fr_ocsc);
}
