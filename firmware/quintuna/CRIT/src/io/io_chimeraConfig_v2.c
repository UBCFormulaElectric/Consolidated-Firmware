#include "crit.pb.h"
#include "hw_gpio.h"
#include "hw_gpios.h"
#include "hw_adc.h"
#include "hw_i2c.h"

const Gpio *id_to_gpio[] = { [crit_GpioNetName_GPIO_NET_NAME_UNSPECIFIED] = NULL };

// TODO: Configure adcs.
const AdcChannel *id_to_adc[] = { [crit_GpioNetName_GPIO_NET_NAME_UNSPECIFIED] = NULL };

// TODO: Configure I2Cs.
const I2cDevice *id_to_i2c[] = { [crit_GpioNetName_GPIO_NET_NAME_UNSPECIFIED] = NULL };