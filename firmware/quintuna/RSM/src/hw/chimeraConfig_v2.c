#include "rsm.pb.h"
#include "shared.pb.h"
#include "hw/chimeraConfig_v2.h"

#include "hw/gpios.h"
#include "hw/i2cs.h"
#include "hw/adcs.h"

const Gpio *id_to_gpio[] = { [rsm_GpioNetName_GPIO_LED]                 = &led_pin,
                             [rsm_GpioNetName_GPIO_BRAKE_LIGHT_EN]      = &brake_light_en_pin,
                             [rsm_GpioNetName_GPIO_RL_INT]              = &rl_int_pin,
                             [rsm_GpioNetName_GPIO_IMU_INT1]            = &imu_int1_pin,
                             [rsm_GpioNetName_GPIO_IMU_INT2]            = &imu_int2_pin,
                             [rsm_GpioNetName_GPIO_BOOT]                = &boot_pin,
                             [rsm_GpioNetName_GPIO_SUSP_TRAVEL_RR_OCSC] = &susp_travel_rr_ocsc_pin,
                             [rsm_GpioNetName_GPIO_SUSP_TRAVEL_RL_OCSC] = &susp_travel_rl_ocsc_pin };

const AdcChannel *id_to_adc[] = { [rsm_AdcNetName_ADC_LC3_OUT]            = &lc3_out,
                                  [rsm_AdcNetName_ADC_SUSP_TRAVEL_RL_3V3] = &susp_travel_rl_3v3,
                                  [rsm_AdcNetName_ADC_SUSP_TRAVEL_RR_3V3] = &susp_travel_rr_3v3,
                                  [rsm_AdcNetName_ADC_BPS_3V3]            = &bps_3v3 };

const I2cDevice
    *id_to_i2c[] = { [rsm_I2cNetName_I2C_IMU_I2C] = &imu_i2c, [rsm_I2cNetName_I2C_R_PUMP_I2C] = &r_pump_i2c };

hw_chimera_v2_Config chimera_v2_config = { .gpio_net_name_tag = GpioNetName_rsm_net_name_tag,
                                           .id_to_gpio        = id_to_gpio,
                                           .adc_net_name_tag  = AdcNetName_rsm_net_name_tag,
                                           .id_to_adc         = id_to_adc,
                                           .i2c_net_name_tag  = I2cNetName_rsm_net_name_tag,
                                           .id_to_i2c         = id_to_i2c };
