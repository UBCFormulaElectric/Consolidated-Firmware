#include "dam.pb.h"
#include "shared.pb.h"
#include "hw/chimeraConfig_v2.h"

#include "hw/gpios.h"
#include "hw/i2cs.h"

const Gpio *id_to_gpio[] = {
    [dam_GpioNetName_GPIO_TELEM_PWR_EN]  = &telem_pwr_en_pin,
    [dam_GpioNetName_GPIO_BUZZER_PWR_EN] = &buzzer_pwr_en_pin,
    [dam_GpioNetName_GPIO_TSIM_RED_EN]   = &tsim_red_en_pin,
    [dam_GpioNetName_GPIO_NTSIM_GRN_EN]  = &ntsim_green_en_pin,
    [dam_GpioNetName_GPIO_R_SHDN_SENSE]  = &r_shdn_sense_pin,
    [dam_GpioNetName_GPIO_L_SHDN_SENSE]  = &l_shdn_sense_pin,
    [dam_GpioNetName_GPIO_LED]           = &led_pin,
    [dam_GpioNetName_GPIO_BOOT]          = &boot_pin,
    [dam_GpioNetName_GPIO_SD_PRESENT]    = &sd_present,
    [dam_GpioNetName_GPIO_N_RTC_INT1]    = &n_rtc_int1_pin,
    [dam_GpioNetName_GPIO_900M]          = &_900m_pin,
    [dam_GpioNetName_GPIO_SD_FAIL]       = &sd_fail_pin,
};

const I2cDevice *id_to_i2c[] = { [dam_I2cNetName_I2C_RTC] = &rtc_i2c };

hw_chimera_v2_Config chimera_v2_config = { .gpio_net_name_tag = GpioNetName_dam_net_name_tag,
                                           .id_to_gpio        = id_to_gpio,
                                           .i2c_net_name_tag  = I2cNetName_dam_net_name_tag,
                                           .id_to_i2c         = id_to_i2c };