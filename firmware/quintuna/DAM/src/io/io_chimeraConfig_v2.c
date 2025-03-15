#include "dam.pb.h"
#include "hw_gpio.h"
#include "hw_gpios.h"
#include "hw_adc.h"
#include "hw_adcs.h"
#include "hw_i2c.h"
#include "hw_i2cs.h"

const Gpio *id_to_gpio[] = {
    [DAM_GpioNetName_GPIO_NET_NAME_UNSPECIFIED] = NULL,
    [DAM_GpioNetName_TELEM_PWR_EN]              = &telem_pwr_en_pin,
    [DAM_GpioNetName_BUZZER_PWR_EN]             = &buzzer_pwr_en_pin,
    [DAM_GpioNetName_TSIM_RED_EN]               = &tsim_red_en_pin,
    [DAM_GpioNetName_NTSIM_GRN_EN]              = &ntsim_green_en_pin,
    [DAM_GpioNetName_R_SHDN_SENSE]              = &r_shdn_sense_pin,
    [DAM_GpioNetName_L_SHDN_SENSE]              = &l_shdn_sense_pin,
    [DAM_GpioNetName_LED]                       = &led_pin,
    [DAM_GpioNetName_BOOT]                      = &boot_pin,
    [DAM_GpioNetName_SD_PRESENT]                = &sd_present,
    [DAM_GpioNetName_N_RTC_INT1]                = &n_rtc_int1_pin,
    [DAM_GpioNetName__900M]                     = &_900m_pin,
    [DAM_GpioNetName_SD_FAIL]                   = &sd_fail_pin,
};

// TODO: Configure adcs.
const AdcChannel *id_to_adc[] = { [DAM_AdcNetName_ADC_NET_NAME_UNSPECIFIED] = NULL };

// TODO: Configure I2Cs.
const I2cDevice
    *id_to_i2c[] = { [DAM_I2cNetName_I2C_NET_NAME_UNSPECIFIED] = NULL, [DAM_I2cNetName_RTC_I2C] = &rtc_i2c };   