#include "f4dev.pb.h"
#include "hw_gpio.h"
#include "hw_gpio_config.h"
#include "hw_adc.h"
#include "hw_adc_config.h"

const Gpio *id_to_gpio[] = {
    [f4dev_GpioNetName_GPIO_6] = &gpio_6,
    [f4dev_GpioNetName_GPIO_5] = &gpio_5,
};

// TODO: Configure adcs.
const AdcChannel *id_to_adc[] = { [0] = &adc_tmp };