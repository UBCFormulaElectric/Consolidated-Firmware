#include "ssm.pb.h"
#include "hw_gpio.h"
#include "hw_gpios.h"
#include "hw_adc.h"
#include "hw_adcs.h"
#include "hw_i2c.h"
#include "hw_i2cs.h"
#include "hw_spi.h"
#include "hw_spis.h"

const Gpio *id_to_gpio[] = {
    [ssm_GpioNetName_GPIO_INDICATOR_1] = &indicator_1, [ssm_GpioNetName_GPIO_INDICATOR_2] = &indicator_2,
    [ssm_GpioNetName_GPIO_INDICATOR_3] = &indicator_3, [ssm_GpioNetName_GPIO_DOUT_4] = &dout_4,
    [ssm_GpioNetName_GPIO_DEBUG_LED] = &debug_led,
};

// TODO: Configure adcs.
const AdcChannel *id_to_adc[] = { [ssm_AdcNetName_ADC_NET_NAME_UNSPECIFIED] = NULL };

// TODO: Configure I2Cs.
const I2cDevice *id_to_i2c[] = { [ssm_I2cNetName_I2C_NET_NAME_UNSPECIFIED] = NULL };

// TODO: Configure SPIs.
const I2cDevice *id_to_spi[] = { [ssm_SpiNetName_SPI_NET_NAME_UNSPECIFIED] = NULL };