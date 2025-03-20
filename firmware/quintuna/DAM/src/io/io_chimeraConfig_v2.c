#include "dam.pb.h"
#include "hw_gpio.h"
#include "hw_gpios.h"
#include "hw_adc.h"
#include "hw_adcs.h"
#include "hw_i2c.h"
#include "hw_i2cs.h"
#include "hw_spi.h"
#include "hw_spis.h"

const Gpio *id_to_gpio[] = {
    [dam_GpioNetName_TELEM_PWR_EN]  = &telem_pwr_en_pin,
    [dam_GpioNetName_BUZZER_PWR_EN] = &buzzer_pwr_en_pin,
    [dam_GpioNetName_TSIM_RED_EN]   = &tsim_red_en_pin,
    [dam_GpioNetName_NTSIM_GRN_EN]  = &ntsim_green_en_pin,
    [dam_GpioNetName_R_SHDN_SENSE]  = &r_shdn_sense_pin,
    [dam_GpioNetName_L_SHDN_SENSE]  = &l_shdn_sense_pin,
    [dam_GpioNetName_LED]           = &led_pin,
    [dam_GpioNetName_BOOT]          = &boot_pin,
    [dam_GpioNetName_SD_PRESENT]    = &sd_present,
    [dam_GpioNetName_N_RTC_INT1]    = &n_rtc_int1_pin,
    [dam_GpioNetName__900M]         = &_900m_pin,
    [dam_GpioNetName_SD_FAIL]       = &sd_fail_pin,
};

// TODO: Configure adcs.
const AdcChannel *id_to_adc[] = { [dam_AdcNetName_ADC_NET_NAME_UNSPECIFIED] = NULL };

const I2cDevice *id_to_i2c[] = { [dam_I2cNetName_RTC_I2C] = &rtc_i2c };

// TODO: Configure SPIs.
const SpiDevice *id_to_spi[] = { [dam_SpiNetName_SPI_NET_NAME_UNSPECIFIED] = NULL };