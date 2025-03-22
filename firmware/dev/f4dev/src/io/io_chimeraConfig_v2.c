#include "f4dev.pb.h"
#include "hw_gpio.h"
#include "hw_gpios.h"
#include "hw_adc.h"
#include "hw_adcs.h"
#include "hw_i2c.h"
#include "hw_i2cs.h"
#include "hw_spi.h"
#include "hw_spis.h"
const Gpio *id_to_gpio[] = {
    [f4dev_GpioNetName_GPIO_6] = &gpio_6,
    [f4dev_GpioNetName_GPIO_5] = &gpio_5,
};

// TODO: Configure adcs.
const AdcChannel *id_to_adc[] = { [f4dev_AdcNetName_ADC_NET_NAME_UNSPECIFIED] = NULL };

// TODO: Configure I2Cs.
const I2cDevice *id_to_i2c[] = { [f4dev_I2cNetName_I2C_NET_NAME_UNSPECIFIED] = NULL };

// TODO: Configure SPIs.
const SpiDevice *id_to_spi[] = { [f4dev_SpiNetName_SPI_NET_NAME_UNSPECIFIED] = NULL };