#pragma once
#include "hw_chimera_v2.h"

extern const Gpio       *id_to_gpio[];
extern const AdcChannel *id_to_adc[];
extern const I2cDevice  *id_to_i2c[];

// Exposed Chimera V2 configs.
extern hw_chimera_v2_Config chimera_v2_config;