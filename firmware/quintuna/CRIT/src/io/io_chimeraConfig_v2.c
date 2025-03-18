#include "io_chimeraConfig_v2.h"
#include "crit.pb.h"
#include "hw_gpio.h"
#include "hw_i2c.h"
#include "hw_adc.h"
#include "hw_gpios.h"
#include "hw_spis.h"

const Gpio *id_to_gpio[] = { [crit_GpioNetName_BOOT]                 = &boot,
                             [crit_GpioNetName_LED]                  = &led,
                             [crit_GpioNetName_TELEM_SIG]            = &telem_sig,
                             [crit_GpioNetName_PUSH_DRIVE_SIG]       = &push_drive_sig,
                             [crit_GpioNetName_ROT_S]                = &rot_s,
                             [crit_GpioNetName_ROT_B]                = &rot_b,
                             [crit_GpioNetName_ROT_A]                = &rot_a,
                             [crit_GpioNetName_LAUNCH_CONTROL_SIG]   = &launch_control_sig,
                             [crit_GpioNetName_TORQUE_VECTORING_SIG] = &torque_vectoring_sig,
                             [crit_GpioNetName_REGEN_SIG]            = &regen_sig,
                             [crit_GpioNetName_LED_RCK]              = &led_rck,
                             [crit_GpioNetName_SEVEN_SEG_RCK]        = &seven_seg_rck };

// TODO: Configure adcs.
const AdcChannel *id_to_adc[] = { [crit_AdcNetName_ADC_NET_NAME_UNSPECIFIED] = NULL };

// TODO: Configure I2Cs.
const I2cDevice *id_to_i2c[] = { [crit_I2cNetName_I2C_NET_NAME_UNSPECIFIED] = NULL };

// TODO: Configure SPIs.
const SpiDevice *id_to_spi[] = { [crit_SpiNetName_SPI_NET_NAME_LED_SPI]       = &led_spi,
                                 [crit_SpiNetName_SPI_NET_NAME_SEVEN_SEG_SPI] = &seven_seg_spi };