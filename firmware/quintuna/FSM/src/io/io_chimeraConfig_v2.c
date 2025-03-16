#include "io_chimeraConfig_v2.h"
#include "fsm.pb.h"
#include "hw_gpios.h"
#include "hw_adcs.h"
#include "hw_i2cs.h"

const Gpio *id_to_gpio[] = {
    [fsm_GpioNetName_BOTS_3v3] = &bots_3v3,           [fsm_GpioNetName_COCKPIT_SHDN_3v3] = &cockpit_shdn_3v3,
    [fsm_GpioNetName_IMU_INT1] = &imu_int1,           [fsm_GpioNetName_IMU_INT2] = &imu_int2,
    [fsm_GpioNetName_NSUSP_FL_OCSC] = &nsusp_fl_ocsc, [fsm_GpioNetName_NSUSP_FR_OCSC] = &nsusp_fr_ocsc,
    [fsm_GpioNetName_FR_INT_3v3] = &fr_int_3v3,       [fsm_GpioNetName_FL_INT_3v3] = &fl_int_3v3,
    [fsm_GpioNetName_DEBUG_LED] = &debug_led,         [fsm_GpioNetName_NSTR_ANGLE_OCSC] = &nstr_angle_ocsc,
    [fsm_GpioNetName_NBPS_F_OCSC] = &nbps_f_ocsc,     [fsm_GpioNetName_BOOT_LED] = &boot_led
};

const AdcChannel *id_to_adc[] = {
    [fsm_AdcNetName_SUSP_FL] = &susp_fl, [fsm_AdcNetName_SUSP_FR] = &susp_fr,     [fsm_AdcNetName_APPS2] = &apps2,
    [fsm_AdcNetName_BPS_F] = &bps_f,     [fsm_AdcNetName_STR_ANGLE] = &str_angle, [fsm_AdcNetName_APPS1] = &apps1
};

const I2cDevice *id_to_i2c[] = { [fsm_I2cNetName_IMU_I2C] = &imu_i2c };