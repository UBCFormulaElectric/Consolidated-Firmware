// #include "fsm.pb.h"
// #include "shared.pb.h"
// #include "hw_chimeraConfig_v2.hpp"
// #include "hw_gpios.hpp"
// #include "hw_i2cs.hpp"
// #include "hw_adcs.hpp"

// const Gpio *id_to_gpio[] = {
//     [fsm_GpioNetName_GPIO_BOTS_3v3] = &bots_3v3,           [fsm_GpioNetName_GPIO_COCKPIT_SHDN_3v3] = &cockpit_shdn_3v3,
//     [fsm_GpioNetName_GPIO_IMU_INT1] = &imu_int1,           [fsm_GpioNetName_GPIO_IMU_INT2] = &imu_int2,
//     [fsm_GpioNetName_GPIO_NSUSP_FL_OCSC] = &nsusp_fl_ocsc, [fsm_GpioNetName_GPIO_NSUSP_FR_OCSC] = &nsusp_fr_ocsc,
//     [fsm_GpioNetName_GPIO_FR_INT_3v3] = &fr_int_3v3,       [fsm_GpioNetName_GPIO_FL_INT_3v3] = &fl_int_3v3,
//     [fsm_GpioNetName_GPIO_DEBUG_LED] = &debug_led,         [fsm_GpioNetName_GPIO_NSTR_ANGLE_OCSC] = &nstr_angle_ocsc,
//     [fsm_GpioNetName_GPIO_NBPS_F_OCSC] = &nbps_f_ocsc,     [fsm_GpioNetName_GPIO_BOOT_LED] = &boot_led
// };

// const AdcChannel *id_to_adc[] = { [fsm_AdcNetName_ADC_SUSP_FL] = &susp_fl,     [fsm_AdcNetName_ADC_SUSP_FR] = &susp_fr,
//                                   [fsm_AdcNetName_ADC_APPS2] = &apps2,         [fsm_AdcNetName_ADC_BPS_F] = &bps_f,
//                                   [fsm_AdcNetName_ADC_STR_ANGLE] = &str_angle, [fsm_AdcNetName_ADC_APPS1] = &apps1 };

// const I2cDevice *id_to_i2c[] = { [fsm_I2cNetName_I2C_IMU] = &imu_i2c };

// hw_chimera_v2_Config chimera_v2_config = { .gpio_net_name_tag = GpioNetName_fsm_net_name_tag,
//                                            .id_to_gpio        = id_to_gpio,
//                                            .adc_net_name_tag  = AdcNetName_fsm_net_name_tag,
//                                            .id_to_adc         = id_to_adc,
//                                            .i2c_net_name_tag  = I2cNetName_fsm_net_name_tag,
//                                            .id_to_i2c         = id_to_i2c };
