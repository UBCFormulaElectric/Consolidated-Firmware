#pragma once
#include "hw_gpio.h"
#include "hw_adc.h"
#include "hw_i2c.h"
#include "hw_spi.h"

extern const Gpio       *id_to_gpio[];
extern const AdcChannel *id_to_adc[];
extern const I2cDevice  *id_to_i2c[];
extern const SpiDevice *id_to_spi[];