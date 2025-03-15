#include "rsm.pb.h"
#include "hw_gpios.h"
#include "hw_adcs.h"
#include "hw_i2cs.h"

const Gpio *id_to_gpio[] = { [rsm_GpioNetName_LED]                 = &led_pin,
                             [rsm_GpioNetName_BRAKE_LIGHT_EN]      = &brake_light_en_pin,
                             [rsm_GpioNetName_RL_INT]              = &rl_int_pin,
                             [rsm_GpioNetName_IMU_INT1]            = &imu_int1_pin,
                             [rsm_GpioNetName_IMU_INT2]            = &imu_int2_pin,
                             [rsm_GpioNetName_BOOT]                = &boot_pin,
                             [rsm_GpioNetName_SUSP_TRAVEL_RR_OCSC] = &susp_travel_rr_ocsc_pin,
                             [rsm_GpioNetName_SUSP_TRAVEL_RL_OCSC] = &susp_travel_rl_ocsc_pin };

// TODO: Configure adcs.
const AdcChannel *id_to_adc[] = { [rsm_AdcNetName_LC3_OUT]            = &lc3_out,
                                  [rsm_AdcNetName_SUSP_TRAVEL_RL_3V3] = &susp_travel_rl_3v3,
                                  [rsm_AdcNetName_SUSP_TRAVEL_RR_3V3] = &susp_travel_rr_3v3,
                                  [rsm_AdcNetName_BPS_3V3]            = &bps_3v3 };

// TODO: Configure I2Cs.
const I2cDevice *id_to_i2c[] = { 
    [rsm_I2cNetName_IMU_I2C] = &imu_i2c, 
    [rsm_I2cNetName_R_PUMP_I2C] = &r_pump_i2c};